#ifndef _NEWPROJECTCOMMAND_H
#define _NEWPROJECTCOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"

class NewProjectCommand: public ActionCommand
{
	Q_OBJECT
public:
    NewProjectCommand(QObject* parent = nullptr);
    virtual ~NewProjectCommand();
	Q_INVOKABLE void execute();
    void requestMenuDialog();
public slots:
    void saveProject(QString projectName);
private slots:
    void slotLanguageChanged();

};
#endif // _NEWPROJECTCOMMAND_H
