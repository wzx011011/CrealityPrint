#ifndef _MODELSELECTIONPROXY_1596685509418_H
#define _MODELSELECTIONPROXY_1596685509418_H
#include <QtCore/QObject>
#include "qtuser3d/module/pickableselecttracer.h"

class ModelSelectionProxy : public QObject , public qtuser_3d::SelectorTracer
{
	Q_OBJECT
public:
	ModelSelectionProxy(QObject* parent = nullptr);
	virtual ~ModelSelectionProxy();

	void setSelectionProxy(QObject* object);

signals:
    void selectChanged();

protected:
	void onSelectionsChanged() override;
	void selectChanged(qtuser_3d::Pickable* pickable) override;

protected:
	QObject* m_selectionProxy;
};

#endif // _MODELSELECTIONPROXY_1596685509418_H
