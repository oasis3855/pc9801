/*********************************************************************
   TXT_INP.CPP  for convinience to input
   This program should compiled in Turbo C++
   Original Copyright (C) Borland Intl.
   modified by Hirokazu Inoue 1991
**********************************************************************/

#define _TXT_INP_MAIN
#include "txt_inp.h"


int txt_jinput(char *str, int str_len, char *old_str,
	int color, int x, int y)

/*	Input String / Edit String
	str	:	string buffer , give / return
	str_len	:	string length
	old_str	:	edit string , if only input ""
	color	:	edit char color
	x,y	:	CONIO.H window (1,1)-(maxX,maxY)

	return	0	:	OK
			K_UP	:	UP KEY
			K_DN	:	DOWN KEY
			K_TAB:	TAB KEY
			K_ESC:	ESCAPE KEY

	Accorde to 2 Byte JAPANESE character set
*/

{
	int retcode;

	if(x <= 0 || x >= 81) x = wherex();
	if(y <= 0 || y >= 26) y = wherey();

	txt_chgkey(0x000c,fkey_sav);
	txt_chgkey(0x000d,fkey_def);
	if(_txt_jinp_up == 0) txt_chgerrkey(0x0019);
	if(_txt_jinp_down == 0) txt_chgerrkey(0x001C);
	signal(SIGINT,SIG_IGN);
	signal(SIGABRT,SIG_IGN);

	if( (strlen(old_str) != 0)&&(strlen(old_str) <= str_len) ){
		strcpy(str,old_str);
	}
	else { str[0] = 0; }

	retcode = txt_jinp_edit(str,"",str_len,color,x,y);

	signal(SIGINT,SIG_DFL);
	signal(SIGABRT,SIG_DFL);
	txt_chgkey(0x000d,fkey_sav);

	return(retcode);


} /* getinput */



int txt_jinp_edit(char *s, const char *legal, const int maxlength,
	 const int text_color, const int mes_x, const int mes_y)
/* Allows the user to edit a string with only certain characters allowed -
   0 : succeed , UPKEY,DOWNKEY,ESC : fail
*/
{
 int c, len = strlen(s), pos = len, insert = TRUE;

 int skip, c2, w;		/* character width */
 int retcode = 0;
 char *output_str;

 if(coreleft() <= (unsigned int)maxlength*2) return(NO_MEM);

 output_str = (char *)malloc(sizeof(char)*(maxlength+5));

 skip = 1;
 if (pos == 1) {
  if (iskanji(c = (unsigned char)s[0])) { skip = 0; pos = len = 0; }
 }

 do
 {

  if (skip) {
   _setcursortype(_NOCURSOR);	/* disable cursor */

   txt_outp(mes_x, mes_y, text_color, maxlength, s, output_str);
   txt_chgcursor(insert);	/* enable cursor */
   gotoxy(txt_fontsize(s, pos) + mes_x, mes_y);
  }

  if (skip) { c = txt_read_key(); } skip = 1;
  switch(c)
  {
   case HOMEKEY :
    pos = 0;
    break;
   case ENDKEY :
    pos = len;
    break;
   case INSKEY :
    insert = !insert;
    txt_chgcursor(insert);
    break;
   case LEFTKEY :
    if (pos > 0)
    {
     pos--;

	if (nthctype(s, pos) == CT_KJ2) pos--;

    }
    else txt_jinp_error();
    break;
   case RIGHTKEY :
    if (pos < len)
    {
     pos++;

	if (nthctype(s, pos) == CT_KJ2) pos++;
    }
    else txt_jinp_error();
    break;
   case BS :
    if (pos > 0)
    {

	if (nthctype(s, pos - 1) == CT_KJ2) w = 2; else w = 1;
	movmem(&s[pos], &s[pos - w], len - pos);
	pos -= w;
	len -= w;

    }
    else txt_jinp_error();
    break;
   case DELKEY :
    if (pos < len)
    {

	if (nthctype(s, pos) == CT_KJ1) w = 2; else w = 1;
	movmem(&s[pos + w], &s[pos], len - pos - w);
	len -= w;

    }
    else txt_jinp_error();
    break;
   case CR :
    retcode = 0;
    break;
   case UPKEY :
    c = CR;
    retcode = K_UP;
    break;
   case DOWNKEY :
    c = CR;
    retcode = K_DN;
    break;
   case ESC :
    len = 0;
    retcode = K_ESC;
    break;
   case K_TAB :
    if(_txt_jinp_tab == 1){
	c = CR;
	retcode = K_TAB;
    }
    break;
   case ER_KEY :
    txt_jinp_error();
    break;
   default :

    if ((c & 0xFF00) == 0x7F00) break;	/* skipout FUNCTION key */
    w = 1; if (iskanji(c)) { c2 = txt_read_key(); w = 2; }
    if (((legal[0] == 0) || (strchr(legal, c) != NULL)) && (c >= ' ')) {
     if (insert) {
	 if (len + w > maxlength) {
		txt_jinp_error();
		break;
	 }
      memmove(&s[pos + w], &s[pos], len - pos);
      len += w;
	}
	else {
	 if (pos >= len) {
	  if (len + w > maxlength) {
		txt_jinp_error();
		break;
	  }
	  len += w;
	 }
	 else {
	  if (w == 2 && pos + 1 >= len) {
	    if (len + 1 > maxlength) break;
	    len++;
	  }
	  else {
	   unsigned int	off;
	   off = pos + w - 1;
	   if (nthctype(s, off) == CT_KJ1) {
	    movmem(&s[off + 2], &s[off + 1], len - off - 1);
	    len--;
	   }
	  }
      }
     }
     s[pos++] = c;
     if (w == 2) s[pos++] = c2;
    }	/* if legal ? */
    else txt_jinp_error();
    break;
  } /* switch */
  s[len] = 0;
 }
 while ((c != CR) && (c != ESC));
 txt_chgcursor(FALSE);
 _setcursortype(_NOCURSOR);
 return(retcode);
} /* editstring */


