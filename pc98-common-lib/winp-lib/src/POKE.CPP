/*      Copyright  1990 E.Toyokuni      Number PO01900903  */
/*                                             CM900811    */

#include <stdio.h>
#include <dos.h>
#include <mem.h>
#include "win_i1.h"
					/* “Á’èƒƒ‚ƒŠ‚Ö‚Ì‘‚İ  */

int poke_str(	int dseg,
	int doff,
	char *dbuff,
	unsigned size )

{
	movedata(FP_SEG(dbuff),FP_OFF(dbuff),dseg,doff,size);

	return(0);
}

