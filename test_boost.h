#include <boost/thread/thread.hpp> 
#include <iostream> 

void hello()
{
	std::cout << "hello world!" << std::endl;
}

void test_boost_thread()
{
	boost::thread thrd(&hello);
	thrd.join();
}
