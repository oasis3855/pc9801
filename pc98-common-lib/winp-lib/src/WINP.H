/**************************************************************************
 WINP.H  for simple window library for Turbo C++
 (C)opyright Hirokazu Inoue 1990,91,92
 Support Nif:GDH03306
**************************************************************************/

#ifndef _WINP
#define _WINP
#define _WIN_SP /* for interface to WIN_SP.H */

#define W_PAGE_1 0
#define W_PAGE_2 1
#define W_RECT 11
#define W_RING 12
#define W_DUP 1
#define W_ON 1
#define W_OFF 0
#define W_DEFB T_WHITE|REVERSE
#define W_DEFC T_WHITE


#define W_FWRITE	0x01		/* init , write */
#define W_RESTORE	0x02		/* Re-write , not save back */
#define W_CLOSE	0x04		/* close */
#define W_MENU		0x08		/* do menu */
#define W_GSV_TXT	0x01		/* text vram */
#define W_GSV_GR	0x02		/* graphic vram */

// window structure in public
typedef struct {
	int left;		/* 2 to 79 */
	int top;		/* 2 to 24 */
	int right;	/* 2 to 79 */
	int bottom;	/* 2 to 24 */
	int color;	/* conio.h 's COLOR */
	int page;		/* VRAM page 0  , No meaning */
	int pat;		/* Boarder pattern 1,11,12 */
	int shrink;	/* Shrink / Large ON = 1 OFF = 0*/
	int slow;		/* Shrink / Large DELAY speed ,default 10*/
	char *sv;		/* SAVE buffer */
	int sv_on;	/* Call MAKE 1:SAVE , DELETE 1:RECOVER */
} WIN_INFO;

// menu structure in public
typedef struct{
	int left;
	int top;
	int data_len;
	int data_line;
	int data_max;
	int color;
	int boarder_color;
} MNU_INFO;

// extend menu structure in public
typedef struct{
	int left;
	int top;
	int data_len;
	int data_max;
	int color;
	int rev_color;
	int boarder_color;
} EXTEND_MNU_INFO;

// graphic back support

typedef struct{
	unsigned char left;
	unsigned char data_len;
	unsigned char top;
	unsigned char data_max;
	int color;
	int select_color;
	int boarder_color;
	unsigned char mode;	/* W_FWRITE , W_CLOSE , W_MENU */
	unsigned char save;	/* W_GSV_TXT | W_GSV_GR == text&graphic saved */
	unsigned char sv_flag;	/* never change , system use */
	unsigned char right_left;	/* use cursor right left key */
	unsigned char cur_topic;	/* 1 ~ */
	void *t_sv;	/* never change */
	void huge *g_sv;	/* never change */
} GMNU_INFO;

typedef struct{
	int left;
	int top;
	int win_len;	/* Window Width  1 ~ */
	int win_line;	/* Window Length 1 ~ */
	int data_max;	/* Max Menu no  1 ~ */
	int color;	/* non selected text color */
	int rev_color;	/* selected topic color */
	int boarder_color;	/* window boarder color */
	int cur_line;	/* current reverse line 1 ~ */
	int cur_topic;	/* current selected topic 1 ~ */
	int init_window;	/* W_FWRITE : MAKE , W_CLOSE : DELETE , OTHER : Do Menu Scroll */
} SCROLL_MNU_INFO;

typedef struct{
	unsigned char data_max;	/* “∆≠∞ÇÕ 1 - data_max */
	int (*mfunc)(int);		/* å¬ï ägí£“∆≠∞ä÷êî */
	int color;			/*  ﬁ∞ÇÃêF */
	int topic_color;		/* í èÌÇÃtopicêF */
	int select_color;		/* îΩì]éûtopicêF */
	unsigned char mode;	/* W_FWRITE , W_CLOSE , W_MENU */
	unsigned char cur_topic;	/* 1 ~ */
} GMNUBAR_INFO;



/* internal functions */
extern int peek_str(int sseg, int soff, char *dbuff, unsigned size);
extern int poke_str(int dseg, int doff, char *dbuff, unsigned size);
extern int textkei3(int  x1, int  y1, int  x2, int  y2, int att, int  kind, int  ura);
extern int dispkei2(int  x, int  y, unsigned char mark, char attr, int  kind, int  ura);
extern void iswap(int *p, int *q);
extern int vramrd(int x, int y, char *mark, char *attr, int  ura);
extern int vramwr(int x, int y, char *mark, char *attr, int  ura);
/* upper 7 functions are Original Copyright (C) 1990.11 C Magazine */

extern unsigned char _col_convert(int att);
extern void mnu_winit_put(char *str[],int data_line,int data_max,int color);
extern int mnu_wread_key(void);
/* menu functions */
extern int mnu_wmax_len(char *str[],int data_max);
extern int mnu_wmenu(char *str[],MNU_INFO *mnu);
extern void win_wscroll(int scr);
/* auto window */
extern int win_wmake(WIN_INFO *winf);
extern int win_wdelete(WIN_INFO *winf);
extern int win_wmove(int xr,int yr, WIN_INFO *winf);
extern void win_wtitle(char *title,int rx_title,int box_on,WIN_INFO *winf);
/* manual window */
extern int win_mmake(int left,int top,int right,int bottom,int color,int pattern,int page);
extern int win_mdelete(int left,int top,int right,int bottom);
/* extend menu 1 */
extern void mnu_winit_put_s(char *str[],int data_max,int cur_line,int color,int rev_color);
extern int mnu_wmenu_s(char *str[],EXTEND_MNU_INFO *mnu,int cur_data);

extern int mnu_gmenu(GMNU_INFO *mnu, int dummy, char **str, int *retcode);
extern void mnu_ginit_put_s(char **str,int data_max,int cur_line,int color,int rev_color);
extern int mnu_gmenu_txt(GMNU_INFO *mnu, int dummy, char *str[], int *retcode);
extern void mnu_ginit_put_s_txt(char *str[],int data_max,int cur_line,int color,int rev_color);

extern int inp_gwin(GMNU_INFO *mnu, char **str, char **mnu_str, int *retcode);
extern void inp_ginit_win(int col, int str_len, int str_max, char **mnu_str, char **str);
extern int inp_gwin_txt(GMNU_INFO *mnu, char *str[], char *mnu_str[], int *retcode);
extern void inp_ginit_win_txt(int col, int data_len, int data_max, char *mstr[], char *str[]);
extern void win_mnu_scroll(int updn, SCROLL_MNU_INFO *m);	/* updn == 1 : DOWN KEY , updn == -1 : UP KEY , updn other : do nothing */
extern void _win_mnu_scroll_dspdt(int _cur_topic_no);


extern int mnu_GLIO_gmenu(GMNU_INFO *mnu, int dummy, char **str, int *retcode);
extern void mnu_GLIO_ginit_put_s(char **str,int data_max,int cur_line,int color,int rev_color);
extern int inp_GLIO_gwin(GMNU_INFO *mnu, char **str, char **mnu_str, int *retcode);
extern void inp_GLIO_ginit_win(int col, int str_len, int str_max, char **mnu_str, char **str);
extern int win_GLIO_gwin(GMNU_INFO *mnu, int *retcode);
extern void win_GLIO_ginit_win(int col, int data_len, int data_max);

extern int mnu_menubar(GMNUBAR_INFO *mnu, char **str, int *x_topic, char *title, int x_title);
extern void mnu_barmake(GMNUBAR_INFO *mnu, char *str[], int *x_topic, char *title, int x_title);


#endif	/* _WINP */
/* end of win_sp.h */
