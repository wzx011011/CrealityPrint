#ifndef _NULLSPACE_MESHLOADERWRAPPER_1590982007351_H
#define _NULLSPACE_MESHLOADERWRAPPER_1590982007351_H
#include "creativekernelexport.h"
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QList>
#include "inout/cxhandlerbase.h"
#include "data/modeln.h"

using namespace creative_kernel;

class CREATIVE_KERNEL_API MeshLoaderWrapper : public QObject, public CXHandleBase
{
	Q_OBJECT

public:
	MeshLoaderWrapper(QObject* parent = nullptr);
	virtual ~MeshLoaderWrapper();

	void load(const QStringList& fileNames);

    Q_INVOKABLE void finishOpen(QString strCurrentFile);
	Q_INVOKABLE void adaptPlatform();//accept();
	Q_INVOKABLE void delSelectedModel();//cancel();
	Q_INVOKABLE QString getMessageText();

public:
	QStringList supportFilters() override; 
	QStringList enableFilters() override;
	void handle(const QString& fileName) override;
	void handle(const QStringList& fileNames) override;

signals:
    void sigFinished(QString str);
	void sigDetectModel();
	void sigSupportFiltersChanged(QStringList newExtensions);

public slots:
	void finishJob();
	void slotSupportFiltersChanged(QStringList newExtensions);
};
#endif // _NULLSPACE_MESHLOADERWRAPPER_1590982007351_H
