/* MAN_MINx.CPP
   MANDEL.EXE 用　マンデルブロート計算 */
#include<stdio.h>
#include<stdlib.h>
#include<process.h>
#include<math.h>
#include<conio.h>
#include<etc.h>
#include"mandel2.h"
#include<glio.h>
#include<gr.h>

#define TRUE 1
#define FALSE 0
#define MAXSIZE 100000000L


void chg_palette(void)
{
	int i;

	switch(cur_palette)
	{
		case	1	:	// Natural Color
			PC98_BGI_16color_palette_init();
			break;
		case	2	:	// Green
			for(i=0;i<=15;i++)
			{
				color_palette_16(15-i,i,0,0);
			}
			color_palette_16(0,0,0,0);
			break;
		case	3	:	// Mono
			for(i=0;i<=15;i++)
			{
				color_palette_16(15-i,i,i,i);
			}
			color_palette_16(0,0,0,0);
			break;
		case	4	:	// Preview
			for(i=0;i<15;i=i+2)
			{
				color_palette_16(i  ,0 ,0 ,0 );
				color_palette_16(i+1,12,12,12);
			}
			break;
		default	:
			break;
	}
	return;
}

int do_mandel(void)
{
	/* use integer caluculation */
	long MandelX,MandelY;	// Mandel num
	long RealNum,ImagNum;	// complex
	double xratio,yratio;	// ratio x y
	float color_width;	// color width
	long LogicX,LogicY;		// point num
	int x,y,col,old_col;
	register int i;
	long LXArray[400],OldLY;
	void *sv_image;
	unsigned int im_size;
	unsigned char *ColorArray;

	if(xmin == xmax || ymin == ymax) return -2;

	if(f_mode <= 0) f_mode = 1;

	textattr(_c_info);

	im_size = ggetsize(0,0,Xwidth,0);
	sv_image = malloc(im_size+5);

	ColorArray = (unsigned char *)malloc(maxloop+1);
	if(ColorArray == NULL) return -1;


	xratio = (double)Xwidth / (double)(xmax - xmin);
	yratio = (double)Ywidth / (double)(ymax - ymin);

	color_width = (float)maxloop / (float)maxcolor;
	for(i=0;i<=maxloop+1;i++)
	{
		ColorArray[i] = ((unsigned char)(maxcolor - (float)i/color_width))%16;
     }

	for(i=0;i<=Xwidth;i++) LXArray[i] = (long)((double)i / xratio + xmin);

	OldLY = ymin - 100;	// below
	for(y=0;y<=Ywidth;y++)
	{
		gotoxy(1,8);
		cprintf("%3d %%",(int)((float)y/(float)Ywidth*100));

		LogicY = (long)((double)(Ywidth - y) / yratio + ymin);

		if(LogicY == OldLY)
		{
			gput(0+xw_min,y+yw_min,GPUT_COPY,sv_image);
			OldLY = LogicY;
			continue;
		}
		OldLY = LogicY;

		if(y%f_mode)
		{
			gput(0+xw_min,y+yw_min,GPUT_COPY,sv_image);
			continue;
		}

		for(x=0;x<=Xwidth;x++)
		{
			LogicX = LXArray[x];

			MandelX = LogicX;
			MandelY = LogicY;

			for(i=0;i<=maxloop;i++)
			{
				RealNum = MandelX * MandelX;
				ImagNum = MandelY * MandelY;
				if((RealNum + ImagNum) >= MAXSIZE) break;
				MandelY = (long)(MandelX * MandelY) / 2500L + LogicY;
				MandelX = (long)(RealNum - ImagNum) / 5000L + LogicX;
			}

			gr_pset(x+xw_min,y+yw_min,(int)ColorArray[i]);
		}
		gget(0+xw_min,y+yw_min,Xwidth+xw_min,y+yw_min,sv_image);
		key_buf_cls();
		if(*_km_m0 & 0x01) break;
	}
	free(ColorArray);
	free(sv_image);
	key_buf_cls();
	return 0;
}


int do_mandel_C(void)
{

	/* use floating point caluculation */
	long double MandelX,MandelY;	// Mandel num
	long double RealNum,ImagNum;	// complex
	float color_width;	// color width
	long double LogicX,LogicY;		// point num
	int x,y,col,old_col;
	register int i;
	long double LXArray[400],OldLY;
	void *sv_image;
	unsigned int im_size;
	unsigned char *ColorArray;

	if(dxmin == dxmax || dymin == dymax) return -2;

	if(f_mode <= 0) f_mode = 1;

	textattr(_c_info);

	im_size = ggetsize(0,0,Xwidth,0);
	sv_image = malloc(im_size+5);

	ColorArray = (unsigned char *)malloc(maxloop+1);
	if(ColorArray == NULL) return -1;

	color_width = (float)maxloop / (float)maxcolor;
	for(i=0;i<=maxloop+1;i++)
	{
		ColorArray[i] = ((unsigned char)(maxcolor - (float)i/color_width))%16;
     }


	for(i=0;i<=Xwidth;i++) LXArray[i] = (long double)i / dxratio + dxmin;

	OldLY = dymin - 100;	// below
	for(y=0;y<=Ywidth;y++)
	{
		gotoxy(1,8);
		cprintf("%3d %%",(int)((float)y/(float)Ywidth*100));

		LogicY = (long double)(Ywidth - y) / dyratio + dymin;

		if(LogicY == OldLY)
		{
			gput(0+xw_min,y+yw_min,GPUT_COPY,sv_image);
			OldLY = LogicY;
			continue;
		}
		OldLY = LogicY;

		if(y%f_mode)
		{
			gput(0+xw_min,y+yw_min,GPUT_COPY,sv_image);
			continue;
		}

		for(x=0;x<=Xwidth;x++)
		{
			LogicX = LXArray[x];

			MandelX = LogicX;
			MandelY = LogicY;

			for(i=0;i<=maxloop;i++)
			{
				RealNum = MandelX * MandelX;
				ImagNum = MandelY * MandelY;
				if((RealNum + ImagNum) >= (long double)4.00) break;
				MandelY = MandelX * MandelY * (long double)2.00 + LogicY;
				MandelX = RealNum - ImagNum + LogicX;
			}
			gr_pset(x+xw_min,y+yw_min,(int)ColorArray[i]);
		}
		gget(0+xw_min,y+yw_min,Xwidth+xw_min,y+yw_min,sv_image);

		key_buf_cls();
		if(*_km_m0 & 0x01) break;
	}

	free(ColorArray);
	free(sv_image);

	key_buf_cls();
	return 0;

}

void mk_mandel_win(void)
{
	chg_palette();
	gsetpattern(32,_bpat);
	gbarp(0,16,639,399,_BLACK);
	set_param();
	prn_data_win();
	gbarf(xw_min-2,yw_min-2,xw_max+2,yw_max+2,_cg_man,_BLACK);
}

int matherr(struct exception *p)
{
	do_error_win("\r\n         浮動小数点演算エラー");
	p->retval = 1.0;
	return 1;
}
