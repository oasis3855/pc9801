;	Text putcode ï∂éöÅióÒÅjï\é¶
;
;

	IDEAL
	MODEL LARGE, C

	CODESEG
PUBLIC	tputchar
PROC	tputchar
	ARG	data:WORD
	USES	ds, si, di

	mov	ax, [data]
	mov	dl, al
	mov	ah, 02h
	int	21h
	ret
ENDP

PUBLIC	tputs
PROC	tputs
	ARG	string:PTR DWORD
	USES	ds, si, di

	lds	si, [string]
@@1:	lodsb
	test	al, al
	jz	@@2
	xor	ah, ah
	push	ax
	call	tputchar
	pop	ax
	jmp	@@1
@@2:
	ret
ENDP

END
