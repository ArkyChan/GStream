#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "TcpConnection.h"
#include "BoostMacro.h"

namespace Gstream{
	class TcpServer : private boost::noncopyable{
	public:
		explicit TcpServer(const std::string& address, const std::string& port);
		void run();
	private:
		void start_accept();
		void handle_accept(const boost::system::error_code& e);
		void handle_stop();
		IO_SERVICE io_service_;
		ASIO::signal_set signals_;
		TCP::acceptor acceptor_;
		TcpConnection new_connection_;
	};
}
#endif