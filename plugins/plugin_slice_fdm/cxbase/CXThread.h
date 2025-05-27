#ifndef _CX_THREAD_H
#define _CX_THREAD_H
#include"CXCellTime.h"

void CXSleep(int mis);
class CXThread {
public:
	virtual bool Start();
	virtual void Stop();
	virtual void SetPause(bool isP);
	virtual bool IsPause()
	{
		isPausing = isPause;
		return isPause;
	}
	///入口主函数
	virtual void Main() {}

protected:
	bool isRuning = false;
	bool isExit = false; ///线程控制
	bool isPause = false;
	bool isPausing = false;

	CELLTimestamp _threadTimer;
private:
	void ThreadMain();
};

#endif
