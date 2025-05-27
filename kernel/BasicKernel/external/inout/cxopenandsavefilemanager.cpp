#include "cxopenandsavefilemanager.h"

#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include "us/slicermanager.h"

CXFileOpenAndSaveManager::CXFileOpenAndSaveManager(QObject* parent)
	: m_invokeObject(nullptr)
	, m_State(OpenSaveState::oss_none)
	, m_externalHandler(nullptr)
	, m_work_model("fdm")
{
}

CXFileOpenAndSaveManager::~CXFileOpenAndSaveManager() 
{
}

void CXFileOpenAndSaveManager::init(QObject* obj)
{
	m_invokeObject = obj;
}

void CXFileOpenAndSaveManager::clear()
{

}

CXFileOpenAndSaveManager& CXFileOpenAndSaveManager::instance()
{
	static CXFileOpenAndSaveManager man;
	return man;
}

QString CXFileOpenAndSaveManager::title()
{
	QString _title("Error");
	switch (m_State)
	{
	case OpenSaveState::oss_open:
	case OpenSaveState::oss_external_open:
		_title = QString("Open");
		break;
	case OpenSaveState::oss_external_save:
	case OpenSaveState::oss_save:
		_title = QString("Save");
		break;
	default:
		break;
	}
	return _title;
}

QStringList CXFileOpenAndSaveManager::nameFilters()
{
	QStringList filterList;
	switch (m_State)
	{
	case OpenSaveState::oss_open:
		filterList = generateFiltersFromHandlers(false);
		break;
	case OpenSaveState::oss_save:
		filterList = generateFiltersFromHandlers(true);
		break;
	case OpenSaveState::oss_external_open:
		filterList = generateFilters(m_externalFilterList);
		break;
	case OpenSaveState::oss_external_save:
		filterList = generateFilters(m_externalFilterList,1);
		break;
	default:
		break;
	}

	return filterList;
}

QString CXFileOpenAndSaveManager::currOpenFile()
{
	return m_lastOpenFile.left(m_lastOpenFile.lastIndexOf("."));//m_lastOpenFile.mid(0,m_lastOpenFile.size()-4);
}

QString CXFileOpenAndSaveManager::currentMachineName()
{
	return SlicerManager::instance().getCurrentMachine();
}

QStringList CXFileOpenAndSaveManager::generateFilters(const QStringList& extensions, int mode)
{
	QStringList filters;
	if (0 == mode)
	{
		QString filtersall;
		for (const QString& ext : extensions)
		{
			filtersall += QString("*.%1 ").arg(ext);
		}
		filters.push_front(filtersall);
	}
	else
	{
		for (const QString& ext : extensions)
		{
			filters.push_back(QString("*.%1 ").arg(ext));
		}
	}
	
	return filters;
}

QStringList CXFileOpenAndSaveManager::generateFiltersFromHandlers(bool saveState)
{
	QMap<QString, CXHandleBase*>& handlers = saveState ? m_saveHandlers : m_openHandlers;
	QStringList filters;
	QString allSuffix;
	for (QMap<QString, CXHandleBase*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		if (it.value()->supportOpenModel() != m_work_model)
		{
			continue;
		}
		QStringList enableFilters = it.value()->enableFilters();
		for (const QString& ext : enableFilters)
		{
			QString suffix = QString("*.%1 ").arg(ext);
			if (!filters.contains(suffix))
			{
				filters << suffix;
				allSuffix += suffix;
			}
		}
	}
	filters.push_front(allSuffix);
	return filters;
}
bool CXFileOpenAndSaveManager::cancelHandle()
{
	if (m_externalHandler)
	{
		m_externalHandler = nullptr;
		m_State = OpenSaveState::oss_none;
		return true;
	}
	return true;
}

void CXFileOpenAndSaveManager::fileOpen(const QString& url)
{
    openWithUrl(QUrl(url),m_work_model);
};

void CXFileOpenAndSaveManager::filesOpen(const QList<QUrl>& urls)
{
	QStringList fileNames;
	for (const QUrl& url : urls)
		fileNames << url.toLocalFile();

    openWithNames(fileNames,m_work_model);
};

void CXFileOpenAndSaveManager::fileSave(const QString& url)
{
	QFileInfo info(url);
	QString suffix = info.suffix();
	if (suffix == nullptr)
	{
		if (m_externalFilterList.size() > 0)
		{
			QString url2 = url + "." + m_externalFilterList[0];
			saveWithUrl(QUrl(url2));
		}
		else
			saveWithUrl(QUrl(url + ".stl"));
	}
	else
		saveWithUrl(QUrl(url));
}

