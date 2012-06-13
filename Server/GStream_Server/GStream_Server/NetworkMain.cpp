//========================================================
// A The network Nexus, hadles reciveing of all client 
// commands and send data to the client
//========================================================

#include <iostream>
#include "NetworkMain.h"

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
	//std::cout << "net_onGameStart" << std::endl;
	sock->write_some(BUFF("Game start!"));
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
