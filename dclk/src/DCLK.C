/* 時計表示プログラム DCLK ver 1.0
	(C) 1993 井上 博計
*/
#pragma inline	/* インラインアセンブラコード有り */
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

char *mstr1[] = {"標準時刻 ",
				 " ２時刻  ",
				 "ｽﾄｯﾌﾟｳｵｯﾁ",
				 "タイマー ",
				 " ｶﾗｰ設定 ",
				 " About.. ",
				 "   終了  "};		/* メインメニュー */
char *mstr2[] = {"   ｎ秒経過   ",
				 "ｎ時間ｎ分ｎ秒経過",
				 "  ﾒｲﾝﾒﾆｭｰ へ  "};		/* ｽﾄｯﾌﾟｳｫｯﾁ */
char *mstr3[] = {"   あとｎ秒   ",
				 "あとｎ時間ｎ分",
				 "   ｎ時ｎ分   ",
				 "  ﾒｲﾝﾒﾆｭｰ へ  "};	/* タイマー */
char *mstr4[] = {" 標準時刻  ",
				 " ｽﾄｯﾌﾟｳｫｯﾁ ",
				 " タイマー  ",
				 "  背景色   ",
				 "ﾒｲﾝﾒﾆｭｰ へ "};	/* タイマー */
char *mstr5[] = {"    通常ﾓｰﾄﾞ   ",
				 "ｲﾙﾐﾈｰｼｮﾝ ﾓｰﾄﾞ 1",
				 "ｲﾙﾐﾈｰｼｮﾝ ﾓｰﾄﾞ 2",
				 "  ﾒｲﾝﾒﾆｭｰ へ   "};		/* 標準時刻モード */

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
	geninterrupt(0x18);		/* グラフィックス表示開始 */
	_CH = 0xc0;
	_AH = 0x42;
	geninterrupt(0x18);		/* グラフィックス 400 ライン */
	PC98_16color(1);		/* グラフィックス 16 色 */
	asm{
		mov al,0			/* 表示バンク０ */
		out 0a4h, al
		jmp $+2
		jmp $+2
		jmp $+2
		mov al,0			/* 描画バンク０ */
		out 0a6h, al
	}

	PC98_grph_cls();
	color_palette_16(0, 0,0,0);			/* 背景 */
	color_palette_16(1, 0xd,0xd,0xd);	/* 時計 */
	color_palette_16(2, 0xb,0x0,0xe);	/* ｽﾄｯﾌﾟｳｵｯﾁ */
	color_palette_16(4, 0x6,0xf,0x3);	/* ﾀｲﾏｰ */

	scrsave = 0;	/* ESC のみチェック */
	cmd = _TIME;	/* 標準時刻表示 */
	for(i=1; i<ac; i++)
	{
		if(av[i][0] != '-') help();	/* ｽｲｯﾁまちがい */
		switch(av[i][1])
		{
			case 's'	:	/* ｽﾄｯﾌﾟｳｫｯﾁ */
					if(av[i][2] == 'n') cmd = _SWN;
					else cmd = _SWNN;
					break;
			case 't'	:	/* ﾀｲﾏｰ */
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
			case 'd'	:	/* 2 時刻 */
					strncpy(buf, av[i]+2, 10);
					cmd = _DUAL;
					break;
			case 'i'	:	/* ｲﾙﾐﾈｰｼｮﾝ */
					if(av[i][2] == '1') cmd = _IRM1;
					else if(av[i][2] == '2') cmd = _IRM2;
					else help();
					break;
			case 'k'	:	/* ESC 以外チェック */
					scrsave = 1;
					break;
			case 'c'	:	/* カラーセット */
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
			dsp_watch1();	/* 標準時刻 */
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
	if(!scrsave) do_esc_menu();	/* メニュー 無限ループ */

	end();	/* scrsave 時のみ到達 */
}

/* 時分秒　表示 */
void disp_time_t(char *tbuf, int line, int seg)
{
	disp_font((tbuf[3] & 0xf0) >> 4	,  0, line, seg);	/* 時 */
	disp_font(tbuf[3] & 0x0f		, 10, line, seg);
	disp_font(10					, 20, line, seg);	/* : */
	disp_font((tbuf[4] & 0xf0) >> 4	, 30, line, seg);	/* 分 */
	disp_font(tbuf[4] & 0x0f		, 40, line, seg);
	disp_font(10					, 50, line, seg);	/* : */
	disp_font((tbuf[5] & 0xf0) >> 4	, 60, line, seg);	/* 秒 */
	disp_font(tbuf[5] & 0x0f		, 70, line, seg);

	return ;
}

/* 時分秒　表示 */
void disp_time_hms(int h, int m, int s, int line, int seg)
{
	disp_font(h / 10,  0, line, seg);	/* 時 */
	disp_font(h % 10, 10, line, seg);
	disp_font(10	, 20, line, seg);	/* : */
	disp_font(m / 10, 30, line, seg);	/* 分 */
	disp_font(m % 10, 40, line, seg);
	disp_font(10	, 50, line, seg);	/* : */
	disp_font(s / 10, 60, line, seg);	/* 秒 */
	disp_font(s % 10, 70, line, seg);

	return ;
}

/* 秒表示 */
void disp_time_s(long sec, int line, int seg)
{
	disp_font(0										,  0, line, seg);	/* 千万秒 */
	disp_font((int)((sec % 10000000L) / 1000000L)	, 10, line, seg);
	disp_font((int)((sec % 1000000L) / 100000L)		, 20, line, seg);
	disp_font((int)((sec % 100000L) / 10000)		, 30, line, seg);
	disp_font((int)((sec % 10000L) / 1000)			, 40, line, seg);
	disp_font((int)((sec % 1000) / 100)				, 50, line, seg);
	disp_font((int)((sec % 100) / 10)				, 60, line, seg);
	disp_font((int)((sec % 10))						, 70, line, seg);	/* １秒 */

	return ;
}

/* フォント表示　１文字分 */
void disp_font(int no, int x, int y, unsigned int seg)
{
/* x : X 座標 / 8 , 0 ～ 79 */
/* y : Y 座標 */
	static int i;

	for(i=0; i<150; i++)
	{
		movedata(_DS, FP_OFF(fntdat) + i*10 + 150*10*no,
				seg, (y+i)*80 + x, sizeof(char)*10);
	}

	return ;
}

/* 時刻を取得する １秒まつ */
int get_time(char *tbuf)
{
	static unsigned int i;
	static char previous;	/* 秒保存 */

	i = _DS;
	_ES = i;
	_BX = FP_OFF(tbuf);
	_AH = 0;
	geninterrupt(0x1c);		/* 時刻の取得 BIOS */

	previous = tbuf[5];	/* 現在の秒を保存 */

	for(;;)
	{
		i = _DS;
		_ES = i;
		_BX = FP_OFF(tbuf);
		_AH = 0;
		geninterrupt(0x1c);		/* 時刻の取得 BIOS */

		if(previous == tbuf[5])
		{	/* 秒が変わっていないとき */
			delay(100);		/* 100 msec の休止 */

			_AH = 1;
			geninterrupt(0x18);	/* キーバッファのセンス */
			i = _BH;
			if(i)
			{	/* キーが押されていたら */
				_AH = 0;
				geninterrupt(0x18);	/* キーデータの読みだし */
				i = _AH;
				if(i == 0) return 0x1b;	/* ESC キーだったら */
				if(scrsave) return 1;	/* ｽｸﾘｰﾝｾｰﾊﾞ ﾓｰﾄﾞ */
			}
		}
		else break;		/* １秒進んだら */
	}
	return 0;
}

/* 標準時刻 */
void dsp_watch1(void)
{
	static char tbuf[10];

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */
		disp_time_t(tbuf, 125, 0xa800);
	}
}

