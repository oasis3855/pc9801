/* MAN_MUSx.CPP
   MANDEL.EXE 用　マウス　プログラム */
#include<dos.h>
#include<conio.h>
#include<etc.h>
#include"mandel2.h"


void do_mouse_plot(void)
{
	struct REGPACK r;
	int col,m_x,m_y;
	long double c_x,c_y;

	r.r_ax = 0x00;		/* マウスドライバー確認 */
	intr(0x33,&r);
	if(r.r_ax == 0) return;

	r.r_ax = 255;		/* カーソル書き込み面設定 */
	r.r_bx = 0x0f;
	if(cur_palette == 2 || cur_palette == 3) r.r_bx = 0x01;
	intr(0x33,&r);

	r.r_ax = 0x0f;		/* ミッキー設定 */
	r.r_cx = 5;
	r.r_dx = 5;
	intr(0x33,&r);

	r.r_ax = 0x07;		/* 水平方向カーソル移動範囲 */
	r.r_cx = xw_min;
	r.r_dx = xw_max;
	intr(0x33,&r);

	r.r_ax = 0x08;		/* 垂直方向カーソル移動範囲 */
	r.r_cx = yw_min;
	r.r_dx = yw_max;
	intr(0x33,&r);

	r.r_ax = 0x01;		/* マウスカーソル ON */
	intr(0x33,&r);

	r.r_ax = 0x04;		/* x y 座標設定 */
	r.r_cx = (xw_max+xw_min)/2;
	r.r_dx = (yw_max+yw_min)/2;
	intr(0x33,&r);

	window(xtw_min,ytw_min,xtw_max,ytw_max);
	textattr(_c_info);


	for(;;)
	{
		r.r_ax = 0x03;		/* ボタンの状態と位置の取得 */
		intr(0x33,&r);
		m_x = r.r_cx;		/* ｘ */
		m_y = r.r_dx;		/* ｙ */
		/* 実数に割当 */
		c_x = ((long double)m_x - (long double)xw_min)/dxratio + dxmin;
		c_y = ((long double)yw_max - (long double)m_y)/dyratio + dymin;

		gotoxy(2,11);
		cprintf("% -17.12Le",c_x);
		gotoxy(2,13);
		cprintf("% -17.12Le",c_y);
		if(r.r_bx & 0x01)	/* 左ボタンが押されたら */
		{
			x_cen = (long double)c_x;
			y_cen = (long double)c_y;
			prn_data_win();
			break;
		}



		_AX = 0x0400;		/* ESC キーチェック */
		geninterrupt(0x18);
		if(_AH & 0x01) break;

	}

	r.r_ax = 0x02;		/* マウスカーソル OFF */
	intr(0x33,&r);

	key_buf_cls();

	prn_data_win();

	return;

}
