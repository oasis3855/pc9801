;
;	gliowork (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD

	CODESEG
PUBLIC	gliowork
PROC	gliowork
	mov	ax, 0620h
	mov	dx, [WORD PTR gworkp+2]
	ret
ENDP
END