void CXFileOpenAndSaveManager::open(CXHandleBase* receiver, const QStringList& filters)
{
	m_State = OpenSaveState::oss_open;
	if (receiver)
	{
		m_externalHandler = receiver;
		m_externalFilterList = filters;
		m_State = OpenSaveState::oss_external_open;
	}

	QMetaObject::invokeMethod(m_invokeObject, 
		"requestOpenFileDialog", Q_ARG(QVariant, QVariant::fromValue(this)));
}

void CXFileOpenAndSaveManager::save(CXHandleBase* receiver, const QStringList& filters)
{
	m_State = OpenSaveState::oss_save;
	if (receiver)
	{
		m_externalHandler = receiver;
		m_externalFilterList = filters;
		m_State = OpenSaveState::oss_external_save;
	}

	QMetaObject::invokeMethod(m_invokeObject, 
		"requestSaveFileDialog", Q_ARG(QVariant, QVariant::fromValue(this)));
}

void CXFileOpenAndSaveManager::openWithParams(const QStringList& params, QString handleModel)
{
	if (params.size() > 1)
	{
		qDebug() << "params length > 1. maybe need open file";
		QString needOpenName;

		for (const QString& fileName : params)
		{
			QFile file(fileName);
			QFileInfo fileInfo(fileName);
			QString suffix = fileInfo.suffix();
			suffix += handleModel;
			if (file.exists() && m_openFilterList.contains(suffix))
			{
				needOpenName = fileName;
				break;
			}
		}

		openWithName(needOpenName);
	}
}

bool CXFileOpenAndSaveManager::openWithName(const QString& fileName, QString handleModel)
{
	if (m_externalHandler)
	{
		m_externalHandler->handle(fileName);
		m_externalHandler = nullptr;
		m_State = OpenSaveState::oss_none;
		return true;
	}

	QFileInfo info(fileName);
	QString suffix = info.suffix();
	suffix = suffix.toLower();
	suffix += handleModel;
	m_lastOpenFile = info.fileName();
	m_lastOpenFilePath = fileName;
	return openWithNameSuffix(fileName, suffix);
}

void CXFileOpenAndSaveManager::openWithNames(const QStringList& fileNames, QString handleModel)
{
	if (m_externalHandler)
	{
		m_externalHandler->handle(fileNames);
		m_externalHandler = nullptr;
		m_State = OpenSaveState::oss_none;
		return;
	}

	if (fileNames.size() > 0)
	{
		QFileInfo info(fileNames.at(0));
		QString suffix = info.suffix();
		suffix = suffix.toLower();
		suffix += handleModel;
		m_lastOpenFile = info.fileName();
		m_lastOpenFilePath = info.absoluteFilePath();
		CXHandleBase* handler = findHandler(suffix, m_openHandlers);
		if (!handler)
		{
			qDebug() << "not register handler for this file type!";
			return;
		}

		if (suffix == "gcodefdm" || suffix == "cx3dfdm")
		{
			handler->handle(fileNames.at(0));
		}
		else
		{
			handler->handle(fileNames);
		}
	}
}

bool CXFileOpenAndSaveManager::openWithUrl(const QUrl& url, QString handleModel)
{
	return openWithName(url.toLocalFile(),handleModel);
}

void CXFileOpenAndSaveManager::openWithUrls(const QList<QUrl>& urls, QString handleModel)
{
	QStringList fileNames;
	for (const QUrl& url:urls)
	{
		fileNames << url.toLocalFile();
	}
	if (m_work_model == "laser")
		handleModel = "laser";
	return openWithNames(fileNames, handleModel);
}

bool CXFileOpenAndSaveManager::saveWithName(const QString& fileName)
{
	m_lastSaveFile = fileName;
	if (m_externalHandler)
	{
		m_externalHandler->handle(fileName);
		m_externalHandler = nullptr;
		m_State = OpenSaveState::oss_none;
		return true;
	}

	QFileInfo info(fileName);
	QString suffix = info.suffix();
	suffix = suffix.toLower();
	return saveWithNameSuffix(fileName, suffix);
}

bool CXFileOpenAndSaveManager::saveWithUrl(const QUrl& url)
{
	return saveWithName(url.toLocalFile());
}

bool CXFileOpenAndSaveManager::openWithNameSuffix(const QString& fileName, const QString suffix)
{
	CXHandleBase* handler = findHandler(suffix, m_openHandlers);
	if (!handler)
	{
		qDebug() << "not register handler for this file type!";
		return false;
	}

	handler->handle(fileName);
	return true;
}

