/* RGB �`���ŃZ�[�u�^���[�h      ���[�W���f���ŃR���p�C�����ĉ����� */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>

void do_rgb_save(char *body);
void do_rgb_load(char *body);
int do_save(FILE *fi, int SEGMENT);
int do_load(FILE *fi, int SEGMENT);
void make_filename(char *filename, char *body, char *ext);

void main(int ac, char *av[])
{
	if(ac == 2) do_rgb_save(av[1]);
	if(ac == 3 && !strcmp(av[1],"-l")) do_rgb_load(av[2]);

	/* �G���[�\�� */
	puts("GRGBSV						(C)1992 Hirokazu Inoue\n"
		 ".R1 .G1 .B1 .E1 (���) �`���ŉ摜���Z�[�u�^���[�h���܂��B\n"
		 "�Z�[�u : GRGBSV <filename>\n"
		 "���[�h : GRGBSV -l <filename>\n"
		 "�������͖߂�l�͂O�ł��B\n"
		 "�߂�l���P�F���Ұ��װ�A�Q�F̧�ٴװ�A�R�F�e�ʕs��\n");
	exit(1);	/* �p�����[�^���P�ȊO */
}

/* �Z�[�u */
void do_rgb_save(char *body)
{
	FILE *fi;
	char filename[100];

	make_filename(filename, body, ".B1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_save(fi,0xa800) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	make_filename(filename, body, ".R1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_save(fi,0xb000) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	make_filename(filename, body, ".G1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_save(fi,0xb800) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	make_filename(filename, body, ".E1");
	if((fi = fopen(filename,"wb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_save(fi,0xe000) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	exit(0);
}

/* ���[�h */
void do_rgb_load(char *body)
{
	FILE *fi;
	char filename[100];

	make_filename(filename, body, ".B1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_load(fi,0xa800) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	make_filename(filename, body, ".R1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_load(fi,0xb000) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	make_filename(filename, body, ".G1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_load(fi,0xb800) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	make_filename(filename, body, ".E1");
	if((fi = fopen(filename,"rb")) == NULL) exit(2);	/* �I�[�v���G���[ */
	if(do_load(fi,0xe000) != 0)
	{
		fclose(fi);
		exit(3);	/* �e�ʕs�� */
	}
	fclose(fi);

	exit(0);
}

/* �t�@�C�����̃{�f�B�[�Ɋg���q��t���� */
void make_filename(char *filename, char *body, char *ext)
{
	strcpy(filename, strtok(body,"."));
	strcat(filename,ext);
}

/* �t�@�C���� SEGMENT:0 ���������� */
int do_save(FILE *fi, int SEGMENT)
{
	char *VRAM;

	VRAM = (char far *)MK_FP(SEGMENT,0);
	if(fwrite(VRAM, sizeof(char), 0x7d00, fi) != 0x7d00) return 1;

	return 0;	/* ���� */
}

/* �t�@�C����� SEGMENT:0 �ɓǂݍ��� */
int do_load(FILE *fi, int SEGMENT)
{
	char *VRAM;

	VRAM = (char far *)MK_FP(SEGMENT,0);
	if(fread(VRAM, sizeof(char), 0x7d00, fi) != 0x7d00) return 1;

	return 0;	/* ���� */
}