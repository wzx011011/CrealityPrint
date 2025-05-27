
/*异步执行一些细小任务，将任务(函数)，而不是消息或对象，加入到执行队列，执行
此功能需要在c++11以上，支持lambada表达示*/

#include"cxthread.h"
#include"cxcelltaskcontainer.h"

void CXCELLTaskContainer::AddTask(CXCELLTask task)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_taskBuf.push_back(task);
}
/////执行任务入口
void CXCELLTaskContainer::Main()
{
	while (!isExit)
	{
		//从缓冲区取出数据
		if (!m_taskBuf.empty())
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			for (auto pTask : m_taskBuf)
			{
				m_tasks.push_back(pTask);
			}
			m_taskBuf.clear();
		}
		//如果没有任务
		if (m_tasks.empty())
		{
			CXSleep(10);
			continue;
		}
		//处理任务
		for (auto pTask : m_tasks)
		{
			pTask();
		}
		m_tasks.clear();
		//处理缓冲队列中的任务
		for (auto pTask : m_taskBuf)
		{
			pTask();
		}
	}
}
