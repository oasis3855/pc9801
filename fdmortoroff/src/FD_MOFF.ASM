; *******************************************************************
; FD_MOFF.ASM
;	1MB/640KB フロッピーディスク　モーター停止
;	 1台ずつ装置を初期化して1MB FDD を確認
;
;	1992.02	井上　博計
; *******************************************************************

	.model	tiny		; COM 形式

	.code
	org	100h

Start:

	mov	dx,offset byte ptr MesCopyR
	call	printf_doll	; メッセージ出力

LoopTop:
	mov	AH,03h		; FDD INITIALIZE , 装置の初期化
	mov	AL,90h
	or	AL,[byte ptr _counter]	; FDD ユニット番号 (DA/UA) 90 - 93
	int	1bh		; DISK BIOS
	jc	NextUnit	; 1MB FDD で無い時

	mov	CL,[byte ptr _counter]
	mov	AL,1
	shl	AL,CL		; DISK_EQUIP 形式へ変換 (DA/UA)
	xor	BX,BX		; BX = 0
	push	BX
	pop	ES		; ES := BX
	test	byte ptr ES:[055ch],AL	; 0000:055c AND AL
	jz	NextUnit	; 存在しない FDD UNIT

	mov	AH,83h		; FDD モータ停止
	mov	AL,90h
	or	AL,[byte ptr _counter]	; FDD ユニット番号 (DA/UA) 90 - 93
	int	1bh		; DISK BIOS
	jc	NotSupport	; モータ停止未対応機種

	mov	dx,offset byte ptr MesUnit
	call	printf_doll	; メッセージ出力
	mov	dl,[byte ptr _counter]
	add	dl,30h		; ASCII コードへ
	mov	ah,02h		; １文字出力
	int	21h		; DOS INT
	mov	dx,offset byte ptr MesOK
	call	printf_doll	; メッセージ出力
	jmp	short NextUnit

NotSupport:
	mov	dx,offset byte ptr MesYELLOW
	call	printf_doll	; メッセージ出力
	mov	dx,offset byte ptr MesUnit
	call	printf_doll	; メッセージ出力
	mov	dl,[byte ptr _counter]
	add	dl,30h		; ASCII コードへ
	mov	ah,02h		; １文字出力
	int	21h		; DOS INT
	mov	dx,offset byte ptr MesNG
	call	printf_doll	; メッセージ出力
	mov	dx,offset byte ptr MesNORMC
	call	printf_doll	; メッセージ出力

NextUnit:
	inc	byte ptr [byte ptr _counter]	; 次のユニット
	cmp	[byte ptr _counter],4
	jz	ProgramEnd		; 最終ドライブ + 1 か？
	jmp	LoopTop

ProgramEnd:

	call	exit		; DOS に戻る。終了

_counter	DB	0	; FDD を数えるカウンタ 0 - 3

MesUnit		DB	'ﾌﾛｯﾋﾟｰ ﾃﾞｨｽｸ ﾕﾆｯﾄ $'
MesOK		DB	' をモータ自動停止モードに設定しました',0dh,0ah,'$'
MesNG		DB	' はモータ自動停止モードはサポートされません',0dh,0ah,'$'
MesYELLOW	DB	1bh,'[21m$'
MesNORMC	DB	1bh,'[m$'
MesCopyR	DB	'FD_MOFF',09h,09h,09h,09h,09h,'(C) 1992 井上 博計',0dh,0ah,'$'

exit	proc	near

	; DOS プログラム終了命令
	mov	ax,4c00h
	int	21h
	ret
	
exit	endp

printf_doll	proc	near
	;CALL	DX:OFFSET of string ('$'で終わる文字列)
	;
	;RET	無し
	;
	;DOS 21H Func 09H 文字列の標準出力

	mov	ax,0900h
	int	21h
	ret

printf_doll	endp

end	Start			; プログラム終了