/* 標準時刻 イルミネーション１ */
void dsp_watch_i1(void)
{
	static char tbuf[10];
	int i, j, r, g, b, rf=1, gf=1, bf=1, rk, gk, bk;
	int ud[] = {0, 8};

	fputs("\x1b[2J", stderr);	/* 画面消去 */
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
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */
		disp_time_t(tbuf, 125, 0xa800);
	}
	color_palette_16(1, 0xd,0xd,0xd);
	color_palette_16(0, 0,0,0);
}

/* 標準時刻 イルミネーション２ */
void dsp_watch_i2(void)
{
	static char tbuf[10];
	int i, j, r, g, b, rf=1, gf=1, bf=1, rk, gk, bk;
	int ud[] = {0, 8};

	fputs("\x1b[2J", stderr);	/* 画面消去 */
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
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */
		disp_time_t(tbuf, 125, 0xa800);
	}
	color_palette_16(1, 0xd,0xd,0xd);
	color_palette_16(0, 0,0,0);
}

/* ２時刻 */
void dsp_watch2(char *buf)
{
	int dh, dm, h, m, s;
	char tbuf[10], buf2[10];

	/* 時刻差の解析 */
	strcpy(buf2, strtok(buf, ":"));
	dh = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	dm = atoi(buf2);
	if(dm < 0) return;		/* 分には符号はない */
	if(dh < 0) dm = -dm;	/* 先頭のマイナスは分にもかかる */
	if(dm >= 60 || dm <= -60) return;
	if(dh >= 24 || dh <= -24) return;

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */

		/* 第１時刻 */
		disp_time_t(tbuf, 30, 0xa800);

		/* 第２時刻 */
		h = ((tbuf[3] & 0xf0) >> 4)*10 + (tbuf[3] & 0x0f);
		m = ((tbuf[4] & 0xf0) >> 4)*10 + (tbuf[4] & 0x0f);
		s = ((tbuf[5] & 0xf0) >> 4)*10 + (tbuf[5] & 0x0f);
		m += dm;	/* 分の調整 */
		if(m < 0)
		{	/* ０分未満 */
			m += 60;
			h--;	/* １時間もどす */
		}
		else if(m >= 60)
		{	/* ６０分をこえる */
			m -= 60;
			h++;	/* １時間ふやす */
		}
		h += dh;	/* 時の調整 */
		if(h < 0)
		{	/* ０時未満 */
			h += 24;
		}
		else if(h >= 24)
		{	/* ２４時をこえる */
			h -= 24;
		}
		disp_time_hms(h, m, s, 220, 0xb000);
	}
}

