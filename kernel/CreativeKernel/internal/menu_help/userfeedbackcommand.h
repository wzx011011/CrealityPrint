#ifndef USERFEEDBACKCOMMAND_H
#define USERFEEDBACKCOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"


class UserFeedbackCommand : public ActionCommand
{
	Q_OBJECT
public:
    UserFeedbackCommand(QObject* parent = nullptr);
    virtual ~UserFeedbackCommand();
	Q_INVOKABLE void execute();
private slots:
    void slotLanguageChanged();

};
#endif
