#include "tips.h"
#include "qtusercore/string/detaildescription.h"

namespace creative_kernel
{
	Tips::Tips(QObject* parent)
		:QObject(parent)
	{
	}

	Tips::~Tips()
	{
	}

	void Tips::show(qtuser_core::DetailDescription* details)
	{
		if (!details) return;

		for (TipsObserver* observer : m_tipsObservers)
			observer->onTips(details);

		emit showDetails(details);
	}

	void Tips::addTipsObserver(TipsObserver* observer)
	{
		if(observer) m_tipsObservers.push_back(observer);
	}

	void Tips::removeTipsObserver(TipsObserver* observer)
	{
		if(observer) m_tipsObservers.removeOne(observer);
	}
}
