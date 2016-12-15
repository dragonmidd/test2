#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

typedef boost::asio::io_service IOS;
typedef boost::asio::ip::tcp TCP;

using namespace std;
using namespace boost::posix_time;

void print_str(const char* str)
{
	std::cout << str << std::endl;
}

void test_asio_synserver()
{
	try
	{
		IOS ios;
		/*
		*���¹��캯���൱�����²���
		* basic_socket_acceptor<tcp> acceptor(io_service);
		* acceptor.open(tcp::v4());
		* acceptor.bind(tcp::endpoint(13));
		* acceptor.listen(0);//default
		*/
		/*
		tcp::endpoint(�˵�)�����������������
		1. IP��ַ(address, address_v4, address_v6)
		2. �˿ں�
		3. Э��汾
		*/
		TCP::acceptor acceptor(ios, TCP::endpoint(TCP::v4(), 8888));
		boost::system::error_code error;

		TCP::socket socket(ios);
		acceptor.accept(socket);
		std::cout << "client:" << socket.remote_endpoint().address() << std::endl;
		std::string message(128, 't');
		std::vector<char> buf(2048);

		int i = 0;
		int64_t millSecStart = microsec_clock::universal_time().time_of_day().total_milliseconds();
		while (true)
		{	
// 			int64_t microSecSend = microsec_clock::universal_time().time_of_day().total_microseconds();
// 			std::string messageSend = message + std::to_string(microSecSend);
				socket.write_some(boost::asio::buffer(message));
//			boost::system::error_code ignored_error;
// 			boost::asio::write(socket,
// 				boost::asio::buffer(message),
// 				boost::asio::transfer_all(),
// 				ignored_error);
//			std::vector<char> buf(2048);
// 			size_t len = socket.read_some(boost::asio::buffer(buf), error);
// 			// ����ҲҪ�ж�һ��, �����������жϿ�, ����������ѭ��.
// 			if (error == boost::asio::error::eof)
// 				break; // Connection closed cleanly by peer.
// 			else if (error)
// 				throw boost::system::system_error(error); // Some other error.
// 			int64_t microSecRecv = microsec_clock::universal_time().time_of_day().total_microseconds();
// 			std::cout << "send toto client:" << microSecSend << std::endl;
// 			std::cout << "recv from client:" << microSecRecv << std::endl;
// 			boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
// 			if (++i >= 100000)
// 			{
// 				int64_t millSecStop = microsec_clock::universal_time().time_of_day().total_milliseconds();
// 				int64_t millSecDiff = millSecStop - millSecStart;
// 				cout << "throughput:" << i*1000 / millSecDiff << "/s" << endl;
// 				millSecStart = millSecStop;
// 				i = 0;
// 			}

		}
	}
	catch (std::exception& e)
	{
		print_str(e.what());
	}
}

void test_asio_synclient()
{
	try
	{
		IOS ios;
		boost::system::error_code error;

		// 1. �ý������ķ���, ��������������, ��
		// query("www.163.com","80"), Ҳ���� query("www.163.com","telnet")
		// echo          7/tcp
		// ftp           21/tcp                 # File Transfer Protocol (Control)
		// telnet        23/tcp                 # Virtual Terminal Protocol
		// smtp          25/tcp                 # Simple Mail Transfer Protocol
		// time          37/tcp  timeserver     # Time
		// 		TCP::resolver resolver(ios);
		// 		TCP::resolver::query query("127.0.0.1", "10000");
		// 		TCP::resolver::iterator endpoint_iterator = resolver.resolve(query);
		// 		TCP::resolver::iterator end;
		// 		TCP::socket socket(ios);
		// 		socket.connect(*endpoint_iterator, error);

		// 2. �ü��ķ���
		TCP::socket socket(ios);
		TCP::endpoint endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 8888);
		socket.connect(endpoint, error);

		// ����Ҫ�ж�һ��, ����û�����ϻ�ͨ��.
		if (error)
			throw boost::system::system_error(error);

		int i = 0;
		std::vector<char> buf(2048);
		int64_t millSecStart = microsec_clock::universal_time().time_of_day().total_milliseconds();
		while (true)
		{
			// boost::array<char, 128> buf;
//			std::vector<char> buf(2048);
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			
			// ����ҲҪ�ж�һ��, �����������жϿ�, ����������ѭ��.
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

// 			int64_t microSecSend= std::atoll(&buf[1024]);
// 			int64_t microSecRecv = microsec_clock::universal_time().time_of_day().total_microseconds();
// 			std::cout << "recv from server:" << microSecRecv << "-" << microSecSend << "=" << microSecRecv-microSecSend << std::endl;

			// bufҪע����ƴ�С
//			socket.write_some(boost::asio::buffer(buf, len), error);

			if (++i >= 1000000)
			{
				int64_t millSecStop = microsec_clock::universal_time().time_of_day().total_milliseconds();
				int64_t millSecDiff = millSecStop - millSecStart;
				cout << "throughput:" << i*1000 / millSecDiff << "/s" << endl;
				millSecStart = millSecStop;
				i = 0;
			}
		}
	}
	catch (std::exception& e)
	{
		print_str(e.what());
	}
}
