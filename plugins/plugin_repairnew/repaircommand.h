#ifndef _NULLSPACE_RepairCOMMAND_1589850876397_H
#define _NULLSPACE_RepairCOMMAND_1589850876397_H
#include "menu/actioncommand.h"
#include "interface/selectorinterface.h"
using namespace creative_kernel;

class RepairOp;
class RepairCommand: public ActionCommand
{
	Q_OBJECT
public:
	RepairCommand();
	virtual ~RepairCommand();

	Q_INVOKABLE void execute();
	Q_INVOKABLE QString getMessageText();
	Q_INVOKABLE void accept();
	Q_INVOKABLE void acceptRepair();

	Q_INVOKABLE void repairModel();
	Q_INVOKABLE void delSelectedModel();

	Q_INVOKABLE int getVerticessizeAll() { return m_nverticessizeAll; }
	Q_INVOKABLE int getFacesizeAll() { return m_facesizeAll; }
	Q_INVOKABLE int getErrorNormalsAll() { return m_errorNormalsAll; }
	Q_INVOKABLE int getErrorEdgesAll() { return m_errorEdgesAll; }
	Q_INVOKABLE int getshellsAll() { return m_shellsAll; }
	Q_INVOKABLE int getholesAll() { return m_holesAll; }
	Q_INVOKABLE int getIntersectsAll() { return m_intersectsAll; }

private  slots:
    void slotLanguageChanged();
	void slotLanguageChanged2();
	void slotRepairSuccess();
	void slotRepair();
	void slotsDetectModel();
protected:
	RepairOp* m_op;
private:
	QString m_strMessageText;
	QList<ModelN*> m_repairModels;

	int m_nverticessizeAll;
	int m_facesizeAll;
	int m_errorNormalsAll;
	int m_errorEdgesAll;
	int m_shellsAll;
	int m_holesAll;
	int m_intersectsAll;
};
#endif // _NULLSPACE_RepairCOMMAND_1589850876397_H
