/* PC-98 INT DCH を使用したテキスト画面制御 */
#include<stdio.h>
#include<dos.h>

void near PC98_print_doll(unsigned char *str)
{
	_DX = FP_OFF(str);
	_CX = 0x0010;
	_AX = 0x0100;
	geninterrupt(0xdc);
}

void near PC98_cls_txt(void)
{
	_DX = 0x0002;
	_CX = 0x0010;
	_AX = 0x0a00;
	geninterrupt(0xdc);
}

void near PC98_locate_txt(unsigned int _x,unsigned int _y)
{
	_DH = (unsigned char)_y;
	_DL = (unsigned char)_x;
	_CX = 0x0010;
	_AX = 0x0300;
	geninterrupt(0xdc);
}
 
void near PC98_color_txt(unsigned char _attr)
{
	_DL = _attr;
	_CX = 0x0010;
	_AX = 0x0200;
	geninterrupt(0xdc);
}

