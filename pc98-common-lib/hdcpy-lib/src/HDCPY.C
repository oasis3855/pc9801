/********************************************************************
 HDCPY.C
 �O���t�B�b�N�^�e�L�X�g�@�n�[�h�R�s�[
 initial 1991.03.15
		 1992.03.29
 (C) Hirokazu Inoue 1991,92

 Bug: Turbo C 2.0 ���������̉�ʂ̓��{��̉E�� Bug (TC2.0 Only)
********************************************************************/
#include    <stdio.h>
#include    <stdlib.h>
#include    <dos.h>
#include    <conio.h>
#include    <ctype.h>
#include    <string.h>

#define     ESC         0x1b
#define     CR          0x0D    /* �v�����^�󎚎w��     */
#define     LF          0x0A    /* �v�����^���s�w��     */
#define     LINE_BYTE   80      /* �o�C�g / G-RAM 1 �s  */
#define     X_MAX       79      /* �󎚔͈͉E�[�ő�l   */
#define     Y_MAX       24      /* �󎚔͈͉��[�ő�l   */
#define     SEG_BLUE    0xa800
#define     SEG_RED     0xb000
#define     SEG_GREEN   0xb800
#define     SEG_GRAY    0xe000

#define	TextV		(unsigned char far *)(0xa0000000L)
#define	TextX		(unsigned char far *)(0xa2000000L)
#define	TextW(x,y)	*(unsigned int far *)(TextV + y*160 + x*2)
#define	TextA(x,y)	*(unsigned int far *)(TextX + y*160 + x*2)


/* �n�[�h�R�s�[�O���[�o���ϐ� */
char _hc_sC[] = {"0111111101111111  "};		/* ���̨��� ��߰ �װ 1:ON 0:OFF    */
int _hc_sX = 1;			/* X �g�� 1:ON 0:OFF */
int _hc_sY = 0;			/* Y �g�� 1:ON 0:OFF */
int _hc_sL = 0;			/* �����W 0-79 */
int _hc_sT = 0;			/* ����W 0-24 */
int _hc_sR = X_MAX;		/* �E���W 0-79 */
int _hc_sB = Y_MAX;		/* �����W 0-24 */
int _hc_sTXT = 1;		/* ÷��Ӱ�� 1:ON 0:OFF */
int _hc_sGR = 1;		/* ���̨���Ӱ�� 1:ON 0:OFF */

unsigned char _hc_fbuf[40],far *_hc_fbuf_far;	/* ̫�� �ޯ̧ */
struct REGPACK reg;

typedef unsigned short  USHORT;
typedef unsigned char   UCHAR;

int hc_hardcopy(void);		/* �n�[�h�R�s�[ */

/* �����֐� */
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
/*  �O���t�B�b�N��ʈ󎚃��C�����[�`��  */
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

	_hc_fbuf_far = _hc_fbuf;			/* �t�H���g�o�b�t�@��far�ɃR�s�[ */

	hc_prn_init();

	y_step    = (_hc_sY) ?  8 : 16;		/* �������g�嗦 */

	/* �ʏ�� 16 �h�b�g�����Ay ���� 2 �{�󎚂̂Ƃ��� 8 �h�b�g����   */
	for (y = _hc_sT * 16; y < _hc_sB * 16 + 16; y += y_step)	/* ���������X�L���� */
	{
		reg.r_ax = 0x0100;			/* ���ޯ̧ �ǂ݂��� check BIOS */
		intr(0x18,&reg);
		if(reg.r_bx & 0x0100 && reg.r_ax == 0x001b){
			hc_prn_close();
			return(0x1b);		/* ESC ���f */
		}

		if(_hc_sTXT & _hc_sGR)	/* ÷�� & ���̨��� */
		{
			for (x = _hc_sL; x <= _hc_sR; x++)			/* ���������X�L���� */
			{
				l_char = hc_get_font(x,y/16,&tattr);	/* ÷�ĕ����t�H���g�𓾂� */
				for (dy = 0; dy < y_step; dy++)			/* ÷�� 1�����ɑΉ�����f�[�^�쐬 */
				{
					font_image = (int )hc_mk_fimage(y%16,dy,l_char);
					font_image = hc_mk_rev_image(font_image,tattr);
					pattern[dy] = hc_VRAM_trans(x, y + dy) | font_image;
				}
				hc_mk_image(pattern, image);		/* �������ɑ����ް��`���� */

				/* send data to printer */
				if(_hc_sX) hc_binp_16x16(image);
				else hc_binp_8x16(image);
			}
		}	/* ÷�� & ���̨��� */
		else if(_hc_sGR)	/* ���̨����̂� */
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
		}	/* ���̨����̂� */
		else		/* ÷�Ă̂� */
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
		}	/* ÷�Ă̂� */
		hc_lp_char(CR);		/* �P�s���s */
		hc_lp_char(LF);
	}
	hc_prn_close();
	return(0);
}


