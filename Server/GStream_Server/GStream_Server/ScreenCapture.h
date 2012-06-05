#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include "Frame.h"
#include "glCap.h"
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

#include "Util.h"

// Window info
struct winInfo
{
	int x;
	int y;
	int w;
	int h;
	int bpp;
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
	unsigned char* ScreenCapture::screenCapture();
	Frame *lastFrame,*curFrame;
	void snap(const char*);
	HDC hDC,hDest;
	winInfo inf;
	COLORREF getPixel(HBITMAP,int,int);
	glCap gcap;
	vpx_codec_ctx_t codec; //codec container
    vpx_codec_enc_cfg_t cfg; //codec config
	vpx_codec_err_t res; //codec error result
	int frame_cnt;
	const vpx_codec_cx_pkt_t *pkt;
	vpx_codec_iter_t iter;
	static void die_codec(vpx_codec_ctx_t *ctx, const char *s){
		const char *detail = vpx_codec_error_detail(ctx);

		printf("%s: %s\n", s, vpx_codec_error(ctx));
		if(detail)
			printf("    %s\n",detail);
	}
	
};
#endif