/* ｎ秒経過 */
void dsp_sto_n(void)
{
	unsigned long now=0L;
	char tbuf[10];

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */

		/* 第１時刻 */
		disp_time_t(tbuf, 30, 0xa800);

		/* 経過秒 */
		disp_time_s(now, 220, 0xb000);

		now++;
	}
}

/* ｎ:ｎ:ｎ経過 */
void dsp_sto_nnn(void)
{
	int h, m, s;
	unsigned long now=0L;
	char tbuf[10];

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */

		/* 第１時刻 */
		disp_time_t(tbuf, 30, 0xa800);

		h = (int)((now/3600L) % 100);	/* 0 - 99 時間 */
		m = (int)((now % 3600) / 60);
		s = (int)(now % 60);

		/* 経過時間 */
		disp_time_hms(h, m, s, 220, 0xb000);

		now++;
	}
}

/* 減算タイマーｎ */
void dsp_timer_n(char *buf)
{
	long timer;
	char tbuf[10];

	timer = atol(buf);
	if(timer < 0) return;	/* ０秒以下はありえない */

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */
		/* 第１時刻 */
		disp_time_t(tbuf, 30, 0xa800);
		/* タイマー */
		disp_time_s(timer, 220, 0xb800);
		timer--;
		if(timer < 0)
		{	/* 時刻到達 */
			alarm();
			wait_kbhit();
			break;
		}
	}
	return ;
}

/* 減算タイマー ｎ:ｎ */
void dsp_timer_nn(char *buf)
{
	int dh, dm, h, m, s;
	long timer;
	char tbuf[10], buf2[10];

	strcpy(buf2, strtok(buf, ":"));
	dh = atoi(buf2);
	strcpy(buf2, strtok(NULL, ":"));
	dm = atoi(buf2);

	if(dh < 0 || dm < 0 || dh >= 100) return;	/* ０以下はありえない */

	timer = dh*3600L + dm*60L;

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */
		/* 第１時刻 */
		disp_time_t(tbuf, 30, 0xa800);
		/* タイマー */
		h = (int)((timer/3600L) % 100);	/* 0 - 99 時間 */
		m = (int)((timer % 3600) / 60);
		s = (int)(timer % 60);
		disp_time_hms(h, m, s, 220, 0xb800);
		timer--;
		if(timer < 0)
		{	/* 時刻到達 */
			alarm();
			wait_kbhit();
			break;
		}
	}
	return ;
}

/* タイマー ｎ時ｎ分 */
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

	fputs("\x1b[2J", stderr);	/* 画面消去 */
	PC98_grph_cls();
	for(;;)
	{
		if(get_time(tbuf)) break;	/* ESC キーで脱出 */
		/* 第１時刻 */
		disp_time_t(tbuf, 30, 0xa800);
		/* タイマー */
		h = ((tbuf[3] & 0xf0) >> 4)*10 + (tbuf[3] & 0x0f);
		m = ((tbuf[4] & 0xf0) >> 4)*10 + (tbuf[4] & 0x0f);
		s = ((tbuf[5] & 0xf0) >> 4)*10 + (tbuf[5] & 0x0f);
		now = h*3600L + m*60L + s;	/* 現在 */
		now = timer - now;	/* あとｎ秒 */
		if(now < 0) now += (24L*3600L);	/* ０以下は補正 */
		h = (int)((now/3600L) % 100);	/* 0 - 99 時間 */
		m = (int)((now % 3600) / 60);
		s = (int)(now % 60);
		disp_time_hms(h, m, s, 220, 0xb800);
		if(now <= 0)
		{	/* 時刻到達 */
			alarm();
			wait_kbhit();
			break;
		}
	}
	return ;
}

