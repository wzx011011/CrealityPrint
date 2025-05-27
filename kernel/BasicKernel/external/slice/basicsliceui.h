#ifndef _NULLSPACE_BASIC_SLICEUI_1590162522691_H
#define _NULLSPACE_BASIC_SLICEUI_1590162522691_H
#include "basickernelexport.h"
#include <QtCore/QObject>
#include <QtGui/QImage>
#include "slice/imgprovider.h"

class ModelSelectionProxy;
class BASIC_KERNEL_API BasicSliceUI: public QObject
{
    Q_OBJECT
public:
	BasicSliceUI(QObject* parent = nullptr);
	virtual ~BasicSliceUI();

	virtual void setObject(QString objname, QObject* object);
	virtual void setParam(QString objname, void* param);

	virtual int initialize();
	virtual int uninitialize();

	virtual int onSliceSuccess(QObject* object = nullptr, void* param = nullptr);
	virtual int onSliceFailed(QObject* object = nullptr, void* param = nullptr);
	virtual int handle(QObject* object = nullptr, void* param = nullptr);
	virtual int onExporSuccess(QObject* object = nullptr, void* param = nullptr);
	virtual void show(bool show);
	virtual void update();

	QObject* findSetObject(QString objname);

 public slots:
    virtual void unpreview()=0;
	virtual void layerImgChange(const QImage& img);

	virtual void preview()=0;

protected:
	QMap<QString, QObject*> m_setObjects;

};
#endif // _NULLSPACE_BASIC_SLICEUI_1590162522691_H
