/* カラーモードの設定 */
#include<dos.h>

void PC98_16color(int sw);

void PC98_16color(int sw)
{
	if(sw) outportb(0x6a,0x01);	/* 16色 */
	else outportb(0x6a,0x00);	/* 8 色 */
	return;
}
