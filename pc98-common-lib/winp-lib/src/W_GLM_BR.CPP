/* use GLIO.LIB */
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include <string.h>
#include<alloc.h>
#include<etc.h>
#include<winp.h>

// extend menu system
int mnu_menubar(GMNUBAR_INFO *mnu, char *str[], int *x_topic, char *title, int x_title)
{

	int add,cur_data,i,sub_mnu_on = 0,key;

	for(;;)
	{
		/* �ƭ��ް��`�� */
		window(1,1,80,25);
		gotoxy(1,1);
		textattr(mnu->color);
		cprintf("%80s"," ");
		gotoxy(x_title,1);
		cprintf("%s",title);
		/* �ƭ����ڂ�`�� */
		textattr(mnu->topic_color);
		for(i=0;i<mnu->data_max;i++)
		{
			gotoxy(*(x_topic+i),1);
			cprintf("%s",*(str+i));
		}
		for(;;)
		{	/* GRPH �� F10 ��҂� */
			_AX = 0x040e;		/* GRPH ������ */
			geninterrupt(0x18);
			if(_AH & 0x08) break;
			_AX = 0x040d;		/* F10 ������ */
			geninterrupt(0x18);
			if(_AH & 0x08) break;
			key_buf_cls();
		}
		for(;;)
		{
			/* ���� ��߯� �𔽓] */
			window(1,1,80,25);
			textattr(mnu->select_color);
			gotoxy(*(x_topic + mnu->cur_topic - 1), 1);
			cprintf("%s",*(str + mnu->cur_topic - 1));

			for(;;)
			{	/* ESC �� Down �L�[��҂� */
				if(sub_mnu_on) break;			/* ���Ƀ��j���[���~��Ă��� */
				key = mnu_wread_key() & 0xff00;
				if(key == 0x0000) break;		/* ESC �� */
				else if(key == 0x3d00)			/* �� �� */
				{
					sub_mnu_on = 1;
					break;
				}
				else if(key == 0x3b00) break;	/* �� �� */
				else if(key == 0x3c00) break;	/* �� �� */
				key_buf_cls();
			}
			if(key == 0x0000) break;	/* ESC �������甲���� */
			if(sub_mnu_on)
			{
				i = mnu->mfunc(mnu->cur_topic);		/* �ƭ��֐����s */
				window(1,1,80,25);
				if(i == 0x1b)
				{
					sub_mnu_on = 0;
					break;	/* ESC Key */
				}
			}
			/* ���� ��߯� �����ǂ���̐F�� */
			textattr(mnu->topic_color);
			gotoxy(*(x_topic + mnu->cur_topic - 1), 1);
			cprintf("%s",*(str + mnu->cur_topic - 1));
			if(i == 0x1c || key == 0x3c00)
			{	/* �� �� */
				mnu->cur_topic++;
				if(mnu->cur_topic > mnu->data_max) mnu->cur_topic = 1;
			}
			else if(i == 0x1d || key == 0x3b00)
			{	/* �� �� */
				mnu->cur_topic--;
				if(mnu->cur_topic <= 0) mnu->cur_topic = mnu->data_max;
			}
		}
	}
	return 0;

}

void mnu_barmake(GMNUBAR_INFO *mnu, char *str[], int *x_topic, char *title, int x_title)
{

	int i;

	/* �ƭ��ް��`�� */
	window(1,1,80,25);
	gotoxy(1,1);
	textattr(mnu->color);
	cprintf("%80s"," ");
	gotoxy(x_title,1);
	cprintf("%s",title);
	/* �ƭ����ڂ�`�� */
	textattr(mnu->topic_color);
	for(i=0;i<mnu->data_max;i++)
	{
		gotoxy(*(x_topic+i),1);
		cprintf("%s",*(str+i));
	}
}