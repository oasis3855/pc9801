/* モノクロ／カラー　モード切り替え */
#include<dos.h>

void PC98_grph_mono(int sw);

void PC98_grph_mono(int sw)
{
	if(sw) outportb(0x68,0x03);	/* mono */
	else outportb(0x68,0x02);	/* color */
	return;
}
