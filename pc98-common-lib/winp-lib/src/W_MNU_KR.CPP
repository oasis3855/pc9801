#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include"win_i1.h"

int mnu_wread_key(void)
{
	struct REGPACK reg;

	reg.r_ax = 0x0018;
	intr(0x18,&reg);
	return(reg.r_ax);

}
