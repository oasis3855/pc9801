C	*************************************************************
C	SAMPLE.FOR : GLIO.LIB デモンストレーション no.2
C	original C version (C) M. Ohno
C	(C) 1993 Hirokazu Inoue
C	*************************************************************

$INCLUDE:'GLIO.FI'
$INCLUDE:'TLIB.FI'

	PROGRAM MAIN

$INCLUDE:'GLIO.FD'
$INCLUDE:'TLIB.FD'

	integer i
	integer*1 buf(5260)

	call tfunc(0)
	call tcls()
	call ginit()
	call gscreen(3,0,0,1)
	call gcls()

	call gcolor(6, -1, 2)
	call gline(0, 0, 639, 399, 4)
	call gbox(100, 100, 399, 299, 5)
	call glines(0, 399, 639, 0, 6, 13311)
	call gboxs(200, 200, 499, 349, 3, 13107)
	call gbar(200, 0, 299, 99, 1)
	call gbarf(300, 100, 399, 199, 6, 2)

	call gbarf(400, 200, 499, 299, 3, 11)
	call gellipse(200, 300, 200, 50, 3)
	call gcirclef(350, 150, 20, 5, 1)
	call gcircle(100, 300, 50, 6)

	call gget(350, 50, 450, 150, buf)
	call gput(300, 200, $GPUT_COPY, buf)

	call gpief(100, 300, 100, 70, 6, 14, 120, 150)
	call gsetpos(150, 100)
	call gpmode(GPUT_COPY)

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
