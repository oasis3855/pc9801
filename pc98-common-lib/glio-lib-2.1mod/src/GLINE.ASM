;
;	gline (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gline
LABEL	gline	PROC
	mov	ax, 0000h
	jmp	SHORT __gline

PUBLIC	glines
LABEL	glines	PROC
	mov	ax, 0100h
	jmp	SHORT __gline

PUBLIC	gbox
LABEL	gbox	PROC
	mov	ax, 0001h
	jmp	SHORT __gline

PUBLIC	gboxs
LABEL	gboxs	PROC
	mov	ax, 0101h
	jmp	SHORT __gline

PUBLIC	gbar
LABEL	gbar	PROC
	mov	ax, 0002h
	jmp	SHORT __gline

PUBLIC	gbarf
LABEL	gbarf	PROC
	mov	ax, 0102h
	jmp	SHORT __gline

PUBLIC	gbarp
LABEL	gbarp	PROC
	mov	ax, 0202h
	jmp	SHORT __gline

PROC	__gline
	ARG	px1:WORD, py1:WORD, px2:WORD, py2:WORD, color:WORD, ext:WORD
	USES	ds, si, di
	
	push	ax
	lds	bx, [gworkp]
	mov	ax, [px1]
	mov	[bx+0], ax
	mov	ax, [py1]
	mov	[bx+2], ax
	mov	ax, [px2]
	mov	[bx+4], ax
	mov	ax, [py2]
	mov	[bx+6], ax
	mov	ax, [color]
	mov	[bx+8], al
	pop	[WORD PTR bx+9]
	mov	ax, [ext]
	mov	[bx+11], ax
	mov	al, [0027h]
	mov	[bx+13], al
	mov	[WORD PTR bx+14], 0028h
	mov	[bx+16], ds
	push	bp
	int	0A7h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
