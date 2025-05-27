#include "cx3dscene.h"
#include <Qt3DCore/QTransform>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#include "us/configurationmanager.h"
#include "us/slicermanager.h"
#include "kernel/abstractkernel.h"
#include "interface/spaceinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/reuseableinterface.h"
#include <QtCore/QFileInfo>
#include "data/fdmsupportgroup.h"
#include <QStandardPaths>
#include "def/settingdef.h"

#include "utils/convexhullcalculator.h"
#include "mmesh/trimesh/trimeshutil.h"
#include "inout/cxopenandsavefilemanager.h"
using namespace creative_kernel;

Cx3dScene::Cx3dScene(QObject* parent)
	:QObject(parent)
{
}

Cx3dScene::~Cx3dScene()
{
	if (m_meshes.size())
	{
		m_meshes.clear();
	}
    if (m_contentXML)
    {
        delete m_contentXML;
        m_contentXML = nullptr;
    }
}


void Cx3dScene::build(Cx3dRenderEx* reader, QThread* mainThread)
{
    m_contentXML = reader->getContentXML();
    m_meshes = reader->getMeshs();
    m_machineSettings = reader->getMachineDefault();
    m_profileSettings = reader->getProfileDefault();

    for (trimesh::TriMesh* mesh : m_meshes)
    {
        mesh->need_bbox();
    }
    if (m_contentXML->sliceType == "FDM")
    {
        m_fdmSup = reader->getFDMSup();
        for (creative_kernel::FDMSupportGroup* fdmSup : m_fdmSup)
        {
            fdmSup->move2MainThread(mainThread);
        }
    }
}

void Cx3dScene::setScene()
{
    setMachineProfile();
    setMeshs();
    creative_kernel::checkModelRange();
    creative_kernel::requestVisUpdate(true);
}

void Cx3dScene::setMachineProfile()
{
	us::ConfigurationManager::instance().setPresetMachineType(m_contentXML->machine);

    QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
    us::ConfigurationManager::instance().setProfilePath(strPath);
    us::ConfigurationManager::instance().settingsFromMachineType(m_contentXML->machine);

	QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();

	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("topbar");
    if (pTopBar)
    {
        QObject* pSelectMachine = pTopBar->findChild<QObject*>("selectMachine");
        if (pSelectMachine)
        {
            QObject* pCombox = pSelectMachine->findChild<QObject*>("machineCommbox");
			if (pCombox)
			{
				QQmlProperty::write(pCombox, "model", QVariant::fromValue(machines));
				int currentIndex = machines.indexOf(m_contentXML->machine);
				if (currentIndex < 0) currentIndex = 0;

				QQmlProperty::write(pCombox, "currentIndex", QVariant::fromValue(currentIndex));
				setMachineSettings(m_contentXML->machine);
			}

        }
    }
}

void Cx3dScene::OldsetMachineProfile()
{
	us::ConfigurationManager::instance().setPresetMachineType(m_contentXML->machine);
	QStringList machines = us::ConfigurationManager::instance().getPresetMachineTypes();
	QObject* rightobj = AbstractKernelUI::getSelf()->getUI("rightPanel");   //  getKernelUI()->rightPanel();
	if (rightobj)
	{
		QObject* sliceobj = rightobj->findChild<QObject*>("sliceUiobj");
		QObject* pRightPanel = sliceobj->findChild<QObject*>("RightInfoPanel");
		if (pRightPanel)
		{
			QObject* pSelectMachine = pRightPanel->findChild<QObject*>("selectMachine");
			if (pSelectMachine)
			{
				QObject* pCombox = pSelectMachine->findChild<QObject*>("machineCommbox");
				if (pCombox)
				{
					QQmlProperty::write(pCombox, "model", QVariant::fromValue(machines));
					int currentIndex = machines.indexOf(m_contentXML->machine);
					if (currentIndex < 0)
					{
						currentIndex = 0;
					}
					else
					{
						SlicerManager::instance().setCurrentMachine(m_contentXML->machine);
					}
					QQmlProperty::write(pCombox, "currentIndex", QVariant::fromValue(currentIndex));
					setMachineSettings(m_contentXML->machine.replace("_CX3D", ""));
				}
			}
		}
	}


    //
}

