#include "simplesupportplugin.h"
#include "supportcommand.h"
#include <QQmlContext>
#include "kernel/translator.h"
#include "kernel/abstractkernel.h"
#include "supportui.h"
#include "interface/selectorinterface.h"
using namespace creative_kernel;
SimpleSupportPlugin::SimpleSupportPlugin(QObject* parent)
	:QObject(parent)
    ,m_supportUI(nullptr)
    //,m_command(nullptr)
{
}

SimpleSupportPlugin::~SimpleSupportPlugin()
{
}

QString SimpleSupportPlugin::name()
{
	return "";
}

QString SimpleSupportPlugin::info()
{
	return "";
}

void SimpleSupportPlugin::initialize()
{
    m_supportUI = new SupportUI(this);
    m_supportUI->showOnKernelUI(true);
    addSelectTracer(m_supportUI);
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    engine->rootContext()->setContextProperty("supportUICpp", m_supportUI);
    connect(&SlicerManager::instance(),SIGNAL(SliceTypeChanged(SlicerManager::SLICERTYPE)),this,SLOT(visibleLeftBarButton(SlicerManager::SLICERTYPE)));
}
void SimpleSupportPlugin::slotLanguageChanged()
{
//    m_command->setName(tr("Support"));
}

void SimpleSupportPlugin::uninitialize()
{
//    supoortUi->showOnKernelUI(false);
//	removeLCommand(m_command);
    disconnect(&SlicerManager::instance(),SIGNAL(SliceTypeChanged(SlicerManager::SLICERTYPE)),this,SLOT(visibleLeftBarButton(SlicerManager::SLICERTYPE)));
    if(m_supportUI)
        removeSelectorTracer(m_supportUI);

}
void SimpleSupportPlugin::visibleLeftBarButton(SlicerManager::SLICERTYPE type)
{
//    if(type== SlicerManager::SLICERTYPE::FDM && !m_command->enabled())
//    {
//        addLCommand(m_command);
//        m_command->setEnabled(true);
//    }else if(type== SlicerManager::SLICERTYPE::DLP && m_command->enabled()){
//        removeLCommand(m_command);
//        m_command->setEnabled(false);
//    }
    if(!m_supportUI)return;
    if(type== SlicerManager::SLICERTYPE::FDM)
    {
        m_supportUI->setFDMVisible(true);
    }
    else
    {
        m_supportUI->setFDMVisible(false);
    }
}
