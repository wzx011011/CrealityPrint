#ifndef CX3DCLOSEPROJECTCOMMAND_H
#define CX3DCLOSEPROJECTCOMMAND_H

#include <QObject>
#include <QUrl>
#include "menu/actioncommand.h"
#include "inout/cxhandlerbase.h"
class Cx3dCloseProjectCommand:public ActionCommand,public CXHandleBase
{
    Q_OBJECT
public:
    Cx3dCloseProjectCommand();
    ~Cx3dCloseProjectCommand();
    Q_INVOKABLE void execute();
    void requestMenuDialog();
    Q_INVOKABLE void saveFile();
    Q_INVOKABLE void noSaveFile();
    Q_INVOKABLE QUrl historyPath();
    Q_INVOKABLE void delDefaultProject();
    void saveProPath();
protected:
    void handle(const QString& fileName) override;
private slots:
    void slotLanguageChanged();
    void finishJob();

    void slotCloseWindow();
};

#endif // CX3DCLOSEPROJECTCOMMAND_H
