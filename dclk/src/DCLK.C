/* ���v�\���v���O���� DCLK ver 1.0
	(C) 1993 ��� ���v
*/
#pragma inline	/* �C�����C���A�Z���u���R�[�h�L�� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <glio.h>
#include <mem.h>
#include <time.h>
#include <signal.h>
#include <etc.h>

#define	FONTS	11

void disp_time_t(char *tbuf, int line, int seg);
void disp_time_hms(int h, int m, int s, int line, int seg);
void disp_time_s(long sec, int line, int seg);
void disp_font(int no, int x, int y, unsigned int seg);
int get_time(char *tbuf);

void dsp_watch1(void);
void dsp_watch_i1(void);
void dsp_watch_i2(void);
void dsp_watch2(char *buf);
void dsp_sto_n(void);
void dsp_sto_nnn(void);
void dsp_timer_n(char *buf);
void dsp_timer_nn(char *buf);
void dsp_timer_dnn(char *buf);

void do_esc_menu(void);
void do_menu2(void);
void do_menu3(void);
void do_menu4(void);
void do_menu5(void);
int do_menu(char *mstr[], int no);
void wrt_topic(char *mstr[], int tp, int no);
void set_palette(int no, char *buf);
char *inp_str(char *mes);
void wait_kbhit(void);
void alarm(void);
void end(void);
void about(void);
void help(void);

char *mstr1[] = {"�W������ ",
				 " �Q����  ",
				 "�į�߳���",
				 "�^�C�}�[ ",
				 " �װ�ݒ� ",
				 " About.. ",
				 "   �I��  "};		/* ���C�����j���[ */
char *mstr2[] = {"   ���b�o��   ",
				 "�����Ԃ������b�o��",
				 "  Ҳ��ƭ� ��  "};		/* �į�߳��� */
char *mstr3[] = {"   ���Ƃ��b   ",
				 "���Ƃ����Ԃ���",
				 "   ��������   ",
				 "  Ҳ��ƭ� ��  "};	/* �^�C�}�[ */
char *mstr4[] = {" �W������  ",
				 " �į�߳��� ",
				 " �^�C�}�[  ",
				 "  �w�i�F   ",
				 "Ҳ��ƭ� �� "};	/* �^�C�}�[ */
char *mstr5[] = {"    �ʏ�Ӱ��   ",
				 "���Ȱ��� Ӱ�� 1",
				 "���Ȱ��� Ӱ�� 2",
				 "  Ҳ��ƭ� ��   "};		/* �W���������[�h */

extern char fntdat[];
char scrsave;

#define	_TIME	0
#define	_IRM1	1
#define	_IRM2	2
#define	_SWN	3
#define	_SWNN	4
#define	_TMRN	5
#define	_TMRNN	6
#define	_TMRD	7
#define	_DUAL	8

void _getenvp(void){ }

