#ifndef ABOUTUSCOMMAND_H
#define ABOUTUSCOMMAND_H

#include <QObject>
#include "menu/actioncommand.h"

class AboutUsCommand: public ActionCommand
{
    Q_OBJECT
public:


    AboutUsCommand();
    ~AboutUsCommand();

    Q_INVOKABLE void execute();
    Q_INVOKABLE QString getWebsite();
    Q_INVOKABLE QString getCurrentVersion();
    void  requestMenuDialog();
    //void requestOpenFileDialog(QObject* receiver);
private slots:
    void slotLanguageChanged();
};

#endif // ABOUTUSCOMMAND_H
