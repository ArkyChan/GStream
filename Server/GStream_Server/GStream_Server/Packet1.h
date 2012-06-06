#include "Packet.h"

class Packet1 : public Packet {
public:
	Packet1(){
		this->ID = 1;
	}

	virtual void send(SOCK* s){
		s->async_write(s,BUFF(this->ID,sizeof(short)));
	}

	virtual void recv(SOCK*,short){

	}
}