#include "addtabledata.h"

#include <QDebug>
#include "kernel/abstractkernel.h"
#include "interface/printermnginterface.h"
#include "kernel/kernel.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"


using namespace creative_kernel;
AddTableData::AddTableData(QObject* parent)
	:QObject(parent)
{
    PrinterNetMng* obj = getKernel()->printerNetMng();
    disconnect(obj, SIGNAL(signalPrinterMsg(const PrinterData&)), this, SLOT(slotPrinterMsg(const PrinterData&)));
    connect(obj, SIGNAL(signalPrinterMsg(const PrinterData&)), this, SLOT(slotPrinterMsg(const PrinterData&)), Qt::UniqueConnection);
    connect(obj, SIGNAL(signalProgressValue(int,int)), AbstractKernelUI::getSelf()->tableModel(), SLOT(slotProgressValue(int,int)), Qt::UniqueConnection);

    connect(AbstractKernelUI::getSelf()->tableModel(), SIGNAL(signalAddPrintnetData(QString, QString, QString)), obj, SLOT(slotAddPrintnetData(QString, QString, QString)), Qt::UniqueConnection);
    connect(AbstractKernelUI::getSelf()->tableModel(), SIGNAL(signalDeletePrinterByMac(QList<QString>)), obj, SLOT(slotDeletePrinterByMac(QList<QString>)), Qt::UniqueConnection);
}

AddTableData::~AddTableData()
{
}
//void set()
//{
//    bSleep = false;
//}
void AddTableData::add()
{
    //AbstractKernelUI::getSelf()->executeCommand("requestLoad", nullptr, "");
    //AbstractKernelUI::getSelf()->switchShowPop(false);
    ////slider bug 2021-1-21
    //QObject* pZsliderObj = AbstractKernelUI::getSelf()->getUI("UIRoot")->findChild<QObject*>("zsliderObj");
    //QMetaObject::invokeMethod(pZsliderObj, "updateSliderValue");
}
Q_INVOKABLE void AddTableData::searchPrinter()
{
    AbstractKernelUI::getSelf()->tableModel()->clearTableModel();
    scanNetPrinter();
    
}
void AddTableData::slotPrinterMsg(const PrinterData& pd)
{
    qDebug() << "slotPrinterMsg";
    qDebug() << pd.toString();
    AbstractKernelUI::getSelf()->tableModel()->addWifiPrinter(pd);
}

void AddTableData::remove(QModelIndexList indexs)
{
}
void AddTableData::selects(QModelIndexList indexs)
{
}
void AddTableData::sendLocalSliceFile(QString filePath)
{
    QString zipFilePath = filePath;
    QFileInfo fileInfo(filePath);
    if (AbstractKernelUI::getSelf()->tableModel()->isTranslateZip())
    {
        QFile::remove(fileInfo.filePath() + "/" + fileInfo.baseName() + ".zip");
        zipFilePath = ZipGCodeFile(filePath);
    }
    getKernel()->printerNetMng()->setFileName(zipFilePath);
    //QList<QString> macDataList = AbstractKernelUI::getSelf()->tableModel()->getSelectPrinterList();
    //if (macDataList.size() == 0)return;
    //getKernel()->printerNetMng()->checkPrinters(macDataList);
    AbstractKernelUI::getSelf()->tableModel()->refresh();
    QMap<int, QString> macDataMap = AbstractKernelUI::getSelf()->tableModel()->getSelectPrinterMap();
    if (macDataMap.size() == 0)return;
    getKernel()->printerNetMng()->slotSelectMacAndRow(macDataMap);
}
void AddTableData::remove(QObject* obj)
{

}
QString AddTableData::ZipGCodeFile(QString filePath)
{
    QuaZip* m_qzip = new QuaZip();
    if (!m_qzip) return "";
    QFileInfo fileinfo(filePath);
    QString path = fileinfo.path();
    QString fileName = fileinfo.baseName();
    QString zipFileName = path + "/" + fileName + ".zip";
    m_qzip->setZipName(zipFileName);
    if (!m_qzip->open(QuaZip::mdCreate))
    {
        m_qzip->close();
        delete(m_qzip);
        m_qzip = nullptr;
        return "";
    }
    QuaZipFile zipFile(m_qzip);

    if (!zipFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileinfo.fileName(), filePath)))
    {
        qDebug() << filePath << "open failed !";
        zipFile.close();
        return "";
    }
    QFile gcodeFile(filePath);
    if (!gcodeFile.open(QIODevice::ReadOnly)) return "";
    zipFile.write(gcodeFile.readAll().toStdString().c_str());
    zipFile.close();
    m_qzip->close();
    delete m_qzip;
    m_qzip = nullptr;
    return zipFileName;
}
