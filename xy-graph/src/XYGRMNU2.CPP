/* XYGRMNUx.CPP
   XYGRPH.EXE 用　メニュー／メイン　プログラム */
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<process.h>
#include<signal.h>
#include<glio.h>
#include<gbios.h>
#include<winp.h>
#include<hdcpy.h>
#include<etc.h>
#include<gr.h>
#include"xygr.h"

char *m_str1[] = {" XYGRPHについて   ",
				  " 使い方の説明     ",
				  " 画像ﾌｧｲﾙロード   ",
				  " 画像ﾌｧｲﾙセーブ   ",
				  " 環境ﾌｧｲﾙロード   ",
				  " 環境ﾌｧｲﾙセーブ   ",
				  " ｸﾞﾗﾌｨｯｸ ｴﾃﾞｨﾀｰ   ",
				  " ﾕｰｻﾞｰｺﾏﾝﾄﾞ       ",
				  " ｺﾏﾝﾄﾞ ｼｪﾙ        ",
				  " クローズ(終了)   "};
char *m_str2[] = {" 標準ｻｲｽﾞプリント ",
				  " 縮小ｻｲｽﾞプリント ",
				  " プレビュー       "};
char *m_str3[] = {" ｙ＝ｆ(x)        ",
				  " ｘ,ｙ＝ｆ(t)     ",
				  " ｙ＝dｆ(x)/dx    ",
				  " 座標書き込み     ",
				  " ウインドウ消去   "};
char *m_str4[] = {" 画面座標設定     ",
				  " ﾏｳｽに依る範囲設定",
				  " ボーダー座標設定 ",
				  " 座標軸間隔の設定 ",
				  " 実行条件文       ",
				  " 変数ｔの範囲設定 ",
				  " ボーダーの有効性 ",
				  " 実行条件文有効性 ",
				  " 座標表示の種類   ",
				  " 原点座標の種類   ",
				  " 補完モード設定   ",
				  " 初期化モード設定 ",
				  " グラフ表示色設定 ",
				  " 座標表示色設定   ",
				  " 原点座標の色設定 "};

char *m_str_inp_xy[] = {"ｘ 最小値",
				  "ｘ 最大値",
				  "ｙ 最小値",
				  "ｙ 最大値"};
char *m_str_inp_xy_bod[] = {"ｘﾎﾞｰﾀﾞｰ 最小値",
				  "ｘﾎﾞｰﾀﾞｰ 最大値",
				  "ｙﾎﾞｰﾀﾞｰ 最小値",
				  "ｙﾎﾞｰﾀﾞｰ 最大値"};
char *m_str_inp_scale[] = {"ｘ座標表示間隔",
				  "ｙ座標表示間隔",
				  "原点ｘ座標表示間隔",
				  "原点ｙ座標表示間隔"};
char *m_str_inp_t[] = {"ｔ 開始値",
				  "ｔ 終了値",
				  "ｔ 増加分"};
char *m_str_fld[] = {"読み込む画像ファイル名"};
char *m_str_fsv[] = {"書き出す画像ファイル名"};
char *m_str_eld[] = {"読み込む環境ファイル名"};
char *m_str_esv[] = {"書き出す環境ファイル名"};
char *m_str_inp_c_grph[] = {"グラフの色 (0 - 15)"};
char *m_str_inp_c_scale[] = {"座標の色 (0 - 15)"};
char *m_str_inp_c_0[] = {"原点座標の色 (0 - 15)"};
char *m_str_inp_fx[] = {"関数 ｙ＝ｆ(ｘ)"};
char *m_str_inp_fxyt[] = {"関数 ｘ＝ｆ(ｔ)",
						"関数 ｙ＝ｇ(ｔ)"};
char *m_str_inp_dfx[] = {"関数 ｙ＝dｆ(ｘ)／dｘ"};
char *m_str_inp_if[] = {"ｆ(ｘ):この式が０以上の時グラフを描きます"};

char *m_str_sw_scale[] = {"   座標無し         ",
				  "   全面に座標表示   ",
				  "   周りに座標表示   "};
char *m_str_sw_0[] = {"   原点座標無し     ",
				  "   直線で表示       ",
				  "   目盛り表示       ",
				  "   目盛り,直線表示  "};
char *m_str_sw_line[] = {"   ドットで表示     ",
				  "   直線補完する     "};
char *m_str_sw_init[] = {"   クリア・座標     ",
				  "   座標             ",
				  "   クリア           ",
				  "   上書き           "};
char *m_str_sw_if[] = {"   実行条件文 無効  ",
				  "   実行条件文 有効  "};