bool CXFileOpenAndSaveManager::saveWithNameSuffix(const QString& fileName, const QString suffix)
{
	CXHandleBase* handler = findHandler(suffix, m_saveHandlers);
	if (!handler)
	{
		qDebug() << "not register handler for this file type!";
		return false;
	}

	handler->handle(fileName);
	return true;
}

CXHandleBase* CXFileOpenAndSaveManager::findHandler(const QString& suffix, QMap<QString, CXHandleBase*>& handlers)
{
	QMap<QString, CXHandleBase*>::iterator it = handlers.find(suffix);
	if (it != handlers.end())
		return it.value();
	return nullptr;
}

void CXFileOpenAndSaveManager::registerOpenHandler(CXHandleBase* handler)
{
	if(handler)
		registerOpenHandler(handler->supportFilters(), handler);
}

void CXFileOpenAndSaveManager::registerOpenHandler(const QStringList& suffixes, CXHandleBase* handler)
{
	for (const QString& suffix : suffixes)
		registerOpenHandler(suffix+ handler->supportOpenModel(), handler);
}

void CXFileOpenAndSaveManager::unRegisterOpenHandler(const QStringList& suffixes, QString handleModel)
{
	for (const QString& suffix : suffixes)
		unRegisterOpenHandler(suffix+ handleModel);
}

void CXFileOpenAndSaveManager::registerOpenHandler(const QString& suffix, CXHandleBase* handler)
{
	if (registerHandler(suffix, handler, m_openHandlers))
		m_openFilterList << suffix;
}

void CXFileOpenAndSaveManager::unRegisterOpenHandler(const QString& suffix)
{
	unRegisterHandler(suffix, m_openHandlers);
	m_openFilterList.removeOne(suffix);
}

void CXFileOpenAndSaveManager::registerSaveHandler(CXHandleBase* handler)
{
	if (handler)
		registerSaveHandler(handler->supportFilters(), handler);
}

void CXFileOpenAndSaveManager::registerSaveHandler(const QStringList& suffixes, CXHandleBase* handler)
{
	for (const QString& suffix : suffixes)
		registerSaveHandler(suffix, handler);
}

void CXFileOpenAndSaveManager::unRegisterSaveHandler(const QStringList& suffixes)
{
	for (const QString& suffix : suffixes)
		unRegisterSaveHandler(suffix);
}

void CXFileOpenAndSaveManager::registerSaveHandler(const QString& suffix, CXHandleBase* handler)
{
	if (registerHandler(suffix, handler, m_saveHandlers))
		m_saveFilterList << suffix;
}

void CXFileOpenAndSaveManager::unRegisterSaveHandler(const QString& suffix)
{
	unRegisterHandler(suffix, m_saveHandlers);
	m_saveFilterList.removeOne(suffix);
}

bool CXFileOpenAndSaveManager::registerHandler(const QString& suffix, CXHandleBase* handler, QMap<QString, CXHandleBase*>& handlers)
{
	if (!handler || handlers.contains(suffix))
	{
		qDebug() << "handler exist for " << suffix;
		return false;
	}

	handlers.insert(suffix, handler);
	return true;
}

void CXFileOpenAndSaveManager::unRegisterHandler(const QString& suffix, QMap<QString, CXHandleBase*>& handlers)
{
	handlers.remove(suffix);
}

QString CXFileOpenAndSaveManager::lastOpenFileName()
{
	return m_lastOpenFile;
}

QString CXFileOpenAndSaveManager::lastOpenFilePath()
{
	return m_lastOpenFilePath;
}

QString CXFileOpenAndSaveManager::lastSaveFileName()
{
	return m_lastSaveFile;
}

void CXFileOpenAndSaveManager::setLastSaveFileName(QString filePath)
{
	m_lastSaveFile = filePath;
}
void CXFileOpenAndSaveManager::setLastOpenFileName(QString filePath)
{
	m_lastOpenFile = filePath;
}
void CXFileOpenAndSaveManager::openDesktopFolder()
{
    QFileInfo info(m_lastSaveFile);
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
}

void CXFileOpenAndSaveManager::openLastSaveFolder()
{
	QFileInfo fileInfo(m_lastSaveFile);
	openFolder(fileInfo.absolutePath());
}

void CXFileOpenAndSaveManager::openFolder(const QString& folder)
{
	//QProcess process;
	//QString strPath = "explorer.exe /select," + folder;
	//qDebug() << "strFile =" << folder;
	//process.startDetached(QStringLiteral("explorer.exe /select,") + strPath);

	QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
}