/****************************************/
/*          �󎚃f�[�^�����߂�          */
/****************************************/
/*
    ���W(x * 8, y) �̃f�[�^�� 1 �o�C�g * 4 �v���[���ǂ݁A
    �F�𒲂ׂāA�󎚂��ׂ��f�[�^�� 1 �o�C�g�Ԃ��B
*/
USHORT hc_VRAM_trans(int x, int y)
{
    long    offset;
    int     i, c, color_max;
    USHORT  ret = 0;
    USHORT  blue, red, green, gray;
    USHORT  blue_p, red_p, green_p ,gray_p;

	/*----- �e�v���[������ 1 byte �����  */
    offset = y * LINE_BYTE + x;
    red   = peekb(SEG_RED,   offset);
    green = peekb(SEG_GREEN, offset);
    blue  = peekb(SEG_BLUE,  offset);
    gray  = peekb(SEG_GRAY,  offset);

    /*----- �p���b�g�ԍ������ԂɌ��Ă��� */
	for (i = 0; i < 16; i++)
	{
		/*----- i �̓p���b�g�ԍ�, c �͈󎚐F    */
		c = *(_hc_sC + i) - '0';

        /*----- �����p���b�g�ԍ� i �����̃C���N��K�v�Ƃ���Ȃ� */
		if (c != 0)		/* 0 �̓v�����Ƃ��Ȃ� */
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
/*   �t�H���g�f�[�^�����߂�  INT18h     */
/****************************************/
int hc_get_font(int x, int y, unsigned int *tattr)
{
	unsigned int tword, l_char=0;

	tword = TextW(x,y);			/* �����f�[�^ */
	*tattr = TextA(x,y);		/* ����ޭ�� */
	if(tword & 0x0080) l_char = 1;		/* 2 �޲� ���� �E�� */
	if(tword >= 0x00ff)		/* VRAM �`���� JIS �ɕϊ� */
	{
		tword &= ~0x8080;
		tword += 0x0020;
		tword = (tword >> 8) + (tword << 8);
	}

	if(tword < 0xff) tword |= 0x8000;	/* ANK */

	reg.r_ax = 0x1400;		/* 16�ޯăt�H���g�����߂� BIOS */
	reg.r_bx = FP_SEG(_hc_fbuf_far);
	reg.r_cx = FP_OFF(_hc_fbuf_far);
	reg.r_dx = tword;
	intr(0x18,&reg);		/* CRT BIOS */

	return l_char;
}

/****************************************/
/*  ̫���ް����Ұ���ް������߂�       */
/****************************************/
char hc_mk_fimage(int under, int f_line, int l_char)
{
	if(under)		/* ������ */
	{
		if(_hc_fbuf[1] == (unsigned char )1)	/* ANK */
		{
			return (char )(_hc_fbuf[f_line + 8 + 2]);
		}
		else			/* 2 �޲� ���� */
		{
			if(l_char) return (char )(_hc_fbuf[f_line * 2 + 17 + 2]);
			return (char )(_hc_fbuf[f_line * 2 + 16 + 2]);
		}
	}
	else		/* �㔼�� ���� �S�� */
	{
		if(_hc_fbuf[1] == (unsigned char )1)	/* ANK */
		{
			return (char )(_hc_fbuf[f_line + 2]);
		}
		else			/* 2 �޲� ���� */
		{
			if(l_char) return (char )(_hc_fbuf[f_line * 2 + 1 + 2]);
			else return (char )(_hc_fbuf[f_line * 2 + 2]);
		}
	}
}


/****************************************/
/*        �t�H���g�p�^�[���̔��]        */
/****************************************/
int hc_mk_rev_image(int f, int tattr)
{
	if(tattr & 0x0004) f = ~f & 0x00ff;		/* reverse */
	if(!(tattr & 0x0001)) f = 0;			/* secret */
	if((tattr & 0x00e0) == 0) f = 0;		/* black */
	return f;
}



/****************************************/
/*        �C���[�W�p�^�[���̍쐬        */
/****************************************/
void hc_mk_image(USHORT *pattern, UCHAR *image)
{
    int     i, y, adrs;
    USHORT  p;

    /*----- _hc_sY �̂Ƃ� pattern �� Y ���� 2 �{�Ɋg��    */
    if (_hc_sY)
    {
        for (i = 7; 0 <= i; i--)
        {
            pattern[i * 2 + 1] = pattern[i * 2] = pattern[i];
        }
    }

    /*----- imange ���N���A���� */
    for (i = 0; i < 32; i++) image[i] = 0;

    /*----- pattern �����Ɍ��Ă����Aimage �Ɋi�[    */
    for (y = 0; y < 16; y++)
    {
        p = pattern[y];
        for (i = 0; i < 8; i++)
        {
            /*----- �r�b�g i �������Ă����� image �ɒl���i�[    */
            if ((p << i) & 0x80)
            {
                image[(i << 1) + (y >> 3)] |= (1 << (y % 8));
            }
        }
    }

    /*----- _hc_sX �̎w�肪����Ƃ� image �� X ���� 2 �{�Ɋg��    */
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
/*       16*16 �h�b�g�C���[�W��       */
/****************************************/
/*
    16 �r�b�g�h�b�g��Ή��O���t�B�b�N�󎚃��[�h�ŁA
    16 �񕪂̃f�[�^�𑗐M�B
*/
void hc_binp_16x16(UCHAR *image)
{
    int     i;

    hc_lp_str("\x1b" "I0016");
    for (i = 0; i < 32; i++) { hc_lp_char((int) *(image + i)); }
}


/****************************************/
/*       8*16 �h�b�g�C���[�W��        */
/****************************************/
/*
    16 �r�b�g�h�b�g��Ή��O���t�B�b�N�󎚃��[�h�ŁA
    8 �񕪂̃f�[�^�𑗐M�B
*/
void hc_binp_8x16(UCHAR *image)
{
    int     i;

    hc_lp_str("\x1b" "I0008");
    for (i = 0; i < 16; i++) { hc_lp_char((int) *(image + i)); }
}


/****************************************/
/*         �R�[�h���M �����o�C�g        */
/****************************************/
void hc_lp_str(char *p)
{
	reg.r_ax = 0x3000;
	reg.r_cx = strlen(p);
	reg.r_bx = FP_OFF(p);
	reg.r_es = FP_SEG(p);

	intr(0x1a,&reg);	/* ����� BIOS */
}


/****************************************/
/*         �R�[�h���M �P�o�C�g          */
/****************************************/
void hc_lp_char(int c)
{
	reg.r_ax = 0x1100 | (0x00ff & c);

	intr(0x1a,&reg);	/* ����� BIOS */
}


/****************************************/
/*          �v�����^�����ݒ�            */
/****************************************/
void hc_prn_init(void)
{
    hc_lp_str("\x1b" "M");      /* �l�C�e�B�u���[�h         */
    hc_lp_str("\x1b" "T12");    /* 12/120 �C���`���s���[�h  */
    hc_lp_char(CR);
    hc_lp_char(LF);
}


/****************************************/
/*            �v�����^�㏈��            */
/****************************************/
void hc_prn_close(void)
{
    hc_lp_str("\x1b" "A");      /* 1/6 �C���`���s���[�h */
    hc_lp_char(CR);
    hc_lp_char(LF);
}

/* end */