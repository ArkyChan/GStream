#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>

// Window info
struct winInfo
{
	int x;
	int y;
	int w;
	int h;
	HWND handle;
};

HBITMAP capScreen(winInfo info);
winInfo hwndTowinInfo(HWND handle);
BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

#endif