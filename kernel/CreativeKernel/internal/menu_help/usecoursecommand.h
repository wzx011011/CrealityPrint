#ifndef USECOURSECOMMAND_H
#define USECOURSECOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"


class UseCourseCommand: public ActionCommand
{
	Q_OBJECT
public:
    UseCourseCommand(QObject* parent = nullptr);
    virtual ~UseCourseCommand();
	Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();

};
#endif // USECOURSECOMMAND_H
