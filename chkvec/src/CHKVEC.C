/********************************************************************
 CHKVEC.C  Š„‚èž‚ÝÍÞ¸À° Áª¯¶°  Ver 1.00   for PC-9801
 (C)opyright 1992 ˆäã ”ŽŒv

 Turbo C/C++, Quick C, LSI-C  ‚ÅºÝÊß²Ù‰Â”\
 ’A‚µATC,QC,LC‚ÅINT 22,23,24h ‚ª QC,LC‚ÅINT 0 ‚É­X‚Ì•s‹ï‡‚ª‹N‚±‚é
  ‰Â”\«—L‚èBÃÞ°ÀÌ§²Ù‚ð‘¼‚ÌºÝÊß²×‚ÅºÝÊß²Ù‚µ‚½‚à‚Ì‚Æ‹¤—L‚·‚é‚Æ‚«‚ÍA
  ’ˆÓ‚µ‚½•û‚ª‚æ‚¢B
********************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>

#define	DATAFILENAME	"CHKVEC.DAT"	/* ÃÞÌ«ÙÄ‚ÌÃÞ°ÀÌ§²Ù–¼ */

#define	PARAM		0
#define	FILEOPEN	1
#define	FILEREAD	2
#define	FILEWRITE	3


void chkvec_main(char *datafile, int key_press_check);
int create_newfile(FILE *out, char *datafile);
void chk_vector(FILE *in);
void vec_error(int vec_no, long saved_vec, long cur_vec);
void fil_error(FILE *in, int no);
void verbose_datafile(char *datafile);
void error(int no);

#ifdef __TURBOC__
/* Turbo C ‚Å‚ÍˆÈ‰º‚ÌÍÞ¸À°‚ð‹N“®Žž‚É•Û‘¶ */
extern long _Int0Vector,_Int4Vector,_Int5Vector,_Int6Vector;
#endif /* __TURBOC__ */

int _error_exist;	/* ´×°‚ÌŒÂ”‚Ì¶³ÝÀ° */

void main(int ac, char *av[])
{
	int i, key_press_check = 1;
	char datafile[125], task_no = 'c';

	strcpy(datafile, DATAFILENAME);	/* ÃÞ°ÀÌ§²Ù–¼‚Ì‰Šú‰» */
	for(i=1; i<ac; i++)
	{	/* ½²¯Á‚Ì‰ðÍ‹y‚ÑÃÞ°ÀÌ§²Ù–¼‚ÌŽæ“¾ */
		if(av[i][0] == '-')
		{	/* ½²¯Á·¬×¸À° ‚Í '-' */
			switch(av[i][1]){
				case	'v'	:	/* ÍÞ¸ÀÃ°ÌÞÙ‚Ì•\Ž¦ */
					task_no = 'v';
					break;
				case	'k'	:	/* ‰½‚©·°‚ð‰Ÿ‚µ‚Ä‰º‚³‚¢‚ÌÁª¯¸‚ÌÈ—ª */
					key_press_check = 0;
					break;
				default		:	/* ‚»‚Ì‘¼‚Ì½²¯Á */
					error(PARAM);
			}
		}
		else strcpy(datafile, av[i]);	/* ½²¯ÁˆÈŠO‚ÍÃÞ°ÀÌ§²Ù–¼‚Æ”FŽ¯ */
	}

	/* ŽÀÛ‚ÌŽdŽ–‚É•ªŠò */
	switch(task_no)
	{
		case	'v'		:	/* ÍÞ¸ÀÃ°ÌÞÙ‚Ì•\Ž¦ */
			verbose_datafile(datafile);
			break;
		case	'c'		:	/* ÃÞÌ«ÙÄ‚Å‚ÍINT‚ÌŒŸ¸ */
			chkvec_main(datafile, key_press_check);
			break;
	}
	exit(0);	/* I—¹ */
}

