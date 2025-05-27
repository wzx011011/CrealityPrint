#ifndef CX3DEXPORTERCOMMAND_H
#define CX3DEXPORTERCOMMAND_H
#include "menu/actioncommand.h"
#include "inout/cxhandlerbase.h"

class CX3DExporterCommand : public ActionCommand, public CXHandleBase
{
    Q_OBJECT
public:

    CX3DExporterCommand(QObject* parent = nullptr);
    Q_INVOKABLE void execute();
    Q_INVOKABLE QUrl historyPath();
    Q_INVOKABLE QString getMessageText();
    Q_INVOKABLE void accept();
private:
    QString m_strMessageText;
protected:
    void handle(const QString& fileName) override;
private slots:
    void slotLanguageChanged();
    void slotOpenLocalFolder();
};

#endif // CX3DEXPORTERCOMMAND_H
