/*
 *	glio.h
 *
 *	Original Copyright (c) 1989 Motohisa Ohno, All Rights Reserved.
 *	Modified/Debug (C) 1992 Hirokazu Inoue.
 */
#if     !defined(__GLIO_DEF_)
#define __GLIO_DEF_

enum GPUT_MODE {
	GPUT_COPY,
	GPUT_NOT,
	GPUT_OR,
	GPUT_AND,
	GPUT_XOR,
};

struct glio_t {
	char mode;
	char active, visual;
	char fore, back;
	char _reserve;
	char palette[8];
	int left, top, right, bottom;
	char v_bank, a_bank;
};

enum _colors {
	_BLACK, _BLUE, _RED, _MAGENTA, _GREEN, _CYAN, _YELLOW, _WHITE,
	_GRAY, _LIGHTBLUE, _LIGHTRED, _LIGHTMAGENTA, _LIGHTGREEN,
	_LIGHTCYAN, _LIGHTYELLOW, _LIGHTWHITE,
};

enum _planes {
	_PL_BLUE = 1,
	_PL_RED = 2,
	_PL_GREEN = 4,
	_PL_INTENSITY = 8,
	_PL_ALL = 15,
};

#ifdef __cplusplus
extern "C"{
#endif

void far ginit(void);
void far gscreen(int mode, int sw, int active, int visual);
void far gview(int left, int top, int right, int bottom, int area, int border);
void far gcolor(int fore, int back, int mode);
void far gpalette(int no, int color);
void far gcls(void);
void far gpset(int px, int py, int color);
void far gline(int left, int top, int right, int bottom, int color);
void far glines(int left, int top, int right, int bottom,
				int color, unsigned style);
void far gbox(int left, int top, int right, int bottom, int color);
void far gboxs(int left, int top, int right, int bottom,
				int color, unsigned style);
void far gbar(int left, int top, int right, int bottom, int color);
void far gbarf(int left, int top, int right, int bottom,
				int color, int fcolor);
void far gbarp(int left, int top, int right, int bottom, int color);
void far gsetpattern(int ln, char far *pattern);
void far gcircle(int px, int py, int radius, int color);
void far gcirclef(int px, int py, int radius, int color, int fcolor);
void far gcirclep(int px, int py, int radius, int color);
void far gellipse(int px, int py, int rx, int ry, int color);
void far gellipsef(int px, int py, int rx, int ry, int color, int fcolor);
void far gellipsep(int px, int py, int rx, int ry, int color);
void far garc(int px, int py, int rx, int ry, int color, int sang, int eang);
void far garcf(int px,int py,int rx,int ry,int col,int fcol,int sang,int eang);
void far garcp(int px, int py, int rx, int ry, int color, int sang, int eang);
void far gpie(int px, int py, int rx, int ry, int color, int sang, int eang);
void far gpief(int px,int py,int rx,int ry,int col,int fcol,int sang,int eang);
void far gpiep(int px, int py, int rx, int ry, int color, int sang, int eang);
void far gpaint(int px, int py, int color, int border);
void far gpaintp(int px, int py, int border);
void far gget(int left, int top, int right, int bottom, void far *buffer);
void far gput(int px, int py, int mode, void far *buffer);
int far ggetsize(int left, int top, int right, int bottom);
void far groll(int xdots, int ydots);
void far groll0(int xdots, int ydots);
void far gputcode(int px, int py, int code, int mode);
int far gputch(int code);
int far gputchar(int code);
int far gputs(char far *str);
void far gpmode(int mode);
int far gpxpos(void);
int far gpypos(void);
void far gsetpos(int px, int py);
int far gpoint(int px, int py);
struct glio_t far * far gliowork(void);

int far gsave(char far *fname);
int far gload(char far *fname);

int gprintf(const char *fmt, ...);
int vgprintf(const char *fmt, void *args);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* !defined(__GLIO_DEF_) */
