#include <stdio.h>
#include <conio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include "txt_inp.h"

int txt_jinp_hist(char *str, int str_len, char *histstr, int max_hist,
	int color, int x, int y)
{
	unsigned int retcode;
	int cur_hist=0,i;
	char *sv_hist;


	if(x <= 0 || x >= 81) x = wherex();
	if(y <= 0 || y >= 26) y = wherey();

	txt_chgkey(0x000c,fkey_sav);
	txt_chgkey(0x000d,fkey_def);
	signal(SIGINT,SIG_IGN);
	signal(SIGABRT,SIG_IGN);

	max_hist--;

	do{
		str[0] = 0;
		if( (cur_hist >= 0) && (strlen((char *)(histstr + cur_hist * str_len)) <= str_len) ){
			strcpy(str,(char *)(histstr + cur_hist * str_len));
		}

		retcode = txt_jinp_edit(str,"",str_len,color,x,y);

		if((retcode == K_DN) && (cur_hist < max_hist)){
			cur_hist++;
		}
		else if(retcode == K_UP){
			if(cur_hist >= 0) cur_hist--;
			else cur_hist = -1;
		}
		else if(retcode != 0) txt_jinp_error();
	}while(retcode == K_UP || retcode == K_DN);

	if(retcode != K_ESC){
		for(i = max_hist ; i >= 1 ; i--){
			strcpy((char *)(histstr + i* str_len) , (char *)(histstr + (i-1) * str_len));
		}
		strcpy(histstr,str);
	}

	signal(SIGINT,SIG_DFL);
	signal(SIGABRT,SIG_DFL);
	txt_chgkey(0x000d,fkey_sav);

	return(retcode);


} /* getinput */


char *txt_get_histbuf(char *hist_buf, int max_hist, int hist_len, int mode)
{
	int buf_len,i;

	buf_len = (hist_len + 1) * max_hist;

	if(mode == _HIST_GET){
		hist_buf = (char *)calloc(buf_len , sizeof(char));
		for(i = 0 ; i < max_hist ; i++){
			strcpy((char *)(hist_buf + i*20) , "");
		}
	}
	if(mode == _HIST_FREE){
		free(hist_buf);
	}
	return( hist_buf );
}