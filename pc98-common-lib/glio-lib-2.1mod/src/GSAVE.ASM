;
;	gsave (in GLIO.LIB)
;
;	Copyright (c) 1990 Motohisa Ohno, All Righrs Reserved.
;
	IDEAL
	MODEL LARGE, C

	DATASEG
	EXTRN	gerrno:WORD
	
	CODESEG
PROC	_gsavesub
	push	ds
	mov	ds, ax
	xor	dx, dx
	mov	cx, 7D00h
	mov	ah, 40h
	int	21h
	jc	@@err
	cmp	ax, cx
	jnc	@@err
	mov	ax, 14
@@err:
	pop	ds
	ret
ENDP

PUBLIC	gsaveall
PROC	gsaveall
	ARG	fname:PTR DWORD
	USES	DS
	
	lds	dx, [fname]
	xor	cx, cx
	mov	ah, 3Ch		; creat handle
	int	21h
	jc	@@err2
	mov	bx, ax

	mov	ax, 0A800h
	call	_gsavesub
	jc	@@err1
	mov	ax, 0B000h
	call	_gsavesub
	jc	@@err1
	mov	ax, 0B800h
	call	_gsavesub
	jc	@@err1
	mov	ax, 0E000h
	call	_gsavesub
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
