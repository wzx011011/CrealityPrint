#include "sliceplugin.h"
#include <QtCore/QDebug>
#include <QtCore/QStandardPaths>
#include <time.h>
#include <QtCore/QtCore>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QQmlContext>
#include <QSettings>

#include "inout/cxopenandsavefilemanager.h"

#include "main/ansycworker.h"
#include "main/sliceattain.h"
#include "main/previewworker.h"

#include "render/slicepreviewflow.h"
#include "render/slicepreviewscene.h"
#include "us/slicermanager.h"

#include "interface/jobsinterface.h"
#include "interface/gadatainterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/renderinterface.h"

#include "interface/selectorinterface.h"
#include "interface/spaceinterface.h"
#include "interface/modelinterface.h"
#include "data/modeln.h"
#include "kernel/abstractkernel.h"
#include "interface/commandoperation.h"

#include "qtuser3d/entity/printerentity.h"
#include "interface/reuseableinterface.h"

#include "zlib.h"

#include "crealitycloud/cxnetworkmanager.h"

#include "interface/gadatainterface.h"
//2020-08-10 liuhong add for inout
#include "inout/cxopenandsavefilemanager.h"

#include "report/slicereport.h"

#include "us/profilemanager.h"
#include "us/slicermanager.h"
#include "def/settingdef.h"
#include "slicecommand.h"
#include <QtQml/QQmlContext>
#include <QQmlApplicationEngine>
#include "usbprintcommand.h"
#include <menu/ccommandlist.h>
using namespace creative_kernel;

SlicePlugin::SlicePlugin(QObject* parent)
    : QObject(parent)
    , m_sliceUI(nullptr)
    , m_bSlicing(false)
    , m_isOnlinePrint(false)
    , m_sliceStartTime(0)
{
    m_slicePreviewFlow = new SlicePreviewFlow(this);
    m_previewWorker = new PreviewWorker(this);
    process = new QProcess();
    isUSBonline = false;
    isRunning = false;

    m_saveGCodeFlag = 0;
    isUploadModelFile = false;
    isClusterUploadGcode = false;

    CXFILE.registerOpenHandler(m_previewWorker);

    m_usbPrinterModel = new USBPrinterOutputDeviceManager();
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    m_usbPrinterModel->start();
    engine->rootContext()->setContextProperty("MachineManager", m_usbPrinterModel);
}

SlicePlugin::~SlicePlugin()
{
    if (m_command)
    {
        delete m_command;
        m_command = nullptr;
    }

    if (process)
    {
        delete process;
        process = nullptr;
    }
}

QString SlicePlugin::name()
{
    return "SliceCommand";
}

QString SlicePlugin::info()
{
    return "";
}

void SlicePlugin::initialize()
{
    SlicerManager::instance().loadDefault();

    m_sliceUI = (BasicSliceUI*)AbstractKernelUI::getSelf()->createSliceUI();  // new SliceUI(this);
    m_sliceUI->initialize();
    
    if (SlicerManager::instance().getCurrentSlicerType() == SlicerManager::SLICERTYPE::FDM)
    {
        m_sliceUI->setObject("slice", this);
        m_sliceUI->setObject("slicemodel", m_slicePreviewFlow);
    }
    m_sliceUI->show(true);

    m_slicePreviewFlow->initialize();
    connect(&SlicerManager::instance(), SIGNAL(SliceTypeChanged(SlicerManager::SLICERTYPE)), this, SLOT(visibleSliceUI(SlicerManager::SLICERTYPE)));
    connect(&SlicerManager::instance(), SIGNAL(MachineChanged(QString)), m_usbPrinterModel, SLOT(onMachineChanged(QString)));
    registerRenderGraph(m_slicePreviewFlow);

    CCommandList::getInstance()->addActionCommad(new UsbPrintCommand(this), "");
    m_command = new SliceCommand(this);
    m_command->setParentObj(this);
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    engine->rootContext()->setContextProperty("slicePlugin", this);
}

void SlicePlugin::uninitialize()
{
    disconnect(&SlicerManager::instance(), SIGNAL(SliceTypeChanged(SlicerManager::SLICERTYPE)), this, SLOT(visibleSliceUI(SlicerManager::SLICERTYPE)));
    //m_sliceUI->uninitialize();
}

QString SlicePlugin::getFileMd5(const QString& filepath)
{
    QFile file(filepath);
    QString Md5Str = "";
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray fileArray = file.readAll();
        QByteArray md5 = QCryptographicHash::hash(fileArray, QCryptographicHash::Md5);
        Md5Str = md5.toHex().toUpper();
    }
    else
    {
        qDebug() << "open file faild";
    }
    file.close();
    return Md5Str;
}

void SlicePlugin::uploadProgress(long long upload_size, long long total_size)
{
    QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
    QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
    int value = ((upload_size*1.0 / total_size*1.0) * 100);
    if (value >= 100)
    {
        value = 99;
    }
    emit sigUpdateProgress(value);
    QMetaObject::invokeMethod(fdmPanel, "updateUploadProgressValue", Q_ARG(QVariant, QVariant::fromValue(value)), Q_ARG(QVariant, QVariant::fromValue(upload_size)));
}

void SlicePlugin::uploadResult(const oss_wrapper::OSSError& error)
{
    QString ret = QString::fromStdString(error.code_);
    if (ret == "200") {
        //std::function<void(const QByteArray)> call_back = std::bind(
        //    &SlicePlugin::uploadGCodeResult, this, std::placeholders::_1);
       // uploadGCode(m_fileName, m_fileKey, call_back);

        QString strContent = QString("{\"list\":[{\"name\": \"%1\", \"filekey\" : \"%2\"}]}").arg(m_fileName).arg(m_fileKey);
        creative_kernel::getcxNetworkManager()->uploadGcode(strContent.toStdString(), std::bind(
            &SlicePlugin::uploadGCodeResult, this, std::placeholders::_1));

        //remove the temp model stl
        QDir delDir(getCanWriteFolder() + "/uploadGcodes");
        delDir.removeRecursively();
    }
    else {
         QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
         QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
         QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
    }
}

void SlicePlugin::uploadModelProgress(long long upload_size, long long total_size)
{
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    QObject* rightMenu = pRoot->findChild<QObject*>("rightbtnmenu");
    int value = ((upload_size * 1.0 / total_size * 1.0) * 100);
    if (value >= 100)
    {
        value = 99;
    }
    QMetaObject::invokeMethod(rightMenu, "updateUploadProgressValue", Q_ARG(QVariant, QVariant::fromValue(value)));
}

