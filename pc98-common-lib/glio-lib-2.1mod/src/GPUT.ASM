;
;	gput (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gput
PROC	gput
	ARG	px:WORD, py:WORD, mode:WORD, buffer:PTR DWORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	les	ax, [buffer]
	mov	[bx+4], ax
	mov	[bx+6], es
	mov	[WORD PTR bx+8], -1
	mov	ax, [mode]
	mov	[bx+10], al
	mov	[BYTE PTR bx+11], 0

	mov	[BYTE PTR bx+12], 0fh	; Mono mode 1
	mov	[BYTE PTR bx+13], 0	; Mono mode 0

	push	bp
	int	0ACh
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
