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

	r.r_ax = 0x00;		// check mouse driver
	intr(0x33,&r);
	if(r.r_ax == 0) return;

	r.r_ax = 0x0f;		// set m/s
	r.r_cx = 5;
	r.r_dx = 5;
	intr(0x33,&r);

	r.r_ax = 0x01;		// cursor ON
	intr(0x33,&r);

	window(xtw_min,ytw_min,xtw_max,ytw_max);
	textattr(_c_info);


	for(;;)
	{
		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);
		m_x = r.r_cx;		// 0 - 639
		m_y = r.r_dx;		// 0 - 399
		// only in window
		if(m_x < xw_min || m_x > xw_max || m_y < yw_min || m_y > yw_max)
		{
			if(m_x < xw_min) m_x = xw_min;
			else if(m_x > xw_max) m_x = xw_max;
			if(m_y < yw_min) m_y = yw_min;
			else if(m_y > yw_max) m_y = yw_max;
			_DX = m_y;
			_CX = m_x;
			_AX = 0x0004;
			geninterrupt(0x33);
		}
		// real x-y co-ordinate
		c_x = ((long double)m_x - (long double)xw_min)/dxratio + dxmin;
		c_y = ((long double)yw_max - (long double)m_y)/dyratio + dymin;

		gotoxy(2,11);
		cprintf("% -17.12Le",c_x);
		gotoxy(2,13);
		cprintf("% -17.12Le",c_y);
		if(r.r_bx & 0x01)	// if LIGHT BUTTON
		{
			x_cen = (long double)c_x;
			y_cen = (long double)c_y;
			prn_data_win();
			break;
		}



		_AX = 0x0400;		// check key sense
		geninterrupt(0x18);
		col = (int)_AH;
		if(col & 0x1) break;

	}

	r.r_ax = 0x02;		// cursor OFF
	intr(0x33,&r);

	prn_data_win();

	return;

}
