/*      Copyright  1990 E.Toyokuni      Number VR05900903  */

#include <stdio.h>
#include <dos.h>
#include "win_i1.h"
					/* ƒeƒLƒXƒg‚u‚q‚`‚l‚Ì’¼Ú“Çž‚Ý  */
int vramrd( int x,
	int y,
	char *mark,
	char *attr,
	int  ura )

{
int cpos =0 ;
int vramseg ;

	if (x >=0 && x < 80 && y>= 0 && y < 25 ){
		cpos = y * 0xa0 + x *2  ;	/*  ‡@  */
	}
	else{
		mark[0]=0 ;	/*@‡A  */
		return(-1);	/*@‡B  */
	}
	if(ura ==1 ){
		vramseg = 0xa100;	/*@‡C  */
	}
	else{
		vramseg = 0xa000;	/*@‡D  */
	}

	peek_str(vramseg,cpos ,mark,2);	/*@‡E  */
	peek_str(vramseg+0x200,cpos,attr,2);/*@‡F  */

	return(0);

}

