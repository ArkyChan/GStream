//========================================================
// A The network Nexus, hadles reciveing of all client 
// commands and send data to the client
//========================================================

#include <iostream>
#include "NetworkMain.h"

// An array of function pointers, the packet id is used to look up the funcion :D
FuncPointer net_packetFuncs[];

// Start the Server
void net_startServer(int port){
	// Just tests for now
	_LOG("Networking setup.",_INFO);

	//thread a new tcp server
	_LOG("Starting TCP server.",_INFO);

	try{
		boost::thread t([&](){
			TcpServer server("0.0.0.0",DEFUALT_PORT);
			server.run();
		});
	}
	catch (std::exception& e){
		std::cout << e.what() << std::endl;
		_LOG("Tcp server failed to start!",_ERROR);
	}
	_LOG("TCP server started.",_INFO);

	// Gotta call em all
	for (int i = 0; i < PCK_ID_COUNT; i++)
		net_packetFuncs[i]();
}

// Clled when a packet is recived
void onPacketRecived(){

}
// Shut down the server
void net_stopServer(){

}

// Recived packets
//===========================

// Get the time taken to send data to the client
void net_onPing(){
	std::cout << "onPing" << std::endl;
}

// Get the clients ip and ports, returns any other ports this server is using
void net_onHandshake(){
	std::cout << "net_onhandshake" << std::endl;
}

// Set the position of the mouse
void net_onMousepos(){
	std::cout << "net_onMousepos" << std::endl;
}

// Simulates the keypress of the key with the given id
void net_onKeypress(){
	std::cout << "net_onKeypress" << std::endl;
}

// Send packets
//===========================

// Bind the functions to their IDs
//===========================
FuncPointer net_packetFuncs[PCK_ID_COUNT] = {
	net_onPing,
	net_onHandshake,
	net_onMousepos,
	net_onKeypress
};
