/* GREDMNUx.CPP
   GEDIT.EXE ƒOƒ‰ƒtƒBƒbƒNƒGƒfƒBƒ^[@ƒƒjƒ…[@ƒvƒƒOƒ‰ƒ€ */
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<conio.h>
#include<glio.h>
#include<etc.h>
#include<txtl.h>

int mus_scrpoint(int *_x1, int *_y1, int *_x2, int *_y2, int swnum);
int mus_in_rect(int _x1, int _y1, int _x2, int _y2, int _x, int _y);
void mus_dsp_sw(int *x1, int *y1, int *x2, int *y2, int num, int m_x, int m_y);
void mk_rev_sw(int x1, int y1, int x2, int y2, unsigned int attr, unsigned int *sv_attr);
void reset_csr_sw(void);
void submnu_line(void);
void submnu_box(void);
void submnu_style(void);
void submnu_system(void);
void submnu_color(unsigned int *col, char *mes);
void disp_Copyright(void);
void quit_ArtWriter(void);
void write_char(void);
int  input_str(void);

void until_mouse_wait(void);
void LowVideo_palette(void);


extern void mouse_locate_boarder(int x1, int y1, int x2, int y2, int& cx, int& cy);
extern void write_line(void);
extern void write_box(int mode);
extern void set_char_pattern(void);
extern void set_fill_pattern(void);
extern void write_free(int mode);


struct REGPACK r;
unsigned int title_bar,rev_sw,csr_sw,fore_color,fill_color,back_color,char_mode;
char char_buf[60];

#define main_title_bar	"\x1b[47m GEDIT.EXE" \
					"@%2s@@\x1b[46m ƒ‰ƒCƒ“ \x1b[47m" \
					" \x1b[46m •Â—Ìˆæ \x1b[47m \x1b[46mƒXƒ^ƒCƒ‹\x1b[47m" \
					" \x1b[46m@•¶Žš@\x1b[47m \x1b[46mƒVƒXƒeƒ€\x1b[47m" \
					"@@@@@@@   \x1b[m"
int main_title_bar_xy[] = {96,112,144,208,216,280,288,352,360,424,432,496,616,632,777};

int sub_line_x[] = {144,208,144,208,144,208,208,224};
int sub_line_y[] = {32,48,48,64,64,80,16,32};
int sub_box_x[] = {216,280,216,280,216,280,216,280,280,296};
int sub_box_y[] = {32,48,48,64,64,80,80,96,16,32};
int sub_style_x[] = {288,352,288,352,288,352,288,352,352,368};
int sub_style_y[] = {32,48,48,64,64,80,80,96,16,32};
int sub_system_x[] = {432,496,432,496,496,512};
int sub_system_y[] = {32,48,48,64,16,32};
int sub_col_x[] = {184,232,184,232,184,232,184,232,184,232,184,232,184,232,
				344,392,344,392,344,392,344,392,344,392,344,392,344,392,344,392,
				472,488};
int sub_col_y[] = {144,160,160,176,176,192,192,208,208,224,224,240,240,256,
				144,160,160,176,176,192,192,208,208,224,224,240,240,256,256,272,
				80,96};

int Copyright_x[] = {280,344};
int Copyright_y[] = {208,224};
int quit_AW_x[] = {208,272,304,416};
int quit_AW_y[] = {160,176,160,176};
int input_str_x[] = {184,248,328,440};
int input_str_y[] = {160,176,160,176};
int x1[20],x2[20],y1[20],y2[20];