void Cx3dScene::setMeshs()
{
    if (m_meshes.size() == m_contentXML->meshPathName.size()
        && m_meshes.size() == m_contentXML->meshMatrix.size())
    {
        for (size_t i = 0; i < m_meshes.size(); i++)
        {
            creative_kernel::ModelN* newModel = new creative_kernel::ModelN();
            TriMeshPtr ptr(m_meshes.at(i));

			mmesh::dumplicateMesh(ptr.get());
			ConvexHullCalculator::calculate(ptr.get(), nullptr);

            creative_kernel::cache(ptr);
            newModel->setTrimesh(ptr);
            newModel->setObjectName(m_contentXML->meshPathName.at(i));
			//newModel->needDetectError();

            addModel(newModel, true);

            newModel->SetInitPosition(m_contentXML->meshMatrix.at(i).Position);//for reset to zero
            newModel->setLocalPosition(m_contentXML->meshMatrix.at(i).Position,true);
            newModel->setLocalScale(m_contentXML->meshMatrix.at(i).Scale, true);
            newModel->setLocalQuaternion(m_contentXML->meshMatrix.at(i).Rotate, true);
            //setSupport(m_contentXML->meshMatrix.at(i).supName, newModel);
            if (m_fdmSup.size())
            {
                newModel->setFDMSup(m_fdmSup.at(i));
            }

            newModel->updateMatrix();
        }
    }
	QList<ModelN*> all_models = modelns();
	if (all_models.size() > 0)
	{
		//selectOne(all_models.back());
		//最新加载的模型，用于默认保存时的名称
		CXFILE.setLastOpenFileName(all_models.back()->objectName());
	}
	//requestVisUpdate(true);
}

void Cx3dScene::setSupport(QString filePathName, creative_kernel::ModelN* model)
{
    QFile file(filePathName);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
    }
    file.close();
}


void Cx3dScene::setMachineSettings(QString& machineName)
{
	QString machinePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines";
	QString strMachinePath = machinePath + "/" + m_contentXML->machine + ".default";
	QDir createfile;
	bool exist = createfile.exists(strMachinePath);
	if (exist)
	{
		createfile.remove(strMachinePath);
	}
	CryptFileDevice* m_pFile = new CryptFileDevice();
	m_pFile->setFileName(strMachinePath);
	if (m_pFile->open(QIODevice::WriteOnly))
	{
		m_pFile->write(m_machineSettings);
		m_pFile->close();
	}

	QString profilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Profiles";
	QString strProfilePath = profilePath + "/" + m_contentXML->machine + "/" + m_contentXML->profile;
	exist = createfile.exists(strProfilePath);
	if (exist)
	{
		createfile.remove(strProfilePath);
	}
	m_pFile->setFileName(strProfilePath);
	if (m_pFile->open(QIODevice::WriteOnly))
	{
		m_pFile->write(m_profileSettings);
		m_pFile->close();
	}


	QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Machines/";
	us::ConfigurationManager::instance().setProfilePath(strPath);
	USettings* settings = us::ConfigurationManager::instance().settingsFromMachineType(machineName);
	QHash<QString, us::SettingItemDef*> hashItemDef = us::SettingDef::instance().getHashItemDef();

	for (const auto val : hashItemDef)
	{
		val->showValueStr = "";
	}

	for (const auto val : settings->settings())
	{
		hashItemDef[val->key()]->showValueStr = val->value().toString();
	}

    SlicerManager::instance().setCurrentMachine(machineName);
    us::GlobalSetting* globalsetting = SlicerManager::instance().globalsettings();
    int machine_width = globalsetting->settings().value("machine_width")->value().toInt();
    int machine_height = globalsetting->settings().value("machine_height")->value().toInt();
    int machine_depth = globalsetting->settings().value("machine_depth")->value().toInt();
    QString qstr = globalsetting->settings().value("machine_slicer_type")->value().toString();
    if (qstr == "FDM")SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::FDM);
    else SlicerManager::instance().setCurrentSlicerType(SlicerManager::SLICERTYPE::DLP);

    qtuser_3d::Box3D box(QVector3D(0, 0, 0), QVector3D(machine_width, machine_depth, machine_height));
	if (globalsetting->settings().value("machine_center_is_zero")->value().toBool())
	{
		float MAX_X = machine_width / 2.0;
		float MAX_Y = machine_depth / 2.0;
		box = qtuser_3d::Box3D(QVector3D(-MAX_X, -MAX_Y, 0), QVector3D(MAX_X, MAX_Y, machine_height));
	}
	
    creative_kernel::setBaseBoundingBox(box);
    creative_kernel::setModelEffectBox(box.min, box.max);
    onMachineSelectChange();

	int index = 1;
	if (m_contentXML->profile.contains("high"))
	{
		index = 0;
	}
	else if (m_contentXML->profile.contains("low"))
	{
		index = 2;
	}
	
	//ID1012357 刷新界面上的参数配置的选择。。与保存时的统一
	QObject* pTopBar = AbstractKernelUI::getSelf()->getUI("rightpanel");
	//init profile index
//	ProfileManager::instance().setProfileIndex(index);
//	QObject* pRightPanel = pTopBar->findChild<QObject*>("RightInfoPanel");
//	if (pRightPanel)
//	{
//		QObject* pProfileList = pRightPanel->findChild<QObject*>("selectProfile");
//		if (pProfileList)
//		{
//			QQmlProperty::write(pProfileList, "currentItemindex", QVariant::fromValue(index));
//			QMetaObject::invokeMethod(pRightPanel, "updateProfileModel");
//		}
//	}
}