void main(int ac, char *av[])
{
	int i, cmd;
	char buf[15];

	fputs("\x1b[>5h\x1b[>1h\x1b[2J", stderr);
	_AH = 0x40;
	geninterrupt(0x18);		/* �O���t�B�b�N�X�\���J�n */
	_CH = 0xc0;
	_AH = 0x42;
	geninterrupt(0x18);		/* �O���t�B�b�N�X 400 ���C�� */
	PC98_16color(1);		/* �O���t�B�b�N�X 16 �F */
	asm{
		mov al,0			/* �\���o���N�O */
		out 0a4h, al
		jmp $+2
		jmp $+2
		jmp $+2
		mov al,0			/* �`��o���N�O */
		out 0a6h, al
	}

	PC98_grph_cls();
	color_palette_16(0, 0,0,0);			/* �w�i */
	color_palette_16(1, 0xd,0xd,0xd);	/* ���v */
	color_palette_16(2, 0xb,0x0,0xe);	/* �į�߳��� */
	color_palette_16(4, 0x6,0xf,0x3);	/* ��ϰ */

	scrsave = 0;	/* ESC �̂݃`�F�b�N */
	cmd = _TIME;	/* �W�������\�� */
	for(i=1; i<ac; i++)
	{
		if(av[i][0] != '-') help();	/* �����܂����� */
		switch(av[i][1])
		{
			case 's'	:	/* �į�߳��� */
					if(av[i][2] == 'n') cmd = _SWN;
					else cmd = _SWNN;
					break;
			case 't'	:	/* ��ϰ */
					switch(av[i][2])
					{
						case 'n'	:
								strncpy(buf, av[i]+3, 10);
								cmd = _TMRN;
								break;
						case 'h'	:
								strncpy(buf, av[i]+3, 10);
								cmd = _TMRNN;
								break;
						default		:
								strncpy(buf, av[i]+2, 10);
								cmd = _TMRD;
								break;
					}
					break;
			case 'd'	:	/* 2 ���� */
					strncpy(buf, av[i]+2, 10);
					cmd = _DUAL;
					break;
			case 'i'	:	/* ���Ȱ��� */
					if(av[i][2] == '1') cmd = _IRM1;
					else if(av[i][2] == '2') cmd = _IRM2;
					else help();
					break;
			case 'k'	:	/* ESC �ȊO�`�F�b�N */
					scrsave = 1;
					break;
			case 'c'	:	/* �J���[�Z�b�g */
					switch(av[i][2])
					{
						case 's'	:
								strncpy(buf, av[i]+3, 10);
								set_palette(2, buf);
								break;
						case 't'	:
								strncpy(buf, av[i]+3, 10);
								set_palette(4, buf);
								break;
						case 'b'	:
								strncpy(buf, av[i]+3, 10);
								set_palette(0, buf);
								break;
						default		:
								strncpy(buf, av[i]+2, 10);
								set_palette(1, buf);
								break;
					}
					break;
			default		:
					help();
		}
	}

	signal(SIGINT, SIG_IGN);
	signal(SIGABRT, SIG_IGN);

	switch(cmd)
	{
		case _TIME	:
			dsp_watch1();	/* �W������ */
			break;
		case _IRM1	:
			dsp_watch_i1();
			break;
		case _IRM2	:
			dsp_watch_i2();
			break;
		case _SWN	:
			dsp_sto_n();
			break;
		case _SWNN	:
			dsp_sto_nnn();
			break;
		case _TMRN	:
			dsp_timer_n(buf);
			break;
		case _TMRNN	:
			dsp_timer_nn(buf);
			break;
		case _TMRD	:
			dsp_timer_dnn(buf);
			break;
		case _DUAL	:
			dsp_watch2(buf);
			break;
	}
	if(!scrsave) do_esc_menu();	/* ���j���[ �������[�v */

	end();	/* scrsave ���̂ݓ��B */
}

/* �����b�@�\�� */
void disp_time_t(char *tbuf, int line, int seg)
{
	disp_font((tbuf[3] & 0xf0) >> 4	,  0, line, seg);	/* �� */
	disp_font(tbuf[3] & 0x0f		, 10, line, seg);
	disp_font(10					, 20, line, seg);	/* : */
	disp_font((tbuf[4] & 0xf0) >> 4	, 30, line, seg);	/* �� */
	disp_font(tbuf[4] & 0x0f		, 40, line, seg);
	disp_font(10					, 50, line, seg);	/* : */
	disp_font((tbuf[5] & 0xf0) >> 4	, 60, line, seg);	/* �b */
	disp_font(tbuf[5] & 0x0f		, 70, line, seg);

	return ;
}

/* �����b�@�\�� */
void disp_time_hms(int h, int m, int s, int line, int seg)
{
	disp_font(h / 10,  0, line, seg);	/* �� */
	disp_font(h % 10, 10, line, seg);
	disp_font(10	, 20, line, seg);	/* : */
	disp_font(m / 10, 30, line, seg);	/* �� */
	disp_font(m % 10, 40, line, seg);
	disp_font(10	, 50, line, seg);	/* : */
	disp_font(s / 10, 60, line, seg);	/* �b */
	disp_font(s % 10, 70, line, seg);

	return ;
}

