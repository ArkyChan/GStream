#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include "Frame.h"

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
	Frame lFrame,pFrame;
	HDC hDC;
	winInfo inf;
	COLORREF getPixel(HBITMAP,int,int);
};
#endif