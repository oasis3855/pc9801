;	Text Clear DOS ´½¹°Ìß•\Ž¦
;	void tcls(void);
;

	IDEAL
	MODEL LARGE, C

	DATASEG

tcls_mes1	db	01bh,'[2J$'

	CODESEG
PUBLIC	tcls
PROC	tcls
	USES	ds, si, di

	mov	dx, offset tcls_mes1
	mov	ah, 09h
	int 21h
	ret
ENDP
END
