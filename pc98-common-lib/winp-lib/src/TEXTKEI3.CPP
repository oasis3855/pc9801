/*      Copyright  1990 E.Toyokuni      Number TE02900909  */
/*           cü‚Ìæ’[•¶š‚ÌC³   Bug fix	90/09/10   */
/*                                             CM901105    */

#include <stdio.h>
#include <stdlib.h>
#include "win_i1.h"

#include "textkei2.h" 
					/*  Œrü‚Ì•`‰æ  */
int textkei3(int  x1,
	 int  y1,
	 int  x2,
	 int  y2,
	 int  att,
	 int  kind,
	 int  ura)

{
	int i ;
	int n;
	unsigned char line ;
	unsigned char linefst ;
	unsigned char lineend ;
	int xn,yn ;
	char attrib ;
	int t_color,t_blink,v_color,v_blink;
/*************************
	t_color = att & 0x0007;
	t_blink = att & 0x0f00;
	v_color = (t_color & 0x04)|((t_color & 0x02)<<2)|((t_color & 0x01)<<1);
	v_blink = (t_blink >> 3) | 0x0001;
	attrib = (char)(v_color << 4)|(v_blink);

	attrib =   att ;
**************************/

	attrib = _col_convert(att);
	ura = 0;	/* always page #0 */


	if(x1 == x2 && y1 == y2)	/*@‡A  */
		return(0) ;
	if(x1> x2)			/*@‡B  */
		iswap (&x1,&x2) ;
	if(y1> y2)
		iswap (&y1,&y2) ;
	if (x1 != x2 && y1 != y2 )	/*@‡C  */
		goto box ;

	if (x1 == x2 ){			/*@‡D  */
		line = SLINE+1 ;
		dispkei2(x1,y1,FSTUD,attrib,kind,ura);
		n= y2 - y1   ;
		for(i=1;i<n;i++){
			dispkei2(x1,y1+i,line,attrib,kind,ura);
		}
		dispkei2(x1,y2,ENDUD,attrib,kind,ura);
	}
	
	if (y1 == y2){			/*@‡F  */
		switch(kind){
			case 2 :			/*@‡E  */
			case 12:line    = DLINE ;
				linefst = DFSTLR ;
				lineend = DENDLR ;
				break ;	
			case 1 :
			case 11:
			default:line    = SLINE ;
				linefst = FSTLR ;
				lineend = ENDLR ;
				break ;

		}
		dispkei2(x1,y1,linefst,attrib,kind,ura);
		n= x2 - x1   ;
		for(i= 1 ;i<n;i++){
			dispkei2(x1+i,y1,line,attrib,kind,ura) ;
		}
		dispkei2(x2,y1,lineend,attrib,kind,ura);
	}
	return(0) ;

box :		/*@‡G  */
	xn = x2 - x1 +1 ;
	yn = y2 - y1 +1 ;
	switch(kind){
		case 1 :
		case 11:
			line = HARDL ;	/*  ‡H  */
			break ;
		case 2 :
		case 12:
			line = SOFTL ;	/*  ‡I  */
			break ;

		default:line = HARDL ;
			break ;

	}
					/*  ‡J  */
	for(i=0;i<xn-1;i++){
		dispkei2(x1+i+1,y1,SLINE,attrib,kind,ura);
	}
	dispkei2(x1,y1,line,attrib,kind,ura)     ; 
	dispkei2(x2,y1,line+1,attrib,kind,ura)   ; 

	for (i=1 ;i<yn-1 ;i++){
		dispkei2(x1,y1+i,0x96,attrib,kind,ura);
		dispkei2(x2,y1+i,0x96,attrib,kind,ura);
	}	

	for(i=0;i<xn-1;i++){
		dispkei2(x1+i+1,y2,SLINE,attrib,kind,ura);
	}
	dispkei2(x1,y2,line+2,attrib,kind,ura)   ; 
	dispkei2(x2,y2,line+3,attrib,kind,ura)   ; 

	return (0) ;
}




