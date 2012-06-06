#include "Packet.h"

class Packet0 : public Packet {
public:
	Packet0(){
		this->ID = 0;
	}

	virtual void send(SOCK* s){
		s->async_write(s,BUFF(this->ID,sizeof(short)));
	}

	virtual void recv(SOCK*,short){

	}
}