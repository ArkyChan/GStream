#include "TcpConnection.h"

namespace Gstream {
	TcpConnection::TcpConnection(IO_SERVICE& io_service) : strand_(io_service), socket_(io_service) {

	}

	SOCK& TcpConnection::socket() {
		return socket_;
	}

	void TcpConnection::start() {
		socket_.async_read_some(BUFF(buffer_),strand_.wrap(BIND(&TcpConnection::handle_read, shared_from_this(),PLACEHOLDER::error,PLACEHOLDER::bytes_transferred)));
	}

	void TcpConnection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred) {
		if (!e)  {
			if (bytes_transferred) {
				//boost::asio::async_write(socket_, 0/*STUFF*/, strand_.wrap(boost::bind(&TcpConnection::handle_write, shared_from_this(), PLACEHOLDER::error)));
			}
		}
	}

	void TcpConnection::handle_write(const boost::system::error_code& e){
		if (!e)  {
		}
	}
}