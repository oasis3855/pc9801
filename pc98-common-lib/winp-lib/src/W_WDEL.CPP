/*
	W_WDEL.C for WIN.LIB
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

int win_wdelete(WIN_INFO *winf)
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
		if(winf->shrink == 1){
			/* Shrink Window Delete */
			for(w_eni = winf->right,w_enj = winf->bottom;w_eni>=winf->left;w_eni=w_eni-2){
				if(w_enj < winf->top) w_enj = winf->top;
				puttext(winf->left-1-_xmin,winf->top-1,
					winf->right+1+_xmax,winf->bottom+1,winf->sv);
				window(winf->left,winf->top,w_eni,w_enj);
				clrscr();
				textkei3(winf->left-2,winf->top-2,w_eni,w_enj,winf->color,winf->pat,W_PAGE_1);
				delay(winf->slow);
				w_enj--;
			}
		}
		/* RESTORE back */
		puttext(winf->left-1-_xmin,winf->top-1,
				winf->right+1+_xmax,winf->bottom+1,winf->sv);
		winf->sv_on = 0;
		free(winf->sv);
	}
	else{
		window(winf->left-1,winf->top-1,winf->right+1,winf->bottom+1);
		clrscr();
	}

	return(0);
}
