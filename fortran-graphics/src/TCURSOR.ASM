;	Cursor エスケープシーケンス
;
;


	IDEAL
	MODEL LARGE, C

	EXTRN	tputnum:PROC

	DATASEG
mes1	db	1bh,'[$'
mes_fon	db	'>1l$'
mes_foff	db	'>1h$'
mes_l20	db	'>3h$'
mes_l25	db	'>3l$'
mes_con	db	'>5l$'
mes_coff	db	'>5h$'

	CODESEG
PUBLIC	tlocate
PROC	tlocate
	ARG	x:WORD, y:WORD
	USES	ds, si, di

	mov	dx, offset mes1
	mov	ah, 09h
	int 21h

	mov	ax, [y]
	push	ax
	call	tputnum
	pop	ax

	mov	dl, ';'
	mov ah, 02h
	int 21h

	mov	ax, [x]
	push	ax
	call	tputnum
	pop	ax

	mov	dl, 'H'
	mov ah, 02h
	int 21h

	ret
ENDP

PUBLIC	tfunc
PROC	tfunc
	ARG	sw:WORD
	USES	ds, si, di

	mov	dx, offset mes1
	mov	ah, 09h
	int 21h

	mov	ax, [sw]
	cmp	ax, 0
	jz	@@1

	mov	dx, offset mes_fon
	mov	ah, 09h
	int 21h
	jmp @@2

@@1:
	mov	dx, offset mes_foff
	mov	ah, 09h
	int 21h
@@2:
	ret
ENDP

PUBLIC	tline25
PROC	tline25
	ARG	sw:WORD
	USES	ds, si, di

	mov	dx, offset mes1
	mov	ah, 09h
	int 21h

	mov	ax, [sw]
	cmp	ax, 0
	jz	@@1

	mov	dx, offset mes_l25
	mov	ah, 09h
	int 21h
	jmp @@2

@@1:
	mov	dx, offset mes_l20
	mov	ah, 09h
	int 21h
@@2:
	ret
ENDP

PUBLIC	tcursor
PROC	tcursor
	ARG	sw:WORD
	USES	ds, si, di

	mov	dx, offset mes1
	mov	ah, 09h
	int 21h

	mov	ax, [sw]
	cmp	ax, 0
	jz	@@1

	mov	dx, offset mes_con
	mov	ah, 09h
	int 21h
	jmp @@2

@@1:
	mov	dx, offset mes_coff
	mov	ah, 09h
	int 21h
@@2:
	ret
ENDP

END
