#include<stdio.h>
#include<dos.h>

/* ﾊｲﾚｿﾞﾘｭｰｼｮﾝ ﾓｰﾄﾞ かどうか確認 */
unsigned int PC98_chk_hi_resolution(void)
{
	volatile unsigned char far *_ck_hirso;

	_ck_hirso = (unsigned char far *)MK_FP(0x0000,0x053c);

	if(*_ck_hirso & 0x80) return 0;
	return 1;
}

/* 16色が使えるか確認 */
unsigned int PC98_chk_16color(void)
{
	volatile unsigned char far *_ck_16;

	_ck_16 = (unsigned char far *)MK_FP(0x0000,0x054c);

	if(*_ck_16 & 0x04) return 1;
	return 0;
}

