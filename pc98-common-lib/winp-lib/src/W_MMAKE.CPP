/*
	W_MMAKE.C for WIN.LIB
		(C)opyright 1990 Hirokazu Inoue  All rights reserved .
		for Turbo C 2.0
	#include WIN.H .
*/

#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<stdlib.h>
#include"win_i1.h"

#pragma warn -par
int win_mmake(int left,
		int top,
		int right,
		int bottom,
		int color,
		int pattern,
		int page = 0)
{
	if(left <= 1 || right >= 80) return(1);
	else if(top <= 1 || bottom >= 25) return(1);
	if(right - left < 0) return(1);
	if(bottom - top < 0) return(1);
	if(!(pattern == 1 || pattern == 11 || pattern == 12)) return(1);

	window(left-1,top-1,right+1,bottom+1);
	textkei3(left-2,top-2,right,bottom,color,pattern,W_PAGE_1);
	window(left,top,right,bottom);

	return(0);

}

#pragma warn .par