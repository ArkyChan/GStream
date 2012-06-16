#define WIN32_LEAN_AND_MEAN

#include "NetworkMain.h"

int main() {
	_LOG("Server start.",_INFO);
	Gstream::net::net_startServer(0);

	std::cin.ignore(2);
	Gstream::net::net_stopServer();
	return 0;
}

/*
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
		d = s->screenCapture();
		m.dumpFrame(d,true);
	}

	cout << "Done (view with image.py)" << endl;
}*/