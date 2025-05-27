#ifndef _VIEWSHOWCOMMAND_H
#define _VIEWSHOWCOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"

class ViewShowCommand : public ActionCommand
{
    Q_OBJECT
public:
    ViewShowCommand(QObject* parent = nullptr);
    ViewShowCommand(EViewShow eType,QObject* parent = nullptr);
    virtual ~ViewShowCommand();

    Q_INVOKABLE void execute();
    void showModelFace();
    void showModelLine();
    void showModelFaceLine();

private:
    int m_nShowType = 0;
private slots:
    void slotLanguageChanged();
};

#endif // _VIEWSHOWCOMMAND_H
