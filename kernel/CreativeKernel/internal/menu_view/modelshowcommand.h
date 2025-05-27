#ifndef _MODELSHOWCOMMAND_1595475565671_H
#define _MODELSHOWCOMMAND_1595475565671_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"

class ModelShowCommand : public ActionCommand
{
    Q_OBJECT
public:
    ModelShowCommand(QObject* parent = nullptr);
    ModelShowCommand(EModelShow eType,QObject* parent = nullptr);
    virtual ~ModelShowCommand();

    Q_INVOKABLE void execute();
    void showModelFace();
    void showModelLine();
    void showModelFaceLine();

private:
    int m_nShowType = 0;
private slots:
    void slotLanguageChanged();
};

#endif // _MODELSHOWCOMMAND_1595475565671_H
