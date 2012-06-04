#include "ScreenCapture.h"

ScreenCapture::ScreenCapture(HWND handle) {
	this->lastFrame = NULL;
	this->curFrame = NULL;
	this->tPix = 0;
	this->TPix = 0;
	this->hDC = GetDC(NULL);
	this->inf = this->hwndTowinInfo(NULL);
	std::cout << "Image res: " << this->inf.w << "x" << this->inf.h << std::endl;
	//std::cout << "Save a frame" << std::endl;

	/*init frames
	this->lastFrame = new Frame(screenCapture(),this->inf.w,this->inf.h,3);
	this->curFrame = this->lastFrame;
	//save it
	//gcap.writeRaw("image.raw",this->inf.w,this->inf.h,3,this->lastFrame->getData());*/
	SaveToFile(capScreen(this->inf),"image.bmp");

	/*
	 * codec settings
	 */
	this->res = vpx_codec_enc_config_default(iface, &this->cfg, 0);
	cfg.rc_target_bitrate = this->inf.w * this->inf.h * cfg.rc_target_bitrate  / cfg.g_w / cfg.g_h;
    cfg.g_w = this->inf.w;
    cfg.g_h = this->inf.h;
	//init codec
	if(vpx_codec_enc_init(&codec, iface, &cfg, 0))
		die_codec(&codec,"Failed to initalize codec");
}

// Capture the screen, returns a bitmap
HBITMAP ScreenCapture::capScreen(winInfo info)
{
	this->hDC = GetDC(info.handle); // get the desktop device context
	this->hDest = CreateCompatibleDC(this->hDC); // create a device context to use yourself

	// create a bitmap
	HBITMAP hbDesktop = CreateCompatibleBitmap(this->hDC, info.w, info.h);

	// use the previously created device context with the bitmap
	SelectObject(this->hDest, hbDesktop);

	// copy from the desktop device context to the bitmap device context
	BitBlt( this->hDest, info.x, info.y, info.w, info.h, this->hDC, 0, 0, SRCCOPY | BLACKNESS);

	DeleteDC(this->hDest);
	ReleaseDC(0,this->hDC);
	return hbDesktop;
}

unsigned int ScreenCapture::p_frame(){
	unsigned int result = 0;

	if(this->lastFrame != NULL){
		this->lastFrame->setData(this->curFrame->getData(),true);
		this->curFrame->setData(screenCapture());
		for(int h = 0;h<this->lastFrame->getHeight();h++){
			for(int w = 0;w<this->lastFrame->getWidth();w++){
				this->tPix++;
				this->TPix++;

				//std::cout << "\r" << this->curFrame->getPixel(w,h) << "-" << this->lastFrame->getPixel(w,h) << "      ";
				//std::cout << "\r" << (this->curFrame->getPixel(w,h)==this->lastFrame->getPixel(w,h) ? "yes" : "no") << " " << h << "x" << w << "          ";
				//Sleep(1000);
				if(this->curFrame->getPixel(w,h) != this->lastFrame->getPixel(w,h)){
					result++;
				}
			}
		}
	}else {
		this->lastFrame = new Frame(/*gcap.capScreen(1280,720)*/screenCapture(),this->inf.w,this->inf.h,3);
		this->curFrame = this->lastFrame;
	}

	return result;
}

void ScreenCapture::snap(const char* file){
	SaveToFile(capScreen(this->inf),file);
}
// Extracts info from a hwn and returns a winInfo struct
winInfo ScreenCapture::hwndTowinInfo(HWND handle)
{
	winInfo info;
	if (handle == NULL)
	{
		info.handle = NULL;
		info.x = 0;
		info.y = 0;
		info.h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		info.w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	}
	else
	{
		info.handle = handle;

		RECT rect;
		// Get the windo info
		GetClientRect(handle, &rect);

		std::cout << rect.top << " " << rect.left << " " << rect.right << " " << rect.bottom << " " << std::endl;
		info.x = rect.top;
		info.y = rect.left;
		info.h = rect.bottom;
		info.w = rect.right;
	}
	return info;
}

unsigned char* ScreenCapture::screenCapture(){
	HBITMAP b = capScreen(this->inf);
	size_t len = this->inf.w*this->inf.h*3;
	unsigned char* data = (unsigned char*)malloc(len);
	GetBitmapBits(b,len,data);
	DeleteObject(b);
	return data;
}
// Save a bit map to disk
BOOL ScreenCapture::SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	HDC hDC;
	int iBits;
	WORD wBitCount;
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
	BITMAP Bitmap;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal,hOldPal=NULL;

	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

COLORREF ScreenCapture::getPixel(HBITMAP bmp,int x,int y){
	COLORREF pixelColor;
	pixelColor = ::GetPixel(this->hDest, x,y);

	DeleteDC(this->hDest);
	ReleaseDC(NULL, hDC);

	return pixelColor;
}
