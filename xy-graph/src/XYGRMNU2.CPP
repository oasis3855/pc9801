/* XYGRMNUx.CPP
   XYGRPH.EXE �p�@���j���[�^���C���@�v���O���� */
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<process.h>
#include<signal.h>
#include<glio.h>
#include<gbios.h>
#include<winp.h>
#include<hdcpy.h>
#include<etc.h>
#include<gr.h>
#include"xygr.h"

char *m_str1[] = {" XYGRPH�ɂ���   ",
				  " �g�����̐���     ",
				  " �摜̧�ك��[�h   ",
				  " �摜̧�كZ�[�u   ",
				  " ��̧�ك��[�h   ",
				  " ��̧�كZ�[�u   ",
				  " ���̨�� ��ި��   ",
				  " հ�ް�����       ",
				  " ����� ���        ",
				  " �N���[�Y(�I��)   "};
char *m_str2[] = {" �W�����ރv�����g ",
				  " �k�����ރv�����g ",
				  " �v���r���[       "};
char *m_str3[] = {" ������(x)        ",
				  " ��,������(t)     ",
				  " ����d��(x)/dx    ",
				  " ���W��������     ",
				  " �E�C���h�E����   "};
char *m_str4[] = {" ��ʍ��W�ݒ�     ",
				  " ϳ��Ɉ˂�͈͐ݒ�",
				  " �{�[�_�[���W�ݒ� ",
				  " ���W���Ԋu�̐ݒ� ",
				  " ���s������       ",
				  " �ϐ����͈̔͐ݒ� ",
				  " �{�[�_�[�̗L���� ",
				  " ���s�������L���� ",
				  " ���W�\���̎��   ",
				  " ���_���W�̎��   ",
				  " �⊮���[�h�ݒ�   ",
				  " ���������[�h�ݒ� ",
				  " �O���t�\���F�ݒ� ",
				  " ���W�\���F�ݒ�   ",
				  " ���_���W�̐F�ݒ� "};

char *m_str_inp_xy[] = {"�� �ŏ��l",
				  "�� �ő�l",
				  "�� �ŏ��l",
				  "�� �ő�l"};
char *m_str_inp_xy_bod[] = {"���ް�ް �ŏ��l",
				  "���ް�ް �ő�l",
				  "���ް�ް �ŏ��l",
				  "���ް�ް �ő�l"};
char *m_str_inp_scale[] = {"�����W�\���Ԋu",
				  "�����W�\���Ԋu",
				  "���_�����W�\���Ԋu",
				  "���_�����W�\���Ԋu"};
char *m_str_inp_t[] = {"�� �J�n�l",
				  "�� �I���l",
				  "�� ������"};
char *m_str_fld[] = {"�ǂݍ��މ摜�t�@�C����"};
char *m_str_fsv[] = {"�����o���摜�t�@�C����"};
char *m_str_eld[] = {"�ǂݍ��ފ��t�@�C����"};
char *m_str_esv[] = {"�����o�����t�@�C����"};
char *m_str_inp_c_grph[] = {"�O���t�̐F (0 - 15)"};
char *m_str_inp_c_scale[] = {"���W�̐F (0 - 15)"};
char *m_str_inp_c_0[] = {"���_���W�̐F (0 - 15)"};
char *m_str_inp_fx[] = {"�֐� ������(��)"};
char *m_str_inp_fxyt[] = {"�֐� ������(��)",
						"�֐� ������(��)"};
char *m_str_inp_dfx[] = {"�֐� ����d��(��)�^d��"};
char *m_str_inp_if[] = {"��(��):���̎����O�ȏ�̎��O���t��`���܂�"};

char *m_str_sw_scale[] = {"   ���W����         ",
				  "   �S�ʂɍ��W�\��   ",
				  "   ����ɍ��W�\��   "};
char *m_str_sw_0[] = {"   ���_���W����     ",
				  "   �����ŕ\��       ",
				  "   �ڐ���\��       ",
				  "   �ڐ���,�����\��  "};
