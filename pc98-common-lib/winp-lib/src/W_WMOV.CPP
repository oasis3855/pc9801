/*
	W_WMOV.C for WIN.LIB
		(C)opyright 1990 Hirokazu Inoue  All rights reserved .
		for Turbo C 2.0
	#include WIN.H .
*/

#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<stdlib.h>
#include"win_i1.h"

int win_wmove(int xr,int yr,WIN_INFO *winf)
{
	int x_size,y_size,o_l,o_t,o_r,o_b,cur_x,cur_y;
	char *scr_sv;

	x_size = winf->right - winf->left;
	y_size = winf->bottom - winf->top;
	o_l = winf->left;
	o_t = winf->top;
	o_r = winf->right;
	o_b = winf->bottom;

	cur_x = wherex();
	cur_y = wherey();

	if(winf->sv_on == 1){
		scr_sv = (char *)malloc((winf->right-winf->left+3)*(winf->bottom-winf->top+3)*4+10);
		if(scr_sv == NULL) return(1);
	}

	winf->left += xr;
	winf->right += xr;
	winf->top += yr;
	winf->bottom += yr;

	if(winf->left + xr <= 1){
		winf->left = 2;
		winf->right = 2 + x_size;
	}
	else if(winf->right + xr >= 80){
		winf->left = 79 - x_size;
		winf->right = 79;
	}
	if(winf->top + yr <= 1){
		winf->top = 2;
		winf->bottom = 2 + y_size;
	}
	else if(winf->bottom + yr >= 25){
		winf->top = 24 - y_size;
		winf->bottom = 24;
	}

	if(winf->sv_on == 1){
		gettext(o_l-1,o_t-1,o_r+1,o_b+1,scr_sv);
	}
	window(o_l-1,o_t-1,o_r+1,o_b+1);
	clrscr();
	if(winf->sv_on == 1){
		puttext(o_l-1,o_t-1,o_r+1,o_b+1,winf->sv);
		gettext(winf->left-1,winf->top-1,winf->right+1,winf->bottom+1,winf->sv);
		puttext(winf->left-1,winf->top-1,winf->right+1,winf->bottom+1,scr_sv);
		free(scr_sv);
	}
	else{

		window(winf->left,winf->top,winf->right,winf->bottom);
		clrscr();
		textkei3(winf->left-2,winf->top-2,winf->right,winf->bottom,winf->color,winf->pat,W_PAGE_1);
	}

	gotoxy(cur_x,cur_y);

	return(0);
}
