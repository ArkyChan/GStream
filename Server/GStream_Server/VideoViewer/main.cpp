#define WIN32_LEAN_AND_MEAN

#include <ClanLib/core.h>
#include <ClanLib\display.h>
#include<ClanLib\gl.h>
#include <ClanLib/application.h>
#include <stdio.h>
#include <ClanLib/network.h>

#include "lzo/lzoconf.h"
#include <lzo\lzo1b.h>

#define USE_LZO1X 1

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

	}else {
		CL_SetupNetwork setup_network;
		try{
			std::cout << "Enter ip:";
			std::string ip = "127.0.0.1"; 
			std::cin >> ip;
			std::cout << "\n\nConnecting...." << std::endl;

			CL_SocketName s(ip,"1337");
			CL_TCPConnection con(s);

			con.write_uint8(9);

			//get size
			int size = 0;
			int frame_size = 1280*720*3; 
			con.read(&size,sizeof(int));

			std::cout << size << std::endl;	

			unsigned char* udata = (unsigned char*)malloc(frame_size);
			unsigned char* data = (unsigned char*)malloc(size);
			unsigned char* stay = (unsigned char*)malloc(frame_size);
			int len,lastSize;
			std::cout << "Starting playback" << std::endl;	
			CL_DisplayWindow window("GStream network viewer",1280,720);

			lzo_init();
			lzo_voidp wrkmen = (lzo_voidp)malloc(LZO1B_MEM_DECOMPRESS);
			lzo_uint out_len = frame_size;

			while((len = con.read(data,size))>0){
				out_len = frame_size;
				lzo1b_decompress(data,size,udata,&out_len,wrkmen);

				if(out_len<frame_size){
					throw CL_Exception("Invalid compressed data");
				}
				
				for(int i=0;i<frame_size;i++){
					if(udata[i] != 255){
						stay[i] = udata[i]; 
					}
				}

				
				glClear(GL_COLOR_BUFFER_BIT);
				glRasterPos2i(-1, 1); 
				glPixelZoom(1,-1); 
				glDrawPixels(1280,720,GL_BGR, GL_UNSIGNED_BYTE , stay);
				window.flip();
				CL_KeepAlive::process();

				//read the next size
				lastSize=size;
				con.read(&size,sizeof(int));
				if(lastSize < size)
					data = (unsigned char*)realloc(data,size);
			}

		}catch(CL_Exception ex){std::cout << ex.get_message_and_stack_trace().c_str() << std::endl;}
	}

	std::cout << "Press enter to exit..";
	std::cin.ignore();
}