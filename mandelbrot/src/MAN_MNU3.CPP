/* MAN_MNUx.CPP
   MANDEL.EXE 用　メニュー／メイン　プログラム */
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<process.h>
#include<signal.h>
#include<glio.h>
#include<winp.h>
#include<hdcpy.h>
#include<etc.h>
#include<gr.h>
#include"mandel2.h"

GMNU_INFO w1,w2,w3,w4,wi,w_pal,w_sv,w_info;
GMNUBAR_INFO	w_bar;


char *m_str1[] = {" Mandelについて   ",
			   " 画像ﾌｧｲﾙロード   ",
			   " 画像ﾌｧｲﾙセーブ   ",
			   " 環境ﾌｧｲﾙロード   ",
			   " 環境ﾌｧｲﾙセーブ   ",
			   " 整数ﾓｰﾄﾞ処理開始 ",
			   " 実数ﾓｰﾄﾞ処理開始 ",
			   " ｺﾏﾝﾄﾞ ｼｪﾙ        ",
			   " クローズ(終了)   "};
char *m_str2[] = {" 標準ｻｲｽﾞプリント ",
			   " 縮小ｻｲｽﾞプリント ",
			   " プレビュー       "};
char *m_str3[] = {" 処理座標設定     ",
			   " ﾏｳｽに依る中心設定",
			   " 処理諧調設定     ",
			   " 計算ﾚﾍﾞﾙ設定     ",
			   " 表示ｶﾗｰﾊﾟﾚｯﾄ設定 ",
			   " 処理簡略化設定   "};
char *m_str4[] = {"実数軸中心値",
			   "虚数軸中心値",
			   "表示範囲"};
char *m_str_pal[] = {"   標準１６色       ",
				 "   緑１６諧調       ",
				 "   ﾓﾉｸﾛ１６諧調     ",
				 "   プレビュー       "};

char *m_str_mc[] = {"処理諧調設定 (最大ｶﾗｰ)"};
char *m_str_ml[] = {"計算レベル設定 (諧調より大)"};
char *m_str_fmode[] = {"簡略化指数"};
char *m_str_fld[] = {"ロードファイル名(8文字)"};
char *m_str_fsv[] = {"セーブファイル名(8文字)"};

char *mbar_str[] = {"ファイル",
				"プリント",
				"オプション"};
int mbar_x[] = {5,17,30};

char _str_dmy[] = {"ﾊｲﾚｿﾞﾘｭｰｼｮﾝﾓｰﾄﾞ では動きません.\n"};
char commandcom[30];

void main(void)
{
	/* 画面の初期化 */
	ginit();
	gscreen(3,1,0,1);
	gcolor(_LIGHTGREEN,_BLACK,2);
	gcls();
	gview(0,0,639,399,-1,-1);
	printf("\x1b[2J\x1b[>1h\x1b[>5h");	/* ｶｰｿﾙ OFF 等 */

	/* マシンのチェック */
	if(PC98_chk_hi_resolution())
	{
		puts(_str_dmy);
		exit(0);
	}

	signal(SIGINT,SIG_IGN);
	signal(SIGFPE,SIG_IGN);
	signal(SIGABRT,SIG_IGN);
	kb_beep_off();

	/* カラー初期値 */
	_c_mnu  = WHITE|REVERSE;	/* メニュー */
	_c_mnuv = WHITE;			/* メニュー反転 */
	_c_box  = WHITE|REVERSE;	/* メニュー枠 */
	_c_barb = WHITE|REVERSE;	/* バー */
	_c_bar  = WHITE|REVERSE;	/* バーメニュー */
	_c_barv = WHITE;			/* バーメニュー反転 */
	_c_info = WHITE;			/* 座標値等表示 */
	_cg_man = _BLUE;			/* マンデル画面ボーダー */
	_cg_info = _BLUE;			/* インフォメーションエリアボーダー */

	strcpy(commandcom,getenv("COMSPEC"));
	if(!strcmp(commandcom,NULL)) strcpy(commandcom,"COMMAND.COM");

	/* 環境の読み込み */
	init_GMNU();
	strcpy(filename,"MANDEL");
	load_env();
	chg_palette();


	/* 全てのパラメータの初期化 */
	init_GMNU();

	/* タイトル */
	mnu_barmake(&w_bar,mbar_str,mbar_x,"ﾏﾝﾃﾞﾙﾌﾞﾛｰﾄ (C) 1992 井上 博計",52);
	chg_palette();
	gsetpattern(32,_bpat);
	gbarp(0,16,639,399,_BLACK);
	delay(1500);
	do_info_win();

	/* MANDEL 計算表示ウインドウ表示 */
	mk_mandel_win();

	// メニュー開始
	mnu_menubar(&w_bar,mbar_str,mbar_x,"ﾏﾝﾃﾞﾙﾌﾞﾛｰﾄ (C) 1992 井上 博計",52);

}

