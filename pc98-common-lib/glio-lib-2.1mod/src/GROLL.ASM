;
;	groll (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	groll
LABEL	groll	PROC
	mov	al, 1
	jmp	SHORT __groll

PUBLIC	groll0
LABEL	groll0	PROC
	mov	al, 0
	jmp	SHORT __groll

PROC	__groll
	ARG	xdots:WORD, ydots:WORD
	USES	ds, si, di
	
	push	ax
	lds	bx, [gworkp]
	mov	ax, [ydots]
	mov	[bx+0], ax
	mov	ax, [xdots]
	mov	[bx+2], ax
	pop	ax
	mov	[BYTE PTR bx+4], al
	push	bp
	int	0AEh
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