/* メインメニュー入り口 */
void do_esc_menu(void)
{
	static int i;

	for(;;)
	{
		fputs("\x1b[m\x1b[>5h\x1b[2J", stderr);	/* 画面消去 */
		fputs("\x1b[23;1H", stderr);	/* goto 23 line */
		for(i=0; i<40; i++) fputs("�｣", stderr);	/* write line */

		switch(do_menu(mstr1, 6))	/* メニュー選択 */
		{
			case 0 :	do_menu5();				/* 標準時刻 */
						break;
			case 1 :								/* ２時刻 */
				dsp_watch2(inp_str("日本時刻との差 [-]hh:mm : "));
						break;
			case 2 :	do_menu2();					/* ｽﾄｯﾌﾟｳｫｯﾁ */
						break;
			case 3 :	do_menu3();					/* ﾀｲﾏｰ */
						break;
			case 4 :	do_menu4();								/* ｶﾗｰ設定 */
						break;
			case 5 :	about();					/* About */
						break;
			case 6 :	end();						/* 終了 */
						break;
			default :	break;
		}
	}
}

/* ｽﾄｯﾌﾟｳｫｯﾁ メニュー */
void do_menu2(void)
{

	for(;;)
	{
		switch(do_menu(mstr2, 2))	/* メニュー選択 */
		{
			case 0 :	dsp_sto_n();	/* ｎ秒経過 */
						return;
			case 1 :	dsp_sto_nnn();	/* ｎ：ｎ：ｎ経過 */
						return;
			case 2 :	return;		/* メインメニューへ */
			default :	break;
		}
	}
}

/* タイマー メニュー */
void do_menu3(void)
{

	for(;;)
	{
		switch(do_menu(mstr3, 3))	/* メニュー選択 */
		{
			case 0 :				/* あとｎ秒 */
						dsp_timer_n(inp_str("タイマーは何秒ですか : "));
						return;
			case 1 :				/* あとｎ：ｎ */
						dsp_timer_nn(inp_str("タイマーは何時間何分ですか  hh:mm : "));
						return;
			case 2 :				/* ｎ：ｎ まで*/
						dsp_timer_dnn(inp_str("何時何分までですか  hh:mm : "));
						return;
			case 3 :	return;		/* メインメニューへ */
			default :	break;
		}
	}
}

/* 表示色 メニュー */
void do_menu4(void)
{

	for(;;)
	{
		switch(do_menu(mstr4, 4))	/* メニュー選択 */
		{
			case 0 :				/* 標準時刻 */
						set_palette(1, inp_str("ｸﾞﾗﾌｨｯｸｽ ﾊﾟﾚｯﾄ rr:gg:bb : "));
						return;
			case 1 :				/* ｽﾄｯﾌﾟｳｫｯﾁ */
						set_palette(2, inp_str("ｸﾞﾗﾌｨｯｸｽ ﾊﾟﾚｯﾄ rr:gg:bb : "));
						return;
			case 2 :				/* タイマー */
						set_palette(4, inp_str("ｸﾞﾗﾌｨｯｸｽ ﾊﾟﾚｯﾄ rr:gg:bb : "));
						return;
			case 3 :				/* 背景 */
						set_palette(0, inp_str("ｸﾞﾗﾌｨｯｸｽ ﾊﾟﾚｯﾄ rr:gg:bb : "));
						return;
			case 4 :	return;		/* メインメニューへ */
			default :	break;
		}
	}
}

/* 標準時刻 メニュー */
void do_menu5(void)
{

	for(;;)
	{
		switch(do_menu(mstr5, 3))	/* メニュー選択 */
		{
			case 0 :				/* 通常モード */
						dsp_watch1();
						return;
			case 1 :				/* イルミネーション１ */
						dsp_watch_i1();
						return;
			case 2 :				/* イルミネーション２ */
						dsp_watch_i2();
						return;
			case 3 :	return;		/* メインメニューへ */
			default :	break;
		}
	}
}


