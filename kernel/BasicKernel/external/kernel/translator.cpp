#include "translator.h"
#include <QTranslator>

#include <QDebug>
#include <QApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlApplicationEngine>
#include <QSettings>
Translator *Translator::getInstance()
{
    static Translator transInstance;
    return &transInstance;
}

Translator::Translator(QObject *parent) : QObject(parent)
{
    m_translator = new QTranslator;
}

Translator::~Translator()
{
    delete m_translator;
}

void Translator::setMainwin(QObject *obj)
{
    m_mainObj = obj;
}
void Translator::loadLanguage_ts(QString strFileName)
{
    if(NULL == m_translator)
    {
        return;
    }

    if(m_translator->load(strFileName))
    {
        QApplication::installTranslator(m_translator);
        emit languageChanged();
        QObject *mainObj = m_appWindow;
        if(!mainObj)return;
        QQmlApplicationEngine* engine =static_cast<QQmlApplicationEngine *>(qmlEngine(mainObj));
        qDebug() << "m_nType ==" << m_nType;
        qDebug() << "strFileName ==" << strFileName;
        engine->retranslate();
        
      //  m_nType=ELanguageType::eEnglish;
    }
    else
    {
        qDebug()<<"load en error";
    }
}


void Translator::loadLanguage(QString lang)
{
    qDebug()<<"load"<<lang;
    if(NULL == m_translator)
    {
        return;
    }

    if(lang.contains("English"))
    {
        if(m_translator->load(":/language_en_us.qm"))
        {
            QApplication::installTranslator(m_translator);
            emit languageChanged();
            QObject *mainObj = m_appWindow;
            if(!mainObj)return;
            QQmlApplicationEngine* engine =static_cast<QQmlApplicationEngine *>(qmlEngine(mainObj));
            engine->retranslate();
            //m_nType=ELanguageType::eEnglish;
        }
        else
        {
            qDebug()<<"load en error";
        }
    }
    else
    {
        if(m_translator->load(":/language_zh_cn.qm"))
        {
            QApplication::installTranslator(m_translator);
            emit languageChanged();

          //  m_nType=ELanguageType::eChianese;
            QObject *mainObj = m_appWindow;
            if(!mainObj)return;
            QQmlApplicationEngine* engine =static_cast<QQmlApplicationEngine *>(qmlEngine(mainObj));
            engine->retranslate();
        }
        else
        {
            qDebug()<<"load cn error";
        }
    }
}
QObject* Translator::appWindow()
{
    return m_appWindow;
}
void Translator::setAppWindow(QObject* appWindow)
{
    m_appWindow = appWindow;
}


int Translator::getLanguageType()
{
    return m_nType;
}
void Translator::setLanguageType(int type)
{
    m_nType = type;
}

int Translator::getMoneyTypeIndex()
{
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    int nType = setting.value("money_type", "0").toInt();

    setting.endGroup();
    return nType;
   
    //QString strMoneyType = getMoneyType();
    //if ("￥" == strMoneyType)return 0;
    //else if ("$" == strMoneyType)return 1;
    //return 0;
}

QString Translator::getMoneyType()
{
    //QSettings setting;
    //setting.beginGroup("language_perfer_config");
    //QString strMoneyType = setting.value("money_type", "￥").toString();
    //setting.endGroup();
    //return strMoneyType;
    int nType = getMoneyTypeIndex();
    if (nType == 0)
        return "RMB";
    else if (nType == 1)
        return "$";
    return  "RMB";
}

void Translator::setMoneyType(QString str)
{
    QSettings setting;
    setting.beginGroup("language_perfer_config");
    setting.setValue("money_type", str);
    setting.endGroup();
    emit moneyTypeChanged();
}
