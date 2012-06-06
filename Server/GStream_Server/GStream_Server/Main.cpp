#define WIN32_LEAN_AND_MEAN

#include "ScreenCapture.h"

#include "boost\thread.hpp"
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>
#include "NetworkMain.h"
#include "vid_Encode.h"
#include "Logger.h"
//#define DEBUG

using namespace std;

int fps = 0;
ScreenCapture *s;
NetworkMain* net;

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

#define TESTRUNS 500
int main() {
	_LOG("Server start.",_INFO);
	net = new NetworkMain();
	net->net_startServer(0);

	//vidTest();
	cin.ignore(2);
	return 0;

	//Sleep(5000);
	unsigned int timeBefore = clock();
	unsigned int times[TESTRUNS];
	cout << "Press enter to start..";
	cin.ignore();
	cout << "Get window handle.." <<endl;

	cout << "Setup time.." << endl;
	s = new ScreenCapture(GetForegroundWindow());

	cout << "Start progress thread" << endl;
	boost::thread workerThread(progress);

	cout << "Do b-frames" << endl;
	unsigned int res=0;
	for (int i = 0; i < TESTRUNS; i++)
	{
		timeBefore = clock();
		s->screenCapture(s->rgbFrame);

		/*
		s->lastFrame->writeRaw("test.data");
		RGBtoYUV420PSameSize(s->lastFrame->getData(),nData,s->inf.bpp,0,s->inf.w,s->inf.h);
		vpx_image_t* img = (vpx_image_t*)nData;
		s->gcap.writeRaw("test.yuv.data",s->inf.w,s->inf.h,4,nData);

		if(vpx_codec_encode(&s->codec,img,s->frame_cnt,1,0,VPX_DL_REALTIME)==0){
		s->pkt = vpx_codec_get_cx_data(&s->codec, &s->iter);
		res = s->pkt->data.frame.sz;
		}else {
		ScreenCapture::die_codec(&s->codec,"Failed encode frame");
		cin.ignore();
		return 0;
		}
		*/

		times[i] = (clock() - timeBefore);
		fps++;
		//cout << res << endl;
	}
	s->rgbFrame->writeRaw("test.data");

	unsigned int sum=0,min=INT_MAX,max=0;
	for (int i = 0; i < TESTRUNS; i++)
	{
		sum += times[i];
		if(times[i]>max)
			max = times[i];
		else if(times[i]<min)
			min = times[i];
	}
	cout << "Avegrage: " << (sum / TESTRUNS) << endl;
	cout << "Min/Max: " << min << "-" << max;

	//startGame("E:\\Terra\\NA\\TERA\\TERA-Launcher.exe");

	//capScreen(0, 0, 720, 640);
	cin.ignore(2);
}

void vidTest()
{
	s = new ScreenCapture(GetForegroundWindow());

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