/* �b�\�� */
void disp_time_s(long sec, int line, int seg)
{
	disp_font(0										,  0, line, seg);	/* �疜�b */
	disp_font((int)((sec % 10000000L) / 1000000L)	, 10, line, seg);
	disp_font((int)((sec % 1000000L) / 100000L)		, 20, line, seg);
	disp_font((int)((sec % 100000L) / 10000)		, 30, line, seg);
	disp_font((int)((sec % 10000L) / 1000)			, 40, line, seg);
	disp_font((int)((sec % 1000) / 100)				, 50, line, seg);
	disp_font((int)((sec % 100) / 10)				, 60, line, seg);
	disp_font((int)((sec % 10))						, 70, line, seg);	/* �P�b */

	return ;
}

/* �t�H���g�\���@�P������ */
void disp_font(int no, int x, int y, unsigned int seg)
{
/* x : X ���W / 8 , 0 �` 79 */
/* y : Y ���W */
	static int i;

	for(i=0; i<150; i++)
	{
		movedata(_DS, FP_OFF(fntdat) + i*10 + 150*10*no,
				seg, (y+i)*80 + x, sizeof(char)*10);
	}

	return ;
}

/* �������擾���� �P�b�܂� */
int get_time(char *tbuf)
{
	static unsigned int i;
	static char previous;	/* �b�ۑ� */

	i = _DS;
	_ES = i;
	_BX = FP_OFF(tbuf);
	_AH = 0;
	geninterrupt(0x1c);		/* �����̎擾 BIOS */

	previous = tbuf[5];	/* ���݂̕b��ۑ� */

	for(;;)
	{
		i = _DS;
		_ES = i;
		_BX = FP_OFF(tbuf);
		_AH = 0;
		geninterrupt(0x1c);		/* �����̎擾 BIOS */

		if(previous == tbuf[5])
		{	/* �b���ς���Ă��Ȃ��Ƃ� */
			delay(100);		/* 100 msec �̋x�~ */

			_AH = 1;
			geninterrupt(0x18);	/* �L�[�o�b�t�@�̃Z���X */
			i = _BH;
			if(i)
			{	/* �L�[��������Ă����� */
				_AH = 0;
				geninterrupt(0x18);	/* �L�[�f�[�^�̓ǂ݂��� */
				i = _AH;
				if(i == 0) return 0x1b;	/* ESC �L�[�������� */
				if(scrsave) return 1;	/* ��ذݾ��� Ӱ�� */
			}
		}
		else break;		/* �P�b�i�񂾂� */
	}
	return 0;
}

/* �W������ */
void dsp_watch1(void)
{
	static char tbuf[10];

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */
		disp_time_t(tbuf, 125, 0xa800);
	}
}

/* �W������ �C���~�l�[�V�����P */
void dsp_watch_i1(void)
{
	static char tbuf[10];
	int i, j, r, g, b, rf=1, gf=1, bf=1, rk, gk, bk;
	int ud[] = {0, 8};

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	color_palette_16(0, 0,0,0);
	rk = 0;
	bk = 1;
	gk = 0;
	randomize();
	r = rand() % 16;
	g = rand() % 16;
	b = rand() % 16;
	for(i=0,j=0;;i^=1,j++)
	{
		if(j >= 60)
		{
			j = 0;
			rk ^= 1;
			gk ^= 1;
			bk ^= 1;
		}
		if(r>=15) rf = -1;
		if(r<=ud[rk] ) rf = 1;
		if(g>=15) gf = -1;
		if(g<=ud[gk] ) gf = 1;
		if(b>=15) bf = -1;
		if(b<=ud[bk] ) bf = 1;
		if(!bk) r += rf;
		else if(i) r += rf;
		if(!rk) g += gf;
		else if(i) g += gf;
		if(!gk) b += bf;
		else if(i) b += bf;
		color_palette_16(1, g, r, b);
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */
		disp_time_t(tbuf, 125, 0xa800);
	}
	color_palette_16(1, 0xd,0xd,0xd);
	color_palette_16(0, 0,0,0);
}

