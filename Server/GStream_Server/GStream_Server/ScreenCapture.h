#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include "Frame.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

namespace Gstream{
	namespace capture {
		// Window info
		struct winInfo
		{
			int x;
			int y;
			int w;
			int h;
			int bpp;
			int size;
			HWND handle;
		};
		class ScreenCapture{
		public:
			ScreenCapture(HWND);
			HBITMAP capScreen(winInfo info);
			winInfo hwndTowinInfo(HWND handle);
			BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
			unsigned int tPix,TPix;
			unsigned char* screenCapture(Frame* frame=NULL,bool freeLast=false);
			Frame *rgbFrame;
			void snap(const char*);
			HDC hDC,hDest;
			winInfo inf;
			COLORREF getPixel(HBITMAP,int,int);
			void setDefaultHeader();
			void setupGDC();
		private:
			BITMAPINFO outputInfo;
			HBITMAP hbDesktop;
			unsigned char* pixelData;
		};
	}
}
#endif