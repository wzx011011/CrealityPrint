#include "managematerial.h"
#include <interface/spaceinterface.h>
#include <interface/modelinterface.h>
#include "kernel/kernelui.h"

#include <QDebug>

#include <QtQml/QQmlProperty>
#include "kernel/translator.h"
#include "def/cryptfiledevice.h"
#include "us/slicermanager.h"

ManageMaterial::ManageMaterial(QObject* parent)
    :ActionCommand(parent)
    , m_obj(nullptr)
{
    m_actionname = "Manage Materials";
    m_actionNameWithout = "Manage Materials";
    m_eParentMenu = eMenuType_Tool;
    m_file = new MangeMaterialFile();
    disconnect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
    connect(Translator::getInstance(),SIGNAL(languageChanged()),this,SLOT(slotLanguageChanged()));
}

ManageMaterial::~ManageMaterial()
{
    if (m_file != nullptr)
    {
        delete m_file;
        m_file = nullptr;
    }
}

void ManageMaterial::slotLanguageChanged()
{
    m_actionname = "Manage Materials";
}
void ManageMaterial::execute()
{
    QObject* pMenu = getKernelUI()->appWindow()->findChild<QObject*>("allMenuDialog");
    if (pMenu)
    {
        QMetaObject::invokeMethod(pMenu, "showDiag");
        m_obj = pMenu->findChild<QObject*>("managematerialObj");
        if (m_obj)
        {
            requestMenuDialog(this);
        }
    }

    QString path = "D:\\Castable.default";
    //QString path = "C:\\Users\\cx1489\\AppData\\Roaming\\Creality\\Creative3D\\Materials\\Castable.default";
    CryptFileDevice* pFile = new CryptFileDevice(path, "8JGSEwfmq5hWKwwW4R-=[X9%OmOh-qwC", "rluH$Z+(ss+E]hms");
    if (pFile->open(QIODevice::WriteOnly))
    {

        std::string str= "material_name=Normal\r\nsensitivity=1\r\nmaterial_type=Normal\r\nexposure_time=5";
        pFile->write(str.c_str(), str.size());
        //QString strtmp = QString(by);
    }
    pFile->close();
    if (pFile->open(QIODevice::ReadOnly))
    {
        char buf[128];
        pFile->readLine(buf,128);
        QString strtmp = buf;
        qDebug() << strtmp;
        pFile->readLine(buf, 128);
        strtmp = buf;
        qDebug() << strtmp;
        pFile->readLine(buf, 128);
        strtmp = buf;
        qDebug() << strtmp;
    }
    pFile->close();

}

Q_INVOKABLE QString ManageMaterial::getValue(QString key)
{
    //	qDebug() << "getValue";
    return m_file->getValue(key);
}


Q_INVOKABLE void ManageMaterial::materialChanged(QString type)
{
    SlicerManager::instance().setCurrentMaterial(type);
    m_file->readDataFromFile(type);
}

Q_INVOKABLE void ManageMaterial::valueChanged(QString key, QString value)
{
    m_file->writeDataToFile(key, value);
    if ("material_cost" == key && "" != value)
    {
        QSettings setting;
        setting.beginGroup("profile_setting");
        setting.setValue("material_cost", value);
        setting.endGroup();
    }
}

Q_INVOKABLE void ManageMaterial::addMaterial(QString newName, QString oldName)
{
    int num = 0;
    m_file->addMaterial(newName, oldName);
    QStringList names = m_file->getAllMaterialName();
    for (int i = 0; i < names.size(); i++)
    {
        if (newName == names.at(i))
        {
            num = i;
            break;
        }
    }
    QMetaObject::invokeMethod(m_obj, "materials", Q_ARG(QVariant, QVariant::fromValue(names)));
    m_file->readDataFromFile(newName);
    QMetaObject::invokeMethod(m_obj, "initValue", Qt::AutoConnection, Q_ARG(QVariant, QVariant::fromValue(num)));
}

Q_INVOKABLE void ManageMaterial::resetMaterial()
{
    m_file->resetType();
}

Q_INVOKABLE void ManageMaterial::deleteMaterial()
{
    if (m_file->deletefile())
    {
        QStringList names = m_file->getAllMaterialName();
        QMetaObject::invokeMethod(m_obj, "materials", Q_ARG(QVariant, QVariant::fromValue(names)));
        QMetaObject::invokeMethod(m_obj, "initValue", Qt::AutoConnection, Q_ARG(QVariant, QVariant::fromValue(0)));
        //update
        QString strTmp = names[0];
        materialChanged(strTmp);
    }
}

QStringList ManageMaterial::getMaterialList()
{
    QStringList names = m_file->getAllMaterialName();
    return names;
}

void ManageMaterial::requestMenuDialog(QObject* receiver)
{
    QMetaObject::invokeMethod(m_obj, "init", Q_ARG(QVariant, QVariant::fromValue(receiver)));
    QStringList names = m_file->getAllMaterialName();
    if (names.size() == 0)
    {
        m_file->initFile();
        names = m_file->getAllMaterialName();
    }
    QMetaObject::invokeMethod(m_obj, "materials", Q_ARG(QVariant, QVariant::fromValue(names)));
    if (names.size())
    {
        m_file->readDataFromFile(names.at(0));
    }

    if ("DLP" == m_file->getCurrentMachine())
    {
        qDebug() << "DLP";
        QMetaObject::invokeMethod(m_obj, "machinetype", Q_ARG(QVariant, QVariant::fromValue(false)));
    }
    else
    {
        qDebug() << "fdm";
        QMetaObject::invokeMethod(m_obj, "machinetype", Q_ARG(QVariant, QVariant::fromValue(true)));
    }

    QMetaObject::invokeMethod(m_obj, "writeValue");
}

QString ManageMaterial::getMoneyType()
{
    QString strMoneyType = Translator::getInstance()->getMoneyType();
    return strMoneyType;
}


