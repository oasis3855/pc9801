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

	printf("バイナリーデータをＣ言語用の配列ファイルに変換します。\n");
	printf("アセンブラ・コード化するデータファイル名 : ");
	gets(in_file);
	printf("書き出すファイル名 : ");
	gets(out_file);

	printf("%s -> %s に変換開始 !\n", in_file, out_file);

	if((in = fopen(in_file, "rb")) == NULL)
	{
		printf("入力ファイルを開けません\n");
		exit(1);
	}
	if((ot = fopen(out_file, "wt")) == NULL)
	{
		printf("出力ファイルを開けません\n");
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
			if(fread(&data, sizeof(char), 1, in) != 1) err("終了");
			if(i != 0)
				if(fprintf(ot, ", ") == EOF) err("出力異常");
/*			if(fprintf(ot, "0") == EOF) err("出力異常");	*/
			if(fprintf(ot, "0%02hX", (char)data) == EOF) err("出力異常");
			if(fprintf(ot, "h") == EOF) err("出力異常");
		}
		if(fprintf(ot, "\n	db	") == EOF) err("出力異常");
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
