/* MAN_FILx.CPP
   MANDEL.EXE 用　ファイル操作／子プロセス　プログラム */
#include<stdio.h>
#include<string.h>
#include<alloc.h>
#include<process.h>
#include<conio.h>
#include<dos.h>
#include"mandel2.h"
#include<glio.h>

char filename[32];
char tmpname[15];
char fildr=' ',tmpdr=' ';

#define	TMP_ERR	"\r\n      テンポラリーファイル　エラー"
#define	DAT_ERR	"\r\n         データファイル　エラー"
#define	ENV_ERR	"\r\n          環境ファイル　エラー"
#define	MEM_ERR	"\r\n           メモリが足りません"

void makefname(char *filepass, char *ext);

void make_fpass(char *filepass, char *ext)
{
	filepass[0] = 0;
	if(fildr != ' ')
	{
		filepass[0] = fildr;
		filepass[1] = ':';
		filepass[2] = (char)0;
	}
	strcat(filepass,strtok(filename,"."));
	strcat(filepass,ext);

	tmpname[0] = 0;
	if(tmpdr != ' ')
	{
		tmpname[0] = tmpdr;
		tmpname[1] = ':';
		tmpname[2] = (char)0;
	}
	strcat(tmpname,"MANDEL.TMP");
}



void save_file(void)
{
	void *sv_image, far *copy_sv_img;
	unsigned int im_size,y;
	int ret;
	FILE *fi;
	char sv_file[25];

	make_fpass(sv_file,".IMG");

	im_size = ggetsize(0+xw_min,0+yw_min,Xwidth+xw_min,Ywidth/4+yw_min);
	sv_image = malloc(im_size+100);
	if(sv_image == NULL)
	{
		do_error_win(MEM_ERR);
		return;
	}
	copy_sv_img = MK_FP(FP_SEG(sv_image),FP_OFF(sv_image));

	// open TMP
	if((fi = fopen(tmpname,"wb")) == NULL)
	{
		do_error_win(TMP_ERR);
		free(sv_image);
		return;
	}

	// save PARAM
	fwrite(&x_cen,sizeof(double),1,fi);
	fwrite(&y_cen,sizeof(double),1,fi);
	fwrite(&xy_scale,sizeof(double),1,fi);
	fwrite(&maxcolor,sizeof(int),1,fi);
	fwrite(&maxloop,sizeof(int),1,fi);
	fwrite(&f_mode,sizeof(int),1,fi);

	// save IMAGE
	for(y=0;y<4;y++)
	{
		gget(0+xw_min,Ywidth/4*y+yw_min,
			Xwidth+xw_min,Ywidth/4*(y+1)+yw_min,
			copy_sv_img);
		fwrite(sv_image,im_size,1,fi);
	}

	fclose(fi);

	free(sv_image);

	/* 圧縮ウインドウ */
	y = w_info.data_max;
	w_info.data_max = 3;
	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);
	textcolor(w_info.color);
	gotoxy(11,2);
	cputs("データファイル圧縮中");

	spawnl(P_WAIT,"MANDEL_C.EXE","MANDEL_C.EXE","e",tmpname,sv_file,NULL);

	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);
	w_info.data_max = y;


	if((fi = fopen(sv_file,"rb")) == NULL)
	{	/* ファイル存在のチェック */
		do_error_win(DAT_ERR);
		return;
	}
	fclose(fi);

	unlink(tmpname);

	return;
}


void load_file(void)
{
	void *sv_image, far *copy_sv_img;
	unsigned int im_size,y;
	int ret;
	FILE *fi;
	char sv_file[25];

	make_fpass(sv_file,".IMG");
	if((fi = fopen(sv_file,"rb")) == NULL)
	{	/* ファイル存在のチェック */
		do_error_win(DAT_ERR);
		return;
	}
	fclose(fi);

	/* 解凍ウインドウ */
	y = w_info.data_max;
	w_info.data_max = 3;
	w_info.mode = W_FWRITE;
	win_GLIO_gwin(&w_info,&ret);
	textcolor(w_info.color);
	gotoxy(11,2);
	cputs("データファイル解凍中");

	spawnl(P_WAIT,"MANDEL_C.EXE","MANDEL_C.EXE","d",sv_file,tmpname,NULL);

	w_info.mode = W_CLOSE;
	win_GLIO_gwin(&w_info,&ret);
	w_info.data_max = y;

	im_size = ggetsize(0+xw_min,0+yw_min,Xwidth+xw_min,Ywidth/4+yw_min);
	sv_image = malloc(im_size+100);
	if(sv_image == NULL)
	{
		do_error_win(MEM_ERR);
		return;
	}
	copy_sv_img = MK_FP(FP_SEG(sv_image),FP_OFF(sv_image));

	if((fi = fopen(tmpname,"rb")) == NULL)
	{
		free(sv_image);
		do_error_win(TMP_ERR);
		return;
	}

	// read PARAM
	fread(&x_cen,sizeof(double),1,fi);
	fread(&y_cen,sizeof(double),1,fi);
	fread(&xy_scale,sizeof(double),1,fi);
	fread(&maxcolor,sizeof(int),1,fi);
	fread(&maxloop,sizeof(int),1,fi);
	fread(&f_mode,sizeof(int),1,fi);

	mk_mandel_win();

	// read IMAGE
	for(y=0;y<4;y++)
	{
		fread(sv_image,im_size,1,fi);
		gput(0+xw_min,Ywidth/4*y+yw_min,
			GPUT_COPY,copy_sv_img);
	}

	fclose(fi);

	free(sv_image);

	unlink(tmpname);

	return;
}

