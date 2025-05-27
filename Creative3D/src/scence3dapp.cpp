#include "scence3dapp.h"
#include <QtCore/QCoreApplication>
#include "qmlworkflow.h"

#include "qtuserqml/macro.h"
#include "qtuserqml/gl/glquickitem.h"
#include "qtuserqml/plugin/toolcommand.h"

#include "kernel/kernelui.h"
#include "qtusercore/module/systemutil.h"
#include"qtusercore/string/resourcesfinder.h"

#include "menu/actioncommand.h"
#include "../internal/menu/ccommandsdata.h"
#include "../internal/rclick_menu/rclickmenudata.h"
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <QDir>
#include <QQuickWindow> 
#include "cxffmpeg/qmlplayer.h"
#include"ccglobal/log.h"
#include "def/settingdef.h"

#define CXSW_SCOPE "com.cxsw.SceneEditor3D"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define COMMON_REASON "Created by Cpp"

#define CXSW_REG(x) QML_INTERFACE(x, CXSW_SCOPE, VERSION_MAJOR, VERSION_MINOR)
#define CXSW_REG_U(x) QML_INTERFACE_U(x, CXSW_SCOPE, VERSION_MAJOR, VERSION_MINOR, COMMON_REASON)

#include "buildinfo.h"
//#include "kernel/resizeqmlwindow.h"
#include "kernel/wizardui.h"
#include <QOpenGLContext>
#include <QGLFormat> 
#include<QSettings>
#include "kernel/translator.h"
#include "MiniDump.h"
#include <QtSerialPort/QSerialPort>
#include "kernel/abstractkernel.h"

    //source源文件目录路径，destination目的文件目录，override文件存在是否覆盖
bool copyDir(const QString& source, const QString& destination, bool override)
{
    QDir directory(source);
    if (!directory.exists())
    {
        qDebug() << "不存在";
        return false;
    }
    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();
    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }
    return !error;
}
void initializeConfig()
{
    QString configDir = qtuser_core::getOrCreateAppDataLocation("sliceconfig");
    qDebug() << configDir;
#ifdef QT_NO_DEBUG

#if defined (__APPLE__)
    int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
    QString srcDir = (QCoreApplication::applicationDirPath().left(index) + "/Resources/resources/sliceconfig/");
#else
    QString srcDir = (QCoreApplication::applicationDirPath() + "/resources/sliceconfig/");
#endif
#else
    QString srcDir = QString(SOURCE_ROOT) + "/resources/sliceconfig";
#endif
    us::SettingDef::instance().initialize(srcDir + "/default/fdm.def.json");
    //copy config 存在不拷贝
    QDir directory(configDir + "/default/");
    QFile file(configDir + "/version.txt");
    if (directory.exists())
    {
        if (file.exists())
        {
            return;
        }
        else
        {
            directory.removeRecursively();
        }
    }
    if (!copyDir(srcDir, configDir, true))
    {
        LOGE("initializeConfig failed ! please check access right ! configDirectory:[%s],wordPath:[%s]", configDir.toStdString().c_str(), srcDir.toStdString().c_str());
        return;
    }
    //拷贝Material文件夹
    QString srcMaterialPath = configDir + "/default/Material";
    QString dstMaterialPath = qtuser_core::getOrCreateAppDataLocation("Materials");
    QDir materialDirectory(dstMaterialPath);
    if (materialDirectory.exists())
    {
       
    }
    if (!copyDir(srcMaterialPath, dstMaterialPath, true))
    {
        LOGE("initializeConfig failed ! please check access right ! configDirectory:[%s],wordPath:[%s]", srcMaterialPath.toStdString().c_str(), srcDir.toStdString().c_str());
        return;
    }
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QString currentVersion = QString("V%1.%2.%3.%4").arg(PROJECT_VERSION_MAJOR).arg(PROJECT_VERSION_MINOR).arg(PROJECT_VERSION_PATCH).arg(PROJECT_BUILD_ID);
    file.write(currentVersion.toLocal8Bit());
    file.close();
}

void scene3dapp_startEngine(QQmlApplicationEngine& engine)
{
#ifdef Q_OS_OSX
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
#endif
    qDebug() << "start...";
    qApp->setOrganizationName("Creality");
    qApp->setOrganizationDomain("FDM");
    qApp->setApplicationName(PROJECT_NAME);//by TCJ "Creative3D"

    initializeConfig();
    initSystemUtil();
    showDetailSystemInfo();
    showSysMemory();

    QSerialPort port;
    port.setPortName("XXx");
#if _WINDOWS || WINDOWS || WIN32 || WIN64
    QString dumpPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QByteArray cdata = dumpPath.toLocal8Bit();
    std::string dpath(cdata);
    MiniDump::EnableAutoDump(dpath, true);//generate windows dump file after crash
#endif

    CXSW_REG(GLQuickItem);
    CXSW_REG(QMLWorkflow); 
    CXSW_REG(QMLPlayer);

    CXSW_REG_U(KernelUI)
    CXSW_REG_U(ToolCommand)

    CXSW_REG_U(ActionCommand)

    QSettings setting;
    QString sysLang = QLocale::system().name();
    if (sysLang.indexOf("en") != -1)
    {
        sysLang = "en";
    }
    if (sysLang == "zh_HK")
    {
        sysLang = "zh_TW";
    }

    if (sysLang != "en" && sysLang != "zh_CN" && sysLang != "zh_TW")
    {
        sysLang = "en";
    }

    setting.beginGroup("language_perfer_config");
    QString strLanguageType = setting.value("language_type", "").toString();
    if (strLanguageType == "")
    {
        setting.setValue("language_type", sysLang+".ts");
        strLanguageType = setting.value("language_type", "").toString();
    }
    QString strPath = ":/translations/" + strLanguageType;
    Translator::getInstance()->loadLanguage_ts(strPath);

    qDebug() << "install command";
    CCommandsData* commandData = new CCommandsData();
    engine.rootContext()->setContextProperty("actionCommands",commandData);

    qDebug() << "install rclick menu";
    RClickMenuData* rclickData = new RClickMenuData();
    engine.rootContext()->setContextProperty("rClickComandsData",rclickData);

    engine.rootContext()->setContextProperty("appDir", qApp->applicationDirPath());

    creative_kernel::AbstractKernelUI::getSelf()->registerUIVariables(engine);

    showSysMemory();
    qDebug() << "install UI";

    qDebug() << "current path application dir = " << QCoreApplication::applicationDirPath();
    qDebug() << "current path current = " << QDir::currentPath();

    //2020/12/31 remove ResizeQmlWindow,only qml do it
//    ResizeQmlWindow *resize = new ResizeQmlWindow();
//    //bind Resize is in mainWindow.qml
//    engine.rootContext()->setContextProperty("Resize", resize);

    //wizard function
    WizardUI *wizardUI = new WizardUI();
    engine.rootContext()->setContextProperty("WizardUI", wizardUI);
    engine.rootContext()->setContextProperty("screenScaleFactor", wizardUI->getScreenScaleFactor());
    //bind wizard is in mainWindow.qml
    engine.load(QUrl(QLatin1String("qrc:/scence3d/res/main.qml")));

    showSysMemory();

    qDebug() << "current path application dir = " << QCoreApplication::applicationDirPath();
    qDebug() << "current path current = " << QDir::currentPath();
}


