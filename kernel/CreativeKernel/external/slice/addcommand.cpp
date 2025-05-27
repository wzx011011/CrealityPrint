#include "addcommand.h"

#include <QDebug>
#include "interface/modelinterface.h"
#include "kernel/abstractkernel.h"
#include "qtuserqml/plugin/treemodel.h"
#include "slice/cusModelListModel.h"
#include "data/modeln.h"
#include "data/modelgroup.h"
#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/jobsinterface.h"
#include "interface/spaceinterface.h"
#include <QSettings>
#include "kernel/kernelui.h"
#include "inout/cxopenandsavefilemanager.h"
#include "interface/gadatainterface.h"
using namespace creative_kernel;
//#include <QQmlProperty>
AddCommand::AddCommand(QObject* parent)
    :QObject(parent)
{

}

AddCommand::~AddCommand()
{
}

void AddCommand::add()
{
    creative_kernel::sendDataToGA("Model Import", "Import Model");
    AbstractKernelUI::getSelf()->executeCommand("requestLoad", nullptr, "");
    //lisugui 2021-1-30
    //AbstractKernelUI::getSelf()->switchShowPop(false);
    //slider bug 2021-1-21
    QObject* pZsliderObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("zsliderObj");
    QMetaObject::invokeMethod(pZsliderObj, "updateSliderValue");
}
void AddCommand::remove(QModelIndexList indexs)
{
    QList<QObject*> ls;
    for (QModelIndex index : indexs) {
        int row = index.row();
        ls.append(AbstractKernelUI::getSelf()->listModel()->getItem(row));
    }
    for(QObject* o:ls)
    {
        remove(o);
    }
    checkModelRange();
    checkBedRange();
    //lisugui 2021-1-30
    //AbstractKernelUI::getSelf()->switchShowPop(false);
    //slider bug 2021-1-21
    QObject* pZsliderObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("zsliderObj");
    QMetaObject::invokeMethod(pZsliderObj, "updateSliderValue");
    AbstractKernelUI::getSelf()->switchPickMode();
}

void AddCommand::remove()
{
    removeSelectionModels();
}

void AddCommand::selects(QModelIndexList indexs)
{
    //qDebug()<<indexs.count();
    QList<qtuser_3d::Pickable*> onList;
    for (QModelIndex index : indexs)
    {
        int row = index.row();
        QObject* pickable = AbstractKernelUI::getSelf()->listModel()->getItem(row);
        onList << qobject_cast<qtuser_3d::Pickable*>(pickable);
    }

    selectMore(onList);
    requestVisUpdate();
}

void AddCommand::selects(QList<int> indexs)
{
    //qDebug()<<indexs.count();
    QList<qtuser_3d::Pickable*> onList;
    for (int index : indexs)
    {
        QObject* pickable = AbstractKernelUI::getSelf()->listModel()->getItem(index);
        onList << qobject_cast<qtuser_3d::Pickable*>(pickable);
    }

    selectMore(onList);
    requestVisUpdate();
}

void AddCommand::cancelSelect(QList<int> indexs)
{
    for (auto index : indexs)
    {
        QObject* pickable = AbstractKernelUI::getSelf()->listModel()->getItem(index);
        //disSelect
        disSelect(qobject_cast<qtuser_3d::Pickable*>(pickable));
    }
}

void AddCommand::remove(QObject* obj)
{
    QList<ModelN*> naddlist;
    QList<ModelN*> nremovelist;

    nremovelist.append(qobject_cast<ModelN*>(obj));

    //    QModelIndex index = AbstractKernelUI::getSelf()->listModel()->getRowIndex(obj);
    //    QModelIndex parentIndex = AbstractKernelUI::getSelf()->listModel()->parent(index);
    //    if(parentIndex.isValid())
    //    {
    //        //The second level and lower level nodes are deleted
    //        QObject *group = AbstractKernelUI::getSelf()->listModel()->getItemObject(parentIndex);
    //        if(group)
    //        {
    //            QObject *model = AbstractKernelUI::getSelf()->listModel()->getItemObject(index);
    //            nremovelist.append(qobject_cast<ModelN*>(model));
    //            modifyGroup(nremovelist, naddlist, qobject_cast<ModelGroup*>(group), true);

    //        }

    //    }
    //For the first level node (root node)
    //    else
    //    {
    //        QObject *model = AbstractKernelUI::getSelf()->listModel()->getItem(parentIndex);
    //        nremovelist.append(qobject_cast<ModelN*>(model));
    //        removeSelectionModels();
    //    }
    removeSelectionModels();
    //set LastOpenFileName after delete model
    QList<ModelN*> models = modelns();
    if (models.size() > 0)
    {
        creative_kernel::ModelN* model = models[0];
        
        CXFILE.setLastOpenFileName(model->objectName());
    }
}

void remove()
{
    removeSelectionModels();
}

void AddCommand::layoutAll()
{
    creative_kernel::sendDataToGA("Model Editing & Layout", "Auto Arrange");
    m_layoutJob = new AutoLayoutJob(this);
    QList<JobPtr> jobs;
    jobs.push_back(JobPtr(m_layoutJob));
    executeJobs(jobs);
    AbstractKernelUI::getSelf()->switchPickMode();//by TCJ
}

Q_INVOKABLE void AddCommand::writeReg(bool flag)
{
    QSettings settings;
    QString path = "UIparam/isPop";
    settings.setValue(path, QVariant(flag));
}

Q_INVOKABLE bool AddCommand::isPopPage()
{
    QSettings setting;
    setting.beginGroup("UIparam");
    bool flag = setting.value("isPop", true).toBool();
    setting.endGroup();

    return flag;
}

void AddCommand::gotoSupportTab()
{
    QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    QMetaObject::invokeMethod(pmainWinbj, "supportTabBtnClicked");
    creative_kernel::sendDataToGA("Support", "Support (Button)");
}

void AddCommand::setModelVisible(int index, bool isVisible)
{
    QObject* pickable = AbstractKernelUI::getSelf()->listModel()->getItem(index);
    qobject_cast<ModelN*>(pickable)->setVisibility(isVisible);
    //disSelect
    disSelect(qobject_cast<qtuser_3d::Pickable*>(pickable));
}