#define WIN32_LEAN_AND_MEAN

#include "ScreenCapture.h"

#include "Logger.h"
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>
#include "NetworkMain.h"
#include "vid_Encode.h"
#include "man_encode.h"

//#define DEBUG

using namespace std;

int fps = 0;
Gstream::capture::ScreenCapture *s;

void progress(){
	while(true){
		TCHAR szNewTitle[MAX_PATH];
		StringCchPrintf(szNewTitle, MAX_PATH, TEXT("%i px/s (%i fps)"), s->tPix,fps);
		SetConsoleTitle(szNewTitle);

		s->tPix=0;
		fps=0;
		Sleep(1000);
	}
}

void vidTest();
void vidTest2();

#define TESTRUNS 500

int main() {
	_LOG("Server start.",_INFO);
	net_startServer(0);
	
	vidTest2();

	cin.ignore(2);
	net_stopServer();
	return 0;
}

void vidTest()
{
	s = new Gstream::capture::ScreenCapture(GetForegroundWindow());

	cout << "Vid start" << endl;
	vid_prep(1280, 720);
	cout << "Encodeing frames" << endl;

	// Encode 5 seconds of captures
	for (int i = 0; i < 30*5; i++)
	{
		s->screenCapture(s->rgbFrame);
		vid_writeFrame((uint8_t*)s->rgbFrame->getData(),s->inf.w*s->inf.h*3);
	}

	vid_end();
	cout << "Done" << endl;
}
void vidTest2()
{
	s = new Gstream::capture::ScreenCapture(GetForegroundWindow());
	s->setupGDC();

	cout << "Vid start" << endl;
	Gstream::encode::man_encode m = *new Gstream::encode::man_encode(1280,720,24);
	cout << "Encodeing frames" << endl;
	unsigned char* d;
	size_t start;
	// Encode 5 seconds of captures
	for (int i = 0; i < 25*5; i++)	{
		start = clock();
		d = m.encodeFrame(s->screenCapture());
		cout << clock()-start << endl;
		m.dumpFrame(d);
	}

	cout << "Done" << endl;
}