int bar_mnu_func(int i)
{
	int chk;
	switch(i)
	{
		case	1:	chk = do_menu_1();break;
		case	2:	chk = do_menu_2();break;
		case	3:	chk = do_menu_3();break;
		default:	chk = 0x1b;break;
	}
	return chk;
}


int do_menu_1(void)
{
	int ret,check;


	w1.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w1,0,m_str1,&ret);		// open menu
	for(;;)
	{
		w1.mode = W_MENU;
		mnu_GLIO_gmenu(&w1,0,m_str1,&ret);		// do menu
		if(ret != 0x0d) break;	/* RET 以外だったらﾒﾆｭｰ抜ける */
		switch(w1.cur_topic)
		{
			case 1	:		// Mandelについて
				do_info_win();
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case 2	:		// 画像読み込み
				if(do_input_char(filename,m_str_fld) == 0)
				{
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					load_file();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	3	:		// 画像書き出し
				if(do_input_char(filename,m_str_fsv) == 0)
				{
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					save_file();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case 4	:		// 環境読み込み
				if(do_input_char(filename,m_str_fld) == 0)
				{
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					load_env();
					init_GMNU();
					restore_screen();
					chg_palette();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	5	:		// 環境書き出し
				if(do_input_char(filename,m_str_fsv) == 0)
				{
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					save_env();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	6	:		// 整数モード実行
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				mk_mandel_win();
				do_mandel();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	7	:		// 実数モード実行
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				mk_mandel_win();
				do_mandel_C();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	8	:		// COMMAND.COM
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				gscreen(-1,3,-1,-1);
				printf("\x1b[2J\x1b[>1l\x1b[>5l\x1b[m");
				printf("EXIT を入力すると MANDEL に戻ります\n");
				spawnl(P_WAIT,commandcom,commandcom,NULL);
				printf("\x1b[2J\x1b[24;43H\x1b[47m何かキーを押すと MANDEL に戻ります");
				getch();
				gscreen(3,1,0,1);
				gcolor(_LIGHTGREEN,_BLACK,2);
				chg_palette();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	9	:		// クローズ
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				gcls();
				gscreen(3,3,-1,-1);
				printf("\x1b[2J\x1b[>1l\x1b[>5l\x1b[m");
				sound(2000);
				delay(20);
				nosound();
				kb_beep_on();
				exit(0);
				break;
			default	:	break;
		}
	}
	check = ret;

	w1.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w1,0,m_str1,&ret);

	return check;


}

void restore_screen(void)
{	/* テキストのみ */
	printf("\x1b[2J\x1b[>1h\x1b[m\x1b[>5h");
	mnu_barmake(&w_bar,mbar_str,mbar_x,"ﾏﾝﾃﾞﾙﾌﾞﾛｰﾄ (C) 1992 井上 博計",52);
	prn_data_win();
}

int do_menu_2(void)
{
	int ret,check;

	w2.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w2,0,m_str2,&ret);	// open menu
	for(;;)
	{
		w2.mode = W_MENU;
		mnu_GLIO_gmenu(&w2,0,m_str2,&ret);	// do menu
		if(ret != 0x0d) break;	/* RET 以外だったらﾒﾆｭｰ抜ける */
		switch(w2.cur_topic)
		{
			case 1	:		// 標準サイズプリント
				w2.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				_hc_sX = 1;_hc_sY = 1;
				hc_hardcopy();
				w2.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				break;
			case 2	:		// 縮小サイズプリント
				w2.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				_hc_sX = 0;_hc_sY = 0;
				hc_hardcopy();
				w2.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				break;
			case 3	:		// プレビュー
				int dummy;
				dummy = cur_palette;
				cur_palette = 4;
				chg_palette();
				getch();
				cur_palette = dummy;
				chg_palette();
				break;
			default	:	break;

		}
	}
	check = ret;
	w2.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w2,0,m_str2,&ret);

	return check;
}


int do_menu_3(void)
{
	int ret,check;

	w3.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w3,0,m_str3,&ret);	// open menu
	for(;;)
	{
		w3.mode = W_MENU;
		mnu_GLIO_gmenu(&w3,0,m_str3,&ret);	// do menu
		if(ret != 0x0d) break;	/* RET 以外だったらﾒﾆｭｰ抜ける */
		switch(w3.cur_topic)
		{
			case 1	:		// 中心座標設定
				do_input_w4();
				prn_data_win();
				w3.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case 2	:		// マウス
				w3.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				do_mouse_plot();
				w3.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case 3	:		// 処理諧調設定
				do_input_int(&maxcolor,m_str_mc);
				if(maxcolor <= 0) maxcolor = 1;
				prn_data_win();
				w3.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case	4	:		// 計算レベル設定
				do_input_int(&maxloop,m_str_ml);
				if(maxloop <= 10) maxloop = 10;
				if(maxloop > 1000) maxloop = 1000;
				prn_data_win();
				w3.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case	5	:		// カラーパレット設定
				do_mnu_palette();
				prn_data_win();
				w3.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case	6	:		// 処理簡略化設定
				do_input_int(&f_mode,m_str_fmode);
				if(f_mode <= 0) f_mode = 1;
				if(f_mode > Ywidth) f_mode = Ywidth;
				prn_data_win();
				w3.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			default	:	break;
		}
	}
	check = ret;
	w3.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w3,0,m_str3,&ret);

	return check;
}

