/* MAN_PARx.CPP
	MANDEL.EXE 用　初期設定プログラム */
#include<conio.h>
#include<string.h>
#include"mandel2.h"
#include<hdcpy.h>
#include<txtl.h>
#include<glio.h>

long xmin,xmax,ymin,ymax;				// man DSP
long double dxmin,dxmax,dymin,dymax;			// man C
int maxloop,maxcolor,f_mode,cur_palette;	// Glob
long double x_cen,y_cen,xy_scale;				// Glob
long double dxratio,dyratio;					// Glob
int _c_mnu,_c_mnuv,_c_box,_c_bar,_c_barv,_c_barb,_c_info;
int _cg_info,_cg_man;

char far _bpat[] = {0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,0x00};
char far _infopat[] = {0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0xff,
			 0x00,0x00,0x00,0x00,0x00};


void set_param(void)
{
	long PmX,PmY,PmR;

	// const

	// DSP param
	PmX = x_cen *  5000;
	PmY = y_cen *  5000;
	PmR = xy_scale * 5000;

	// DSP long
	xmin = PmX - PmR / 2;
	xmax = PmX + PmR / 2;
	ymax = PmY + PmR / 2;
	ymin = PmY - PmR / 2;

	// double
	dxmin = (long double)x_cen - (long double)xy_scale / (long double)2.0;
	dxmax = (long double)x_cen + (long double)xy_scale / (long double)2.0;
	dymax = (long double)y_cen + (long double)xy_scale / (long double)2.0;
	dymin = (long double)y_cen - (long double)xy_scale / (long double)2.0;

	if(dxmax == dxmin || dymax == dymin)
	{
		dxmax = dxmin + 1.0e-10;
		dymax = dymin + 1.0e-10;
	}

	// double
	dxratio = (long double)Xwidth / (long double)(dxmax - dxmin);
	dyratio = (long double)Ywidth / (long double)(dymax - dymin);

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
	cprintf("実数 中心値\r\n");		// 1
	gotoxy(1,2);
	cprintf(" % -18.13Lg\r\n",x_cen);	// 2
	gotoxy(1,3);
	cprintf("虚数 中心値 \r\n");		// 3
	gotoxy(1,4);
	cprintf(" % -18.13Lg\r\n",y_cen);	// 4
	gotoxy(1,5);
	cprintf("表示範囲 \r\n");		// 5
	gotoxy(1,6);
	cprintf("  %-18.13Lg\r\n",xy_scale);	// 6
	gotoxy(1,7);
	cprintf("簡略化 %-2d ",f_mode);	// 7

	gotoxy(1,8);
	cprintf("    %% 完了 ");			// 8

	gotoxy(1,10);
	cprintf("実数: ");				// 10
	gotoxy(1,12);
	cprintf("虚数: ");				// 11
	gotoxy(1,14);
	cprintf("処理諧調 %-02d",maxcolor);
	gotoxy(1,15);
	cprintf("処理ﾚﾍﾞﾙ %-3d ",maxloop);	// 13

}


void init_GMNU(void)
{
	long PmX,PmY,PmR;

	w1.left = 5;
	w1.top = 3;
	w1.data_len = 18;
	w1.data_max = 9;
	w1.color = _c_mnu;
	w1.select_color = _c_mnuv;
	w1.boarder_color = _c_box;
	w1.mode = W_FWRITE;
	w1.save = W_GSV_TXT | W_GSV_GR;
	w1.right_left = W_ON;
	w1.cur_topic = 1;

	w2.left = 17;
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

	w3.left = 30;
	w3.top = 3;
	w3.data_len = 18;
	w3.data_max = 6;
	w3.color = _c_mnu;
	w3.select_color = _c_mnuv;
	w3.boarder_color = _c_box;
	w3.mode = W_FWRITE;
	w3.save = W_GSV_TXT | W_GSV_GR;
	w3.right_left = W_ON;
	w3.cur_topic = 1;

	w4.left = 37;
	w4.top = 8;
	w4.data_len = 30;
	w4.data_max = 3;
	w4.color = _c_mnu;
	w4.select_color = _c_mnuv;
	w4.boarder_color = _c_box;
	w4.mode = W_FWRITE;
	w4.save = W_GSV_TXT | W_GSV_GR;
	w4.right_left = W_OFF;
	w4.cur_topic = 1;

	wi.left = 35;
	wi.top = 9;
	wi.data_len = 30;
	wi.data_max = 1;
	wi.color = _c_mnu;
	wi.select_color = _c_mnuv;
	wi.boarder_color = _c_box;
	wi.mode = W_FWRITE;
	wi.save = W_GSV_TXT | W_GSV_GR;
	wi.right_left = W_OFF;
	wi.cur_topic = 1;

	w_pal.left = 38;
	w_pal.top = 8;
	w_pal.data_len = 20;
	w_pal.data_max = 4;
	w_pal.color = _c_mnu;
	w_pal.select_color = _c_mnuv;
	w_pal.boarder_color = _c_box;
	w_pal.mode = W_FWRITE;
	w_pal.save = W_GSV_TXT | W_GSV_GR;
	w_pal.right_left = W_OFF;
	w_pal.cur_topic = 1;

	w_sv.left = 8;
	w_sv.top = 8;
	w_sv.data_len = 18;
	w_sv.data_max = 2;
	w_sv.color = _c_mnu;
	w_sv.select_color = _c_mnuv;
	w_sv.boarder_color = _c_box;
	w_sv.mode = W_FWRITE;
	w_sv.save = W_GSV_TXT | W_GSV_GR;
	w_sv.right_left = W_OFF;
	w_sv.cur_topic = 1;

	w_bar.data_max = 3;
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

	maxcolor = 15;
	maxloop = 40;
	x_cen = -0.8;
	y_cen = 0;
	xy_scale = 2.6;
	cur_palette = 1;
	f_mode = 1;

	set_param();
	_hc_sX = 1;		// 2*2
	_hc_sY = 1;
	_txt_jinp_bptone = 1200;
	strcpy(_hc_sC,"0101010101010101");

	strcpy(filename,"");

}
