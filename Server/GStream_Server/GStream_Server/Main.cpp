#include "ScreenCapture.h"
#include "boost\thread.hpp"
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>

//#define DEBUG

using namespace std;
#define WIN32_LEAN_AND_MEAN

ScreenCapture *s;

void progress(){
	while(true){
		TCHAR szNewTitle[MAX_PATH];
		StringCchPrintf(szNewTitle, MAX_PATH, TEXT("%i px/s (%i-TotalPx)"), s->tPix,s->TPix);
		SetConsoleTitle(szNewTitle);

		s->tPix=0;
		Sleep(1000);
	}
}

#define TESTRUNS 500
int main() {
	//Sleep(5000);
	unsigned int timeBefore = clock();
	unsigned int times[TESTRUNS];
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
		res = s->p_frame();
		times[i] = (clock() - timeBefore);

		//cout << res << endl;
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
	cout << "Min/Max: " << min << "-" << max << endl;
	
	//startGame("E:\\Terra\\NA\\TERA\\TERA-Launcher.exe");

	//capScreen(0, 0, 720, 640);
	cin.ignore(2);
}
