;
;	gview (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gview
PROC	gview
	ARG	px1:WORD, py1:WORD, px2:WORD, py2:WORD, area:WORD, border:WORD
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
	mov	ax, [area]
	mov	[bx+8], al
	mov	ax, [border]
	mov	[bx+9], al
	push	bp
	int	0A2h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
