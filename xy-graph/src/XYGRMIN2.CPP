/* XYGRMINx.CPP
   XYGRPH.EXE 用　計算／表示 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<process.h>
#include<math.h>
#include<conio.h>
#include<etc.h>
#include<glio.h>
#include<gr.h>
#include<exmath.h>
#include"xygr.h"

/* 浮動小数点エラーへの対応 */
int matherr(struct exception *e)
{
	asm	fninit;
	math_err_flag = 1;
	e->retval = 0.0;
	return(1);
}

/* グラフィックパレットの変更 */
void chg_palette(void)
{
	int i;

	switch(cur_palette)
	{
		case	1	:	/* 16色 */
			PC98_BGI_16color_palette_init();
			break;
		case	2	:	/* 緑１６諧調 */
			for(i=0;i<=15;i++)
			{
				color_palette_16(15-i,i,0,0);
			}
			color_palette_16(0,0,0,0);
			break;
		case	3	:	/* モノクロ */
			for(i=0;i<=15;i++)
			{
				color_palette_16(15-i,i,i,i);
			}
			color_palette_16(0,0,0,0);
			break;
		case	4	:	/* プレビュー */
			for(i=0;i<15;i++) color_palette_16(i+1,12,12,12);
			color_palette_16(0, 0,0,0);
			color_palette_16(8, 0,0,0);
			break;
		default	:
			break;
	}
	return;
}

/* 文を解析して値を返す／入力メニュー用 ,parser版atof*/
double calc_str(char *str)
{
	int dummy;
	static double variables[30];	/* パーサ用の変数 */
	double ret;

	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	ret = parser(str,&dummy,variables,26);
	if(dummy == 0) return 0.0;	/* 式のエラー */

	return ret;
}


/* グラフを描く */
void write_graph(int mode)
{
	if(sw_initmode == 1 || sw_initmode == 3) write_window();
	if(sw_initmode == 1 || sw_initmode == 2) write_scale();

	switch(mode)
	{
		case	1	:	/* ｙ＝ｆ(ｘ) */
			write_graph_x();
			break;
		case	2	:	/* ｘ，ｙ＝ｆ(ｔ) */
			write_graph_t();
			break;
		case	3	:	/* ｙ＝dｆ(ｘ)/dｘ */
			write_graph_d();
			break;
		default		:
			break;
	}
	key_buf_cls();
}

/* ｙ＝ｆ(ｘ) のグラフを描く */
void write_graph_x(void)
{
	int i,dummy,sv_line;
	double variables[30];	/* パーサ用の変数 */
	int x;					/* CRT 上での座標 */
	double LogicX,LogicY;	/* 実際の実数値 */
	double old_LogicY;		/* 直線補完時の１つ前のｙ座標 */

	set_param();

	/* 式のエラーの検査 */
	for(i = 0;i <= 25;i++) variables[i] = 0;
	variables['X'-'A'] = x_min;
	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	parser(formula_string,&dummy,variables,26);
	if(dummy == 0) return;	/* 式のエラー */

	sv_line = 0;	/* 直線補完用 */
	math_err_flag = 0;	/* 浮動小数点フラグ */

	for(x=0; x<=Xwidth; x++)
	{
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		LogicX = (double)x / x_ratio + x_min;	/* 実際のｘ座標 */
		if(sw_bod == 2)
		{
			if(LogicX < xl_min || LogicX > xl_max)
			{	/* ﾎﾞｰﾀﾞの外 */
				sv_line = 0;
				continue;
			}
		}
		*(variables + 0x17) = LogicX;
		LogicY = parser(formula_string,&dummy,variables,26);
		if(LogicY > y_max || LogicY < y_min || math_err_flag)
		{
			sv_line = 0;
			math_err_flag = 0;	/* 浮動小数点エラー */
			continue;	/* ｳｲﾝﾄﾞｳ外 */
		}
		if(sw_bod == 2)
		{
			if(LogicY < yl_min || LogicY > yl_max)
			{	/* ﾎﾞｰﾀﾞの外 */
				sv_line = 0;
				continue;
			}
		}

		if(sw_if == 2)
		{	/* if 文の検査 */
			variables['Y'-'A'] = LogicY;
			if(parser(formula_str_if,&dummy,variables,26) <= 0 || math_err_flag)
			{	/* if 文が０以下だったら */
				sv_line = -1;
				math_err_flag = 0;	/* 浮動小数点エラー */
				continue;
			}
		}

		gpset(x+xw_min,yw_max-(LogicY - y_min)*y_ratio,_c_gr_grph);

		if(sw_gr_line == 2 && sv_line)
		{	/* 直線補完 */
			gline(x-1+xw_min, yw_max-(old_LogicY - y_min)*y_ratio,
				  x+xw_min, yw_max-(LogicY - y_min)*y_ratio,
				  _c_gr_grph);
			old_LogicY = LogicY;
			sv_line = 1;	/* フラグ */
		}
		else if(sw_gr_line == 2)
		{
			old_LogicY = LogicY;
			sv_line = 1;	/* フラグ */
		}


	}
	return ;
}

