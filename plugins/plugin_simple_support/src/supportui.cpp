#include "supportui.h"
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QQmlContext>
#include "kernel/abstractkernel.h"
#include "interface/selectorinterface.h"
#include "supportcommand.h"
#include "simplesupportop.h"
#include "interface/visualsceneinterface.h"
#include "interface/gadatainterface.h"
using namespace creative_kernel;
SupportUI::SupportUI(QObject *parent)
:QObject(parent)
, m_qmlUI(nullptr)
,m_pOp(nullptr)
,m_bFDMType(true)
{
     QObject *mainObj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
     if(mainObj)
     {
        m_root = mainObj->findChild<QObject*>("idSupportTabBtn");
        QQmlEngine* engine = qmlEngine(m_root);

        qDebug() << "load xml UI22";
        QQmlComponent component(engine, QUrl::fromLocalFile(":/support/simple_support/SupportUIPanel.qml"));
        m_qmlUI = component.create(qmlContext(m_root));
        m_qmlUI->setParent(m_root);
        m_qmlUI->setObjectName("xxxxx");
        QQmlProperty::write(m_qmlUI, "com", QVariant::fromValue<QObject*>(this));
//        QQmlEngine::setObjectOwnership(m_qmlUI, QQmlEngine::CppOwnership);

//        connect(m_root,SIGNAL(supprotExecute()),this,SLOT(slotSupportExecute()));
        connect(m_root, SIGNAL(supportSelected(bool)), this, SLOT(slotSupportSelState(bool))); 
        connect(m_root, SIGNAL(supportModelChange()), this, SLOT(slotSupportModelChange()));
     }
     //execute();
}

void SupportUI::slotSupportModelChange()
{
    onSelectionsChanged();
}


void SupportUI::slotSupportExecute()
{
    AbstractKernelUI::getSelf()->switchPickMode();
    if(m_bFDMType)
    {
        QQmlProperty::write(m_qmlUI, "visible", QVariant::fromValue(true));
        execute();        
    }
    else
    {
        QQmlProperty::write(m_qmlUI, "visible", QVariant::fromValue(false));
    }
    creative_kernel::sendDataToGA("Support", "Support (Icon)");
}

void SupportUI::setFDMVisible(bool bFDMType)
{
    m_bFDMType = bFDMType;
}
void SupportUI::showOnKernelUI(bool show)
{
    QQmlProperty::write(m_qmlUI, "parent", QVariant::fromValue<QObject*>(show ? m_root : nullptr));
}

SupportUI::~SupportUI()
{}

void SupportUI::onSelectionsChanged()
{
    if (m_bFDMType)
    {
        qDebug() << "onSelectionsChanged()";
        if (!m_root)return;
        QList<ModelN*> selections = selectionms();

        if (selections.size() >= 1)
        {
            QMetaObject::invokeMethod(m_root, "setSupportBtnEnabled", Q_ARG(QVariant, QVariant::fromValue(true)));
            if (m_state)
            {
                if (m_pOp)
                    setVisOperationMode(m_pOp);
            }
        }
        else
        {
            QMetaObject::invokeMethod(m_root, "setSupportBtnEnabled", Q_ARG(QVariant, QVariant::fromValue(false)));
            if (m_state)
                setVisOperationMode(nullptr);
        }
    }
}
void SupportUI::selectChanged(qtuser_3d::Pickable* pickable)
{

}
Q_INVOKABLE bool SupportUI::hasSupport()
{
    if (m_pOp)
    {
        return m_pOp ->hasSupport();
    }
    return false;
}
void SupportUI::execute()
{
    if(!m_pOp)
    {
        m_pOp = new SimpleSupportOp(this);
    }
    //if (!m_pOp->getShowPop())
    //{
    //    setVisOperationMode(m_pOp);
    //}
    //else
    //{
    //    setVisOperationMode(nullptr);
    //}
    setVisOperationMode(m_pOp);
    //by TCJ
//    AbstractKernelUI::getSelf()->switchShowPop(false);
    //enableSupportMode(true);
}

void SupportUI::addMode()
{
    if (m_pOp)
    {
        m_pOp->setAddMode();
    }
}
void SupportUI::deleteMode()
{
    if (m_pOp)
    {
        m_pOp->setDeleteMode();
    }
}
//void SupportUI::execute()
//{

//}
void SupportUI::removeAll()
{
    if (m_pOp)
    {
        m_pOp->deleteAllSupport();
    }

}
void SupportUI::autoAddSupport(qreal size, qreal angle, bool platform)
{
    qDebug() << "support autoAddSupport";
    if (m_pOp)
    {
        m_pOp->setSupportAngle(angle);
        m_pOp->setSupportSize(size);
        m_pOp->autoSupport(platform);
    }
}

void SupportUI::changeSize(qreal size)
{    if (m_pOp)
    {
        m_pOp->setSupportSize(size);
    }
}

void SupportUI::changeAngle(qreal angle)
{
    if (m_pOp)
    {
        m_pOp->setSupportAngle(angle);
    }
}

void SupportUI::slotSupportSelState(bool state)
{
    m_state = state;
}

void SupportUI::onSliceClicked(bool doSlice)
{
    if (doSlice)
    {
        QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
        QMetaObject::invokeMethod(pmainWinbj, "printerConfigBtnClicked");

        QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
        QMetaObject::invokeMethod(pRifhtPanel, "sliceClickFromSupport");
    }
    else
    {
        creative_kernel::sendDataToGA("Configuration Profile", "Back to Printing Profile");
    }
    
}

void SupportUI::backToPrinterConfig()
{
    QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
    QMetaObject::invokeMethod(pmainWinbj, "printerConfigBtnClicked");
}
