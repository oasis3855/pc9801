#include<stdio.h>
#include<dos.h>
/* MANDEL.OV2 用 */
/***********************************************************
	squeeze.c -- 簡単な動的辞書圧縮プログラム.
	ANSI C にほぼ準拠したつもりです.
	Turbo C 2.0 でテストしました.
	付属の "bitio.c" を同じディレクトリに置いてコンパイル
	してください.
	このファイルの配布・利用は自由です.
	タブは4桁単位です.
************************************************************
	奥村 晴彦
		PC-VAN:      SCIENCE
		NIFTY-Serve: PAF01022
		住所:        239 横須賀市長沢580
		             グリーンハイツ12-2-404
	Haruhiko Okumura
		CompuServe:  74050,1022
		Internet:    74050.1022@compuserve.com
		Snailmail:   12-2-404 Green Heights, Nagasawa,
		             Yokosuka 239, Japan
***********************************************************/
#include "bitio.c"
#define N        256    /* 文字の種類 (文字 = 0..N-1) */
#define MAXDICT 4096    /* 辞書サイズ 4096, 8192, ... */
#define MAXMATCH 100    /* 最大一致長 */
#define NIL  MAXDICT    /* ノード番号として存在しない値 */

static unsigned char character[MAXDICT];
static int parent[MAXDICT], lchild[MAXDICT],  /* 親, 左の子 */
           rsib[MAXDICT], lsib[MAXDICT],  /* 右左のきょうだい */
           dictsize = N;  /* 現在の辞書サイズ */
static int newer[MAXDICT], older[MAXDICT];  /* 待ち行列ポインタ */
static int qin = NIL, qout = NIL;       /* 待ち行列の入口, 出口 */
static int match[MAXMATCH];  /* 一致文字列 */
static int bitlen = 1;  /* 現在の符号語の長さ */
static int bitmax = 2;  /* 1 << bitlen */

/* ノード p を LRU 待ち行列から外す (size > 1; p は最後でない) */
void dequeue(int p)
{
	int n, o;

	if (p == qout) {  /* 先頭の場合 */
		qout = newer[p];  older[qout] = NIL;
	} else {
		o = older[p];  n = newer[p];
		newer[o] = n;  older[n] = o;
	}
}

/* ノード p を待ち行列の要素 q の後ろに挿入 (q が NIL なら最初に) */
void enqueue(int p, int q)
{
	if (qin == NIL) {  /* 待ち行列が空 */
		older[p] = newer[p] = NIL;  qin = qout = p;
	} else if (q == NIL) {  /* 待ち行列の最初に付ける */
		older[p] = NIL;  newer[p] = qout;
		qout = older[qout] = p;
	} else if (q == qin) {  /* 待ち行列の最後に付ける */
		older[p] = qin;  newer[p] = NIL;
		qin = newer[qin] = p;
	} else {  /* 待ち行列の途中に割り入る */
		older[p] = q;
		newer[p] = newer[q];
		newer[q] = older[newer[p]] = p;
	}
}

/* ノード p の文字 c に当たる子を返す (なければ NIL) */
int child(int p, int c)
{
	p = lchild[p];
	while (p != NIL && c != character[p]) p = rsib[p];
	return p;
}

/* 親ノード parp の文字 c に当たる子として葉ノード p を挿入 */
void addleaf(int parp, int p, int c)
{
	int q;

	character[p] = c;
	parent[p] = parp;
	lchild[p] = lsib[p] = NIL;
	q = lchild[parp];  rsib[p] = q;
	if (q != NIL) lsib[q] = p;
	lchild[parp] = p;
}

/* 葉ノード p を削除 */
void deleteleaf(int p)
{
	int left, right;

	left = lsib[p];  right = rsib[p];
	if (left != NIL) rsib[left] = right;
	else      lchild[parent[p]] = right;
	if (right != NIL) lsib[right] = left;
}

/* 辞書木の初期化 */
void init_tree(void)
{
	int i;

	for (i = 0; i < N; i++) {
		character[i] = i;
		parent[i] = lchild[i] = lsib[i] = rsib[i] = NIL;
	}
}

