/* MANDEL.OV1 用　インクルード　ファイル */
#ifndef _WINP
#include<winp.h>
#endif
#ifndef __MANDEL_H
#define __MANDEL_H

#ifndef __TURBOC__
#error Compile only in Turbo C++
#endif __TURBOC__

/* グラフィックウインドウ */
#define	xw_min	(136-50)
#define	xw_max	(504-50)
#define	yw_min	26
#define	yw_max	383

/* インフォメーション用テキストウインドウ */
#define	xtw_min	(66-6)
#define	xtw_max	79
#define	ytw_min	3
#define	ytw_max	17

#define	Xwidth	(xw_max - xw_min)
#define	Ywidth	(yw_max - yw_min)

extern int do_menu_1(void);
extern int do_menu_2(void);
extern int do_menu_3(void);
extern void do_input_w4(void);
extern void do_input_int(int *num, char *m_str[]);
extern int do_input_char(char *str, char *m_str[]);
extern void do_mnu_palette(void);
extern void do_menu_sv(void);
extern void do_info_win(void);
extern void do_error_win(char *str);

extern void restore_screen(void);

extern char *m_str1[];
extern char *m_str2[];
extern char *m_str3[];
extern char *m_str4[];
extern char *m_str_mc[];
extern char *m_str_ml[];
extern char *m_str_pal[];

/* MAN_MIN.C */
#ifdef __cplusplus
extern "C"{
#endif
extern void chg_palette(void);
extern int do_mandel(void);
extern int do_mandel_C(void);
#ifdef __cplusplus
}
#endif

extern void do_mouse_plot(void);

extern long xmin,xmax,ymin,ymax;
extern long double dxmin,dxmax,dymin,dymax;
extern int maxloop,maxcolor,f_mode,cur_palette;
extern long double x_cen,y_cen,xy_scale;
extern long double dxratio,dyratio;
extern int _c_mnu,_c_mnuv,_c_box,_c_bar,_c_barv,_c_barb,_c_info;
extern int _cg_info,_cg_man;	/* エリアボーダー色 */
extern char far _bpat[33],_infopat[33];	/* ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ ｸﾞﾗﾌｨｯｸ ﾊﾟﾀｰﾝ */

/* MAN_PAR.CPP */
extern void set_param(void);
extern void prn_data_win(void);
extern void mk_mandel_win(void);
extern void init_GMNU(void);

extern void save_file(void);
extern void load_file(void);
extern void save_env(void);
extern void load_env(void);

extern GMNU_INFO w1,w2,w3,w4,wi,w_pal,w_sv,w_info;
extern GMNUBAR_INFO	w_bar;
extern int bar_mnu_func(int i);

extern char filename[];
extern char tmpname[];
extern char fildr,tmpdr;

#endif __MANDEL_H