/* window internal functions by C Magazine 1990 */
#define W_PAGE_1 0
#define W_PAGE_2 1
#define W_RECT 11
#define W_RING 12
#define W_DUP 1
#define W_ON 1
#define W_OFF 0;
#define W_DEFB T_WHITE|REVERSE
#define W_DEFC T_WHITE

typedef struct {
	int left;		/* 2 to 79 */
	int top;		/* 2 to 24 */
	int right;	/* 2 to 79 */
	int bottom;	/* 2 to 24 */
	int color;	/* conio.h 's COLOR */
	int page;		/* VRAM page 0 , 1 */
	int pat;		/* Boarder pattern 1,11,12 */
	int shrink;	/* Shrink / Large ON = 1 OFF = 0*/
	int slow;		/* Shrink / Large DELAY speed ,default 10*/
	char *sv;		/* SAVE buffer */
	int sv_on;	/* Call MAKE 1:SAVE , DELETE 1:RECOVER */
} WIN_INFO;


typedef struct{
	int left;
	int top;
	int data_len;
	int data_line;
	int data_max;
	int color;
	int boarder_color;
} MNU_INFO;

extern int peek_str(int sseg, int soff, char *dbuff, unsigned size);
extern int poke_str(int dseg, int doff, char *dbuff, unsigned size);
extern int textkei3(int  x1, int  y1, int  x2, int  y2, int att, int  kind, int  ura);
extern int dispkei2(int  x, int  y, unsigned char mark, char attr, int  kind, int  ura);
extern void iswap(int *p, int *q);
extern int vramrd(int x, int y, char *mark, char *attr, int  ura);
extern int vramwr(int x, int y, char *mark, char *attr, int  ura);
extern unsigned char _col_convert(int att);
extern void mnu_winit_put(char *str[],int data_line,int data_max,int color);
extern int mnu_wread_key(void);
extern int mnu_wmax_len(char *str[],int data_max);
extern int mnu_wmenu(char *str[],MNU_INFO *mnu);
extern void win_wscroll(int scr);
extern int win_wmake(WIN_INFO *winf);
extern int win_wdelete(WIN_INFO *winf);
extern int win_wmove(int xr,int yr, WIN_INFO *winf);
extern void win_wtitle(char *title,int rx_title,int box_on,WIN_INFO *winf);
extern int win_mmake(int left,int top,int right,int bottom,int color,int pattern,int page);
extern int win_mdelete(int left,int top,int right,int bottom);
