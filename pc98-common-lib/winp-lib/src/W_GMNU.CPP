#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include<graphics.h>
#include<alloc.h>
#include"winp.h"




// extend menu system
int mnu_gmenu(GMNU_INFO *mnu, int dummy, char *str[], int *retcode)
{
	int add,cur_data;
	WIN_INFO w;	// make & close window

	add = dummy;	// dummy

	if(mnu->left <= 1 || (mnu->left+mnu->data_len-1) >= 80) return(-1);
	if(mnu->top <- 1 || (mnu->top+mnu->data_max-1) >= 25) return(-1);
	if(mnu->data_max < 1 || mnu->data_len < 1) return(-1);

	w.left = mnu->left; w.top = mnu->top;
	w.right = mnu->left + mnu->data_len - 1;
	w.bottom = mnu->top + mnu->data_max - 1;
	w.color = mnu->boarder_color;
	w.pat = W_RECT; w.shrink = W_OFF;

	setviewport(0,0,639,399,1);

	if(mnu->mode == W_FWRITE)
	{
		// GRPH SEC
		if(mnu->save & W_GSV_GR)
		{
			mnu->sv_flag = 0;
			mnu->g_sv = (void huge *)farmalloc(imagesize((w.left-2)*8,(w.top-2)*16,
								(w.right+1)*8,(w.bottom+1)*16)+0x100);
			if(mnu->g_sv != NULL)
			{
				getimage((w.left-2)*8,(w.top-2)*16,
						(w.right+1)*8,(w.bottom+1)*16,mnu->g_sv);
                    setviewport((w.left-2)*8,(w.top-2)*16,
						(w.right+1)*8,(w.bottom+1)*16,1);
				clearviewport();
				setviewport(0,0,639,399,1);
				mnu->sv_flag |= W_GSV_GR;
			}
		}

		// TEXT SEC
		if(mnu->save & W_GSV_TXT) w.sv_on = W_ON;
		else w.sv_on = W_OFF;
		win_wmake(&w);
		mnu->t_sv = (void *)w.sv;
		if(w.sv_on == W_ON) mnu->sv_flag |= W_GSV_TXT;
		mnu_ginit_put_s(str,mnu->data_max,mnu->cur_topic,mnu->color,mnu->select_color);

		return 0;
	}	// W_FWRITE

	if(mnu->mode == W_RESTORE)
	{
		w.sv_on = W_OFF;
		win_wmake(&w);
		mnu_ginit_put_s(str,mnu->data_max,mnu->cur_topic,mnu->color,mnu->select_color);
		return 0;
	}	// W_RESTORE

	if(mnu->mode == W_CLOSE)
	{
		// GRPH
		if(mnu->sv_flag & W_GSV_GR)
		{
			putimage((w.left-2)*8,(w.top-2)*16,mnu->g_sv,COPY_PUT);
			farfree((void far *)mnu->g_sv);
		}
		// TEXT
		w.sv = (char *)mnu->t_sv;
		if(mnu->sv_flag & W_GSV_TXT) w.sv_on = W_ON;
		else w.sv_on = W_OFF;
		win_wdelete(&w);
		return 0;
	}

	if(!(mnu->mode & W_MENU)) return -1;

	_wscroll = 0;

	cur_data = mnu->cur_topic;

	for(;;){
		add = 0;
		switch(mnu_wread_key() & 0xff00){
			case 0x3d00 : add = 1;break;		/* Down Key */
			case 0x3a00 : add = -1;break;		/* Up key */
			case 0x1c00 :					// RET
						*retcode = 0x000d;
						mnu->cur_topic = cur_data;
						_wscroll = 1;
						return(0);
			case 0x0000 :					// ESC
						*retcode = 0x001b;
						mnu->cur_topic = cur_data;
						_wscroll = 1;
						return(0);
			case 0x3b00 :					// Left key
						if(mnu->right_left)
						{
							mnu->cur_topic = cur_data;
							*retcode = 0x001d;
							_wscroll = 1;
							return 0;
						}
						break;
			case 0x3c00 :					// Right key
						if(mnu->right_left)
						{
							mnu->cur_topic = cur_data;
							*retcode = 0x001c;
							_wscroll = 1;
							return 0;
						}
						break;
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
				textattr(mnu->select_color);
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
				textattr(mnu->select_color);
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
				textattr(mnu->select_color);
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
				textattr(mnu->select_color);
				cputs(str[cur_data - 1]);
				textattr(mnu->color);
			}
		}	/* Cursor Up Key */
	}	/* For(;;) */

}


void mnu_ginit_put_s(char *str[],int data_max,int cur_line,int color,int rev_color)
{
	int i;

	_wscroll = 0;

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

	_wscroll = 1;
	return;
}
