#ifndef UnitTestTool_H_
#define UnitTestTool_H_

#include <QVariant>
#include <QObject>
#include <QString>
#include <QPointer>
#include <QTranslator>
#include <QFileInfo>

#include <QtQml/QQmlApplicationEngine>

class BaseSettings : public QObject
{
public:
    static QVariant getSettingValue(const QString& key, const QVariant& defaultValue);
    static void setSettingValue(const QString& key, const QVariant& value);
};
bool readErrorFiles(const QString dirPath, QString& resultMessage);
void loadText(const QString& fileName, QString& message);
void loadJson(const QString& fileName, QList<QString>& listLine);
QList<QFileInfo> entryFileInfoList(const QString& path, const QString& suffix);
QStringList loadErrorList(const QString& fileName);
QString JsonToString(const QJsonObject object, const QString& key);
int JsonToInt(const QJsonObject object, const QString& key);

class UnitTestTool : public QObject {
    
    Q_OBJECT     
    Q_PROPERTY(QString _3mfPath READ slicer3MFPath WRITE set3MFPath NOTIFY mfPathChanged)
    Q_PROPERTY(QString scpPath READ scpFilePath WRITE setScpFilePath NOTIFY scpPathChanged)
    Q_PROPERTY(QString slicerBLPath READ slicerBLPath WRITE setSlicerBLPath NOTIFY slicerBLPathChanged)
    Q_PROPERTY(QString compareSliceBLPath READ compareSliceBLPath WRITE setCompareSlicerBLPath NOTIFY slicerComparePathChanged)
    Q_PROPERTY(QString message READ message  NOTIFY messageChanged)
    Q_PROPERTY(QString c3dExePath READ c3dExePath WRITE setC3DExePath NOTIFY exePathChanged)

    Q_PROPERTY(QString cur3mfFile READ cur3mfFile WRITE setCur3mfFile CONSTANT)
    Q_PROPERTY(QString orcaAppPath READ orcaAppPath WRITE setOrcaAppPath CONSTANT)
    Q_PROPERTY(QString slicerCacheDir READ slicerCacheDir WRITE setSlicerCacheDir CONSTANT)
    Q_PROPERTY(QString gcodeDir READ gcodeDir WRITE setGcodeDir CONSTANT)

    Q_PROPERTY(bool isSimple READ isSimple WRITE setSimple NOTIFY simpleWayChanged)
public:
    void initTranslator();
    void initDirConfig();
public:
    explicit UnitTestTool(QObject* parent = nullptr);

    virtual ~UnitTestTool();

    void setQmlEngine(QQmlEngine* engine);

    Q_INVOKABLE void changeLanguage(int index);

    QString slicer3MFPath() const;
    void set3MFPath(const QString& strPath);

    QString slicerBLPath() const;
    void setSlicerBLPath(const QString& strPath);

    QString compareSliceBLPath()const;
    void setCompareSlicerBLPath(const QString& strPath);

    QString scpFilePath()const;
    void setScpFilePath(const QString& strPath);

    QString c3dExePath() const;
    void setC3DExePath(const QString& strPath);
    bool isSimple() const;
    void setSimple(bool val);

    QString message()const;
    Q_INVOKABLE void processCommandLine();

    QString cur3mfFile() const;
    void setCur3mfFile(const QString& strFile);

    QString orcaAppPath() const;
    void setOrcaAppPath(const QString& strPath);

    QString slicerCacheDir() const;
    void setSlicerCacheDir(const QString& strPath);

    QString gcodeDir() const;
    void setGcodeDir(const QString& strPath);
private:
    void runPythonScript();
    //_sourcePath : .cxcache   .3mf
    void slicerGcode(const QString& _sourcePath,const QString&exePath,const QString& cacheDir,const QString&gcodeDir);
public:
    Q_INVOKABLE void clearMessageText();
    Q_INVOKABLE void doTest(bool exit = false,int type = 1,const QString&filepath = "");
    Q_INVOKABLE void doTestOneByOne(const int& type = 1);
    Q_INVOKABLE void scpMapping3MF(const QString& _3mfPath,const QString& scpName = QString());
    Q_INVOKABLE void generateScript();
    Q_INVOKABLE void swichMessageShow(bool onlyError);
    Q_INVOKABLE void failedFileShow();
    Q_INVOKABLE void vldTest();
    Q_INVOKABLE void runGCodeCompare(const QString&sourcePath,int appType = 0);      //ÔËÐÐ 0:c3d  1: orca 
Q_SIGNALS:
    void exePathChanged();
    void mfPathChanged();
    void slicerBLPathChanged();
    void slicerComparePathChanged();
    void scpPathChanged();
    void messageChanged();
    void simpleWayChanged();
private:
    bool m_simpleWay;
    QString m_c3dExePath;
    QString m_scpPath;
    QString m_3mfPath;
    QString m_slicerBLPath;
    QString m_slicerComparePath;

    QString m_mess_text;

    QList<QString> m_messageLines;
    QStringList m_errorFiles;
private:
    QString _languageTs;
    QTranslator* m_translator;
    QQmlEngine* m_qmlEngine;
private:
    QString m_orcaAppPath;
    QString m_slicerCacheDir;
    QString m_slicerGcodeDir;
    QString m_cur3mfFilePath;

};

#endif // !DUMPTOOL_DUMPTOOL_H_