char *m_str_sw_line[] = {"   �h�b�g�ŕ\��     ",
				  "   �����⊮����     "};
char *m_str_sw_init[] = {"   �N���A�E���W     ",
				  "   ���W             ",
				  "   �N���A           ",
				  "   �㏑��           "};
char *m_str_sw_if[] = {"   ���s������ ����  ",
				  "   ���s������ �L��  "};
char *m_str_sw_bod[] = {"   �{�[�_�[ ����    ",
				  "   �{�[�_�[ �L��    "};


/* ���j���[�o�[�@�^�C�g�� */
char *mbar_str[] = {"�t�@�C��",
				"�v�����g",
				"�W���u",
				"�I�v�V����"};
/* ���j���[�o�[���W */
int mbar_x[] = {4,14,25,34};

/* �_�~�[�o�b�t�@ */
char _str_dmy[] = {"ʲڿ�ح����Ӱ�� �ł͓����܂���.\n"};
/* �L�[���͗p�o�b�t�@�@�V���[�g */
char *buf_s[] = {"(C)opyright 1992 ��� ���v     ",
				 "Nifty Serve GDH03306           ",
				 "PC VAN KYJ15060                ",
				 "Ұق͈ȏ�̏��܂łǂ����I�I    "};
/* �L�[���͗p�o�b�t�@�@�����O */
char *buf_l[]={"���̃v���O�����͂������W�n�̂Q�����O���t��`���܂��B          ",
			   "GRPH �L�[���� F10 �L�[�Ń��j���[�Ɉڂ�܂��B                  "};


void main(int ac, char *av[])
{
	puts("XYGRPH.EXE						(C) 1992 Hirokazu Inoue\n");
	if(ac >= 2 && !strcmp(av[1],"-?"))
	{	/* �w���v */
		puts("usage : XYGRPH (-?) <ENV file>\n"
			 "To activate menu , press 'GRPH' or 'F�10'\n"
			 "This program is for PC-9801 MS-DOS normal mode\n");
		exit(0);
	}

	/* ��ʂ̏����� */
	init_graphic();
	gcls();
	printf("\x1b[2J\x1b[>1h\x1b[>5h");	/* ���� OFF �� */

	/* �}�V���̃`�F�b�N */
	if(PC98_chk_hi_resolution())
	{
		printf("%s\n",_str_dmy);
		exit(0);
	}

	signal(SIGINT,SIG_IGN);
	signal(SIGFPE,SIG_IGN);
	signal(SIGABRT,SIG_IGN);
	kb_beep_off();

	/* �J���[�����l */
	_c_mnu  = WHITE|REVERSE;	/* ���j���[ */
	_c_mnuv = WHITE;			/* ���j���[���] */
	_c_box  = WHITE|REVERSE;	/* ���j���[�g */
	_c_barb = WHITE|REVERSE;	/* �o�[ */
	_c_bar  = WHITE|REVERSE;	/* �o�[���j���[ */
	_c_barv = WHITE;			/* �o�[���j���[���] */
	_c_info = WHITE;			/* ���W�l���\�� */
	_cg_info = _WHITE;			/* �C���t�H���[�V�����G���A�{�[�_�[ */
	_c_gr_grph = _LIGHTGREEN;	/* �O���t�̐F */
	_c_gr_scale = _LIGHTRED;	/* ���W�̐F */
	_c_gr_0 = _WHITE;			/* ���_���W�̐F */

	strcpy(commandcom,getenv("COMSPEC"));
	if(!strcmp(commandcom,NULL)) strcpy(commandcom,"COMMAND.COM");

	/* ���̓ǂݍ��� */
	init_GMNU();
	strcpy(filename,"XYGRPH");	/* �ŏ��̊��t�@�C���� */
	if(ac > 1) strcpy(filename,av[1]);	/* ���t�@�C�������w�肳�ꂽ�Ƃ� */
	strcpy(exe_gedit_file,"GEDIT.EXE");
	strcpy(exe_plot_file,"MPS.EXE");
	strcpy(exe_gsv_file,"GRGBSV.EXE");
	strcpy(exe_gld_file,"GRGBSV.EXE -l");
	do_load_env();	/* ���t�@�C����ǂݍ��� */
	chg_palette();


	/* �S�Ẵp�����[�^�̏����� */
	init_GMNU();

	/* �^�C�g�� */
	mnu_barmake(&w_bar,mbar_str,mbar_x,"�w�x �fRAPH",69);
	chg_palette();
	gsetpattern(32,_bpat);
	gbarp(0,16,639,399,_BLACK);
	delay(1500);
	do_info_win();

	/* XYGraph �v�Z�\���E�C���h�E�\�� */
	write_window();
	write_scale();


	/* ���j���[�J�n */
	mnu_menubar(&w_bar,mbar_str,mbar_x,"�w�x �fRAPH",69);

}