/* �W������ �C���~�l�[�V�����Q */
void dsp_watch_i2(void)
{
	static char tbuf[10];
	int i, j, r, g, b, rf=1, gf=1, bf=1, rk, gk, bk;
	int ud[] = {0, 8};

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	color_palette_16(1, 0,0,0);
	rk = 0;
	bk = 0;
	gk = 1;
	randomize();
	r = rand() % 16;
	g = rand() % 16;
	b = rand() % 16;
	for(i=0,j=0;;i^=1,j++)
	{
		if(j >= 60)
		{
			j = 0;
			rk ^= 1;
			gk ^= 1;
			bk ^= 1;
		}
		if(r>=15) rf = -1;
		if(r<=ud[rk] ) rf = 1;
		if(g>=15) gf = -1;
		if(g<=ud[gk] ) gf = 1;
		if(b>=15) bf = -1;
		if(b<=ud[bk] ) bf = 1;
		if(!bk) r += rf;
		else if(i) r += rf;
		if(!rk) g += gf;
		else if(i) g += gf;
		if(!gk) b += bf;
		else if(i) b += bf;
		color_palette_16(0, g, r, b);
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */
		disp_time_t(tbuf, 125, 0xa800);
	}
	color_palette_16(1, 0xd,0xd,0xd);
	color_palette_16(0, 0,0,0);
}

/* �Q���� */
void dsp_watch2(char *buf)
{
	int dh, dm, h, m, s;
	char tbuf[10], buf2[10];

	/* �������̉�� */
	strcpy(buf2, strtok(buf, ":"));
	dh = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	dm = atoi(buf2);
	if(dm < 0) return;		/* ���ɂ͕����͂Ȃ� */
	if(dh < 0) dm = -dm;	/* �擪�̃}�C�i�X�͕��ɂ������� */
	if(dm >= 60 || dm <= -60) return;
	if(dh >= 24 || dh <= -24) return;

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */

		/* ��P���� */
		disp_time_t(tbuf, 30, 0xa800);

		/* ��Q���� */
		h = ((tbuf[3] & 0xf0) >> 4)*10 + (tbuf[3] & 0x0f);
		m = ((tbuf[4] & 0xf0) >> 4)*10 + (tbuf[4] & 0x0f);
		s = ((tbuf[5] & 0xf0) >> 4)*10 + (tbuf[5] & 0x0f);
		m += dm;	/* ���̒��� */
		if(m < 0)
		{	/* �O������ */
			m += 60;
			h--;	/* �P���Ԃ��ǂ� */
		}
		else if(m >= 60)
		{	/* �U�O���������� */
			m -= 60;
			h++;	/* �P���Ԃӂ₷ */
		}
		h += dh;	/* ���̒��� */
		if(h < 0)
		{	/* �O������ */
			h += 24;
		}
		else if(h >= 24)
		{	/* �Q�S���������� */
			h -= 24;
		}
		disp_time_hms(h, m, s, 220, 0xb000);
	}
}

/* ���b�o�� */
void dsp_sto_n(void)
{
	unsigned long now=0L;
	char tbuf[10];

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */

		/* ��P���� */
		disp_time_t(tbuf, 30, 0xa800);

		/* �o�ߕb */
		disp_time_s(now, 220, 0xb000);

		now++;
	}
}

/* ��:��:���o�� */
void dsp_sto_nnn(void)
{
	int h, m, s;
	unsigned long now=0L;
	char tbuf[10];

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */

		/* ��P���� */
		disp_time_t(tbuf, 30, 0xa800);

		h = (int)((now/3600L) % 100);	/* 0 - 99 ���� */
		m = (int)((now % 3600) / 60);
		s = (int)(now % 60);

		/* �o�ߎ��� */
		disp_time_hms(h, m, s, 220, 0xb000);

		now++;
	}
}

