;
;	gpset (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gpset
PROC	gpset
	ARG	px:WORD, py:WORD, color:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	ax, [color]
	mov	[bx+4], al
	mov	ah, 1
	push	bp
	int	0A6h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret	
ENDP
END
