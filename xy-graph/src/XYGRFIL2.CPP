/* XYGRFILx.CPP
   XYGRPH.EXE �p�@�t�@�C������^�q�v���Z�X�@�v���O���� */
#include<stdio.h>
#include<string.h>
#include<alloc.h>
#include<process.h>
#include<conio.h>
#include<dos.h>
#include<glio.h>
#include"xygr.h"

#define	HELPWIN_LEN	22

#define HLP_ERR	"\r\n     �w���v�t�@�C����������܂���"
#define	GED_ERR	"\r\n     ���̨����ި�������s�ł��܂���"
#define	GSV_ERR	"\r\n      ���̨�����ނ����s�ł��܂���"
#define	GLD_ERR	"\r\n      ���̨��۰�ނ����s�ł��܂���"
#define	GPL_ERR	"\r\n      հ�ް����ނ����s�ł��܂���"
#define	TMP_ERR	"\r\n      �e���|�����[�t�@�C���@�G���["
#define	DAT_ERR	"\r\n         �f�[�^�t�@�C���@�G���["
#define	ENV_ERR	"\r\n          ���t�@�C���@�G���["
#define	MEM_ERR	"\r\n           ������������܂���"

/* �G���[���b�Z�[�W��\������ */
void do_error_win(char *str)
{
	int ret,dummy;

	/* �E�C���h�E�̍s����ς��� */
	dummy = w_info.data_max;
	w_info.data_max = 3;

	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);

	textcolor(w_info.color);

	gotoxy(1,1);
	cprintf("%s",str);

	getch();
	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);

	w_info.data_max = dummy;

}

/* �I�����C���w���v */
void do_help(void)
{
	FILE *in;
	char str[256];
	int page,i,counter;

	if((in = fopen("XYGRPH.HLP","rb")) == NULL)
	{	/* �w���v�t�@�C������ */
		do_error_win(HLP_ERR);
		return ;
	}

	gscreen(-1, 3, -1, -1);		/* �O���t�B�b�N�\����~ */
	win_mmake(2, 3, 79, 24, _c_box, W_RECT, W_PAGE_1);
	_wscroll = 0;	/* �X�N���[���̋֎~ */
	textattr(_c_mnu);

	page = 1;	/* �ŏ��͂P�y�[�W��\�� */

	for(;;)
	{
		/* �y�[�W�̐擪�֍s�� */
		counter = 0;
		fseek(in,0L,SEEK_SET);	/* �t�@�C���������߂� */
		for(;;)
		{
			if(fgets(str,100,in) == NULL)
			{	/* '^L' �t�@�C���G���h */
				page--;		/* ���̃y�[�W�ɖ߂� */
				fseek(in,0L,SEEK_SET);
				counter = 0;
				continue;
			}
			if(str[0] == 0x00)
			{	/* NULL �w���v�̏I��� */
				page--;		/* ���̃y�[�W�ɖ߂� */
				fseek(in,0L,SEEK_SET);
				counter = 0;
				continue;
			}
			if(str[0] == 0x0c) counter++;	/* ���y�[�W */
			if(counter >= page) break;
		}

		for(i=1;i<=HELPWIN_LEN;i++)
		{	/* ��ʂ̃N���A */
			gotoxy(1,i);
			cprintf("%78s"," ");
		}

		/* �\�� */
		gotoxy(1,1);
		for(i=1;i<=HELPWIN_LEN-1;i++)
		{
			if(fgets(str,100,in) == NULL)
			{	/* �t�@�C���G���h */
				break;
			}
			if(str[0] == 0xc) break;	/* ���y�[�W */
			gotoxy(1,i);
			cprintf("%s",str);		/* �\�� */
		}

		for(;;)
		{
			gotoxy(1,HELPWIN_LEN);
			cprintf("ROLL UP : ���y�[�W   ROLL DOWN : �O�y�[�W   ESC : �w���v�̏I��",page);
			i = mnu_wread_key();
			if(i == 0x3600 || i == 0x3700 || i == 0x001b) break;
		}
		if(i == 0x001b) break;	/* �I�� */
		if(i == 0x3700) page--;	/* �� �L�[ */
		if(i == 0x3600) page++;	/* �� �L�[ */
		if(page <= 0) page = 1;		/* �ŏ��̃y�[�W�͂P�y�[�W */

	}

	fclose(in);

	win_mdelete(2,3,79,24);		/* �E�C���h�E�̏��� */
	gscreen(3, 1, 0, 1);		/* �O���t�B�b�N�\���J�n */

	return ;
}