int bar_mnu_func(int i)
{
	int chk;
	switch(i)
	{
		case	1:	chk = do_menu_1();break;
		case	2:	chk = do_menu_2();break;
		case	3:	chk = do_menu_3();break;
		case	4:	chk = do_menu_4();break;
		default:	chk = 0x1b;break;
	}
	return chk;
}


int do_menu_1(void)
{
	int ret,check;


	w1.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w1,0,m_str1,&ret);		// open menu
	for(;;)
	{
		w1.mode = W_MENU;
		mnu_GLIO_gmenu(&w1,0,m_str1,&ret);		// do menu
		if(ret != 0x0d) break;	/* RET �ȊO���������ƭ������� */
		switch(w1.cur_topic)
		{
			case 1	:			/* ���̃v���O�����ɂ��� */
				do_info_win();
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	2	:		/* ������ */
				do_help();
				restore_screen();
				chg_palette();
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case 	3	:		/* �摜�ǂݍ��� */
				if(do_input_char_1l(filename,m_str_fld))
				{	/* �摜���[�h */
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					do_load_file();
					init_graphic();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{	/* �L�����Z�� */
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	4	:		/* �摜�����o�� */
				if(do_input_char_1l(filename,m_str_fsv))
				{	/* �摜�Z�[�u */
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					do_save_file();
					init_graphic();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{	/* �L�����Z�� */
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case 	5	:		/* ���ǂݍ��� */
				if(do_input_char_1l(filename,m_str_eld))
				{	/* �����[�h */
					w1.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
					do_load_env();
					init_GMNU();
					init_graphic();
					write_window();		/* �O���t�B�b�N���������� */
					write_scale();
					restore_screen();
					w1.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				else
				{	/* �L�����Z�� */
					w1.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				}
				break;
			case	6	:		/* �������o�� */
				if(do_input_char_1l(filename,m_str_esv))
				{	/* ���Z�[�u */
					do_save_env();
				}
				w1.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	7	:		/* ���̨�� ��ި�� */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				exe_gedit();
				init_graphic();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	8	:		/* ��ۯ�� */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				exe_plot();
				init_graphic();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	9	:		/* COMMAND.COM */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				gscreen(-1,3,-1,-1);
				printf("\x1b[2J\x1b[>1l\x1b[>5l\x1b[m");
				printf("EXIT ����͂���� XYGRPH �ɖ߂�܂�\n");
				spawnl(P_WAIT,commandcom,commandcom,NULL);
				printf("\x1b[2J\x1b[24;43H\x1b[47m�����L�[�������� XYGRPH �ɖ߂�܂�");
				getch();
				init_graphic();
				restore_screen();
				w1.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				break;
			case	10	:		/* �N���[�Y */
				w1.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w1,0,m_str1,&ret);
				gcls();
				gscreen(3,3,-1,-1);
				printf("\x1b[2J\x1b[>1l\x1b[>5l\x1b[m");
				sound(2000);
				delay(20);
				nosound();
				kb_beep_on();
				exit(0);
				break;
			default	:	break;
		}
	}
	check = ret;

	w1.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w1,0,m_str1,&ret);

	return check;


}

int do_menu_2(void)
{
	int ret,check;

	w2.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w2,0,m_str2,&ret);	// open menu
	for(;;)
	{
		w2.mode = W_MENU;
		mnu_GLIO_gmenu(&w2,0,m_str2,&ret);	// do menu
		if(ret != 0x0d) break;	/* RET �ȊO���������ƭ������� */
		switch(w2.cur_topic)
		{
			case 1	:		// �W���T�C�Y�v�����g
				w2.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				_hc_sX = 1;_hc_sY = 1;
				hc_hardcopy();
				w2.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				break;
			case 2	:		// �k���T�C�Y�v�����g
				w2.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				_hc_sX = 0;_hc_sY = 0;
				hc_hardcopy();
				w2.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w2,0,m_str2,&ret);
				break;
			case 3	:		// �v���r���[
				int dummy;
				dummy = cur_palette;
				cur_palette = 4;
				chg_palette();
				getch();
				cur_palette = dummy;
				chg_palette();
				break;
			default	:	break;

		}
	}
	check = ret;
	w2.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w2,0,m_str2,&ret);

	return check;
}


