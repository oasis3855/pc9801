/********************************************************************
 HDCPY.C
 グラフィック／テキスト　ハードコピー
 initial 1991.03.15
		 1992.03.29
 (C) Hirokazu Inoue 1991,92

 Bug: Turbo C 2.0 統合環境中の画面の日本語の右側 Bug (TC2.0 Only)
********************************************************************/
#include    <stdio.h>
#include    <stdlib.h>
#include    <dos.h>
#include    <conio.h>
#include    <ctype.h>
#include    <string.h>

#define     ESC         0x1b
#define     CR          0x0D    /* プリンタ印字指令     */
#define     LF          0x0A    /* プリンタ改行指令     */
#define     LINE_BYTE   80      /* バイト / G-RAM 1 行  */
#define     X_MAX       79      /* 印字範囲右端最大値   */
#define     Y_MAX       24      /* 印字範囲下端最大値   */
#define     SEG_BLUE    0xa800
#define     SEG_RED     0xb000
#define     SEG_GREEN   0xb800
#define     SEG_GRAY    0xe000

#define	TextV		(unsigned char far *)(0xa0000000L)
#define	TextX		(unsigned char far *)(0xa2000000L)
#define	TextW(x,y)	*(unsigned int far *)(TextV + y*160 + x*2)
#define	TextA(x,y)	*(unsigned int far *)(TextX + y*160 + x*2)


/* ハードコピーグローバル変数 */
char _hc_sC[] = {"0111111101111111  "};		/* ｸﾞﾗﾌｨｯｸｽ ｺﾋﾟｰ ｶﾗｰ 1:ON 0:OFF    */
int _hc_sX = 1;			/* X 拡大 1:ON 0:OFF */
int _hc_sY = 0;			/* Y 拡大 1:ON 0:OFF */
int _hc_sL = 0;			/* 左座標 0-79 */
int _hc_sT = 0;			/* 上座標 0-24 */
int _hc_sR = X_MAX;		/* 右座標 0-79 */
int _hc_sB = Y_MAX;		/* 下座標 0-24 */
int _hc_sTXT = 1;		/* ﾃｷｽﾄﾓｰﾄﾞ 1:ON 0:OFF */
int _hc_sGR = 1;		/* ｸﾞﾗﾌｨｯｸｽﾓｰﾄﾞ 1:ON 0:OFF */

unsigned char _hc_fbuf[40],far *_hc_fbuf_far;	/* ﾌｫﾝﾄ ﾊﾞｯﾌｧ */
struct REGPACK reg;

typedef unsigned short  USHORT;
typedef unsigned char   UCHAR;

int hc_hardcopy(void);		/* ハードコピー */

/* 内部関数 */
USHORT hc_VRAM_trans(int x, int y);
void hc_mk_image(USHORT *pattern, UCHAR *image);
void hc_binp_16x16(UCHAR *image);
void hc_binp_8x16(UCHAR *image);
void hc_lp_str(char *p);
void hc_lp_char(int c);
void hc_prn_init(void);
void hc_prn_close(void);
char hc_mk_fimage(int under, int f_line, int l_char);
int hc_get_font(int x, int y, unsigned int *tattr);
int hc_mk_rev_image(int f, int tattr);


