;
;	garc (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD
	EXTRN	_sintbl:WORD

	CODESEG

;	AX = angle
LABEL	_g_cos	NEAR
	add	ax, 90

PROC	_g_sin	NEAR
	cwd
	mov	cx, 360
	idiv	cx
	mov	bx, dx
	shl	bx, 1
	test	bx, bx
	jns	@@1
	neg	bx
@@1:	cmp	bx, 90*2
	ja	@@2
	mov	ax, [_sintbl+bx]
	jmp	SHORT @@q
@@2:	cmp	bx, 180*2
	ja	@@3
	sub	bx, 180*2
	neg	bx
	mov	ax, [_sintbl+bx]
	jmp	SHORT @@q
@@3:	cmp	bx, 270*2
	ja	@@4
	sub	bx, 180*2
	mov	ax, [_sintbl+bx]
	neg	ax
	jmp	SHORT @@q
@@4:	sub	bx, 360*2
	neg	bx
	mov	ax, [_sintbl+bx]
	neg	ax
@@q:	ret
ENDP

PUBLIC	garc
LABEL	garc	PROC
	mov	al, 00000101B
	jmp	SHORT __garc

PUBLIC	garcp
LABEL	garcp	PROC
	mov	al, 01100101B
	jmp	SHORT __garc

PUBLIC	gpie
LABEL	gpie	PROC
	mov	al, 00001111B
	jmp	SHORT __garc

PUBLIC	gpiep
LABEL	gpiep	PROC
	mov	al, 01101111B
	jmp	SHORT __garc

PROC	__garc
	ARG px:WORD,py:WORD,rx:WORD,ry:WORD,color:WORD,sang:WORD,eang:WORD
	USES	ds, si, di

	push	ax

	mov	ax, [sang]
	call	_g_cos
	imul	[rx]
	mov	cx, 1000h
	idiv	cx
	add	ax, [px]
	push	ax

	mov	ax, [sang]
	call	_g_sin
	imul	[ry]
	mov	cx, 1000h
	idiv	cx
	neg	ax
	add	ax, [py]
	push	ax

	mov	ax, [eang]
	call	_g_cos
	imul	[rx]
	mov	cx, 1000h
	idiv	cx
	add	ax, [px]
	push	ax

	mov	ax, [eang]
	call	_g_sin
	imul	[ry]
	mov	cx, 1000h
	idiv	cx
	neg	ax
	add	ax, [py]
	push	ax
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	ax, [rx]
	mov	[bx+4], ax
	mov	ax, [ry]
	mov	[bx+6], ax
	mov	ax, [color]
	mov	[bx+8], al
	mov	[bx+18], al

	pop	[WORD PTR bx+16]
	pop	[WORD PTR bx+14]
	pop	[WORD PTR bx+12]
	pop	[WORD PTR bx+10]	
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

PUBLIC	garcf
LABEL	garcf	PROC
	mov	al, 00100101B
	jmp	SHORT __garcf

PUBLIC	gpief
LABEL	gpief	PROC
	mov	ax, 00101111B
	jmp	SHORT __garcf

PUBLIC	__garcf
PROC	__garcf
	ARG px:WORD,py:WORD,rx:WORD,ry:WORD,col:WORD,fcol:WORD,sa:WORD,ea:WORD
	USES	ds, si, di

	push	ax

	mov	ax, [sa]
	call	_g_cos
	imul	[rx]
	mov	cx, 1000h
	idiv	cx
	add	ax, [px]
	push	ax

	mov	ax, [sa]
	call	_g_sin
	imul	[ry]
	mov	cx, 1000h
	idiv	cx
	neg	ax
	add	ax, [py]
	push	ax

	mov	ax, [ea]
	call	_g_cos
	imul	[rx]
	mov	cx, 1000h
	idiv	cx
	add	ax, [px]
	push	ax

	mov	ax, [ea]
	call	_g_sin
	imul	[ry]
	mov	cx, 1000h
	idiv	cx
	neg	ax
	add	ax, [py]
	push	ax
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	ax, [rx]
	mov	[bx+4], ax
	mov	ax, [ry]
	mov	[bx+6], ax
	mov	ax, [col]
	mov	[bx+8], al
	mov	ax, [fcol]
	mov	[bx+18], al
	pop	[WORD PTR bx+16]
	pop	[WORD PTR bx+14]
	pop	[WORD PTR bx+12]
	pop	[WORD PTR bx+10]	
	pop	ax
	mov	[BYTE PTR bx+9], al
	push	bp
	int	0A8h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
