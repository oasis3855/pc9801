/* XYGR???x.CPP 用の総合ヘッダーファイル */
#ifndef _WINP
#include<winp.h>
#endif

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

/******* XYGRMNUx.CPP の関数 ********/
/* バーに表示されるサブメニュー */
extern int bar_mnu_func(int i);
extern int do_menu_1(void);
extern int do_menu_2(void);
extern int do_menu_3(void);
extern int do_menu_4(void);

extern void do_info_win(void);		/* 著作権表示 */
extern void restore_screen(void);
extern void init_graphic(void);

/* 入力部／数値 */
extern int do_input_int_1s(int *a, char *m_str[]);
extern int do_input_double_3s(double *a, double *b, double *c, char *m_str[]);
extern int do_input_double_4s(double*, double*, double*, double*, char *m_str[]);
extern void do_inp_xy(void);
extern void do_inp_xy_bod(void);
extern void do_inp_scale(void);
extern void do_inp_t(void);
extern void do_inp_c_grph(void);
extern void do_inp_c_scale(void);
extern void do_inp_c_0(void);

/* 入力部／文字 */
extern int do_input_char_1l(char*, char *m_str[]);
extern int do_input_char_2l(char *buf1, char *buf2, char *m_str[]);
extern int do_inp_fx(void);
extern int do_inp_fxyt(void);
extern int do_inp_dfx(void);
extern int do_inp_if(void);

/* スイッチ */
extern int do_switch(int *sw, int max_sw, char *m_str[]);
extern int do_sw_scale(void);
extern int do_sw_0(void);
extern int do_sw_line(void);
extern int do_sw_init(void);
extern int do_sw_bod(void);
extern int do_sw_if(void);

/******* XYGRFILx.CPP の関数 ********/
/* 初期化及び設定 */
extern void init_GMNU(void);		/* 最初に１回だけ呼ぶ */
extern void set_param(void);

/* 座標関係 */
extern void write_scale(void);		/* ここを呼び出す */
extern void write_boarder_scale(void);
extern void write_plane_scale(void);
extern void write_0_line(void);
extern void write_0_scale(void);

/* 画面を構築 */
extern void write_window(void);
extern void prn_data_win(void);

/******* XYGRMINx.CPP の関数 ********/
/* グラフを描く */
extern void write_graph(int mode);	/* ここを呼び出す */
extern void write_graph_x(void);
extern void write_graph_t(void);
extern void write_graph_d(void);

extern void chg_palette(void);		/* パレット設定 */
extern double calc_str(char *str);	/* parser版 atof */

/******* XYGRFILx.CPP の関数 ********/
extern void do_error_win(char *str);
extern void do_help(void);
extern void exe_gedit(void);
extern void exe_plot(void);
extern void do_save_file(void);
extern void do_load_file(void);
extern void do_save_env(void);
extern void do_load_env(void);

/******* XYGRMUSx.CPP の関数 ********/
extern void until_mouse_wait(void);
extern void do_mouse_plot(void);


/******* XYGRPARx.CPP で宣言されたグローバル変数 ********/
extern int maxloop,maxcolor,f_mode,cur_palette;
extern double dxratio,dyratio;
extern double x_min,x_max,y_min,y_max;
extern double x_ratio,y_ratio;
extern int x_width,y_width;
extern unsigned char formula_string[];
extern unsigned char formula_str_x[],formula_str_y[];
extern unsigned char formula_str_if[];
extern unsigned char formula_str_dfx[];
extern int sw_scale,sw_0,sw_if,sw_bod,sw_gr_line,sw_initmode;
extern double x_scale,y_scale,x_scale_0,y_scale_0;
extern double xl_min,xl_max,yl_min,yl_max,tl_min,tl_max,t_delta;
extern int math_err_flag;

extern GMNU_INFO w1,w2,w3,w4,w_info;
extern GMNU_INFO wi_1s,wi_3s,wi_4s,wi_1l,wi_2l,w_sw;
extern GMNUBAR_INFO w_bar;

extern int _c_mnu,_c_mnuv,_c_box,_c_bar,_c_barv,_c_barb,_c_info;
extern int _cg_info,_cg_man;
extern int _c_gr_grph,_c_gr_scale,_c_gr_0;
extern char far _bpat[33],_infopat[33];	/* ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ ｸﾞﾗﾌｨｯｸ ﾊﾟﾀｰﾝ */

extern unsigned char commandcom[], filename[];
extern char exe_gedit_file[],exe_plot_file[],exe_gsv_file[],exe_gld_file[];
