;	���̨��� 16 �F���[�h�� PC-9801 �������ݒ�
;	(C)opyright 1991,92,93 ��㔎�v
	.model	tiny	; (COM �`���v���O����)
	.code
	org	100h
start:
	push	cs
	pop	ds
	
	call	gdc_init	; GDC ÷��/���̨����ݒ�
	call	g16_init	; ��گĐݒ�
	call	g_end		; ���̨����o�͒�~
	call	g_cls		; ���̨�����ʏ���
	call	beep_timer_init	; �ް�߉����g���ݒ�

	mov	ax,1000h	; BIOS �������߂̐ݒ�
	int	18h

	mov	dx,offset operation	; MS-DOS �ɂ��÷�ĉ�ʐݒ�
	call	printf_dol

	call	exit		; �I�� (return 0)

;	MS-DOS ÷�� ��ذ� ����  �\��������
operation	db	1bh,'[>5l',1bh,'[>3l',1bh,'[>1l',1bh,')0',1bh,'[2J','$'

;	MS-DOS �W���I���R�}���h
exit	proc	near

	mov	ax,4c00h
	int	21h
	ret
	
exit	endp


;	MS-DOS ������\���R�}���h
printf_dol	proc	near
	;����	DX : '$' �ŏI��镶����ւ̵̾���߲���

	mov	ax,0900h	; MS-DOS Func 09h :������\��
	int	21h
	ret

printf_dol	endp

;	���̨��� BIOS : ���̨����o�͂̒�~
g_end	proc	near
	mov	ax,4100h
	int	18h
	ret
g_end	endp

;	���̨������ݏ��� �֐�
;	G-VRAM �� 00h �Ŗ��߂�
g_cls	proc	near
	push	es
	push	di

	mov	ax,0a800h	; �̈� #0 (A8000 - AFFFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h	; ܰ�ނŲݸ���Ă̈�,����
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	mov	ax,0b000h	; �̈� #1 (B0000 - B7FFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	mov	ax,0b800h	; �̈� #2 (B8000 - BFFFF)
	mov	es,ax
	mov	di,0
	mov	cx,04000h
	xor	ax,ax		; ax = 0
	cld
	rep	stosw

	mov	ax,0e000h	; �g����ݸ (E0000 - E7FFF)
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

; ��گ� �װ �ް� ( GRB �� )
pal_data:
	db	00h,00h,00h	;	��گ� 0
	db	00h,00h,0fh
	db	00h,0fh,00h
	db	00h,0fh,0fh
	db	0fh,00h,00h
	db	0fh,00h,0fh
	db	0fh,0fh,00h
	db	0fh,0fh,0fh	;	��گ� 7
	db	07h,07h,07h	;	��گ� 8
	db	00h,00h,0ah
	db	00h,0ah,00h
	db	00h,0ah,0ah
	db	0ah,00h,00h
	db	0ah,00h,0ah
	db	0ah,0ah,00h
	db	0ah,0ah,0ah	;	��گ� 15

;	16 �F ��گď������֐�
g16_init	proc	near
	xor	bl, bl		; BL = 0 (�����)
g16_loop:
	mov	al, bl		; ��گĔԍ� ( 0 - 15 )
	out	0a8h, al	; ��گĔԍ��w��R�}���h

	mov	al, 3
	mul	bl		; AX = AL * BL
	mov	bp, ax
	mov	al, byte ptr [pal_data + bp]
	out	0aah, al	; ��گ� �� �w��R�}���h

	mov	al, 3
	mul	bl		; AX = AL * BL
	mov	bp, ax
	mov	al, byte ptr [pal_data + bp + 1]
	out	0ach, al	; ��گ� �� �w��R�}���h

	mov	al, 3
	mul	bl		; AX = AL * BL
	mov	bp, ax
	mov	al, byte ptr [pal_data + bp + 2]
	out	0aeh, al	; ��گ� �� �w��R�}���h

	inc	bl		;	BL = BL + 1 (�����)
	cmp	bl, 16
	jae	g16_init_exit	;	if(bl >= 16) �ݒ�I��
	jmp	g16_loop	;	������گĂ�

g16_init_exit:
	ret
g16_init	endp


;	GDC ������
gdc_init	proc	near
	mov	al,07h
	out	68h,al	; pc-98 7*13 ̫��
	mov	al,00h
	out	68h,al	; vertical line Ӱ�� ÷��
	mov	al,02h
	out	68h,al	; �װ ���̨��� Ӱ��
	mov	al,04h
	out	68h,al	; 80 ��� ÷��
	mov	al,08h
	out	68h,al	; 400 ײ� ���̨���
	mov	al,0fh
	out	68h,al	; CRT �\���J�n
	mov	al,01h
	out	6ah,al	; 16 �FӰ��
	mov	al,00h
	out	6ch,al	; �ް�ް�װ ������
	mov	al,00h
	out	0a4h,al	; ���̨��� �\���� ��ݸ 0
	mov	al,00h
	out	0a6h,al	; ���̨��� �������݂� ��ݸ 0
	ret
gdc_init	endp

;	�ް�߉��̎��g���ݒ� (2KHz)
;	PC-9801 E/F/M ���ł͎g�p�s�\
beep_timer_init	proc	near
	; set timer beep intr to cx	10MHz = 1229 , 8MHz = 998
	mov	cx,04cdh	; 10 MHz �n
	push	es
	mov	ax,0000h
	mov	es,ax
	test	es:byte ptr [0501],80h	; PC-9801 ܰ��ر�Q��
	je	pc98_clock_10MHz
	mov	cx,03e6h	; 8MHz �n
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