void SlicePlugin::uploadModelResult(const oss_wrapper::OSSError& error)
{
    QString ret = QString::fromStdString(error.code_);
    if (ret == "200") {

        QFileInfo fileInfo(m_modelFileName);
        //std::function<void(const QByteArray)> call_back = std::bind(
          //  &SlicePlugin::modelGroupCreateResult, this, std::placeholders::_1);
        //modelGroupCreate(m_categoryId, m_groupName, m_groupDesc, m_isShare, m_modelType, m_license, m_isOriginal, m_fileKey, CXFILE.currOpenFile(), fileInfo.size(), call_back);
        QString strContent = QString("{\"groupItem\":{\"categoryId\": %1, \"groupName\": \"%2\", \"groupDesc\": \"%3\", \"isShared\": %4, \"modelColor\": %5, \"license\": \"%6\", \"isOriginal\": %7, \"covers\":[]}, \"modelList\":[{\"fileKey\":\"%8\", \"fileName\":\"%9\", \"fileSize\":%10}]}")
            .arg(m_categoryId)
            .arg(m_groupName)
            .arg(m_groupDesc)
            .arg(m_isShare)
            .arg(m_modelType)
            .arg(m_license)
            .arg(m_isOriginal)
            .arg(m_fileKey)
            .arg(CXFILE.currOpenFile())
            .arg(fileInfo.size());
        creative_kernel::getcxNetworkManager()->modelGroupCreate(strContent.toStdString(), std::bind(
            &SlicePlugin::modelGroupCreateResult, this, std::placeholders::_1));


        //remove the temp model stl
        if (QFile::exists(m_modelFileName))
        {
            QFile::remove(m_modelFileName);
        }
    }
    else
    {
        qDebug() << QString::fromStdString(error.message_);
    }
}

void SlicePlugin::getCategoryListResult(const QByteArray& response)
{
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    QObject* rightMenu = pRoot->findChild<QObject*>("rightbtnmenu");

    QJsonParseError json_error;
    QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
    if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
        QString strJson(document.toJson(QJsonDocument::Compact));
        QJsonObject object = document.object();
        int state = object.value(QString::fromLatin1("code")).toInt();

        if (state == 0)
        {

            QMetaObject::invokeMethod(rightMenu, "uploadModelDlgCombobox", Q_ARG(QVariant, QVariant::fromValue(strJson)), Q_ARG(QVariant, QVariant::fromValue(isUploadModelFile)));
        }
        else
        {
            QString errorMsg = object.value(QString::fromLatin1("msg")).toString();
            qDebug() << errorMsg << "\n";
        }
    }
    else
    {
        QMetaObject::invokeMethod(rightMenu, "uploadModelError");
    }

    isUploadModelFile = false;
}

void SlicePlugin::uploadGCodeResult(const std::string& response)
{
    if (!response.empty()) {
        QString strJson(response.c_str());

        QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
        emit sigUploadSuccess(strJson);
        QMetaObject::invokeMethod(fdmPanel, "uploadGCodeSuccess", Q_ARG(QVariant, QVariant::fromValue(strJson)));
    }
    else {
        QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
        QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
    }
    
}

void SlicePlugin::modelGroupCreateResult(const std::string& response)
{
    QQmlApplicationEngine* engine = creative_kernel::AbstractKernelUI::getSelf()->getRegisterUIVariables();
    QObject* pRoot = engine->rootObjects().first();
    QObject* rightMenu = pRoot->findChild<QObject*>("rightbtnmenu");

    if (!response.empty()) {
        QString strJson(response.c_str());
        QMetaObject::invokeMethod(rightMenu, "uploadModelSuccess");
    }
    else {
        QMetaObject::invokeMethod(rightMenu, "uploadModelError");
    }
}

void SlicePlugin::uploadModelGroupSingleProgress(long long upload_size, long long total_size)
{
    if (uploadGroupCount == 1)
    {
        QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
        int value = ((upload_size * 1.0 / total_size * 1.0) * 100);
        if (value >= 100)
        {
            value = 99;
        }
        QMetaObject::invokeMethod(pRifhtPanel, "updateUploadProgressValue", Q_ARG(QVariant, QVariant::fromValue(value)));
    }
}

void SlicePlugin::uploadModelGroupSingleResult(const oss_wrapper::OSSError& error)
{
    QString ret = QString::fromStdString(error.code_);
    if (ret == "200") {
        uploadGroupCountTmp++;
        QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
        int value = ((uploadGroupCountTmp * 1.0 / uploadGroupCount * 1.0) * 100);
        if (value >= 100)
        {
            value = 99;
        }
        QMetaObject::invokeMethod(pRifhtPanel, "updateUploadProgressValue", Q_ARG(QVariant, QVariant::fromValue(value)));

        if (uploadGroupCount == uploadGroupCountTmp)
        {
            //std::function<void(const QByteArray)> call_back = std::bind(
             //   &SlicePlugin::modelGroupCreateResult, this, std::placeholders::_1);
            //modelGroupCreate(m_categoryId, m_groupName, m_groupDesc, m_isShare, m_modelType, m_license, m_isOriginal, modelNameList, modelFilekeyList, modelPathList, call_back);
            QString strContent = "";
            if ((modelNameList.size() == modelFilekeyList.size()) && (modelFilekeyList.size() == modelPathList.size()))
            {
                QString list_str = "[";
                for (int i = 0; i < modelNameList.size(); i++)
                {
                    QFileInfo info(QString(modelPathList.at(i)));
                    QString strTmp = QString("{\"fileKey\":\"%1\", \"fileName\":\"%2\", \"fileSize\":%3},").arg(QString(modelFilekeyList.at(i))).arg(QString(modelNameList.at(i))).arg(info.size());
                    list_str += strTmp;
                }
                list_str.chop(1);
                list_str += "]";

               strContent = QString("{\"groupItem\":{\"categoryId\": %1, \"groupName\": \"%2\", \"groupDesc\": \"%3\", \"isShared\": %4, \"modelColor\": %5, \"license\": \"%6\", \"isOriginal\": %7, \"covers\":[]}, \"modelList\":%8}")
                    .arg(m_categoryId)
                    .arg(m_groupName)
                    .arg(m_groupDesc)
                    .arg(m_isShare)
                    .arg(m_modelType)
                    .arg(m_license)
                    .arg(m_isOriginal)
                    .arg(list_str);
            }

            creative_kernel::getcxNetworkManager()->modelGroupCreate(strContent.toStdString(), std::bind(
                &SlicePlugin::modelGroupCreateResult, this, std::placeholders::_1));

            //remove the temp model stl
            QDir delDir(getCanWriteFolder() + "/uploadModels");
            delDir.removeRecursively();
        }
    }
}

