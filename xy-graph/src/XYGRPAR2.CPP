/* XYGRPARx.CPP
	XYGRPH.EXE 用　初期設定プログラム */
#include<conio.h>
#include<string.h>
#include<math.h>
#include<hdcpy.h>
#include<txtl.h>
#include<glio.h>
#include<etc.h>
#include"xygr.h"

double x_min,x_max,y_min,y_max;			// man C
int maxloop,maxcolor,f_mode,cur_palette;	// Glob
double dxratio,dyratio;					// Glob
double x_ratio,y_ratio;
int x_width,y_width;
unsigned char formula_string[80];					/* ｙ＝ｆ(ｘ) */
unsigned char formula_str_x[80],formula_str_y[80];	/* ｙ＝ｆ(ｔ),ｘ＝ｆ(ｔ) */
unsigned char formula_str_dfx[80];					/* ｙ＝dｆ(ｘ)/dｘ */
unsigned char formula_str_if[80];
int sw_scale,sw_0,sw_if,sw_bod,sw_gr_line,sw_initmode;
double x_scale,y_scale,x_scale_0,y_scale_0;
double xl_min,xl_max,yl_min,yl_max,tl_min,tl_max,t_delta;
int math_err_flag;

/* メニュー構造体 */
GMNU_INFO w1,w2,w3,w4,wi,wd,w_pal,w_info;
GMNU_INFO wi_1s,wi_3s,wi_4s,wi_1l,wi_2l,w_sw;
GMNUBAR_INFO	w_bar;

int _c_mnu,_c_mnuv,_c_box,_c_bar,_c_barv,_c_barb,_c_info;
int _cg_info,_cg_man;
int _c_gr_grph,_c_gr_scale,_c_gr_0;

char commandcom[50],filename[80];
char exe_gedit_file[50],exe_plot_file[50],exe_gsv_file[50],exe_gld_file[50];

char far _bpat[] = {0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,0x00};
char far _infopat[] = {0x00,0x00,0x00,0xf0,
			 0x00,0x00,0x00,0x0f,
			 0x00,0x00,0x00,0xf0,
			 0x00,0x00,0x00,0x0f,
			 0x00,0x00,0x00,0xf0,
			 0x00,0x00,0x00,0x0f,
			 0x00,0x00,0x00,0xf0,
			 0x00,0x00,0x00,0x0f,0x00};



void write_window(void)
{
	chg_palette();
	gsetpattern(32,_bpat);
	gbarp(0,16,639,399,_BLACK);	/* 画面全体 */
	set_param();
	prn_data_win();
	gbarf(xw_min-2,yw_min-2,xw_max+2,yw_max+2,_cg_info,_BLACK);	/* 関数域 */
}

/* 座標スケールを描く */
void write_scale(void)
{
	set_param();

	gview(xw_min,yw_min,xw_max,yw_max,-1,-1);
	switch(sw_scale)
	{	/* 座標 */
		case	1	:	/* 無し */
			break;
		case	2	:	/* 全面 */
			write_plane_scale();
			break;
		case	3	:	/* 周りだけ */
			write_boarder_scale();
			break;
		default	:	break;
	}
	switch(sw_0)
	{	/* 原点座標 */
		case	1	:	/* 無し */
			break;
		case	2	:	/* 直線 */
			write_0_line();
			break;
		case	3	:	/* 座標 */
			write_0_scale();
			break;
		case	4	:	/* 直線＆座標 */
			write_0_line();
			write_0_scale();
			break;
		default		:	break;
	}
	gview(0, 0, 639, 399,-1,-1);
	key_buf_cls();
}

/* 全面座標 */
void write_plane_scale(void)
{
	double i_start,i;

	/* ｘ座標を描く */
	modf(x_min/x_scale,&i_start);
	for(i = i_start - 2 ;; i++){
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		if(i*x_scale > x_max) break;
		if(i*x_scale < x_min) continue;
		gline((i*x_scale-x_min)*x_ratio+xw_min, yw_min,
			  (i*x_scale-x_min)*x_ratio+xw_min, yw_max,
			  _c_gr_scale);
		if(*_km_m0 & 0x01) return;
	}
	/* ｙ座標を描く */
	modf(y_min/y_scale,&i_start);
	for(i = i_start - 2 ;; i++){
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		if(i*y_scale > y_max) break;
		if(i*y_scale < y_min) continue;
		gline(xw_min, yw_max - (i*y_scale-y_min)*y_ratio,
			  xw_max, yw_max - (i*y_scale-y_min)*y_ratio,
			  _c_gr_scale);
		if(*_km_m0 & 0x01) return;
	}
}

