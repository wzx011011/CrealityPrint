#ifndef _ZSLIDERINFO_H
#define _ZSLIDERINFO_H
#include <QtCore/QObject>
#include <QObject>

#include "modelspaceobserver.h"
class ZSliderInfo: public QObject
        , public SpaceTracer
{

	Q_OBJECT
public:
    ZSliderInfo(QObject* parent = nullptr);
    virtual ~ZSliderInfo();
	void setObject(QObject* object);
    void enableVisible();
    void disableVisible();
    Q_INVOKABLE int layers();
    Q_INVOKABLE void setTopCurrentLayer(float layer);
    Q_INVOKABLE void setBottomCurrentLayer(float layer);
    void initMaxLayer();
    Q_INVOKABLE void  updateZHeight(float layer);
protected slots:
    void onBoxChanged(qtuser_3d::Box3D& box) override;
    void onSceneChanged(qtuser_3d::Box3D& box) override;
    void onGlobalBoxChanged(qtuser_3d::Box3D& box) override {}
signals:
    void maxZHeightChanged();
private:
    QObject* m_object;
    int m_maxLayer =0;
};
#endif // _ZSLIDERINFO_H
