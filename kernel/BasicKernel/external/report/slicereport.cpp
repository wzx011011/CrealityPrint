#include "slicereport.h"
#include "us/slicermanager.h"
#include "us/profilemanager.h"
#include <QDebug>
#include "us/configurationmanager.h"
#include "kernel/translator.h"

void CXSLiceReport::retrieveData()
{
	
	m_settings = SlicerManager::instance().globalsettings();
	if (!m_settings)
	{
		qDebug() << "retrieve Data failed ,globalsetting is nullptr!";
		return;
	}
	machine_name = m_settings->value("machine_name", "default");
	machine_height = m_settings->value("machine_height", "0");
	machine_width = m_settings->value("machine_width", "0");
	machine_depth = m_settings->value("machine_depth", "0");
	resolution_x = m_settings->value("resolution_x", "0");
	resolution_y = m_settings->value("resolution_y", "0");

	m_parameterTempl = ProfileManager::instance().getcurrentProfileName();

	exposure_time = m_settings->value("exposure_time", "6");
	first_exposure_time = m_settings->value("first_exposure_time", "50");
	layer_height = m_settings->value("layer_height", "0.05");
	layer_height = QString::number(layer_height.toFloat(), 'f', 2);
	lifting_speed = m_settings->value("lifting_speed", "65");

	lifting_distance = m_settings->value("lifting_distance", "5");
	first_lift_distance= m_settings->value("first_lift_distance", "5");
	first_lift_speed = m_settings->value("first_lift_speed", "65");
	retract_speed = m_settings->value("retract_speed", "150");
	light_off_delay = m_settings->value("light_off_delay", "0");
	first_layer_count = m_settings->value("first_layer_count", "5");
	
	//////resin 
    resin_type = m_settings->value("material_type","normal");//m_settings->value("resin_type", "normal");
	resin_density = m_settings->value("resin_density", "1.1");
	resin_price = m_settings->value("resin_price", "0.3");

    ///calc print time
    if (ProfileManager::instance().getMachineType(SlicerManager::instance().getCurrentMachine()) == "DLP")
    {
        //calc dlp print time
        calcValue();
    }
    else
    {
        /////need density to change
		QSettings setting;
		setting.beginGroup("profile_setting");
		float unitPrice = setting.value("material_cost", "0.3").toFloat();
		setting.endGroup();

		int nTemp = (m_materialLength.toFloat() + 0.005) * 100;
		float materialLength = nTemp / 100.0;
		m_materialMoney = QString::number(materialLength * unitPrice, 'f', 2);
        m_materialWeight = QString::number((m_materialLength.toDouble() * 5.88), 'f', 2);
        m_printTime = QString("%1h%2m%3s").arg((int)(m_printTime.toInt()) / 3600).arg((int)(m_printTime.toInt()) / 60 % 60).arg((int)(m_printTime.toInt()) % 60);
    }
    //lisugui 2020-11-13 Currency type $ =  /7 default is rmb
    QString strMoneyType = Translator::getInstance()->getMoneyType();
    m_noChanged_materialMoney = m_materialMoney;
    if(strMoneyType == "$")
    {
        m_materialMoney = QString::number((m_noChanged_materialMoney.toDouble() / 7),'f',4) ;
    }

    disconnect(Translator::getInstance(),SIGNAL(moneyTypeChanged()),this,SLOT(slotMoneyTypechanged()));
    connect(Translator::getInstance(),SIGNAL(moneyTypeChanged()),this,SLOT(slotMoneyTypechanged()));
}

void CXSLiceReport::set_exposureTime(QString pexposureTime)
{
	exposure_time = pexposureTime; 
}

void CXSLiceReport::set_risingSpeed(QString prisingSpeed)
{ 
	lifting_speed = prisingSpeed; 
}

void CXSLiceReport::set_risingDistance(QString prisingDist)
{
	lifting_distance = prisingDist; 
}

void CXSLiceReport::set_retract_speed(QString gSpeed)
{
	retract_speed = gSpeed; 
}

void CXSLiceReport::set_firstExposureTime(QString pfirstExposureTime)
{
	first_exposure_time = pfirstExposureTime; 
}

void CXSLiceReport::calcValue()
{
	double printTime =
		first_layer_count.toInt() * (first_exposure_time.toInt() + light_off_delay.toInt() + first_lift_distance.toDouble() / first_lift_speed.toDouble() + first_lift_distance.toDouble() / retract_speed.toDouble())
		+ (m_layerCount.toInt() - first_layer_count.toInt()) * (exposure_time.toInt() + light_off_delay.toInt() + lifting_distance.toDouble() / lifting_speed.toDouble() + lifting_distance.toDouble() / retract_speed.toDouble());

	m_printTime = QString("%1h%2m%3s").arg((int)printTime / 3600).arg((int)printTime / 60 % 60).arg((int)printTime % 60);
	m_materialWeight = QString::number(m_volume.toDouble() * resin_density.toDouble());
	m_materialMoney = QString::number(m_volume.toDouble() * resin_price.toDouble());
}
QString CXSLiceReport::exposureTime()
{
    return exposure_time;
}

QString CXSLiceReport:: getMoneyType()
{
    QString strMoneyType = Translator::getInstance()->getMoneyType();
    qDebug() << "strMoneyType =" <<strMoneyType;
    return strMoneyType;
}
void CXSLiceReport:: slotMoneyTypechanged()
{
    qDebug()<<"slotMoneyTypechanged !!";
    QString strMoneyType = Translator::getInstance()->getMoneyType();
    if(strMoneyType == "$")
    {
        m_materialMoney = QString::number((m_noChanged_materialMoney.toDouble() / 7),'f',4) ;
    }
    else if(strMoneyType == "RMB")
    {
        m_materialMoney = m_noChanged_materialMoney;
    }
    emit reportMoneyTypeChanged();

}
