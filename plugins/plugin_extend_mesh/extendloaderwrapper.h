#ifndef _NULLSPACE_EXTENDLOADERWRAPPER_1590982007351_H
#define _NULLSPACE_EXTENDLOADERWRAPPER_1590982007351_H
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QList>
#include "inout/cxhandlerbase.h"
#include "photo2mesh.h"
class ExtendLoaderWrapper : public QObject, public CXHandleBase
{
	Q_OBJECT

public:
	ExtendLoaderWrapper(QObject* parent = nullptr);
	virtual ~ExtendLoaderWrapper();

	QStringList supportFilters();

	void load(const QStringList& fileNames);

    Q_INVOKABLE void finishOpen(QString strCurrentFile);
	/*Q_INVOKABLE void adaptFunction();*/

	Q_INVOKABLE void accept();
	Q_INVOKABLE void cancel();

    Q_INVOKABLE void setBaseHeight(float baseHeight);
    Q_INVOKABLE void setMaxHeight(float maxHeight);

    Q_INVOKABLE void setLighterOrDarker(int index);
//    Q_INVOKABLE void lighterIsHigh();
//    Q_INVOKABLE void darkerIsHigh();

    Q_INVOKABLE void setMeshWidth(float meshX);
     Q_INVOKABLE void setBlur(int blur);

	 Q_INVOKABLE void adaptPlatform();
	 Q_INVOKABLE void delSelectedModel();
	 Q_INVOKABLE QString getMessageText();

public:
	void handle(const QString& fileName) override;
	void handle(const QStringList& fileNames) override;
signals:
    void sigFinished(QString str);
public slots:
	void finishJob();

private:
    QStringList m_filesName;
    Photo2Mesh m_pt2mesh;
};
#endif // _NULLSPACE_EXTENDLOADERWRAPPER_1590982007351_H