int do_menu_3(void)
{
	int ret,check;

	w3.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w3,0,m_str3,&ret);	// open menu
	for(;;)
	{
		w3.mode = W_MENU;
		mnu_GLIO_gmenu(&w3,0,m_str3,&ret);	// do menu
		if(ret != 0x0d) break;	/* RET �ȊO���������ƭ������� */
		switch(w3.cur_topic)
		{
			case	1	:
				if(do_inp_fx())
				{
					w3.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
					write_graph(1);
					w3.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				else
				{
					w3.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				break;
			case	2	:
				if(do_inp_fxyt())
				{
					w3.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
					write_graph(2);
					w3.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				else
				{
					w3.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				break;
			case	3	:
				if(do_inp_dfx())
				{
					w3.mode = W_CLOSE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
					write_graph(3);
					w3.mode = W_FWRITE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				else
				{
					w3.mode = W_RESTORE;
					mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				}
				break;
			case	4	:	/* ���W�`�� */
				w3.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				write_scale();
				w3.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			case	5	:	/* ��ʏ��� */
				w3.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				write_window();
				w3.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w3,0,m_str3,&ret);
				break;
			default	:	break;
		}
	}
	check = ret;
	w3.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w3,0,m_str3,&ret);

	return check;
}

int do_menu_4(void)
{
	int ret,check;

	w4.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w4,0,m_str4,&ret);	// open menu
	for(;;)
	{
		w4.mode = W_MENU;
		mnu_GLIO_gmenu(&w4,0,m_str4,&ret);	// do menu
		if(ret != 0x0d) break;	/* RET �ȊO���������ƭ������� */
		switch(w4.cur_topic)
		{
			case	1	:	/* XY ���W���� */
				do_inp_xy();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case 	2	:	/* �}�E�X */
				w4.mode = W_CLOSE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				do_mouse_plot();
				w4.mode = W_FWRITE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case 	3	:	/* XY �ް�ް���W���� */
				do_inp_xy_bod();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	4	:	/* ���W���̊Ԋu�ݒ� */
				do_inp_scale();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	5	:	/* if�� */
				do_inp_if();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	6	:	/* ���͈̔͂�ǂݍ��� */
				do_inp_t();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	7	:
				do_sw_bod();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	8	:
				do_sw_if();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	9	:	/* ���W�̎�� */
				do_sw_scale();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	10	:	/* ���_���W�̎�� */
				do_sw_0();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	11	:	/* �����⊮�̃X�C�b�` */
				do_sw_line();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	12	:	/* �`�揉�����Ɋւ���K��l */
				do_sw_init();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	13	:	/* �O���t�̐F�̓ǂݍ��� */
				do_inp_c_grph();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	14	:	/* ���W�̐F�̓ǂݍ��� */
				do_inp_c_scale();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			case	15	:	/* ���_���W�̐F�̓ǂݍ��� */
				do_inp_c_0();
				w4.mode = W_RESTORE;
				mnu_GLIO_gmenu(&w4,0,m_str4,&ret);
				break;
			default	:	break;
		}
	}
	check = ret;
	w4.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w4,0,m_str4,&ret);

	return check;
}