void SlicePlugin::uploadGcodeFiles(const QStringList& fileNames)
{
    clusterUploadGcodeTmpCtn = 0;
    clusterUploadGcodeCtn = fileNames.length();
    clusterUploadGcodeNameList.clear();
    clusterUploadGcodeFilekeyList.clear();
    for (int i = 0; i < fileNames.length(); i++)
    {
        QString fileName = fileNames[i];
        QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
        QMetaObject::invokeMethod(fdmPanel, "letClusterUploadGcodeDigShow");

        QString modelPath = getCanWriteFolder() + "/uploadGcodes";
        QDir tempDir;
        if (!tempDir.exists(modelPath))
        {
            tempDir.mkpath(modelPath);
        }

        QFileInfo info(fileName);
       // m_fileName
        //QString file_name = info.fileName().section(".", 0, 0);
        clusterUploadGcodeNameList.append(info.fileName().section(".", 0, 0));
        QString gzFileName = modelPath + "/" + info.fileName().section(".", 0, 0) + ".gcode" + ".gz";
        if (QFile::exists(gzFileName))
        {
            QFile::remove(gzFileName);
        }

        QFile file(fileName);
        QByteArray gzipBateArray = "";
        if (file.open(QIODevice::ReadOnly/* | QIODevice::Text*/))
        {
            //gzipBateArray = file.readAll();
            int len = 0;
            char buf[1024];
            while (len = file.read(buf, 1024))
            {
                gzipBateArray.append((const char*)buf, len);
            }
        }
        file.close();
        gzFile gzfp = gzopen(gzFileName.toLocal8Bit(), "wb");
        if (!gzfp)
        {
            qDebug() << "gzopen error";
        }
        if (gzwrite(gzfp, gzipBateArray, gzipBateArray.size()) > 0)
        {
            qDebug() << "gzwrite ok";
        }
        gzclose(gzfp);

        //oSSInfo aliyunInfo;
       // getAliyunOssInfo(aliyunInfo);
        oss_wrapper::OSSInterface::initOSSConfig(m_accessKeyId, m_secretAccessKey, m_endPoint, m_fileBucket, m_sessionToken);
        //m_fileKey
        QString file_key = m_prefixPath + "/" + getFileMd5(gzFileName) + ".gcode.gz";
        clusterUploadGcodeFilekeyList.append(file_key);
        std::function<void(long long, long long)> progress_call_back =
            std::bind(&SlicePlugin::uploadClusterGcodeFilesProgress, this, std::placeholders::_1,
                std::placeholders::_2);
        std::function<void(const oss_wrapper::OSSError&)> result_call_back =
            std::bind(&SlicePlugin::uploadClusterGcodeFilesResult, this, std::placeholders::_1);

        oss_wrapper::OSSInterface::uploadObject(file_key, gzFileName, progress_call_back, result_call_back);
    }
}

void SlicePlugin::uploadClusterGcodeFilesProgress(long long upload_size, long long total_size)
{
}

void SlicePlugin::uploadClusterGcodeFilesResult(const oss_wrapper::OSSError& error)
{
    QString ret = QString::fromStdString(error.code_);
    if (ret == "200") {

        clusterUploadGcodeTmpCtn++;
        QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
        int value = ((clusterUploadGcodeTmpCtn * 1.0 / clusterUploadGcodeCtn * 1.0) * 100);
        if (value >= 100)
        {
            value = 99;
        }
        QMetaObject::invokeMethod(fdmPanel, "updateUploadProgressValue", Q_ARG(QVariant, QVariant::fromValue(value)), Q_ARG(QVariant, QVariant::fromValue(value)));

        if (clusterUploadGcodeCtn == clusterUploadGcodeTmpCtn)
        {
            //std::function<void(const QByteArray)> call_back = std::bind(
            //    &SlicePlugin::uploadGCodeResult, this, std::placeholders::_1);
            //uploadGCode(clusterUploadGcodeNameList, clusterUploadGcodeFilekeyList, call_back);

            QString list_str = "[";
            for (int i = 0; i < clusterUploadGcodeNameList.size(); i++)
            {
                QString strTmp = QString("{\"name\": \"%1\", \"filekey\" : \"%2\"},").arg(QString(clusterUploadGcodeNameList.at(i))).arg(QString(clusterUploadGcodeFilekeyList.at(i)));
                list_str += strTmp;
            }
            list_str.chop(1);
            list_str += "]";

            QString strContent = QString("{\"list\":%1}").arg(list_str);
            creative_kernel::getcxNetworkManager()->uploadGcode(strContent.toStdString(), std::bind(
                &SlicePlugin::uploadGCodeResult, this, std::placeholders::_1));

            //remove the temp model stl
            QDir delDir(getCanWriteFolder() + "/uploadGcodes");
            delDir.removeRecursively();
        }
    }
    else {
        QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
        QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
        QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
    }
}

void SlicePlugin::getAliyunOssInfomation(int type)
{
    getAliyunOssInfoType = type;
    getAliyunOssInfoCtn = 0;
    getAliyunOssKey();
    getAliyunOssBucket();

}

void SlicePlugin::getAliyunOssKey()
{
    QString strContent = QString("{}");
    QString strurl = "/api/cxy/account/v2/getAliyunInfo";
    std::function<void(QByteArray)> call_back = std::bind(
        &SlicePlugin::funcAliyunOssKeyResult, this, std::placeholders::_1);
    QMap<QString, QString> data;
    data.clear();
    data.insert("__REQUEST_URl__", strurl);
    data.insert("__REQUEST_TYPE__", "1");
    data.insert("__REQUEST_BODY__", strContent.toUtf8());
    data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
    data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
    creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void SlicePlugin::funcAliyunOssKeyResult(const QByteArray& response)
{
    QJsonParseError json_error;
    QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
    if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
        QString strJson(document.toJson(QJsonDocument::Compact));

        QJsonObject object = document.object();
        const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
        const QJsonObject aliyunInfo = result.value(QString::fromLatin1("aliyunInfo")).toObject();
        int state = object.value(QString::fromLatin1("code")).toInt();

        if (state == 0)
        {
            m_accessKeyId = aliyunInfo.value(QString::fromLatin1("accessKeyId")).toString();
            m_secretAccessKey = aliyunInfo.value(QString::fromLatin1("secretAccessKey")).toString();
            m_sessionToken = aliyunInfo.value(QString::fromLatin1("sessionToken")).toString();

            getAliyunOssInfoCtn++;
        }
        else
        {
            if (getAliyunOssInfoType == 1)
            {
                QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
                QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
                QMetaObject::invokeMethod(pRifhtPanel, "uploadModelError");
            }
            else {
                QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
                QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
                QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
            }
        }
    }
    else
    {
        if (getAliyunOssInfoType == 1)
        {
            QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
            QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
            QMetaObject::invokeMethod(pRifhtPanel, "uploadModelError");
        }
        else {
            QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
            QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
            QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
        }
    }


    if (getAliyunOssInfoCtn >= 2)
    {
        //todo
        if (getAliyunOssInfoType == 1)
        {
            uploadModelToAliyunOss();
        }
        else if (getAliyunOssInfoType == 2)
        {
            uploadGcodeToAliyunFromSofeware();
        }
        else if (getAliyunOssInfoType == 3)
        {
            uploadLocalSingleGcodeToAliyun();
        }
        else if (getAliyunOssInfoType == 4)
        {
            uploadGcodeFiles(m_uploadClusterFileNameList);
        }
    }
}

