/* XYGRMINx.CPP
   XYGRPH.EXE �p�@�v�Z�^�\�� */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<process.h>
#include<math.h>
#include<conio.h>
#include<etc.h>
#include<glio.h>
#include<gr.h>
#include<exmath.h>
#include"xygr.h"

/* ���������_�G���[�ւ̑Ή� */
int matherr(struct exception *e)
{
	asm	fninit;
	math_err_flag = 1;
	e->retval = 0.0;
	return(1);
}

/* �O���t�B�b�N�p���b�g�̕ύX */
void chg_palette(void)
{
	int i;

	switch(cur_palette)
	{
		case	1	:	/* 16�F */
			PC98_BGI_16color_palette_init();
			break;
		case	2	:	/* �΂P�U�~�� */
			for(i=0;i<=15;i++)
			{
				color_palette_16(15-i,i,0,0);
			}
			color_palette_16(0,0,0,0);
			break;
		case	3	:	/* ���m�N�� */
			for(i=0;i<=15;i++)
			{
				color_palette_16(15-i,i,i,i);
			}
			color_palette_16(0,0,0,0);
			break;
		case	4	:	/* �v���r���[ */
			for(i=0;i<15;i++) color_palette_16(i+1,12,12,12);
			color_palette_16(0, 0,0,0);
			color_palette_16(8, 0,0,0);
			break;
		default	:
			break;
	}
	return;
}

/* ������͂��Ēl��Ԃ��^���̓��j���[�p ,parser��atof*/
double calc_str(char *str)
{
	int dummy;
	static double variables[30];	/* �p�[�T�p�̕ϐ� */
	double ret;

	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	ret = parser(str,&dummy,variables,26);
	if(dummy == 0) return 0.0;	/* ���̃G���[ */

	return ret;
}


/* �O���t��`�� */
void write_graph(int mode)
{
	if(sw_initmode == 1 || sw_initmode == 3) write_window();
	if(sw_initmode == 1 || sw_initmode == 2) write_scale();

	switch(mode)
	{
		case	1	:	/* ������(��) */
			write_graph_x();
			break;
		case	2	:	/* ���C������(��) */
			write_graph_t();
			break;
		case	3	:	/* ����d��(��)/d�� */
			write_graph_d();
			break;
		default		:
			break;
	}
	key_buf_cls();
}

/* ������(��) �̃O���t��`�� */
void write_graph_x(void)
{
	int i,dummy,sv_line;
	double variables[30];	/* �p�[�T�p�̕ϐ� */
	int x;					/* CRT ��ł̍��W */
	double LogicX,LogicY;	/* ���ۂ̎����l */
	double old_LogicY;		/* �����⊮���̂P�O�̂����W */

	set_param();

	/* ���̃G���[�̌��� */
	for(i = 0;i <= 25;i++) variables[i] = 0;
	variables['X'-'A'] = x_min;
	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	parser(formula_string,&dummy,variables,26);
	if(dummy == 0) return;	/* ���̃G���[ */

	sv_line = 0;	/* �����⊮�p */
	math_err_flag = 0;	/* ���������_�t���O */

	for(x=0; x<=Xwidth; x++)
	{
		if(kbhit() && getch() == 0x1b) return;	/* ESC �`�F�b�N */
		LogicX = (double)x / x_ratio + x_min;	/* ���ۂ̂����W */
		if(sw_bod == 2)
		{
			if(LogicX < xl_min || LogicX > xl_max)
			{	/* �ް�ނ̊O */
				sv_line = 0;
				continue;
			}
		}
		*(variables + 0x17) = LogicX;
		LogicY = parser(formula_string,&dummy,variables,26);
		if(LogicY > y_max || LogicY < y_min || math_err_flag)
		{
			sv_line = 0;
			math_err_flag = 0;	/* ���������_�G���[ */
			continue;	/* ����޳�O */
		}
		if(sw_bod == 2)
		{
			if(LogicY < yl_min || LogicY > yl_max)
			{	/* �ް�ނ̊O */
				sv_line = 0;
				continue;
			}
		}

		if(sw_if == 2)
		{	/* if ���̌��� */
			variables['Y'-'A'] = LogicY;
			if(parser(formula_str_if,&dummy,variables,26) <= 0 || math_err_flag)
			{	/* if �����O�ȉ��������� */
				sv_line = -1;
				math_err_flag = 0;	/* ���������_�G���[ */
				continue;
			}
		}

		gpset(x+xw_min,yw_max-(LogicY - y_min)*y_ratio,_c_gr_grph);

		if(sw_gr_line == 2 && sv_line)
		{	/* �����⊮ */
			gline(x-1+xw_min, yw_max-(old_LogicY - y_min)*y_ratio,
				  x+xw_min, yw_max-(LogicY - y_min)*y_ratio,
				  _c_gr_grph);
			old_LogicY = LogicY;
			sv_line = 1;	/* �t���O */
		}
		else if(sw_gr_line == 2)
		{
			old_LogicY = LogicY;
			sv_line = 1;	/* �t���O */
		}


	}
	return ;
}

/* ���A������(��) �̃O���t��`�� */
void write_graph_t(void)
{
	int i,dummy,sv_line;
	double variables[30];	/* �p�[�T�p�̕ϐ� */
	double LogicX,LogicY,t;	/* ���ۂ̎����l */
	double old_LogicX,old_LogicY;		/* �����⊮���̂P�O�̍��W */

	set_param();

	/* ���̃G���[�̌��� */
	for(i = 0;i < 26;i++) variables[i] = 0;
	variables['T'-'A'] = tl_min;
	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	parser(formula_str_x,&dummy,variables,26);
	if(dummy == 0) return;
	parser(formula_str_y,&dummy,variables,26);
	if(dummy == 0) return;

	sv_line = 0;	/* �����⊮�p */

	for(t = tl_min;t <= tl_max;t += t_delta){
		if(kbhit() && getch() == 0x1b) return;	/* ESC �`�F�b�N */
		variables['T'-'A'] = t;
		LogicX = parser(formula_str_x,&dummy,variables,26);
		if(LogicX < x_min || LogicX > x_max || math_err_flag)
		{	/* �E�C���h�E�̊O */
			sv_line = 0;
			math_err_flag = 0;	/* ���������_�G���[ */
			continue;
		}
		if(sw_bod == 2)
		{
			if(LogicX < xl_min || LogicX > xl_max)
			{	/* �ް�ނ̊O */
				sv_line = 0;
				continue;
			}
		}

		LogicY = parser(formula_str_y,&dummy,variables,26);
		if(LogicY > y_max || LogicY < y_min || math_err_flag)
		{	/* ����޳�O */
			sv_line = 0;
			math_err_flag = 0;	/* ���������_�G���[ */
			continue;
		}
		if(sw_bod == 2)
		{
			if(LogicY < yl_min || LogicY > yl_max)
			{	/* �ް�ނ̊O */
				sv_line = 0;
				continue;
			}
		}

		if(sw_if == 2)
		{	/* if ���̌��� */
			variables['X'-'A'] = LogicX;
			variables['Y'-'A'] = LogicY;
			if(parser(formula_str_if,&dummy,variables,26) <= 0 || math_err_flag)
			{	/* if �����O�ȉ��������� */
				sv_line = -1;
				math_err_flag = 0;	/* ���������_�G���[ */
				continue;
			}
		}

		gpset((LogicX-x_min)*x_ratio+xw_min,
			  yw_max-(LogicY - y_min)*y_ratio, _c_gr_grph);

		if(sw_gr_line == 2 && sv_line)
		{	/* �����⊮ */
			gline((old_LogicX-x_min)*x_ratio+xw_min, yw_max-(old_LogicY - y_min)*y_ratio,
				  (LogicX-x_min)*x_ratio+xw_min, yw_max-(LogicY - y_min)*y_ratio,
				  _c_gr_grph);
			old_LogicX = LogicX;
			old_LogicY = LogicY;
			sv_line = 1;	/* �t���O */
		}
		else if(sw_gr_line == 2)
		{
			old_LogicX = LogicX;
			old_LogicY = LogicY;
			sv_line = 1;	/* �t���O */
		}

	}
	return ;
}

