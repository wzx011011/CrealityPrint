#ifndef _NULLSPACE_PARAMETERSETTING_15916733167626_H
#define _NULLSPACE_PARAMETERSETTING_15916733167626_H
#include <QtCore/QObject>
#include "editprofile.h"
#include "parameterfile.h"
#include "qrcodeimageprovider.h"
#include "cameraServer.h"
#include <vector>


class QNetworkReply;
class QueryEquipmentInfoTask;
class QueryGroupPrintDeviceInfoTask;
class ProfileListModel;
class ParameterSettingUI: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QStringList printerList READ printerList WRITE setPrinterList NOTIFY printerListChanged)
    ParameterSettingUI();
    virtual ~ParameterSettingUI();

    void invokeFunc(const char* func, QObject* report = nullptr);
    void setqmlUI(QObject* obj);
    Q_INVOKABLE QPointF getWindowPos();

    //new param ui
    //add
    Q_INVOKABLE QString getNewProfileName();
    Q_INVOKABLE QStringList getProfileList();
    Q_INVOKABLE int getCurProfileIndex();
    Q_INVOKABLE QStringList getMaterialList();
    Q_INVOKABLE int getCurMetrailIndex();
    Q_INVOKABLE QStringList getPrinterList();
    Q_INVOKABLE int getCurPrinterIndex();
    Q_INVOKABLE int getMachineSeries(const QString& machinType);
    Q_INVOKABLE bool deleteProfile(QString fileName = QString());
    Q_INVOKABLE QString getCurProfilePath(QString name = QString());
    //打印机参数恢复出厂设置
	Q_INVOKABLE bool resetMachineParam(const QString& machineName);
    Q_INVOKABLE void resetProfile(const bool& isFactory=false);
    //确定文件是否重复
    Q_INVOKABLE bool isRepeatFileName(const QString& fileName);

    void initProfileList();
    //edit
    //delete
    const QStringList &printerList() const;
    void setPrinterList(const QStringList &newPrinterList);
    void initProfileListModel();

protected:

public slots://qml data
    void onMachineChanged(QString text);
    void onMachineAdded(QString text, bool needUpdate=true);
    void onSelectMachineChanged(QString text);
    void onMachineDeleted(QString text);
    void onFDMView();
    void onLaserView();
    void onPlotterView();
    void onEditProfiled();
    void onEditAddProfiled();
    void onPrinterchanged(QString currenttext);
    bool onConfirmNewName(QString currenttext);
    void onSaveProfile(QString newProfile, QString printer, QString material);

    void onResetbtn(const bool& isFactory=false);
    void onSaveProfilebtn();
    void onValchanged(QString key, QString value);
    bool onDeleteProfilebtn();
    void onManagerMachine(const QString& machineName = QString(), bool isReset = false);
    void onSaveManageConfig(const QString& machineName = QString());
    void onAddNewPrinter(QString newprinter);
    void onManagerMachineChanged(QString name);
    void onMangerPrinterClicked();
    void onEditProfileClicked();
    void onAddProfileClicked();

    //lisugui 2020-10-9
    void onSaveExtruderConfig(int nExtruder);
    void onValuechanged_Extruder(QString key,QString value,int type); //type = 0,1
    void onExtruderCntChanged(int extruderCnt);
    void onMachineManager();
    void onShowAddPrinterDlg();
    void onChangeLanguage(int nType);

private:
    void updateMachineList(bool isDel=false, QString machineName = QString());
    int getCurrentLanguage();


signals:
    void sigNewProfileCheck(QString msg);

    void printerListChanged();
    void sigLanguageChanged();

private slots:
    void slotLanguageChanged();

private:
    QObject* m_qmlUI;

    Editprofile* m_editprofile;
    ParameterFile* m_file;
    bool isFirstStart;
    // ExtruderProfile *m_extruderfile;

    QString m_configRoot;
    QStringList m_printerList;
    ProfileListModel* m_ProfileMode;
};


#endif // _NULLSPACE_PARAMETERSETTING_15916733167626_H