/* 周りだけ座標 */
void write_boarder_scale(void)
{
	double i_start,i;

	/* ｘ座標を描く */
	modf(x_min/x_scale,&i_start);
	for(i = i_start - 2 ;; i++){
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		if(i*x_scale > x_max) break;
		if(i*x_scale < x_min) continue;
		gline((i*x_scale-x_min)*x_ratio+xw_min, yw_min,
			  (i*x_scale-x_min)*x_ratio+xw_min, yw_min+4,
			  _c_gr_scale);
		gline((i*x_scale-x_min)*x_ratio+xw_min, yw_max,
			  (i*x_scale-x_min)*x_ratio+xw_min, yw_max-4,
			  _c_gr_scale);
		if(*_km_m0 & 0x01) return;
	}

	/* ｙ座標を描く */
	modf(y_min/y_scale,&i_start);
	for(i = i_start - 2 ;; i++){
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		if(i*y_scale > y_max) break;
		if(i*y_scale < y_min) continue;
		gline(xw_min, yw_max - (i*y_scale-y_min)*y_ratio,
			  xw_min+4, yw_max - (i*y_scale-y_min)*y_ratio,
			  _c_gr_scale);
		gline(xw_max, yw_max - (i*y_scale-y_min)*y_ratio,
			  xw_max-4, yw_max - (i*y_scale-y_min)*y_ratio,
			  _c_gr_scale);
		if(*_km_m0 & 0x01) return;
	}
}

/* 原点座標を直線で */
void write_0_line(void)
{
	if(x_min > 0 || x_max < 0) ;
	else
	{	/* ｘ＝０の座標、縦線 */
		gline((0-x_min)*x_ratio+xw_min, yw_min,
			  (0-x_min)*x_ratio+xw_min, yw_max,
			  _c_gr_0);
	}
	if(y_min > 0 || y_max < 0) ;
	else
	{	/* ｙ＝０の座標、横線 */
		gline(xw_min, yw_max - (0-y_min)*y_ratio,
			  xw_max, yw_max - (0-y_min)*y_ratio,
			  _c_gr_0);
	}
}

/* 原点座標をドットで */
void write_0_scale(void)
{
	double i_start,i;

	if(x_min > 0 || x_max < 0) ;
	else
	{	/* ｘ＝０の座標、縦 */
		modf(y_min/y_scale_0,&i_start);
		for(i = i_start - 2 ;; i++){
			if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
			if(i*y_scale_0 > y_max) break;
			if(i*y_scale_0 < y_min) continue;
			gline((0-x_min)*x_ratio+xw_min - 2, yw_max - (i*y_scale_0-y_min)*y_ratio,
				  (0-x_min)*x_ratio+xw_min + 2, yw_max - (i*y_scale_0-y_min)*y_ratio,
				  _c_gr_0);
			if(*_km_m0 & 0x01) return;
		}
	}
	if(y_min > 0 || y_max < 0) ;
	else
	{	/* ｙ＝０の座標、横 */
		modf(x_min/x_scale_0,&i_start);
		for(i = i_start - 2 ;; i++){
			if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
			if(i*x_scale_0 > x_max) break;
			if(i*x_scale_0 < x_min) continue;
			gline((i*x_scale_0-x_min)*x_ratio+xw_min,  yw_max - (0-y_min)*y_ratio - 2,
				  (i*x_scale_0-x_min)*x_ratio+xw_min,  yw_max - (0-y_min)*y_ratio + 2,
				  _c_gr_0);
			if(*_km_m0 & 0x01) return;
		}
	}
}


/* 各変数より重要なパラメータを計算 */
void set_param(void)
{
	if(x_max == x_min || y_max == y_min)
	{
		x_max = x_min + 0.000001;
		y_max = y_min + 0.000001;
	}

	/* 画面座標と実数座標の比 */
	x_ratio = (double)Xwidth / (double)(x_max - x_min);
	y_ratio = (double)Ywidth / (double)(y_max - y_min);

}


