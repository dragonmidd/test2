#include <stdio.h>
#include "test_boost.h"
#include "test_asio.h"
#include "test_async_asio.h"
#include "test_spsc_queue.h"
#include "test_data_type.h"

int main(int argc, char** argv)
{
//	test_boost_thread();

// #ifdef ASIO_CLIENT
// //	test_asio_synclient();
// 	test_asio_asynclient();
// #else
// //	test_asio_synserver();
// 	test_asio_asynserver();
// #endif

//	test_spsc_queue();

	test_data_type();

	getchar();

}