void main(void)
{
	int ch,i;

	// init var
	title_bar = 0;	// Up title
	rev_sw = 0;
	csr_sw = 0;
	// GLIO color & copy mode defaults
	fore_color = _LIGHTGREEN;
	fill_color = _LIGHTGREEN;
	back_color = _BLACK;
	char_mode = GPUT_COPY;

	r.r_ax = 0x00;		// check mouse driver
	intr(0x33,&r);
	if(r.r_ax == 0) return;

	r.r_ax = 0x0f;		// set m/s
	r.r_cx = 5;
	r.r_dx = 5;
	intr(0x33,&r);

	r.r_ax = 0x04;		// locate(0,0)
	r.r_cx = 0;
	r.r_dx = 0;
	intr(0x33,&r);

	r.r_ax = 0x01;		// cursor ON
	intr(0x33,&r);

	ginit();
	gscreen(3,-1,-1,-1);
	gcolor(fore_color,back_color,2);
	LowVideo_palette();
	gpmode(GPUT_OR);
	gsetpattern(6, "\x55\xAA\x00\xAA\x55\x00");

	printf("\x1b[>5h\x1b[>1h\x1b[2J");
	printf(main_title_bar,title_bar ? "£":"¥");
	disp_Copyright();
	printf("\x1b[>5h\x1b[>1h\x1b[2J");
	printf(main_title_bar,title_bar ? "£":"¥");
	for(i=0;i<7;i++)
	{
		x1[i] = main_title_bar_xy[i*2+0];
		x2[i] = main_title_bar_xy[i*2+1];
		y1[i] = 1;
		y2[i] = 16;
	}

	for(;;)
	{
		ch = mus_scrpoint(x1,y1,x2,y2,7);
		r.r_ax = 0x01;		// cursor ON
		intr(0x33,&r);

		switch(ch)
		{
			case	1	:
				title_bar ^= 0xffff;
				printf("%s%80s%s",title_bar ? "\x1b[1;1H":"\x1b[25;1H"," ",
							    title_bar ? "\x1b[25;1H":"\x1b[1;1H");
				printf(main_title_bar,title_bar ? "£":"¥");
				break;
			case	2	:
				submnu_line();
				break;
			case	3	:
				submnu_box();
				break;
			case	4	:
				submnu_style();
				break;
			case	5	:
				write_char();
				break;
			case	6	:
				submnu_system();
				break;
			case	7	:
				printf("\x1b[2J\x1b[1;78H\x1b[47m \x1b[m");
				PC98_BGI_16color_palette_init();
				do
				{
					r.r_ax = 0x03;		// read x/y & button
					intr(0x33,&r);

				}while(!r.r_bx);
				printf("\x1b[2J");
				printf("%s%80s%s",title_bar ? "\x1b[1;1H":"\x1b[25;1H"," ",
							    title_bar ? "\x1b[25;1H":"\x1b[1;1H");
				printf(main_title_bar,title_bar ? "£":"¥");
				LowVideo_palette();
				break;

		}

		/******** reset xy *********/
		for(i=0;i<7;i++)
		{
			x1[i] = main_title_bar_xy[i*2+0];
			x2[i] = main_title_bar_xy[i*2+1];
		}
		if(title_bar)
		{
			for(i=0;i<7;i++)
			{
				y1[i] = 384;
				y2[i] = 400;
			}
		}
		else
		{
			for(i=0;i<7;i++)
			{
				y1[i] = 1;
				y2[i] = 16;
			}
		}
		/****** reset .. *********/

	}
}


int mus_scrpoint(int *_x1, int *_y1, int *_x2, int *_y2, int swnum)
{
	static int i,m_x,m_y;

	for(;;)
	{
		if(kbhit() && getch() == 0x1b) // ESC
		{
			return 0;
		}
		if(kbhit() && getch() == 'q') /* Q ·° */
		{
			r.r_ax = 0x02;		// cursor OFF
			intr(0x33,&r);
			PC98_BGI_16color_palette_init();
			key_buf_cls();
			printf("\x1b[2J");	// return to XYGRAPH.EXE
			exit(0);
		}


		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);
		m_x = r.r_cx;		// 0 - 639
		m_y = r.r_dx;		// 0 - 399
		if(r.r_bx & 0x01)	// if LIGHT BUTTON
		{
			until_mouse_wait();
			for(i=0;i<swnum;i++)
			{
				if(mus_in_rect(*(_x1+i), *(_y1+i), *(_x2+i), *(_y2+i), m_x, m_y))
				{
					mus_dsp_sw(x1,y1,x2,y2,0 /* no sw */,m_x,m_y);
					return (i+1);
				}
			}
		}
		mus_dsp_sw(x1,y1,x2,y2,swnum,m_x,m_y);
	}
}


