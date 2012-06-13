#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

#include "TcpServer.h"
#include "Packet.h"

void net_startServer(int port);
void net_stopServer();

#endif