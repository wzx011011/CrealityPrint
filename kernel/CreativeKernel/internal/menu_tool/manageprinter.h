#ifndef _MANAGEPRINTE_H
#define _MANAGEPRINTE_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"
#include "managematerialfile.h"

class ManagePrinter : public ActionCommand
{
	Q_OBJECT
public:
    ManagePrinter(QObject* parent = nullptr);
    virtual ~ManagePrinter();

	Q_INVOKABLE void execute();
protected:
	void requestMenuDialog(QObject* receiver);
private slots:
    void slotLanguageChanged();
private:
    QObject* m_obj;

};

#endif // _MANAGEPRINTE_H