/* すべてのメニューでの選択 */
int do_menu(char *mstr[], int no)
{
	static int tp, key;

	wrt_topic(mstr, 0, no);

	for(tp = 0;;)
	{
		_AH = 0;
		geninterrupt(0x18);	/* キーデータの読みだし */
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
			default :	fputs("\x07", stderr);	/* ビープ音 */
		}
		if(key == 0x1c) return tp;
	}
}

/* メニュー項目表示 */
void wrt_topic(char *mstr[], int tp, int no)
{
	static int i;

	fputs("\x1b[24;1H\x1b[0K", stderr);	/* ２４行目のクリア */
	for(i=0; i<=no; i++)
	{
		if(i == tp) fputs("\x1b[47m", stderr);
		fputs(mstr[i], stderr);
		if(i == tp) fputs("\x1b[m", stderr);
		fputs("  ", stderr);
	}
	return ;
}

/* グラフィックスパレット設定 */
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

/* 文字列の入力 */
char *inp_str(char *mes)
{
	static char buf[40];
	fputs("\x1b[24;1H\x1b[0K", stderr);	/* ２４行目のクリア */
	fputs("\x1b[>5l", stderr);			/* カーソル ON */
	fputs(mes, stderr);
	fgets(buf, 20, stdin);
	fputs("\x1b[>5h", stderr);			/* カーソル OFF */

	return buf;
}

/* キーが入力されるのを待つ */
void wait_kbhit(void)
{
	_AH = 0;
	geninterrupt(0x18);	/* キーデータの読みだし */
	return ;
}

/* タイマーのアラーム */
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

/* 終了 */
void end(void)
{
	signal(SIGABRT, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	PC98_grph_cls();
	PC98_16color_palette_init();
	_AH = 0x41;
	geninterrupt(0x18);	/* グラフィックス表示停止 */

	fputs("\x1b[>5l\x1b[>1l\x1b[2J", stderr);	/* カーソル ON & クリア */
	exit(0);
}

/* About */
void about(void)
{
	fputs("\x1b[24;1H\x1b[0K", stderr);	/* ２４行目のクリア */
	fputs(" DCLK	:	\x1b[20mDigital CLocK\x1b[m  Ver 1.0		(C) 1993 Hirokazu Inoue", stderr);
	wait_kbhit();
	return ;
}

void help(void)
{
	_AH = 0x41;
	geninterrupt(0x18);	/* グラフィックス表示停止 */
	fputs("\x1b[2J"
			"DCLK	:	Digital CLocK  Ver 1.0		(C) 1993 Hirokazu Inoue\n"
			"使用法\n"
			"\n"
			"DCLK -[c/cb/cs/ct/k/d/i1/i2/s/sn/t/th/tn][\x1b[4m#/##:##/##:##:##\x1b[m]\n"
			"\n"
			"-c\x1b[4mrr:gg:bb\x1b[m          : 標準 ｶﾗｰﾊﾟﾚｯﾄ\n"
			"-cb\x1b[4mrr:gg:bb\x1b[m         : ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ ｶﾗｰﾊﾟﾚｯﾄ\n"
			"-cs\x1b[4mrr:gg:bb\x1b[m         : ｽﾄｯﾌﾟｳｫｯﾁ ｶﾗｰﾊﾟﾚｯﾄ\n"
			"-ct\x1b[4mrr:gg:bb\x1b[m         : 減算ﾀｲﾏｰ ｶﾗｰﾊﾟﾚｯﾄ\n"
			"-k                  : ｽｸﾘｰﾝｾｰﾊﾞ ﾓｰﾄﾞ\n"
			"-d\x1b[4m-hh:mm\x1b[m            : 第２時刻\n"
			"-i1                 : ｲﾙﾐﾈｰｼｮﾝ 1\n"
			"-i2                 : ｲﾙﾐﾈｰｼｮﾝ 2\n"
			"-s                  : ｽﾄｯﾌﾟｳｫｯﾁ\n"
			"-sn                 : ｽﾄｯﾌﾟｳｫｯﾁ 秒表示\n"
			"-t\x1b[4mhh:mm\x1b[m             : ﾀｲﾏｰ ﾀﾞｲﾚｸﾄ 指定\n"
			"-th\x1b[4mhh:mm\x1b[m            : ﾀｲﾏｰ\n"
			"-tn\x1b[4mnnnn\x1b[m             : ﾀｲﾏｰ 秒指定\n", stderr);
	wait_kbhit();
	end();
}
/* ファイルエンド [EOF] */