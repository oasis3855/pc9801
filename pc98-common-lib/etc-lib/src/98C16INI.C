/* PC98 のデフォルトカラー */
#include<stdio.h>
#include<dos.h>

void PC98_16color_palette_init(void);


void PC98_16color_palette_init(void)
{
    int i, red, green, blue;

    outportb(0x6a, 0x01);	/* 16色モードに変更 */

    for (i = 0; i < 16; i++)
    {
        red   = (i & 2) / 2 * 15;
        green = (i & 4) / 4 * 15;
        blue  = (i & 1)     * 15;
        if (8 <= i)
        {
            red   = red   / 2 + 4;
            green = green / 2 + 4;
            blue  = blue  / 2 + 4;
        }
        outp(0xa8, i);
        outp(0xac, red);
        outp(0xaa, green);
        outp(0xae, blue);
    }
}

