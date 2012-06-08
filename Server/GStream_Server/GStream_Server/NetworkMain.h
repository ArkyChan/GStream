#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

#include "TcpServer.h"

#define DEFUALT_PORT "1337"

typedef void(*FuncPointer)();
#include "Logger.h"

using namespace Gstream;

// Numer of packet ids 
#define PCK_ID_COUNT PID_COUNT-1

// The packets
enum packetID{
	P_ID_PING,
	P_ID_HANDSHAKE, // hehe
	P_ID_MOUSEPOS,
	P_ID_KEYPRESS,
	P_ID_FRENDLYHELLO, // meh for fun
	PID_COUNT // How many ids are their :o , make sure its the lat one
};

void net_startServer(int port);
void net_stopServer();

#endif