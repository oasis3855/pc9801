/* XYGRMUSx.CPP
   XYGRPH.EXE �p�@�}�E�X�@�v���O���� */
#include<stdlib.h>
#include<dos.h>
#include<conio.h>
#include<etc.h>
#include<glio.h>
#include<gbios.h>
#include"xygr.h"

/* �}�E�X�{�^�����������̂�҂� */
void until_mouse_wait(void)
{
	static struct REGPACK r;
	do
	{
		r.r_ax = 0x03;		/* x,y ���W, �{�^����ǂ� */
		intr(0x33,&r);

	}while(r.r_bx & 0x01);
}

/* �}�E�X�ō��W������ */
void do_mouse_plot(void)
{
	struct REGPACK r;
	int click=0,old_x=0,old_y=0,old_x2,old_y2;
	int m_x,m_y;	/* ���݂̃}�E�X�̈ʒu */
	double c_x,c_y,c_x2,c_y2;

	r.r_ax = 0x00;		/* �}�E�X�h���C�o�[�m�F */
	intr(0x33,&r);
	if(r.r_ax == 0) return;	/* �}�E�X�h���C�o�[���� */

	r.r_ax = 255;		/* �J�[�\���������ݖʐݒ� */
	r.r_bx = 0x0f;
	if(cur_palette == 2 || cur_palette == 3) r.r_bx = 0x01;
	intr(0x33,&r);

	r.r_ax = 0x0f;		/* �~�b�L�[�ݒ� */
	r.r_cx = 5;
	r.r_dx = 5;
	intr(0x33,&r);

	r.r_ax = 0x07;		/* ���������J�[�\���ړ��͈� */
	r.r_cx = xw_min;
	r.r_dx = xw_max;
	intr(0x33,&r);

	r.r_ax = 0x08;		/* ���������J�[�\���ړ��͈� */
	r.r_cx = yw_min;
	r.r_dx = yw_max;
	intr(0x33,&r);

	r.r_ax = 0x01;		/* �}�E�X�J�[�\�� ON */
	intr(0x33,&r);

	r.r_ax = 0x04;		/* x y ���W�ݒ� */
	r.r_cx = (xw_max+xw_min)/2;
	r.r_dx = (yw_max+yw_min)/2;
	intr(0x33,&r);

	gline(1,399,2,399,_GRAY);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);	/* GBIOS ������ */

	/* ���W�\���̂��߂̃e�L�X�g�E�C���h�E */
	window(xtw_min,ytw_min,xtw_max,ytw_max);
	textattr(_c_info);


	for(;;)
	{
		if(kbhit() && getch() == 0x1b) /* ESC�L�[�`�F�b�N */
		{
			_AX = 0x02;		/* �}�E�X�J�[�\�� OFF */
			geninterrupt(0x33);
			if(click) gbios_rect(old_x,old_y,m_x,m_y);
			break;	/* �}�E�X�I�� */
		}

		r.r_ax = 0x03;		/* x,y ���W, �{�^����ǂ� */
		intr(0x33,&r);
		m_x = r.r_cx;
		m_y = r.r_dx;

		/* �������W�ɕϊ� */
		c_x = ((double)m_x - (double)xw_min)/x_ratio + x_min;
		c_y = ((double)yw_max - (double)m_y)/y_ratio + y_min;
		/* ���W�l��\�� */
		gotoxy(1,14);
		cprintf("X : % -11.5g  ",c_x);
		gotoxy(1,15);
		cprintf("Y : % -11.5g  ",c_y);

		if(click)
		{	/* �����`��`�� */
			if(m_x == old_x2 && m_y == old_y2) ;	/* �J�[�\���������Ȃ����� */
			else
			{
				_AX = 0x02;		/* �}�E�X�J�[�\�� OFF */
				geninterrupt(0x33);
				gbios_rect(old_x,old_y,old_x2,old_y2);	/* �O�̒����`������ */
				old_x2 = m_x;
				old_y2 = m_y;
				gbios_rect(old_x,old_y,m_x,m_y);	/* �V���Ȓ����`��`�� */
				_AX = 0x01;		/* �}�E�X�J�[�\�� ON */
				geninterrupt(0x33);
			}
		}
		if(r.r_bx & 0x01)	/* �{�^���������ꂽ�� */
		{
			if(click == 0)
			{	/* �P��ڂ̃{�^�� */
				old_x = m_x;
				old_y = m_y;
				old_x2 = m_x;
				old_y2 = m_y;
				click = 1;
				until_mouse_wait();
			}
			else
			{	/* �Q��ڂ̃{�^�� */
				r.r_ax = 0x02;		/* �}�E�X�J�[�\�� OFF */
				intr(0x33,&r);

				gbios_rect(old_x,old_y,m_x,m_y);	/* �O�̒����`������ */

				/* �������W�ɕϊ� */
				c_x = ((double)m_x - (double)xw_min)/x_ratio + x_min;
				c_y = ((double)yw_max - (double)m_y)/y_ratio + y_min;
				c_x2 = ((double)old_x - (double)xw_min)/x_ratio + x_min;
				c_y2 = ((double)yw_max - (double)old_y)/y_ratio + y_min;

				/* �V�������W�̐ݒ� */
				x_min = min(c_x,c_x2);
				x_max = max(c_x,c_x2);
				y_min = min(c_y,c_y2);
				y_max = max(c_y,c_y2);

				click = 0;
				until_mouse_wait();
				break;	/* �}�E�X�I�� */
			}
		}
	}
	gbios_setmode(0,GBIOS_COPY,GBIOS_GREEN,0xffff);
	set_param();
	prn_data_win();
	return;
}