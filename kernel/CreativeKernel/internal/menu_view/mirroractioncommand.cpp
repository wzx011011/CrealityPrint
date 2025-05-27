#include "mirroractioncommand.h"

#include "interface/modelinterface.h"
#include "interface/selectorinterface.h"
#include "kernel/translator.h"
MirrorCommand::MirrorCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Mirror");
    m_actionNameWithout = "Mirror";
    m_eParentMenu = eMenuType_View;
}
MirrorCommand::MirrorCommand(EMirrorCommand eType, QObject* parent)
    :ActionCommand(parent)
{
    m_nType = eType;
    m_eParentMenu = eMenuType_View;
    switch (m_nType)
    {
    case eMirror_X:
        m_actionNameWithout = "Mirrror X";
        m_actionname = tr("Mirrror X") ;
        break;
    case eMirror_Y:
        m_actionNameWithout = "Mirrror Y";
        m_actionname = tr("Mirrror Y") ;
        break;
    case eMirror_Z:
        m_actionNameWithout = "Mirrror Z";
        m_actionname = tr("Mirrror Z") ;
        break;
    case eMirroe_Reset:
        m_actionNameWithout = "Mirrror ReSet";
        m_actionname = tr("Mirrror ReSet");
        break;

    }
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

MirrorCommand::~MirrorCommand()
{
}

void MirrorCommand::execute()
{
    switch (m_nType)
    {
    case eMirror_X:
        mirrorX();
        break;
    case eMirror_Y:
        mirrorY();
        break;
    case eMirror_Z:
        mirrorZ();
        break;
    case eMirroe_Reset:
        reset();
        break;

    }
}
void MirrorCommand::slotLanguageChanged()
{
    switch (m_nType)
    {
    case eMirror_X:
        m_actionname = tr("Mirrror X") ;
        break;
    case eMirror_Y:
        m_actionname = tr("Mirrror Y") ;
        break;
    case eMirror_Z:
        m_actionname = tr("Mirrror Z") ;
        break;
    case eMirroe_Reset:
        m_actionname = tr("Mirrror ReSet");
        break;

    }
}
void MirrorCommand::mirrorX()
{
	creative_kernel::mirrorX(get(), true);
}

void MirrorCommand::mirrorY()
{
	creative_kernel::mirrorY(get(), true);
}

void MirrorCommand::mirrorZ()
{
	creative_kernel::mirrorZ(get(), true);
}

void MirrorCommand::reset()
{
	creative_kernel::mirrorReset(get(), true);
}

creative_kernel::ModelN* MirrorCommand::get()
{
	QList<creative_kernel::ModelN*> selections = creative_kernel::selectionms();
	return selections.size() > 0 ? selections.at(0) : nullptr;
}

