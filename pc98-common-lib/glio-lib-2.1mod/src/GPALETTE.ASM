;
;	gpalette (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gpalette
PROC	gpalette
	ARG	no:WORD, color:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [no]
	mov	[bx+0], al
	mov	ax, [color]
	mov	[bx+1], ax
	push	bp
	int	0A4h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
