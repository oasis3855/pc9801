;
;	gpoint (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gpoint
PROC	gpoint
	ARG	px:WORD, py:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	push	ds
	pop	es
	push	bp
	int	0AFh
	pop	bp
	push	ax
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	pop	ax
	cbw
	ret
ENDP
END
