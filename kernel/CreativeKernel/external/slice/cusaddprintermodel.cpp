#include <QTime>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "cusaddprintermodel.h"
#include <QString>
#include <QVariantMap>
#include <QDebug>
#include <QTimer>
#include "kernel/abstractkernel.h"
#include <QMetaObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include"us/configurationmanager.h"
#include "us/configurationmanager.h"
#include "us/profilemanager.h"

#pragma execution_character_set("utf-8")
static int process = 0;
using namespace creative_kernel;


CusAddPrinterModel::CusAddPrinterModel()
{
}

void CusAddPrinterModel::changePrinterDataSource(int dataType)
{
	//
	if (dataType == m_preDataSource)
	{
		return;
	}
	switch (dataType) 
	{
	case ENDER_DATA:
		updatePrinterInfoList(m_EnderInfoList);
		break;
	case CR_DATA:
		updatePrinterInfoList(m_CrInfoList);
		break;
	case SERMOON_DATA:
		updatePrinterInfoList(m_SermoonInfoList);
		break;
	case OTHERS_DATA:
		updatePrinterInfoList(m_OthersInfoList);
		break;
	case FAST_DATA:
		updatePrinterInfoList(m_PrintScreenList);
		break;
	}
	m_preDataSource = dataType;
}

void CusAddPrinterModel::addPrinterInfoListFromJson()
{
	m_EnderInfoList.clear();
	m_CrInfoList.clear();
	m_SermoonInfoList.clear();
	m_PrintScreenList.clear();
	m_OthersInfoList.clear();
	QString configRoot = us::ConfigurationManager::instance().getConfigRoot();
	//get the machine list from Json file
	QString fileName = "machineList";
#ifdef MACHINE_FILE_SUBNAME
	fileName = fileName + "_sub";
#endif // MACHINE_FILE_SUBNAME

	fileName = configRoot +"/default/" + fileName + ".json";
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "File open failed!";
	}
	else {
		qDebug() << "File open successfully!";
	}
	QJsonParseError* error = new QJsonParseError;
	QJsonDocument document = QJsonDocument::fromJson(file.readAll(), error);

	//check json file
	if (error->error != QJsonParseError::NoError)
	{
		qDebug() << "parseJson:" << error->errorString();
	}
	else {
		QString strJson(document.toJson(QJsonDocument::Compact));
		QJsonArray arr = document.array();
		for (int i = 0; i < arr.size(); i++)
		{
			QString nameType = arr.at(i).toObject().value(QString::fromLatin1("name")).toString();
			QJsonArray objArr = arr.at(i).toObject().value(QString::fromLatin1("subnodes")).toArray();
			for (int i = 0; i < objArr.size(); i++)
			{
				QString name = objArr.at(i).toObject().value(QString::fromLatin1("name")).toString();
				
				QString config = us::ConfigurationManager::instance().getConfigRoot();
				QString path = config + "/default/";
				us::ConfigurationManager::instance().setProfilePath(path);
				USettings* settings = MACHINECONF(name);

				int machine_width = 0;
				int machine_height = 0;
				int machine_depth = 0;
				if(settings->settings().value("machine_width"))
					machine_width = settings->settings().value("machine_width")->value().toInt();
				if (settings->settings().value("machine_height"))
					machine_height = settings->settings().value("machine_height")->value().toInt();
				if (settings->settings().value("machine_depth"))
					machine_depth = settings->settings().value("machine_depth")->value().toInt();
				float machine_NozzleSize = 0.0;
				if (settings->settings().find("machine_nozzle_size") != settings->settings().end())
				{
					machine_NozzleSize = settings->settings().value("machine_nozzle_size")->value().toFloat();
				}
				AddPrinterInfo info;
				info.setPrinterName(name);
				info.setMachineDepth(machine_depth);
				info.setMachineWidth(machine_width);
				info.setMachineHeight(machine_height);
				info.setNozzleSize(machine_NozzleSize);
				QString imageUrl = "qrc:/UI/photo/machineImage_" + name + ".png";
				info.setPrinterImageUrl(imageUrl);

				if (nameType == "ENDER series") {
					m_EnderInfoList.push_back(info);
				}
				else if (nameType == "CR series") {
					m_CrInfoList.push_back(info);
				}
				else if (nameType == "Sermoon series") {
					m_SermoonInfoList.push_back(info);
				}
				else if (nameType == "Others") {
					m_OthersInfoList.push_back(info);
				}
				else if (nameType == "Fast series") {
					m_PrintScreenList.push_back(info);
				}
				
			}
		}
	}

	//
	changePrinterDataSource(CurDataType::ENDER_DATA);
}



AddPrinterInfo::AddPrinterInfo()
{
}

AddPrinterInfo::~AddPrinterInfo()
{

}

int CusAddPrinterModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return m_AddPrinterInfoList.count();
}

QVariant CusAddPrinterModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_AddPrinterInfoList.count())
        return QVariant();

    const AddPrinterInfo& cwInfo = m_AddPrinterInfoList[index.row()];
	switch (role)
	{
	case PRINTERIMAGEURL_ROLE:
		return cwInfo.printerImageUrl();
		break;
	case PRINTERNAME_ROLE:
		return cwInfo.printerName();
		break;
	case MACHINEDEPTH_ROLE:
		return cwInfo.machineDepth();
		break;
	case MACHINEWIDTH_ROLE:
		return cwInfo.machineWidth();
		break;
	case MACHINEHEIGHT_ROLE:
		return cwInfo.machineHeight();
		break;
	case NOZZLESIZE_ROLE:
		return cwInfo.nozzleSize();
		break;
	default:
		break;
	}
	return QVariant();
}

QHash<int, QByteArray> CusAddPrinterModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[PRINTERIMAGEURL_ROLE] = "cusPrinterImageUrl";
	roles[PRINTERNAME_ROLE] = "cusprinterName";
	roles[MACHINEDEPTH_ROLE] = "cusMachineDepth";
	roles[MACHINEWIDTH_ROLE] = "cusMachineWidth";
	roles[MACHINEHEIGHT_ROLE] = "cusMachineheight";
	roles[NOZZLESIZE_ROLE] = "cusNozzleSize";
	return roles;
}

void CusAddPrinterModel::addModelData(AddPrinterInfo info)
{
	beginInsertRows(QModelIndex(), m_AddPrinterInfoList.size(), m_AddPrinterInfoList.size());
	m_AddPrinterInfoList.push_back(info);
	endInsertRows();
}

void CusAddPrinterModel::updatePrinterInfoList(QList<AddPrinterInfo>& list)
{
	beginResetModel();
	clearPrinterInfoList();
	for (int i = 0; i < list.size(); i++)
	{
		addModelData(list[i]);
	}
	endResetModel();
}
