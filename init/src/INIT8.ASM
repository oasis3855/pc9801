;	ｸﾞﾗﾌｨｯｸｽ 8 色モードで PC-9801 を初期設定
;	(C)opyright 1991,92,93 井上博計
	.model	tiny	; (COM 形式プログラム)
	.code
	org	100h
start:
	push	cs
	pop	ds
	
	call	gdc_init	; GDC ﾃｷｽﾄ/ｸﾞﾗﾌｨｯｸｽ設定
	call	g8_init	; ﾊﾟﾚｯﾄ設定
	call	g_end		; ｸﾞﾗﾌｨｯｸｽ出力停止
	call	g_cls		; ｸﾞﾗﾌｨｯｸｽ画面消去
	call	beep_timer_init	; ﾋﾞｰﾌﾟ音周波数設定

	mov	ax,1000h	; BIOS ｶｰｿﾙﾀｲﾌﾟの設定
	int	18h

	mov	dx,offset operation	; MS-DOS によるﾃｷｽﾄ画面設定
	call	printf_dol

	call	exit		; 終了 (return 0)

;	MS-DOS ﾃｷｽﾄ ｽｸﾘｰﾝ 制御  表示文字列
operation	db	1bh,'[>5l',1bh,'[>3l',1bh,'[>1l',1bh,')0',1bh,'[2J','$'

;	MS-DOS 標準終了コマンド
exit	proc	near

	mov	ax,4c00h
	int	21h
	ret
	
exit	endp


;	MS-DOS 文字列表示コマンド
printf_dol	proc	near
	;引数	DX : '$' で終わる文字列へのｵﾌｾｯﾄﾎﾟｲﾝﾀｰ

	mov	ax,0900h	; MS-DOS Func 09h :文字列表示
	int	21h
	ret

printf_dol	endp

;	ｸﾞﾗﾌｨｯｸｽ BIOS : ｸﾞﾗﾌｨｯｸｽ出力の停止
g_end	proc	near
	mov	ax,4100h
	int	18h
	ret
g_end	endp

;	ｸﾞﾗﾌｨｯｸｽｶﾞﾒﾝ消去 関数
;	G-VRAM を 00h で埋める
g_cls	proc	near
	push	es
	push	di

	mov	ax,0a800h	; 領域 #0 (A8000 - AFFFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h	; ﾜｰﾄﾞでｲﾝｸﾘﾒﾝﾄの為,半分
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	mov	ax,0b000h	; 領域 #1 (B0000 - B7FFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	mov	ax,0b800h	; 領域 #2 (B8000 - BFFFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	mov	ax,0e000h	; 拡張ﾊﾞﾝｸ (E0000 - E7FFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	pop	di
	pop	es
	ret
g_cls	endp

;	8 色 ﾊﾟﾚｯﾄ初期化関数
g8_init	proc	near
	mov	al, 37h
	out	0a8h, al
	mov	al, 15h
	out	0aah, al
	mov	al, 26h
	out	0ach, al
	mov	al, 04h
	out	0aeh, al

	ret
g8_init	endp


;	GDC 初期化
gdc_init	proc	near
	mov	al,07h
	out	68h,al	; pc-98 7*13 ﾌｫﾝﾄ
	mov	al,00h
	out	68h,al	; vertical line ﾓｰﾄﾞ ﾃｷｽﾄ
	mov	al,02h
	out	68h,al	; ｶﾗｰ ｸﾞﾗﾌｨｯｸｽ ﾓｰﾄﾞ
	mov	al,04h
	out	68h,al	; 80 ｶﾗﾑ ﾃｷｽﾄ
	mov	al,08h
	out	68h,al	; 400 ﾗｲﾝ ｸﾞﾗﾌｨｯｸｽ
	mov	al,0fh
	out	68h,al	; CRT 表示開始
	mov	al,00h
	out	6ah,al	; 8 色ﾓｰﾄﾞ
	mov	al,00h
	out	6ch,al	; ﾎﾞｰﾀﾞｰｶﾗｰ を消す
	mov	al,00h
	out	0a4h,al	; ｸﾞﾗﾌｨｯｸｽ 表示は ﾊﾞﾝｸ 0
	mov	al,00h
	out	0a6h,al	; ｸﾞﾗﾌｨｯｸｽ 書き込みは ﾊﾞﾝｸ 0
	ret
gdc_init	endp

;	ﾋﾞｰﾌﾟ音の周波数設定 (2KHz)
;	PC-9801 E/F/M 等ではK使用不可能
beep_timer_init	proc	near
	; set timer beep intr to cx	10MHz = 1229 , 8MHz = 998
	mov	cx,04cdh	; 10 MHZ 系
	push	es
	mov	ax,0000h
	mov	es,ax
	test	es:byte ptr [0501],80h	; PC-9801 ﾜｰｸｴﾘｱ参照
	je	pc98_clock_10MHz
	mov	cx,03e6h	; 8 MHz 系
pc98_clock_10MHz:
	pop	es

	mov	dx,03fdfh
	mov	al,7eh
	out	dx,al	; timer mode
	mov	cx,04cdh

	mov	dx,03fdbh
	mov	al,cl	; load LSB
	out	dx,al
	nop
	nop

	mov	al,ch	; load MSB
	out	dx,al
	nop
	nop

	ret
beep_timer_init	endp

end	start