void SlicePlugin::getAliyunOssBucket()
{
    QString strContent = QString("{}");
    QString strurl = "/api/cxy/v2/common/getOssInfo";
    std::function<void(QByteArray)> call_back = std::bind(
        &SlicePlugin::funcAliyunOssBucketResult, this, std::placeholders::_1);
    QMap<QString, QString> data;
    data.clear();
    data.insert("__REQUEST_URl__", strurl);
    data.insert("__REQUEST_TYPE__", "1");
    data.insert("__REQUEST_BODY__", strContent.toUtf8());
    data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
    data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
    creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
}

void SlicePlugin::funcAliyunOssBucketResult(const QByteArray& response)
{
    QJsonParseError json_error;
    QJsonDocument document = QJsonDocument::fromJson(response, &json_error);
    if (!document.isNull() && json_error.error == QJsonParseError::NoError) {
        QString strJson(document.toJson(QJsonDocument::Compact));

        QJsonObject object = document.object();
        const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
        const QJsonObject info = result.value(QString::fromLatin1("info")).toObject();
        const QJsonObject file = info.value(QString::fromLatin1("file")).toObject();
        const QJsonObject internal = info.value(QString::fromLatin1("internal")).toObject();
        int state = object.value(QString::fromLatin1("code")).toInt();

        if (state == 0)
        {
            m_endPoint = info.value(QString::fromLatin1("endpoint")).toString();
            m_fileBucket = file.value(QString::fromLatin1("bucket")).toString();
            m_prefixPath = file.value(QString::fromLatin1("prefixPath")).toString();
            m_internalBucket = internal.value(QString::fromLatin1("bucket")).toString();
            m_internalPrefixPath = internal.value(QString::fromLatin1("prefixPath")).toString();

            getAliyunOssInfoCtn++;
        }
        else
        {
            if (getAliyunOssInfoType == 1)
            {
                QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
                QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
                QMetaObject::invokeMethod(pRifhtPanel, "uploadModelError");
            }
            else {
                QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
                QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
                QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
            }
        }
    }
    else
    {
        if (getAliyunOssInfoType == 1)
        {
            QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
            QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
            QMetaObject::invokeMethod(pRifhtPanel, "uploadModelError");
        }
        else {
            QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
            QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
            QMetaObject::invokeMethod(fdmPanel, "uploadGCodeError");
        }
    }

    if (getAliyunOssInfoCtn >= 2)
    {
        //todo
        if (getAliyunOssInfoType == 1)
        {
            uploadModelToAliyunOss();
        }
        else if (getAliyunOssInfoType == 2)
        {
            uploadGcodeToAliyunFromSofeware();
        }
        else if (getAliyunOssInfoType == 3)
        {
            uploadLocalSingleGcodeToAliyun();
        }
        else if (getAliyunOssInfoType == 4)
        {
            uploadGcodeFiles(m_uploadClusterFileNameList);
        }
    }
}

void SlicePlugin::uploadModelToAliyunOss()
{
    QString modelPath = getCanWriteFolder() + "/uploadModels";
    QDir tempDir;
    if (!tempDir.exists(modelPath))
    {
        tempDir.mkpath(modelPath);
    }

    if (m_uploadModelType == 1) //combination
    {
        QString name = CXFILE.currOpenFile();
        m_modelFileName = modelPath + +"/" + name + ".stl";
        creative_kernel::cmdSaveAs(m_modelFileName);

       // oSSInfo aliyunInfo;
       // getModelAliyunOssInfo(aliyunInfo);
        oss_wrapper::OSSInterface::initOSSConfig(m_accessKeyId, m_secretAccessKey, m_endPoint, m_internalBucket, m_sessionToken);
        m_fileKey = m_internalPrefixPath + "/" + getFileMd5(m_modelFileName) + ".stl";
        std::function<void(long long, long long)> progress_call_back =
            std::bind(&SlicePlugin::uploadModelProgress, this, std::placeholders::_1,
                std::placeholders::_2);
        std::function<void(const oss_wrapper::OSSError&)> result_call_back =
            std::bind(&SlicePlugin::uploadModelResult, this, std::placeholders::_1);

        oss_wrapper::OSSInterface::uploadObject(m_fileKey, m_modelFileName, progress_call_back, result_call_back);
    }
    else if (m_uploadModelType == 0) //no combination
    {
        modelNameList.clear();
        modelPathList.clear();
        modelFilekeyList.clear();
        creative_kernel::cmdSaveSelectStl(modelPath);
        //oSSInfo aliyunInfo;
        //getModelAliyunOssInfo(aliyunInfo);
        oss_wrapper::OSSInterface::initOSSConfig(m_accessKeyId, m_secretAccessKey, m_endPoint, m_internalBucket, m_sessionToken);
        uploadGroupCount = 0;
        uploadGroupCountTmp = 0;
        QList<ModelN*> models = selectionms();
        size_t size = models.size();
        uploadGroupCount = size;
        if (size > 0)
        {
            for (size_t i = 0; i < models.size(); i++)
            {
                QString name = models[i]->objectName();
                QString name1 = models[i]->objectName().section(".", 0, 0);

                modelNameList.append(models[i]->objectName().section(".", 0, 0));
                QString strPath = modelPath + "/" + models[i]->objectName().section(".", 0, 0) + ".stl";
                modelPathList.append(strPath);
                QString fileKey = "";
                fileKey = m_internalPrefixPath + "/" + getFileMd5(strPath) + ".stl";
                modelFilekeyList.append(fileKey);

                std::function<void(long long, long long)> progress_call_back =
                    std::bind(&SlicePlugin::uploadModelGroupSingleProgress, this, std::placeholders::_1,
                        std::placeholders::_2);
                std::function<void(const oss_wrapper::OSSError&)> result_call_back =
                    std::bind(&SlicePlugin::uploadModelGroupSingleResult, this, std::placeholders::_1);

                oss_wrapper::OSSInterface::uploadObject(fileKey, strPath, progress_call_back, result_call_back);
            }
        }
    }
    else if (m_uploadModelType == 3) //local file
    {
        uploadGroupCount = 0;
        uploadGroupCountTmp = 0;
        modelNameList.clear();
        modelFilekeyList.clear();

        //oSSInfo aliyunInfo;
        //getModelAliyunOssInfo(aliyunInfo);
        oss_wrapper::OSSInterface::initOSSConfig(m_accessKeyId, m_secretAccessKey, m_endPoint, m_internalBucket, m_sessionToken);

        uploadGroupCount = modelPathList.size();
        if (modelPathList.size())
        {
            for (QString strPath : modelPathList)
            {
                QString file_name = strPath.right(strPath.length() - strPath.lastIndexOf("/") - 1);
                QString suffix = file_name.right(file_name.length() - file_name.lastIndexOf("."));
                modelNameList.append(file_name.section(".", 0, 0));

                QString fileKey = "";
                fileKey = m_internalPrefixPath + "/" + getFileMd5(strPath) + suffix;
                modelFilekeyList.append(fileKey);

                std::function<void(long long, long long)> progress_call_back =
                    std::bind(&SlicePlugin::uploadModelGroupSingleProgress, this, std::placeholders::_1,
                        std::placeholders::_2);
                std::function<void(const oss_wrapper::OSSError&)> result_call_back =
                    std::bind(&SlicePlugin::uploadModelGroupSingleResult, this, std::placeholders::_1);

                oss_wrapper::OSSInterface::uploadObject(fileKey, strPath, progress_call_back, result_call_back);
            }
        }
    }
}

