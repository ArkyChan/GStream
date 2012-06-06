#ifndef PACKET_H
#define PAKCET_H

class Packet{
public:
	virtual ~Packet() = 0;
	virtual void send(SOCK*) = 0;
	virtual void recv(SOCK*,short) = 0;
private:
	short ID;
};
#endif