/****************************************/
/*  グラフィック画面印字メインルーチン  */
/****************************************/
int hc_hardcopy(void)
{
	UCHAR image[32];
	USHORT pattern[16];
	int x, y, dy, y_step,font_image,l_char;
	unsigned int tattr;

	if(_hc_sL < 0 || X_MAX < _hc_sL) return(1);
	if(_hc_sT < 0 || Y_MAX < _hc_sT) return(1);
	if(_hc_sR < 0 || X_MAX < _hc_sR) return(1);
	if(_hc_sB < 0 || Y_MAX < _hc_sB) return(1);
	if(_hc_sR < _hc_sL) return(1);
	if(_hc_sB < _hc_sT) return(1);

	_hc_fbuf_far = _hc_fbuf;			/* フォントバッファをfarにコピー */

	hc_prn_init();

	y_step    = (_hc_sY) ?  8 : 16;		/* ｙ方向拡大率 */

	/* 通常は 16 ドットおき、y 方向 2 倍印字のときは 8 ドットおき   */
	for (y = _hc_sT * 16; y < _hc_sB * 16 + 16; y += y_step)	/* 垂直方向スキャン */
	{
		reg.r_ax = 0x0100;			/* ｷｰﾊﾞｯﾌｧ 読みだし check BIOS */
		intr(0x18,&reg);
		if(reg.r_bx & 0x0100 && reg.r_ax == 0x001b){
			hc_prn_close();
			return(0x1b);		/* ESC 中断 */
		}

		if(_hc_sTXT & _hc_sGR)	/* ﾃｷｽﾄ & ｸﾞﾗﾌｨｯｸｽ */
		{
			for (x = _hc_sL; x <= _hc_sR; x++)			/* 水平方向スキャン */
			{
				l_char = hc_get_font(x,y/16,&tattr);	/* ﾃｷｽﾄ文字フォントを得る */
				for (dy = 0; dy < y_step; dy++)			/* ﾃｷｽﾄ 1文字に対応するデータ作成 */
				{
					font_image = (int )hc_mk_fimage(y%16,dy,l_char);
					font_image = hc_mk_rev_image(font_image,tattr);
					pattern[dy] = hc_VRAM_trans(x, y + dy) | font_image;
				}
				hc_mk_image(pattern, image);		/* ﾌﾟﾘﾝﾀｰに送るﾃﾞｰﾀ形式に */

				/* send data to printer */
				if(_hc_sX) hc_binp_16x16(image);
				else hc_binp_8x16(image);
			}
		}	/* ﾃｷｽﾄ & ｸﾞﾗﾌｨｯｸｽ */
		else if(_hc_sGR)	/* ｸﾞﾗﾌｨｯｸｽのみ */
		{
			for (x = _hc_sL; x <= _hc_sR; x++)
			{
				for (dy = 0; dy < y_step; dy++)
				{
					pattern[dy] = hc_VRAM_trans(x, y + dy);
				}
				hc_mk_image(pattern, image);

				/* send data to printer */
				if(_hc_sX) hc_binp_16x16(image);
				else hc_binp_8x16(image);
			}
		}	/* ｸﾞﾗﾌｨｯｸｽのみ */
		else		/* ﾃｷｽﾄのみ */
		{
			for (x = _hc_sL; x <= _hc_sR; x++)
			{
				l_char = hc_get_font(x,y/16,&tattr);
				for (dy = 0; dy < y_step; dy++)
				{
					font_image = (int )hc_mk_fimage(y%16,dy,l_char);
					font_image = hc_mk_rev_image(font_image,tattr);
					pattern[dy] = font_image;
				}
				hc_mk_image(pattern, image);

				/* send data to printer */
				if(_hc_sX) hc_binp_16x16(image);
				else hc_binp_8x16(image);
			}
		}	/* ﾃｷｽﾄのみ */
		hc_lp_char(CR);		/* １行改行 */
		hc_lp_char(LF);
	}
	hc_prn_close();
	return(0);
}


/****************************************/
/*          印字データを求める          */
/****************************************/
/*
    座標(x * 8, y) のデータを 1 バイト * 4 プレーン読み、
    色を調べて、印字すべきデータを 1 バイト返す。
*/
USHORT hc_VRAM_trans(int x, int y)
{
    long    offset;
    int     i, c, color_max;
    USHORT  ret = 0;
    USHORT  blue, red, green, gray;
    USHORT  blue_p, red_p, green_p ,gray_p;

	/*----- 各プレーンから 1 byte ずつ取る  */
    offset = y * LINE_BYTE + x;
    red   = peekb(SEG_RED,   offset);
    green = peekb(SEG_GREEN, offset);
    blue  = peekb(SEG_BLUE,  offset);
    gray  = peekb(SEG_GRAY,  offset);

    /*----- パレット番号を順番に見ていく */
	for (i = 0; i < 16; i++)
	{
		/*----- i はパレット番号, c は印字色    */
		c = *(_hc_sC + i) - '0';

        /*----- もしパレット番号 i がそのインクを必要とするなら */
		if (c != 0)		/* 0 はプリンとしない */
		{
			blue_p  = (i & 0x01) ? blue  : ~blue;
			green_p = (i & 0x02) ? green : ~green;
			red_p   = (i & 0x04) ? red   : ~red;
			gray_p  = (i & 0x08) ? gray  : ~gray;

			ret |= (blue_p & red_p & green_p & gray_p);
		}
	}
    return(ret);
}


/****************************************/
/*   フォントデータを求める  INT18h     */
/****************************************/
int hc_get_font(int x, int y, unsigned int *tattr)
{
	unsigned int tword, l_char=0;

	tword = TextW(x,y);			/* 文字データ */
	*tattr = TextA(x,y);		/* ｱﾄﾘﾋﾞｭｰﾄ */
	if(tword & 0x0080) l_char = 1;		/* 2 ﾊﾞｲﾄ 漢字 右側 */
	if(tword >= 0x00ff)		/* VRAM 形式を JIS に変換 */
	{
		tword &= ~0x8080;
		tword += 0x0020;
		tword = (tword >> 8) + (tword << 8);
	}

	if(tword < 0xff) tword |= 0x8000;	/* ANK */

	reg.r_ax = 0x1400;		/* 16ﾄﾞｯﾄフォントを求める BIOS */
	reg.r_bx = FP_SEG(_hc_fbuf_far);
	reg.r_cx = FP_OFF(_hc_fbuf_far);
	reg.r_dx = tword;
	intr(0x18,&reg);		/* CRT BIOS */

	return l_char;
}

