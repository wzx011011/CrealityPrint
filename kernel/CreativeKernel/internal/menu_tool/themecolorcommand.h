#ifndef THEMECOLORCOMMAND_H
#define THEMECOLORCOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"
#include <QMap>
#include <QTranslator>

class ThemeColorCommand : public ActionCommand
{
    Q_OBJECT
public:
    ThemeColorCommand(QObject* parent = nullptr);
    ThemeColorCommand(int nType,QObject* parent = nullptr);
    virtual ~ThemeColorCommand();
    void updateChecked();
    Q_INVOKABLE void execute();

public slots:
    void slotLanguageChanged();
    //void slotThemeChanged();
private:
    int m_ntype =0;
};

#endif
