/********************************************************************
 HDCPY.H  ���̨��/÷�� ʰ�޺�߰
 (C)opyright Hirokazu Inoue 1991,92
 Support Nif:GDH03306
********************************************************************/

#ifndef _HDCPY_HEADER
#define _HDCPY_HEADER

/* �ް��͊��ɐݒ肳��Ă��� */
extern char _hc_sC[]		/* ���̨��� ��߰ �װ 1:ON 0:OFF    */
extern int _hc_sX;		/* X �g�� 1:ON 0:OFF */
extern int _hc_sY;		/* Y �g�� 1:ON 0:OFF */
extern int _hc_sL;		/* �����W 0-79 */
extern int _hc_sT;		/* ����W 0-24 */
extern int _hc_sR;		/* �E���W 0-79 */
extern int _hc_sB;		/* �����W 0-24 */
extern int _hc_sTXT;		/* ÷��Ӱ�� 1:ON 0:OFF */
extern int _hc_sGR;		/* ���̨���Ӱ�� 1:ON 0:OFF */

#ifdef __cplusplus
extren "C"{
#endif

extern int hc_hardcopy(void);		/* �n�[�h�R�s�[ */

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* _HDCPY_HEADER */