/* �O���t�B�b�N�G�f�B�^�[�N�� */
void exe_gedit(void)
{
	char exe_file[30],par[50],dummy[50];

	/* ���s�t�@�C���ƃp�����[�^�̊��� */
	strcpy(dummy,exe_gedit_file);
	strcpy(exe_file,strtok(exe_gedit_file," \n"));	/* ���s�t�@�C�� */
	strcpy(par,strtok(NULL,"\n"));					/* �p�����[�^ */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* ���s�ł��Ȃ����� */
		do_error_win(GED_ERR);
		return ;
	}
}

/* �w�x�Ղ�����[�N�� */
void exe_plot(void)
{
	char exe_file[30],par[50],dummy[50];

	/* ���s�t�@�C���ƃp�����[�^�̊��� */
	strcpy(dummy,exe_plot_file);
	strcpy(exe_file,strtok(dummy," \n"));	/* ���s�t�@�C�� */
	strcpy(par,strtok(NULL,"\n"));			/* �p�����[�^ */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* ���s�ł��Ȃ����� */
		do_error_win(GPL_ERR);
		return ;
	}
}

/* �摜�t�@�C���̏����o�� */
void do_save_file(void)
{
	char exe_file[30],par[50],dummy[50];

	if(!strlen(filename)) return;

	/* ���s�t�@�C���ƃp�����[�^�̊��� */
	strcpy(dummy,exe_gsv_file);
	strcpy(exe_file,strtok(dummy," \n"));	/* ���s�t�@�C�� */
	strcpy(par,strtok(NULL,"\n"));			/* �p�����[�^ */
	strcat(par," ");
	strcat(par,filename);	/* �p�����[�^�@�{�@�t�@�C���� */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* ���s�ł��Ȃ����� */
		do_error_win(GSV_ERR);
		return ;
	}
}

/* �摜�t�@�C���̓ǂݍ��� */
void do_load_file(void)
{
	char exe_file[30],par[50],dummy[50];

	if(!strlen(filename)) return;

	/* ���s�t�@�C���ƃp�����[�^�̊��� */
	strcpy(dummy,exe_gld_file);
	strcpy(exe_file,strtok(dummy," \n"));	/* ���s�t�@�C�� */
	strcpy(par,strtok(NULL,"\n"));			/* �p�����[�^ */
	strcat(par," ");
	strcat(par,filename);	/* �p�����[�^�@�{�@�t�@�C���� */

	if(spawnlp(P_WAIT,exe_file,exe_file,par,NULL) == -1)
	{	/* ���s�ł��Ȃ����� */
		do_error_win(GLD_ERR);
		return ;
	}
}

/* ���t�@�C���̏������� */
void do_save_env(void)
{
	FILE *fi;
	char _filename[80];
	int i;

	if(!strlen(filename)) return;

	strcpy(_filename,strtok(filename,"."));
	strcat(_filename,".ENV");
	if((fi = fopen(_filename,"wt")) == NULL)
	{	/* ����� �װ */
		do_error_win(ENV_ERR);
		return;
	}

	fprintf(fi,"XYGRPH ver2 ENV File\n");
	fprintf(fi,
			":GEDIT\n"
			"%s\n"
			":USERPROG\n"
			"%s\n"
			":GSAVE\n"
			"%s\n"
			":GLOAD\n"
			"%s\n"
			"\n",
			exe_gedit_file,
			exe_plot_file,
			exe_gsv_file,
			exe_gld_file);

	fprintf(fi,
			 "menu      = %04x\n"
			 "menu rev  = %04x\n"
			 "menu box  = %04x\n"
			 "bar menu  = %04x\n"
			 "bar topic = %04x\n"
			 "bar rev   = %04x\n"
			 "info      = %04x\n"
			 "info box  = %04x\n"
			 "col grph  = %04x\n"
			 "col scale = %04x\n"
			 "col zero  = %04x\n",
			 _c_mnu,
			 _c_mnuv,
			 _c_box,
			 _c_barb,
			 _c_bar,
			 _c_barv,
			 _c_info,
			 _cg_info,
			 _c_gr_grph,
			 _c_gr_scale,
			 _c_gr_0);


	fprintf(fi,"\n"
			   "background pattern\n");
	for(i=0;i<8;i++) fprintf(fi,"%02x,%02x,%02x,%02x\n",
							_bpat[i*4+0],_bpat[i*4+1],
							_bpat[i*4+2],_bpat[i*4+3]);
	fprintf(fi,"\n"
			   "info area pattern\n");
	for(i=0;i<8;i++) fprintf(fi,"%02x,%02x,%02x,%02x\n",
							_infopat[i*4+0],_infopat[i*4+1],
							_infopat[i*4+2],_infopat[i*4+3]);


	fclose(fi);

}

