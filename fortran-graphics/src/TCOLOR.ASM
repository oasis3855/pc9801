;	Change Color エスケープシーケンス
;	void tcolor(int color);
;


	IDEAL
	MODEL LARGE, C

	EXTRN	tputnum:PROC

	DATASEG
mes1	db	1bh,'[$'

	CODESEG
PUBLIC	tcolor
PROC	tcolor
	ARG	color:WORD
	USES	ds, si, di

	mov	dx, offset mes1
	mov	ah, 09h
	int 21h

	mov	ax, [color]
	push	ax
	call	tputnum
	pop	ax

	mov	dl, 'm'
	mov ah, 02h
	int 21h

	ret
ENDP

PUBLIC	tcolorm
PROC	tcolorm
	ARG	color:WORD, attr1:WORD, attr2:WORD
	USES	ds, si, di

	mov	dx, offset mes1
	mov	ah, 09h
	int 21h

	mov	ax, [color]
	push	ax
	call	tputnum
	pop	ax

	mov	ax, [attr1]
	cmp	ax, 0
	jz	tcolorm_l1
	mov	dl, ';'
	mov	ah, 02h
	int	21h
	mov	ax, [attr1]
	push	ax
	call	tputnum
	pop	ax
tcolorm_l1:

	mov	ax, [attr2]
	cmp	ax, 0
	jz	tcolorm_l2
	mov	dl, ';'
	mov	ah, 02h
	int	21h
	mov	ax, [attr2]
	push	ax
	call	tputnum
	pop	ax
tcolorm_l2:

	mov	dl, 'm'
	mov ah, 02h
	int 21h

	ret
ENDP

END


