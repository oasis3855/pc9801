/* Turbo C++ CONIO.H Color  ->  Turbo C CONIO.H Color    *
 *  Convert Program for Normal mode PC-9801              *
 *  (c) 1991 Hirokazu Inoue                              */
// c++ module
#include "win_i1.h"

unsigned char _col_convert(int att);

unsigned char _col_convert(int att)
{
	unsigned int t_color,t_blink,v_color,v_blink;
	unsigned char attrib;

	t_color = att & 0x0007;
	t_blink = att & 0x0f00;
	v_color = (t_color & 0x04)|((t_color & 0x02)<<2)|((t_color & 0x01)<<1);
	v_blink = (t_blink >> 7) | 0x0001;
	attrib = (char)(v_color << 4)|(v_blink);

	return(attrib);	/* This is Text-VRAM Attribute */
}