/* MS-DOS ﾜｰｸ領域のキーボードバッファを強制消去 */
#include<dos.h>

void key_buf_cls(void)
{
	poke(0,0x524,0x502);
	poke(0,0x526,0x502);
	pokeb(0,0x528,0);
	return ;
}
