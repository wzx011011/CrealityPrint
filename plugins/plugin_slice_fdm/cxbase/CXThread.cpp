#include "cxbase/CXThread.h"
#include <thread>

using namespace std;


void CXSleep(int mis)
{
	chrono::milliseconds du(mis);
	this_thread::sleep_for(du);
}

void CXThread::SetPause(bool isP)
{
	isPause = isP;
	for (int i = 0; i < 10; i++)
	{
		if (isPausing == isP)
		{
			break;
		}
		CXSleep(10);
	}
}
bool CXThread::Start()
{
	isExit = false;
	isPause = false;
	thread th(&CXThread::ThreadMain, this);
	th.detach();
	_threadTimer.update();
	return true;
}

void CXThread::ThreadMain()
{
	isRuning = true;
	printf("threadMain etnter!");
	Main();
	printf("threadMain exit!");
	isRuning = false;
}

void CXThread::Stop()
{
	isExit = true;
	for (int i = 0; i < 200; i++)
	{
		if (!isRuning) {
			printf("Stop thread success");
			return;
		}
		CXSleep(1);
	}
	printf("Stop thread overtime!");

}
