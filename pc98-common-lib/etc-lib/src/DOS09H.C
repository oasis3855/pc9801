/* DOS INT 21H ‚ğg—p‚µ‚½•¶š—ñ‚b‚q‚so—Í
   •¶š—ñ‚Í $ ‚ÅI‚í‚é– */

#include<stdio.h>
#include<dos.h>

void near dos_print_doll(unsigned char *str)
{
	_DX = FP_OFF(str);
	_AX = 0x0900;
	geninterrupt(0x21);

	return;
}

