#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "BoostMacro.h"

namespace Gstream {
	/// Represents a single connection from a client.
	class TcpConnection : public ENABLE_SHARED<TcpConnection>, private boost::noncopyable {
	public:
		explicit TcpConnection(IO_SERVICE& io_service);
		SOCK& socket();
		void start();
		void reset(SHARED_PTR<TcpConnection>*);
	private:
		void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
		void handle_write(const boost::system::error_code& e);
		IO_SERVICE::strand strand_;
		SOCK socket_;
		boost::array<char, 8192> buffer_;
	};

	typedef SHARED_PTR<TcpConnection> connection_ptr;
}
#endif