/* ���Z�^�C�}�[�� */
void dsp_timer_n(char *buf)
{
	long timer;
	char tbuf[10];

	timer = atol(buf);
	if(timer < 0) return;	/* �O�b�ȉ��͂��肦�Ȃ� */

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */
		/* ��P���� */
		disp_time_t(tbuf, 30, 0xa800);
		/* �^�C�}�[ */
		disp_time_s(timer, 220, 0xb800);
		timer--;
		if(timer < 0)
		{	/* �������B */
			alarm();
			wait_kbhit();
			break;
		}
	}
	return ;
}

/* ���Z�^�C�}�[ ��:�� */
void dsp_timer_nn(char *buf)
{
	int dh, dm, h, m, s;
	long timer;
	char tbuf[10], buf2[10];

	strcpy(buf2, strtok(buf, ":"));
	dh = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	dm = atoi(buf2);

	if(dh < 0 || dm < 0 || dh >= 100) return;	/* �O�ȉ��͂��肦�Ȃ� */

	timer = dh*3600L + dm*60L;

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */
		/* ��P���� */
		disp_time_t(tbuf, 30, 0xa800);
		/* �^�C�}�[ */
		h = (int)((timer/3600L) % 100);	/* 0 - 99 ���� */
		m = (int)((timer % 3600) / 60);
		s = (int)(timer % 60);
		disp_time_hms(h, m, s, 220, 0xb800);
		timer--;
		if(timer < 0)
		{	/* �������B */
			alarm();
			wait_kbhit();
			break;
		}
	}
	return ;
}

/* �^�C�}�[ �������� */
void dsp_timer_dnn(char *buf)
{
	int dh, dm, h, m, s;
	long timer, now;
	char tbuf[10], buf2[10];

	strcpy(buf2, strtok(buf, ":"));
	dh = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	dm = atoi(buf2);

	if(dh < 0 || dm < 0 || dh >= 24 || dm >= 60) return;

	timer = dh*3600L + dm*60L;

	fputs("\x1b[2J", stderr);	/* ��ʏ��� */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC �L�[�ŒE�o */
		/* ��P���� */
		disp_time_t(tbuf, 30, 0xa800);
		/* �^�C�}�[ */
		h = ((tbuf[3] & 0xf0) >> 4)*10 + (tbuf[3] & 0x0f);
		m = ((tbuf[4] & 0xf0) >> 4)*10 + (tbuf[4] & 0x0f);
		s = ((tbuf[5] & 0xf0) >> 4)*10 + (tbuf[5] & 0x0f);
		now = h*3600L + m*60L + s;	/* ���� */
		now = timer - now;	/* ���Ƃ��b */
		if(now < 0) now += (24L*3600L);	/* �O�ȉ��͕␳ */
		h = (int)((now/3600L) % 100);	/* 0 - 99 ���� */
		m = (int)((now % 3600) / 60);
		s = (int)(now % 60);
		disp_time_hms(h, m, s, 220, 0xb800);
		if(now <= 0)
		{	/* �������B */
			alarm();
			wait_kbhit();
			break;
		}
	}
	return ;
}

/* ���C�����j���[����� */
void do_esc_menu(void)
{
	static int i;

	for(;;)
	{
		fputs("\x1b[m\x1b[>5h\x1b[2J", stderr);	/* ��ʏ��� */
		fputs("\x1b[23;1H", stderr);	/* goto 23 line */
		for(i=0; i<40; i++) fputs("��", stderr);	/* write line */

		switch(do_menu(mstr1, 6))	/* ���j���[�I�� */
		{
			case 0 :	do_menu5();				/* �W������ */
						break;
			case 1 :								/* �Q���� */
				dsp_watch2(inp_str("���{�����Ƃ̍� [-]hh:mm : "));
						break;
			case 2 :	do_menu2();					/* �į�߳��� */
						break;
			case 3 :	do_menu3();					/* ��ϰ */
						break;
			case 4 :	do_menu4();								/* �װ�ݒ� */
						break;
			case 5 :	about();					/* About */
						break;
			case 6 :	end();						/* �I�� */
						break;
			default :	break;
		}
	}
}

