/* ボーダーカラー設定 */
#include<dos.h>

void PC98_boarder_color(int col);


void PC98_boarder_color(int col)
{
	/* CONIO.H のカラー／Ｔｕｒｂｏ　Ｃ＋＋ */
	char _param=0x00;

	_param |= ((char)col & 0x02) << 1;
	_param |= ((char)col & 0x04) >> 1;
	_param |= (char)col & 0x09;
	_param = _param << 4;

	outportb(0x6c,_param);
	return;
}
