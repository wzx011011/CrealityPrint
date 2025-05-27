#include "userfeedbackcommand.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include <QQmlProperty>
#include <QtCore/QCoreApplication>
#include<QDesktopServices>
#include <QtCore/QSettings>
UserFeedbackCommand::UserFeedbackCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("User Feedback");
    m_actionNameWithout = "User Feedback";
    m_eParentMenu = eMenuType_Help;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
UserFeedbackCommand::~UserFeedbackCommand()
{
}

void UserFeedbackCommand::execute()
{   

	QString urlStr = "";

	QSettings setting;
	setting.beginGroup("profile_setting");
	QString strStartType = setting.value("service_type", "0").toString();
	if (strStartType == "0")
	{
		urlStr = "https://www.crealitycloud.cn/channel-category/3d-print-6136c1ff2dc3378244a97785";
	}
	else
	{
		urlStr = "https://www.crealitycloud.com/channel-category/3d-print-6131c1c283a38350d7c53ffe";
	}
	setting.endGroup();


    QDesktopServices::openUrl(QUrl(urlStr));
}
void UserFeedbackCommand::slotLanguageChanged()
{
    m_actionname = tr("User Feedback");
}
