#include "TcpServer.h"

namespace Gstream {
	TcpServer::TcpServer(const std::string& address, const std::string& port,FuncPointer* callBacks) : signals_(io_service_), acceptor_(io_service_), new_connection_() {
		func_ptr = callBacks;
		signals_.add(SIGINT);
		signals_.add(SIGTERM);
#if defined(SIGQUIT)
		signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
		signals_.async_wait(BIND(&TcpServer::handle_stop, this));

		TCP::resolver resolver(io_service_);
		TCP::resolver::query query(address, port);
		TCP::endpoint endpoint = *resolver.resolve(query);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(TCP::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();

		start_accept();
	}

	void TcpServer::run(){
		std::vector<boost::shared_ptr<boost::thread> > threads;
		boost::shared_ptr<boost::thread> thread(new boost::thread(BIND(&IO_SERVICE::run, &io_service_)));
		threads.push_back(thread);

		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
	}

	void TcpServer::start_accept()	{
		new_connection_.reset(new TcpConnection(io_service_,func_ptr));
		acceptor_.async_accept(new_connection_->socket(),BIND(&TcpServer::handle_accept, this,PLACEHOLDER::error));
	}

	void TcpServer::handle_accept(const boost::system::error_code& e)	{
		if (!e)	{
			new_connection_->start();
		}

		start_accept();
	}

	void TcpServer::handle_stop(){
		io_service_.stop();
	}
}