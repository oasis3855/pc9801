;
;	gload (in GLIO.LIB)
;
;	Copyright (c) 1990 Motohisa Ohno, All rights reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gerrno:WORD
gbuf	DW	3 dup (?)

	CODESEG
PROC	_gloadsub
	push	ds
	mov	ds, ax
	xor	dx, dx
	mov	cx, 7D00h
	mov	ah, 3Fh
	int	21h
	jc	@@err
	cmp	ax, cx
	jnc	@@err
	mov	ax, 14
@@err:
	pop	ds
	ret
ENDP

PUBLIC	gloadall
PROC	gloadall
	ARG	fname:PTR DWORD
	USES	DS
	
	lds	dx, [fname]
	mov	ax, 3D00h
	int	21h
	jc	@@err2
	mov	bx, ax
	
	mov	ax, 0A800h
	call	_gloadsub
	jc	@@err1
	mov	ax, 0B000h
	call	_gloadsub
	jc	@@err1
	mov	ax, 0B800h
	call	_gloadsub
	jc	@@err1
	mov	ax, 0E000h
	call	_gloadsub
	jc	@@err1
	xor	ax, ax

@@err1:	push	ax
	mov	ah, 3Eh
	int	21h
	pop	ax

@@err2:	test	al, al
	jz	@@err3
	mov	ah, 1

@@err3:	mov	[gerrno], ax
	ret
ENDP
END	