/* CHKVEC ‚Ì ÍÞ¸À° CHECK Ù°ÁÝ */
void chkvec_main(char *datafile, int key_press_check)
{
	FILE *in;

	if((in = fopen(datafile, "rb")) == NULL)
	{	/* ÃÞ°ÀÌ§²Ù‚ªŒ©‚Â‚©‚ç‚È‚¢‚Æ‚«‚ÍV‚µ‚¢ƒtƒ@ƒCƒ‹‚ðì‚é */
		fclose(in);
		if(create_newfile(in, datafile))
			printf("Š„‚èž‚ÝÍÞ¸À° ÃÞ°ÀÌ§²Ù ‚ðì¬‚µ‚Ü‚µ‚½\n");
		else printf("ÃÞ°ÀÌ§²Ù ‚Ìì¬‚ð’†Ž~‚µ‚Ü‚µ‚½\n");

		return;
	}

	/* ŒŸ¸ƒ‹[ƒ`ƒ“ */
	_error_exist = 0;	/* ´×°ŒÂ”‚ð0ŒÂ‚É‰Šú‰» */
	printf("Š„‚èž‚ÝÍÞ¸À°‚ÌÁª¯¸’† ...\n");
	chk_vector(in);
	if(!_error_exist) printf("³í : ‘‚«Š·‚¦‚ç‚ê‚½ÍÞ¸À°‚Í‚ ‚è‚Ü‚¹‚ñ\n");
	else
	{
		printf("\x1b[4m%d ŒÂ‚ÌÍÞ¸À°‚ª‘‚«Š·‚¦‚ç‚ê‚Ä‚¢‚Ü‚·\x1b[m\n",_error_exist);
		if(key_press_check)
		{
			printf("\n‰½‚©·°‚ð‰Ÿ‚µ‚Ä‰º‚³‚¢\n");
			getch();		/* PC-9801ˆÈŠO‚Å‚ÍºÝÊß²Ù‚µ’¼‚¹‚ÎŽg—p‰Â”\ */
		}
	}

	return;
}

