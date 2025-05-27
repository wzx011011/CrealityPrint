#ifndef ABSTRACT_KERNEL_1592188654160_H
#define ABSTRACT_KERNEL_1592188654160_H
#include "basickernelexport.h"
#include "qtuserqml/plugin/toolcommand.h"
#include "qtuserqml/plugin/treemodel.h"
#include "slice/cusModelListModel.h"
#include "menu/actioncommand.h"
#include "slice/basicsliceui.h"
#include "kernel/abstractappsetting.h"
#include "tablemodel.h"

#include <QQmlApplicationEngine>

namespace creative_kernel
{ 

class BASIC_KERNEL_API AbstractKernelUI : public QObject
{
    Q_OBJECT
public:
    virtual void addModel(QObject* model, QObject* group) = 0;
    virtual void delModel(QObject* model) = 0;
    virtual void addGroup(int index, QObject* group) = 0;

    virtual void switchPickMode() = 0;
    virtual void showAdaptShow(QObject* datafrom) = 0;

    virtual void switchPopupMode() = 0;
    //lisugui 2021-1-30
    virtual void switchShowPop(bool bVis = false) = 0;
    virtual void refreshPickPanel() =0;
    //end
    virtual void executeCommand(QString cmdname, QObject* receiver, QString menuObjName) = 0;

    virtual QObject* getUI(QString uiname) = 0;

    virtual AbstractAppSetting* getAppSettings() = 0;

    virtual int addToolCommand(ToolCommand* command, QString key, int index = 0) = 0;
    virtual int removeToolCommand(ToolCommand* command, QString key) = 0;

    virtual int addActionCommand(ActionCommand* command, QString key) = 0;
    virtual int removeActionlCommand(ActionCommand* command, QString key) = 0;

    virtual qtuser_qml::TreeModel* treeModel() = 0;
    virtual CusModelListModel* listModel() = 0;
    //lisugui 2021-3-27
    virtual TableModel* tableModel() = 0;

    virtual BasicSliceUI* createSliceUI() = 0;

    static AbstractKernelUI* getSelf();

    void registerUIVariables(QQmlApplicationEngine &engine);
    QQmlApplicationEngine *getRegisterUIVariables();
protected:
    static AbstractKernelUI* m_self;

public:
signals:
    void sigChangeMenuLanguage();
signals:
    void sigColseAction();
signals:
    void closeWindow();
private:
    QQmlApplicationEngine *m_engine;
};
}
#endif // ABSTRACT_KERNEL_1592188654160_H
