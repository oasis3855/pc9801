;
;	gputcode (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C
	
	DATASEG
	EXTRN	gworkp:DWORD
	EXTRN	gerrno:WORD
	EXTRN	_gpx:WORD
	EXTRN	_gpy:WORD
	EXTRN	_gpmode:WORD
	EXTRN	_gtabsiz:BYTE

_kanji	DB	0

	CODESEG
PROC	getline	NEAR
	mov	es, [WORD PTR gworkp+2]
	mov	dx, 200
	cmp	[BYTE PTR es:0620h], 2
	jb	@@1
	mov	dx, 400
@@1:	ret
ENDP

PUBLIC	gputcode
PROC	gputcode
	ARG	px:WORD, py:WORD, code:WORD, mode:WORD
	USES	ds, si, di
	
	lds	bx, [gworkp]
	mov	ax, [px]
	mov	[bx+0], ax
	mov	ax, [py]
	mov	[bx+2], ax
	mov	ax, [code]
	mov	[bx+4], ax
	mov	ax, [mode]
	mov	[bx+6], al
	mov	[BYTE PTR bx+7], 0
	push	bp
	int	0ADh
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
	ret
ENDP

PUBLIC	gputch
PROC	gputch
	ARG	code:WORD
	
	mov	cx, 8
	mov	ax, [code]
	mov	ah, [_kanji]
	test	ah, ah
	jz	@@1

	mov	cx, 16
	mov	[_kanji], 0
	shl	ah, 1
	sub	al, 1fh
	js	@@jis
	cmp	al, 061h
	adc	al, 0deh
@@jis:	add	ax, 1fa1h
	and	ax, 07f7fh
	jmp	SHORT @@3

@@1:	cmp	al,81h
	jb	@@3
	cmp	al,9Fh
	jbe	@@2
	cmp	al,0E0h
	jb	@@3
	cmp	al,0FCh
	ja	@@3
@@2:	mov	[_kanji], al
	jmp	SHORT @@q

@@3:	call	getline
	push	cx
	push	[_gpmode]
	push	ax
	mov	ax, [_gpx]
	add	ax, cx
	cmp	ax, 640
	jb	@@5
	mov	[_gpx], 0
	mov	ax, [_gpy]
	add	ax, 16
	cmp	ax, dx
	jb	@@5
	mov	[_gpy], 0
@@5:	push	[_gpy]
	push	[_gpx]
	call	gputcode
	add	sp, 8
	pop	cx
	add	[_gpx], cx

@@q:	mov	ax, [code]
	ret
ENDP

PUBLIC	gputchar
PROC	gputchar
	ARG	code:WORD
	
	mov	ax, [code]
	cmp	al, 7		; BELL
	jne	@@1
;
	jmp	SHORT @@q

@@1:	cmp	al, 8		; BS
	jne	@@2
	mov	ax, [_gpx]
	sub	ax, 8
	jnb	@@1a
	xor	ax, ax
@@1a:	mov	[_gpx], ax
	jmp	SHORT @@q
	
@@2:	cmp	al, 9		; TAB
	jne	@@3
	mov	al, [_gtabsiz]
	mov	cl, 8
	mul	cl
	mov	cx, ax
	mov	ax, [_gpx]
	add	ax, cx
	xor	dx, dx
	div	cx
	mul	cx
	cmp	ax, 640
	jb	@@2b
	call	getline
	mov	ax, [_gpy]
	add	ax, 16
	cmp	ax, dx
	jb	@@2a
	xor	ax, ax
@@2a:	mov	[_gpy], ax
	xor	ax, ax
@@2b:	mov	[_gpx], ax
	jmp	SHORT @@q

@@3:	cmp	al, 10		; LINEFEED
	jne	@@4
	call	getline
	mov	ax, [_gpy]
	add	ax, 16
	cmp	ax, dx
	jb	@@3a
	xor	ax, ax
@@3a:	mov	[_gpy], ax
	mov	[_gpx], 0
	jmp	SHORT @@q

@@4:	cmp	al, ' '
	jb	@@q
	push	ax
	call	gputch
	pop	cx

@@q:	mov	ax, [code]
	ret
ENDP

PUBLIC	gpmode
PROC	gpmode
	ARG	mode:WORD
	
	push	[_gpmode]
	mov	ax, [mode]
	mov	[_gpmode], ax
	pop	ax
	ret
ENDP

PUBLIC	gpxpos
PROC	gpxpos
	mov	ax, [_gpx]
	ret
ENDP

PUBLIC	gpypos
PROC	gpypos
	mov	ax, [_gpy]
	ret
ENDP

PUBLIC	gsetpos
PROC	gsetpos
	ARG	px:WORD, py:WORD

	mov	ax, [px]
	mov	[_gpx], ax
	mov	ax, [py]
	mov	[_gpy], ax	
	ret
ENDP
END
