// ウインドウを描く
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include<alloc.h>
#include"winp.h"
#include<txtl.h>
#include<glio.h>

#define TRUE	1
#define FALSE	0

void win_GLIO_ginit_win(int col, int data_len, int data_max);


int win_GLIO_gwin(GMNU_INFO *mnu, int *retcode)
{
	int i,cur_data,check;
	WIN_INFO w;	// make & close window

	*retcode = -1;	// error !


	if(mnu->left <= 1 || (mnu->left+mnu->data_len-1) >= 80) return(-1);
	if(mnu->top <- 1 || (mnu->top+mnu->data_max-1) >= 25) return(-1);
	if(mnu->data_max < 1 || mnu->data_len < 1) return(-1);

	w.left = mnu->left; w.top = mnu->top;
	w.right = mnu->left + mnu->data_len - 1;
	w.bottom = mnu->top + mnu->data_max - 1;
	w.color = mnu->boarder_color;
	w.pat = W_RECT; w.shrink = W_OFF;

	gview(0,0,639,399,-1,-1);

	if(mnu->mode == W_FWRITE)
	{
		// GRPH SEC
		if(mnu->save & W_GSV_GR)
		{
			mnu->sv_flag = 0;
			mnu->g_sv = (void huge *)farmalloc(ggetsize((w.left-2)*8,(w.top-2)*16,
								(w.right+1)*8,(w.bottom+1)*16)+0x100);
			if(mnu->g_sv != NULL)
			{
				gget((w.left-2)*8,(w.top-2)*16,
						(w.right+1)*8,(w.bottom+1)*16,mnu->g_sv);
				gbar((w.left-2)*8,(w.top-2)*16,
						(w.right+1)*8,(w.bottom+1)*16,_BLACK);
				gview(0,0,639,399,-1,-1);
				mnu->sv_flag |= W_GSV_GR;
			}
		}

		// TEXT SEC
		if(mnu->save & W_GSV_TXT) w.sv_on = W_ON;
		else w.sv_on = W_OFF;
		win_wmake(&w);
		mnu->t_sv = (void *)w.sv;
		if(w.sv_on == W_ON) mnu->sv_flag |= W_GSV_TXT;
		win_GLIO_ginit_win(mnu->boarder_color,mnu->data_len,mnu->data_max);

		return 0;
	}	// W_FWRITE

	if(mnu->mode == W_RESTORE)
	{
		w.sv_on = W_OFF;
		win_wmake(&w);
		win_GLIO_ginit_win(mnu->boarder_color,mnu->data_len,mnu->data_max);
		return 0;
	}	// W_RESTORE

	if(mnu->mode == W_CLOSE)
	{
		// GRPH
		if(mnu->sv_flag & W_GSV_GR)
		{
			gput((w.left-2)*8,(w.top-2)*16,GPUT_COPY,mnu->g_sv);
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

}

/* ウインドウ内を指定されたアトリビュートで埋める */
void win_GLIO_ginit_win(int col, int data_len, int data_max)
{
	int i;

	_wscroll = 0;

	textattr(col);
	for(i=0;i<=data_max;i++)
	{
		gotoxy(1,1);
		insline();
		cprintf("%*s",data_len,"");
	}
	_wscroll = 1;

}