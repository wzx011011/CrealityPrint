#ifndef _MANAGEMATERIALFILE_1594085332399_H
#define _MANAGEMATERIALFILE_1594085332399_H
#include <QtCore/QObject>
#include <QtCore/QHash>
#include "def/cryptfiledevice.h"

class MangeMaterialFile: public QObject
{

public:
	MangeMaterialFile(QObject* parent = nullptr);
	virtual ~MangeMaterialFile();

//protected:
	QStringList getAllMaterialName();
	QString getCurrentMachine();
	void initFile();
	void addMaterial(QString newName, QString oldName);
	QString getMaterialType(const QString path);

	void readDataFromFile(QString strPath);
	void writeDataToFile(QString key,QString value,bool bAutoSupport=false);
	bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist=true);
	QString getLocalPath();
	QString mkMutiDir(const QString path);
	QString getValue(QString key);
	QHash<QString, QString> getData();
	void resetType();
	bool delFile(const QString& path);
	void setCurrentType(QString type);
	QString getCurrentType();


	bool deletefile();
private:
	QHash<QString, QString> m_data;
	QString m_currentType;
	CryptFileDevice* m_pFile;
	QString m_configRoot;
};
#endif // _MANAGEMATERIALFILE_1594085332399_H