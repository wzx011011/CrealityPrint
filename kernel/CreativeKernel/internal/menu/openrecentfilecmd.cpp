#include "openrecentfilecmd.h"
#include <QtCore/QUrl>
#include <QFileInfo>

#include "submenurecentfiles.h"
#include "kernel/translator.h"
#include "inout/cxopenandsavefilemanager.h"
#include "kernel/kernelui.h"
#include "interface/modelinterface.h"

OpenRecentFileCmd::OpenRecentFileCmd()
{
    m_actionname = "";
    m_eParentMenu = eMenuType_File;
}

OpenRecentFileCmd::~OpenRecentFileCmd()
{
}

void OpenRecentFileCmd:: execute()
{
    QFileInfo fileInfo(m_actionname);
    if (!fileInfo.exists())
    {
        getKernelUI()->requestMenuDialog(this, "openRecentlyFileDlg");
        return;
    }

    if (m_actionname.startsWith("file:///"))
    {
        QUrl url(m_actionname);
        CXFILE.openWithUrl(url);
    }
    else
    {
        CXFILE.openWithName(m_actionname);
    }

    SubMenuRecentFiles::getInstance()->setMostRecentFile(m_actionname);
}

void OpenRecentFileCmd::slotLanguageChanged()
{
//    m_actionname = tr("Open")  + "          " +  m_shortcut;
}
