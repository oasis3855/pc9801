/********************************************************************
 HDCPY.H  ｸﾞﾗﾌｨｯｸ/ﾃｷｽﾄ ﾊｰﾄﾞｺﾋﾟｰ
 (C)opyright Hirokazu Inoue 1991,92
 Support Nif:GDH03306
********************************************************************/

#ifndef _HDCPY_HEADER
#define _HDCPY_HEADER

/* ﾃﾞｰﾀは既に設定されている */
extern char _hc_sC[]		/* ｸﾞﾗﾌｨｯｸｽ ｺﾋﾟｰ ｶﾗｰ 1:ON 0:OFF    */
extern int _hc_sX;		/* X 拡大 1:ON 0:OFF */
extern int _hc_sY;		/* Y 拡大 1:ON 0:OFF */
extern int _hc_sL;		/* 左座標 0-79 */
extern int _hc_sT;		/* 上座標 0-24 */
extern int _hc_sR;		/* 右座標 0-79 */
extern int _hc_sB;		/* 下座標 0-24 */
extern int _hc_sTXT;		/* ﾃｷｽﾄﾓｰﾄﾞ 1:ON 0:OFF */
extern int _hc_sGR;		/* ｸﾞﾗﾌｨｯｸｽﾓｰﾄﾞ 1:ON 0:OFF */

#ifdef __cplusplus
extren "C"{
#endif

extern int hc_hardcopy(void);		/* ハードコピー */

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* _HDCPY_HEADER */
