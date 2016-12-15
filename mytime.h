#pragma once
#include <string>
using namespace std;

#ifdef _WIN32
#include <Windows.h>
#endif

/// ��ȷ��΢���ʱ��ֵ
struct STUMicrSecTime
{
	long                                         m_lSec;
	long                                         m_lMicrSec;

	STUMicrSecTime()
		: m_lSec(0)
		, m_lMicrSec(0)
	{

	}

	/// ����-�����������ʱ���ֵ����λ΢�룩
	const long operator - (const STUMicrSecTime& right) const
	{
		return (m_lSec - right.m_lSec) * 1000000 + (m_lMicrSec - right.m_lMicrSec);
	}

};

/// ΢���
class CMicrSecWatch
{
	

	struct STUTimePoint
	{
		string                                  m_ssLabel;
		STUMicrSecTime                          m_oTime;
	};

public:
	CMicrSecWatch();
	~CMicrSecWatch();

public:
	/// ��ʼ��ʱ
	void Start();


	/// ���ü�ʱ�㣬���ؼ�ʱ���õ�΢����; ��ʾ���ϸ���ʱ�㿪ʼ���˿̵�΢������
	int SetTimePoint(const char* szLabel);


	/// ֹͣ��ʱ��������м�ʱ��
	void Stop();
public:
	/// ��ȡ��ǰʱ�䣬��ȷ��΢��
	static int GetCurrTimeVal(STUMicrSecTime& oTimeVal);


private:
	STUTimePoint                            m_arrTimePoint[6];
	int                                     m_iCount;
};


CMicrSecWatch::CMicrSecWatch()
	: m_iCount(0)
{

}


CMicrSecWatch::~CMicrSecWatch()
{

}


void CMicrSecWatch::Start()
{
	CMicrSecWatch::GetCurrTimeVal(m_arrTimePoint[0].m_oTime);
	m_iCount = 1;
}


int CMicrSecWatch::SetTimePoint(const char* szLabel)
{
	if (m_iCount < 1 || m_iCount>5)
	{
		return -1;
	}

	m_arrTimePoint[m_iCount].m_ssLabel = szLabel ? szLabel : "";
	CMicrSecWatch::GetCurrTimeVal(m_arrTimePoint[m_iCount++].m_oTime);
	return 0;
}


void CMicrSecWatch::Stop()
{
	for (int i = 1; i < m_iCount; i++)
	{
		printf("####Label[%s]:usec=%ld from last####\n", m_arrTimePoint[i].m_ssLabel.c_str(), m_arrTimePoint[i].m_oTime - m_arrTimePoint[i - 1].m_oTime);
	}
}

int CMicrSecWatch::GetCurrTimeVal(STUMicrSecTime& oTimeVal)
{
#ifdef _WIN32

	LARGE_INTEGER liTime;
	LONGLONG llTime;
	double dbFreq, dbTime;

	QueryPerformanceCounter(&liTime);
	llTime = liTime.QuadPart;

	QueryPerformanceFrequency(&liTime);
	dbFreq = (double)liTime.QuadPart;

	dbTime = llTime / dbFreq;

	oTimeVal.m_lSec = (long)dbTime;
	oTimeVal.m_lMicrSec = (long)((dbTime - oTimeVal.m_lSec) * 1000000);

#else
	// 	timeval oSysTimeVal;
	// 	if (gettimeofday(&oSysTimeVal, NULL) != 0)
	// 	{
	// 		return -1;
	// 	}
	// 
	// 	oTimeVal.m_lSec = (long)oSysTimeVal.tv_sec;
	// 	oTimeVal.m_lMicrSec = (long)oSysTimeVal.tv_usec;

	timespec oSysTimeVal;
	if (clock_gettime(CLOCK_REALTIME, &oSysTimeVal) != 0)
	{
		return -1;
	}

	oTimeVal.m_lSec = (long)oSysTimeVal.tv_sec;
	oTimeVal.m_lMicrSec = (long)oSysTimeVal.tv_nsec / 1000;


#endif

	return 0;
}
