#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<io.h>
#include<dos.h>
#include<ctype.h>

void file_open(void);
void file_error(char *str);
void setnum(void);
int write_num(int line);
void re_write(void);
void help(void);


char data_file[50];
char new_file[50];
int over_write,start_line,line_step,zero;
FILE *in,*ot;


void main(int ac, char *av[])
{
	int i,j;

	over_write = 0;
	zero = 1;
	j = 0;
	strcpy(data_file,"");
	strcpy(new_file,"");

	if(ac == 1) help();
	for(i=1;i<ac;i++)
	{
		if(av[i][0] == '-')
		{
			switch(av[i][1])
			{
				case	'l'	:	start_line = atoi(av[i]+2);
							break;
				case	's'	:	line_step = atoi(av[i]+2);
							break;
				case	'z'	:	zero = 0;
							break;
				default	:	help();
			}
		}
		else
		{
			if(j == 0) strcpy(data_file,strlwr(av[i]));
			if(j == 1) strcpy(new_file,strlwr(av[i]));
			if(j >= 2) help();
			j++;
		}
	}

	if(!strcmpi(new_file,""))
	{
		strcpy(new_file,data_file);
		over_write = 1;
	}
	if(!strcmpi(new_file,data_file))
	{
		strcpy(new_file,"setnum.tmp");
		over_write = 1;
	}

	if(start_line > 30000 || start_line < 1) start_line = 100;
	if(line_step > 5000 || line_step < 1) line_step = 10;

	fprintf(stderr,"SETNUM �s�ԍ�����   Ver 2.0                        (C) 1990,1991 ��� ���v\n"
				" ̧�� %s  ->  ̧�� %s\n"
				" ײ� %04d , �ï�� %04d\n"
				"����������\n",data_file,new_file,start_line,line_step);

	file_open();
	setnum();
	fcloseall();
	re_write();

	fprintf(stderr,"�I��\n");
}

void file_open(void)
{
	in = fopen(data_file,"rb");
	if(in == NULL) file_error(data_file);
	_chmod(new_file,1,FA_ARCH);
	unlink(new_file);
	ot = fopen(new_file,"wb");
	if(ot == NULL) file_error(new_file);
}

void file_error(char *str)
{
	fprintf(stderr,"\x1b[21m̧�� : %s ����� / ײ� �װ\x1b[m\n",str);
	sound(1500);
	delay(50);
	sound(500);
	delay(150);
	sound(2000);
	nosound();
	fcloseall();
	exit(1);
}

void setnum(void)
{
	int i,buf,check;
	unsigned counter=0;

	i = start_line;

	if(write_num(i)) file_error(new_file);
	for(;;)
	{
		buf = fgetc(in);
		if(buf == EOF) break;
		if(fputc(buf,ot)==EOF) file_error(new_file);
		if(buf == 0x0a)	// LF
		{
			i += line_step;
			if(i < 0)
			{
				fprintf(stderr,"\n\x1b[21m�s�ԍ��@�I�[�o�[�t���[\n"
							"�s�ԍ��J�n�l�^�X�e�b�v�����������ĉ�����\x1b[m\n");
				sound(1500);
				delay(50);
				sound(500);
				delay(150);
				sound(2000);
				nosound();
				fcloseall();
				exit(1);
			}
			if(write_num(i)) file_error(new_file);
			if(!(counter%50))
			{
				fputc('.',stderr);
				if(kbhit())
				{
					if(getch() == 0x1b)
					{
						fprintf(stderr,"\nESCAPE\n");
						exit(0);
					}
				}
			}
			counter++;
		}
	}
	fprintf(stderr,"\n");

	return;
}

int write_num(int line)
{
	static char l_buf[20];
	static int dmy;

	if(zero)
	{
		sprintf(l_buf,"%04d",line);
		dmy = fwrite(l_buf,sizeof(char),strlen(l_buf),ot);
	}
	else
	{
		sprintf(l_buf,"%d",line);
		dmy = fwrite(l_buf,sizeof(char),strlen(l_buf),ot);
	}


	if(dmy != strlen(l_buf)) return 1;
	return 0;
}

void re_write(void)
{
	int i;

	if(over_write)
	{
		fprintf(stderr,"̧�� %s ���㏑�����܂��� (y/n) : ",data_file);
		for(;;)
		{
			i = tolower(getch());
			fprintf(stderr,"\n");
			if(i == 'y' || i == 'n') break;
		}
		if(i == 'y')
		{
			_chmod(data_file,1,FA_ARCH);
			unlink(data_file);
			rename(new_file,data_file);	// new_file -> data_file
		}
		else fprintf(stderr,"����߂܂���\n");
	}

	return;
}

void help(void)
{
	fprintf(stderr,"SETNUM     Ver 2.0                    (C) 1990,1991 ��� ���v\n"
				"÷�čs�ԍ������v���O����\n"
				"\n"
				"�g�p�@\n"
				" SETNUM [�ǂݍ���̧��] ([�����o��̧��]) (-l�J�n�s) (-s�X�e�b�v) (-z(���))\n"
				" ( ) �̍��ڏȗ����ɂ́A�����o��̧�قƓǂݍ���̧�ق͓���\n"
				" �܂��A�J�n�s�͂P�O�O�A�X�e�b�v�͂P�O�ł�\n"
				" -z �I�v�V�����ōs�ԍ��擪���[���Ŗ��߂܂���\n"
				"�s�� int �^( 32767 �܂� )���z����ƈُ�I�����܂�\n"
				"\n");
	exit(1);

}