void save_env(void)
{

	FILE *fi;
	char sv_file[25];
	int i;

	make_fpass(sv_file,".ENV");

	if((fi = fopen(sv_file,"wb")) == NULL)
	{	/* ｵｰﾌﾟﾝ ｴﾗｰ */
		do_error_win(ENV_ERR);
		return;
	}

	/* save */
	fprintf(fi,"menu = %04d\r\n"
			 "menu rev = %04d\r\n"
			 "menu box = %04d\r\n"
			 "bar back = %04d\r\n"
			 "bar topic = %04d\r\n"
			 "bar rev = %04d\r\n"
			 "info = %04d\r\n"
			 "Grph mandel boarder = %04d\r\n"
			 "Grph info boarder = %04d\r\n"
			 "\r\n"
			 "Grph background\r\n",
			 _c_mnu,
			 _c_mnuv,
			 _c_box,
			 _c_barb,
			 _c_bar,
			 _c_barv,
			 _c_info,
			 _cg_man,
			 _cg_info);

	for(i=0;i<8;i++) fprintf(fi,"%03hu,%03hu,%03hu,%03hu\r\n",
							_bpat[i*4+0],_bpat[i*4+1],
							_bpat[i*4+2],_bpat[i*4+3]);
	fprintf(fi,"\r\nGrph info\r\n");
	for(i=0;i<8;i++) fprintf(fi,"%03hu,%03hu,%03hu,%03hu\r\n",
							_infopat[i*4+0],_infopat[i*4+1],
							_infopat[i*4+2],_infopat[i*4+3]);

	fprintf(fi,"\r\ndata file drive = '%c'\r\n"
			 "\r\n"
			 "temp file drive = '%c'\r\n"
			 "\r\n",
			 fildr,tmpdr);



	fprintf(fi,"\r\nend.\r\n");

	fclose(fi);

	return;
}

void load_env(void)
{

	FILE *fi;
	char sv_file[25];
	int i,_a,_b,_c,_d;

	make_fpass(sv_file,".ENV");

	if((fi = fopen(sv_file,"rb")) == NULL)
	{	/* ｵｰﾌﾟﾝ ｴﾗｰ */
		do_error_win(ENV_ERR);
		return;
	}


	/* save */
	fscanf(fi,"menu = %04d\r\n"
			 "menu rev = %04d\r\n"
			 "menu box = %04d\r\n"
			 "bar back = %04d\r\n"
			 "bar topic = %04d\r\n"
			 "bar rev = %04d\r\n"
			 "info = %04d\r\n"
			 "Grph mandel boarder = %04d\r\n"
			 "Grph info boarder = %04d\r\n"
			 "\r\n"
			 "Grph background\r\n",
			 &_c_mnu,
			 &_c_mnuv,
			 &_c_box,
			 &_c_barb,
			 &_c_bar,
			 &_c_barv,
			 &_c_info,
			 &_cg_man,
			 &_cg_info);

	for(i=0;i<8;i++)
	{
		fscanf(fi,"%03d,%03d,%03d,%03d\r\n",
							&_a,&_b,
							&_c,&_d);
		_bpat[i*4+0] = (char)_a;
		_bpat[i*4+1] = (char)_b;
		_bpat[i*4+2] = (char)_c;
		_bpat[i*4+3] = (char)_d;
	}
	fscanf(fi,"\r\nGrph info\r\n");
	for(i=0;i<8;i++)
	{
		fscanf(fi,"%03d,%03d,%03d,%03d\r\n",
							&_a,&_b,
							&_c,&_d);
		_infopat[i*4+0] = (char)_a;
		_infopat[i*4+1] = (char)_b;
		_infopat[i*4+2] = (char)_c;
		_infopat[i*4+3] = (char)_d;
	}
	fscanf(fi,"\r\ndata file drive = '%c'\r\n"
			 "\r\n"
			 "temp file drive = '%c'\r\n"
			 "\r\n",
			 &fildr,&tmpdr);


	rewind(fi);
	fflush(fi);

	fclose(fi);

	return;
}

