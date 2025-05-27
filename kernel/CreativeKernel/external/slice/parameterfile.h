#ifndef _NULLSPACE_PARAMETERFILE_15916733167626_H
#define _NULLSPACE_PARAMETERFILE_15916733167626_H
#include <QtCore/QObject>
//#include "profilelistmodel.h"
#include <QDir>
#include "def/cryptfiledevice.h"
#include "us/profile.h"

class ParameterFile: public QObject
{
	enum ProfileType
	{
		PROFILE,
		MATERIAL,
		MACHINE
	};

	Q_OBJECT
public:
    ParameterFile(const QString& configRoot="");
    virtual ~ParameterFile();

    void updateProfileByMachineName(QString name);
	void updateProfileLayerHeight(QString name);

	QString getCoachPath(int = ProfileType::PROFILE);
	void copyBaseQuality(QString name);
	void copyBaseMachine(QString name, bool isCover = false);
	QString getMachineType(const QString path);

	QStringList getAllfile(QString name, int type = ProfileType::PROFILE);
	//QString getcurrentProfileName();
	void addNewProfile(QString newProfile, QString printer, QString material, QString oldProfile);
	//int setNewProfileIndex(QString name);
	void saveProfile();
	bool delFile(const QString& path);
	void saveMachineProfile(const QString& machineName = QString());
	void addNewMachine(QString newPrinter, QString oldPrinter);
	bool delMachinesFile();

	void initMachineSetting(QStringList& list);
	QMap<QString, QString> getSettingFile();
protected:
    QList<Profile*> getDefaultFDMProfiles(QString name);
    QFileInfoList getFileName(QString name, int type = ProfileType::PROFILE);
	QString mkMutiDir(const QString path);//create folder
	bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist = true);
	QStringList getMaterialType(QString machineName);
	QString readLine(QString path);
	bool delDir(const QString& path);
private:
    //ProfileListModel* m_pCurrentProfileModel = nullptr;
    CryptFileDevice* m_pFile;
	QString m_config_root;

};
#endif // _NULLSPACE_PARAMETERFILE_15916733167626_H
