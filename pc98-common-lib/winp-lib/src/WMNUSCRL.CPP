/*********
スクロールメニュー選択
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
 *選択スクロール
 *
 *主なグローバルデータ
 *	int *m->cur_topic	:	現在反転している番号	(1 ~ Max_Topic)
 *	int *m->cur_line	:	現在反転している行番号　	(1 ~ Max_Line)
 *	int m->data_max	:	最大メニュー数	(1 ~ )
 *	int *m->win_line	:	表示行数	(1 ~ )
 *	int updn			:	カーソル上下   1 : DOWN Key  , -1 : UP Key
 *
 *
 ****************/

	if(m->init_window == W_FWRITE)
	{
		// ウインドウ　メイク
		win_mmake(m->left,m->top,m->left + m->win_len,m->top + m->win_line - 1,m->boarder_color,W_RECT,W_PAGE_1);
		clrscr();
		_wscroll = 0;
		// ウインドウ内に初期データを書く
		int i;
		textattr(m->color);
		for(i=1;i<=m->win_line && (m->cur_topic-m->cur_line+i)<=m->data_max;i++){
			gotoxy(1,i);
			_win_mnu_scroll_dspdt(m->cur_topic-m->cur_line+i);
		}
		// カレントデータを反転
		gotoxy(1,m->cur_line);
		textattr(m->rev_color);
		_win_mnu_scroll_dspdt(m->cur_topic);

		return;
	}
	else if(m->init_window == W_CLOSE)
	{
		// ウインドウ　クローズ
		win_mdelete(m->left,m->top,m->left + m->win_len,m->top + m->win_line - 1);
		return;
	}


	// 現在行のアトリビュートを消す
	gotoxy(1,m->cur_line);
	textattr(m->color);
	_win_mnu_scroll_dspdt(m->cur_topic);

	if(updn == -1){
		/* UP キー */
		if(m->cur_line == 1){
			// 最上段の時
			if(m->cur_topic== 1){
				// データ１＆最上段　　そのまま
				gotoxy(1,1);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
			else {
				// データデクリメント
				m->cur_topic--;
				gotoxy(1,1);
				insline();
				gotoxy(1,1);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
		}
		else {
			// データデクリメント＆ラインデクリメント
			m->cur_topic--;
			m->cur_line--;
			gotoxy(1,m->cur_line);
			textattr(m->rev_color);
			_win_mnu_scroll_dspdt(m->cur_topic);
		}
	}
	else if(updn == 1){
		/* DOWN キー */
		if(m->cur_topic == m->data_max){
			// 最大データの時　　そのまま
			gotoxy(1,m->cur_line);
			textattr(m->rev_color);
			_win_mnu_scroll_dspdt(m->cur_topic);
			return;
		}
		if(m->cur_line == m->win_line){
			// 最下段の時
			if(m->cur_topic== m->data_max){
				// 再下段＆最大データ　　そのまま
				gotoxy(1,m->win_line);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
			else {
				// データインクリメント
				m->cur_topic++;
				gotoxy(1,1);
				delline();
				gotoxy(1,m->win_line);
				textattr(m->rev_color);
				_win_mnu_scroll_dspdt(m->cur_topic);
			}
		}
		else {
			// データインクリメント＆ラインインクリメント
			m->cur_topic++;
			m->cur_line++;
			gotoxy(1,m->cur_line);
			textattr(m->rev_color);
			_win_mnu_scroll_dspdt(m->cur_topic);
		}
	}

}

/***********
このような関数が必要
void _win_mnu_scroll_dspdt(int _cur_topic_no)
{
	cprintf("%-20s",topic[_cur_topic_no-1]);

}
*************/