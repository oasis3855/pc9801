; *******************************************************************
; FD_MOFF2.ASM
;	1MB/640KB �t���b�s�[�f�B�X�N�@���[�^�[��~
;	 PC-9801 �̃��[�N�G���A���Q�Ƃ��A������
;
;	1992.02	���@���v
; *******************************************************************

	.model	tiny		; COM �`��

	.code
	org	100h

Start:

	mov	dx,offset byte ptr MesCopyR
	call	printf_doll	; ���b�Z�[�W�o��

LoopTop:

	mov	CL,[byte ptr _counter]
	mov	AL,1
	shl	AL,CL		; DISK_EQUIP �`���֕ϊ� (DA/UA)
	xor	BX,BX		; BX = 0
	push	BX
	pop	ES		; ES := BX
	test	byte ptr ES:[055ch],AL	; 0000:055c AND AL
	jz	NextUnit	; ���݂��Ȃ� FDD UNIT

	mov	AH,83h		; FDD ���[�^��~
	mov	AL,90h
	or	AL,[byte ptr _counter]	; FDD ���j�b�g�ԍ� (DA/UA) 90 - 93
	int	1bh		; DISK BIOS
	jc	NotSupport	; ���[�^��~���Ή��@��

	mov	dx,offset byte ptr MesUnit
	call	printf_doll	; ���b�Z�[�W�o��
	mov	dl,[byte ptr _counter]
	add	dl,30h		; ASCII �R�[�h��
	mov	ah,02h		; �P�����o��
	int	21h		; DOS INT
	mov	dx,offset byte ptr MesOK
	call	printf_doll	; ���b�Z�[�W�o��
	jmp	short NextUnit

NotSupport:
	mov	dx,offset byte ptr MesYELLOW
	call	printf_doll	; ���b�Z�[�W�o��
	mov	dx,offset byte ptr MesUnit
	call	printf_doll	; ���b�Z�[�W�o��
	mov	dl,[byte ptr _counter]
	add	dl,30h		; ASCII �R�[�h��
	mov	ah,02h		; �P�����o��
	int	21h		; DOS INT
	mov	dx,offset byte ptr MesNG
	call	printf_doll	; ���b�Z�[�W�o��
	mov	dx,offset byte ptr MesNORMC
	call	printf_doll	; ���b�Z�[�W�o��

NextUnit:
	inc	byte ptr [byte ptr _counter]	; ���̃��j�b�g
	cmp	[byte ptr _counter],4
	jz	ProgramEnd		; �ŏI�h���C�u + 1 ���H
	jmp	LoopTop

ProgramEnd:

	call	exit		; DOS �ɖ߂�B�I��

_counter	DB	0	; FDD �𐔂���J�E���^ 0 - 3

MesUnit		DB	'�ۯ�߰ �ި�� �Ư� $'
MesOK		DB	' �����[�^������~���[�h�ɐݒ肵�܂���',0dh,0ah,'$'
MesNG		DB	' �̓��[�^������~���[�h�̓T�|�[�g����܂���',0dh,0ah,'$'
MesYELLOW	DB	1bh,'[21m$'
MesNORMC	DB	1bh,'[m$'
MesCopyR	DB	'FD_MOFF',09h,09h,09h,09h,09h,'(C) 1992 ��� ���v',0dh,0ah,'$'

exit	proc	near

	; DOS �v���O�����I������
	mov	ax,4c00h
	int	21h
	ret
	
exit	endp

printf_doll	proc	near
	;CALL	DX:OFFSET of string ('$'�ŏI��镶����)
	;
	;RET	����
	;
	;DOS 21H Func 09H ������̕W���o��

	mov	ax,0900h
	int	21h
	ret

printf_doll	endp

end	Start			; �v���O�����I��

