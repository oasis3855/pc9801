/*
	W_MDEL.C for WIN.LIB
		(C)opyright 1990 Hirokazu Inoue  All rights reserved .
		for Turbo C 2.0
	#include WIN.H .
*/

#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<stdlib.h>
#include"win_i1.h"

int win_mdelete(int left,
		int top,
		int right,
		int bottom)
{
	if(left <= 1 || right >= 80) return(1);
	else if(top <= 1 || bottom >= 25) return(1);
	if(right - left < 0) return(1);
	if(bottom - top < 0) return(1);

	window(left-1,top-1,right+1,bottom+1);
	clrscr();

	return(0);
}
