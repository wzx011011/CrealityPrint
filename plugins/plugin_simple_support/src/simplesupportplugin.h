#ifndef _NULLSPACE_SIMPLESUPPORTPLUGIN_1590465245019_H
#define _NULLSPACE_SIMPLESUPPORTPLUGIN_1590465245019_H
#include "creativeinterface.h"
#include "us/slicermanager.h"
//class SupportCommand;
class SupportUI;
class SimpleSupportPlugin: public QObject, public CreativeInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "creative.SimpleSupportPlugin")
	Q_INTERFACES(CreativeInterface)
public:
	SimpleSupportPlugin(QObject* parent = nullptr);
	virtual ~SimpleSupportPlugin();

protected:
	QString name() override;
	QString info() override;

	void initialize() override;
	void uninitialize() override;
private  slots:
    void slotLanguageChanged();
public slots:
    void visibleLeftBarButton(SlicerManager::SLICERTYPE type);
protected:
   // SupportCommand* m_command;
    SupportUI * m_supportUI;
};
#endif // _NULLSPACE_SIMPLESUPPORTPLUGIN_1590465245019_H