void init_GMNU(void)
{
	w1.left = 4;
	w1.top = 3;
	w1.data_len = 18;
	w1.data_max = 10;
	w1.color = _c_mnu;
	w1.select_color = _c_mnuv;
	w1.boarder_color = _c_box;
	w1.mode = W_FWRITE;
	w1.save = W_GSV_TXT | W_GSV_GR;
	w1.right_left = W_ON;
	w1.cur_topic = 1;

	w2.left = 14;
	w2.top = 3;
	w2.data_len = 18;
	w2.data_max = 3;
	w2.color = _c_mnu;
	w2.select_color = _c_mnuv;
	w2.boarder_color = _c_box;
	w2.mode = W_FWRITE;
	w2.save = W_GSV_TXT | W_GSV_GR;
	w2.right_left = W_ON;
	w2.cur_topic = 1;

	w3.left = 25;
	w3.top = 3;
	w3.data_len = 18;
	w3.data_max = 5;
	w3.color = _c_mnu;
	w3.select_color = _c_mnuv;
	w3.boarder_color = _c_box;
	w3.mode = W_FWRITE;
	w3.save = W_GSV_TXT | W_GSV_GR;
	w3.right_left = W_ON;
	w3.cur_topic = 1;

	w4.left = 34;
	w4.top = 3;
	w4.data_len = 18;
	w4.data_max = 15;
	w4.color = _c_mnu;
	w4.select_color = _c_mnuv;
	w4.boarder_color = _c_box;
	w4.mode = W_FWRITE;
	w4.save = W_GSV_TXT | W_GSV_GR;
	w4.right_left = W_ON;
	w4.cur_topic = 1;



	w_bar.data_max = 4;
	w_bar.mfunc = bar_mnu_func;
	w_bar.color = _c_barb;
	w_bar.topic_color = _c_bar;
	w_bar.select_color = _c_barv;
	w_bar.cur_topic = 1;

	w_info.left = 20;
	w_info.top = 10;
	w_info.data_len = 40;
	w_info.data_max = 8;
	w_info.color = _c_mnu;
	w_info.select_color = _c_mnuv;
	w_info.boarder_color = _c_box;
	w_info.mode = W_FWRITE;
	w_info.save = W_GSV_TXT | W_GSV_GR;
	w_info.right_left = W_ON;
	w_info.cur_topic = 1;

	wi_1s.left = 37;
	wi_1s.top = 8;
	wi_1s.data_len = 30;
	wi_1s.data_max = 1;
	wi_1s.color = _c_mnu;
	wi_1s.select_color = _c_mnuv;
	wi_1s.boarder_color = _c_box;
	wi_1s.mode = W_FWRITE;
	wi_1s.save = W_GSV_TXT | W_GSV_GR;
	wi_1s.right_left = W_OFF;
	wi_1s.cur_topic = 1;

	wi_3s.left = 37;
	wi_3s.top = 8;
	wi_3s.data_len = 30;
	wi_3s.data_max = 3;
	wi_3s.color = _c_mnu;
	wi_3s.select_color = _c_mnuv;
	wi_3s.boarder_color = _c_box;
	wi_3s.mode = W_FWRITE;
	wi_3s.save = W_GSV_TXT | W_GSV_GR;
	wi_3s.right_left = W_OFF;
	wi_3s.cur_topic = 1;

	wi_4s.left = 37;
	wi_4s.top = 8;
	wi_4s.data_len = 30;
	wi_4s.data_max = 4;
	wi_4s.color = _c_mnu;
	wi_4s.select_color = _c_mnuv;
	wi_4s.boarder_color = _c_box;
	wi_4s.mode = W_FWRITE;
	wi_4s.save = W_GSV_TXT | W_GSV_GR;
	wi_4s.right_left = W_OFF;
	wi_4s.cur_topic = 1;

	wi_1l.left = 3;
	wi_1l.top = 10;
	wi_1l.data_len = 50;
	wi_1l.data_max = 1;
	wi_1l.color = _c_mnu;
	wi_1l.select_color = _c_mnuv;
	wi_1l.boarder_color = _c_box;
	wi_1l.mode = W_FWRITE;
	wi_1l.save = W_GSV_TXT | W_GSV_GR;
	wi_1l.right_left = W_OFF;
	wi_1l.cur_topic = 1;

	wi_2l.left = 3;
	wi_2l.top = 10;
	wi_2l.data_len = 50;
	wi_2l.data_max = 2;
	wi_2l.color = _c_mnu;
	wi_2l.select_color = _c_mnuv;
	wi_2l.boarder_color = _c_box;
	wi_2l.mode = W_FWRITE;
	wi_2l.save = W_GSV_TXT | W_GSV_GR;
	wi_2l.right_left = W_OFF;
	wi_2l.cur_topic = 1;

	w_sw.left = 37;
	w_sw.top = 8;
	w_sw.data_len = 20;
	w_sw.data_max = 3;
	w_sw.color = _c_mnu;
	w_sw.select_color = _c_mnuv;
	w_sw.boarder_color = _c_box;
	w_sw.mode = W_FWRITE;
	w_sw.save = W_GSV_TXT | W_GSV_GR;
	w_sw.right_left = W_OFF;
	w_sw.cur_topic = 1;

	x_min = -5.0;			/* 画面の範囲 */
	x_max = 5.0;
	y_min = -5.0;
	y_max = 5.0;

	xl_min = x_min;		/* ボーダーの設定 */
	xl_max = x_max;
	yl_min = y_min;
	yl_max = y_max;

	tl_min = 0.0;			/* ｘ，ｙ＝ｆ(ｔ)のｔの間隔設定 */
	tl_max = 3.1415926 * 2.0;
	t_delta = 0.02;

	x_scale = 1.0;		/* 座標間隔 */
	y_scale = 1.0;
	x_scale_0 = 0.2;
	y_scale_0 = 0.2;

	cur_palette = 1;
	maxcolor = 15;
	maxloop = 40;
	_txt_jinp_bptone = 1200;

	_hc_sX = 1;		/* ハードコピーのサイズ */
	_hc_sY = 1;
	strcpy(_hc_sC,"0111111101111111");

	strcpy(formula_string,"");	/* 数式 */
	strcpy(formula_str_if,"");
	strcpy(formula_str_x,"");
	strcpy(formula_str_y,"");
	strcpy(formula_str_dfx,"");

	sw_scale = 3;		/* スイッチの規定値 */
	sw_0 = 3;
	sw_gr_line = 2;
	sw_if = 1;
	sw_bod = 1;
	sw_initmode = 1;

	set_param();

	strcpy(filename,"");

}