int mus_in_rect(int _x1, int _y1, int _x2, int _y2, int _x, int _y)
{
	if( _x <= _x2 &&
	    _x >= _x1 &&
	    _y <= _y2 &&
	    _y >= _y1    ) return 1;

	return 0;
}

unsigned int sv_attr,swx1,swy1,swx2,swy2,curx,cury,ocurx,ocury,csr_attr,csr_ever;
void mus_dsp_sw(int *x1, int *y1, int *x2, int *y2, int num, int m_x, int m_y)
{
	static int i,cur_sw;

	csr_ever = 0;
	curx = m_x/8+1;
	cury = m_y/16+1;
	if(csr_sw)
	{
		if(ocurx == curx && ocury == cury) csr_ever = 1;
		else
		{
			mk_rev_sw(ocurx,ocury,ocurx,ocury,csr_attr,&csr_attr);
			csr_sw = 0;
		}
	}
	for(i=0;i<num;i++)
	{
		if(mus_in_rect(*(x1+i), *(y1+i), *(x2+i), *(y2+i), m_x, m_y))
		{
			cur_sw = i+1;
			if(cur_sw == rev_sw)
			{
				return;
			}
			else
			{
				if(csr_sw)
				{
					mk_rev_sw(ocurx,ocury,ocurx,ocury,csr_attr,&csr_attr);
					csr_sw = 0;
				}
				if(rev_sw) mk_rev_sw(swx1,swy1,swx2,swy2,sv_attr,&sv_attr);
				swx1 = *(x1+i)/8 + 1;
				swx2 = *(x2+i)/8;
				swy1 = *(y1+i)/16 + 1;
				swy2 = *(y2+i)/16;
				rev_sw = cur_sw;
				mk_rev_sw(swx1,swy1,swx2,swy2,0x85,&sv_attr);
				return;
			}
		}
	}

	if(rev_sw)
	{
		mk_rev_sw(swx1,swy1,swx2,swy2,sv_attr,&sv_attr);
		rev_sw = 0;
	}

	ocurx = curx;
	ocury = cury;
	if(num && !csr_ever)
	{	/* num == 0 is switch delete mode */
		mk_rev_sw(curx,cury,curx,cury,0x25,&csr_attr);
		csr_sw = 1;
	}
}

void reset_csr_sw(void)
{
	if(csr_sw)
	{
		mk_rev_sw(ocurx,ocury,ocurx,ocury,csr_attr,&csr_attr);
		csr_sw = 0;
	}
	return;
}



unsigned int far *vram_attr = (unsigned int far *)MK_FP(0xa200,0);

void mk_rev_sw(int x1, int y1, int x2, int y2, unsigned int attr, unsigned int *sv_attr)
{
	static int i,j;

	*sv_attr = *(vram_attr + 80*(y1-1) + (x1-1));

	for(i=x1;i<=x2;i++)
	{
		for(j=y1;j<=y2;j++)
		{
			*(vram_attr + 80*(j-1) + (i-1)) = attr;
		}
	}
	return;
}