/****************************************/
/*  ﾌｫﾝﾄﾃﾞｰﾀよりｲﾒｰｼﾞﾃﾞｰﾀを求める       */
/****************************************/
char hc_mk_fimage(int under, int f_line, int l_char)
{
	if(under)		/* 下半分 */
	{
		if(_hc_fbuf[1] == (unsigned char )1)	/* ANK */
		{
			return (char )(_hc_fbuf[f_line + 8 + 2]);
		}
		else			/* 2 ﾊﾞｲﾄ 漢字 */
		{
			if(l_char) return (char )(_hc_fbuf[f_line * 2 + 17 + 2]);
			return (char )(_hc_fbuf[f_line * 2 + 16 + 2]);
		}
	}
	else		/* 上半分 又は 全体 */
	{
		if(_hc_fbuf[1] == (unsigned char )1)	/* ANK */
		{
			return (char )(_hc_fbuf[f_line + 2]);
		}
		else			/* 2 ﾊﾞｲﾄ 漢字 */
		{
			if(l_char) return (char )(_hc_fbuf[f_line * 2 + 1 + 2]);
			else return (char )(_hc_fbuf[f_line * 2 + 2]);
		}
	}
}


/****************************************/
/*        フォントパターンの反転        */
/****************************************/
int hc_mk_rev_image(int f, int tattr)
{
	if(tattr & 0x0004) f = ~f & 0x00ff;		/* reverse */
	if(!(tattr & 0x0001)) f = 0;			/* secret */
	if((tattr & 0x00e0) == 0) f = 0;		/* black */
	return f;
}



/****************************************/
/*        イメージパターンの作成        */
/****************************************/
void hc_mk_image(USHORT *pattern, UCHAR *image)
{
    int     i, y, adrs;
    USHORT  p;

    /*----- _hc_sY のとき pattern を Y 方向 2 倍に拡大    */
    if (_hc_sY)
    {
        for (i = 7; 0 <= i; i--)
        {
            pattern[i * 2 + 1] = pattern[i * 2] = pattern[i];
        }
    }

    /*----- imange をクリアする */
    for (i = 0; i < 32; i++) image[i] = 0;

    /*----- pattern を順に見ていき、image に格納    */
    for (y = 0; y < 16; y++)
    {
        p = pattern[y];
        for (i = 0; i < 8; i++)
        {
            /*----- ビット i が立っていたら image に値を格納    */
            if ((p << i) & 0x80)
            {
                image[(i << 1) + (y >> 3)] |= (1 << (y % 8));
            }
        }
    }

    /*----- _hc_sX の指定があるとき image を X 方向 2 倍に拡大    */
    if (_hc_sX)
    {
        for (i = 15; 0 <= i; i--)
        {
            adrs = (i * 2) - (i & 1);
            image[adrs + 2] = image[adrs] = image[i];
        }
    }
}


/****************************************/
/*       16*16 ドットイメージ印字       */
/****************************************/
/*
    16 ビットドット列対応グラフィック印字モードで、
    16 列分のデータを送信。
*/
void hc_binp_16x16(UCHAR *image)
{
    int     i;

    hc_lp_str("\x1b" "I0016");
    for (i = 0; i < 32; i++) { hc_lp_char((int) *(image + i)); }
}


/****************************************/
/*       8*16 ドットイメージ印字        */
/****************************************/
/*
    16 ビットドット列対応グラフィック印字モードで、
    8 列分のデータを送信。
*/
void hc_binp_8x16(UCHAR *image)
{
    int     i;

    hc_lp_str("\x1b" "I0008");
    for (i = 0; i < 16; i++) { hc_lp_char((int) *(image + i)); }
}


/****************************************/
/*         コード送信 複数バイト        */
/****************************************/
void hc_lp_str(char *p)
{
	reg.r_ax = 0x3000;
	reg.r_cx = strlen(p);
	reg.r_bx = FP_OFF(p);
	reg.r_es = FP_SEG(p);

	intr(0x1a,&reg);	/* ﾌﾟﾘﾝﾀ BIOS */
}


/****************************************/
/*         コード送信 １バイト          */
/****************************************/
void hc_lp_char(int c)
{
	reg.r_ax = 0x1100 | (0x00ff & c);

	intr(0x1a,&reg);	/* ﾌﾟﾘﾝﾀ BIOS */
}


/****************************************/
/*          プリンタ初期設定            */
/****************************************/
void hc_prn_init(void)
{
    hc_lp_str("\x1b" "M");      /* ネイティブモード         */
    hc_lp_str("\x1b" "T12");    /* 12/120 インチ改行モード  */
    hc_lp_char(CR);
    hc_lp_char(LF);
}


/****************************************/
/*            プリンタ後処理            */
/****************************************/
void hc_prn_close(void)
{
    hc_lp_str("\x1b" "A");      /* 1/6 インチ改行モード */
    hc_lp_char(CR);
    hc_lp_char(LF);
}

/* end */