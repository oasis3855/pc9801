/*********
�X�N���[�����j���[�I��
*********/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include"winp.h"

/*********
typedef struct{
	int left;
	int top;
	int win_len;	// Window Width  1 ~
	int win_line;	// Window Lwngth 1 ~
	int data_max;	// Max Menu no
	int color;
	int rev_color;
	int boarder_color;
	int cur_line;	// current reverse line 1 ~
	int cur_topic;	// current selected topic 1 ~
	int init_window;	// W_FWRITE : MAKE , W_CLOSE : DELETE , OTHER : Do Menu Scroll
} SCROLL_MNU_INFO;
***********/

void win_mnu_scroll(int updn, SCROLL_MNU_INFO *m);
extern void _win_mnu_scroll_dspdt(int _cur_topic_no);


void win_mnu_scroll(int updn, SCROLL_MNU_INFO *m)
{
/****************
 *�I���X�N���[��
 *
 *��ȃO���[�o���f�[�^
 *	int *m->cur_topic	:	���ݔ��]���Ă���ԍ�	(1 ~ Max_Topic)
 *	int *m->cur_line	:	���ݔ��]���Ă���s�ԍ��@	(1 ~ Max_Line)
 *	int m->data_max	:	�ő僁�j���[��	(1 ~ )
 *	int *m->win_line	:	�\���s��	(1 ~ )
 *	int updn			:	�J�[�\���㉺   1 : DOWN Key  , -1 : UP Key
 *
 *
 ****************/

	if(m->init_window == W_FWRITE)
	{
		// �E�C���h�E�@���C�N
		win_mmake(m->left,m->top,m->left + m->win_len,m->top + m->win_line - 1,m->boarder_color,W_RECT,W_PAGE_1);
		clrscr();
		_wscroll = 0;
		// �E�C���h�E���ɏ����f�[�^������
		int i;
		textattr(m->color);
		for(i=1;i<=m->win_line && (m->cur_topic-m->cur_line+i)<=m->data_max;i++){
			gotoxy(1,i);
			_win_mnu_scroll_dspdt(m->cur_topic-m->cur_line+i);
		}
		// �J�����g�f�[�^�𔽓]
		gotoxy(1,m->cur_line);
		textattr(m->rev_color);
		_win_mnu_scroll_dspdt(m->cur_topic);

		return;
	}
	else if(m->init_window == W_CLOSE)
	{
		// �E�C���h�E�@�N���[�Y
		win_mdelete(m->left,m->top,m->left + m->win_len,m->top + m->win_line - 1);
		return;
	}


	// ���ݍs�̃A�g���r���[�g������
	gotoxy(1,m->cur_line);
	textattr(m->color);
	_win_mnu_scroll_dspdt(m->cur_topic);

	if(updn == -1){
		/* UP �L�[ */
		if(m->cur_line == 1){
			// �ŏ�i�̎�
			if(m->cur_topic== 1){
				// �f�[�^�P���ŏ�i�@�@���̂܂�
				gotoxy(1,1);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
			else {
				// �f�[�^�f�N�������g
				m->cur_topic--;
				gotoxy(1,1);
				insline();
				gotoxy(1,1);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
		}
		else {
			// �f�[�^�f�N�������g�����C���f�N�������g
			m->cur_topic--;
			m->cur_line--;
			gotoxy(1,m->cur_line);
			textattr(m->rev_color);
			_win_mnu_scroll_dspdt(m->cur_topic);
		}
	}
	else if(updn == 1){
		/* DOWN �L�[ */
		if(m->cur_topic == m->data_max){
			// �ő�f�[�^�̎��@�@���̂܂�
			gotoxy(1,m->cur_line);
			textattr(m->rev_color);
			_win_mnu_scroll_dspdt(m->cur_topic);
			return;
		}
		if(m->cur_line == m->win_line){
			// �ŉ��i�̎�
			if(m->cur_topic== m->data_max){
				// �ĉ��i���ő�f�[�^�@�@���̂܂�
				gotoxy(1,m->win_line);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
			else {
				// �f�[�^�C���N�������g
				m->cur_topic++;
				gotoxy(1,1);
				delline();
				gotoxy(1,m->win_line);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
		}
		else {
			// �f�[�^�C���N�������g�����C���C���N�������g
			m->cur_topic++;
			m->cur_line++;
			gotoxy(1,m->cur_line);
			textattr(m->rev_color);
			_win_mnu_scroll_dspdt(m->cur_topic);
		}
	}

}

/***********
���̂悤�Ȋ֐����K�v
void _win_mnu_scroll_dspdt(int _cur_topic_no)
{
	cprintf("%-20s",topic[_cur_topic_no-1]);

}
*************/