void submnu_box(void)
{
	int i,ch;

	printf("\x1b[47m"
		  "\x1b[2;26H†±†£†£†£†£ "
		  "\x1b[3;26H†¥\x1b[46m‹éŒ`@@\x1b[47m†¥"
		  "\x1b[4;26H†¥\x1b[46m‹éŒ`“h‚è\x1b[47m†¥"
		  "\x1b[5;26H†¥\x1b[46m‘È‰~@@\x1b[47m†¥"
		  "\x1b[6;26H†¥\x1b[46m‘È‰~“h‚è\x1b[47m†¥"
		  "\x1b[7;26H†¹†£†£†£†£†½\x1b[m");

	for(i=0;i<5;i++)
	{
		x1[i] = sub_box_x[i*2+0];
		x2[i] = sub_box_x[i*2+1];
		y1[i] = sub_box_y[i*2+0];
		y2[i] = sub_box_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,5);

	reset_csr_sw();
	for(i=2;i<=7;i++) printf("\x1b[%d;26H%13s",i," ");

	switch(ch)
	{
		case	1	:
			PC98_BGI_16color_palette_init();
			write_box(0);
			LowVideo_palette();
			break;
		case	2	:
			PC98_BGI_16color_palette_init();
			write_box(1);
			LowVideo_palette();
			break;
		case	3	:
			PC98_BGI_16color_palette_init();
			write_box(2);
			LowVideo_palette();
			break;
		case	4	:
			PC98_BGI_16color_palette_init();
			write_box(3);
			LowVideo_palette();
			break;
	}
	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void submnu_style(void)
{
	int i,ch;

	printf("\x1b[47m"
		  "\x1b[2;35H†±†£†£†£†£ "
		  "\x1b[3;35H†¥\x1b[46m•¶Žš•`‰æ\x1b[47m†¥"
		  "\x1b[4;35H†¥\x1b[46m•`‰æF@\x1b[47m†¥"
		  "\x1b[5;35H†¥\x1b[46m”wŒiF@\x1b[47m†¥"
		  "\x1b[6;35H†¥\x1b[46m—ÌˆæF@\x1b[47m†¥"
		  "\x1b[7;35H†¹†£†£†£†£†½\x1b[m");

	for(i=0;i<5;i++)
	{
		x1[i] = sub_style_x[i*2+0];
		x2[i] = sub_style_x[i*2+1];
		y1[i] = sub_style_y[i*2+0];
		y2[i] = sub_style_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,5);

	reset_csr_sw();
	for(i=2;i<=7;i++) printf("\x1b[%d;35H%13s",i," ");
	switch(ch)
	{
		case	1	:	;break;
		case	2	:	submnu_color(&fore_color,"•`‰æF^ƒtƒHƒAƒOƒ‰ƒ“ƒh ƒJƒ‰[");break;
		case	3	:
			submnu_color(&back_color,"”wŒiF^ƒoƒbƒNƒOƒ‰ƒ“ƒh ƒJƒ‰[");
			gcolor(fore_color,back_color,2);
			break;
		case	4	:	submnu_color(&fill_color,"—ÌˆæF^ƒtƒBƒ‹ ƒJƒ‰[");break;

	}
	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void submnu_system(void)
{
	int i,ch;

	printf("\x1b[47m"
		  "\x1b[2;53H†±†£†£†£†£ "
		  "\x1b[3;53H†¥\x1b[46mAbout ..\x1b[47m†¥"
		  "\x1b[4;53H†¥\x1b[46mI—¹@@\x1b[47m†¥"
		  "\x1b[5;53H†¹†£†£†£†£†½\x1b[m");

	for(i=0;i<3;i++)
	{
		x1[i] = sub_system_x[i*2+0];
		x2[i] = sub_system_x[i*2+1];
		y1[i] = sub_system_y[i*2+0];
		y2[i] = sub_system_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,3);

	reset_csr_sw();
	for(i=2;i<=5;i++) printf("\x1b[%d;53H%13s",i," ");

	switch(ch)
	{
		case	1	:	disp_Copyright();break;
		case	2	:	quit_ArtWriter();break;
	}
	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void submnu_line(void)
{
	int i,ch;

	printf("\x1b[47m"
		  "\x1b[2;17H†±†£†£†£†£ "
		  "\x1b[3;17H†¥\x1b[46m’¼ü@@\x1b[47m†¥"
		  "\x1b[4;17H†¥\x1b[46m‹OÕ(‘¾)\x1b[47m†¥"
		  "\x1b[5;17H†¥\x1b[46m‹OÕ(×)\x1b[47m†¥"
		  "\x1b[6;17H†¹†£†£†£†£†½\x1b[m");

	for(i=0;i<4;i++)
	{
		x1[i] = sub_line_x[i*2+0];
		x2[i] = sub_line_x[i*2+1];
		y1[i] = sub_line_y[i*2+0];
		y2[i] = sub_line_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,4);

	reset_csr_sw();
	for(i=2;i<=6;i++) printf("\x1b[%d;17H%13s",i," ");
	switch(ch)
	{
		case	1	:
			PC98_BGI_16color_palette_init();
			write_line();
			LowVideo_palette();
			break;
		case	2	:
			PC98_BGI_16color_palette_init();
			write_free(0);
			LowVideo_palette();
			break;
		case	3	:
			PC98_BGI_16color_palette_init();
			write_free(1);
			LowVideo_palette();
			break;
	}
	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void submnu_color(unsigned int *col, char *mes)
{
	static int i,ch;

	printf("\x1b[6;20H\x1b[47m†®†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢ ");
	printf("\x1b[7;20H†¤@%32s@@†¤",mes);
	printf("\x1b[8;20H†¤@@@@@@@@@@@@@@@@@@@†¤");
	printf("\x1b[9;20H†¤@@@@@@@@@@@@@@@@@@@†¤");
	printf("\x1b[10;20H†¤@\x1b[46m@•@\x1b[47m@@@\x1b[40m@\x1b[47m@@@\x1b[46m@ŠD@\x1b[47m@@@\x1b[40m™\x1b[47m@†¤");
	printf("\x1b[11;20H†¤@\x1b[46m@Â@\x1b[47m@@@\x1b[44m@\x1b[47m@@@\x1b[46m@–¾Â\x1b[47m@@@\x1b[44m@\x1b[47m@†¤");
	printf("\x1b[12;20H†¤@\x1b[46m@Ô@\x1b[47m@@@\x1b[41m@\x1b[47m@@@\x1b[46m@–¾Ô\x1b[47m@@@\x1b[41m@\x1b[47m@†¤");
	printf("\x1b[13;20H†¤@\x1b[46m@Ž‡@\x1b[47m@@@\x1b[45m@\x1b[47m@@@\x1b[46m@–¾Ž‡\x1b[47m@@@\x1b[45m@\x1b[47m@†¤");
	printf("\x1b[14;20H†¤@\x1b[46m@—Î@\x1b[47m@@@\x1b[42m@\x1b[47m@@@\x1b[46m@–¾—Î\x1b[47m@@@\x1b[42m@\x1b[47m@†¤");
	printf("\x1b[15;20H†¤@\x1b[46m@…@\x1b[47m@@@\x1b[46m@\x1b[47m@@@\x1b[46m@–¾…\x1b[47m@@@\x1b[46m@\x1b[47m@†¤");
	printf("\x1b[16;20H†¤@\x1b[46m@‰©@\x1b[47m@@@\x1b[43m@\x1b[47m@@@\x1b[46m@–¾‰©\x1b[47m@@@\x1b[43m@\x1b[47m@†¤");
	printf("\x1b[17;20H†¤@@@@@@@@@@@\x1b[46m@”’@\x1b[47m@@@@@†¤");
	printf("\x1b[18;20H†¤@@@@@@@@@@@@@@@@@@@†¤");
	printf("\x1b[19;20H†¶†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†º\x1b[m");

	for(i=0;i<16;i++)
	{
		x1[i] = sub_col_x[i*2+0];
		x2[i] = sub_col_x[i*2+1];
		y1[i] = sub_col_y[i*2+0];
		y2[i] = sub_col_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,16);

	reset_csr_sw();
	for(i=6;i<=19;i++) printf("\x1b[%d;20H%44s",i," ");

	if(ch >= 1 && ch <= 7) *col = ch-1;
	else if(ch >= 8 && ch <= 15) *col = ch;

	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void disp_Copyright(void)
{
	static int i;
	printf("\x1b[8;23H\x1b[47m†±†£†£†£†£†£†£†£†£†£†£†£†£†£†£†£†µ");
	printf("\x1b[9;23H†¥@@@@@@@@@@@@@@@†¥");
	printf("\x1b[10;23H†¥@@  \x1b[43mŠÈˆÕ¸Þ×Ì¨¯¸´ÃÞ¨À°\x1b[47m@@@ †¥");
	printf("\x1b[11;23H†¥@(C) Hirokazu Inoue 1990,91  †¥");
	printf("\x1b[12;23H†¥@@@@Version 2.10.1@@@@†¥");
	printf("\x1b[13;23H†¥@@@@@@@@@@@@@@@†¥");
	printf("\x1b[14;23H†¥@@@@@ \x1b[46m@—¹‰ð@\x1b[47m@@@@@ †¥");
	printf("\x1b[15;23H†¥@@@@@@@@@@@@@@@†¥");
	printf("\x1b[16;23H†¹†£†£†£†£†£†£†£†£†£†£†£†£†£†£†£†½\x1b[m");

	x1[0] = Copyright_x[0];
	x2[0] = Copyright_x[1];
	y1[0] = Copyright_y[0];
	y2[0] = Copyright_y[1];

	mus_scrpoint(x1,y1,x2,y2,1);

	reset_csr_sw();
	for(i=8;i<=16;i++) printf("\x1b[%d;23H%35s",i," ");

	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void quit_ArtWriter(void)
{
	static int i,ch;

	printf("\x1b[8;23H\x1b[47m†±†£†£†£†£†£†£†£†£†£†£†£†£†£†£†£†µ");
	printf("\x1b[9;23H†¥ŠÈˆÕ¸Þ×Ì¨¯¸´ÃÞ¨À°‚ðI—¹‚µ‚Ü‚·.†¥");
	printf("\x1b[10;23H†¥@@@@@@@@@@@@@@@†¥");
	printf("\x1b[11;23H†¥@\x1b[46m@—¹‰ð@\x1b[47m@@\x1b[46m@ƒLƒƒƒ“ƒZƒ‹@\x1b[47m@†¥");
	printf("\x1b[12;23H†¥@@@@@@@@@@@@@@@†¥");
	printf("\x1b[13;23H†¹†£†£†£†£†£†£†£†£†£†£†£†£†£†£†£†½\x1b[m");

	for(i=0;i<2;i++)
	{
		x1[i] = quit_AW_x[i*2+0];
		x2[i] = quit_AW_x[i*2+1];
		y1[i] = quit_AW_y[i*2+0];
		y2[i] = quit_AW_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,2);

	if(ch == 1)
	{
		/* Quit */
		r.r_ax = 0x02;		// cursor OFF
		intr(0x33,&r);
		PC98_BGI_16color_palette_init();
		key_buf_cls();
		printf("\x1b[2J");	// return to XYGRAPH.EXE
		exit(0);
	}

	reset_csr_sw();
	for(i=8;i<=13;i++) printf("\x1b[%d;23H%35s",i," ");
	rev_sw = 0;	/* resume mouse reverse */
	csr_sw = 0;

	return;
}

void write_char(void)
{
	int m_x,m_y;

	if(!input_str()) return;

	PC98_BGI_16color_palette_init();
	_AX = 0x01;		// cursor ON
	geninterrupt(0x33);

	for(;;)
	{
		if(kbhit() && getch() == 0x1b) return;	// ESC


		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);
		m_x = r.r_cx;		// 0 - 639
		m_y = r.r_dx;		// 0 - 399
		// only in window
		mouse_locate_boarder(0,0,631,384,m_x,m_y);
		if(r.r_bx & 0x01)	// if LIGHT BUTTON
		{
			until_mouse_wait();
			r.r_ax = 0x02;		// cursor OFF
			intr(0x33,&r);
			gsetpos(m_x,m_y);
			gcolor(fore_color,back_color,2);
			gputs(char_buf);
			r.r_ax = 0x01;		// cursor ON
			intr(0x33,&r);
			LowVideo_palette();
			return;
		}
	}

	LowVideo_palette();
	return;
}

int input_str(void)
{
	static int i,ch;

	printf("\x1b[47m"
		  "\x1b[5;10H†®†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†²"
		  "\x1b[6;10H†¤@•¶ŽšF•¶Žš—ñ“ü—Í@@@@@@@@@@@@@@@@@@@@†¤"
		  "\x1b[7;10H†¤@@@@@@@@@@@@@@@@@@@@@@@@@@@@@†¤"
		  "\x1b[8;10H†¤@@@@@@@@@@@@@@@@@@@@@@@@@@@@@†¤"
		  "\x1b[9;10H†¤ \x1b[m @@@@@@@@@@@@@@@@@@@@@@@@@@@ \x1b[47m †¤"
		  "\x1b[10;10H†¤@@@@@@@@@@@@@@@@@@@@@@@@@@@@@†¤"
		  "\x1b[11;10H†¤@@@@@@@@@@@@@@@@@@@@@@@@@@@@@†¤"
		  "\x1b[12;10H†¤@@@ØÀ°Ý ·° (ENTETR) ‚ÅŒˆ’èAESC ·° ‚Å’†’f@@@@@@@†¤"
		  "\x1b[13;10H†¶†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†º"
		  "\x1b[m");

	key_buf_cls();
	_txt_jinp_bptone = 1500;
	txt_jinput(char_buf,56,"",GREEN,13,9);
	printf("\x1b[9;13H\x1b[m%56s\x1b[9;13H\x1b[21m%s\x1b[m","",char_buf);
	printf("\x1b[12;13H\x1b[47m%56s\x1b[m","");
	printf("\x1b[11;12H\x1b[47m@@@@@@\x1b[46m@—¹‰ð@\x1b[47m@@@@@\x1b[46m@ƒLƒƒƒ“ƒZƒ‹@\x1b[47m@@@@@@@\x1b[m");
	key_buf_cls();

	for(i=0;i<2;i++)
	{
		x1[i] = input_str_x[i*2+0];
		x2[i] = input_str_x[i*2+1];
		y1[i] = input_str_y[i*2+0];
		y2[i] = input_str_y[i*2+1];
	}

	ch = mus_scrpoint(x1,y1,x2,y2,2);

	reset_csr_sw();
	for(i=5;i<=13;i++) printf("\x1b[%d;10H%62s",i," ");
	if(ch == 2) return 0;		// cansel

	return 1;


}

void until_mouse_wait(void)
{
	do
	{
		r.r_ax = 0x03;		// read x/y & button
		intr(0x33,&r);

	}while(r.r_bx & 0x01);
}

void LowVideo_palette(void)
{
	color_palette_16(0 ,0 ,0 ,0 );
	color_palette_16(1 ,0 ,0 ,6 );
	color_palette_16(2 ,0 ,6 ,0 );
	color_palette_16(3 ,0 ,6 ,6 );
	color_palette_16(4 ,6 ,0 ,0 );
	color_palette_16(5 ,6 ,0 ,6 );
	color_palette_16(6 ,6 ,6 ,0 );
	color_palette_16(7 ,6 ,6 ,6 );
	color_palette_16(8 ,2 ,2 ,2 );
	color_palette_16(9 ,2 ,2 ,6 );
	color_palette_16(10,2 ,6 ,2 );
	color_palette_16(11,2 ,6 ,6 );
	color_palette_16(12,6 ,2 ,2 );
	color_palette_16(13,6 ,2 ,6 );
	color_palette_16(14,6 ,6 ,2 );
	color_palette_16(15,6 ,6 ,6 );

}