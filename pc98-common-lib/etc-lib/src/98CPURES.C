/* プロテクトモード関連 286/386 */
#include<dos.h>
void PC98_286CPU_reset(void);
void PC98_286CPU_protect_mode(void);
void PC98_386CPU_protect_access(int sw);

/* V30では保証無し */

/* CPU/NDP 初期化、リアルモードへ、*/
void PC98_286CPU_reset(void)
{
	outportb(0x00f0,0x00);
	return;
}

/* プロテクトモードへ、*/
void PC98_286CPU_protect_mode(void)
{
	outportb(0x00f2,0x00);
	return;
}

/* プロテクトメモリ　アクセス権 */
void PC98_386CPU_protect_access(int sw)
{
	if(sw) outportb(0xf6,0x00);	/* 可能 */
	else outportb(0xf6,0x01);	/* 禁止 */
	return;
}

