//========================================================
// A The network Nexus, hadles reciveing of all client 
// commands and send data to the client
//========================================================

#include <iostream>
#include "NetworkMain.h"

// An array of function pointers, the packet id is used to look up the funcion :D
FuncPointer net_packetFuncs[];

// Start the Server
void NetworkMain::net_startServer(int port){
	// Just tests for now
	_LOG("Networking setup.",_INFO);
	// Gotta call em all
	for (int i = 0; i < PCK_ID_COUNT; i++)
		net_packetFuncs[i]();

	//thread a new tcp server
	startTCP();
}

// Clled when a packet is recived
void onPacketRecived()
{

}

void NetworkMain::startTCP(){
	_LOG("Starting TCP layer IO service.",_INFO);
	try{
		IO_SERVICE io_service;
		this->tcp = new TcpLayer(io_service);
		IO_SERVICE::work work(io_service);
		this->threadPool.create_thread([&](){io_service.run();});
	}
	catch (std::exception& e){
		_LOG(e.what(),_ERROR);
	}
	_LOG("TCP IO service started.",_INFO);
}
// Shut down the server
void NetworkMain::net_stopServer()
{

}

// Recived packets
//===========================

// Get the time taken to send data to the client
void net_onPing()
{
	std::cout << "onPing" << std::endl;
}

// Get the clients ip and ports, returns any other ports this server is using
void net_onHandshake()
{
	std::cout << "net_onhandshake" << std::endl;
}

// Set the position of the mouse
void net_onMousepos()
{
	std::cout << "net_onMousepos" << std::endl;
}

// Simulates the keypress of the key with the given id
void net_onKeypress()
{
	std::cout << "net_onKeypress" << std::endl;
}

// Send packets
//===========================

// Bind the functions to their IDs
//===========================
FuncPointer net_packetFuncs[PCK_ID_COUNT] =
{
	net_onPing,
	net_onHandshake,
	net_onMousepos,
	net_onKeypress
};