;
;	gpaint (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gpaint
PROC	gpaint
	ARG	px:WORD, py:WORD, color:WORD, border:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	ax, [color]
	mov	[bx+4], al
	mov	ax, [border]
	mov	[bx+5], al
	mov	[WORD PTR bx+6], 0ff0h
	mov	[WORD PTR bx+8], 0af0h
	push	bp
	int	0A9h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
