#include<dos.h>

/* 16 色　カラーパレット　設定 */
void color_palette_16(unsigned int palette_no, unsigned int _green, unsigned int _red, unsigned int _blue)
{
	outportb(0x6a, 0x01);			/* 16 色モード      */

	outportb(0xa8, palette_no & 0x0f);			/* color # */
	outportb(0xaa, _green & 0x0f);
	outportb(0xac, _red & 0x0f);
	outportb(0xae, _blue & 0x0f);

	return ;
}
