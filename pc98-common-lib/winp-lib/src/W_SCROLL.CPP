#include<stdio.h>
#include<conio.h>
#include"win_i1.h"

void win_wscroll(int scr);

void win_wscroll(int scr)
{
	if(scr == 1){
		/* Scroll up */
		gotoxy(1,1);
		delline();
	}
	else{
		/* Scroll down */
		gotoxy(1,1);
		insline();
	}
	return;
}