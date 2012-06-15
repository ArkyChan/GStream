#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

#include "TcpServer.h"
#include "Packet.h"
#include "ScreenCapture.h"
#include "man_encode.h"

void net_startServer(int port);
void net_stopServer();

#endif