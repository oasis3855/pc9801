/* XYGRFILx.CPP
   XYGRPH.EXE 用　ファイル操作／子プロセス　プログラム */
#include<stdio.h>
#include<string.h>
#include<alloc.h>
#include<process.h>
#include<conio.h>
#include<dos.h>
#include<glio.h>
#include"xygr.h"

#define	HELPWIN_LEN	22

#define HLP_ERR	"\r\n     ヘルプファイルが見つかりません"
#define	GED_ERR	"\r\n     ｸﾞﾗﾌｨｯｸｴﾃﾞｨﾀｰが実行できません"
#define	GSV_ERR	"\r\n      ｸﾞﾗﾌｨｯｸｾｰﾊﾞが実行できません"
#define	GLD_ERR	"\r\n      ｸﾞﾗﾌｨｯｸﾛｰﾀﾞが実行できません"
#define	GPL_ERR	"\r\n      ﾕｰｻﾞｰｺﾏﾝﾄﾞが実行できません"
#define	TMP_ERR	"\r\n      テンポラリーファイル　エラー"
#define	DAT_ERR	"\r\n         データファイル　エラー"
#define	ENV_ERR	"\r\n          環境ファイル　エラー"
#define	MEM_ERR	"\r\n           メモリが足りません"

/* エラーメッセージを表示する */
void do_error_win(char *str)
{
	int ret,dummy;

	/* ウインドウの行数を変える */
	dummy = w_info.data_max;
	w_info.data_max = 3;

	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);

	textcolor(w_info.color);

	gotoxy(1,1);
	cprintf("%s",str);

	getch();
	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);

	w_info.data_max = dummy;

}

/* オンラインヘルプ */
void do_help(void)
{
	FILE *in;
	char str[256];
	int page,i,counter;

	if((in = fopen("XYGRPH.HLP","rb")) == NULL)
	{	/* ヘルプファイル無し */
		do_error_win(HLP_ERR);
		return ;
	}

	gscreen(-1, 3, -1, -1);		/* グラフィック表示停止 */
	win_mmake(2, 3, 79, 24, _c_box, W_RECT, W_PAGE_1);
	_wscroll = 0;	/* スクロールの禁止 */
	textattr(_c_mnu);

	page = 1;	/* 最初は１ページを表示 */

	for(;;)
	{
		/* ページの先頭へ行く */
		counter = 0;
		fseek(in,0L,SEEK_SET);	/* ファイルを巻き戻す */
		for(;;)
		{
			if(fgets(str,100,in) == NULL)
			{	/* '^L' ファイルエンド */
				page--;		/* 元のページに戻る */
				fseek(in,0L,SEEK_SET);
				counter = 0;
				continue;
			}
			if(str[0] == 0x00)
			{	/* NULL ヘルプの終わり */
				page--;		/* 元のページに戻る */
				fseek(in,0L,SEEK_SET);
				counter = 0;
				continue;
			}
			if(str[0] == 0x0c) counter++;	/* 改ページ */
			if(counter >= page) break;
		}

		for(i=1;i<=HELPWIN_LEN;i++)
		{	/* 画面のクリア */
			gotoxy(1,i);
			cprintf("%78s"," ");
		}

		/* 表示 */
		gotoxy(1,1);
		for(i=1;i<=HELPWIN_LEN-1;i++)
		{
			if(fgets(str,100,in) == NULL)
			{	/* ファイルエンド */
				break;
			}
			if(str[0] == 0xc) break;	/* 改ページ */
			gotoxy(1,i);
			cprintf("%s",str);		/* 表示 */
		}

		for(;;)
		{
			gotoxy(1,HELPWIN_LEN);
			cprintf("ROLL UP : 次ページ   ROLL DOWN : 前ページ   ESC : ヘルプの終了",page);
			i = mnu_wread_key();
			if(i == 0x3600 || i == 0x3700 || i == 0x001b) break;
		}
		if(i == 0x001b) break;	/* 終了 */
		if(i == 0x3700) page--;	/* ↑ キー */
		if(i == 0x3600) page++;	/* ↓ キー */
		if(page <= 0) page = 1;		/* 最初のページは１ページ */

	}

	fclose(in);

	win_mdelete(2,3,79,24);		/* ウインドウの消去 */
	gscreen(3, 1, 0, 1);		/* グラフィック表示開始 */

	return ;
}

/* グラフィックエディター起動 */
void exe_gedit(void)
{
	char exe_file[30],par[50],dummy[50];

	/* 実行ファイルとパラメータの完成 */
	strcpy(dummy,exe_gedit_file);
	strcpy(exe_file,strtok(exe_gedit_file," \n"));	/* 実行ファイル */
	strcpy(par,strtok(NULL,"\n"));					/* パラメータ */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* 実行できなかった */
		do_error_win(GED_ERR);
		return ;
	}
}

/* ＸＹぷろったー起動 */
void exe_plot(void)
{
	char exe_file[30],par[50],dummy[50];

	/* 実行ファイルとパラメータの完成 */
	strcpy(dummy,exe_plot_file);
	strcpy(exe_file,strtok(dummy," \n"));	/* 実行ファイル */
	strcpy(par,strtok(NULL,"\n"));			/* パラメータ */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* 実行できなかった */
		do_error_win(GPL_ERR);
		return ;
	}
}

/* 画像ファイルの書き出し */
void do_save_file(void)
{
	char exe_file[30],par[50],dummy[50];

	if(!strlen(filename)) return;

	/* 実行ファイルとパラメータの完成 */
	strcpy(dummy,exe_gsv_file);
	strcpy(exe_file,strtok(dummy," \n"));	/* 実行ファイル */
	strcpy(par,strtok(NULL,"\n"));			/* パラメータ */
	strcat(par," ");
	strcat(par,filename);	/* パラメータ　＋　ファイル名 */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* 実行できなかった */
		do_error_win(GSV_ERR);
		return ;
	}
}