/* Œ»Ý‚ÌŠ„‚èž‚ÝÍÞ¸À°‚Ì•Û‘¶ */
int create_newfile(FILE *out, char *datafile)
{
	int i;
	unsigned long vec;

	printf("V‚µ‚¢ÃÞ°ÀÌ§²Ù %s ‚ðì‚Á‚Ä‚à‚¢‚¢‚Å‚·‚©  (y / n)\n", datafile);
	for(;;)
	{
		i = toupper(getch());
		if(i == 'Y') break;
		if(i == 'N') return 0;	/* ì¬‚µ‚È‚¢ */
	}

	if((out = fopen(datafile, "wb")) == NULL) fil_error(out, FILEOPEN);

	/* Š„‚èž‚ÝÍÞ¸À°‚Ì‘‚«ž‚Ý */
#ifdef __TURBOC__
	if(fwrite(&_Int0Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	for(i=1; i<= 3; i++)
	{
		vec = (long)getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
	if(fwrite(&_Int4Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	if(fwrite(&_Int5Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	if(fwrite(&_Int6Vector, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	for(i=7; i<= 0xff; i++)
	{
		vec = (long)getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
#else	/* TURBO C ˆÈŠO‚Å‚Í */
	for(i=0; i<= 0xff; i++)
	{
		vec = (long)_dos_getvect(i);
		if(fwrite(&vec, sizeof(long), 1, out) != 1) fil_error(out, FILEWRITE);
	}
#endif /* __TURBOC__ */

	fclose(out);
	return 1;	/* ÃÞ°ÀÌ§²Ù‚ðì¬‚µ‚½ */
}

/* Š„‚èž‚ÝÍÞ¸À°‚ÌŒŸ¸ */
void chk_vector(FILE *in)
{
	int i;
	unsigned long saved_vec,cur_vec;

	/* Š„‚èž‚ÝÍÞ¸À°‚Ì”äŠr */
#ifdef __TURBOC__
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int0Vector) vec_error(0, saved_vec, _Int0Vector);
	for(i=1; i<= 3; i++)
	{
		cur_vec = (long)getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int4Vector) vec_error(4, saved_vec, _Int4Vector);
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int5Vector) vec_error(5, saved_vec, _Int5Vector);
	if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
	if(saved_vec != _Int6Vector) vec_error(6, saved_vec, _Int6Vector);
	for(i=7; i<= 0xff; i++)
	{
		cur_vec = (long)getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
#else	/* TURBO C ˆÈŠO‚Å‚Í */
	for(i=0; i<= 0xff; i++)
	{
		cur_vec = (long)_dos_getvect(i);
		if(fread(&saved_vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		if(saved_vec != cur_vec) vec_error(i, saved_vec, cur_vec);
	}
#endif /* __TURBOC__ */
}

/* •ÏX‚³‚ê‚½ÍÞ¸À°‚Ì•\Ž¦ */
void vec_error(int vec_no, long saved_vec, long cur_vec)
{
	_error_exist++;		/* ¶³ÝÀ°‚ð²Ý¸ØÒÝÄ */
	printf("Š„‚èž‚ÝÍÞ¸À° %02X ”Ô‚ª•ÏX‚³‚ê‚Ä‚¢‚Ü‚·  %04X:%04X ¨ %04X:%04X\n",
		   vec_no,
		   (unsigned int)((saved_vec & 0xffff0000L) >> 16),
		   (unsigned int)(saved_vec & 0xffffL),
		   (unsigned int)((cur_vec & 0xffff0000L) >> 16),
		   (unsigned int)(cur_vec & 0xffffL) );
}

/* ÍÞ¸À° Ã°ÌÞÙ ‚Ì•\Ž¦ */
void verbose_datafile(char *datafile)
{
	FILE *in;
	int i;
	unsigned long vec;

	if((in = fopen(datafile, "rb")) == NULL) fil_error(in, FILEOPEN);
	printf("Š„‚èž‚Ý”Ô†  ¾¸ÞÒÝÄ:µÌ¾¯Ä\n"
		   "†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢\n");
	for(i=0; i<=0xff; i++)
	{	/* 1‚Â‚¸‚Â•\Ž¦ */
		if(fread(&vec, sizeof(long), 1, in) != 1) fil_error(in, FILEREAD);
		printf(" %02X             %04X:%04X\n",
			   i, (unsigned int)((vec & 0xffff0000L) >> 16),
			   (unsigned int)(vec & 0xffffL) );
	}
	fclose(in);

	return;
}

/* Ì§²Ù‚ð•Â‚¶‚Ä‚©‚çƒGƒ‰[ˆ—‚Ö */
void fil_error(FILE *in, int no)
{
	fclose(in);
	error(no);
}


/* ƒGƒ‰[ˆ— */
void error(int no)
{
	switch(no)
	{
		case	PARAM	:
			printf("Š„‚èž‚ÝÍÞ¸À° Áª¯¶°  Ver 1.00		(C) 1992 ˆäã ”ŽŒv\n\n"
				   "Žg—p–@ : CHKVEC (-vk \x1b[4mÃÞ°ÀÌ§²Ù–¼\x1b[m)\n\n");
			break;
		case    FILEOPEN	:
			printf("Ì§²Ù‚ªŒ©‚Â‚©‚ç‚È‚¢A–”‚Í“Ç‚Ýž‚ß‚Ü‚¹‚ñ\n\n");
			break;
		case	FILEREAD	:
			printf("Ì§²Ù‚ª“Ç‚ß‚Ü‚¹‚ñ\n\n");
			break;
		case	FILEWRITE	:
			printf("Ì§²Ù‚ª‘‚¯‚Ü‚¹‚ñ\n\n");
			break;
	}
	exit(1);	/* ˆÙíI—¹ */
}
