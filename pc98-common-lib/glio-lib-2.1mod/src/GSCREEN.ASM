;
;	gscreen (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD
	EXTRN	_gcmode:WORD

	CODESEG
PUBLIC	gscreen
PROC	gscreen
	ARG	mode:WORD, sw:WORD, active:WORD, visual:WORD
	USES	ds, si, di

	lds	bx, [gworkp]
	mov	ax, [mode]
	mov	[bx+0], al

	cmp	ax, 1
	je	@@CMono
	cmp	ax, 2
	je	@@CMono
	mov	ax, 011B	; Color 8/16 flag ON
	and	[_gcmode], ax	; Color mode
	jmp	SHORT @@Cend
@@CMono:
	mov	ax,0100B	; Monochrome flag
	or	[_gcmode], ax	; Color flag MONO
@@Cend:

	mov	ax, [sw]
	mov	[bx+1], al
	mov	ax, [active]
	mov	[bx+2], al
	mov	ax, [visual]
	mov	[bx+3], al
	push	bp
	int	0A1h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
