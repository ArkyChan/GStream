#ifndef GL_INCLUDE
#define GL_INCLUDE

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "Logger.h"

#define SOCK boost::asio::ip::tcp::socket
#define BUFF boost::asio::buffer
#define IO_SERVICE boost::asio::io_service
#define ENABLE_SHARED boost::enable_shared_from_this
#define PLACEHOLDER boost::asio::placeholders
#define TCP boost::asio::ip::tcp
#define ASIO boost::asio
#define BIND boost::bind
#define SHARED_PTR boost::shared_ptr
#define CAST boost::lexical_cast

#define DEFUALT_PORT "1337"

// The packets
enum packetID {
	P_ID_PING,
	P_ID_HANDSHAKE, // hehe
	P_ID_MOUSEPOS,
	P_ID_KEYPRESS,
	P_ID_FRENDLYHELLO, // meh for fun
	P_ID_ERRORMSG,
	P_ID_LOGIN,
	P_ID_CL_SETUP,
	P_ID_STATS,
	P_ID_GAMESTART,
	P_ID_CMD,
	P_ID_COUNT // How many ids are their :o , make sure its the lat one
};

//callback pointer
typedef void(*FuncPointer)(SOCK*);
typedef void(*socketWrite)(unsigned char*);

#endif