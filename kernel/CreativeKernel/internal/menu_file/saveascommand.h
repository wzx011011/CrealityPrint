#ifndef _SAVEASCOMMAND_H
#define _SAVEASCOMMAND_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"
#include "inout/cxhandlerbase.h"

class SaveAsCommand: public ActionCommand, public CXHandleBase
{
	Q_OBJECT
public:
    SaveAsCommand(QObject* parent = nullptr);
    virtual ~SaveAsCommand();

	Q_INVOKABLE void execute();
	Q_INVOKABLE QString getMessageText();
	Q_INVOKABLE void accept();
private:
	QString m_strMessageText;
protected:
	void handle(const QString& fileName) override;
private slots:
    void slotLanguageChanged();
};
#endif // _SAVEASCOMMAND_H
