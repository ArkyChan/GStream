#include <ClanLib/core.h>
#include <ClanLib\display.h>
#include<ClanLib\gl.h>
#include <ClanLib/application.h>
#include <stdio.h>

int main(){
	CL_SetupCore setup_core;
    CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;

	FILE* f = 0;
	fopen_s(&f,"test.data","rb");
	if(f != 0){
		int rez = 1280*720;
		int frame_size = rez*3;
		unsigned char* stay = (unsigned char*)malloc(frame_size);
		unsigned char* frame = (unsigned char*)malloc(frame_size);

		CL_DisplayWindow window("GStream output viewer",1280,720);
		CL_GraphicContext gc = window.get_gc();
		CL_InputContext ic = window.get_ic();
		
		while (!ic.get_keyboard().get_keycode(CL_KEY_ESCAPE)){
			if(fread(frame,frame_size,1,f)==0){
				break;
			}else{
				for(int i=0;i<frame_size;i++){
					if(frame[i] != 255){
						stay[i] = frame[i]; 
					}
				}
			}
			
			glClear(GL_COLOR_BUFFER_BIT);
			glRasterPos2i(-1, 1); 
			glPixelZoom(1,-1); 
			glDrawPixels(1280,720,GL_RGB, GL_UNSIGNED_BYTE , stay);
			window.flip();

			CL_KeepAlive::process();
			Sleep(80);
		}
		fclose(f);

		glFinish();
		std::cout << "Press enter to exit";
		std::cin.ignore();

	}
}