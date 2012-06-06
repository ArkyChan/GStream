#ifndef TCP_LAYER
#define TCP_LAYER

#define DEFUALT_PORT 5080

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>


#include "Logger.h"
#include "BoostMacro.h"
#include "Packet.h"
#include "Client.h"
#include "tcp_connection.h"

class TcpLayer : public ENABLE_SHARED<TcpLayer>{
public:
	TcpLayer(IO_SERVICE& io_service) : acceptor_(io_service, TCP::endpoint(TCP::v4(), DEFUALT_PORT)) {
		start_accept();
	}
private:
	void start_signal_wait()	{
		signal_.async_wait(boost::bind(&TcpLayer::handle_signal_wait, this));
	}
	void handle_signal_wait()	{
		if (acceptor_.is_open())
		{
			int status = 0;
			while (waitpid(-1, &status, WNOHANG) > 0) {}

			start_signal_wait();
		}
	}
	void start_accept(){
		tcp_connection::pointer new_connection =  tcp_connection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_connection->socket(), BIND(&TcpLayer::handle_accept, this, new_connection,PLACEHOLDER::error));
	}

	void handle_accept(tcp_connection::pointer new_connection,const boost::system::error_code& error){
		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}

	TCP::acceptor acceptor_;
};
#endif