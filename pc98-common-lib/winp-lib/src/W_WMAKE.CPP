/*
	W_WMAKE.C for WIN.LIB
		(C)opyright 1990 Hirokazu Inoue  All rights reserved .
		for Turbo C 2.0
	#include WIN.H .
*/

#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<stdlib.h>
#include<dos.h>
#include"win_i1.h"


int win_wmake(WIN_INFO *winf)
{
	int w_eni,w_enj;
	int _xmin=1,_xmax=1;	/* to Kanji, save TEXT */

	if(winf->left <= 1 || winf->right >= 80) return(1);
	else if(winf->top <= 1 || winf->bottom >= 25) return(1);
	if(winf->right - winf->left < 0) return(1);
	if(winf->bottom - winf->top < 0) return(1);
	if(!(winf->pat == 1 || winf->pat == 11 || winf->pat == 12)) return(1);

	if(winf->left == 2) _xmin = 0;
	if(winf->right == 79) _xmax = 0;

	if(winf->sv_on == 1){
		/* SAVE back */
		winf->sv = (char *)malloc((winf->right-winf->left+3+_xmin+_xmax)*(winf->bottom-winf->top+3)*4+10);
		if(winf->sv == NULL){
			winf->sv_on = 0;
			return(1);
		}
		gettext(winf->left-1-_xmin,winf->top-1,
				winf->right+1+_xmax,winf->bottom+1,winf->sv);
		winf->sv_on = 1;
	}
	if(winf->shrink == 1){

		for(w_eni = winf->left,w_enj = winf->top;w_eni<=winf->right;w_eni=w_eni+2){
			if(w_enj > winf->bottom) w_enj = winf->bottom;
			window(winf->left,winf->top,w_eni,w_enj);
			clrscr();
			textkei3(winf->left-2,winf->top-2,w_eni,w_enj,winf->color,winf->pat,W_PAGE_1);
			delay(winf->slow);
			w_enj++;
		}
	}

	window(winf->left-1,winf->top-1,winf->right+1,winf->bottom+1);
	clrscr();
	textkei3(winf->left-2,winf->top-2,winf->right,winf->bottom,winf->color,winf->pat,W_PAGE_1);
	window(winf->left,winf->top,winf->right,winf->bottom);

	return(0);
}
