C	*************************************************************
C	GLIODEMO.FOR : GLIO.LIB デモンストレーション no.1
C	original C version (C) M. Ohno
C	(C) 1993 Hirokazu Inoue
C	*************************************************************

$INCLUDE:'GLIO.FI'
$INCLUDE:'TLIB.FI'

	PROGRAM MAIN

$INCLUDE:'GLIO.FD'
$INCLUDE:'TLIB.FD'

	integer i

	call tfunc(0)
	call tcls()
	call ginit()
	call gscreen(3,0,0,1)
	call gcls()

	do 10 i=0, 638
		call gline(i, 0, 639-i, 399, i-i/16*16+1)
 10	continue
 	do 20 i=0, 400
 		call gline(639, i, 0, 399-i, i-i/16*16+1)
 20	continue
	do 30 i=0, 638
		call gline(i, 0, 639-i, 399, 0)
 30	continue
 	do 40 i=0, 400
 		call gline(639, i, 0, 399-i, 0)
 40	continue

	do 50 i=0,200
		call gbox(i, i, 639-i, 399-i, i-i/16*16+1)
 50	continue
	do 60 i=0,200
		call gbox(i, i, 639-i, 399-i, 0)
 60	continue
	do 70 i=0,200
		call gbar(i, i, 639-i, 399-i, i-i/16*16+1)
 70	continue

	call gcls()

	do 80 i=0,160,4
		call gellipse(320, 200, i*2, i, (i/4)-(i/4)/16*16+1)
 80	continue

	do 90 i=0,160,4
		call gellipse(320, 200, i*2, i,(i/4)-(i/4)/16*16)
 90	continue

	call gcls()
	
	call gcolor(6, 0, 0)
	call gsetpos(285, 330)
	call gputs('GLIODEMO'C)
	call gcolor(5, 0, 0)
	call gsetpos(285, 380)
	call gputs('The end.'C)

	do 100 i=0,200,4
		call groll(0,4)
 100	continue

	call tfunc(1)
	call HitKey()

 	call gcls()
 	call gscreen(-1, 2, -1, -1)
	call tcls()

	end


C	キー入力を促す
	subroutine HitKey()

$INCLUDE:'TLIB.FD'

	integer*2 dummy

C	ビープ音
	call tputchar(7)
	call tcolorm($TGREEN, $UNDERLINE, $DEFAULT)
	call tlocate(57,23)
	call tputs('何かキーを押して下さい'C)
	dummy = tgetch()
	call tcolor($DEFAULT)
	return
	end
