#ifndef _NULLSPACE_SLICEUI_1590162522691_H
#define _NULLSPACE_SLICEUI_1590162522691_H
#include <QtCore/QObject>
#include <QtGui/QImage>
#include "slice/imgprovider.h"
#include "slice/basicsliceui.h"
#include "slice/addcommand.h"
#include "slice/parametersettingsui.h"
#include "slice/crealitycloudui.h"

void profileListCallBack(std::list<std::string> profileList);

class ModelSelectionProxy;
class SliceUI: public BasicSliceUI
{
    Q_OBJECT
public:
    SliceUI(QObject* parent = nullptr);
    virtual ~SliceUI();

    void setObject(QString objname, QObject* object) override;
    void setParam(QString objname, void* param) override;

    int initialize() override;
    int uninitialize() override;

    int onSliceSuccess(QObject* object = nullptr, void* param = nullptr) override;
    int onSliceFailed(QObject* object = nullptr, void* param = nullptr) override;
    int handle(QObject* object = nullptr, void* param = nullptr) override;
    int onExporSuccess(QObject* object = nullptr, void* param = nullptr) override;

    void show(bool show) override;
    void update() override;

    void invokeFunc(const char* func, QObject* report=nullptr);
    void invokeProgress(float r);
    QObject* getParasetting();

    void enableStartSliceBtn();

    Q_INVOKABLE void getProfileList(const QString& printerName);
    Q_INVOKABLE QString getMessageText();
    Q_INVOKABLE void accept();
    Q_INVOKABLE  QString curMachineType();
    Q_INVOKABLE QObject* mainObj();

    //addCommand 接口
    Q_INVOKABLE void layoutAll();
    Q_INVOKABLE void deleteModel(QList<int> indexs);
    Q_INVOKABLE void selectModel(QList<int> indexs);
    Q_INVOKABLE void cancelSelect(QList<int> indexs);
    Q_INVOKABLE void setModelVisible(int index, bool isVisible);


signals:
    void sigGetProfileList(QStringList profileList);
    void nozzleCountChange(int count);
    void sigSliceSuccess(QObject* obj);
    void sigStartPreview(QString deviceType);

public slots:
    void unpreview() override;
    void layerImgChange(const QImage& img) override;
    void preview() override;
    //2020/09/02 lisugui write,control infopanel show

private:
    void profileListCallBack(std::list<std::string> profileList);

    QString m_strMessageText;
    void setInfoShowObj();

public:
    ImgProvider* m_imgProvider = nullptr;

protected:
    QObject* m_root;
    QObject* m_qmlUI;
    QObject* m_paraSettingUI;
    QObject* m_modelSelection;
    ModelSelectionProxy* m_selectionProxy;
    ParameterSettingUI* m_parameterSettingUI;
    CrealityCloudUI* m_crealityCloudUI;
    AddCommand* m_addCommand;

    int m_uiInited;
};
#endif // _NULLSPACE_SLICEUI_1590162522691_H
