/* 本体フォントを切り替える
   漢字表示に対して保証無し */
#include<dos.h>

void PC98_88font(int sw);

void PC98_88font(int sw)
{
	if(sw) outportb(0x68,0x06);	/* 8801 font */
	else outportb(0x68,0x07);	/* 9801 font */
	return;
}
