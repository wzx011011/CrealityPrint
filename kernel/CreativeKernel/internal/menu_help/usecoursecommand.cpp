#include "usecoursecommand.h"
#include "kernel/translator.h"
#include "kernel/kernelui.h"
#include <QQmlProperty>
#include <QtCore/QCoreApplication>
#include<QDesktopServices>
UseCourseCommand::UseCourseCommand(QObject* parent)
    :ActionCommand(parent)
{
    m_actionname = tr("Use Course");
    m_actionNameWithout = "Use Course";
    m_eParentMenu = eMenuType_Help;
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}
UseCourseCommand::~UseCourseCommand()
{
}

void UseCourseCommand::execute()
{
#if defined (__APPLE__)
    int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
    QFileInfo info(QCoreApplication::applicationDirPath().left(index)+"/Resources/resources/manual/");
#else
    QFileInfo info(QCoreApplication::applicationDirPath()+"/resources/manual/");
#endif
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
}
void UseCourseCommand::slotLanguageChanged()
{
    m_actionname = tr("Use Course");
}

