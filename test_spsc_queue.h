#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include "mytime.h"

using namespace std;
using namespace boost::posix_time;
typedef boost::lockfree::spsc_queue<int64_t, boost::lockfree::capacity<102400> >  SPSC_QUEUE;
// typedef boost::lockfree::queue<long>  SPSC_QUEUE;

void producer(SPSC_QUEUE& spscQueue)
{
	int64_t value = 0;
	while (true)
	{
//		int64_t usPush = microsec_clock::universal_time().time_of_day().total_microseconds();
//		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::microsec(1));

// 		STUMicrSecTime* usPush = new STUMicrSecTime();
// 		CMicrSecWatch::GetCurrTimeVal(*usPush);

// 		LARGE_INTEGER usPush;
// 		QueryPerformanceCounter(&usPush);
// 		double db1 = 17.7113456;
// 		double db2 = 98.7354633;
// 		int64_t i = 0;
// 		int64_t loopCount = 100;
// 		while (++i<=loopCount)
// 		{		
// 			db2 *= db2 / db1 + db1*db2 + db1 / db2;
// 			db1 *= db2 / db1 + db1*db2 + db1 / db2;
// 
// 			int64_t j = 0;
// 			double db3 = db2*db1*db2*db1*1024.18036476;
// 			while (++j<=0)
// 			{
// 				db2 *= db2*db3 / db1 + db1*db3*db2 + db1*db3 / db2;
// 				db1 *= db2 / (db3*db1) + db1*db3*db2 + db1 / (db3*db2);
// 				db3 = db2*db1*db2*db1*1024.18036476;
// 			}
// 		}
//		spscQueue.push((int64_t)usPush.QuadPart);
		while (!spscQueue.push(++value));
//		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
//		while (!spscQueue.push(++value));
// 		if (value>=1000000000)
// 		{
// 			cout << "produce end:" << value << endl;
// 			break;
// 		}
	}
}

void consumer(SPSC_QUEUE& spscQueue)
{
	int64_t value = 0;
	int64_t count = 0;
	int64_t millSecStart = microsec_clock::universal_time().time_of_day().total_milliseconds();
	while (true)
	{
		while (spscQueue.pop(value))
		{
//			int64_t usPop = microsec_clock::universal_time().time_of_day().total_microseconds();
// 			int64_t nsPush = value;
// 			STUMicrSecTime usPop;
// 			CMicrSecWatch::GetCurrTimeVal(usPop);
// 			STUMicrSecTime* usPush = (STUMicrSecTime*)value;
			
// 			LARGE_INTEGER usPop;
// 			QueryPerformanceCounter(&usPop);
// 
// 			LARGE_INTEGER freq;
// 			QueryPerformanceFrequency(&freq);
//			int64_t usPush = value;
//			cout << "push->pop:" << (usPop.QuadPart - usPush)*1000000/freq.QuadPart << "us" << endl;
//			cout << "push->pop:" << (usPop - usPush) << "us" << endl;

			if (++count>=100000000)
			{
				int64_t millSecStop = microsec_clock::universal_time().time_of_day().total_milliseconds();
				int64_t millSecDiff = millSecStop - millSecStart;
				cout << "throughput:" << count/(1000*millSecDiff) << "bw/s" << endl;
				millSecStart = millSecStop;
				count = 0;
			}
		}
	}
}


void test_spsc_queue()
{
	SPSC_QUEUE spscQueue;
	boost::thread producerThrd(boost::bind(&producer, boost::ref(spscQueue)));
	boost::thread consumerThrd(boost::bind(&consumer, boost::ref(spscQueue)));
	producerThrd.join();
	consumerThrd.join();
}

