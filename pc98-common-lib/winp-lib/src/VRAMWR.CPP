/*      Copyright  1990 E.Toyokuni      Number VR04900902  */
/*      				       CM900815	   */

#if !defined(NOTLIB)
	#include <stdio.h>
	#include <stdlib.h>
	#include <dos.h> 
#endif
#include "win_i1.h"
				/*  ƒeƒLƒXƒg‚u‚q‚`‚l‚Ö‚Ì’¼Ú‘ž‚Ý  */
int vramwr(
	int x,
	int y,
	char *mark,
	char *attr,
	int  ura )
{
int cpos =0 ;
int vramseg ;	/*@‡A  */

	if (x >=0 && x < 80 && y>= 0 && y < 25 ){
		cpos = y * 0xa0 + x *2  ;
	}
	else{
		return(-1);	/*  ‡@  */
	}
	if(ura == 1 ){
		vramseg = 0xa100;	/*@‡B  */
	}
	else{
		vramseg = 0xa000;	/*@‡C  */
	}
	poke_str(vramseg,cpos ,mark,2);	/*@‡D  */
	poke_str(vramseg+0x200,cpos,attr,1);/*@‡E  */
	return(0);
	
}

