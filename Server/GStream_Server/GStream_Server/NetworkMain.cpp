//========================================================
// A The network Nexus, hadles reciveing of all client 
// commands and send data to the client
//========================================================

#include <iostream>
#include "NetworkMain.h"
#include <zlib.h>

FuncPointer net_packetFuncs[];

// Start the Server
void net_startServer(int port){
	//thread a new tcp server
	_LOG("Starting TCP server.",_INFO);

	try{
		boost::thread t([&](){
			Gstream::TcpServer server("0.0.0.0",DEFUALT_PORT,net_packetFuncs);
			server.run();
		});
	}
	catch (std::exception& e){
		std::cout << e.what() << std::endl;
		_LOG("Tcp server failed to start!",_ERROR);
	}

	/* Gotta call em all
	for (int i = 0; i < P_ID_COUNT; i++)
		net_packetFuncs[i](0);*/
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
	Gstream::capture::ScreenCapture* s = new Gstream::capture::ScreenCapture(GetForegroundWindow());
	s->setupGDC();
	Gstream::encode::man_encode m = *new Gstream::encode::man_encode(1280,720,24);
	unsigned char* buffer = (unsigned char*)malloc(1280*720*3);

	for(int i=0;i<400;i++){
		if(sock->is_open()){
			//pk_frame frame = {(uint32_t)s->inf.size,0,m.encodeFrame(s->screenCapture()),0};
			z_stream strm;
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			deflateInit(&strm, 1);
			strm.avail_in = 1280*720*3;
			strm.avail_in = 1280*720*3;
			strm.avail_out = 1280*720*3;
			strm.next_in = m.encodeFrame(s->screenCapture());
			strm.next_out = buffer;
			deflate(&strm,Z_FINISH);
			deflateEnd(&strm);
			int s = (1280*720*3)-strm.avail_out;

			std::cout << s << std::endl;

			sock->write_some(BUFF((void*)s,sizeof(int)));
			sock->write_some(BUFF(buffer,1280*720*3-strm.avail_out));
		}else
			break;
	}
}

void net_onCMD(SOCK* sock){
	//std::cout << "net_onCMD" << std::endl;
	sock->write_some(BUFF("CMD got"));
}

// Send packets
//===========================

// Bind the functions to their IDs
//===========================
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
