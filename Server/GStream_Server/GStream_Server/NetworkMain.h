#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

#include "TcpServer.h"

#define DEFUALT_PORT "1337"

void net_startServer(int port);
void net_stopServer();

#endif