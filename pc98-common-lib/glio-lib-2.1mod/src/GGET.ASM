;
;	gget (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C

	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gget
PROC	gget
	ARG	px1:WORD, py1:WORD, px2:WORD, py2:WORD, buffer:PTR DWORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px1]
	mov	[bx+0], ax
	mov	ax, [py1]
	mov	[bx+2], ax
	mov	ax, [px2]
	mov	[bx+4], ax
	mov	ax, [py2]
	mov	[bx+6], ax
	les	ax, [buffer]
	mov	[bx+8], ax
	mov	[bx+10], es
	mov	[WORD PTR bx+12], -1
	push	bp
	int	0ABh
	pop	bp
	mov	ah, 5
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
