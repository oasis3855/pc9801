;
;	gpaintp (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gpaintp
PROC	gpaintp
	ARG	px:WORD, py:WORD, border:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	al, [0027h]
	mov	[bx+5], al
	mov	[WORD PTR bx+6], 0028h
	mov	[bx+8], ds
	mov	ax, [border]
	mov	[bx+10], al
	mov	[WORD PTR bx+16], 0ff0h
	mov	[WORD PTR bx+18], 0af0h
	push	bp
	int	0AAh
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
