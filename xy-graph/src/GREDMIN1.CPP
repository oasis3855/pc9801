/* GREDMINx.CPP
   GEDIT.EXE グラフィックエディター　描画　プログラム */
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include<glio.h>
#include<etc.h>
#include<gbios.h>

extern struct REGPACK r;
extern unsigned int title_bar,rev_sw,fore_color,fill_color,back_color,char_mode;
extern void until_mouse_wait(void);

void mouse_locate_boarder(int x1, int y1, int x2, int y2, int& cx, int& cy);
void write_line(void);
void write_box(int mode);
void write_free(int mode);

void mouse_locate_boarder(int x1, int y1, int x2, int y2, int& cx, int& cy)
{
	if(cx < x1 || cx > x2 || cy < y1 || cy > y2)
	{
		if(cx < x1) cx = x1;
		else if(cx > x2) cx = x2;
		if(cy < y1) cy = y1;
		else if(cy > y2) cy = y2;
		_DX = cy;
		_CX = cx;
		_AX = 0x0004;
		geninterrupt(0x33);
	}
	return;
}


void write_line(void)
{
	int click=0,old_x=0,old_y=0,old_x2,old_y2;
	int col,m_x,m_y;

	gline(1,399,2,399,_GREEN);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);
	_AX = 0x01;		// cursor ON
	geninterrupt(0x33);

	for(;;)
	{
		if(kbhit() && getch() == 0x1b) // ESC
		{
			_AX = 0x02;		// cursor OFF
			geninterrupt(0x33);
			if(click) gbios_line(old_x,old_y,m_x,m_y);
			return;
		}

		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);
		m_x = r.r_cx;		// 0 - 639
		m_y = r.r_dx;		// 0 - 399
		// only in window
		mouse_locate_boarder(0,0,639,399,m_x,m_y);
		if(click)
		{
			if(m_x == old_x2 && m_y == old_y2) ;	/* カーソルが動かなかった */
			else
			{
				_AX = 0x02;		// cursor OFF
				geninterrupt(0x33);
				gbios_line(old_x,old_y,old_x2,old_y2);
				old_x2 = m_x;
				old_y2 = m_y;
				gbios_line(old_x,old_y,m_x,m_y);
				_AX = 0x01;		// cursor ON
				geninterrupt(0x33);
			}
		}
		if(r.r_bx & 0x01)	// if LIGHT BUTTON
		{
			if(click == 0)
			{
				// first push
				old_x = m_x;
				old_y = m_y;
				old_x2 = old_x;
				old_y2 = old_y;
				click = 1;
				until_mouse_wait();
			}
			else
			{
				r.r_ax = 0x02;		// cursor OFF
				intr(0x33,&r);

				gbios_line(old_x,old_y,m_x,m_y);

				gline(old_x,old_y,m_x,m_y,fore_color);
				click = 0;
				until_mouse_wait();

				r.r_ax = 0x01;		// cursor ON
				intr(0x33,&r);

				return;
			}
		}
	}
}

void write_box(int mode)
{
	int click=0,old_x=0,old_y=0,old_x2,old_y2;
	int col,m_x,m_y,cenx,ceny,radx,rady;

	gline(1,399,2,399,_GREEN);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);
	_AX = 0x01;		// cursor ON
	geninterrupt(0x33);

	for(;;)
	{
		if(kbhit() && getch() == 0x1b) // ESC
		{
			_AX = 0x02;		// cursor OFF
			geninterrupt(0x33);
			if(click) gbios_rect(old_x,old_y,m_x,m_y);
			return;
		}

		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);
		m_x = r.r_cx;		// 0 - 639
		m_y = r.r_dx;		// 0 - 399
		// only in window
		mouse_locate_boarder(0,0,639,399,m_x,m_y);
		if(click)
		{
			if(m_x == old_x2 && m_y == old_y2) ;	/* カーソルが動かなかった */
			else
			{
				_AX = 0x02;		// cursor OFF
				geninterrupt(0x33);
				gbios_rect(old_x,old_y,old_x2,old_y2);
				old_x2 = m_x;
				old_y2 = m_y;
				gbios_rect(old_x,old_y,m_x,m_y);
				_AX = 0x01;		// cursor ON
				geninterrupt(0x33);
			}
		}
		if(r.r_bx & 0x01)	// if LIGHT BUTTON
		{
			if(click == 0)
			{
				// first push
				old_x = m_x;
				old_y = m_y;
				old_x2 = old_x;
				old_y2 = old_y;
				click = 1;
				until_mouse_wait();
			}
			else
			{
				r.r_ax = 0x02;		// cursor OFF
				intr(0x33,&r);

				gbios_rect(old_x,old_y,m_x,m_y);

				/* for circle */
				if(old_x > m_x) cenx = (old_x + m_x)/2;
				else cenx = (m_x + old_x)/2;
				if(old_y > m_y) ceny = (old_y + m_y)/2;
				else ceny = (m_y + old_y)/2;

				if(old_x > m_x) radx = (old_x - m_x)/2;
				else radx = (m_x - old_x)/2;
				if(old_y > m_y) rady = (old_y - m_y)/2;
				else rady = (m_y - old_y)/2;

				switch(mode)
				{
					case	0:	gbox(old_x,old_y,m_x,m_y,fore_color);break;
					case	1:	gbarf(old_x,old_y,m_x,m_y,fore_color,fill_color);break;
					case	2:	garc(cenx,ceny,radx,rady,fore_color,0,360);break;
					case	3:	garcf(cenx,ceny,radx,rady,fore_color,fill_color,0,360);break;
				}
				click = 0;
				until_mouse_wait();

				r.r_ax = 0x01;		// cursor ON
				intr(0x33,&r);

				return;
			}
		}
	}
}

void write_free(int mode)
{
	int col,m_x,m_y;

	gline(1,399,2,399,_GREEN);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);
	_AX = 0x01;		// cursor ON
	geninterrupt(0x33);

	for(;;)
	{
		if(kbhit() && getch() == 0x1b) // ESC
		{
			_AX = 0x02;		// cursor OFF
			geninterrupt(0x33);
			return;
		}

		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);
		m_x = r.r_cx;		// 0 - 639
		m_y = r.r_dx;		// 0 - 399
		// only in window
		mouse_locate_boarder(0,0,639,399,m_x,m_y);
		if(r.r_bx & 0x01)	// if LIGHT BUTTON
		{
				r.r_ax = 0x02;		// cursor OFF
				intr(0x33,&r);

				switch(mode)
				{
					case	0:	gcirclef(m_x,m_y,5,fore_color,fore_color);break;
					case	1:	gcirclef(m_x,m_y,1,fore_color,fore_color);break;
				}

				r.r_ax = 0x01;		// cursor ON
				intr(0x33,&r);
		}
	}
}
