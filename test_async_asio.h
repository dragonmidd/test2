#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp> 
#include <iostream>

using namespace std;
using namespace boost::posix_time;

namespace
{
	typedef boost::asio::io_service IoService;
	typedef boost::asio::ip::tcp TCP;

	std::string make_daytime_string()
	{
		using namespace std;
		time_t now = std::time(NULL);
		return string(ctime(&now));
	}

	class tcp_connection
		: public boost::enable_shared_from_this<tcp_connection>
	{
	public:
		typedef boost::shared_ptr<tcp_connection> pointer;

		static pointer create(IoService& io_service)
		{
			return pointer(new tcp_connection(io_service));
		}

		TCP::socket& socket()
		{
			return socket_;
		}

		void send()
		{
//			message_ = make_daytime_string();

			boost::asio::async_write(
				socket_,
				boost::asio::buffer(message_),
				boost::bind(&tcp_connection::handle_write,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}

		void start()
		{
			//			message_ = make_daytime_string();

//			socket_.async_read_some(
			boost::asio::async_read(
				socket_,
				boost::asio::buffer(recvBuf, 512),
				boost::bind(&tcp_connection::handle_read,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
				);
		}

	private:
		tcp_connection(IoService& io_service)
			: socket_(io_service)
			, message_(512, 't')
			, recvBuf(2048,'\0')
			, recvCount(0)
			, millSecStart(microsec_clock::universal_time().time_of_day().total_milliseconds())
		{
		}

		void handle_write(const boost::system::error_code& error,
			size_t bytes_transferred)
		{
//			cout << "write " << bytes_transferred << " bytes" << endl;
//			boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
			send();
		}

		void handle_read(const boost::system::error_code& error,
			size_t bytes_transferred)
		{
			if (!error)
			{
// 				cout << "read " << bytes_transferred << " bytes" << endl;
// 				recvBuf[0] = '\0';

				if (++recvCount>=100000)
				{
					int64_t millSecStop = microsec_clock::universal_time().time_of_day().total_milliseconds();
					int64_t millSecDiff = millSecStop - millSecStart;
					cout << "throughput:" << recvCount*1000 / millSecDiff << "/s" << endl;
					millSecStart = millSecStop;
					recvCount = 0;
				}

				start();
			}
			else
			{
				cout << "read error:" << error << endl;
			}
		}

		TCP::socket socket_;
		std::string message_;
		std::vector<char> recvBuf;
		int64_t recvCount;
		int64_t millSecStart;
	};

	class tcp_server
	{
	public:
		tcp_server(IoService& io_service)
			: acceptor_(io_service, TCP::endpoint(TCP::v4(), 8888))
		{
			start_accept();
		}
	private:
		void start_accept()
		{
			tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_io_service());

			acceptor_.async_accept(
				new_connection->socket(),
				boost::bind(&tcp_server::handle_accept,
				this,
				new_connection,
				boost::asio::placeholders::error));
		}

		void handle_accept(tcp_connection::pointer new_connection,
			const boost::system::error_code& error)
		{
			if (!error)
			{
				cout << "accept a connection request" << endl;
				new_connection->start();
				start_accept();
			}
		}

		TCP::acceptor acceptor_;
	};


	class tcp_client
	{
	public:
		tcp_client(IoService& io_service)
			: endpoint_(boost::asio::ip::address_v4::from_string("127.0.0.1"), 8888)
			, io_service_(io_service)
		{
			start_connect();
		}
	private:
		void start_connect()
		{
			tcp_connection::pointer new_connection = tcp_connection::create(io_service_);

			new_connection->socket().async_connect(
				endpoint_,
				boost::bind(&tcp_client::handle_connect,
				this,
				new_connection,
				boost::asio::placeholders::error));
		}

		void handle_connect(tcp_connection::pointer new_connection,
			const boost::system::error_code& error)
		{
			if (!error)
			{
				cout << "connect to server ok" << endl;
				new_connection->send();
//				new_connection->start();
			}
		}

		TCP::endpoint endpoint_;
		IoService& io_service_;
	};

	// tcp_connection与tcp_server封装后
	void test_asio_asynserver()
	{
		try
		{
			IoService io_service;
			tcp_server server(io_service);

			// 只有io_service类的run()方法运行之后回调对象才会被调用
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// tcp_connection与tcp_server封装后
	void test_asio_asynclient()
	{
		try
		{
			IoService io_service;
			tcp_client server(io_service);

			// 只有io_service类的run()方法运行之后回调对象才会被调用
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}


