/*      Copyright  1990 E.Toyokuni      Number PE01900902  */
/*                                             CM900810    */

#include <stdio.h>
#include <dos.h>
#include <mem.h>
#include "win_i1.h"
					/* “Á’èƒƒ‚ƒŠ‚Ì“Ç‚İ  */
int peek_str(	int sseg,
	int soff,
	char *dbuff,
	unsigned size )

{
	movedata(sseg,soff,FP_SEG(dbuff),FP_OFF(dbuff),size);

	return(0);
}

