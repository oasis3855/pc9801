;	keyboard input
;
;

	IDEAL
	MODEL LARGE, C


	CODESEG
PUBLIC	tkbhit
PROC	tkbhit
	USES	ds, si, di

	mov	ah, 01h
	int	18h
	xor	ah, ah
	mov	al, bh
	ret
ENDP

PUBLIC	tgetch
PROC	tgetch
	USES	ds, si, di

	mov	ah, 00h
	int	18h
	xor	ah, ah
	ret
ENDP

END
