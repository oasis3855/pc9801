/* ANK ƒXƒgƒŠƒ“ƒO@‚ğ@‚r‚ˆ‚‰‚†‚”|‚i‚h‚r@‚QƒoƒCƒgƒR[ƒh‚Åo—Í‚·‚éˆ×‚Ì
   ŠÖ”ŒS

   Ver 1.0   1991.07.11 Hirokazu Inoue
*/
#include<stdio.h>
#include<jctype.h>
#include<dos.h>

void sjis_num_prn(int _num);
int _sjis_ipow(int _base, int _radix);
char *sjis_str_cnv(char *_str, char *_dst);
void dos_print_char(unsigned char _c);
void sjis_str_prn(unsigned char *str);

void sjis_num_prn(int _c)
{
	int c,i,max;
	unsigned code;

	if(_c < 0)
	{
		/* •‰‚Ì”‚Ìˆ— */
		c = 0x817c;	/* | */
		dos_print_char(c >> 8);
		dos_print_char((unsigned char)c);
		_c = -_c;
	}

	i = _c;

	/* Œ…”‚ğ‹‚ß‚é 1 - */
	max = 1;
	for(;;)
	{
		i /= 10;
		if(i == 0) break;
		max++;
	}

	i = _c;

	do{
		/* ã‚ÌŒ…‚æ‚è‡”Ô‚Éæ‚èo‚µ‚ÄA‚P•¶š‚¸‚ÂƒvƒŠƒ“ƒg */
		c = _c/(_sjis_ipow(10,max-1));
		_c = _c - c*_sjis_ipow(10,max-1);
		c = (unsigned char)(c+0x30);	/* to char */
		code = hantozen(c);
		if ((code >> 8) != 0) dos_print_char(code >> 8);
		dos_print_char((unsigned char)code);
		max--;
	}while(max >= 1);
}

int _sjis_ipow(int base, int radix)
{
	/* ‚d‚l‚t‚ğg—p‚µ‚È‚¢@‚˜‚Ì‚æ */
	int i,j=1;
	if(radix == 0) return 1;		/* x^0 = 1 */
	for(i=1;i<=radix;i++) j *= base;
	return j;
}

void sjis_str_prn(unsigned char *str)
{
	int c,i;

	for(i=0;*(str+i)!=NULL;i++)
	{
		c = hantozen((int )*(str+i));
		if ((c >> 8) != 0) dos_print_char(c >> 8);
		dos_print_char((unsigned char)c);
	}
	return;
}


char *sjis_str_cnv(char *str, char *dst)
{
	/* ‚`‚m‚j‚PƒoƒCƒg•¶š‚ğ‚r|‚i‚h‚r‚QƒoƒCƒg•¶š‚É•ÏŠ· */
	int c,i;

	for(i=0;*(str+i)!=NULL;i++)
	{
		c = (int )*(str+i);
		*(dst+i*2+1) = (char )( hantozen(c) & 0x00ff);
		*(dst+i*2  ) = (char )((hantozen(c) & 0xff00) >> 8);
	}
	*(dst+i*2) = NULL;

	return dst;
}

void dos_print_char(unsigned char _c)
{
	/* ‚P•¶š‚ÌƒXƒNƒŠ[ƒ“o—Í */
	_DL = _c;
	_AX = 0x0200;
	geninterrupt(0x21);
	return;
}
