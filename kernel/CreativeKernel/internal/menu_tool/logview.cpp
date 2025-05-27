#include "logview.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"
//#include <QtCore/QCoreApplication>
#include<QDesktopServices>
#include <QApplication>
#include <QFile>

LogView::LogView(QObject* parent)
	:ActionCommand(parent)
{
	m_actionname = tr("Log View");
    m_actionNameWithout = "Log View";
	m_eParentMenu = eMenuType_Tool;

	disconnect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
	connect(Translator::getInstance(), SIGNAL(languageChanged()), this, SLOT(slotLanguageChanged()));
}

LogView::~LogView()
{

}

void LogView::execute()
{
	QString Directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QString logpath = Directory + "/Log/";//(QApplication::applicationName() + ".log");
	QDir dir(logpath);
	if (!dir.exists())
		return;

	QFileInfo info(logpath);
	QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
}

void LogView::requestMenuDialog(QObject* receiver)
{

}

void LogView::slotLanguageChanged()
{
	m_actionname = tr("Log View");
}
