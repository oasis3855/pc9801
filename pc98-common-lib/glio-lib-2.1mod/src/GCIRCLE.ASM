;
;	gcircle (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gcircle
LABEL	gcircle	PROC
	mov	al, 00000000B
	jmp	SHORT __gcircle


PUBLIC	gcirclef
LABEL	gcirclef PROC
	mov	al, 00100000B
	jmp	SHORT __gcircle

PUBLIC	gcirclep
LABEL	gcirclep PROC
	mov	al, 01100000B
	jmp	SHORT __gcircle

PROC	__gcircle
	ARG	px:WORD, py:WORD, radius:WORD, color:WORD, fcolor:WORD
	USES	ds, si, di
	
	push	ax
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	ax, [radius]
	mov	[bx+4], ax
	mov	[bx+6], ax
	mov	ax, [color]
	mov	[bx+8], al
	mov	ax, [fcolor]
	mov	[bx+18], al
	pop	ax
	mov	[BYTE PTR bx+9], al
	test	al, 01000000B
	jz	@@1
	mov	al, [0027h]
	mov	[bx+18], al
	mov	[WORD PTR bx+19], 0028h
	mov	[bx+21], ds
@@1:
	push	bp
	int	0A8h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
