#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDebug>
#include <QQuickWindow>
#include <QDir>
#include <QQmlContext>
#include <QTranslator>
#include "wizardui.h"
#include "cuswifimanagermodel.h"
void InitUiByLanguage();
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    qDebug()<<app.applicationDirPath();
//    qDebug()<<QDir::currentPath();
    InitUiByLanguage();

    QQmlApplicationEngine engine;
    engine.addImportPath("../../../c3d/plugins");
//    engine.addImportPath("D:/work/FDM/Creative3D/plugins");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    WizardUI wizardUI;
    engine.rootContext()->setContextProperty("WizardUI", &wizardUI);



    //注册wifi和创想云模型
    CusWifiManagerModel* cwModel = new CusWifiManagerModel();
    engine.rootContext()->setContextProperty("cwModel", cwModel);
//    cwModel->registerEngine(&engine);


    engine.load(url);

    QFile file(":/UI/qml/images/move.png");

    if(file.exists())
    {
        qDebug()<<"--";
    }
    return app.exec();
}

void InitUiByLanguage()
{

    QTranslator translator;
    QString strLanguageFile;
//    if (strLanguage.compare("en") == 0)
//    {
//        strLanguageFile = qApp->applicationDirPath() + QString("/languages/%1/%2").arg(LHT_SYNCCLIENT_VERSION_PRODOCUTNAME).arg(LHT_SYNCCLIENT_EN_FILE);
//    }
//    else if (strLanguage.compare("zh") == 0)
//    {
//        strLanguageFile = qApp->applicationDirPath() + QString("/languages/%1/%2").arg(LHT_SYNCCLIENT_VERSION_PRODOCUTNAME).arg(LHT_SYNCCLIENT_ZH_FILE);
//    }
    qDebug()<<"qApp->applicationDirPath() =" << qApp->applicationDirPath();
    strLanguageFile = "E:/work_Creality/Creative3D/locales/zh_CN.ts";
    if (QFile(strLanguageFile).exists())
    {
        translator.load(strLanguageFile);
        qApp->installTranslator(&translator);
    }
    else
    {
        qDebug() << "[houqd] authclient language file does not exists ...";
    }
}
