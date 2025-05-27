#include "websitecommand.h"
#include <QDesktopServices>
#include <QUrl>

WebsiteCommand::WebsiteCommand(QObject* parent)
	:ToolCommand(parent)
{
    orderindex=200;
    m_name = tr("WebSite");
    m_enabledIcon = "qrc:/kernel/images/website.png";
    m_pressedIcon = "qrc:/kernel/images/website.png";
}

WebsiteCommand::~WebsiteCommand()
{
}

void WebsiteCommand::execute()
{
    QDesktopServices::openUrl(QUrl("https://www.cxsw3d.com/"));
}
