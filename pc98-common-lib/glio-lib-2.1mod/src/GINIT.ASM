;
;	ginit (in GLIO.LIB)
;
;	Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
;
	IDEAL
	MODEL LARGE, C

	DATASEG
	EVEN
	PUBLIC	gworkp, gerrno, _sintbl, _gpx, _gpy, _gpmode, _gtabsiz, _gcmode
gworkp	DD	?
gerrno	DW	?
	DB	10h dup (?)
_gwork	DB	128h dup (?)
	DB	'SIN TBL',0
_sintbl	DW 0000H,0047H,008EH,00D6H,011DH,0164H,01ACH,01F3H,023AH,0280H
	DW 02C7H,030DH,0353H,0399H,03DEH,0424H,0469H,04ADH,04F1H,0535H
	DW 0578H,05BBH,05FEH,0640H,0681H,06C3H,0703H,0743H,0782H,07C1H
	DW 07FFH,083DH,087AH,08B6H,08F2H,092DH,0967H,09A1H,09D9H,0A11H
	DW 0A48H,0A7FH,0AB4H,0AE9H,0B1DH,0B50H,0B82H,0BB3H,0BE3H,0C13H
	DW 0C41H,0C6FH,0C9BH,0CC7H,0CF1H,0D1BH,0D43H,0D6BH,0D91H,0DB6H
	DW 0DDBH,0DFEH,0E20H,0E41H,0E61H,0E80H,0E9DH,0EBAH,0ED5H,0EEFH
	DW 0F08H,0F20H,0F37H,0F4DH,0F61H,0F74H,0F86H,0F97H,0FA6H,0FB4H
	DW 0FC1H,0FCDH,0FD8H,0FE1H,0FE9H,0FF0H,0FF6H,0FFAH,0FFDH,0FFFH
	DW 1000H
_gpx	DW	0
_gpy	DW	0
_gpmode	DW	2	; GPUT_OR
_gtabsiz DB	8
_gcmode	DW	1	; Color8/16/Monochrome Flag

	DB	(1200h - 128h - 91*2 - 5) dup (?)

__CopyR	DB	'==GLIO/PC-9801 H.I 1992==',0

;
;	OFFSET		
;	0000h~0007h:	(GLIO.gcolor)
;	0008h~0026h:	parameter
;	0027h:		tile pattern length
;	0028h~0127h:	tile pattern
;	0128h~     :    sin/cos table
;
;	0620h~069Fh:	(GLIO)
;		0620h:		screen mode
;		0621h:		active page
;		0622h:		visual page
;		0623h:		fore-ground color
;		0624h:		back-ground color
;		0626h~062dh:	color code
;		062eh~062fh:	viewport (left)
;		0630h~0631h:	viewport (top)
;		0632h~0633h:	viewport (right)
;		0634h~0635h:	viewport (bottom)
;		0636h:		active bank
;		0637h:		visual bank
;
;	06A0h~0FFFh:	PAINT work
;	1000h~11FFh:	(GLIO)
;
;	1380h~13FFh:	(GLIO.gcopy)
;

	CODESEG
PROC	stop_handler
	iret
ENDP

PUBLIC	ginit
PROC	ginit
	USES	si, di, ds

	cld
	xor	ax, ax
	mov	es, ax
	mov	ax, 0F990h
	mov	ds, ax
	mov	si, 4 + 2
	mov	di, 0A0h*4
	mov	cx, 10h
@@1:	movsw
	stosw
	inc	si
	inc	si
	loop	@@1
	mov	di, 0C5h*4
	mov	ax, OFFSET stop_handler
	stosw
	mov	ax, SEG stop_handler
	stosw

	mov	ax, OFFSET _gwork
	mov	cl, 4
	shr	ax, cl
	add	ax, @data
	mov	dx, @data
	mov	ds, dx
	mov	[WORD PTR gworkp], 8
	mov	[WORD PTR gworkp+2], ax
	mov	ds, ax
	push	bp
	int	0A0h
	pop	bp
	mov	al, ah
	xor	ah, ah
	mov	[gerrno], ax
;
	mov	ax, 0
	mov	[_gcmode],ax	; Color flag
	ret
ENDP
END
