/* グラフィックス消去　インラインアセンブラ版 */
#include<stdio.h>

#pragma inline

void PC98_grph_cls(void)
{

	asm{

	push	es
	push	di
	push	dx

	mov	ax,0a800h
	mov	es,ax
	mov	di,0
	mov	cx,03e80h
	xor	ax,ax
	cld
	rep	stosw

	mov	ax,0b000h
	mov	es,ax
	mov	di,0
	mov	cx,03e80h
	xor	ax,ax
	cld
	rep	stosw

	mov	ax,0b800h
	mov	es,ax
	mov	di,0
	mov	cx,03e80h
	xor	ax,ax
	cld
	rep	stosw

	mov	ax,0e000h
	mov	es,ax
	mov	di,0
	mov	cx,03e80h
	xor	ax,ax
	cld
	rep	stosw

	pop	dx
	pop	di
	pop	es

	}

}
