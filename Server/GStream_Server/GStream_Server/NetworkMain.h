#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

typedef void(*FuncPointer)();
#include "Logger.h"
#include "TCP_layer.h"

// Numer of packet ids 
#define PCK_ID_COUNT PID_COUNT-1

// The packets
enum packetID
{
	P_ID_PING,
	P_ID_HANDSHAKE, // hehe
	P_ID_MOUSEPOS,
	P_ID_KEYPRESS,
	P_ID_FRENDLYHELLO, // meh for fun
	PID_COUNT // How many ids are their :o , make sure its the lat one
};

class NetworkMain {
public:
	void net_startServer(int port);
	void net_stopServer();
private:
	void startTCP();

	TcpLayer* tcp;
	boost::thread_group threadPool;
};

#endif