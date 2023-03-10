/********************************************************************
 CHKVEC.C  割り込みﾍﾞｸﾀｰ ﾁｪｯｶｰ  Ver 1.00   for PC-9801
 (C)opyright 1992 井上 博計

 Turbo C/C++, Quick C, LSI-C  でｺﾝﾊﾟｲﾙ可能
 但し、TC,QC,LCでINT 22,23,24h が QC,LCでINT 0 に少々の不具合が起こる
  可能性有り。ﾃﾞｰﾀﾌｧｲﾙを他のｺﾝﾊﾟｲﾗでｺﾝﾊﾟｲﾙしたものと共有するときは、
  注意した方がよい。
********************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>

#define	DATAFILENAME	"CHKVEC.DAT"	/* ﾃﾞﾌｫﾙﾄのﾃﾞｰﾀﾌｧｲﾙ名 */

#define	PARAM		0
#define	FILEOPEN	1
#define	FILEREAD	2
#define	FILEWRITE	3


void chkvec_main(char *datafile, int key_press_check);
int create_newfile(FILE *out, char *datafile);
void chk_vector(FILE *in);
void vec_error(int vec_no, long saved_vec, long cur_vec);
void fil_error(FILE *in, int no);
void verbose_datafile(char *datafile);
void error(int no);

#ifdef __TURBOC__
/* Turbo C では以下のﾍﾞｸﾀｰを起動時に保存 */
extern long _Int0Vector,_Int4Vector,_Int5Vector,_Int6Vector;
#endif /* __TURBOC__ */

int _error_exist;	/* ｴﾗｰの個数のｶｳﾝﾀｰ */

void main(int ac, char *av[])
{
	int i, key_press_check = 1;
	char datafile[125], task_no = 'c';

	strcpy(datafile, DATAFILENAME);	/* ﾃﾞｰﾀﾌｧｲﾙ名の初期化 */
	for(i=1; i<ac; i++)
	{	/* ｽｲｯﾁの解析及びﾃﾞｰﾀﾌｧｲﾙ名の取得 */
		if(av[i][0] == '-')
		{	/* ｽｲｯﾁｷｬﾗｸﾀｰ は '-' */
			switch(av[i][1]){
				case	'v'	:	/* ﾍﾞｸﾀﾃｰﾌﾞﾙの表示 */
					task_no = 'v';
					break;
				case	'k'	:	/* 何かｷｰを押して下さいのﾁｪｯｸの省略 */
					key_press_check = 0;
					break;
				default		:	/* その他のｽｲｯﾁ */
					error(PARAM);
			}
		}
		else strcpy(datafile, av[i]);	/* ｽｲｯﾁ以外はﾃﾞｰﾀﾌｧｲﾙ名と認識 */
	}

	/* 実際の仕事に分岐 */
	switch(task_no)
	{
		case	'v'		:	/* ﾍﾞｸﾀﾃｰﾌﾞﾙの表示 */
			verbose_datafile(datafile);
			break;
		case	'c'		:	/* ﾃﾞﾌｫﾙﾄではINTの検査 */
			chkvec_main(datafile, key_press_check);
			break;
	}
	exit(0);	/* 終了 */
}

/* CHKVEC の ﾍﾞｸﾀｰ CHECK ﾙｰﾁﾝ */
void chkvec_main(char *datafile, int key_press_check)
{
	FILE *in;

	if((in = fopen(datafile, "rb")) == NULL)
	{	/* ﾃﾞｰﾀﾌｧｲﾙが見つからないときは新しいファイルを作る */
		fclose(in);
		if(create_newfile(in, datafile))
			printf("割り込みﾍﾞｸﾀｰ ﾃﾞｰﾀﾌｧｲﾙ を作成しました\n");
		else printf("ﾃﾞｰﾀﾌｧｲﾙ の作成を中止しました\n");

		return;
	}

	/* 検査ルーチン */
	_error_exist = 0;	/* ｴﾗｰ個数を0個に初期化 */
	printf("割り込みﾍﾞｸﾀｰのﾁｪｯｸ中 ...\n");
	chk_vector(in);
	if(!_error_exist) printf("正常 : 書き換えられたﾍﾞｸﾀｰはありません\n");
	else
	{
		printf("\x1b[4m%d 個のﾍﾞｸﾀｰが書き換えられています\x1b[m\n",_error_exist);
		if(key_press_check)
		{
			printf("\n何かｷｰを押して下さい\n");
			getch();		/* PC-9801以外ではｺﾝﾊﾟｲﾙし直せば使用可能 */
		}
	}

	return;
}

