#ifndef _NULLSPACE_CREATIVEPLUGINCENTER_1589530786572_H
#define _NULLSPACE_CREATIVEPLUGINCENTER_1589530786572_H
#include <QtCore/QObject>
#include "creativeinterface.h"
#include <QtCore/QMap>

class CreativePluginCenter: public QObject
{
	Q_OBJECT
public:
	CreativePluginCenter(QObject* parent = nullptr);
	virtual ~CreativePluginCenter();

	void load();
	void initialize();
	void uninitialize();
    CreativeInterface* getPluginByName(QString name);
protected:
	QStringList m_searchPathes;

    QMap<QString,CreativeInterface*> m_interfaces;
};
#endif // _NULLSPACE_CREATIVEPLUGINCENTER_1589530786572_H
