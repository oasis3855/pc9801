/* CPU Ç™ 286 à»è„Ç©ämîF */
#include<stdio.h>
#include<dos.h>

unsigned int PC98_chk_8086(void)
{
	unsigned int _CPU_type;

	__emit__(0x54);		/* PUSH SP */
	__emit__(0x58);		/* POP AX */
	__emit__(0x29,0xe0);	/* SUB AX,SP */
	__emit__(0x40);		/* INC AX */
	__emit__(0x40);		/* INC AX */
	_CPU_type = _AX;
	/* if 80286/80386 _AX = 2, 8086/V30 _AX = 0*/
	/* because PUSH SP push different Word at 286,86 */

	if(_CPU_type == 2) return 0;			/* 286/386 */
	return 1;					/* 86 */
}
