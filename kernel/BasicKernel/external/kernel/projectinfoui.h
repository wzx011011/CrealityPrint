#ifndef CREATIVE_KERNEL_PROJECTINFOUI_1594457868780_H
#define CREATIVE_KERNEL_PROJECTINFOUI_1594457868780_H
#include "basickernelexport.h"
#include <QTimer>


class BASIC_KERNEL_API ProjectInfoUI:public QObject
{
    Q_OBJECT
public:
    ProjectInfoUI(QObject* parent = nullptr);
    virtual ~ProjectInfoUI();

    static ProjectInfoUI * instance();
    float getMinutes();
    void setMinute(float fMinute);


    QString getProjectName();
    void setProjectName(QString strProName);

    QString getProjectPath();
    void setProjectPath(QString strProPath);
    void updateProjectNameUI();
    QString getNameFromPath(QString path);

    void clearSecen();
    void requestMenuDialog();

    void updateFileStateUI();

    Q_INVOKABLE QString getMessageText();
    Q_INVOKABLE void accept();
    Q_INVOKABLE void cancel();
signals :
    void minutesChanged(float fMinute);
    void acceptDialog();
    void cancelDialog();
private slots:
    void slotLanguageChanged();
private:
    float m_fMinutes =0.0;
    QString m_strProjectPath ;
    QString m_strProjectName;
    QString m_strMessageText;
    bool m_bAcceptDialog = false;
};

#endif // CREATIVE_KERNEL_PROJECTINFOUI_1594457868780_H
