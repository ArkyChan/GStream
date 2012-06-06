#include "tcp_connection.h"

namespace Gstream {
	tcp_connection::tcp_connection(IO_SERVICE& io_service) : strand_(io_service), socket_(io_service) {

	}

	SOCK& tcp_connection::socket() {
		return socket_;
	}

	void tcp_connection::start() {
		socket_.async_read_some(BUFF(buffer_),strand_.wrap(BIND(&tcp_connection::handle_read, shared_from_this(),PLACEHOLDER::error,PLACEHOLDER::bytes_transferred)));
	}

	void tcp_connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred) {
		if (!e)  {
			if (bytes_transferred) {
				//boost::asio::async_write(socket_, 0/*STUFF*/, strand_.wrap(boost::bind(&tcp_connection::handle_write, shared_from_this(), PLACEHOLDER::error)));
			}
		}
	}

	void tcp_connection::handle_write(const boost::system::error_code& e){
		if (!e)  {
		}
	}
}