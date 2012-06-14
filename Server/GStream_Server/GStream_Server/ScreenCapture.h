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
#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#define iface (vpx_codec_vp8_cx())
#define fourcc    0x30385056

#define IVF_FILE_HDR_SZ  (32)
#define IVF_FRAME_HDR_SZ (12)

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