/* Menu select OBJ
	(C) 1991 Hirokazu Inoue   All rights reserved
	for WINS.LIB

	Ver 1.00	1991.01.05
*/
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include"win_i1.h"


int mnu_wmenu(char *str[],MNU_INFO *mnu)
{
	int cur_data=1,cur_line=1,i,add;

	if(mnu->left <= 1 || (mnu->left+mnu->data_len-1) >= 80) return(-1);
	if(mnu->top <- 1 || (mnu->top+mnu->data_line-1) >= 25) return(-1);
	if(mnu->data_line < 1 || mnu->data_len < 1) return(-1);
	if(mnu->data_max < 1) return(-1);


	i = win_mmake(mnu->left,mnu->top,mnu->left+mnu->data_len-1,mnu->top+mnu->data_line-1,mnu->boarder_color,W_RECT,W_PAGE_1);
	if(i == 1) return(-1);

	textattr(mnu->color);

	mnu_winit_put(str,mnu->data_line,mnu->data_max,mnu->color);

	for(;;){
		add = 0;
		switch(mnu_wread_key() & 0xff00){
			case 0x3d00 : add = 1;break;		/* Down Key */
			case 0x3a00 : add = -1;break;		/* Up key */
			case 0x1c00 : return(cur_data);	/* Ret */
			case 0x0000 : return(0);			/* ESC */
			default : break;
		}
		if(add == 1){
			/* cursor down */
			if(cur_line == mnu->data_line){
				if(cur_data == mnu->data_max){}
				else{
					gotoxy(1,cur_line);
					cputs(str[cur_data-1]);
					gotoxy(1,1);delline();
					cur_data++;
					gotoxy(1,mnu->data_line);
					textattr(mnu->color|REVERSE);
					cputs(str[cur_data-1]);
					textattr(mnu->color);
				}
			}
			else{
               	if(cur_data == mnu->data_max){}
				else{
					gotoxy(1,cur_line);
					cputs(str[cur_data-1]);
					cur_data++;
					cur_line++;
					gotoxy(1,cur_line);
					textattr(mnu->color|REVERSE);
					cputs(str[cur_data-1]);
					textattr(mnu->color);
				}
			}
		}	/* Cursor Down Key */
		else if(add == -1){
			/* cursor up */
			if(cur_line == 1){
				if(cur_data == 1){}
				else{
					gotoxy(1,cur_line);
					cputs(str[cur_data-1]);
					gotoxy(1,1);insline();
					cur_data--;
					gotoxy(1,1);
					textattr(mnu->color|REVERSE);
					cputs(str[cur_data-1]);
					textattr(mnu->color);
				}
			}
			else{
				if(cur_data == 1){}
				else{
					gotoxy(1,cur_line);
					cputs(str[cur_data-1]);
					cur_data--;
					cur_line--;
					gotoxy(1,cur_line);
					textattr(mnu->color|REVERSE);
					cputs(str[cur_data-1]);
					textattr(mnu->color);
				}
			}
		}	/* Cursor Up Key */
	}	/* For(;;) */

}


void mnu_winit_put(char *str[],int data_line,int data_max,int color)
{
	int i;

	textattr(color|REVERSE);
	gotoxy(1,1);
	cputs(str[0]);
	textattr(color);

	for(i=1;i<data_max;i++){
		if((i+1) > data_line) break;
		cputs("\r\n");
		cputs(str[i]);
	}
	return;
}
