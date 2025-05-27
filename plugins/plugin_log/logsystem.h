#ifndef _NULLSPACE_LOGSYSTEM_1589943065396_H
#define _NULLSPACE_LOGSYSTEM_1589943065396_H
#include "creativeinterface.h"

class LogSystem: public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.LogSystem")
	Q_INTERFACES(CreativeInterface)
public:
	LogSystem();
	virtual ~LogSystem();

	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;
};
#endif // _NULLSPACE_LOGSYSTEM_1589943065396_H