void do_input_w4(void)
{
	int check,i,retcode;
	long PmX,PmY,PmR;
	char *buf[] = {"(C)opyright 1992 井上 博計     ",
				"Nifty Serve GDH03306           ",
				"ﾒｰﾙは以上の所までどうぞ！！    "};


	strncpy(buf[0],gcvt(x_cen,15,_str_dmy),w4.data_len - 2);
	strncpy(buf[1],gcvt(y_cen,15,_str_dmy),w4.data_len - 2);
	strncpy(buf[2],gcvt(xy_scale,15,_str_dmy),w4.data_len - 2);

	w4.mode = W_FWRITE;
	check = inp_GLIO_gwin(&w4,buf,m_str4,&retcode);
	if(check != 0) return;

	w4.mode = W_MENU;
	inp_GLIO_gwin(&w4,buf,m_str4,&retcode);
	if(retcode == 0)
	{
		x_cen = atof(buf[0]);
		y_cen = atof(buf[1]);
		xy_scale = atof(buf[2]);
	}

	if(xy_scale <= 0) xy_scale = 1.0e-10;

	w4.mode = W_CLOSE;
	inp_GLIO_gwin(&w4,buf,m_str4,&retcode);

	set_param();
}


void do_input_int(int *num, char *m_str[])
{
	int check,i,retcode;
	char *buf[] = {"(C)opyright 1992 井上 博計     ",
				"Nifty Serve GDH03306           ",
				"ﾒｰﾙは以上の所までどうぞ！！    "};

	strncpy(buf[0],itoa(*num,_str_dmy,10),wi.data_len-2);


	wi.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi,buf,m_str,&retcode);
	if(check != 0) return;

	wi.mode = W_MENU;
	inp_GLIO_gwin(&wi,buf,m_str,&retcode);
	if(retcode == 0) *num = (int)atoi(buf[0]);


	wi.mode = W_CLOSE;
	inp_GLIO_gwin(&wi,buf,m_str,&retcode);


}

int do_input_char(char *str, char *m_str[])
{
	int check,i,retcode;
	char *buf[] = {"(C)opyright 1992 井上 博計     ",
				"Nifty Serve GDH03306           ",
				"ﾒｰﾙは以上の所までどうぞ！！    "};

	strncpy(buf[0],str,10);

	wi.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi,buf,m_str,&retcode);
	if(check != 0) return -1;

	check = -1;

	wi.mode = W_MENU;
	inp_GLIO_gwin(&wi,buf,m_str,&retcode);
	if(retcode == 0)
	{
		strcpy(str,buf[0]);
		check = 0;
	}


	wi.mode = W_CLOSE;
	inp_GLIO_gwin(&wi,buf,m_str,&retcode);

	return check;

}

void do_mnu_palette(void)
{
	int ret,i;

	/* 現在のパレットをチェック */
	for(i=0;i<w_pal.data_max;i++) m_str_pal[i][1] = ' ';
	m_str_pal[cur_palette-1][1] = 'O';

	w_pal.cur_topic = cur_palette;	// cur palette
	w_pal.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w_pal,0,m_str_pal,&ret);	// open menu
	w_pal.mode = W_MENU;
	mnu_GLIO_gmenu(&w_pal,0,m_str_pal,&ret);	// do menu
	if(ret != 0x1b)
	{
		switch(w_pal.cur_topic)
		{
			case 1	:		// Natural Col
				cur_palette = 1;
				break;
			case 2	:		// Green
				cur_palette = 2;
				break;
			case 3	:		// Mono
				cur_palette = 3;
				break;
			case	4	:		// Preview
				cur_palette = 4;
				break;
			default	:	break;
		}
		chg_palette();
	}
	w_pal.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w_pal,0,m_str_pal,&ret);

}

void do_info_win(void)
{
	int ret;

	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);

	textattr(w_info.color);
	gotoxy(9,2);
	cputs("マンデルブロート集合描画");
	gotoxy(12,3);
	cputs("MANDEL.EXE / PC-9801");
	gotoxy(15,4);
	cputs("Version 2.00B");
	gotoxy(12,5);
	cputs("(C) 1992 井上 博計");

	gotoxy(11,8);
	cputs("何かｷｰを押して下さい");

	getch();
	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);
}

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