/* 画像ファイルの読み込み */
void do_load_file(void)
{
	char exe_file[30],par[50],dummy[50];

	if(!strlen(filename)) return;

	/* 実行ファイルとパラメータの完成 */
	strcpy(dummy,exe_gld_file);
	strcpy(exe_file,strtok(dummy," \n"));	/* 実行ファイル */
	strcpy(par,strtok(NULL,"\n"));			/* パラメータ */
	strcat(par," ");
	strcat(par,filename);	/* パラメータ　＋　ファイル名 */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* 実行できなかった */
		do_error_win(GLD_ERR);
		return ;
	}
}

/* 環境ファイルの書き込み */
void do_save_env(void)
{
	FILE *fi;
	char _filename[80];
	int i;

	if(!strlen(filename)) return;

	strcpy(_filename,strtok(filename,"."));
	strcat(_filename,".ENV");
	if((fi = fopen(_filename,"wt")) == NULL)
	{	/* ｵｰﾌﾟﾝ ｴﾗｰ */
		do_error_win(ENV_ERR);
		return;
	}

	fprintf(fi,"XYGRPH ver2 ENV File\n");
	fprintf(fi,
			":GEDIT\n"
			"%s\n"
			":USERPROG\n"
			"%s\n"
			":GSAVE\n"
			"%s\n"
			":GLOAD\n"
			"%s\n"
			"\n",
			exe_gedit_file,
			exe_plot_file,
			exe_gsv_file,
			exe_gld_file);

	fprintf(fi,
			 "menu      = %04x\n"
			 "menu rev  = %04x\n"
			 "menu box  = %04x\n"
			 "bar menu  = %04x\n"
			 "bar topic = %04x\n"
			 "bar rev   = %04x\n"
			 "info      = %04x\n"
			 "info box  = %04x\n"
			 "col grph  = %04x\n"
			 "col scale = %04x\n"
			 "col zero  = %04x\n",
			 _c_mnu,
			 _c_mnuv,
			 _c_box,
			 _c_barb,
			 _c_bar,
			 _c_barv,
			 _c_info,
			 _cg_info,
			 _c_gr_grph,
			 _c_gr_scale,
			 _c_gr_0);


	fprintf(fi,"\n"
			   "background pattern\n");
	for(i=0;i<8;i++) fprintf(fi,"%02x,%02x,%02x,%02x\n",
							_bpat[i*4+0],_bpat[i*4+1],
							_bpat[i*4+2],_bpat[i*4+3]);
	fprintf(fi,"\n"
			   "info area pattern\n");
	for(i=0;i<8;i++) fprintf(fi,"%02x,%02x,%02x,%02x\n",
							_infopat[i*4+0],_infopat[i*4+1],
							_infopat[i*4+2],_infopat[i*4+3]);


	fclose(fi);

}

/* 環境ファイルの読み込み */
void do_load_env(void)
{
	FILE *fi;
	char dummy[70],_filename[80];
	int i,_a,_b,_c,_d;

	if(!strlen(filename)) return;

	strcpy(_filename,strtok(filename,"."));
	strcat(_filename,".ENV");
	if((fi = fopen(_filename,"rt")) == NULL)
	{	/* ｵｰﾌﾟﾝ ｴﾗｰ */
		do_error_win(ENV_ERR);
		return;
	}

	fgets(dummy,250,fi);	/* ﾀｲﾄﾙのｽｷｯﾌﾟ */
	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	fgets(dummy,250,fi);
	strcpy(exe_gedit_file,strtok(dummy,"\n"));
	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	fgets(dummy,250,fi);
	strcpy(exe_plot_file,strtok(dummy,"\n"));
	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	fgets(dummy,250,fi);
	strcpy(exe_gsv_file,strtok(dummy,"\n"));
	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	fgets(dummy,250,fi);
	strcpy(exe_gld_file,strtok(dummy,"\n"));

	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	fscanf(fi,
			 "menu      = %04x\n"
			 "menu rev  = %04x\n"
			 "menu box  = %04x\n"
			 "bar menu  = %04x\n"
			 "bar topic = %04x\n"
			 "bar rev   = %04x\n"
			 "info      = %04x\n"
			 "info box  = %04x\n"
			 "col grph  = %04x\n"
			 "col scale = %04x\n"
			 "col zero  = %04x\n",
			 &_c_mnu,
			 &_c_mnuv,
			 &_c_box,
			 &_c_barb,
			 &_c_bar,
			 &_c_barv,
			 &_c_info,
			 &_cg_info,
			 &_c_gr_grph,
			 &_c_gr_scale,
			 &_c_gr_0);

	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	for(i=0;i<8;i++)
	{
		fscanf(fi,"%03x,%03x,%03x,%03x\n",
							&_a,&_b,
							&_c,&_d);
		_bpat[i*4+0] = (char)_a;
		_bpat[i*4+1] = (char)_b;
		_bpat[i*4+2] = (char)_c;
		_bpat[i*4+3] = (char)_d;
	}
	fgets(dummy,250,fi);	/* ｺﾒﾝﾄのｽｷｯﾌﾟ */
	for(i=0;i<8;i++)
	{
		fscanf(fi,"%03x,%03x,%03x,%03x\n",
							&_a,&_b,
							&_c,&_d);
		_infopat[i*4+0] = (char)_a;
		_infopat[i*4+1] = (char)_b;
		_infopat[i*4+2] = (char)_c;
		_infopat[i*4+3] = (char)_d;
	}

	fflush(fi);

	fclose(fi);
}
