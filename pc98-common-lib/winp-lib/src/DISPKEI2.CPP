/*      Copyright  1990 E.Toyokuni      Number DI01900911  */
/*                                             CM90080502  */

// c++ version

#include <stdio.h>
#include "win_i1.h"

#include "textkei2.h" 

int dispkei2(int  x, int  y, unsigned char mark, char attr, int  kind, int  ura);

int dispkei2(int  x,
	 int  y,
	 unsigned char mark,
	 char attr,
	 int  kind,
	 int  ura )

{
	unsigned char moji[4];
	unsigned char attri[4];
	unsigned char  c[4];
	unsigned char cu[4];
	unsigned char cd[4];
	unsigned char cl[4];
	unsigned char cr[4];
	unsigned char up,down,right,left ;
	unsigned char dainyu ;
	unsigned char result ;
	
	if (kind>=10){				/*  á@  */
		if( mark == FSTLR  || mark == ENDLR ||		/*Å@áA  */
		    mark == DFSTLR || mark == DENDLR ){
			mark = LR ;
		}
		if( mark == FSTUD  || mark == ENDUD ){
			mark = UD ;
		}
		result = mark;			/*Å@áB  */
		goto SPEED ;
	}

	vramrd(x,y,moji,attri,ura);

	up=down=right=left=NO  ;		/*Å@áC  */

	vramrd(x,y,  c ,attri,ura);		/*Å@áD  */
	vramrd(x,y-1,cu,attri,ura);
	vramrd(x,y+1,cd,attri,ura);
	vramrd(x-1,y,cl,attri,ura);
	vramrd(x+1,y,cr,attri,ura);


	if(cu[1]==0 && ((cu[0] == LRD)|| (cu[0]==UDR)||(cu[0]==UDL)||           (cu[0]==LUKAGI)||(cu[0]==RUKAGI)|| (cu[0]==LUMARU)|| (cu[0]==RUMARU)||          (cu[0]==CROSS)||(cu[0]==UD)||(cu[0]==DUDR)||(cu[0]==DCROSS)||(cu[0]==DUDL) )){

 	     		up=YES ;		/*Å@áE  */

	}

	if(cd[1]==0 && ((cd[0] == LRU)||(cd[0]==UDR)||(cd[0]==UDL)||            (cd[0]==LDKAGI)|| (cd[0]==RDKAGI) || (cd[0]==LDMARU)||(cd[0]==RDMARU)||         (cd[0]==CROSS) || (cd[0]==UD)||(cd[0]==DUDR)||(cd[0]==DCROSS)||(cd[0]==DUDL))){

			down=YES ;		/*Å@áF  */

	}

	if(cl[1]==0 ){
		if( ((cl[0] == LRU)||(cl[0]==LRD)||(cl[0]==UDR)
		||(cl[0]==LUKAGI)||(cl[0]==LDKAGI) || (cl[0]==LUMARU) 
		|| (cl[0]==LDMARU)|| (cl[0]==CROSS) || (cl[0]==LR) ) ){

			left=YES ;		/*Å@áG  */

		}

		if( ((cl[0] == DLR)||(cl[0]==DUDR)||(cl[0]==DCROSS) ) ){

			left=NIJUYES ;		/*  áH  */

		}
	}

	if(cr[1]==0 ){
		if( (cr[0] == LRU)||(cr[0]==LRD)||(cr[0]==UDL)
		||(cr[0]==RUKAGI)||(cr[0]==RDKAGI) || (cr[0]==RUMARU) 
		|| (cr[0]==RDMARU)||(cr[0]==CROSS) || (cr[0]==LR)  ){

			right=YES ;		/*  áI  */

		}

		if( ((cr[0] == DLR)||(cr[0]==DUDL)||(cr[0]==DCROSS) ) ){

			right=NIJUYES ;		/*  áJ  */

		}
	}


	dainyu = mark;

	switch(mark){
		case FSTLR  :right=YES		;break ;	/*  áK  */
		case ENDLR  :left =YES		;break ;	
		case FSTUD  :down =YES		;break ;
		case ENDUD  :up   =YES		;break ;
		case DFSTLR :right=NIJUYES	;break ;	/*  áL  */
		case DENDLR :left =NIJUYES	;break ;
		case LR     :left =YES    ;right=YES	;break ;
		case UD     :up   =YES    ;down =YES	;break ;
	        case LUKAGI :
		case LUMARU :right=YES	  ;down =YES	;break ;
		case RUKAGI :
		case RUMARU :left =YES    ;down =YES	;break ;
		case LDKAGI :
		case LDMARU :right=YES    ;up   =YES	;break ;
		case RDKAGI :
		case RDMARU :left =YES    ;up   =YES	;break ;
		case DLR    :left =NIJUYES;right=NIJUYES;break ;
	}
	

	if( (left==YES) || (right==YES) )
		dainyu = LR ;			/*  áM  */
	if( (left==NIJUYES) || (right==NIJUYES) )
		dainyu = DLR ;			/*  áN  */
	if( (up==YES) || (down==YES)   )
		dainyu = UD ; 
	if( (up==YES) && (left==YES) && (right==YES) )
		dainyu = LRU ;			/*  áO  */
	if( (down==YES) && (left==YES) && (right==YES) )
		dainyu = LRD ;
	if( (up==YES) && (down==YES) && (left==YES)  )
		dainyu = UDL ; 
	if( (up==YES) && (down==YES) && (left==NIJUYES)  )
		dainyu = DUDL ; 
	if( (up==YES) && (down==YES) && (right==YES)  )
		dainyu = UDR ; 
	if( (up==YES) && (down==YES) && (right==NIJUYES)  )
		dainyu = DUDR ; 
	if( (up==YES) && (down==NO ) && (left==NO ) && (right==YES)  )
		dainyu = LDKAGI;
	if( (up==YES) && (down==NO ) && (left==YES) && (right==NO )  )
		dainyu = RDKAGI;
	if( (up==NO ) && (down==YES) && (left==NO ) && (right==YES)  )
		dainyu = LUKAGI;
	if( (up==NO ) && (down==YES) && (left==YES) && (right==NO )  )
		dainyu = RUKAGI;
	if( (up==YES) && (down==YES) && (left==YES) && (right==YES)  )
		dainyu = CROSS ;			/*  áP  */
	if( (up==YES) && (down==YES) && (left==NIJUYES) && (right==NIJUYES)  )
		dainyu = DCROSS ;

	result=dainyu  ;	/*  áQ  */
	
SPEED :
	moji[0] = result ;	/*  áR  */
	moji[1] = 0    ;	/*  áS  */
	attri[0]=attr  ;
	attri[1]=attr  ;

	vramwr(x,y,moji,attri,ura);	/* (21) */
	
	return(0);
}

