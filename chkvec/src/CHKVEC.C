/********************************************************************
 CHKVEC.C  ���荞���޸�� �����  Ver 1.00   for PC-9801
 (C)opyright 1992 ��� ���v

 Turbo C/C++, Quick C, LSI-C  �ź��߲ى\
 �A���ATC,QC,LC��INT 22,23,24h �� QC,LC��INT 0 �ɏ��X�̕s����N����
  �\���L��B�ް�̧�ق𑼂̺��ׂ߲ź��߲ق������̂Ƌ��L����Ƃ��́A
  ���ӂ��������悢�B
********************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>

#define	DATAFILENAME	"CHKVEC.DAT"	/* ��̫�Ă��ް�̧�ٖ� */

#define	PARAM		0
#define	FILEOPEN	1
#define	FILEREAD	2
#define	FILEWRITE	3


void chkvec_main(char *datafile, int key_press_check);
int create_newfile(FILE *out, char *datafile);
void chk_vector(FILE *in);
void vec_error(int vec_no, long saved_vec, long cur_vec);
void fil_error(FILE *in, int no);
void verbose_datafile(char *datafile);
void error(int no);

#ifdef __TURBOC__
/* Turbo C �ł͈ȉ����޸�����N�����ɕۑ� */
extern long _Int0Vector,_Int4Vector,_Int5Vector,_Int6Vector;
#endif /* __TURBOC__ */

int _error_exist;	/* �װ�̌��̶���� */

void main(int ac, char *av[])
{
	int i, key_press_check = 1;
	char datafile[125], task_no = 'c';

	strcpy(datafile, DATAFILENAME);	/* �ް�̧�ٖ��̏����� */
	for(i=1; i<ac; i++)
	{	/* �����̉�͋y���ް�̧�ٖ��̎擾 */
		if(av[i][0] == '-')
		{	/* ������׸�� �� '-' */
			switch(av[i][1]){
				case	'v'	:	/* �޸�ð��ق̕\�� */
					task_no = 'v';
					break;
				case	'k'	:	/* �������������ĉ������������̏ȗ� */
					key_press_check = 0;
					break;
				default		:	/* ���̑��̽��� */
					error(PARAM);
			}
		}
		else strcpy(datafile, av[i]);	/* �����ȊO���ް�̧�ٖ��ƔF�� */
	}

	/* ���ۂ̎d���ɕ��� */
	switch(task_no)
	{
		case	'v'		:	/* �޸�ð��ق̕\�� */
			verbose_datafile(datafile);
			break;
		case	'c'		:	/* ��̫�Ăł�INT�̌��� */
			chkvec_main(datafile, key_press_check);
			break;
	}
	exit(0);	/* �I�� */
}

/* CHKVEC �� �޸�� CHECK ٰ�� */
void chkvec_main(char *datafile, int key_press_check)
{
	FILE *in;

	if((in = fopen(datafile, "rb")) == NULL)
	{	/* �ް�̧�ق�������Ȃ��Ƃ��͐V�����t�@�C������� */
		fclose(in);
		if(create_newfile(in, datafile))
			printf("���荞���޸�� �ް�̧�� ���쐬���܂���\n");
		else printf("�ް�̧�� �̍쐬�𒆎~���܂���\n");

		return;
	}

	/* �������[�`�� */
	_error_exist = 0;	/* �װ����0�ɏ����� */
	printf("���荞���޸���������� ...\n");
	chk_vector(in);
	if(!_error_exist) printf("���� : ����������ꂽ�޸���͂���܂���\n");
	else
	{
		printf("\x1b[4m%d ���޸���������������Ă��܂�\x1b[m\n",_error_exist);
		if(key_press_check)
		{
			printf("\n�������������ĉ�����\n");
			getch();		/* PC-9801�ȊO�łͺ��߲ق������Ύg�p�\ */
		}
	}

	return;
}

