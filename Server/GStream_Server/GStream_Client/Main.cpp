#include <ClanLib/core.h>
#include <ClanLib\display.h>
#include<ClanLib\gl.h>
#include <ClanLib/application.h>
#include <stdio.h>
#include <Windows.h>
#include <string>

#include "UI_Main.h"

using namespace std;

class Program
{
public:
	static int main(const std::vector<CL_String> &args);
};

// Get the UI ready
UI* ui;

int Program::main (const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;

	/*CL_DisplayWindowDescription window_description;
	window_description.set_title("Gee Gee Gee Gee Stream Babay Baby");
	window_description.set_size(CL_Size(1440, 900), true);
	window_description.set_fullscreen(true);
	window_description.set_decorations(false);

	CL_DisplayWindow window = CL_DisplayWindow(window_description);*/
	
	CL_DisplayWindow window("Gee Gee Gee Gee Stream Babay Baby",1280,720);
	
	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();

	string path(args[0].c_str());
	ui = new UI(&window, 1280, 692, path.substr(0, path.find_last_of("\\")).c_str(), "ui/main.html");

	glRasterPos2i(-1, 1);
	glPixelZoom(1,-1); 

	while (!ic.get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
		glClear(GL_COLOR_BUFFER_BIT); 

		//CL_Draw::fill(gc, CL_Rectf(40.0f, 60.0f, 800.0f, 800.0f), CL_Colorf(1.0f, 0.0f, 0.0f));

		ui->render();

		window.flip();

		CL_KeepAlive::process();
		Sleep(33);
	}

	return 0;
}

CL_ClanApplication app(Program::main);