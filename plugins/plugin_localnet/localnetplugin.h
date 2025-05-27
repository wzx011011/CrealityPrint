#ifndef _LOCALNETPLUGIN_H
#define _LOCALNETPLUGIN_H
#include "creativeinterface.h"
#include <QtQml/QQmlComponent>

class LocalNetCommand;
class CusListModel;
class LocalNetPlugin: public QObject, public CreativeInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "creative.InfoPlugin")
    Q_INTERFACES(CreativeInterface)
public:
    LocalNetPlugin(QObject* parent = nullptr);
    virtual ~LocalNetPlugin();

    QString name() override;
    QString info() override;

    void initialize() override;
    void uninitialize() override;

protected:
    QObject* m_root;

    QQmlComponent* m_localnetcomponent;

    QObject* m_localNetUI;
    LocalNetCommand* m_localNetCmd;
    CusListModel* m_PrinterList = nullptr;

};
#endif // _LOCALNETPLUGIN_H
