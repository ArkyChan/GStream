#include "ScreenCapture.h"

//#define DEBUG

using namespace std;

#define TESTRUNS 500
int main()
{
	HBITMAP pic;
	
	Sleep(5000);

	unsigned int timeBefore = clock();
	unsigned int times[TESTRUNS];
	for (int i = 0; i < TESTRUNS; i++)
	{
		timeBefore = clock();
		winInfo wInfo = hwndTowinInfo(GetForegroundWindow());
		pic = capScreen(wInfo);
		times[i] = (clock() - timeBefore);
	}

	SaveToFile(pic, "Test.bmp");

	int sum = 0;
	cout << "Times: " << endl;
	for (int i = 0; i < TESTRUNS; i++)
	{
		sum += times[i];
		cout << times[i] << endl;
	}
	cout << "Avegrage: " << (sum / TESTRUNS) << endl;
	

	//startGame("E:\\Terra\\NA\\TERA\\TERA-Launcher.exe");

	//capScreen(0, 0, 720, 640);
	cin.ignore(2);
}
