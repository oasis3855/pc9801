#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <glio.h>
#include <mem.h>

void main(void)
{
	char *data;
	unsigned int i,j /*,k*/,x,y;
	FILE *fi;

	if((data = malloc(10*150*2)) == NULL)
	{
		printf("メモリが足りません\n");
		exit(1);
	}

	if((fi = fopen("IMAGE.DAT","ab")) == NULL)
	{
		printf("ファイルがオープンできません\n");
		exit(1);
	}

	if(fseek(fi, 0L, SEEK_END)) printf("失敗\n");

	x = 0;
	y = 0;

	for(i=0; i<150; i++)
	{
		for(j=0; j<10; j++)
		{
			movedata(0xa800, (y+i)*80 + x + j,
					_DS, FP_OFF(data) + i*10 + j, sizeof(char));
		}
	}

	if(fwrite(data, sizeof(char), 10*150, fi) != 10*150)
	{
		printf("書き込めません\n");
	}
	printf("終了\n");
	fclose(fi);
	free(data);
}
