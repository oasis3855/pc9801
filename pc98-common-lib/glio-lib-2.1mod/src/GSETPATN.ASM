;
;	gsetpattern (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gsetpattern
PROC	gsetpattern
	ARG	ln:WORD, pattern:PTR DWORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	cx, [ln]
	mov	[0027h], cl
	xor	ch, ch
	push	ds
	pop	es
	mov	di, 0028h
	lds	si, [pattern]
	rep movsb
	xor	ax, ax
	mov	[gerrno], ax
	ret
ENDP
END
