#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<string.h>
#include"winp.h"


// extend menu system
int mnu_wmenu_s(char *str[],EXTEND_MNU_INFO *mnu,int cur_data = 1)
{
	int i,add;

	if(mnu->left <= 1 || (mnu->left+mnu->data_len-1) >= 80) return(-1);
	if(mnu->top <- 1 || (mnu->top+mnu->data_max-1) >= 25) return(-1);
	if(mnu->data_max < 1 || mnu->data_len < 1) return(-1);
	if(mnu->data_max < 1) return(-1);


	i = win_mmake(mnu->left,mnu->top,mnu->left+mnu->data_len-1,mnu->top+mnu->data_max-1,mnu->boarder_color,W_RECT,W_PAGE_1);
	if(i == 1) return(-1);

	mnu_winit_put_s(str,mnu->data_max,cur_data,mnu->color,mnu->rev_color);

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
			if(cur_data == mnu->data_max)
			{
				// To top of line
				gotoxy(1,cur_data);
				cputs(str[cur_data - 1]);
				cur_data = 1;
				textattr(mnu->rev_color);
				gotoxy(1,1);
				cputs(str[0]);
				textattr(mnu->color);
			}
			else{
				// 1 line down
				gotoxy(1,cur_data);
				cputs(str[cur_data - 1]);
				cur_data++;
				gotoxy(1,cur_data);
				textattr(mnu->rev_color);
				cputs(str[cur_data - 1]);
				textattr(mnu->color);
			}
		}	/* Cursor Down Key */
		else if(add == -1){
			/* cursor up */
			if(cur_data == 1)
			{
				// To last of line
				gotoxy(1,1);
				cputs(str[0]);
				cur_data = mnu->data_max;
				textattr(mnu->rev_color);
				gotoxy(1,cur_data);
				cputs(str[cur_data - 1]);
				textattr(mnu->color);
			}
			else{
				// 1 line up
				gotoxy(1,cur_data);
				cputs(str[cur_data - 1]);
				cur_data--;
				gotoxy(1,cur_data);
				textattr(mnu->rev_color);
				cputs(str[cur_data - 1]);
				textattr(mnu->color);
			}
		}	/* Cursor Up Key */
	}	/* For(;;) */

}

