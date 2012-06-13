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
		int frame_size = 1280*720*3;
		unsigned char* frame = (unsigned char*)malloc(frame_size);
		
		CL_DisplayWindow window("GStream output viewer",1280,720);
		CL_GraphicContext gc = window.get_gc();
		CL_InputContext ic = window.get_ic();
		while (!ic.get_keyboard().get_keycode(CL_KEY_ESCAPE)){
			gc.clear();
			fread(frame,frame_size,1,f);
			CL_PixelBuffer buffer(1280,720,CL_TextureFormat::cl_rgb8,frame,true);
			//buffer.upload_data(gc.get_cliprect(),frame);

			gc.draw_pixels(0,0,buffer,gc.get_cliprect());
			window.flip();
			CL_KeepAlive::process();
		}
	}
}