void do_info_win(void)
{
	int ret;

	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);

	textattr(w_info.color);
	gotoxy(10,2);
	cputs("�w�x�@�fRAPH  ���ع����");
	gotoxy(12,3);
	cputs("XYGRPH.EXE / PC-9801");
	gotoxy(15,4);
	cputs("Version 2.00");
	gotoxy(12,5);
	cputs("(C) 1992 ��� ���v");

	gotoxy(11,8);
	cputs("�������������ĉ�����");

	getch();
	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);
}


void restore_screen(void)
{	/* �e�L�X�g�̂� */
	printf("\x1b[2J\x1b[>1h\x1b[m\x1b[>5h");
	mnu_barmake(&w_bar,mbar_str,mbar_x,"�w�x �fRAPH",69);
	prn_data_win();
}

/* �O���t�B�b�N�������������� */
void init_graphic(void)
{
	ginit();
	gscreen(3,1,0,1);
	gbios_setmode(0,GBIOS_XOR,GBIOS_GREEN,0xf0f0);
	gbios_rect(10,200,30,300);
	gbios_rect(10,200,30,300);
	gbios_setmode(0,GBIOS_COPY,GBIOS_GREEN,0xffff);
	gcolor(_LIGHTGREEN,_BLACK,2);
	gview(0,0,639,399,-1,-1);
	gsetpattern(32,_bpat);

	chg_palette();
}


/* �������P�ǂݍ��ޔėp�֐� */
int do_input_int_1s(int *a, char *m_str[])
{
	int check,retcode;

	strncpy(buf_s[0],itoa(*a,_str_dmy,10),wi_4s.data_len - 2);

	wi_1s.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_1s,buf_s,m_str,&retcode);
	if(check != 0) return 0;

	wi_1s.mode = W_MENU;
	inp_GLIO_gwin(&wi_1s,buf_s,m_str,&retcode);
	if(retcode == 0)
	{
		*a = atoi(buf_s[0]);
	}
	wi_1s.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_1s,buf_s,m_str,&retcode);

	if(!retcode) return 1;	/* ���͂��ꂽ */
	return 0;				/* �L�����Z�����ꂽ */
}

/* �������R�ǂݍ��ޔėp�֐� */
int do_input_double_3s(double *a, double *b, double *c,
					   char *m_str[])
{
	int check,retcode;

	strncpy(buf_s[0],gcvt(*a,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[1],gcvt(*b,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[2],gcvt(*c,15,_str_dmy),wi_4s.data_len - 2);

	wi_3s.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_3s,buf_s,m_str,&retcode);
	if(check != 0) return 0;

	wi_3s.mode = W_MENU;
	inp_GLIO_gwin(&wi_3s,buf_s,m_str,&retcode);
	if(retcode == 0)
	{
		*a = calc_str(buf_s[0]);
		*b = calc_str(buf_s[1]);
		*c = calc_str(buf_s[2]);
	}
	wi_3s.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_3s,buf_s,m_str,&retcode);

	if(!retcode) return 1;	/* ���͂��ꂽ */
	return 0;				/* �L�����Z�����ꂽ */
}

