;
;	ggetsize (in GLIO.LIB)
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
PUBLIC	ggetsize
PROC	ggetsize
	ARG	px1:WORD, py1:WORD, px2:WORD, py2:WORD
	USES	ds

	lds	ax, [gworkp]
	mov	ax, [px2]
	sub	ax, [px1]
	mov	cx, 3
	shr	ax, cl
	inc	ax
	mov	bx, ax
	mov	ax, [py2]
	sub	ax, [py1]
	inc	ax
;	cmp	[BYTE PTR 0620h], 1
;	je	@@1
;	cmp	[BYTE PTR 0620h], 2
;	je	@@1
;
	test	[_gcmode], 2
	jnz	@@C16		; 16 Color
	test	[_gcmode], 4
	jnz	@@CMono		; Monochrome
	mov	cx, 3		; 8 Color
	jmp	SHORT @@Cend
@@C16:	mov	cx, 4		; 16 Color
	jmp	SHORT @@Cend
@@CMono:
	mov	cx, 1		; Monochrome
@@Cend:
;
	mul	cx
	test	dx, dx
	jnz	@@2
@@1:	mul	bx
	test	dx, dx
	jnz	@@2
	add	ax, 4
	jmp	SHORT @@3
@@2:	mov	ax, -1
@@3:	ret
ENDP
END
