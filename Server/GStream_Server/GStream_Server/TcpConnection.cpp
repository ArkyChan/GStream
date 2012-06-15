#include "TcpConnection.h"

namespace Gstream {
	TcpConnection::TcpConnection(IO_SERVICE& io_service,FuncPointer* callBacks) : strand_(io_service), socket_(io_service) {
		func_ptr = callBacks;
	}

	SOCK& TcpConnection::socket() {
		return socket_;
	}

	void TcpConnection::start() {
		getHostName();
		_LOG("Client connected.",hostName.c_str());
		read();
	}

	void TcpConnection::read(){
		socket_.async_read_some(BUFF(incomming_packet),strand_.wrap(BIND(&TcpConnection::handle_read, shared_from_this(),PLACEHOLDER::error,PLACEHOLDER::bytes_transferred)));
	}
	void TcpConnection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred) {
		if (!e)  {
			if (bytes_transferred) {
				try{
					uint8_t c = (uint8_t)incomming_packet[0];
					
					if(c >= P_ID_COUNT || c < 0){
						std::cout << c << std::endl;
						throw std::exception("Out of range");
					}
					func_ptr[c](&socket_);
					read();
				}catch(std::exception&){
					invalidData();
					read();
				}
			}
		}
	}
	void TcpConnection::getHostName(){
		if(hostName.empty()){
			ASIO::ip::address addr = socket_.remote_endpoint().address();
			hostName = addr.to_string();
			_MAKE_IP(hostName);
		}
	}
	void TcpConnection::invalidData(){
		_LOG("Invalid packet ID recieved",hostName.c_str());
		boost::asio::async_write(socket_, BUFF("Invalid packet ID!"), strand_.wrap(boost::bind(&TcpConnection::handle_write, shared_from_this(), PLACEHOLDER::error)));
	}
	void TcpConnection::handle_write(const boost::system::error_code& e){
		if (!e)  {
		}
	}
}