/* �������S�ǂݍ��ޔėp�֐� */
int do_input_double_4s(double *a, double *b, double *c, double *d,
					   char *m_str[])
{
	int check,retcode;

	strncpy(buf_s[0],gcvt(*a,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[1],gcvt(*b,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[2],gcvt(*c,15,_str_dmy),wi_4s.data_len - 2);
	strncpy(buf_s[3],gcvt(*d,15,_str_dmy),wi_4s.data_len - 2);

	wi_4s.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_4s,buf_s,m_str,&retcode);
	if(check != 0) return 0;

	wi_4s.mode = W_MENU;
	inp_GLIO_gwin(&wi_4s,buf_s,m_str,&retcode);
	if(retcode == 0)
	{
		*a = calc_str(buf_s[0]);
		*b = calc_str(buf_s[1]);
		*c = calc_str(buf_s[2]);
		*d = calc_str(buf_s[3]);
	}
	wi_4s.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_4s,buf_s,m_str,&retcode);

	if(!retcode) return 1;	/* ���͂��ꂽ */
	return 0;				/* �L�����Z�����ꂽ */
}

/* ��ʂ̍��W��ǂݍ��� */
void do_inp_xy(void)
{
	do_input_double_4s(&x_min, &x_max, &y_min, &y_max, m_str_inp_xy);

	if(x_min >= x_max) x_max = x_min + 0.001;
	if(y_min >= y_max) y_max = y_min + 0.001;

}

/* �{�[�_�[�̍��W��ǂݍ��� */
void do_inp_xy_bod(void)
{
	do_input_double_4s(&xl_min, &xl_max, &yl_min, &yl_max, m_str_inp_xy_bod);

	if(xl_min >= xl_max) xl_max = xl_min + 0.001;
	if(yl_min >= yl_max) yl_max = yl_min + 0.001;
}

/* ���W���̊Ԋu��ǂݍ��� */
void do_inp_scale(void)
{
	do_input_double_4s(&x_scale, &y_scale, &x_scale_0, &y_scale_0, m_str_inp_scale);

	if(x_scale <= 0.0) x_scale = 1.0;
	if(y_scale <= 0.0) y_scale = 1.0;
	if(x_scale_0 <= 0.0) x_scale_0 = 1.0;
	if(y_scale_0 <= 0.0) y_scale_0 = 1.0;
}

/* ���͈̔͂�ǂݍ��� */
void do_inp_t(void)
{
	do_input_double_3s(&tl_min, &tl_max, &t_delta, m_str_inp_t);

	if(t_delta <= 0.0) t_delta = 0.001;
	if(tl_max <= tl_min) tl_max = tl_min + 0.001;
}

/* �O���t�̐F��ǂݍ��� */
void do_inp_c_grph(void)
{
	do_input_int_1s(&_c_gr_grph, m_str_inp_c_grph);

	if(_c_gr_grph < 0 || _c_gr_grph > 15) _c_gr_grph = _LIGHTGREEN;
}

/* ���W�̐F��ǂݍ��� */
void do_inp_c_scale(void)
{
	do_input_int_1s(&_c_gr_scale, m_str_inp_c_scale);

	if(_c_gr_scale < 0 || _c_gr_scale > 15) _c_gr_scale = _LIGHTRED;
}

/* ���_���W�̐F��ǂݍ��� */
void do_inp_c_0(void)
{
	do_input_int_1s(&_c_gr_0, m_str_inp_c_0);

	if(_c_gr_0 < 0 || _c_gr_0 > 15) _c_gr_0 = _LIGHTRED;
}

/*  ��������P�ǂݍ��ޔėp�֐� */
int do_input_char_1l(char *buf, char *m_str[])
{
	int check,retcode;

	strncpy(buf_l[0],buf,60);

	wi_1l.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_1l,buf_l,m_str,&retcode);
	if(check != 0) return -1;

	check = 0;
	wi_1l.mode = W_MENU;
	inp_GLIO_gwin(&wi_1l,buf_l,m_str,&retcode);
	if(retcode == 0)
	{
		check = 1;
		strcpy(buf,buf_l[0]);
	}

	wi_1l.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_1l,buf_l,m_str,&retcode);

	if(check) return 1;	/* ���͂��ꂽ */
	return 0;				/* �L�����Z�����ꂽ */
}

