;
;	gputs (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C

	EXTRN	gputch:PROC
	
	CODESEG
PUBLIC	gputs
PROC	gputs
	ARG	string:PTR DWORD
	USES	ds, si
	
	lds	si, [string]
@@1:	lodsb
	test	al, al
	jz	@@2
	xor	ah, ah
	push	ax
	call	gputch
	pop	cx
	jmp	@@1
@@2:	mov	ax, cx
	ret
ENDP
END
