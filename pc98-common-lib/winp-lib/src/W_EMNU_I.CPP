#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include"winp.h"


void mnu_winit_put_s(char *str[],int data_max,int cur_line,int color,int rev_color)
{
	int i;

	// write all line in normal attr
	gotoxy(1,1);
	textattr(color);
	for(i=0;i<data_max;i++){
		cputs("\r\n");
		cputs(str[i]);
	}

	// write reverse line
	textattr(rev_color);
	gotoxy(1,cur_line);
	cputs(str[cur_line - 1]);

	// default color
	textattr(color);

	return;
}
