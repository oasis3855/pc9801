;
;	gcolor (in GLIO.LIB)
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
PUBLIC	gcolor
PROC	gcolor
	ARG	fore:WORD, back:WORD, mode:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [fore]
	mov	[bx+3], al
	mov	ax, [back]
	mov	[bx+1], al
	mov	ax, [mode]
	mov	[bx+4], ax

	and	[_gcmode], 0100B	; Clear Color 8/16 flag
	or	[_gcmode], ax		; Set Color 8/16 flag

	push	bp
	int	0A3h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP
END
