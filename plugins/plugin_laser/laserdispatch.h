#ifndef _NULLSPACE_LASERDISPATCH_1589986318632_H
#define _NULLSPACE_LASERDISPATCH_1589986318632_H
#include <QtCore/QObject>
#include "qtuser3d/module/pickableselecttracer.h"
#include <QtGui/QVector3D>
#include "modelspaceobserver.h"
class LaserDispatch: public QObject
    , public qtuser_3d::SelectorTracer
        , public SpaceTracer
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QVector3D size READ size)
public:
    LaserDispatch(QObject* parent = nullptr);
    virtual ~LaserDispatch();

	QString text();
    QVector3D size();
	void setText(const QString& text);
	void setObject(QObject* object);
protected:
	void onSelectionsChanged() override;
	void selectChanged(qtuser_3d::Pickable* pickable) override;

    void onBoxChanged(qtuser_3d::Box3D& box) override;
    void onSceneChanged(qtuser_3d::Box3D& box) override;
	void onGlobalBoxChanged(qtuser_3d::Box3D& box) override {}
protected slots:
    void enableVisible();
    void disableVisible();
signals:
	void textChanged();
protected:
	QString m_text;
	QObject* m_object;
};
#endif // _NULLSPACE_LASERDISPATCH_1589986318632_H