void SlicePlugin::uploadGcodeToAliyunFromSofeware()
{
    QString modelPath = getCanWriteFolder() + "/uploadGcodes";
    QDir tempDir;
    if (!tempDir.exists(modelPath))
    {
        tempDir.mkpath(modelPath);
    }
    QString fileName = modelPath + "/" + m_uploadGcodeName + ".gcode";
    SliceAttain* attain = m_slicePreviewFlow->attain();
    if (attain)
    {
        attain->saveGCode(fileName, nullptr);
    }
    QString gzFileName = fileName + ".gz";
    if (QFile::exists(gzFileName))
    {
        QFile::remove(gzFileName);
    }

    QFile file(fileName);
    QByteArray gzipBateArray = "";
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //gzipBateArray = file.readAll();
        int len = 0;
        char buf[1024];
        while (len = file.read(buf, 1024))
        {
            gzipBateArray.append((const char*)buf, len);
        }
    }
    file.close();
    gzFile gzfp = gzopen(gzFileName.toLocal8Bit(), "wb");
    if (!gzfp)
    {
        qDebug() << "gzopen error";
    }
    if (gzwrite(gzfp, gzipBateArray, gzipBateArray.size()) > 0)
    {
        qDebug() << "gzwrite ok";
    }
    gzclose(gzfp);

    //oSSInfo aliyunInfo;
   // getAliyunOssInfo(aliyunInfo);
    oss_wrapper::OSSInterface::initOSSConfig(m_accessKeyId, m_secretAccessKey, m_endPoint, m_fileBucket, m_sessionToken);
    m_fileKey = m_prefixPath + "/" + getFileMd5(gzFileName) + ".gcode.gz";
    std::function<void(long long, long long)> progress_call_back =
        std::bind(&SlicePlugin::uploadProgress, this, std::placeholders::_1,
            std::placeholders::_2);
    std::function<void(const oss_wrapper::OSSError&)> result_call_back =
        std::bind(&SlicePlugin::uploadResult, this, std::placeholders::_1);

    oss_wrapper::OSSInterface::uploadObject(m_fileKey, gzFileName, progress_call_back, result_call_back);
}

void SlicePlugin::uploadLocalSingleGcodeToAliyun()
{
    QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
    QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");
    QMetaObject::invokeMethod(fdmPanel, "letClusterUploadGcodeDigShow");

    QString modelPath = getCanWriteFolder() + "/uploadGcodes";
    QDir tempDir;
    if (!tempDir.exists(modelPath))
    {
        tempDir.mkpath(modelPath);
    }

    QFileInfo info(m_uploadGcodeName);
    m_fileName = info.fileName().section(".", 0, 0);
    QString gzFileName = modelPath + "/" + info.fileName().section(".", 0, 0) + ".gcode" + ".gz";
    if (QFile::exists(gzFileName))
    {
        QFile::remove(gzFileName);
    }

    QFile file(m_uploadGcodeName);
    QByteArray gzipBateArray = "";
    if (file.open(QIODevice::ReadOnly/* | QIODevice::Text*/))
    {
        //gzipBateArray = file.readAll();
        int len = 0;
        char buf[1024];
        while (len = file.read(buf, 1024))
        {
            gzipBateArray.append((const char*)buf, len);
        }
    }
    file.close();
    gzFile gzfp = gzopen(gzFileName.toLocal8Bit(), "wb");
    if (!gzfp)
    {
        qDebug() << "gzopen error";
    }
    if (gzwrite(gzfp, gzipBateArray, gzipBateArray.size()) > 0)
    {
        qDebug() << "gzwrite ok";
    }
    gzclose(gzfp);

    //oSSInfo aliyunInfo;
    //getAliyunOssInfo(aliyunInfo);
    oss_wrapper::OSSInterface::initOSSConfig(m_accessKeyId, m_secretAccessKey, m_endPoint, m_fileBucket, m_sessionToken);
    m_fileKey = m_prefixPath + "/" + getFileMd5(gzFileName) + ".gcode.gz";
    std::function<void(long long, long long)> progress_call_back =
        std::bind(&SlicePlugin::uploadProgress, this, std::placeholders::_1,
            std::placeholders::_2);
    std::function<void(const oss_wrapper::OSSError&)> result_call_back =
        std::bind(&SlicePlugin::uploadResult, this, std::placeholders::_1);

    oss_wrapper::OSSInterface::uploadObject(m_fileKey, gzFileName, progress_call_back, result_call_back);
}

void SlicePlugin::visibleSliceUI(SlicerManager::SLICERTYPE type)
{
    if (type == SlicerManager::SLICERTYPE::FDM)
    {
        m_sliceUI->setObject("slice", this);
        m_sliceUI->setObject("slicemodel", m_slicePreviewFlow);
        m_sliceUI->update();
        CXFILE.registerOpenHandler(m_previewWorker);
    }
    else
    {
        m_slicePreviewFlow->clear();
        CXFILE.unRegisterOpenHandler(m_previewWorker->supportFilters());
    }
}