/* ���݂̊��荞���޸���̕ۑ� */
int create_newfile(FILE *out, char *datafile)
{
	int i;
	unsigned long vec;

	printf("�V�����ް�̧�� %s ������Ă������ł���  (y / n)\n", datafile);
	for(;;)
	{
		i = toupper(getch());
		if(i == 'Y') break;
		if(i == 'N') return 0;	/* �쐬���Ȃ� */
	}

	if((out = fopen(datafile, "wb")) == NULL) fil_error(out, FILEOPEN);

	/* ���荞���޸���̏������� */
#ifdef __TURBOC__
	if(fwrite(&_Int0Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	for(i=1; i<= 3; i++)
	{
		vec = (long)getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
	if(fwrite(&_Int4Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	if(fwrite(&_Int5Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	if(fwrite(&_Int6Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	for(i=7; i<= 0xff; i++)
	{
		vec = (long)getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
#else	/* TURBO C �ȊO�ł� */
	for(i=0; i<= 0xff; i++)
	{
		vec = (long)_dos_getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
#endif /* __TURBOC__ */

	fclose(out);
	return 1;	/* �ް�̧�ق��쐬���� */
}

/* ���荞���޸���̌��� */
void chk_vector(FILE *in)
{
	int i;
	unsigned long saved_vec,cur_vec;

	/* ���荞���޸���̔�r */
#ifdef __TURBOC__
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int0Vector) vec_error(0, saved_vec, _Int0Vector);
	for(i=1; i<= 3; i++)
	{
		cur_vec = (long)getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int4Vector) vec_error(4, saved_vec, _Int4Vector);
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int5Vector) vec_error(5, saved_vec, _Int5Vector);
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int6Vector) vec_error(6, saved_vec, _Int6Vector);
	for(i=7; i<= 0xff; i++)
	{
		cur_vec = (long)getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
#else	/* TURBO C �ȊO�ł� */
	for(i=0; i<= 0xff; i++)
	{
		cur_vec = (long)_dos_getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
#endif /* __TURBOC__ */
}

/* �ύX���ꂽ�޸���̕\�� */
void vec_error(int vec_no, long saved_vec, long cur_vec)
{
	_error_exist++;		/* �������ݸ���� */
	printf("���荞���޸�� %02X �Ԃ��ύX����Ă��܂�  %04X:%04X �� %04X:%04X\n",
		   vec_no,
		   (unsigned int)((saved_vec & 0xffff0000L) >> 16),
		   (unsigned int)(saved_vec & 0xffffL),
		   (unsigned int)((cur_vec & 0xffff0000L) >> 16),
		   (unsigned int)(cur_vec & 0xffffL) );
}

/* �޸�� ð��� �̕\�� */
void verbose_datafile(char *datafile)
{
	FILE *in;
	int i;
	unsigned long vec;

	if((in = fopen(datafile, "rb")) == NULL) fil_error(in, FILEOPEN);
	printf("���荞�ݔԍ�  ������:�̾��\n"
		   "��������������������������\n");
	for(i=0; i<=0xff; i++)
	{	/* 1���\�� */
		if(fread(&vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		printf(" %02X             %04X:%04X\n",
			   i, (unsigned int)((vec & 0xffff0000L) >> 16),
			   (unsigned int)(vec & 0xffffL) );
	}
	fclose(in);

	return;
}

/* ̧�ق���Ă���G���[������ */
void fil_error(FILE *in, int no)
{
	fclose(in);
	error(no);
}


/* �G���[���� */
void error(int no)
{
	switch(no)
	{
		case	PARAM	:
			printf("���荞���޸�� �����  Ver 1.00		(C) 1992 ��� ���v\n\n"
				   "�g�p�@ : CHKVEC (-vk \x1b[4m�ް�̧�ٖ�\x1b[m)\n\n");
			break;
		case    FILEOPEN	:
			printf("̧�ق�������Ȃ��A���͓ǂݍ��߂܂���\n\n");
			break;
		case	FILEREAD	:
			printf("̧�ق��ǂ߂܂���\n\n");
			break;
		case	FILEWRITE	:
			printf("̧�ق������܂���\n\n");
			break;
	}
	exit(1);	/* �ُ�I�� */
}