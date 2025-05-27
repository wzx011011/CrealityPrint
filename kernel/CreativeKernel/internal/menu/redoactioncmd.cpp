#include "redoactioncmd.h"

//#include <QtQml/QQmlComponent>
//#include "interface/uiinterface.h"
#include "interface/undointerface.h"
#include "kernel/translator.h"

using namespace creative_kernel;

RedoActionCmd::RedoActionCmd()
{
    m_shortcut = "Ctrl+Shift+Z";      //不能有空格
    m_actionname = tr("Redo");
    m_actionNameWithout = "Redo";
    m_eParentMenu = eMenuType_Edit;
    m_icon = "qrc:/kernel/images/redo.png";
//    m_root = nullptr;
//    m_component = nullptr;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
RedoActionCmd::~RedoActionCmd()
{}
void RedoActionCmd::execute()
{
    redo();


//    m_root = getUIRoot();

//    if (!m_component)
//    {
//        QQmlEngine* engine = qmlEngine(m_root);
//        m_component = new QQmlComponent(engine, QUrl::fromLocalFile("qrc:/kernel/qml/Clone.qml"), this);
//        QQmlContext* context = qmlContext(m_root);
//        m_component->create(context);
//    }
}
void RedoActionCmd::slotLanguageChanged()
{
    m_actionname = tr("Redo") + "      " + m_shortcut;
}
