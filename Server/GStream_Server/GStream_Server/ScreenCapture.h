#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include "Frame.h"
#include "glCap.h"

// Window info
struct winInfo
{
	int x;
	int y;
	int w;
	int h;
	HWND handle;
};
class ScreenCapture{
public:
	ScreenCapture(HWND);
	HBITMAP capScreen(winInfo info);
	winInfo hwndTowinInfo(HWND handle);
	BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
	unsigned int p_frame();
	unsigned int tPix,TPix;
	Frame *lastFrame,*curFrame;
	HDC hDC,hDest;
	winInfo inf;
	COLORREF getPixel(HBITMAP,int,int);
	glCap gcap;
};
#endif