bool SlicePlugin::isslice()
{
    return m_bSlicing;
}

void SlicePlugin::onSliceSuccess(SliceAttain* attain)
{
    //qDebug() << "fdm onSliceSuccess";
	m_slicePreviewFlow->setSliceAttain(attain);
    m_sliceUI->setParam("fdmColor", (void*)(attain->nozzle() - 1));
    m_printingTime = attain->printingTime();
    m_sliceUI->onSliceSuccess(attain->getReport());
    m_bSlicing = false;
}

void SlicePlugin::onSliceFailed()
{
    qDebug() << "fdm onSliceFailed";
    m_sliceUI->onSliceFailed();
    QObject* pSlicePanel = AbstractKernelUI::getSelf()->getUI("rightpanel");
    QObject* pRifhtPanel = pSlicePanel->findChild<QObject*>("RightInfoPanel");
    QMetaObject::invokeMethod(pRifhtPanel, "enableSliceBtn");

    m_bSlicing = false;
}

QString SlicePlugin::getTmpFilePeth()
{
    return m_strTmpFilePath;
}

void SlicePlugin::localSaveTmpGcode()
{
    QString prefix = getCanWriteFolder();
    QString fileName = CXFILE.currOpenFile();
    QString codePath = prefix + "/" + "1.cxcode";
    QString filePath = prefix + QString("/%1.gcode").arg(fileName);

    QDir dir(prefix);
    QStringList entryList = dir.entryList(QStringList()<<"*.gcode", QDir::Files);
    foreach (QString strName, entryList)
    {
        if(strName.compare(fileName, Qt::CaseInsensitive) != 0) dir.remove(strName);
    }
    if(QFile::exists(codePath)) QFile::remove(codePath);

    m_filePath = filePath;
    m_strTmpFilePath = filePath;
    m_sliceUI->handle();
    m_filePath = filePath;

    SliceAttain* attain = m_slicePreviewFlow->attain();
    if(attain)
    {
        QString hasPreview = GLOBALPARAM("has_preview_img", "0");
        bool isGcode = CXFILE.lastOpenFileName().endsWith(".gcode");
        qDebug()<<"isGcode"<<isGcode<<"filePath"<<filePath;

        if(hasPreview == "true" && !isGcode)
        {
            //QString currentMachineName = SlicerManager::instance().getCurrentMachine();
            auto f = [this, attain, filePath, codePath](QImage& image)
            {
                //image.save(savePath);
                attain->saveGCode(filePath, &image);
                attain->saveGCode(codePath, &image);
                m_slicePreviewFlow->endRequest();

                emit sigSendGcodeInfo(filePath);
                if(!isUSBonline) onSaveSuccess();
                //onExporSuccess();
            };

            m_slicePreviewFlow->requestPreview(f);
        }
        else {
            if(isGcode)
            {
                QFile::copy(CXFILE.lastOpenFilePath(), filePath);
            }
            else {
                attain->saveGCode(filePath, nullptr);
                attain->saveGCode(codePath, nullptr);
            }

            emit sigSendGcodeInfo(filePath);
            if(!isUSBonline) onSaveSuccess();
            //onExporSuccess();
        }
    }
}

//void SlicePlugin::handle(const QString& fileName)
//{
//    qDebug() << "fdm save gcode";
//    m_sliceUI->handle();
//    m_filePath = fileName;
//    SliceAttain* attain = m_slicePreviewFlow->attain();
//    if (attain)
//    {
//        QString currentMachineName = SlicerManager::instance().getCurrentMachine();

//        {
//            auto f = [this, attain, fileName](QImage& image)
//            {
//                image.save("test.bmp");
//                attain->saveGCode(fileName, &image);
//                m_slicePreviewFlow->endRequest();
//                onExporSuccess();
//            };

//         //   bool hasPreviewImage = GLOBALPARAM("has_preview_img", "0").toInt();
//            QString hasPreviewImage = GLOBALPARAM("has_preview_img", "0");

//            if (hasPreviewImage == "true")
//                m_slicePreviewFlow->requestPreview(f);
//            else
//            {
//                attain->saveGCode(fileName, nullptr);
//                onExporSuccess();
//            }
//        }
//    }
//}

void SlicePlugin::handle(const QStringList& fileNames)
{
    if (isClusterUploadGcode)
    {
        m_uploadClusterFileNameList.clear();
        m_uploadClusterFileNameList = fileNames;
        getAliyunOssInfomation(4);
        isClusterUploadGcode = false;
        return;
    }

    if (isUploadModelFile)
    {
        modelPathList.clear();
        modelPathList = fileNames;

        QString strContent = QString("{\"type\":%1}").arg(7);
        QString strurl = "/api/cxy/v2/common/categoryList";
        std::function<void(QByteArray)> call_back = std::bind(
            &SlicePlugin::getCategoryListResult, this, std::placeholders::_1);
        QMap<QString, QString> data;
        data.clear();
        data.insert("__REQUEST_URl__", strurl);
        data.insert("__REQUEST_TYPE__", "1");
        data.insert("__REQUEST_BODY__", strContent.toUtf8());
        data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
        data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
        creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
        return;
    }
}

void SlicePlugin::handle(const QString& fileName)
{
    if (isClusterUploadGcode)
    {
        m_uploadGcodeName = fileName;
        getAliyunOssInfomation(3);
        isClusterUploadGcode = false;
        return;
    }

    if (isUploadModelFile)
    {
        modelPathList.clear();
        modelPathList.append(fileName);

        QString strContent = QString("{\"type\":%1}").arg(7);
        QString strurl = "/api/cxy/v2/common/categoryList";
        std::function<void(QByteArray)> call_back = std::bind(
            &SlicePlugin::getCategoryListResult, this, std::placeholders::_1);
        QMap<QString, QString> data;
        data.clear();
        data.insert("__REQUEST_URl__", strurl);
        data.insert("__REQUEST_TYPE__", "1");
        data.insert("__REQUEST_BODY__", strContent.toUtf8());
        data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
        data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
        creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);
        return;
    }
    m_sliceUI->handle();
    m_filePath = fileName;

    QFileInfo fileinfo(fileName);
    QString suffix = fileinfo.suffix();

    QString sourcefileName = "";
    if ("cxcode" == suffix)
    {
        sourcefileName = getCanWriteFolder() + "/" + "1.cxcode";
        m_filePath = fileName;
    }
    else
    {
        sourcefileName = getCanWriteFolder() + QString("/%1.gcode").arg(CXFILE.currOpenFile());//"/1.gcode"
        m_filePath = fileName;
    }

    qDebug() << "handle to " << sourcefileName;

    QByteArray bUtf8 = fileName.toUtf8();
    QString fileName_utf8 = bUtf8;

    bool bRet = copyFileToPath(sourcefileName, fileName_utf8);
    if (bRet)
    {
        onExporSuccess();
        //delete cache file
        QFile fileTemp(sourcefileName);
        fileTemp.remove();
    }
}

