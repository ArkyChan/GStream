//========================================================
// A The network Nexus, hadles reciveing of all client 
// commands and send data to the client
//========================================================

#include "NetworkMain.h"
#include <strsafe.h>

#include <lzo\lzoconf.h>
#include <lzo\lzo1x.h>

#define USE_LZO1X 1

namespace Gstream {
	namespace net {
		unsigned int dataQ;

		void progress(){
			while(true){
				TCHAR szNewTitle[MAX_PATH];
				StringCchPrintf(szNewTitle, MAX_PATH, TEXT("%i Kb/s"),(int)(dataQ/1024));
				SetConsoleTitle(szNewTitle);

				dataQ=0;
				Sleep(1000);
			}
		}
		// Start the Server
		void net_startServer(int port){
			//thread a new tcp server
			_LOG("Starting TCP server.",_INFO);
			boost::thread t(progress);

			try{
				boost::thread t([&](){
					TcpServer server("0.0.0.0",DEFUALT_PORT,net_packetFuncs);
					server.run();
				});
			}
			catch (std::exception& e){
				std::cout << e.what() << std::endl;
				_LOG("Tcp server failed to start!",_ERROR);
			}
		}

		// Shut down the server
		void net_stopServer(){
			_LOG("Shutting server down..\n",_INFO);
		}

		// Recived packets
		//===========================

		// Get the time taken to send data to the client
		void net_onPing(SOCK* sock){
			//std::cout << "net_onPing" << std::endl;
			sock->write_some(BUFF("PONG"));
		}

		// Get the clients ip and ports, returns any other ports this server is using
		void net_onHandshake(SOCK* sock){
			//std::cout << "net_onhandshake" << std::endl;
			sock->write_some(BUFF("Handshake :)"));
		}

		// Set the position of the mouse
		void net_onMousepos(SOCK* sock){
			//std::cout << "net_onMousepos" << std::endl;
			sock->write_some(BUFF("Mouse pos change"));
		}

		// Simulates the keypress of the key with the given id
		void net_onKeypress(SOCK* sock){
			//std::cout << "net_onKeypress" << std::endl;
			sock->write_some(BUFF("Keypress :)"));
		}

		void net_onHello(SOCK* sock){
			//std::cout << "net_onHello" << std::endl;
			sock->write_some(BUFF("Hey ^^"));
		}

		void net_onError(SOCK* sock){
			//std::cout << "net_onError" << std::endl;
			sock->write_some(BUFF("Oh no error!"));
		}

		void net_onLogin(SOCK* sock){
			//std::cout << "net_onLogin" << std::endl;
			sock->write_some(BUFF("LOL login"));
		}

		void net_onSetup(SOCK* sock){
			//std::cout << "net_onSetup" << std::endl;
			sock->write_some(BUFF("Setup stuff"));
		}

		void net_onStats(SOCK* sock){
			//std::cout << "net_onStats" << std::endl;
			sock->write_some(BUFF("Stats req"));
		}

		void net_onGameStart(SOCK* sock){
			//time to pipe them some tasty video
			int buffSize = (1280*720*3);
			capture::ScreenCapture* s = new capture::ScreenCapture(GetForegroundWindow());
			s->setupGDC();
			encode::man_encode m = *new encode::man_encode(1280,720,24);
			unsigned char* buffer = (unsigned char*)malloc(buffSize);
			unsigned char* img = 0;

			lzo_init();
			lzo_voidp wrkmen = (lzo_voidp)malloc(LZO1X_1_MEM_COMPRESS);
			lzo_uint out_len;

			for(int i=0;i<400;i++){
				if(sock->is_open()){
					img = m.encodeFrame(s->screenCapture());
					lzo1x_1_compress(img,buffSize,buffer,&out_len,wrkmen);
					dataQ += out_len;

					sock->write_some(BUFF(&out_len,sizeof(lzo_uint)));
					sock->write_some(BUFF(buffer,out_len));
				}else
					break;
			}
		}

		void net_onCMD(SOCK* sock){
			//std::cout << "net_onCMD" << std::endl;
			sock->write_some(BUFF("CMD got"));
		}

		FuncPointer net_packetFuncs[P_ID_COUNT] = {
			net_onPing,
			net_onHandshake,
			net_onMousepos,
			net_onKeypress,
			net_onHello,
			net_onError,
			net_onLogin,
			net_onSetup,
			net_onStats,
			net_onGameStart,
			net_onCMD
		};
	}
}