/*  ��������Q�ǂݍ��ޔėp�֐� */
int do_input_char_2l(char *buf1, char *buf2, char *m_str[])
{
	int check,retcode;

	strncpy(buf_l[0],buf1,60);
	strncpy(buf_l[1],buf2,60);

	wi_2l.mode = W_FWRITE;
	check = inp_GLIO_gwin(&wi_2l,buf_l,m_str,&retcode);
	if(check != 0) return -1;

	check = 0;
	wi_2l.mode = W_MENU;
	inp_GLIO_gwin(&wi_2l,buf_l,m_str,&retcode);
	if(retcode == 0)
	{
		check = 1;
		strcpy(buf1,buf_l[0]);
		strcpy(buf2,buf_l[1]);
	}

	wi_2l.mode = W_CLOSE;
	inp_GLIO_gwin(&wi_2l,buf_l,m_str,&retcode);

	if(check) return 1;	/* ���͂��ꂽ */
	return 0;				/* �L�����Z�����ꂽ */
}

/* ������(��) ����� */
int do_inp_fx(void)
{
	return do_input_char_1l(formula_string, m_str_inp_fx);
}

/* ������(��),������(��) ����� */
int do_inp_fxyt(void)
{
	return do_input_char_2l(formula_str_x, formula_str_y, m_str_inp_fxyt);
}

/* ����d��(��)�^d�� ����� */
int do_inp_dfx(void)
{
	return do_input_char_1l(formula_str_dfx, m_str_inp_dfx);
}

/* if �� ����� */
int do_inp_if(void)
{
	return do_input_char_1l(formula_str_if, m_str_inp_if);
}

/* �X�C�b�`�̔ėp�֐� */
int do_switch(int *sw, int max_sw, char *m_str[])
{
	int ret,i;

	w_sw.data_max = max_sw;		/* �ő�X�C�b�`���̒�` */
	w_sw.cur_topic = *sw;		/* ���݂̃X�C�b�` */

	/* ���݂̃X�C�b�`�Ɉ��t���� */
	for(i=0;i<w_sw.data_max;i++) m_str[i][1] = ' ';
	m_str[*sw-1][1] = 'O';

	w_sw.cur_topic = *sw;
	w_sw.mode = W_FWRITE;
	mnu_GLIO_gmenu(&w_sw,0,m_str,&ret);
	w_sw.mode = W_MENU;
	mnu_GLIO_gmenu(&w_sw,0,m_str,&ret);
	i = ret;
	w_sw.mode = W_CLOSE;
	mnu_GLIO_gmenu(&w_sw,0,m_str,&ret);

	if(i == 0x1b) return 0;	/* �L�����Z�����ꂽ */
	*sw = w_sw.cur_topic;		/* �X�C�b�`�̍X�V */
	return 1;	/* ���͂��ꂽ */
}

/* ���W�̎�ނ�I�� */
int do_sw_scale(void)
{
	return do_switch(&sw_scale, 3, m_str_sw_scale);
}

/* ���_���W�̎�ނ�I�� */
int do_sw_0(void)
{
	return do_switch(&sw_0, 4, m_str_sw_0);
}

/* �����⊮�� on/off */
int do_sw_line(void)
{
	return do_switch(&sw_gr_line, 2, m_str_sw_line);
}

/* �`�掞�̉�ʏ������Ɋւ���X�C�b�` */
int do_sw_init(void)
{
	return do_switch(&sw_initmode, 4, m_str_sw_init);
}

/* �{�[�_�[���W�̗L�����ւ���X�C�b�` */
int do_sw_bod(void)
{
	return do_switch(&sw_bod, 2, m_str_sw_bod);
}

/* if ���̗L�����Ɋւ���X�C�b�` */
int do_sw_if(void)
{
	return do_switch(&sw_if, 2, m_str_sw_if);
}