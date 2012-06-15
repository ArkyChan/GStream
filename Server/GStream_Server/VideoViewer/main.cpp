#define WIN32_LEAN_AND_MEAN

#include <ClanLib/core.h>
#include <ClanLib\display.h>
#include<ClanLib\gl.h>
#include <ClanLib/application.h>
#include <stdio.h>
#include <ClanLib/network.h>
#include <zlib.h>

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

	}else {
		CL_SetupNetwork setup_network;

		try{
			CL_SocketName s("127.0.0.1","1337");
			CL_TCPConnection con(s);

			con.write_uint8(9);

			//get size
			
			int size = con.read_uint32();
			std::cout << size << std::endl;	
			unsigned char* data = (unsigned char*)malloc(size);
			unsigned char* stay = (unsigned char*)malloc(size);
			int len;
			std::cout << "Starting playback" << std::endl;	
			CL_DisplayWindow window("GStream network viewer",1280,720);
			
			while((len = con.read(data,101*1024,false))>0){
				z_stream strm;
				strm.zalloc = Z_NULL;
				strm.zfree = Z_NULL;
				strm.opaque = Z_NULL;
				strm.avail_in = 0;
				strm.next_in = Z_NULL;
				inflateInit(&strm);
				strm.avail_out = size;
				strm.avail_in = len;
				strm.next_in  = data;
				strm.next_out = data;
				inflate(&strm, Z_FINISH);
				inflateEnd(&strm);

				for(int i=0;i<size;i++){
					//data[i] = _byteswap_ushort(data[i]);
					if(data[i] != 255){
						stay[i] = data[i]; 
					}
				}

				glClear(GL_COLOR_BUFFER_BIT);
				glRasterPos2i(-1, 1); 
				glPixelZoom(1,-1); 
				glDrawPixels(1280,720,GL_BGR, GL_UNSIGNED_BYTE , stay);
				window.flip();
				CL_KeepAlive::process();
			}

		}catch(CL_Exception ex){std::cout << ex.get_message_and_stack_trace().c_str() << std::endl;}
	}
	std::cout << "Press enter to exit..";
	std::cin.ignore();
}