#ifndef _CXCellTime_hpp_
#define _CXCellTime_hpp_

#include <chrono>

using namespace std::chrono;

class CELLTimestamp
{
public:
	CELLTimestamp()
	{
		update();
	}
	~CELLTimestamp()
	{}

	void update()
	{
		_begin = high_resolution_clock::now();
	}
	/**
	*   获取当前秒
	*/
	double getElapsedSecond()
	{
		return  getElapsedTimeInMicroSec() * 0.000001;
	}
	/**
	*   获取毫秒
	*/
	double getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}
	/**
	*   获取微秒
	*/
	long long getElapsedTimeInMicroSec()
	{
		return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
	}
	time_t getNowInMilliSec()
	{
		return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
	}
protected:
	time_point<high_resolution_clock> _begin;
};

#endif // !_CELLTimestamp_hpp_
