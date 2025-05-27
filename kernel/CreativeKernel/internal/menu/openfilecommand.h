#ifndef OPENFILECOMMAND_H
#define OPENFILECOMMAND_H
#include "menu/actioncommand.h"

class OpenFileCommand: public ActionCommand
{
    Q_OBJECT
public:
    OpenFileCommand();
    ~OpenFileCommand();

    Q_INVOKABLE void execute();
public  slots:
    void slotFinished(QString strFilePath);
private:
    QString m_strFilePath;
private slots:
    void slotLanguageChanged();
};

#endif // OPENFILECOMMAND_H
