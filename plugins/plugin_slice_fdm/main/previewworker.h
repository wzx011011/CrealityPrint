#ifndef _PREVIEWWORKER_1603796964788_H
#define _PREVIEWWORKER_1603796964788_H
#include "inout/cxhandlerbase.h"
#include <QtCore/QObject>
#include <QtCore/QStringList>

class SlicePlugin;
class PreviewWorker : public QObject, public CXHandleBase
{
public:
	PreviewWorker(QObject* parent = nullptr);
	virtual ~PreviewWorker();
public:

	QStringList supportFilters() override;
	QStringList enableFilters() override;
protected:
	void handle(const QString& fileName) override;
protected:
	SlicePlugin* m_receiver;
};

#endif // _PREVIEWWORKER_1603796964788_H