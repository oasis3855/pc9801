/*       1990 E.Toyokuni      Number IS01900902  */
/*      　　　　　　　　　　　　　　 CM900807    */
#include "win_i1.h"

				/*  整数値のスワップ */
void iswap(
	int *p,
	int *q )

{
	int dummy ;
	dummy = *p ;	/*  ①  */
	*p = *q ;	/*　②  */
	*q = dummy ;	/*　③  */
}

