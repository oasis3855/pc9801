/*
	W_WTITLE.C for WIN.LIB
		(C)opyright 1990 Hirokazu Inoue  All rights reserved .
		for Turbo C 2.0
	#include WIN.H .
*/

#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<stdlib.h>
#include"win_i1.h"

void win_wtitle(char *title,int rx,int box,WIN_INFO *winf)
{
	int rev=0,bl=0,col,cur_x,cur_y;
	unsigned char v_color;

	v_color = _col_convert(winf->color);

	cur_x = wherex();
	cur_y = wherey();

	switch(v_color & 0xf0){
		case 0x20:col = 18;break;
		case 0x40:col = 17;break;
		case 0x60:col = 19;break;
		case 0x80:col = 20;break;
		case 0xa0:col = 22;break;
		case 0xc0:col = 21;break;
		case 0xe0:col = 23;break;
		default:col = 30;break;
	}
	if(v_color & 0x02) bl = 1;
	if(v_color & 0x04) rev = 1;
	if(box == 1){
		if(rev == 1 && bl == 1)printf("\x1b[%d;%dH\x1b[%d;5;7m\xdb\x1b[m",winf->top-1,winf->left,col);
		if(rev == 1 && bl == 0)printf("\x1b[%d;%dH\x1b[%d;7m\xdb\x1b[m",winf->top-1,winf->left,col);
		if(rev == 0 && bl == 1)printf("\x1b[%d;%dH\x1b[%d;5m\xdb\x1b[m",winf->top-1,winf->left,col);
		if(rev == 0 && bl == 0)printf("\x1b[%d;%dH\x1b[%dm\xdb\x1b[m",winf->top-1,winf->left,col);
	}
	if(title != NULL){
		if(rev == 1 && bl == 1)printf("\x1b[%d;%dH\x1b[%d;5;7m%s\x1b[m",winf->top-1,winf->left+rx,col,title);
		if(rev == 1 && bl == 0)printf("\x1b[%d;%dH\x1b[%d;7m%s\x1b[m",winf->top-1,winf->left+rx,col,title);
		if(rev == 0 && bl == 1)printf("\x1b[%d;%dH\x1b[%d;5m%s\x1b[m",winf->top-1,winf->left+rx,col,title);
		if(rev == 0 && bl == 0)printf("\x1b[%d;%dH\x1b[%dm%s\x1b[m",winf->top-1,winf->left+rx,col,title);
	}

	gotoxy(cur_x,cur_y);
}