/* ����d��(��)/d�� �̃O���t��`�� */
void write_graph_d(void)
{
	int i,dummy,sv_line;
	double variables[30];	/* �p�[�T�p�̕ϐ� */
	int x;					/* CRT ��ł̍��W */
	double LogicX,LogicY,LogicDY;	/* ���ۂ̎����l */
	double DeltaY;			/* �����l */
	double dx;				/* ���̊Ԋu */
	double old_DeltaY;		/* �����⊮���̂P�O�̍��W */

	set_param();

	/* ���̃G���[�̌��� */
	for(i = 0;i <= 25;i++) variables[i] = 0;
	variables['X'-'A'] = x_min;
	variables['P'-'A'] = M_PI;
	variables['E'-'A'] = M_E;
	parser(formula_str_dfx,&dummy,variables,26);
	if(dummy == 0) return;	/* ���̃G���[ */

	sv_line = 0;	/* �����⊮�p */

	dx = 1.0 / x_ratio;		/* �h�b�g�Ԃ̂��̊Ԋu */
	if(dx <= 0.0) return;	/* ���Z�̃G���[�ƂȂ� */

	for(x=0; x<=Xwidth; x++)
	{
		if(kbhit() && getch() == 0x1b) return;	/* ESC �`�F�b�N */
		LogicX = (double)x / x_ratio + x_min;	/* ���ۂ̂����W */
		if(sw_bod == 2)
		{
			if(LogicX < xl_min || LogicX > xl_max)
			{	/* �ް�ނ̊O */
				sv_line = 0;
				continue;
			}
		}
		*(variables + 0x17) = LogicX;
		LogicY = parser(formula_str_dfx,&dummy,variables,26);
		if(math_err_flag)
		{
			sv_line = 0;
			math_err_flag = 0;	/* ���������_�G���[ */
			continue;
		}
		*(variables + 0x17) = LogicX + dx;
		LogicDY = parser(formula_str_dfx,&dummy,variables,26);
		if(math_err_flag)
		{
			sv_line = 0;
			math_err_flag = 0;	/* ���������_�G���[ */
			continue;
		}
		DeltaY = (LogicDY - LogicY) / dx;
		if(DeltaY > y_max || DeltaY < y_min)
		{
			sv_line = 0;
			continue;	/* ����޳�O */
		}
		if(sw_bod == 2)
		{
			if(DeltaY < yl_min || DeltaY > yl_max)
			{	/* �ް�ނ̊O */
				sv_line = 0;
				continue;
			}
		}

		if(sw_if == 2)
		{	/* if ���̌��� */
			variables['Y'-'A'] = LogicY;
			if(parser(formula_str_if,&dummy,variables,26) <= 0 || math_err_flag)
			{	/* if �����O�ȉ��������� */
				sv_line = -1;
				math_err_flag = 0;	/* ���������_�G���[ */
				continue;
			}
		}

		gpset(x+xw_min,yw_max-(DeltaY - y_min)*y_ratio,_c_gr_grph);

		if(sw_gr_line == 2 && sv_line)
		{	/* �����⊮ */
			gline(x-1+xw_min, yw_max-(old_DeltaY - y_min)*y_ratio,
				  x+xw_min, yw_max-(DeltaY - y_min)*y_ratio,
				  _c_gr_grph);
			old_DeltaY = DeltaY;
			sv_line = 1;	/* �t���O */
		}
		else if(sw_gr_line == 2)
		{
			old_DeltaY = DeltaY;
			sv_line = 1;	/* �t���O */
		}
	}
	return ;
}