#ifndef NETWORKMAIN_H
#define NETWORKMAIN_H

#include "TcpServer.h"
#include "Packet.h"
#include "ScreenCapture.h"
#include "man_encode.h"

namespace Gstream {
	namespace net {
		FuncPointer net_packetFuncs[];

		void net_startServer(int);
		void net_stopServer();
		void net_onPing(SOCK*);
		void net_onHandshake(SOCK*);
		void net_onMousepos(SOCK*);
		void net_onKeypress(SOCK*);
		void net_onHello(SOCK*);
		void net_onError(SOCK*);
		void net_onLogin(SOCK*);
		void net_onSetup(SOCK*);
		void net_onStats(SOCK*);
		void net_onGameStart(SOCK*);
		void net_onCMD(SOCK*);
		}
}

#endif