/*********************************************************************
 ETC.H  PC-9801の様々な関数(BIOS等)
 (C)opyright 井上 博計 1990,91,92
 Support Nifty Serve:GDH03306
*********************************************************************/

#ifndef _ETC_HEADER
#define _ETC_HEADER
#define _ETC_S_HEADER  /* ETC_S.H との互換 */

/* MS-DOS ｷｰﾎﾞｰﾄﾞﾏﾄﾘｸｽ BIOS ﾜｰｸ領域 (DOS5 DOSSWAPでは使用不可) */
extern volatile unsigned char far *_km_m0, far *_km_m1, far *_km_m2;
extern volatile unsigned char far *_km_m3, far *_km_m4, far *_km_m5;
extern volatile unsigned char far *_km_m6, far *_km_m7, far *_km_m8;
extern volatile unsigned char far *_km_m9, far *_km_ma, far *_km_mb;
extern volatile unsigned char far *_km_mc, far *_km_md, far *_km_me;

#ifdef __cplusplus
extern "C"{
#endif

/* CPU & 98本体に関する関数 */
extern void  PC98_286CPU_reset(void);		/* ｿﾌﾄｳｴｱ ﾘｾｯﾄ */
extern void  PC98_286CPU_protect_mode(void);	/* A24ﾗｲﾝを有効に */
extern void  PC98_386CPU_protect_access(int sw);	/* 1:protect,0:real */
extern unsigned int  PC98_chk_8086(void);	/* 1:86/V30, 0:286/386 */
extern unsigned int  PC98_chk_hi_resolution(void);	/* 1:ﾊｲﾚｿﾞ, 0:ﾉｰﾏﾙ */
extern unsigned int  PC98_chk_16color(void);	/* 16色:1, 8色:0 */

/* テキスト画面に対する関数 */
extern void  PC98_88font(int sw);	/* 1:6x8 , 0:7x13  ANK ﾌｫﾝﾄ */
extern void  PC98_cls_txt(void);
extern void  PC98_print_doll(unsigned char *str);	/* "...$" 文字列 */
extern void  PC98_locate_txt(unsigned int _x, unsigned int _y);
extern void  PC98_color_txt(unsigned char _attr);
extern void  dos_print_doll(unsigned char *str);	/* INT 21H "...$" */
extern void  dos_print_char(unsigned char _c);	/* INT 21H putchar() */


/* グラフィック画面に関する関数 */
extern void  PC98_grph_cls(void);			/* ｸﾞﾗﾌｨｯｸｽ画面消去 */
extern void  PC98_16color(int sw);			/* 1:16色, 0:8色 ｸﾞﾗﾌｨｯｸｽ */
extern void  PC98_grph_mono(int sw);		/* 1:ﾓﾉｸﾛ, 0:ｶﾗｰ ｸﾞﾗﾌｨｯｸｽ */
extern void  color_palette_16(unsigned int pal_no,unsigned int _g,
                 unsigned int _r, unsigned int _b); /* ｶﾗｰﾊﾟﾚｯﾄ変更 */
extern void  PC98_boarder_color(int _col);	/* CONIO.H のカラー */
extern void  PC98_16color_palette_init(void);	/* ｶﾗｰﾊﾟﾚｯﾄを標準に初期化 */
extern void  PC98_BGI_16color_palette_init(void);	/* ｶﾗｰﾊﾟﾚｯﾄをBGIに初期化 */


/* その他の関数 */
extern void  key_buf_cls(void);			/* BIOS ﾜｰｸ領域のｷｰﾊﾞｯﾌｧをｸﾘｱ */


/* ANK -> S-JIS に変換する関数 */
extern void  sjis_num_prn(int _num);	/* 数字 : ANK -> S-JIS で画面に出力 */
extern unsigned int  _sjis_ipow(int _base, int _radix);
                 /* iのn乗整数版  i^n */
extern unsigned char  *sjis_str_cnv(unsigned char *_str, unsigned char *_dst);	/* 文字列 : ANK -> S-JIS を返す */
extern void  sjis_str_prn(unsigned char *str);	/* 文字列 : ANK -> S-JIS で画面に出力 */

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* _ETC_HEADER */
