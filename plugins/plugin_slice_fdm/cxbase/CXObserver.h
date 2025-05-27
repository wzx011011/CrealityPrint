#ifndef _CX_OBSERVER_H
#define _CX_OBSERVER_H

#include "CXThread.h"
#include <vector>
#include <mutex>
#include <string>
#include <memory>
class CXObserver :public CXThread {
public:
	virtual void Update(std::string data) {}
	virtual void Update(std::shared_ptr<void> object) {}

	void AddObs(CXObserver* obs);

	void Notify(std::string data);

	void Notify(std::shared_ptr<void> object);

protected:
	std::vector<CXObserver*> obss;
	std::mutex mux;
};


#endif //XPLAY_IOBSERVER_H