/* ｘ、ｙ＝ｆ(ｔ) のグラフを描く */
void write_graph_t(void)
{
	int i,dummy,sv_line;
	double variables[30];	/* パーサ用の変数 */
	double LogicX,LogicY,t;	/* 実際の実数値 */
	double old_LogicX,old_LogicY;		/* 直線補完時の１つ前の座標 */

	set_param();

	/* 式のエラーの検査 */
	for(i = 0;i < 26;i++) variables[i] = 0;
	variables['T'-'A'] = tl_min;
	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	parser(formula_str_x,&dummy,variables,26);
	if(dummy == 0) return;
	parser(formula_str_y,&dummy,variables,26);
	if(dummy == 0) return;

	sv_line = 0;	/* 直線補完用 */

	for(t = tl_min;t <= tl_max;t += t_delta){
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		variables['T'-'A'] = t;
		LogicX = parser(formula_str_x,&dummy,variables,26);
		if(LogicX < x_min || LogicX > x_max || math_err_flag)
		{	/* ウインドウの外 */
			sv_line = 0;
			math_err_flag = 0;	/* 浮動小数点エラー */
			continue;
		}
		if(sw_bod == 2)
		{
			if(LogicX < xl_min || LogicX > xl_max)
			{	/* ﾎﾞｰﾀﾞの外 */
				sv_line = 0;
				continue;
			}
		}

		LogicY = parser(formula_str_y,&dummy,variables,26);
		if(LogicY > y_max || LogicY < y_min || math_err_flag)
		{	/* ｳｲﾝﾄﾞｳ外 */
			sv_line = 0;
			math_err_flag = 0;	/* 浮動小数点エラー */
			continue;
		}
		if(sw_bod == 2)
		{
			if(LogicY < yl_min || LogicY > yl_max)
			{	/* ﾎﾞｰﾀﾞの外 */
				sv_line = 0;
				continue;
			}
		}

		if(sw_if == 2)
		{	/* if 文の検査 */
			variables['X'-'A'] = LogicX;
			variables['Y'-'A'] = LogicY;
			if(parser(formula_str_if,&dummy,variables,26) <= 0 || math_err_flag)
			{	/* if 文が０以下だったら */
				sv_line = -1;
				math_err_flag = 0;	/* 浮動小数点エラー */
				continue;
			}
		}

		gpset((LogicX-x_min)*x_ratio+xw_min,
			  yw_max-(LogicY - y_min)*y_ratio, _c_gr_grph);

		if(sw_gr_line == 2 && sv_line)
		{	/* 直線補完 */
			gline((old_LogicX-x_min)*x_ratio+xw_min, yw_max-(old_LogicY - y_min)*y_ratio,
				  (LogicX-x_min)*x_ratio+xw_min, yw_max-(LogicY - y_min)*y_ratio,
				  _c_gr_grph);
			old_LogicX = LogicX;
			old_LogicY = LogicY;
			sv_line = 1;	/* フラグ */
		}
		else if(sw_gr_line == 2)
		{
			old_LogicX = LogicX;
			old_LogicY = LogicY;
			sv_line = 1;	/* フラグ */
		}

	}
	return ;
}