/* ���t�@�C���̓ǂݍ��� */
void do_load_env(void)
{
	FILE *fi;
	char dummy[70],_filename[80];
	int i,_a,_b,_c,_d;

	if(!strlen(filename)) return;

	strcpy(_filename,strtok(filename,"."));
	strcat(_filename,".ENV");
	if((fi = fopen(_filename,"rt")) == NULL)
	{	/* ����� �װ */
		do_error_win(ENV_ERR);
		return;
	}

	fgets(dummy,250,fi);	/* ���ق̽���� */
	fgets(dummy,250,fi);	/* ���Ă̽���� */
	fgets(dummy,250,fi);
	strcpy(exe_gedit_file,strtok(dummy,"\n"));
	fgets(dummy,250,fi);	/* ���Ă̽���� */
	fgets(dummy,250,fi);
	strcpy(exe_plot_file,strtok(dummy,"\n"));
	fgets(dummy,250,fi);	/* ���Ă̽���� */
	fgets(dummy,250,fi);
	strcpy(exe_gsv_file,strtok(dummy,"\n"));
	fgets(dummy,250,fi);	/* ���Ă̽���� */
	fgets(dummy,250,fi);
	strcpy(exe_gld_file,strtok(dummy,"\n"));

	fgets(dummy,250,fi);	/* ���Ă̽���� */
	fscanf(fi,
			 "menu      = %04x\n"
			 "menu rev  = %04x\n"
			 "menu box  = %04x\n"
			 "bar menu  = %04x\n"
			 "bar topic = %04x\n"
			 "bar rev   = %04x\n"
			 "info      = %04x\n"
			 "info box  = %04x\n"
			 "col grph  = %04x\n"
			 "col scale = %04x\n"
			 "col zero  = %04x\n",
			 &_c_mnu,
			 &_c_mnuv,
			 &_c_box,
			 &_c_barb,
			 &_c_bar,
			 &_c_barv,
			 &_c_info,
			 &_cg_info,
			 &_c_gr_grph,
			 &_c_gr_scale,
			 &_c_gr_0);

	fgets(dummy,250,fi);	/* ���Ă̽���� */
	fgets(dummy,250,fi);	/* ���Ă̽���� */
	for(i=0;i<8;i++)
	{
		fscanf(fi,"%03x,%03x,%03x,%03x\n",
							&_a,&_b,
							&_c,&_d);
		_bpat[i*4+0] = (char)_a;
		_bpat[i*4+1] = (char)_b;
		_bpat[i*4+2] = (char)_c;
		_bpat[i*4+3] = (char)_d;
	}
	fgets(dummy,250,fi);	/* ���Ă̽���� */
	for(i=0;i<8;i++)
	{
		fscanf(fi,"%03x,%03x,%03x,%03x\n",
							&_a,&_b,
							&_c,&_d);
		_infopat[i*4+0] = (char)_a;
		_infopat[i*4+1] = (char)_b;
		_infopat[i*4+2] = (char)_c;
		_infopat[i*4+3] = (char)_d;
	}

	fflush(fi);

	fclose(fi);
}