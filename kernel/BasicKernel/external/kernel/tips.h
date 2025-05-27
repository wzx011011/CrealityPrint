#ifndef _CREATIVE_KERNEL_TIPS_1590388899689_H
#define _CREATIVE_KERNEL_TIPS_1590388899689_H
#include "basickernelexport.h"
#include <QtCore/QList>

namespace qtuser_core
{
	class DetailDescription;
}
namespace creative_kernel
{
	class TipsObserver
	{
	public:
		virtual ~TipsObserver() {}

		virtual void onTips(qtuser_core::DetailDescription* details) = 0;
	};

	class BASIC_KERNEL_API Tips: public QObject
	{
		Q_OBJECT
	public:
		Tips(QObject* parent = nullptr);
		virtual ~Tips();

		void show(qtuser_core::DetailDescription* details);
		void addTipsObserver(TipsObserver* observer);
		void removeTipsObserver(TipsObserver* observer);
	signals:
		void showDetails(QObject* object);
	protected:
		QList<TipsObserver*> m_tipsObservers;
	};
}
#endif // _CREATIVE_KERNEL_TIPS_1590388899689_H