/* 現在の割り込みﾍﾞｸﾀｰの保存 */
int create_newfile(FILE *out, char *datafile)
{
	int i;
	unsigned long vec;

	printf("新しいﾃﾞｰﾀﾌｧｲﾙ %s を作ってもいいですか  (y / n)\n", datafile);
	for(;;)
	{
		i = toupper(getch());
		if(i == 'Y') break;
		if(i == 'N') return 0;	/* 作成しない */
	}

	if((out = fopen(datafile, "wb")) == NULL) fil_error(out, FILEOPEN);

	/* 割り込みﾍﾞｸﾀｰの書き込み */
#ifdef __TURBOC__
	if(fwrite(&_Int0Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	for(i=1; i<= 3; i++)
	{
		vec = (long)getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
	if(fwrite(&_Int4Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	if(fwrite(&_Int5Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	if(fwrite(&_Int6Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	for(i=7; i<= 0xff; i++)
	{
		vec = (long)getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
#else	/* TURBO C 以外では */
	for(i=0; i<= 0xff; i++)
	{
		vec = (long)_dos_getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
#endif /* __TURBOC__ */

	fclose(out);
	return 1;	/* ﾃﾞｰﾀﾌｧｲﾙを作成した */
}

/* 割り込みﾍﾞｸﾀｰの検査 */
void chk_vector(FILE *in)
{
	int i;
	unsigned long saved_vec,cur_vec;

	/* 割り込みﾍﾞｸﾀｰの比較 */
#ifdef __TURBOC__
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int0Vector) vec_error(0, saved_vec, _Int0Vector);
	for(i=1; i<= 3; i++)
	{
		cur_vec = (long)getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int4Vector) vec_error(4, saved_vec, _Int4Vector);
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int5Vector) vec_error(5, saved_vec, _Int5Vector);
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int6Vector) vec_error(6, saved_vec, _Int6Vector);
	for(i=7; i<= 0xff; i++)
	{
		cur_vec = (long)getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
#else	/* TURBO C 以外では */
	for(i=0; i<= 0xff; i++)
	{
		cur_vec = (long)_dos_getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
#endif /* __TURBOC__ */
}

/* 変更されたﾍﾞｸﾀｰの表示 */
void vec_error(int vec_no, long saved_vec, long cur_vec)
{
	_error_exist++;		/* ｶｳﾝﾀｰをｲﾝｸﾘﾒﾝﾄ */
	printf("割り込みﾍﾞｸﾀｰ %02X 番が変更されています  %04X:%04X → %04X:%04X\n",
		   vec_no,
		   (unsigned int)((saved_vec & 0xffff0000L) >> 16),
		   (unsigned int)(saved_vec & 0xffffL),
		   (unsigned int)((cur_vec & 0xffff0000L) >> 16),
		   (unsigned int)(cur_vec & 0xffffL) );
}

/* ﾍﾞｸﾀｰ ﾃｰﾌﾞﾙ の表示 */
void verbose_datafile(char *datafile)
{
	FILE *in;
	int i;
	unsigned long vec;

	if((in = fopen(datafile, "rb")) == NULL) fil_error(in, FILEOPEN);
	printf("割り込み番号  ｾｸﾞﾒﾝﾄ:ｵﾌｾｯﾄ\n"
		   "?｢?｢?｢?｢?｢?｢?｢?｢?｢?｢?｢?｢?｢\n");
	for(i=0; i<=0xff; i++)
	{	/* 1つずつ表示 */
		if(fread(&vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		printf(" %02X             %04X:%04X\n",
			   i, (unsigned int)((vec & 0xffff0000L) >> 16),
			   (unsigned int)(vec & 0xffffL) );
	}
	fclose(in);

	return;
}

/* ﾌｧｲﾙを閉じてからエラー処理へ */
void fil_error(FILE *in, int no)
{
	fclose(in);
	error(no);
}


/* エラー処理 */
void error(int no)
{
	switch(no)
	{
		case	PARAM	:
			printf("割り込みﾍﾞｸﾀｰ ﾁｪｯｶｰ  Ver 1.00		(C) 1992 井上 博計\n\n"
				   "使用法 : CHKVEC (-vk \x1b[4mﾃﾞｰﾀﾌｧｲﾙ名\x1b[m)\n\n");
			break;
		case    FILEOPEN	:
			printf("ﾌｧｲﾙが見つからない、又は読み込めません\n\n");
			break;
		case	FILEREAD	:
			printf("ﾌｧｲﾙが読めません\n\n");
			break;
		case	FILEWRITE	:
			printf("ﾌｧｲﾙが書けません\n\n");
			break;
	}
	exit(1);	/* 異常終了 */
}