/* �į�߳��� ���j���[ */
void do_menu2(void)
{

	for(;;)
	{
		switch(do_menu(mstr2, 2))	/* ���j���[�I�� */
		{
			case 0 :	dsp_sto_n();	/* ���b�o�� */
						return;
			case 1 :	dsp_sto_nnn();	/* ���F���F���o�� */
						return;
			case 2 :	return;		/* ���C�����j���[�� */
			default :	break;
		}
	}
}

/* �^�C�}�[ ���j���[ */
void do_menu3(void)
{

	for(;;)
	{
		switch(do_menu(mstr3, 3))	/* ���j���[�I�� */
		{
			case 0 :				/* ���Ƃ��b */
						dsp_timer_n(inp_str("�^�C�}�[�͉��b�ł��� : "));
						return;
			case 1 :				/* ���Ƃ��F�� */
						dsp_timer_nn(inp_str("�^�C�}�[�͉����ԉ����ł���  hh:mm : "));
						return;
			case 2 :				/* ���F�� �܂�*/
						dsp_timer_dnn(inp_str("���������܂łł���  hh:mm : "));
						return;
			case 3 :	return;		/* ���C�����j���[�� */
			default :	break;
		}
	}
}

/* �\���F ���j���[ */
void do_menu4(void)
{

	for(;;)
	{
		switch(do_menu(mstr4, 4))	/* ���j���[�I�� */
		{
			case 0 :				/* �W������ */
						set_palette(1, inp_str("���̨��� ��گ� rr:gg:bb : "));
						return;
			case 1 :				/* �į�߳��� */
						set_palette(2, inp_str("���̨��� ��گ� rr:gg:bb : "));
						return;
			case 2 :				/* �^�C�}�[ */
						set_palette(4, inp_str("���̨��� ��گ� rr:gg:bb : "));
						return;
			case 3 :				/* �w�i */
						set_palette(0, inp_str("���̨��� ��گ� rr:gg:bb : "));
						return;
			case 4 :	return;		/* ���C�����j���[�� */
			default :	break;
		}
	}
}

/* �W������ ���j���[ */
void do_menu5(void)
{

	for(;;)
	{
		switch(do_menu(mstr5, 3))	/* ���j���[�I�� */
		{
			case 0 :				/* �ʏ탂�[�h */
						dsp_watch1();
						return;
			case 1 :				/* �C���~�l�[�V�����P */
						dsp_watch_i1();
						return;
			case 2 :				/* �C���~�l�[�V�����Q */
						dsp_watch_i2();
						return;
			case 3 :	return;		/* ���C�����j���[�� */
			default :	break;
		}
	}
}


/* ���ׂẴ��j���[�ł̑I�� */
int do_menu(char *mstr[], int no)
{
	static int tp, key;

	wrt_topic(mstr, 0, no);

	for(tp = 0;;)
	{
		_AH = 0;
		geninterrupt(0x18);	/* �L�[�f�[�^�̓ǂ݂��� */
		key = _AH;
		switch(key)
		{
			case 0x1c :
						break;
			case 0x3c :
						tp++;
						if(tp > no) tp = 0;
						wrt_topic(mstr, tp, no);
						break;
			case 0x3b :
						tp--;
						if(tp < 0) tp = no;
						wrt_topic(mstr, tp, no);
						break;
			default :	fputs("\x07", stderr);	/* �r�[�v�� */
		}
		if(key == 0x1c) return tp;
	}
}

/* ���j���[���ڕ\�� */
void wrt_topic(char *mstr[], int tp, int no)
{
	static int i;

	fputs("\x1b[24;1H\x1b[0K", stderr);	/* �Q�S�s�ڂ̃N���A */
	for(i=0; i<=no; i++)
	{
		if(i == tp) fputs("\x1b[47m", stderr);
		fputs(mstr[i], stderr);
		if(i == tp) fputs("\x1b[m", stderr);
		fputs("  ", stderr);
	}
	return ;
}

