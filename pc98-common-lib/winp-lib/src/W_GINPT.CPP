// input
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include<alloc.h>
#include"winp.h"
#include<txtl.h>

#define StrBuf(i)	(unsigned char *)(str + (mnu->data_len+1)*(i))
#define TRUE	1
#define FALSE	0

int inp_gwin_txt(GMNU_INFO *mnu, char *str[], char *mnu_str[], int *retcode);
void inp_ginit_win_txt(int col, int data_len, int data_max, char *mstr[], char *str[]);


// extend input system
int inp_gwin_txt(GMNU_INFO *mnu, char *str[], char *mnu_str[], int *retcode)
{
	int i,cur_data,check;
	WIN_INFO w;	// make & close window
	char *buf;
	int pt;		// buffer pointer

	*retcode = -1;	// error !


	if(mnu->left <= 1 || (mnu->left+mnu->data_len-1) >= 80) return(-1);
	if(mnu->top <- 1 || (mnu->top+mnu->data_max*2-1) >= 25) return(-1);
	if(mnu->data_max*2 < 1 || mnu->data_len < 1) return(-1);

	w.left = mnu->left; w.top = mnu->top;
	w.right = mnu->left + mnu->data_len - 1;
	w.bottom = mnu->top + mnu->data_max*2 - 1;
	w.color = mnu->boarder_color;
	w.pat = W_RECT; w.shrink = W_OFF;


	if(mnu->mode == W_FWRITE)
	{
		// GRPH SEC
		// TEXT SEC
		if(mnu->save & W_GSV_TXT) w.sv_on = W_ON;
		else w.sv_on = W_OFF;
		win_wmake(&w);
		mnu->t_sv = (void *)w.sv;
		if(w.sv_on == W_ON) mnu->sv_flag |= W_GSV_TXT;
		inp_ginit_win_txt(mnu->boarder_color,mnu->data_len,mnu->data_max,mnu_str,str);

		return 0;
	}	// W_FWRITE

	if(mnu->mode == W_RESTORE)
	{
		w.sv_on = W_OFF;
		win_wmake(&w);
		inp_ginit_win_txt(mnu->boarder_color,mnu->data_len,mnu->data_max,mnu_str,str);
		return 0;
	}	// W_RESTORE

	if(mnu->mode == W_CLOSE)
	{
		// GRPH
		// TEXT
		w.sv = (char *)mnu->t_sv;
		if(mnu->sv_flag & W_GSV_TXT) w.sv_on = W_ON;
		else w.sv_on = W_OFF;
		win_wdelete(&w);
		return 0;
	}

	if(!(mnu->mode & W_MENU)) return -1;

	_txt_jinp_up = TRUE;
	_txt_jinp_down = TRUE;

	buf = (unsigned char *)malloc((mnu->data_len + 2) * mnu->data_max + 2);
	if(buf == NULL) return -1;


	pt = mnu->data_len + 1;

	for(i=0;i<mnu->data_max;i++)
	{
		strncpy(buf + pt*i,str[i],mnu->data_len-2);
	}


	i = mnu->cur_topic;

	for(;;)
	{
		check = txt_jinput(buf+pt*(i-1),mnu->data_len-1,buf+pt*(i-1),mnu->color,1,i*2);
		switch(check)
		{
			case	K_UP	:
					i--;
					if(i <= 0) i = mnu->data_max;
					break;
			case	K_DN	:
					i++;
					if(i > mnu->data_max) i = 1;
					break;
			case	K_ESC	:
					free(buf);
					*retcode = 0x1b;
					return 0;
			case	0	:	// ret
					for(i=0;i<mnu->data_max;i++)
					{
						strncpy(str[i],buf+pt*i,mnu->data_len-2);
					}
					mnu->cur_topic = i;
					free(buf);
					*retcode = 0;
					return 0;
			default	:	break;
		}
	}
}


void inp_ginit_win_txt(int col, int data_len, int data_max, char *mstr[], char *str[])
{
	int i;

	_wscroll = 0;

	textattr(col);
	for(i=0;i<=data_max*2;i++)
	{
		gotoxy(1,1);
		insline();
		cprintf("%*s",data_len,"");
	}
	for(i=0;i<data_max;i++)
	{
		gotoxy(1,i*2+1);
		cputs(mstr[i]);					// string
	}
	textattr(WHITE);
	for(i=0;i<data_max;i++)
	{
		gotoxy(1,i*2+2);
		cprintf("%*s",data_len,"");		// input blank
	}
	for(i=0;i<data_max;i++)
	{
		gotoxy(1,i*2+2);
		cputs(str[i]);
	}


	_wscroll = 1;

}
