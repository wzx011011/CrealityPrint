#include "cxbase/CXObserver.h"

///主体函数, 添加观察者
void CXObserver::AddObs(CXObserver* obs)
{
	if (!obs) return;
	mux.lock();
	obss.push_back(obs);
	mux.unlock();
}
///通知所有观察者
void CXObserver::Notify(std::string data)
{
	mux.lock();
	for (int i = obss.size() - 1; i >= 0; i--)
	{
		obss[i]->Update(data);
	}
	mux.unlock();
}
///通知所有观察者
void CXObserver::Notify(std::shared_ptr<void> object)
{
	mux.lock();
	for (int i = obss.size() - 1; i >= 0; i--)
	{
		obss[i]->Update(object);
	}
	mux.unlock();
}
