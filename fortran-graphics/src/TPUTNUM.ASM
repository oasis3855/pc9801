;	Print number 数値表示
;	void tputnum(int num);
;


	IDEAL
	MODEL LARGE, C

	CODESEG
PUBLIC	tputnum
PROC	tputnum
	ARG	num:WORD
	USES	ds, si, di

	mov	bx,[num]
	test	bx, 8000h	; 負の数をテスト
	jz	tputnum_l1
	mov	dl, '-'
	mov	ah, 02h
	int	21h
	xor	bx, 0ffffh		; 符号クリアー
	add	bx, 1			; 負数補正
tputnum_l1:

	xor cx,cx			; cx = 0 数値が現れたら 1

	xor	dx, dx			; dx = 0
	mov	ax, bx
	mov	bx, 10000
	div	bx				; DX+AX / BX = AX ... DX
	push	dx			; 余り dx を保存
	cmp	al, 0
	jz	tputnum_l2
	add	al, '0'
	mov	dl, al
	mov	ah, 02h
	int	21h				; 10000 の位表示
	mov	cx, 1
tputnum_l2:

	xor	dx, dx			; dx = 0
	pop ax					; 余り
	mov	bx, 1000
	div	bx				; DX+AX / BX = AX ... DX
	push	dx			; 余り dx を保存
	cmp	cx, 0
	jnz	tputnum_l31
	cmp	al, 0
	jz	tputnum_l3
tputnum_l31:
	add	al, '0'
	mov	dl, al
	mov	ah, 02h
	int	21h				; 1000 の位表示
	mov	cx, 1
tputnum_l3:

	xor	dx, dx			; dx = 0
	pop ax					; 余り
	mov	bx, 100
	div	bx				; DX+AX / BX = AX ... DX
	push	dx			; 余り dx を保存
	cmp	cx, 0
	jnz	tputnum_l41
	cmp	al, 0
	jz	tputnum_l4
tputnum_l41:
	add	al, '0'
	mov	dl, al
	mov	ah, 02h
	int	21h				; 100 の位表示
	mov	cx, 1
tputnum_l4:

	xor	dx, dx			; dx = 0
	pop ax					; 余り
	mov	bx, 10
	div	bx				; DX+AX / BX = AX ... DX
	push	dx			; 余り dx を保存
	cmp	cx, 0
	jnz	tputnum_l51
	cmp	al, 0
	jz	tputnum_l5
tputnum_l51:
	add	al, '0'
	mov	dl, al
	mov	ah, 02h
	int	21h				; 10 の位表示
tputnum_l5:

	pop	dx				; 余り
	add	dl, '0'
	mov ah, 02h
	int 21h


	ret
ENDP
END