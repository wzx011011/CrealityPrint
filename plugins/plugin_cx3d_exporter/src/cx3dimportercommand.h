#ifndef CX3DIMPORTERCOMMAND_H
#define CX3DIMPORTERCOMMAND_H
#include <QtCore/QObject>
#include "inout/cxhandlerbase.h"
#include "menu/actioncommand.h"

class CX3DImporterCommand : public ActionCommand, public CXHandleBase
{
    Q_OBJECT
    Q_PROPERTY(QStringList nameFilters READ nameFilters CONSTANT)
public:

    CX3DImporterCommand(QObject* parent = nullptr);
    Q_INVOKABLE void execute();
    QStringList nameFilters();
    void setNewPath(QString pathName);

    QStringList supportFilters() override;
    QStringList enableFilters() override;
    void handle(const QString& fileName) override;
public slots:
    void slotSaveLastPro();
    void slotCancelSaveLastPro();
private slots:
    void slotLanguageChanged();

private:
    QString m_strNewPath;
};

#endif // CX3DIMPORTERCOMMAND_H
