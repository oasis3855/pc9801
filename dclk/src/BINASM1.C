#include <stdio.h>
#include <stdlib.h>

void err(char *str);
unsigned long c;
FILE *in, *ot;

void main(void)
{
	char in_file[125], out_file[125];
	long i;
	unsigned int data;

	printf("�o�C�i���[�f�[�^���b����p�̔z��t�@�C���ɕϊ����܂��B\n");
	printf("�A�Z���u���E�R�[�h������f�[�^�t�@�C���� : ");
	gets(in_file);
	printf("�����o���t�@�C���� : ");
	gets(out_file);

	printf("%s -> %s �ɕϊ��J�n !\n", in_file, out_file);

	if((in = fopen(in_file, "rb")) == NULL)
	{
		printf("���̓t�@�C�����J���܂���\n");
		exit(1);
	}
	if((ot = fopen(out_file, "wt")) == NULL)
	{
		printf("�o�̓t�@�C�����J���܂���\n");
		fcloseall();
		exit(1);
	}

	fprintf(ot, "; DATA FILE ( %s ) for ASM Program\n"
				";\n"
				"; with C interface\n"
				";\n"
				"\n"
				"	DOSSEG\n"
				"	.MODEL	small\n"
				"	.DATA\n"
				"	PUBLIC C DIY\n"
				"\n"
				"DIY	db	", out_file);


	for(c=0;;)
	{
		for(i=0; i<10; i++,c++)
		{
			if(fread(&data, sizeof(char), 1, in) != 1) err("�I��");
			if(i != 0)
				if(fprintf(ot, ", ") == EOF) err("�o�ُ͈�");
/*			if(fprintf(ot, "0") == EOF) err("�o�ُ͈�");	*/
			if(fprintf(ot, "0%02hX", (char)data) == EOF) err("�o�ُ͈�");
			if(fprintf(ot, "h") == EOF) err("�o�ُ͈�");
		}
		if(fprintf(ot, "\n	db	") == EOF) err("�o�ُ͈�");
	}

}

void err(char *str)
{
	fprintf(ot, "\n	END\n");
	fprintf(ot, ";\n; %lu Bytes\n", c);
	fcloseall();
	printf("%s\n", str);
	exit(0);
}