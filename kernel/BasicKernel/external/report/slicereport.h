#ifndef _SLICEREPORT_H
#define _SLICEREPORT_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QtCore/QAbstractListModel>
#include "basickernelexport.h"
namespace us
{
	class GlobalSetting;
}

struct ReportInfo
{
	double volume;
	int layers;
	int printTime;
	float materialLenth;
};

class BASIC_KERNEL_API CXSLiceReport : public QObject
{
	Q_OBJECT
public:
	CXSLiceReport(ReportInfo* sliceInfo, QObject* parent = nullptr) :QObject(parent)
	{
		m_layerCount = QString::number(sliceInfo->layers);
		m_volume = QString::number(sliceInfo->volume/1000,'f',2);
		m_printTime = QString::number(sliceInfo->printTime);
		m_materialLength = QString::number(sliceInfo->materialLenth);

		retrieveData();
	};
	virtual ~CXSLiceReport() {};

	Q_INVOKABLE QString machine() { return machine_name; };
	Q_INVOKABLE QString resintype() { return resin_type; };
	Q_INVOKABLE QString volume() { return m_volume; };
	Q_INVOKABLE QString material_weight() { return m_materialWeight; };
	Q_INVOKABLE QString printing_time() { return m_printTime; };
	Q_INVOKABLE QString retractSpeed() { return retract_speed; };
	Q_INVOKABLE QString material_money() { return m_materialMoney; };
	Q_INVOKABLE QString material_length() { return m_materialLength; };


	Q_INVOKABLE QString parameter_template() { return m_parameterTempl; };
	Q_INVOKABLE QString machineHeight() { return machine_height; };

	Q_INVOKABLE QString exposureTime();
	Q_INVOKABLE QString layerHeight() { return layer_height; };
	Q_INVOKABLE QString layerCount() { return m_layerCount; };
	Q_INVOKABLE QString risingSpeed() { return lifting_speed; };
	Q_INVOKABLE QString liftingDistance() { return lifting_distance; };

	Q_INVOKABLE QString firstExposureTime() { return first_exposure_time; };
	Q_INVOKABLE QString firstRisingDistance() { return first_lift_distance; };
	Q_INVOKABLE QString firstRisingSpeed() { return first_lift_speed; };

	Q_INVOKABLE QString curOpenFileType() { return m_curFileType; };


	void set_machine(QString pmachine_name) { machine_name = pmachine_name; };
	void set_resolution_x(QString presolution_x) { resolution_x = presolution_x; };
	void set_resolution_y(QString presolution_y) { resolution_y = presolution_y; };


	void set_resintype(QString presin_type ) { resin_type = presin_type; };
	void set_volume(QString pvolume) { m_volume = pvolume; };
	void set_material_weight(QString pmaterial_weight) { m_materialWeight = pmaterial_weight; };
	void set_printing_time(QString printing_time) { m_printTime = printing_time; };
	void set_material_money(QString pmaterial_money) { m_materialMoney = pmaterial_money; };
	void set_material_length(QString pmaterial_length) { m_materialLength = pmaterial_length; };
	void set_parameter_template(QString pparameter_template) { m_parameterTempl = pparameter_template; };
	void set_machineHeight(QString pmachineHeight) { machine_height = pmachineHeight; };

	void set_layerHeight(QString playerHeight) { layer_height = playerHeight; };
	void set_layerCount(QString playerCount) { m_layerCount = playerCount; };

	void set_TotalVolume(QString totalVolume) { total_volume = totalVolume; };

	Q_INVOKABLE void set_exposureTime(QString pexposureTime);
	Q_INVOKABLE void set_risingSpeed(QString prisingSpeed);
	Q_INVOKABLE void set_risingDistance(QString prisingDist);
	Q_INVOKABLE void set_retract_speed(QString gSpeed);
	Q_INVOKABLE void set_firstExposureTime(QString pfirstExposureTime);

	void calcValue();

	void set_curOpenFileType(QString filetype) { m_curFileType = filetype; };

	Q_INVOKABLE void finish(QString strCurrentFile)
	{

	};

    ///lisugui 2020-11-03
    Q_INVOKABLE QString getMoneyType();
signals:
	void reportMoneyTypeChanged();
public slots:
    void slotMoneyTypechanged();
private:
	void retrieveData();
	//////get from globalsetting 
	QString machine_name = "";
	QString machine_height = "";
	QString machine_width = "";
	QString machine_depth = "";
	QString resolution_x = "";
	QString resolution_y = "";

	/////dlp para  //´òÓ¡²ÎÊý
	QString m_parameterTempl = ""; 
	QString exposure_time = "";
	QString first_exposure_time = "";
	QString layer_height = "";
	QString lifting_speed = "";
	QString lifting_distance = "";
	QString first_lift_distance = "";
	QString  first_lift_speed = "";
	QString retract_speed = "";
	QString light_off_delay = "";
	QString first_layer_count ="";
    //resin 
	QString resin_type = "";
	QString resin_density = "";
	QString resin_price = "";


	////// need calc
	QString m_layerCount = "100";
	QString m_volume = "100";
	QString m_materialLength = "100";
	QString m_materialWeight = "100";
	QString m_printTime = "100";
	QString m_materialMoney = "100";

	us::GlobalSetting* m_settings = nullptr;

	QString m_curFileType = "";

    //jilu  bu bian de cai liao feiyong
    QString m_noChanged_materialMoney = "100";

	QString total_volume;
};

#endif
