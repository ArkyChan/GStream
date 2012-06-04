#include "ScreenCapture.h"
#include "boost\thread.hpp"
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>
//#define DEBUG

using namespace std;
#define WIN32_LEAN_AND_MEAN

int fps = 0;
ScreenCapture *s;

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

#define TESTRUNS 500
int main() {
	//Sleep(5000);
	unsigned int timeBefore = clock();
	unsigned int times[TESTRUNS];
	cout << "Pres enter to start..";
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

		//res = s->p_frame();
		unsigned char* nData = (unsigned char*)malloc(1920*1080*3);
		unsigned char* data = s->screenCapture();

		s->gcap.writeRaw("test.rgb.raw",1920,1080,4,data);
		RGBtoYUV420PSameSize(data,nData,1,0,1920,1080);
		vpx_image_t* img = (vpx_image_t*)nData;
		s->gcap.writeRaw("test.yuv.raw",1920,1080,3,nData);

		if(vpx_codec_encode(&s->codec,img,s->frame_cnt,1,0,VPX_DL_REALTIME)==0){
			s->pkt = vpx_codec_get_cx_data(&s->codec, &s->iter);
			res = s->pkt->data.frame.sz;
		}else {
			ScreenCapture::die_codec(&s->codec,"Failed encode frame");
			cin.ignore();
			return 0;
		}

		times[i] = (clock() - timeBefore);
		fps++;
		/*char str[20];
		sprintf_s(str,"image%i.bmp",i);
		s->snap(str);*/
		cout << res << endl;
	}

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
