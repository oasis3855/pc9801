#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include"win_i1.h"


int mnu_wmax_len(char *str[],int data_max)
{
	int i,len=0,max_len=0;
	for(i=0;i<data_max;i++){
		len = strlen(str[i]);
		if(len > max_len) max_len = len;
	}
	return(max_len+1);
}
