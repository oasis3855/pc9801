C	TLIB.LIB �T���v��

$INCLUDE:'TLIB.FI'

$INCLUDE:'TLIB.FD'

	integer i,j
	integer*2 k

	call tcls()
	call tcolor($DEFAULT)
	call HorizLine(1)
	call tlocate(28,1)
	call tputs('TLIB.LIB �T���v���v���O����'C)

	call tlocate(1,10)
	call tputs('���̃v���O������ �e�L�X�g���C�u�����[ TLIB.LIB'C)
	call tputs('�̓���������������'C)
	call tlocate(1,12)
	call tputs('�f�����X�g���[�V�����v���O�����ł��B'C)
	call HitKey()
	
	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(34,1)
	call tputs('�F�̃T���v��'C)
	do 20 i=1,7
	 call tcolor($TBLACK+i)
	 call tlocate(5,i+1)
	 call tputs('�J���[�̎w�肪�ł��܂�'C)
	 call tcolor($TRBLACK+i)
	 call tlocate(53,i+1)
	 call tputs('�J���[�̎w�肪�ł��܂�'C)
 20	continue
 	call tcolorm($TGREEN, $UNDERLINE, $DEFAULT)
 	call tlocate(5,15)
 	call tputs('�A���_�[���C���������܂�'C)
 	call tcolorm($TYELLOW, $BLINK, $DEFAULT)
 	call tlocate(5,17)
 	call tputs('�_�ł��ł��܂�'C)
 	call tcolorm($TCYAN, $VERT, $DEFAULT)
 	call tlocate(5,19)
 	call tputs('����Ȃ̂��ł��܂�'C)
	call tcolor($TRRED)
	call tlocate(5,21)
	write(*,*) '�����ӁFWRITE ���ł̓J�����̎w��͏o���܂���'

	call HitKey()

	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(32,1)
	call tputs('�X�N���[�����[�h'C)

	call tlocate(1,10)
	write(*,*) '�����L�[��������'
	call tlocate(1,12)
	write(*,*) '  �J�[�\���������A�Q�O�s���[�h�ɂȂ�A'
	call tlocate(1,14)
	write(*,*) '  �t�@���N�V�����L�[�������܂��B'

	call HitKey()

	call tfunc(0)
	call tline25(0)
	call tcursor(0)

	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(32,1)
	call tputs('�X�N���[�����[�h'C)

	call tcolor($TCYAN)
	do 30 i=2,20
	 call tlocate(i*2+20,i)
	 call tputnum(i)
	 call tputs(' �s��'C)
 30	continue
 	call tcolor($DEFAULT)
	call tlocate(1,10)
	write(*,*) '�����L�[�������� ���ɖ߂�܂�'

	k = tgetch()

	call tfunc(1)
	call tline25(1)
	call tcursor(1)

	call tcls()
	call HorizLine(1)
	call tcolor($DEFAULT)
	call tlocate(34,1)
	call tputs('���悤�Ȃ�'C)

	call tlocate(1,10)
	call tcolor($TGREEN)
	write(*,*) '�ȏ�Ńf�����X�g���[�V�������I���܂�'

	call HitKey()

 	call tcolor($DEFAULT)
 	call tcls()
	
	end

C	y �s�ڂɐ�����������
	subroutine HorizLine(y)

$INCLUDE:'TLIB.FD'

	integer y
	integer*2 y1

	y1 = y
	call tlocate(1,y)
	do 10 i=1,40
 10	 call tputs('��'C)
	return
	end

C	�L�[���͂𑣂�
	subroutine HitKey()

$INCLUDE:'TLIB.FD'

	integer*2 dummy

C	�r�[�v��
	call tputchar(7)
	call tcolorm($TGREEN, $UNDERLINE, $DEFAULT)
	call tlocate(57,23)
	call tputs('�����L�[�������ĉ�����'C)
	dummy = tgetch()
	call tcolor($DEFAULT)
	return
	end
