#ifndef _CXFileOpenAndSaveManager_H
#define _CXFileOpenAndSaveManager_H
#include "basickernelexport.h"
#include "inout/cxhandlerbase.h"
#include "inout/cxioccontainer.h"

#include <QtCore/QHash>
#include <QtCore/QMap>

typedef void (*requestFileNameFunCallBack)(QString&,void*);
typedef void (*requestFilesNameFunCallBack)(QStringList&, void*);

enum class OpenSaveState
{
	oss_none,
	oss_open,
	oss_save,
	oss_external_open,
	oss_external_save
};

class BASIC_KERNEL_API CXFileOpenAndSaveManager : public QObject
{
	Q_OBJECT
public:
	CXFileOpenAndSaveManager(QObject* parent = nullptr);
	virtual ~CXFileOpenAndSaveManager();

	static CXFileOpenAndSaveManager& instance();
	void init(QObject* obj);
	void clear();

	Q_INVOKABLE QString title();
	Q_INVOKABLE QStringList nameFilters();
	Q_INVOKABLE QString currOpenFile();
	
	Q_INVOKABLE void setWorkModel(const QString& model) { m_work_model = model; }

	Q_INVOKABLE void fileOpen(const QString& url);
	Q_INVOKABLE void fileSave(const QString& url);
	Q_INVOKABLE void filesOpen(const QList<QUrl>& urls);
	//lisugui 2021-1-12  handle cancel function
	Q_INVOKABLE bool cancelHandle();

	Q_INVOKABLE QString currentMachineName();

public:
	void open(CXHandleBase* receiver = nullptr, const QStringList& filters = QStringList());
	void save(CXHandleBase* receiver = nullptr, const QStringList& filters = QStringList());

	void openWithParams(const QStringList& fileNames, QString handleModel = "fdm");
	bool openWithName(const QString& fileName, QString handleModel = "fdm");
	void openWithNames(const QStringList& fileNames, QString handleModel = "fdm");
	bool openWithUrl(const QUrl& url, QString handleModel = "fdm");
	void openWithUrls(const QList<QUrl>& urls, QString handleModel = "fdm");

	bool saveWithName(const QString& fileName);
	bool saveWithUrl(const QUrl& url);

	void registerOpenHandler(CXHandleBase* handler);
	void registerOpenHandler(const QStringList& suffixes, CXHandleBase* handler);
	void unRegisterOpenHandler(const QStringList& suffixes, QString handleModel="fdm");
	
	void registerOpenHandler(const QString& suffix, CXHandleBase* handler);
	void unRegisterOpenHandler(const QString& suffix);

	void registerSaveHandler(CXHandleBase* handler);
	void registerSaveHandler(const QStringList& suffixes, CXHandleBase* handler);
	void unRegisterSaveHandler(const QStringList& suffixes);
	void registerSaveHandler(const QString& suffix, CXHandleBase* handler);
	void unRegisterSaveHandler(const QString& suffix);

	QString lastOpenFileName();
	QString lastOpenFilePath();
	QString lastSaveFileName();
	void setLastSaveFileName(QString filePath);
	void setLastOpenFileName(QString filePath);

	void openDesktopFolder();
	void openLastSaveFolder();
	void openFolder(const QString& folder);
protected:
	bool openWithNameSuffix(const QString& fileName, const QString suffix);
	bool saveWithNameSuffix(const QString& fileName, const QString suffix);

	bool registerHandler(const QString& suffix, CXHandleBase* handler, QMap<QString, CXHandleBase*>& handlers);
	void unRegisterHandler(const QString& suffix, QMap<QString, CXHandleBase*>& handlers);
	CXHandleBase* findHandler(const QString& suffix, QMap<QString, CXHandleBase*>& handlers);

	QStringList generateFilters(const QStringList& extensions, int mode = 0);//by TCJ
	QStringList generateFiltersFromHandlers(bool saveState);
protected:
	QObject* m_invokeObject;
	OpenSaveState m_State;

	QMap<QString, CXHandleBase*> m_openHandlers;
	QStringList m_openFilterList;

	QMap<QString, CXHandleBase*> m_saveHandlers;
	QStringList m_saveFilterList;

	QStringList m_externalFilterList;
	CXHandleBase* m_externalHandler;

	QString m_lastSaveFile;
	QString m_lastOpenFile;
	QString m_lastOpenFilePath;

	QString m_work_model;
};

#endif // !_CXFileOpenAndSaveManager_H
