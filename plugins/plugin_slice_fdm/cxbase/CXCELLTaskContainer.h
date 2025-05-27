#ifndef _CXCELL_TASK_H_
#define _CXCELL_TASK_H_

#include <mutex>
#include <list>
#include <functional>
#include "CXThread.h"

class CXCELLTaskContainer :public CXThread
{
public:
	int m_container_id = -1;
private:
	///暂时支持无参无返回值的任务
	typedef std::function<void()> CXCELLTask;

public:
	void AddTask(CXCELLTask task);
	void Close()
	{
		Stop();
		m_taskBuf.clear();
		m_tasks.clear();
	}
	/////执行任务入口
	virtual void Main();
private:
	//待执行任务
	std::list<CXCELLTask> m_tasks;
	//任务缓冲
	std::list<CXCELLTask> m_taskBuf;
	std::mutex m_mutex;
};


#endif
