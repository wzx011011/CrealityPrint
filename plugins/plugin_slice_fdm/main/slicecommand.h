#ifndef SLICECOMMAND_1594085332399_H
#define SLICECOMMAND_1594085332399_H

#include<QObject>

class SlicePlugin;
namespace creative_kernel
{
    class AddTableData;
}

class SliceCommand : public QObject
{
    Q_OBJECT
public:
    explicit SliceCommand(QObject *parent = nullptr);
    virtual ~SliceCommand();

    void setParentObj(SlicePlugin* slicePlugin)
    {
        m_slicePlugin = slicePlugin;
    };
    Q_INVOKABLE QObject* getSlicePlugin();
    void sliceSuccess();//QObject* object
    Q_INVOKABLE void sliceUnPreview();
    int onExportSuccess(QObject* object);

    Q_INVOKABLE QObject* getTableModel();

    Q_INVOKABLE void startSearchWifi();
    Q_INVOKABLE void sendSliceFile();

    void onSaveSliceSuccess(QString filePath);

    bool copyFile(QString srcPath, QString dstPath, bool coverFileIfExist);


signals:
    void successSliceShow(QObject *repotObject);
private :
    SlicePlugin * m_slicePlugin = nullptr;

    creative_kernel::AddTableData* m_addTable = nullptr;
    QObject* m_footStatus;


};

#endif // SLICECOMMAND_1594085332399_H