bool SlicePlugin::copyFileToPath(QString sourceDir, QString toDir, bool deleteFileIfExist)
{
    QDir* m_createfile = new QDir();
    // toDir.replace("\\","/");
    // （1）源文件和目标文件一样，（源文件就在目标目录下）返回ture
    if (sourceDir == toDir)
    {
        return true;
    }
    // （2）源文件不存在，返回false
    if (!QFile::exists(sourceDir))
    {
        return false;
    }
    // （3）源文件不在目标目录下，但目标目录下的目标文件已存在
    bool exist = m_createfile->exists(toDir); // 如果目标文件存在，返回ture
    if (exist)
    {
        if (deleteFileIfExist)
        {
            // 如果目标文件存在，且bool量deleteFileIfExist为1，删除目标文件
            m_createfile->remove(toDir);
        }
    }
    // （4）把源文件复制成目标文件，操作成功返回ture
    if (!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}
//save tmp gcode
void SlicePlugin::onSaveSuccess()
{
    if (m_saveGCodeFlag == 1)
    {
        sliceMainOp("beginsave");
        return;
    }
    else if (m_saveGCodeFlag == 2)
    {
        //SliceAttain* attain = m_slicePreviewFlow->attain();
        if (m_command)
        {
            m_command->sliceSuccess();
        }
    }

    m_saveGCodeFlag = 0;
}
void SlicePlugin::onExporSuccess()
{
    qDebug() << "fdm slice onExporSuccess";
    m_sliceUI->onExporSuccess(nullptr, (void*)m_isOnlinePrint);
    if (!m_isOnlinePrint)
    {
        CXFILE.setLastSaveFileName(m_filePath.replace("/", "\\"));
    }
    m_isOnlinePrint = false;
}


int SlicePlugin::sliceMainOp(QString op, int param)
{
    op = op.toLower();
    qDebug() << "SlicePlugin::sliceMainOp begin op = " << op;
    if (op == "slice")
    {
        qInfo() << "fdm slice";
        creative_kernel::sendDataToGA("Slice", "Start Slicing");
        m_bSlicing = true;
        m_sliceStartTime = (double)clock();

        QSharedPointer<AnsycWorker> worker(new AnsycWorker());

        worker->setRemainAttain(m_slicePreviewFlow->takeAttain());
        connect(worker.data(), SIGNAL(sliceSuccess(SliceAttain*)), this, SLOT(onSliceSuccess(SliceAttain*)));
        connect(worker.data(), SIGNAL(sliceFailed()), this, SLOT(onSliceFailed()));

        us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
        QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();
        for (const auto val : globalsetting->settings())
        {
            globalsetting->settings().value(val->key())->setValue(hashItemDef[val->key()]->showValueStr);
        }
        worker->setGlobalSettings(globalsetting);
        creative_kernel::executeJob(worker);
        return 0;
    }
    else if (op == "preview")
    {
        qDebug() << "fdm slice beginto preview ";

        renderRenderGraph(m_slicePreviewFlow);
        qDebug() << "fdm slice preview after renderRenderGraph";

        selectOne(nullptr);
        SlicerManager::instance().setDoingSlice(true);
        QTimer::singleShot(200, m_sliceUI, SLOT(preview()));
        return 0;
    }
    else if (op == "unpreview")
    {
        qDebug() << "fdm slice unpreview";
        renderDefaultRenderGraph();
        selectOne(nullptr);
        SlicerManager::instance().setDoingSlice(false);
        QTimer::singleShot(200, m_sliceUI, SLOT(unpreview()));

        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        QMetaObject::invokeMethod(pTopBar, "letSelectPrinterEnable", Q_ARG(QVariant, QVariant::fromValue(true)));
        m_slicePreviewFlow->resetSteps();
        return 0;
    }
    else if (op == "detectfdmsupports")
    {
        bool support_enable = GLOBALPARAM("support_enable", "false").toLower() == "true" ? true : false;
        if (support_enable)
            return false;

        return spaceHasFDMSupport();
    }
    else if (op == "cancelfdmsupports")
    {
        spaceCancelFDMSupport();
        return 0;
    }
    else if (op == "usbonlineprint")
    {
        if (isRunning)
        {
            return 0;
        }

        isUSBonline = true;
        QString fileName = getCanWriteFolder() + "/" + "temp.gcode";

        if (QFile::exists(fileName))
        {
            QFile::remove(fileName);
        }
        SliceAttain* attain = m_slicePreviewFlow->attain();
        if (attain)
        {
            attain->saveGCode(fileName, nullptr);
        }

        QObject* pmainWinbj = AbstractKernelUI::getSelf()->getUI("UIAppWindow");
        m_usbPrinterModel->startPrint("", fileName.toStdString(), m_printingTime);
        QMetaObject::invokeMethod(pmainWinbj, "showFdmPmDlg", Q_ARG(QVariant, QVariant::fromValue(fileName)));

        creative_kernel::sendDataToGA("rightPanle", "usbonlineprint");
        isUSBonline = false;
        return 0;
    }
    else if (op == "beginsave")
    {
        qDebug() << "beginSave";
        QString format = GLOBALPARAM("export_format", "gcode");
        QStringList filters = format.split(",");
        filters << "cxcode";//by TCJ
        CXFILE.save(this, filters);
        isUploadModelFile = false;
        creative_kernel::sendDataToGA("G-code Import", "G-code Import");
        return 0;
    }
    else if (op == "savetmpgcode")
    {
        m_saveGCodeFlag = 1;
        localSaveTmpGcode();
    }
    else if (op == "updatetmpgcode")
    {
        creative_kernel::sendDataToGA("G-code Upload", "G-code Upload");
        m_saveGCodeFlag = param;
        localSaveTmpGcode();
    }
    else if (op == "getuploadmodelname")
    {
        bool loginStatus = (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1);
        QString modelName = loginStatus ? CXFILE.currOpenFile() : QString();
        emit sigSendModelName(modelName);
    }
    else if (op == "sigselecttype")
    {
        m_slicePreviewFlow->scene()->setColorType(param);
        renderOneFrame();
        return 0;
    }
    else if (op == "changeshowinfo")
    {
        int type = (param & 0xff00) >> 8;
        int state = param & 0xff;

        qtuser_3d::PrinterEntity* entity = getCachedPrinterEntity();
        if (0 == type)
        {
            entity->showPrinterEntity(state);
        }

        if (1 == type)
        {
            m_slicePreviewFlow->scene()->showIndicator(state);
        }
        renderOneFrame();
        return 0;
    }
    else if (op == "uploadslicetocrealitycloud")
    {
        if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1)
        {
            QObject* sliceui = AbstractKernelUI::getSelf()->getUI("rightpanel");
            QObject* fdmPanel = sliceui->findChild<QObject*>("FDMPreviewPanel");

            if(fdmPanel)
            {
                QString name = CXFILE.currOpenFile();
                QMetaObject::invokeMethod(fdmPanel, "uploadDlgShow", Q_ARG(QVariant, QVariant::fromValue(name)));
            }
        }
        else
        {
            QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
            //QObject* pLogin = pTopBar->findChild<QObject*>("LoginDlg");
            QString param = "gcodeUpload";
            QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(param)));
        }
    }
    else if (op == "fdmpreviewpanel.visible=true")
    {
        qDebug() << "SlicePlugin::sliceMainOp fdmpreviewpanel.visible=true";
        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        QMetaObject::invokeMethod(pTopBar, "letSelectPrinterEnable", Q_ARG(QVariant, QVariant::fromValue(false)));
    }

    return 0;
}

