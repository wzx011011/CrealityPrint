#include "modelshowcommand.h"
#include "kernel/translator.h"
#include "interface/commandoperation.h"

ModelShowCommand::ModelShowCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = "ModelShow";
    m_actionNameWithout = "ModelShow";
    m_eParentMenu = eMenuType_View;
}

ModelShowCommand::ModelShowCommand(EModelShow eType, QObject* parent)
    :ActionCommand(parent)
{
    m_nShowType = eType;
    m_eParentMenu = eMenuType_View;
    switch (eType)
    {
        case eLineShow:
           m_actionname = tr("ShowModelLine") ;
           m_actionNameWithout = "ShowModelLine";
        break;
        case eFaceShow:
          m_actionname = tr("ShowModelFace") ;
          m_actionNameWithout = "ShowModelFace";
        break;
        case eFaceLineShow:
          m_actionname = tr("ShowModelFaceLine") ;
          m_actionNameWithout = "ShowModelFaceLine";
        break;
    }
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
void ModelShowCommand::slotLanguageChanged()
{
    switch (m_nShowType)
    {
        case eLineShow:
           m_actionname = tr("ShowModelLine") ;
        break;
        case eFaceShow:
          m_actionname = tr("ShowModelFace") ;
        break;
        case eFaceLineShow:
          m_actionname = tr("ShowModelFaceLine") ;
        break;
    }
}
ModelShowCommand::~ModelShowCommand()
{
}

void ModelShowCommand::execute()
{
    switch (m_nShowType)
    {
        case eLineShow:
           showModelLine();
        break;
        case eFaceShow:
          showModelFace();
        break;
        case eFaceLineShow:
          showModelFaceLine();
        break;
        default:break;
    }
}

void ModelShowCommand::showModelFace()
{
    creative_kernel::cmdShowModelFace();
}

void ModelShowCommand::showModelLine()
{
    creative_kernel::cmdShowModelLine();
}

void ModelShowCommand::showModelFaceLine()
{
    creative_kernel::cmdShowModelFaceLine();
}
