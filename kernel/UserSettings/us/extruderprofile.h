#ifndef EXTRUDERPROFILE_H
#define EXTRUDERPROFILE_H
#include <QtCore/QObject>
#include <QtCore/QHash>
#include "usersettingsexport.h"
#include "usettings.h"
#include <QFile>

//#include "utils/cryptfiledevice.h"
using namespace us;
class CryptFileDevice;
class USER_SETTINGS_API ExtruderProfile: public QObject
{

public:
	ExtruderProfile(QObject* parent = nullptr);
	virtual ~ExtruderProfile();
    static ExtruderProfile * instance();
//protected:
//	QStringList getAllExtruderName();
	QString getCurrentMachine();
	void initFile(int nExtruder = 2);
    void addExtruder(QString newName, QString oldName);
//	QString getExtruderType(const QString path);

    void readDataFromFile(int nExtruder = 0);
//    void writeDataToFile(QString key, QString value, int nExtruder =0);
	bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist=false/*true*/);
	QString getLocalPath();
	QString mkMutiDir(const QString path);
    QString getValue(QString key, int nExtruder);
	QHash<QString, QString> getData();
//	void resetType();
	bool delFile(const QString& path);
//	void setCurrentType(QString type);
//	QString getCurrentType();


	bool deletefile();
    void getALLExtruderfile(std::map<QString,QString>& value,int nExtruder = 0);
    void setKeyValue(QString key,QString value,int nExtruder =0);
    void saveExtruderData(int nExtruder);
private:
	QHash<QString, QString> m_data;

    QHash<QString, QString> m_tmpdata0;
    QHash<QString, QString> m_tmpdata1;
//	QString m_currentType;
    CryptFileDevice* m_pFile;
	QString m_configRoot;
   // QFile *m_pFile;
};
#endif // EXTRUDERPROFILE_H