unsigned int txt_fontsize(char *s, unsigned int nbyte)
/* Return number of font size in first <nbytes> of <s> */
{
 unsigned int	 byte = 0, font = 0;
 int  mode = CT_ANK;
 unsigned char	 c;
 unsigned int	 ch;

 while (byte < nbyte && (c = s[byte]) != 0) {
  if (mode == CT_KJ1) {
   mode = chkctype(c, 1);
  } else {
   mode = chkctype(c, 0);
  }
  if (mode == CT_ILGL) break;
  if (mode == CT_KJ2) {
   ch = ((unsigned char) s[byte - 1] << 8) | c;
   if (!is_half_of_kanji(ch)) font++;
  } else {
   font++;
  }
  byte++;
 }
 return font;
}


void txt_outp(int col, int row, int color, int width, char *str, char *buf)
/* Prints a string in video memory at a selected location in a color */
{

 int len;

 strcpy(buf,str);
 buf[width] = 0;
 if ((len = strlen(buf)) < width)
  setmem(&buf[len], width - len, ' ');

 textcolor(color);
 gotoxy(col, row);
 cputs(buf);
} /* txt_outp */



void txt_chgcursor(int insmode)
/* Changes the cursor shape based on the current insert mode */
{
 if (insmode)
  _setcursortype(_LINECURSOR);
 else
  _setcursortype(_SOLIDCURSOR);
} /* txt_chgcursor */

int is_half_of_kanji(unsigned short int c)
/* Test for half font of kanji */
{
 if (c >= 0x8540 && c <= 0x869E) return 1; return 0;
}



int txt_read_key(void)
/* Uses the BIOS to read the next keyboard character */
{
 unsigned int	 key;

 key = getch();				/* get key */
 if( key == 0x7F ) { key = ( key << 8 ) | getch(); }
 return( key );
} /* txt_read_key */


void txt_chgkey(int func, unsigned char *tab)
/* Expanded BIOS of Keys for PC-9801 only */
{
 union REGS		 ireg, oreg;
 struct SREGS		 sreg;
 unsigned char far	*ftab;
 int			 i;
 ftab = tab;
 ireg.x.cx = func;		/* BIOS function code */
 ireg.x.dx = FP_OFF(ftab);
 sreg.ds   = FP_SEG(ftab);
 ireg.x.ax = 0x0015;		/* CURSOR key code */
 for(i = 0; i < 11; i++) {
  int86x(0XDC, &ireg, &oreg, &sreg);
  ireg.x.ax++;
  ireg.x.dx += 6;
 }
}


void txt_chgerrkey(int key)
/* Expanded BIOS of Keys for PC-9801 only                    *
 * key  0x0015 RLUP , 0x0016 RLDN , 0x0017 INS , 0x0018 DEL  *
 *      0x0019 UP , 0x001A LFT , 0x001B RGT , 0x001C DN      *
 *      0x001D CLR , 0x001E HLP , 0x001F HM                  */
{
 union REGS		 ireg, oreg;
 struct SREGS		 sreg;
 unsigned char err_key[7] = {0x7F, ER_KEY LMK, 0, 0, 0, 0};

 ireg.x.cx = 0x000D;		/* BIOS function code : set key */
 ireg.x.dx = FP_OFF(err_key);
 sreg.ds   = FP_SEG(err_key);
 ireg.x.ax = key;
 int86x(0x00DC, &ireg, &oreg, &sreg);

}



void txt_jinp_error()
{
    if(_txt_jinp_beep == 1)
    {
	sound(_txt_jinp_bptone);
	delay(_txt_jinp_bplen);
	nosound();
    }
}

/* end of TXT_INP.CPP */
