C	TLIB.LIB サンプル

$INCLUDE:'TLIB.FI'

$INCLUDE:'TLIB.FD'

	integer i,j
	integer*2 k

	call tcls()
	call tcolor($DEFAULT)
	call HorizLine(1)
	call tlocate(28,1)
	call tputs('TLIB.LIB サンプルプログラム'C)

	call tlocate(1,10)
	call tputs('このプログラムは テキストライブラリー TLIB.LIB'C)
	call tputs('の働きをお見せする'C)
	call tlocate(1,12)
	call tputs('デモンストレーションプログラムです。'C)
	call HitKey()
	
	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(34,1)
	call tputs('色のサンプル'C)
	do 20 i=1,7
	 call tcolor($TBLACK+i)
	 call tlocate(5,i+1)
	 call tputs('カラーの指定ができます'C)
	 call tcolor($TRBLACK+i)
	 call tlocate(53,i+1)
	 call tputs('カラーの指定ができます'C)
 20	continue
 	call tcolorm($TGREEN, $UNDERLINE, $DEFAULT)
 	call tlocate(5,15)
 	call tputs('アンダーラインも引けます'C)
 	call tcolorm($TYELLOW, $BLINK, $DEFAULT)
 	call tlocate(5,17)
 	call tputs('点滅もできます'C)
 	call tcolorm($TCYAN, $VERT, $DEFAULT)
 	call tlocate(5,19)
 	call tputs('こんなのもできます'C)
	call tcolor($TRRED)
	call tlocate(5,21)
	write(*,*) '※注意：WRITE 文ではカラムの指定は出来ません'

	call HitKey()

	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(32,1)
	call tputs('スクリーンモード'C)

	call tlocate(1,10)
	write(*,*) '何かキーを押すと'
	call tlocate(1,12)
	write(*,*) '  カーソルが消え、２０行モードになり、'
	call tlocate(1,14)
	write(*,*) '  ファンクションキーも消えます。'

	call HitKey()

	call tfunc(0)
	call tline25(0)
	call tcursor(0)

	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(32,1)
	call tputs('スクリーンモード'C)

	call tcolor($TCYAN)
	do 30 i=2,20
	 call tlocate(i*2+20,i)
	 call tputnum(i)
	 call tputs(' 行目'C)
 30	continue
 	call tcolor($DEFAULT)
	call tlocate(1,10)
	write(*,*) '何かキーを押すと 元に戻ります'

	k = tgetch()

	call tfunc(1)
	call tline25(1)
	call tcursor(1)

	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(34,1)
	call tputs('さようなら'C)

	call tlocate(1,10)
	call tcolor($TGREEN)
	write(*,*) '以上でデモンストレーションを終わります'

	call HitKey()

 	call tcolor($DEFAULT)
 	call tcls()
	
	end

C	y 行目に水平線を引く
	subroutine HorizLine(y)

$INCLUDE:'TLIB.FD'

	integer y
	integer*2 y1

	y1 = y
	call tlocate(1,y)
	do 10 i=1,40
 10	 call tputs('�｣'C)
	return
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

