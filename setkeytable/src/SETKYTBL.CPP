/*		SETKYTBL.C
*				(c) H.Inoue 1990
*		set all  key  by KEY.TBL
*
*	Ver 1.1	1990 JUL 12	developed from SETKEY.EXE V 1.0
*	Ver 2.0	1991 APR 26
*
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>

void beep(void);

void main(int ac, char *av[])
{
	struct REGPACK reg;
	char check,key_table[800];
	int i;
	FILE *in;
	char buf[100];
	char data_file[50];

	fprintf(stderr,"SETKYTBL.EXE   V 2.0	                (C) 1990,1991  ��� ���v\n"
			  "\n");

	if(ac == 2)
	{
		strcpy(data_file,av[1]);
		fprintf(stderr,"�� �ް� ̧�� %s\n",data_file);

	}
	else{
		fprintf(stderr,"KEY.EXE �ɂ���č쐬���ꂽ�L�[�e�[�u���E�f�[�^�t�@�C����ǂݍ���\n"
					"  �V�X�e���̃L�[�e�[�u�������������܂�\n"
					"KEY.EXE �� MS-DOS Ver 3.3x �̂��̂��g�p�\�i�`��������Ȃ瑼��\n"
					"  �o�[�W�����̂��̂��g�p�\�j\n"
					"\n"
					"�ǂݍ��ރt�@�C��������͂��ĉ����� (���� �݂̂� A:\\KEY.TBL)\n"
					" �� �ް� ̧�� : ");

		gets(buf);
		strcpy(data_file,buf);
		if(!strcmp(buf,"")) strcpy(data_file,"A:\\KEY.TBL");
		if(buf[0] == 0x1b) exit(0);

	}


	// sweep key data buffers
	for(i=0;i<=786;i++) key_table[i] = 0;

	in = fopen(data_file,"rb");
	if(in == NULL){
		fprintf(stderr,"\x1b[21m̧�� : %s �ǂݍ��݃G���[\x1b[m\n",data_file);
		beep();
		exit(1);
	}
	fseek(in,16,SEEK_SET);
	check = fread(key_table,sizeof(char),786,in);
	fclose(in);

	if(check == 0){
		fprintf(stderr,"\x1b[21m̧�� : %s �ް� ̫�ϯ� �װ\x1b[m\n",data_file);
		beep();
		exit(1);
	}

     reg.r_cx = 0x000d;
	reg.r_dx = FP_OFF(key_table);
	reg.r_ds = FP_SEG(key_table);
	reg.r_ax = 0x00ff;
	intr(0xdc,&reg);

	fprintf(stderr,"�ݒ�I��\n");

	exit(0);
}

void beep(void)
{
	sound(1500);
	delay(50);
	sound(500);
	delay(200);
	sound(2000);
	nosound();
}