void prn_data_win(void)
{
	/* インフォメーションエリアを描く */
	gsetpattern(32,_infopat);
	gbarp((xtw_min-1)*8-2,(ytw_min-1)*16-2,xtw_max*8+2,ytw_max*16+2,
			_cg_info);
	/* インフォメーションエリア　テキストウインドウ */
	window(xtw_min,ytw_min,xtw_max,ytw_max);
	textattr(_c_info);
	clrscr();

	gotoxy(1,1);
	cprintf("X min  : % -10.5g",x_min);
	gotoxy(1,2);
	cprintf("X max  : % -10.5g",x_max);
	gotoxy(1,3);
	cprintf("Y min  : % -10.5g",y_min);
	gotoxy(1,4);
	cprintf("Y max  : % -10.5g",y_max);


	gotoxy(1,6);
	cprintf("座標1 X: %-10.5g",x_scale);
	gotoxy(1,7);
	cprintf("座標1 Y: %-10.5g",y_scale);
	gotoxy(1,8);
	cprintf("座標2 X: %-10.5g",x_scale_0);
	gotoxy(1,9);
	cprintf("座標2 Y: %-10.5g",y_scale_0);

	gotoxy(1,11);
	cprintf("補完 : %1s 初期化 : %d",(sw_gr_line==2) ? "O":"X",sw_initmode);
	gotoxy(1,12);
	cprintf("座標 : %d ０座標 : %d",sw_scale-1,sw_0);
	gotoxy(1,13);
	cprintf("条件 : %1s ﾎﾞｰﾀﾞ  : %1s",(sw_if==1)? "X":"O",(sw_bod==1)? "X":"O");


}