/* 木の更新 */
void update(int *match, int curlen, int prevp, int prevlen)
{
	int p, c, i;

	if (prevp == NIL) return;
	for (i = 0; i < curlen; i++) {
		if (++prevlen > MAXMATCH) return;
		c = match[i];
		if ((p = child(prevp, c)) == NIL) {
			if (dictsize < MAXDICT) p = dictsize++;  /* dictsize < NIL */
			else {
				if (prevp == qout) return;
				p = qout;  dequeue(p);  deleteleaf(p);
			}
			addleaf(prevp, p, c);
			if (prevp < N) enqueue(p, qin);
			else           enqueue(p, older[prevp]);
		}
		prevp = p;
	}
}

void output(int p)
{
	if (p < N) {
		putbit(0);  putbits(8, p);
	} else {
		while ((dictsize - N) >= bitmax) {
			bitlen++;  bitmax <<= 1;
		}
		putbit(1);  putbits(bitlen, p - N);
	}
}

int	input(void)
{
	int i;

	if ((dictsize - N) >= bitmax) {
		bitmax <<= 1;  bitlen++;
	}
	if ((i = getbit()) == EOF) return EOF;
	if (i == 0) return getbits(8);
	if ((i = getbits(bitlen)) == EOF) return EOF;
	return i + N;
}

void encode(void)  /* 圧縮 */
{
	int p, c, q, curptr, curlen, prevptr, prevlen;
	unsigned long int incount, printcount, cr;

	init_tree();  curptr = NIL;  curlen = 0;
	incount = printcount = 0;  c = getc(infile);
	while (c != EOF) {
		prevptr = curptr;  prevlen = curlen;  curlen = 0;
		q = qin;  p = c;
		do {
			if (p >= N)
				if (p == q)	q = older[p];
				else {  dequeue(p);  enqueue(p, q);  }
			match[curlen++] = c;  curptr = p;
			c = getc(infile);  p = child(curptr, c);
		} while (p != NIL);
		output(curptr);
		update(match, curlen, prevptr, prevlen);
		if ((incount += curlen) > printcount) {
			printcount += 1024;
		}
	}
	putbits(7, 0);  /* ビットバッファをフラッシュ */
	if (incount != 0) {
		cr = (1000 * outcount + incount / 2) / incount;
	}
}

void decode(unsigned long int size)  /* 復元 */
{
	int p, i, curptr, curlen, prevptr, prevlen, *base;
	unsigned long int count, printcount;

	init_tree();
	curptr = NIL;  curlen = 0;  count = printcount = 0;
	while (count < size) {
		if ((p = input()) == EOF) exit(0x1b);
		if (p >= dictsize) exit(0x1b);
		prevptr = curptr;  prevlen = curlen;
		curptr = p;  curlen = 0;
		while (p != NIL) {
			if (p >= N && p != qin) {
				dequeue(p);  enqueue(p, qin);
			}
			curlen++;
			match[MAXMATCH - curlen] = character[p];
			p = parent[p];
		}
		base = &match[MAXMATCH - curlen];
		for (i = 0; i < curlen ; i++) putc(base[i], outfile);
		update(base, curlen, prevptr, prevlen);
		if ((count += curlen) > printcount) {
			printcount += 1024;
		}
	}
}

int main(int argc, char *argv[])
{
	int c;
	unsigned long int size;  /* 元のバイト数 */
	union REGS reg;
	char _mes[] = {"Compress/Extract file : [e/d] datafile outputfile\r\n$"};

	if (argc != 4 || ((c = *argv[1]) != 'E' && c != 'e'
						   && c != 'D' && c != 'd'))
	{
		reg.x.dx = (char *)_mes;
		reg.x.ax = 0x0900;
		intdos(&reg,&reg);
		exit(0x1b);
	}
	if ((infile  = fopen(argv[2], "rb")) == NULL) exit(0x1b);
	if ((outfile = fopen(argv[3], "wb")) == NULL) exit(0x1b);
	if (c == 'E' || c == 'e') {
		fseek(infile, 0L, SEEK_END);  /* infile の末尾を探す */
		size = ftell(infile);     /* infile のバイト数 */
		fwrite(&size, sizeof size, 1, outfile);
		rewind(infile);
		encode();  /* 圧縮 */
	} else {
		fread(&size, sizeof size, 1, infile);  /* 元のバイト数 */
		decode(size);  /* 復元 */
	}
	fclose(infile);  fclose(outfile);
	return EXIT_SUCCESS;
}
