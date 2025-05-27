#ifndef LANGUAGECOMMAND_H
#define LANGUAGECOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"
#include <QMap>
#include <QTranslator>

class LanguageCommand : public ActionCommand
{
    Q_OBJECT
public:
    LanguageCommand(QObject* parent = nullptr);
    LanguageCommand(int nType,QObject* parent = nullptr);
    virtual ~LanguageCommand();
    void updateChecked();
    Q_INVOKABLE void execute();
    void requestMenuDialog();
public slots:
    void slotLanguageChanged();
private:
    int m_ntype =0;
};

#endif // LANGUAGECOMMAND_H