/* �O���t�B�b�N�X�p���b�g�ݒ� */
void set_palette(int no, char *buf)
{
	int r, g, b;
	char buf2[10];

	strcpy(buf2, strtok(buf, ":"));
	r = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	g = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	b = atoi(buf2);

	if(r < 0 || g < 0 || b < 0 ||
		r > 0xf || g > 0xf || b > 0xf) return;
	color_palette_16(no, g, r, b);
	return ;
}

/* ������̓��� */
char *inp_str(char *mes)
{
	static char buf[40];
	fputs("\x1b[24;1H\x1b[0K", stderr);	/* �Q�S�s�ڂ̃N���A */
	fputs("\x1b[>5l", stderr);			/* �J�[�\�� ON */
	fputs(mes, stderr);
	fgets(buf, 20, stdin);
	fputs("\x1b[>5h", stderr);			/* �J�[�\�� OFF */

	return buf;
}

/* �L�[�����͂����̂�҂� */
void wait_kbhit(void)
{
	_AH = 0;
	geninterrupt(0x18);	/* �L�[�f�[�^�̓ǂ݂��� */
	return ;
}

/* �^�C�}�[�̃A���[�� */
void alarm(void)
{
	int i;

	for(i=0; i<5; i++)
	{
		sound(1500);
		delay(800);
		nosound();
		delay(300);
	}
	sound(2000);
	nosound();

	return ;
}

/* �I�� */
void end(void)
{
	signal(SIGABRT, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	PC98_grph_cls();
	PC98_16color_palette_init();
	_AH = 0x41;
	geninterrupt(0x18);	/* �O���t�B�b�N�X�\����~ */

	fputs("\x1b[>5l\x1b[>1l\x1b[2J", stderr);	/* �J�[�\�� ON & �N���A */
	exit(0);
}

/* About */
void about(void)
{
	fputs("\x1b[24;1H\x1b[0K", stderr);	/* �Q�S�s�ڂ̃N���A */
	fputs(" DCLK	:	\x1b[20mDigital CLocK\x1b[m  Ver 1.0		(C) 1993 Hirokazu Inoue", stderr);
	wait_kbhit();
	return ;
}

void help(void)
{
	_AH = 0x41;
	geninterrupt(0x18);	/* �O���t�B�b�N�X�\����~ */
	fputs("\x1b[2J"
			"DCLK	:	Digital CLocK  Ver 1.0		(C) 1993 Hirokazu Inoue\n"
			"�g�p�@\n"
			"\n"
			"DCLK -[c/cb/cs/ct/k/d/i1/i2/s/sn/t/th/tn][\x1b[4m#/##:##/##:##:##\x1b[m]\n"
			"\n"
			"-c\x1b[4mrr:gg:bb\x1b[m          : �W�� �װ��گ�\n"
			"-cb\x1b[4mrr:gg:bb\x1b[m         : �ޯ���׳��� �װ��گ�\n"
			"-cs\x1b[4mrr:gg:bb\x1b[m         : �į�߳��� �װ��گ�\n"
			"-ct\x1b[4mrr:gg:bb\x1b[m         : ���Z��ϰ �װ��گ�\n"
			"-k                  : ��ذݾ��� Ӱ��\n"
			"-d\x1b[4m-hh:mm\x1b[m            : ��Q����\n"
			"-i1                 : ���Ȱ��� 1\n"
			"-i2                 : ���Ȱ��� 2\n"
			"-s                  : �į�߳���\n"
			"-sn                 : �į�߳��� �b�\��\n"
			"-t\x1b[4mhh:mm\x1b[m             : ��ϰ �޲ڸ� �w��\n"
			"-th\x1b[4mhh:mm\x1b[m            : ��ϰ\n"
			"-tn\x1b[4mnnnn\x1b[m             : ��ϰ �b�w��\n", stderr);
	wait_kbhit();
	end();
}
/* �t�@�C���G���h [EOF] */