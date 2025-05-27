#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include "basickernelexport.h"
class QTranslator;

enum ELanguageType
{
    eLanguage_AR_TS =0,
    eLanguage_DE_TS,
    eLanguage_EN_TS,
    eLanguage_FR_TS,
    eLanguage_HR_TS,
    eLanguage_IT_TS,
    eLanguage_JP_TS,
    eLanguage_KO_TS,
    eLanguage_PL_TS,
    eLanguage_RU_TS,
    eLanguage_TR_TS,
    eLanguage_ZHCN_TS,
    eLanguage_ZHTW_TS
};

class BASIC_KERNEL_API Translator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* appWindow READ appWindow WRITE setAppWindow)
public:
    static Translator* getInstance();
    int getLanguageType();
    void setLanguageType(int type);


    int getMoneyTypeIndex();
    QString getMoneyType();
    void setMoneyType(QString str);
    Q_INVOKABLE void setMainwin(QObject* obj);
public :
    explicit Translator(QObject *parent = 0);
    ~Translator();
    QObject* appWindow();
    void setAppWindow(QObject* appWindow);



signals:
    void languageChanged();
    void moneyTypeChanged();

public slots:
    void loadLanguage(QString lang);
    void loadLanguage_ts(QString strFileName);

private:
    QTranslator*  m_translator;
    QObject * m_mainObj = nullptr;
    QObject *m_appWindow = nullptr;
    int m_nType=0;
};

#endif // TRANSLATOR_H
