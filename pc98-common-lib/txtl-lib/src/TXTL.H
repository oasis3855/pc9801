/********************************************************************
 TXTL.H    for Turbo C++
 (C)opyright Hirokazu Inoue 1991,92
 Support Nif:GDH03306
********************************************************************/

#ifndef _TXTL
#define _TXTL
#define _TXT_INP /* for interface TXTL_SP.H */

#define K_UP		0x0001
#define K_DN		0x0002
#define K_ESC		0x001b
#define K_TAB		0x0009
#define NO_MEM		0x0011

#define _HIST_GET	0x0001
#define _HIST_FREE	0x0002


extern int txt_jinput(char *str, int str_len, char *old_str,
	    int color, int x, int y);
extern int txt_jinp_hist(char *str, int str_len, char *histstr,
            int max_hist, int color, int x, int y);
char *txt_get_histbuf(char *hist_buf, int max_hist, int hist_len,
            int mode);

extern unsigned int _txt_jinp_beep;
extern unsigned int _txt_jinp_bplen;
extern unsigned int _txt_jinp_bptone;
extern unsigned int _txt_jinp_tab;
extern unsigned int _txt_jinp_up;
extern unsigned int _txt_jinp_down;

#endif	/* _TXTL */
