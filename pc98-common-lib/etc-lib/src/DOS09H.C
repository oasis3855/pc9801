/* DOS INT 21H を使用した文字列ＣＲＴ出力
   文字列は $ で終わる事 */

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