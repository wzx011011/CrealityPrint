#ifndef _MANAGEMATERIAL_H
#define _MANAGEMATERIAL_H
#include "menu/actioncommand.h"
#include "../menu/menuCmdDataType.h"
#include "managematerialfile.h"

class ManageMaterial : public ActionCommand
{
	Q_OBJECT
public:
	ManageMaterial(QObject* parent = nullptr);
	virtual ~ManageMaterial();

	Q_INVOKABLE void execute();
	Q_INVOKABLE QString getValue(QString key);
	Q_INVOKABLE void materialChanged(QString type);
	Q_INVOKABLE void valueChanged(QString key,QString value);
	Q_INVOKABLE void addMaterial(QString newName,QString oldName);
	Q_INVOKABLE void resetMaterial();
	Q_INVOKABLE void deleteMaterial();
    Q_INVOKABLE QStringList getMaterialList();
	Q_INVOKABLE QString getMoneyType();

protected:
	void requestMenuDialog(QObject* receiver);
private slots:
    void slotLanguageChanged();
private:
    QObject* m_obj;
	MangeMaterialFile* m_file;
};

#endif // _MANAGEMATERIAL_H
