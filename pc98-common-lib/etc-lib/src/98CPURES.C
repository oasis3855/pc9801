/* �v���e�N�g���[�h�֘A 286/386 */
#include<dos.h>
void PC98_286CPU_reset(void);
void PC98_286CPU_protect_mode(void);
void PC98_386CPU_protect_access(int sw);

/* V30�ł͕ۏؖ��� */

/* CPU/NDP �������A���A�����[�h�ցA*/
void PC98_286CPU_reset(void)
{
	outportb(0x00f0,0x00);
	return;
}

/* �v���e�N�g���[�h�ցA*/
void PC98_286CPU_protect_mode(void)
{
	outportb(0x00f2,0x00);
	return;
}

/* �v���e�N�g�������@�A�N�Z�X�� */
void PC98_386CPU_protect_access(int sw)
{
	if(sw) outportb(0xf6,0x00);	/* �\ */
	else outportb(0xf6,0x01);	/* �֎~ */
	return;
}

