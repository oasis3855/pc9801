/* XYGRMUSx.CPP
   XYGRPH.EXE 用　マウス　プログラム */
#include<stdlib.h>
#include<dos.h>
#include<conio.h>
#include<etc.h>
#include<glio.h>
#include<gbios.h>
#include"xygr.h"

/* マウスボタンが離されるのを待つ */
void until_mouse_wait(void)
{
	static struct REGPACK r;
	do
	{
		r.r_ax = 0x03;		/* x,y 座標, ボタンを読む */
		intr(0x33,&r);

	}while(r.r_bx & 0x01);
}

/* マウスで座標を決定 */
void do_mouse_plot(void)
{
	struct REGPACK r;
	int click=0,old_x=0,old_y=0,old_x2,old_y2;
	int m_x,m_y;	/* 現在のマウスの位置 */
	double c_x,c_y,c_x2,c_y2;

	r.r_ax = 0x00;		/* マウスドライバー確認 */
	intr(0x33,&r);
	if(r.r_ax == 0) return;	/* マウスドライバー無し */

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

	gline(1,399,2,399,_GRAY);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);	/* GBIOS 初期化 */

	/* 座標表示のためのテキストウインドウ */
	window(xtw_min,ytw_min,xtw_max,ytw_max);
	textattr(_c_info);


	for(;;)
	{
		if(kbhit() && getch() == 0x1b) /* ESCキーチェック */
		{
			_AX = 0x02;		/* マウスカーソル OFF */
			geninterrupt(0x33);
			if(click) gbios_rect(old_x,old_y,m_x,m_y);
			break;	/* マウス終了 */
		}

		r.r_ax = 0x03;		/* x,y 座標, ボタンを読む */
		intr(0x33,&r);
		m_x = r.r_cx;
		m_y = r.r_dx;

		/* 実数座標に変換 */
		c_x = ((double)m_x - (double)xw_min)/x_ratio + x_min;
		c_y = ((double)yw_max - (double)m_y)/y_ratio + y_min;
		/* 座標値を表示 */
		gotoxy(1,14);
		cprintf("X : % -11.5g  ",c_x);
		gotoxy(1,15);
		cprintf("Y : % -11.5g  ",c_y);

		if(click)
		{	/* 長方形を描く */
			if(m_x == old_x2 && m_y == old_y2) ;	/* カーソルが動かなかった */
			else
			{
				_AX = 0x02;		/* マウスカーソル OFF */
				geninterrupt(0x33);
				gbios_rect(old_x,old_y,old_x2,old_y2);	/* 前の長方形を消去 */
				old_x2 = m_x;
				old_y2 = m_y;
				gbios_rect(old_x,old_y,m_x,m_y);	/* 新たな長方形を描く */
				_AX = 0x01;		/* マウスカーソル ON */
				geninterrupt(0x33);
			}
		}
		if(r.r_bx & 0x01)	/* ボタンが押されたら */
		{
			if(click == 0)
			{	/* １回目のボタン */
				old_x = m_x;
				old_y = m_y;
				old_x2 = m_x;
				old_y2 = m_y;
				click = 1;
				until_mouse_wait();
			}
			else
			{	/* ２回目のボタン */
				r.r_ax = 0x02;		/* マウスカーソル OFF */
				intr(0x33,&r);

				gbios_rect(old_x,old_y,m_x,m_y);	/* 前の長方形を消去 */

				/* 実数座標に変換 */
				c_x = ((double)m_x - (double)xw_min)/x_ratio + x_min;
				c_y = ((double)yw_max - (double)m_y)/y_ratio + y_min;
				c_x2 = ((double)old_x - (double)xw_min)/x_ratio + x_min;
				c_y2 = ((double)yw_max - (double)old_y)/y_ratio + y_min;

				/* 新しい座標の設定 */
				x_min = min(c_x,c_x2);
				x_max = max(c_x,c_x2);
				y_min = min(c_y,c_y2);
				y_max = max(c_y,c_y2);

				click = 0;
				until_mouse_wait();
				break;	/* マウス終了 */
			}
		}
	}
	gbios_setmode(0,GBIOS_COPY,GBIOS_GREEN,0xffff);
	set_param();
	prn_data_win();
	return;
}
