/*  INPUT FUNCTION
    BASED ON TURBO C  MCALC
    by Hirokazu Inoue 1991
*/

#ifndef _TXT_INP
#define _TXT_INP

#define TRUE		1
#define FALSE		0

#define BS		0x0008
#define FORMFEED	0x000C
#define CR		0x000D
#define ESC		0x001B
#define PGUPKEY	0x7F36
#define PGDNKEY	0x7F37
#define INSKEY		0x7F38
#define DELKEY		0x7F39
#define UPKEY		0x7F3A
#define LEFTKEY	0x7F3B
#define RIGHTKEY	0x7F3C
#define DOWNKEY	0x7F3D
#define HOMEKEY	0x7F3E
#define HELPKEY	0x7F3F
#define ENDKEY		0x7F40
#define F1		0x7F62
#define F2		0x7F63
#define F3		0x7F64
#define F4		0x7F65
#define F5		0x7F66
#define F6		0x7F67
#define F7		0x7F68
#define F8		0x7F69
#define F9		0x7F6A
#define F10		0x7F6B
#define ER_KEY		0x7f6C

#define K_UP		0x0001
#define K_DN		0x0002
#define K_ESC		0x001b
#define K_TAB		0x0009
#define NO_MEM		0x0011

#define _HIST_GET	0x0001
#define _HIST_FREE	0x0002


#define	LMK	& 0xff

#ifdef _TXT_INP_MAIN

#include <alloc.h>
#include <dos.h>
#include <string.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <jstring.h>
#include <jctype.h>
#include <signal.h>


unsigned int _txt_jinp_beep = 0x0001;
unsigned int _txt_jinp_bplen = 25;
unsigned int _txt_jinp_bptone = 4000;
unsigned int _txt_jinp_tab = 0;
unsigned int _txt_jinp_up = 0;
unsigned int _txt_jinp_down = 0;

unsigned char fkey_def[67] = {		/* key table for PC-9801 */
  0x7F, PGUPKEY  LMK, 0, 0, 0, 0,		/* ROLL UP */
  0x7F, PGDNKEY  LMK, 0, 0, 0, 0,		/* ROLL DOWN */
  0x7F, INSKEY   LMK, 0, 0, 0, 0,		/* INS */
  0x7F, DELKEY   LMK, 0, 0, 0, 0,		/* DEL */
  0x7F, UPKEY    LMK, 0, 0, 0, 0,		/* UP */
  0x7F, LEFTKEY  LMK, 0, 0, 0, 0,		/* LEFT */
  0x7F, RIGHTKEY LMK, 0, 0, 0, 0,		/* RIGHT */
  0x7F, DOWNKEY  LMK, 0, 0, 0, 0,		/* DOWN */
  0x7F, HOMEKEY  LMK, 0, 0, 0, 0,		/* CLR */
  0x7F, HELPKEY  LMK, 0, 0, 0, 0,		/* HELP */
  0x7F, ENDKEY   LMK, 0, 0, 0, 0, 		/* HOME */
};

unsigned char fkey_sav[67];			/* default save storage */


int txt_jinput(char *str, int str_len, char *old_str,
	int color, int x, int y);
int txt_jinp_edit(char *s, const char *legal, const int maxlength,
	const int text_color, const int mes_x, const int mes_y);
unsigned int txt_fontsize(char *s, unsigned int nbyte);
void txt_outp(int col, int row, int color, int width,char *str,char *buf);
void txt_chgcursor(int insmode);
void txt_chgkey(int func, unsigned char *tab);
void txt_chgerrkey(int key);
void txt_jinp_error();
int txt_read_key(void);
int is_half_of_kanji(unsigned short int c);


#else	/* _TXT_INP_MAIN */


extern int txt_jinput(char *str, int str_len, char *old_str,
	int color, int x, int y);
extern int txt_jinp_edit(char *s, const char *legal, const int maxlength,
	const int text_color, const int mes_x, const int mes_y);
extern unsigned int txt_fontsize(char *s, unsigned int nbyte);
extern void txt_outp(int col, int row, int color, int width,char *str,char *buf);
extern void txt_chgcursor(int insmode);
extern void txt_chgkey(int func, unsigned char *tab);
extern void txt_chgerrkey(int key);
extern void txt_jinp_error();
extern int txt_read_key(void);
extern int is_half_of_kanji(unsigned short int c);



extern unsigned char fkey_def[67];
extern unsigned char fkey_sav[67];


extern unsigned int _txt_jinp_beep;
extern unsigned int _txt_jinp_bplen;
extern unsigned int _txt_jinp_bptone;
extern unsigned int _txt_jinp_tab;
extern unsigned int _txt_jinp_up;
extern unsigned int _txt_jinp_down;

int txt_jinp_hist(char *str, int str_len, char *histstr, int max_hist,
	int color, int x, int y);
char *txt_get_histbuf(char *hist_buf, int max_hist, int hist_len, int mode);

#endif	/* _TXT_INP_MAIN */
#endif	/* _TXT_INP */