/* ｙ＝dｆ(ｘ)/dｘ のグラフを描く */
void write_graph_d(void)
{
	int i,dummy,sv_line;
	double variables[30];	/* パーサ用の変数 */
	int x;					/* CRT 上での座標 */
	double LogicX,LogicY,LogicDY;	/* 実際の実数値 */
	double DeltaY;			/* 微分値 */
	double dx;				/* ｘの間隔 */
	double old_DeltaY;		/* 直線補完時の１つ前の座標 */

	set_param();

	/* 式のエラーの検査 */
	for(i = 0;i <= 25;i++) variables[i] = 0;
	variables['X'-'A'] = x_min;
	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	parser(formula_str_dfx,&dummy,variables,26);
	if(dummy == 0) return;	/* 式のエラー */

	sv_line = 0;	/* 直線補完用 */

	dx = 1.0 / x_ratio;		/* ドット間のｘの間隔 */
	if(dx <= 0.0) return;	/* わり算のエラーとなる */

	for(x=0; x<=Xwidth; x++)
	{
		if(kbhit() && getch() == 0x1b) return;	/* ESC チェック */
		LogicX = (double)x / x_ratio + x_min;	/* 実際のｘ座標 */
		if(sw_bod == 2)
		{
			if(LogicX < xl_min || LogicX > xl_max)
			{	/* ﾎﾞｰﾀﾞの外 */
				sv_line = 0;
				continue;
			}
		}
		*(variables + 0x17) = LogicX;
		LogicY = parser(formula_str_dfx,&dummy,variables,26);
		if(math_err_flag)
		{
			sv_line = 0;
			math_err_flag = 0;	/* 浮動小数点エラー */
			continue;
		}
		*(variables + 0x17) = LogicX + dx;
		LogicDY = parser(formula_str_dfx,&dummy,variables,26);
		if(math_err_flag)
		{
			sv_line = 0;
			math_err_flag = 0;	/* 浮動小数点エラー */
			continue;
		}
		DeltaY = (LogicDY - LogicY) / dx;
		if(DeltaY > y_max || DeltaY < y_min)
		{
			sv_line = 0;
			continue;	/* ｳｲﾝﾄﾞｳ外 */
		}
		if(sw_bod == 2)
		{
			if(DeltaY < yl_min || DeltaY > yl_max)
			{	/* ﾎﾞｰﾀﾞの外 */
				sv_line = 0;
				continue;
			}
		}

		if(sw_if == 2)
		{	/* if 文の検査 */
			variables['Y'-'A'] = LogicY;
			if(parser(formula_str_if,&dummy,variables,26) <= 0 || math_err_flag)
			{	/* if 文が０以下だったら */
				sv_line = -1;
				math_err_flag = 0;	/* 浮動小数点エラー */
				continue;
			}
		}

		gpset(x+xw_min,yw_max-(DeltaY - y_min)*y_ratio,_c_gr_grph);

		if(sw_gr_line == 2 && sv_line)
		{	/* 直線補完 */
			gline(x-1+xw_min, yw_max-(old_DeltaY - y_min)*y_ratio,
				  x+xw_min, yw_max-(DeltaY - y_min)*y_ratio,
				  _c_gr_grph);
			old_DeltaY = DeltaY;
			sv_line = 1;	/* フラグ */
		}
		else if(sw_gr_line == 2)
		{
			old_DeltaY = DeltaY;
			sv_line = 1;	/* フラグ */
		}
	}
	return ;
}
