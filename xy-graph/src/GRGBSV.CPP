/* RGB 形式でセーブ／ロード      ラージモデルでコンパイルして下さい */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>

void do_rgb_save(char *body);
void do_rgb_load(char *body);
int do_save(FILE *fi, int SEGMENT);
int do_load(FILE *fi, int SEGMENT);
void make_filename(char *filename, char *body, char *ext);

void main(int ac, char *av[])
{
	if(ac == 2) do_rgb_save(av[1]);
	if(ac == 3 && !strcmp(av[1],"-l")) do_rgb_load(av[2]);

	/* エラー表示 */
	puts("GRGBSV						(C)1992 Hirokazu Inoue\n"
		 ".R1 .G1 .B1 .E1 (ﾍﾞﾀ) 形式で画像をセーブ／ロードします。\n"
		 "セーブ : GRGBSV <filename>\n"
		 "ロード : GRGBSV -l <filename>\n"
		 "成功時は戻り値は０です。\n"
		 "戻り値が１：ﾊﾟﾗﾒｰﾀｴﾗｰ、２：ﾌｧｲﾙｴﾗｰ、３：容量不足\n");
	exit(1);	/* パラメータが１個以外 */
}

/* セーブ */
void do_rgb_save(char *body)
{
	FILE *fi;
	char filename[100];

	make_filename(filename, body, ".B1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* オープンエラー */
	if(do_save(fi,0xa800) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	make_filename(filename, body, ".R1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* オープンエラー */
	if(do_save(fi,0xb000) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	make_filename(filename, body, ".G1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* オープンエラー */
	if(do_save(fi,0xb800) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	make_filename(filename, body, ".E1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* オープンエラー */
	if(do_save(fi,0xe000) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	exit(0);
}

/* ロード */
void do_rgb_load(char *body)
{
	FILE *fi;
	char filename[100];

	make_filename(filename, body, ".B1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* オープンエラー */
	if(do_load(fi,0xa800) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	make_filename(filename, body, ".R1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* オープンエラー */
	if(do_load(fi,0xb000) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	make_filename(filename, body, ".G1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* オープンエラー */
	if(do_load(fi,0xb800) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	make_filename(filename, body, ".E1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* オープンエラー */
	if(do_load(fi,0xe000) != 0)
	{
		fclose(fi);
		exit(3);	/* 容量不足 */
	}
	fclose(fi);

	exit(0);
}

/* ファイル名のボディーに拡張子を付ける */
void make_filename(char *filename, char *body, char *ext)
{
	strcpy(filename, strtok(body,"."));
	strcat(filename,ext);
}

/* ファイルへ SEGMENT:0 を書き込む */
int do_save(FILE *fi, int SEGMENT)
{
	char *VRAM;

	VRAM = (char far *)MK_FP(SEGMENT,0);
	if(fwrite(VRAM, sizeof(char), 0x7d00, fi) != 0x7d00) return 1;

	return 0;	/* 正常 */
}

/* ファイルより SEGMENT:0 に読み込む */
int do_load(FILE *fi, int SEGMENT)
{
	char *VRAM;

	VRAM = (char far *)MK_FP(SEGMENT,0);
	if(fread(VRAM, sizeof(char), 0x7d00, fi) != 0x7d00) return 1;

	return 0;	/* 正常 */
}