void SlicePlugin::uploadToAliyun(QString file_name)
{
    m_fileName = file_name;
    m_uploadGcodeName = file_name;

    getAliyunOssInfomation(2);
}

void SlicePlugin::uploadModelToAliyun(int categoryId, QString groupName, QString groupDesc, bool isShare, bool isOriginal, int modelType, QString license, int isCombine)
{
    m_categoryId = categoryId;
    m_groupName = groupName;
    m_groupDesc = groupDesc;
    if (isShare) {
        m_isShare = "true";
    }
    else {
        m_isShare = "false";
    }
    if (isOriginal) {
        m_isOriginal = "true";
    }
    else {
        m_isOriginal = "false";
    }
    m_modelType = modelType;
    m_license = license;
    m_uploadModelType = isCombine;

    getAliyunOssInfomation(1);
}

void SlicePlugin::uploadModelBtnClick()
{

    isUploadModelFile = false;

    if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1) {

        QString strContent = QString("{\"type\":%1}").arg(7);
        QString strurl = "/api/cxy/v2/common/categoryList";
        std::function<void(QByteArray)> call_back = std::bind(
            &SlicePlugin::getCategoryListResult, this, std::placeholders::_1);
        QMap<QString, QString> data;
        data.clear();
        data.insert("__REQUEST_URl__", strurl);
        data.insert("__REQUEST_TYPE__", "1");
        data.insert("__REQUEST_BODY__", strContent.toUtf8());
        data.insert("__REQUEST_TOKEN__", creative_kernel::getcxNetworkManager()->getLoginInfoToken());
        data.insert("__REQUEST_USER_ID__", creative_kernel::getcxNetworkManager()->getLoginInfoUserID());
        creative_kernel::getcxNetworkManager()->addRequestTask(1, data, call_back);

        creative_kernel::sendDataToGA("Model Upload", "Model Upload (Platform)");
    }
    else
    {
        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        QObject* pLogin = pTopBar->findChild<QObject*>("LoginDlg");
        QString param = "modelUpload";
        QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(param)));
    }
}

void SlicePlugin::uploadModelLocalFileBtnClick()
{
    if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1) {

        QStringList filters;
        filters.push_back("stl");
        filters.push_back("obj");
        filters.push_back("ply");
        filters.push_back("off");
        filters.push_back("3mf");
        filters.push_back("3ds");
        filters.push_back("wrl");
        filters.push_back("dae");
        filters.push_back("cxbin");
        CXFILE.open(this, filters);
        isUploadModelFile = true;
        creative_kernel::sendDataToGA("Model Upload", "Model Upload (Local)");
        isClusterUploadGcode = false;
    }
    else
    {
        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        QObject* pLogin = pTopBar->findChild<QObject*>("LoginDlg");
        QString param = "modelLocalFileUpload";
        QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(param)));
    }
}

bool SlicePlugin::outPlatform()
{
    int ret = creative_kernel::checkModelRange();
    if (ret) {
        return true;
    }
    else {
        return false;
    }
}

void SlicePlugin::viewMyModel()
{
    QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
    QString type = "mymodel";
    QMetaObject::invokeMethod(pUserInfoDlg, "setUserInfoDlgShow", Q_ARG(QVariant, QVariant::fromValue(type)));
}

void SlicePlugin::viewMyUploads()
{
    QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
    QString type = "myuploads";
    QMetaObject::invokeMethod(pUserInfoDlg, "setUserInfoDlgShow", Q_ARG(QVariant, QVariant::fromValue(type)));
}

void SlicePlugin::cloudPrinting(QString gcodeID)
{
    QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    QObject* pUserInfoDlg = pTopBar->findChild<QObject*>("UserInfoDlg");
    QMetaObject::invokeMethod(pUserInfoDlg, "printCloudPrinting", Q_ARG(QVariant, QVariant::fromValue(gcodeID)));
}

void SlicePlugin::clusterUploadLocalGcodeToAliyun()
{

    if (creative_kernel::getcxNetworkManager()->getLoginInfoLoginState() == 1) {
        QStringList filters;
        filters.push_back("gcode");
        CXFILE.open(this, filters);

#ifdef __APPLE__
        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        QMetaObject::invokeMethod(pTopBar, "closeGroupPrintInfoDlg");
#endif

        isClusterUploadGcode = true;
        isUploadModelFile = false;
    }
    else
    {
        QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
        QObject* pLogin = pTopBar->findChild<QObject*>("LoginDlg");
        QString param = "clusteruploadgcode";
        QMetaObject::invokeMethod(pTopBar, "showUploadMsgDlg", Q_ARG(QVariant, QVariant::fromValue(param)));
    }
}

QObject *SlicePlugin::sliceModel()
{
    return m_slicePreviewFlow;
}

QObject *SlicePlugin::slicePlugin()
{
    return this;
}

bool SlicePlugin::hasSwitch()
{
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    QString machine_support_slicer_type = globalsetting->settings().value("machine_support_slicer_type")->value().toString();
    QStringList machine_support_types = machine_support_slicer_type.split("-");
    return machine_support_types.length()>1;
}

void SlicePlugin::onProcessStateChanged(QProcess::ProcessState state)
{
    qDebug() << "show state:";
    switch (state)
    {
    case QProcess::NotRunning:
        qDebug() << "Not Running";
        isRunning = false;
        break;
    case QProcess::Starting:
        qDebug() << "Starting";
        break;
    case QProcess::Running:
        qDebug() << "Running";
        break;
    default:
        qDebug() << "otherState";
        break;
    }
}


