/*		SETKYTBL.C
*				(c) H.Inoue 1990
*		set all  key  by KEY.TBL
*
*	Ver 1.1	1990 JUL 12	developed from SETKEY.EXE V 1.0
*	Ver 2.0	1991 APR 26
*
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>

void beep(void);

void main(int ac, char *av[])
{
	struct REGPACK reg;
	char check,key_table[800];
	int i;
	FILE *in;
	char buf[100];
	char data_file[50];

	fprintf(stderr,"SETKYTBL.EXE   V 2.0	                (C) 1990,1991  井上 博計\n"
			  "\n");

	if(ac == 2)
	{
		strcpy(data_file,av[1]);
		fprintf(stderr,"ｷｰ ﾃﾞｰﾀ ﾌｧｲﾙ %s\n",data_file);

	}
	else{
		fprintf(stderr,"KEY.EXE によって作成されたキーテーブル・データファイルを読み込み\n"
					"  システムのキーテーブルを書き換えます\n"
					"KEY.EXE は MS-DOS Ver 3.3x のものが使用可能（形式が同一なら他の\n"
					"  バージョンのものも使用可能）\n"
					"\n"
					"読み込むファイル名を入力して下さい (ﾘﾀｰﾝ のみで A:\\KEY.TBL)\n"
					" ｷｰ ﾃﾞｰﾀ ﾌｧｲﾙ : ");

		gets(buf);
		strcpy(data_file,buf);
		if(!strcmp(buf,"")) strcpy(data_file,"A:\\KEY.TBL");
		if(buf[0] == 0x1b) exit(0);

	}


	// sweep key data buffers
	for(i=0;i<=786;i++) key_table[i] = 0;

	in = fopen(data_file,"rb");
	if(in == NULL){
		fprintf(stderr,"\x1b[21mﾌｧｲﾙ : %s 読み込みエラー\x1b[m\n",data_file);
		beep();
		exit(1);
	}
	fseek(in,16,SEEK_SET);
	check = fread(key_table,sizeof(char),786,in);
	fclose(in);

	if(check == 0){
		fprintf(stderr,"\x1b[21mﾌｧｲﾙ : %s ﾃﾞｰﾀ ﾌｫｰﾏｯﾄ ｴﾗｰ\x1b[m\n",data_file);
		beep();
		exit(1);
	}

     reg.r_cx = 0x000d;
	reg.r_dx = FP_OFF(key_table);
	reg.r_ds = FP_SEG(key_table);
	reg.r_ax = 0x00ff;
	intr(0xdc,&reg);

	fprintf(stderr,"設定終了\n");

	exit(0);
}

void beep(void)
{
	sound(1500);
	delay(50);
	sound(500);
	delay(200);
	sound(2000);
	nosound();
}