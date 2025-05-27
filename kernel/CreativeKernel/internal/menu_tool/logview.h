#ifndef _LOGVIEW_H
#define _LOGVIEW_H
#include "menu/actioncommand.h"

class LogView : public ActionCommand
{
    Q_OBJECT
public:
    LogView(QObject* parent = nullptr);
    virtual ~LogView();

    Q_INVOKABLE void execute();
protected:
    void requestMenuDialog(QObject* receiver);
private slots:
    void slotLanguageChanged();
private:
    //QObject* m_obj;

};

#endif
