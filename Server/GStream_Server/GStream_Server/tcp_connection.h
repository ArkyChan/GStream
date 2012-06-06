#ifndef TCP_CONNECTION
#define TCP_CONNECTION

class tcp_connection : public ENABLE_SHARED<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(IO_SERVICE& io_service){
		return pointer(new tcp_connection(io_service));
	}

	SOCK& socket(){
		return socket_;
	}

	void start(){
		ASIO::async_write(socket_, BUFF("Hello :)"),BIND(&tcp_connection::handle_write, shared_from_this(),PLACEHOLDER::error,PLACEHOLDER::bytes_transferred));
	}

private:
	tcp_connection(IO_SERVICE& io_service) : socket_(io_service){
		//init code here
	}

	void handle_write(const boost::system::error_code& ,size_t len){
		_LOG(len + " data wrote",_INFO);
	}

	SOCK socket_;
};

#endif