char *m_str_sw_bod[] = {"   ボーダー 無効    ",
				  "   ボーダー 有効    "};


/* メニューバー　タイトル */
char *mbar_str[] = {"ファイル",
				"プリント",
				"ジョブ",
				"オプション"};
/* メニューバー座標 */
int mbar_x[] = {4,14,25,34};

/* ダミーバッファ */
char _str_dmy[] = {"ﾊｲﾚｿﾞﾘｭｰｼｮﾝﾓｰﾄﾞ では動きません.\n"};
/* キー入力用バッファ　ショート */
char *buf_s[] = {"(C)opyright 1992 井上 博計     ",
				 "Nifty Serve GDH03306           ",
				 "PC VAN KYJ15060                ",
				 "ﾒｰﾙは以上の所までどうぞ！！    "};
/* キー入力用バッファ　ロング */
char *buf_l[]={"このプログラムはｘｙ座標系の２次元グラフを描きます。          ",
			   "GRPH キー又は F10 キーでメニューに移れます。                  "};


void main(int ac, char *av[])
{
	puts("XYGRPH.EXE						(C) 1992 Hirokazu Inoue\n");
	if(ac >= 2 && !strcmp(av[1],"-?"))
	{	/* ヘルプ */
		puts("usage : XYGRPH (-?) <ENV file>\n"
			 "To activate menu , press 'GRPH' or 'F･10'\n"
			 "This program is for PC-9801 MS-DOS normal mode\n");
		exit(0);
	}

	/* 画面の初期化 */
	init_graphic();
	gcls();
	printf("\x1b[2J\x1b[>1h\x1b[>5h");	/* ｶｰｿﾙ OFF 等 */

	/* マシンのチェック */
	if(PC98_chk_hi_resolution())
	{
		printf("%s\n",_str_dmy);
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
	_cg_info = _WHITE;			/* インフォメーションエリアボーダー */
	_c_gr_grph = _LIGHTGREEN;	/* グラフの色 */
	_c_gr_scale = _LIGHTRED;	/* 座標の色 */
	_c_gr_0 = _WHITE;			/* 原点座標の色 */

	strcpy(commandcom,getenv("COMSPEC"));
	if(!strcmp(commandcom,NULL)) strcpy(commandcom,"COMMAND.COM");

	/* 環境の読み込み */
	init_GMNU();
	strcpy(filename,"XYGRPH");	/* 最初の環境ファイル名 */
	if(ac > 1) strcpy(filename,av[1]);	/* 環境ファイル名が指定されたとき */
	strcpy(exe_gedit_file,"GEDIT.EXE");
	strcpy(exe_plot_file,"MPS.EXE");
	strcpy(exe_gsv_file,"GRGBSV.EXE");
	strcpy(exe_gld_file,"GRGBSV.EXE -l");
	do_load_env();	/* 環境ファイルを読み込む */
	chg_palette();


	/* 全てのパラメータの初期化 */
	init_GMNU();

	/* タイトル */
	mnu_barmake(&w_bar,mbar_str,mbar_x,"ＸＹ ＧRAPH",69);
	chg_palette();
	gsetpattern(32,_bpat);
	gbarp(0,16,639,399,_BLACK);
	delay(1500);
	do_info_win();

	/* XYGraph 計算表示ウインドウ表示 */
	write_window();
	write_scale();


	/* メニュー開始 */
	mnu_menubar(&w_bar,mbar_str,mbar_x,"ＸＹ ＧRAPH",69);

}

int bar_mnu_func(int i)
{
	int chk;
	switch(i)
	{
		case	1:	chk = do_menu_1();break;
		case	2:	chk = do_menu_2();break;
		case	3:	chk = do_menu_3();break;
		case	4:	chk = do_menu_4();break;
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
			case 1	:			/* このプログラムについて */
				do_info_win();
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	2	:		/* 説明書 */
				do_help();
				restore_screen();
				chg_palette();
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case 	3	:		/* 画像読み込み */
				if(do_input_char_1l(filename,m_str_fld))
				{	/* 画像ロード */
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					do_load_file();
					init_graphic();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{	/* キャンセル */
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	4	:		/* 画像書き出し */
				if(do_input_char_1l(filename,m_str_fsv))
				{	/* 画像セーブ */
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					do_save_file();
					init_graphic();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{	/* キャンセル */
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case 	5	:		/* 環境読み込み */
				if(do_input_char_1l(filename,m_str_eld))
				{	/* 環境ロード */
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					do_load_env();
					init_GMNU();
					init_graphic();
					write_window();		/* グラフィックを書き直す */
					write_scale();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{	/* キャンセル */
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	6	:		/* 環境書き出し */
				if(do_input_char_1l(filename,m_str_esv))
				{	/* 環境セーブ */
					do_save_env();
				}
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	7	:		/* ｸﾞﾗﾌｨｯｸ ｴﾃﾞｨﾀｰ */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				exe_gedit();
				init_graphic();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	8	:		/* ﾌﾟﾛｯﾀｰ */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				exe_plot();
				init_graphic();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	9	:		/* COMMAND.COM */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				gscreen(-1,3,-1,-1);
				printf("\x1b[2J\x1b[>1l\x1b[>5l\x1b[m");
				printf("EXIT を入力すると XYGRPH に戻ります\n");
				spawnl(P_WAIT,commandcom,commandcom,NULL);
				printf("\x1b[2J\x1b[24;43H\x1b[47m何かキーを押すと XYGRPH に戻ります");
				getch();
				init_graphic();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	10	:		/* クローズ */
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
			case	1	:
				if(do_inp_fx())
				{
					w3.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
					write_graph(1);
					w3.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				else
				{
					w3.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				break;
			case	2	:
				if(do_inp_fxyt())
				{
					w3.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
					write_graph(2);
					w3.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				else
				{
					w3.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				break;
			case	3	:
				if(do_inp_dfx())
				{
					w3.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
					write_graph(3);
					w3.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				else
				{
					w3.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				break;
			case	4	:	/* 座標描画 */
				w3.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				write_scale();
				w3.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case	5	:	/* 画面消去 */
				w3.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				write_window();
				w3.mode = W_FWRITE;
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

int do_menu_4(void)
{
	int ret,check;

	w4.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w4,0,m_str4,&ret);	// open menu
	for(;;)
	{
		w4.mode = W_MENU;
		mnu_GLIO_gmenu(&w4,0,m_str4,&ret);	// do menu
		if(ret != 0x0d) break;	/* RET 以外だったらﾒﾆｭｰ抜ける */
		switch(w4.cur_topic)
		{
			case	1	:	/* XY 座標入力 */
				do_inp_xy();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case 	2	:	/* マウス */
				w4.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				do_mouse_plot();
				w4.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case 	3	:	/* XY ﾎﾞｰﾀﾞｰ座標入力 */
				do_inp_xy_bod();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	4	:	/* 座標軸の間隔設定 */
				do_inp_scale();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	5	:	/* if文 */
				do_inp_if();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	6	:	/* ｔの範囲を読み込む */
				do_inp_t();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	7	:
				do_sw_bod();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	8	:
				do_sw_if();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	9	:	/* 座標の種類 */
				do_sw_scale();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	10	:	/* 原点座標の種類 */
				do_sw_0();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	11	:	/* 直線補完のスイッチ */
				do_sw_line();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	12	:	/* 描画初期化に関する規定値 */
				do_sw_init();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	13	:	/* グラフの色の読み込み */
				do_inp_c_grph();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	14	:	/* 座標の色の読み込み */
				do_inp_c_scale();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	15	:	/* 原点座標の色の読み込み */
				do_inp_c_0();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			default	:	break;
		}
	}
	check = ret;
	w4.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w4,0,m_str4,&ret);

	return check;
}

void do_info_win(void)
{
	int ret;

	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);

	textattr(w_info.color);
	gotoxy(10,2);
	cputs("ＸＹ　ＧRAPH  ｱﾌﾟﾘｹｰｼｮﾝ");
	gotoxy(12,3);
	cputs("XYGRPH.EXE / PC-9801");
	gotoxy(15,4);
	cputs("Version 2.00");
	gotoxy(12,5);
	cputs("(C) 1992 井上 博計");

	gotoxy(11,8);
	cputs("何かｷｰを押して下さい");

	getch();
	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);
}


void restore_screen(void)
{	/* テキストのみ */
	printf("\x1b[2J\x1b[>1h\x1b[m\x1b[>5h");
	mnu_barmake(&w_bar,mbar_str,mbar_x,"ＸＹ ＧRAPH",69);
	prn_data_win();
}

/* グラフィックを初期化し直す */
void init_graphic(void)
{
	ginit();
	gscreen(3,1,0,1);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);
	gbios_rect(10,200,30,300);
	gbios_rect(10,200,30,300);
	gbios_setmode(0,GBIOS_COPY,GBIOS_GREEN,0xffff);
	gcolor(_LIGHTGREEN,_BLACK,2);
	gview(0,0,639,399,-1,-1);
	gsetpattern(32,_bpat);

	chg_palette();
}


/* 整数を１個読み込む汎用関数 */
int do_input_int_1s(int *a, char *m_str[])
{
	int check,retcode;

	strncpy(buf_s[0],itoa(*a,_str_dmy,10),wi_4s.data_len - 2);

	wi_1s.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_1s,buf_s,m_str,&retcode);
	if(check != 0) return 0;

	wi_1s.mode = W_MENU;
	inp_GLIO_gwin(&wi_1s,buf_s,m_str,&retcode);
	if(retcode == 0)
	{
		*a = atoi(buf_s[0]);
	}
	wi_1s.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_1s,buf_s,m_str,&retcode);

	if(!retcode) return 1;	/* 入力された */
	return 0;				/* キャンセルされた */
}

/* 実数を３個読み込む汎用関数 */
int do_input_double_3s(double *a, double *b, double *c,
					   char *m_str[])
{
	int check,retcode;

	strncpy(buf_s[0],gcvt(*a,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[1],gcvt(*b,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[2],gcvt(*c,15,_str_dmy),wi_4s.data_len - 2);

	wi_3s.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_3s,buf_s,m_str,&retcode);
	if(check != 0) return 0;

	wi_3s.mode = W_MENU;
	inp_GLIO_gwin(&wi_3s,buf_s,m_str,&retcode);
	if(retcode == 0)
	{
		*a = calc_str(buf_s[0]);
		*b = calc_str(buf_s[1]);
		*c = calc_str(buf_s[2]);
	}
	wi_3s.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_3s,buf_s,m_str,&retcode);

	if(!retcode) return 1;	/* 入力された */
	return 0;				/* キャンセルされた */
}

/* 実数を４個読み込む汎用関数 */
int do_input_double_4s(double *a, double *b, double *c, double *d,
					   char *m_str[])
{
	int check,retcode;

	strncpy(buf_s[0],gcvt(*a,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[1],gcvt(*b,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[2],gcvt(*c,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[3],gcvt(*d,15,_str_dmy),wi_4s.data_len - 2);

	wi_4s.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_4s,buf_s,m_str,&retcode);
	if(check != 0) return 0;

	wi_4s.mode = W_MENU;
	inp_GLIO_gwin(&wi_4s,buf_s,m_str,&retcode);
	if(retcode == 0)
	{
		*a = calc_str(buf_s[0]);
		*b = calc_str(buf_s[1]);
		*c = calc_str(buf_s[2]);
		*d = calc_str(buf_s[3]);
	}
	wi_4s.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_4s,buf_s,m_str,&retcode);

	if(!retcode) return 1;	/* 入力された */
	return 0;				/* キャンセルされた */
}

/* 画面の座標を読み込む */
void do_inp_xy(void)
{
	do_input_double_4s(&x_min, &x_max, &y_min, &y_max, m_str_inp_xy);

	if(x_min >= x_max) x_max = x_min + 0.001;
	if(y_min >= y_max) y_max = y_min + 0.001;

}

/* ボーダーの座標を読み込む */
void do_inp_xy_bod(void)
{
	do_input_double_4s(&xl_min, &xl_max, &yl_min, &yl_max, m_str_inp_xy_bod);

	if(xl_min >= xl_max) xl_max = xl_min + 0.001;
	if(yl_min >= yl_max) yl_max = yl_min + 0.001;
}

/* 座標軸の間隔を読み込む */
void do_inp_scale(void)
{
	do_input_double_4s(&x_scale, &y_scale, &x_scale_0, &y_scale_0, m_str_inp_scale);

	if(x_scale <= 0.0) x_scale = 1.0;
	if(y_scale <= 0.0) y_scale = 1.0;
	if(x_scale_0 <= 0.0) x_scale_0 = 1.0;
	if(y_scale_0 <= 0.0) y_scale_0 = 1.0;
}

/* ｔの範囲を読み込む */
void do_inp_t(void)
{
	do_input_double_3s(&tl_min, &tl_max, &t_delta, m_str_inp_t);

	if(t_delta <= 0.0) t_delta = 0.001;
	if(tl_max <= tl_min) tl_max = tl_min + 0.001;
}

/* グラフの色を読み込む */
void do_inp_c_grph(void)
{
	do_input_int_1s(&_c_gr_grph, m_str_inp_c_grph);

	if(_c_gr_grph < 0 || _c_gr_grph > 15) _c_gr_grph = _LIGHTGREEN;
}

/* 座標の色を読み込む */
void do_inp_c_scale(void)
{
	do_input_int_1s(&_c_gr_scale, m_str_inp_c_scale);

	if(_c_gr_scale < 0 || _c_gr_scale > 15) _c_gr_scale = _LIGHTRED;
}

/* 原点座標の色を読み込む */
void do_inp_c_0(void)
{
	do_input_int_1s(&_c_gr_0, m_str_inp_c_0);

	if(_c_gr_0 < 0 || _c_gr_0 > 15) _c_gr_0 = _LIGHTRED;
}

/*  文字列を１個読み込む汎用関数 */
int do_input_char_1l(char *buf, char *m_str[])
{
	int check,retcode;

	strncpy(buf_l[0],buf,60);

	wi_1l.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_1l,buf_l,m_str,&retcode);
	if(check != 0) return -1;

	check = 0;
	wi_1l.mode = W_MENU;
	inp_GLIO_gwin(&wi_1l,buf_l,m_str,&retcode);
	if(retcode == 0)
	{
		check = 1;
		strcpy(buf,buf_l[0]);
	}

	wi_1l.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_1l,buf_l,m_str,&retcode);

	if(check) return 1;	/* 入力された */
	return 0;				/* キャンセルされた */
}

/*  文字列を２個読み込む汎用関数 */
int do_input_char_2l(char *buf1, char *buf2, char *m_str[])
{
	int check,retcode;

	strncpy(buf_l[0],buf1,60);
	strncpy(buf_l[1],buf2,60);

	wi_2l.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_2l,buf_l,m_str,&retcode);
	if(check != 0) return -1;

	check = 0;
	wi_2l.mode = W_MENU;
	inp_GLIO_gwin(&wi_2l,buf_l,m_str,&retcode);
	if(retcode == 0)
	{
		check = 1;
		strcpy(buf1,buf_l[0]);
		strcpy(buf2,buf_l[1]);
	}

	wi_2l.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_2l,buf_l,m_str,&retcode);

	if(check) return 1;	/* 入力された */
	return 0;				/* キャンセルされた */
}

/* ｙ＝ｆ(ｘ) を入力 */
int do_inp_fx(void)
{
	return do_input_char_1l(formula_string, m_str_inp_fx);
}

/* ｘ＝ｆ(ｔ),ｙ＝ｇ(ｔ) を入力 */
int do_inp_fxyt(void)
{
	return do_input_char_2l(formula_str_x, formula_str_y, m_str_inp_fxyt);
}

/* ｙ＝dｆ(ｘ)／dｘ を入力 */
int do_inp_dfx(void)
{
	return do_input_char_1l(formula_str_dfx, m_str_inp_dfx);
}

/* if 文 を入力 */
int do_inp_if(void)
{
	return do_input_char_1l(formula_str_if, m_str_inp_if);
}

/* スイッチの汎用関数 */
int do_switch(int *sw, int max_sw, char *m_str[])
{
	int ret,i;

	w_sw.data_max = max_sw;		/* 最大スイッチ数の定義 */
	w_sw.cur_topic = *sw;		/* 現在のスイッチ */

	/* 現在のスイッチに印を付ける */
	for(i=0;i<w_sw.data_max;i++) m_str[i][1] = ' ';
	m_str[*sw-1][1] = 'O';

	w_sw.cur_topic = *sw;
	w_sw.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w_sw,0,m_str,&ret);
	w_sw.mode = W_MENU;
	mnu_GLIO_gmenu(&w_sw,0,m_str,&ret);
	i = ret;
	w_sw.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w_sw,0,m_str,&ret);

	if(i == 0x1b) return 0;	/* キャンセルされた */
	*sw = w_sw.cur_topic;		/* スイッチの更新 */
	return 1;	/* 入力された */
}

/* 座標の種類を選ぶ */
int do_sw_scale(void)
{
	return do_switch(&sw_scale, 3, m_str_sw_scale);
}

/* 原点座標の種類を選ぶ */
int do_sw_0(void)
{
	return do_switch(&sw_0, 4, m_str_sw_0);
}

/* 直線補完の on/off */
int do_sw_line(void)
{
	return do_switch(&sw_gr_line, 2, m_str_sw_line);
}

/* 描画時の画面初期化に関するスイッチ */
int do_sw_init(void)
{
	return do_switch(&sw_initmode, 4, m_str_sw_init);
}

/* ボーダー座標の有効性関するスイッチ */
int do_sw_bod(void)
{
	return do_switch(&sw_bod, 2, m_str_sw_bod);
}

/* if 文の有効性に関するスイッチ */
int do_sw_if(void)
{
	return do_switch(&sw_if, 2, m_str_sw_if);
}
