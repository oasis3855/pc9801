/* Turbo C/C++ の BGI のデフォルトカラーパレット */

extern void color_palette_16(unsigned int pal_no,unsigned int _g,
             unsigned int _r, unsigned int _b); /* Change Color palette */

void PC98_BGI_16color_palette_init(void)
{
	color_palette_16(0 ,0 ,0 ,0 );
	color_palette_16(1 ,0 ,0 ,12);
	color_palette_16(2 ,0 ,12,0 );
	color_palette_16(3 ,0 ,12,12);
	color_palette_16(4 ,12,0 ,0 );
	color_palette_16(5 ,12,0 ,12);
	color_palette_16(6 ,12,12,0 );
	color_palette_16(7 ,11,11,11);
	color_palette_16(8 ,4 ,4 ,4 );
	color_palette_16(9 ,8 ,8 ,15);
	color_palette_16(10,8 ,15,8 );
	color_palette_16(11,8 ,15,15);
	color_palette_16(12,15,8 ,8 );
	color_palette_16(13,15,8 ,15);
	color_palette_16(14,15,15,8 );
	color_palette_16(15,15,15,15);

	return;
}