#include "unittesttool.h"
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QSvgGenerator>
#include <QtSvg/QSvgWidget>
#include <QSettings>
#include <QStandardPaths>
#include <QtCore/qprocess.h>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtCore/qmath.h>
#include "unittool.h"
//#pragma execution_character_set("utf-8")

QVariant BaseSettings::getSettingValue(const QString& key, const QVariant& defaultValue)
{
	QSettings setting;
	setting.beginGroup("too_baseline_config");
	QVariant val = setting.value(key, defaultValue);
	setting.endGroup();

	return val;
}

void BaseSettings::setSettingValue(const QString& key, const QVariant& value)
{
	QSettings setting;
	setting.beginGroup("too_baseline_config");
	setting.setValue(key, value);
	setting.endGroup();
}
QString JsonToString(const QJsonObject object, const QString& key)
{
	QJsonValue val = object.value(key);
	if (!val.isNull() && val.isString())
	{
		return val.toString();
	}
	return QString();
}
int JsonToInt(const QJsonObject object, const QString& key)
{
	QJsonValue val = object.value(key);
	if (val.isDouble() && qFloor(val.toDouble()) == val.toDouble()) {
		// 是整数
		return val.toInt();
	}
	return 0;
	
}
bool readErrorFiles(const QString dirPath, QString& resultMessage)
{
	// 获取所有文件和目录
	QDir dir(dirPath);
	if (!dir.exists())return false;
	QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	for (auto fileInfo : list)
	{
		qDebug() << fileInfo.fileName();
		if (fileInfo.isFile()) {
			qDebug() << "Found file:" << fileInfo.absoluteFilePath();
			//cxkernel::openFileWithName(fileInfo.absoluteFilePath());
			if (fileInfo.suffix() == "errtxt")
			{
				resultMessage += fileInfo.baseName().left(fileInfo.baseName().lastIndexOf("_compare")) + " file error :\n";
				loadText(fileInfo.absoluteFilePath(),resultMessage);
			}
			else
			{
				continue;
			}
		}
	}
	return true;
}

void loadText(const QString& fileName,QString &message)
{
	QFile qFile(fileName);
	if (!qFile.open(QIODevice::ReadOnly))
	{
		qDebug() << QString("loadSliceProfile [%1] failed.").arg(fileName);
		return;
	}
	while (qFile.atEnd() == false) {
		QString line = qFile.readLine().simplified();
		//if (line.isEmpty()) {
		//	continue;
		//}
		if (line.contains("error:"))
		{
			message	+= line + "\n";
		}
		else
		{
			continue;
		}
		//QStringList words = line.split(":");
		//if (words.size() > 1) {

		//}
	}
	qFile.close();
}

void loadJson(const QString& fileName, QList<QString>& listLine)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Cannot open file for reading\ ";
		return ;
	}

	QByteArray data = file.readAll();
	file.close();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
	if (jsonDoc.isNull() || !jsonDoc.isObject()) {
		qDebug() << "Invalid JSON document";
		return;
	}

	// 检查是否是对象
	if (jsonDoc.isObject()) {
		QJsonObject jsonObj = jsonDoc.object();
		// 访问数据
		QJsonValue sValue2 = jsonObj.value("ResultList");
		if (sValue2.isArray())
		{
			QJsonArray arr = sValue2.toArray();
			for (auto arrVal: arr)
			{
				QString message = "";
				QJsonObject resultObject =  arrVal.toObject();
				QString path = JsonToString(resultObject, "path"); //resultObject.value("path").toString();
				QString startTime = JsonToString(resultObject, "startTime");//resultObject.value("startTime").toString();
				QString endTime = JsonToString(resultObject, "endTime");  //resultObject.value("endTime").toString();
				QString resultState = JsonToString(resultObject, "result");  //resultObject.value("result").toString();
				QString errorReason = JsonToString(resultObject, "reason");  //resultObject.value("reason").toString();
				message += resultState + ":" + path + "\n";
				message += "startTime: " + startTime + "; endTime: "+ endTime + "\n";
				if (!errorReason.isEmpty())
				{
					message += errorReason + "\n";
				}
				listLine.push_back(message);
			}
		}
		QJsonValue sValue = jsonObj.value("Statistics");
		if (sValue.isObject())
		{
			QString message = "";
			QJsonObject sObject = sValue.toObject();
			int ncount = JsonToInt(sObject, "total");
			int errorcount = JsonToInt(sObject, "errorcount");
			int passcount = JsonToInt(sObject, "passcount"); 
			message += "total: " + QString::number(ncount) + "    passcount: " + QString::number(passcount) + "    errorcount: " + QString::number(errorcount);
			listLine.push_back(message);
		}
	}
	else {
		qWarning() << "JSON 解析错误!";
	}
}

QList<QFileInfo> entryFileInfoList(const QString& path,const QString&suffix) {
	QList<QFileInfo> findFilesInfo;

	QFileInfo fileInfo(path);
	//单个文件
	if (fileInfo.exists() && fileInfo.isFile()) {
		findFilesInfo.append(fileInfo);
		return findFilesInfo;
	}

	QDir dir(path);

	// 获取所有文件和目录
	QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);

		// 如果是文件，检查扩展名
		qDebug() << fileInfo.fileName();
		if (fileInfo.isFile() && fileInfo.suffix().toLower() == suffix) {
			qDebug() << "Found file:" << fileInfo.absoluteFilePath();
			findFilesInfo.append(fileInfo);
		}

		// 如果是目录，递归调用
		if (fileInfo.isDir()) {
			findFilesInfo += entryFileInfoList(fileInfo.absoluteFilePath(), suffix);
		}
	}
	return findFilesInfo;
}
QStringList loadErrorList(const QString& fileName)
{
	QStringList list;
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Cannot open file for reading\ ";
		return list;
	}

	QByteArray data = file.readAll();
	file.close();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
	if (jsonDoc.isNull() || !jsonDoc.isArray()) {
		qDebug() << "Invalid JSON isArray document";
		return list;
	}
	if (jsonDoc.isArray())
	{
		QJsonArray arr = jsonDoc.array();
		for (auto array: arr)
		{
			qDebug() << array.toString();
			list << array.toString();
		}
	}
	
	return list;
}

UnitTestTool::UnitTestTool(QObject* parent) : QObject(parent)
, m_qmlEngine(nullptr)
{
	m_simpleWay = false;
	m_translator = new QTranslator(this);
	initDirConfig();
	
}

UnitTestTool::~UnitTestTool() {

}
void UnitTestTool::initTranslator()
{
	QString language_file_name = "en.ts";
	m_translator->load(QStringLiteral(":/language/%1").arg(language_file_name));
	QCoreApplication::installTranslator(m_translator);
	int* a = new int(10);

}
void UnitTestTool::initDirConfig()
{
	const QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	//源码路径
	const QString sourcePath = CUR_SOURCE_ROOT + QStringLiteral("/UnitTest");
	//当前可执行程序路径
	const QString curApplicationPath =  QCoreApplication::applicationDirPath() + QStringLiteral("/UnitTest");
	//1.基线默认目录
	bool baselineRet = QFileInfo::exists(QStringLiteral("%1/BaseLine/").arg(sourcePath));
	if (baselineRet)
	{
		m_slicerBLPath = QStringLiteral("%1/BaseLine/").arg(sourcePath);
	}
	else
	{
		baselineRet = QFileInfo::exists(QStringLiteral("%1/BaseLine/").arg(curApplicationPath));
		if (baselineRet)
		{
			m_slicerBLPath = QStringLiteral("%1/BaseLine/").arg(curApplicationPath);
		}
		else
		{
			m_slicerBLPath = BaseSettings::getSettingValue("tool_slicer_baseline_path", QString("%1/").arg(defaultPath)).toString();
		}
	}
	//脚本文件目录
	bool marcoRet = QFileInfo::exists(QStringLiteral("%1/Marco/").arg(sourcePath));
	if (marcoRet)
	{
		m_scpPath = QStringLiteral("%1/Marco/").arg(sourcePath);
	}
	else
	{
		marcoRet = QFileInfo::exists(QStringLiteral("%1/Marco/").arg(curApplicationPath));
		if (marcoRet)
		{
			m_scpPath = QStringLiteral("%1/Marco/").arg(curApplicationPath);
		}
		else
		{
			m_scpPath = BaseSettings::getSettingValue("tool_scp_path", QString("%1/").arg(defaultPath)).toString();
		}
	}
	//3mf 路径
	bool _3mfRet = QFileInfo::exists(QStringLiteral("%1/Source/3MF/").arg(sourcePath));
	if (_3mfRet)
	{
		m_3mfPath = QStringLiteral("%1/Source/3MF/").arg(sourcePath);
	}
	else
	{
		_3mfRet = QFileInfo::exists(QStringLiteral("%1/Source/3MF/").arg(curApplicationPath));
		if (_3mfRet)
		{
			m_3mfPath = QStringLiteral("%1/Source/3MF/").arg(curApplicationPath);
		}
		else
		{
			m_3mfPath = BaseSettings::getSettingValue("tool_3mf_path", QString("%1/").arg(defaultPath)).toString();
		}
	}
	//result 路径
	bool _resultPathRet = QFileInfo::exists(QStringLiteral("%1/Result/").arg(sourcePath));
	if (_resultPathRet)
	{
		m_slicerComparePath = QStringLiteral("%1/Result/").arg(sourcePath);
	}
	else
	{
		_resultPathRet = QFileInfo::exists(QStringLiteral("%1/Result/").arg(curApplicationPath));
		if (_resultPathRet)
		{
			m_slicerComparePath = QStringLiteral("%1/Result/").arg(curApplicationPath);
		}
		else
		{
			m_slicerComparePath = BaseSettings::getSettingValue("tool_slicer_compare_path", QString("%1/").arg(defaultPath)).toString();
		}
	}
	m_c3dExePath = QStringLiteral("%1/CrealityPrint.exe").arg(QCoreApplication::applicationDirPath());


	m_orcaAppPath = BaseSettings::getSettingValue("tool_orcaApp_path","").toString();
	m_cur3mfFilePath = BaseSettings::getSettingValue("tool_cur3mf_path", "").toString();
	auto dirFuc = [&](const QString& curDir) {
		QDir dir(curDir);
		if (!dir.exists()) {
			dir.mkpath(".");
		}
	};
	const QString curSlicerDataDir =  QCoreApplication::applicationDirPath() + QStringLiteral("/SlicerData");
	m_slicerCacheDir =  curSlicerDataDir + "/cache/";
	m_slicerGcodeDir =  curSlicerDataDir + "/gcode/";
	dirFuc(curSlicerDataDir);
	dirFuc(m_slicerCacheDir);
	dirFuc(m_slicerGcodeDir);
}
void UnitTestTool::setQmlEngine(QQmlEngine* engine)
{
	m_qmlEngine = engine;
}

void UnitTestTool::changeLanguage(int index)
{
	QString file = "";
	switch (index)
	{
	case 0:
		file = QStringLiteral(":/language/%1").arg("en.ts");
		break;
	case 1:
		file = QStringLiteral(":/language/%1").arg("zh_CN.ts");
		break;
	default:
		break;
	}

	if (m_translator->load(file))
	{
		if (m_qmlEngine)
			m_qmlEngine->retranslate();
		return;
	}
}

QString UnitTestTool::slicer3MFPath() const
{
	return m_3mfPath;
}

void UnitTestTool::set3MFPath(const QString& strPath)
{
	m_3mfPath = strPath;
	BaseSettings::setSettingValue("tool_3mf_path", strPath);
	Q_EMIT mfPathChanged();
}

QString UnitTestTool::scpFilePath()const
{
	return m_scpPath;
}
void UnitTestTool::setScpFilePath(const QString& strPath)
{
	m_scpPath = strPath;
	BaseSettings::setSettingValue("tool_scp_path", strPath);
	Q_EMIT scpPathChanged();
}

QString UnitTestTool::c3dExePath() const
{
	return m_c3dExePath;
}
void UnitTestTool::setC3DExePath(const QString& strPath)
{
	m_c3dExePath = strPath;
	emit exePathChanged();
}
bool UnitTestTool::isSimple() const
{
	return m_simpleWay;
}
void UnitTestTool::setSimple(bool val)
{
	m_simpleWay = val;
	emit simpleWayChanged();
}
QString UnitTestTool::slicerBLPath() const
{
	return m_slicerBLPath;
}
void UnitTestTool::setSlicerBLPath(const QString& strPath)
{
	m_slicerBLPath = strPath;
	BaseSettings::setSettingValue("tool_slicer_baseline_path", strPath);
	Q_EMIT slicerBLPathChanged();
}

QString UnitTestTool::compareSliceBLPath()const
{
	return m_slicerComparePath;
}
void UnitTestTool::setCompareSlicerBLPath(const QString& strPath)
{
	m_slicerComparePath = strPath;
	BaseSettings::setSettingValue("tool_slicer_compare_path", strPath);
	Q_EMIT slicerComparePathChanged();
}
QString UnitTestTool::cur3mfFile() const {
	return m_cur3mfFilePath;
}
void UnitTestTool::setCur3mfFile(const QString& strFile) {
	m_cur3mfFilePath = strFile;
	BaseSettings::setSettingValue("tool_cur3mf_path", strFile);
}

QString UnitTestTool::orcaAppPath() const {
	return m_orcaAppPath;
}
void UnitTestTool::setOrcaAppPath(const QString& strPath) {
	m_orcaAppPath = strPath;
	BaseSettings::setSettingValue("tool_orcaApp_path", strPath);
}

QString UnitTestTool::slicerCacheDir() const {
	return m_slicerCacheDir;
}
void UnitTestTool::setSlicerCacheDir(const QString& strPath) {
	m_slicerCacheDir = strPath;
}

QString UnitTestTool::gcodeDir() const
{
	return m_slicerGcodeDir;
}
void UnitTestTool::setGcodeDir(const QString& strPath) {
	m_slicerGcodeDir = strPath;
}

QString UnitTestTool::message() const
{
	return m_mess_text;
}
void UnitTestTool::clearMessageText()
{
	m_mess_text = "";
	emit messageChanged();
}
void UnitTestTool::doTest(bool exit,int type, const QString& filepath)
{
	//-bltest --3mfpath "F:\3MFPath" --blpath F:\BLTEST --cmppath "F:\COMPARETEST"
	//清空描述
	m_mess_text.clear();
	QStringList cmdList;

	cmdList << "-bltest";
	switch (type)
	{
	case 0:
		cmdList << "-generate";
		m_mess_text += "Generate result:\n";
		break;
	case 1:
		cmdList << "-compare";
		m_mess_text += "Compare result:\n";
		break;
	case 2:
		cmdList << "-update";
		m_mess_text += "Update result:\n";
		break;
	default:
		cmdList << "-compare";
		break;
	}
	cmdList << "--blpath";
	cmdList << m_slicerBLPath;
	cmdList << "--cmppath";
	cmdList << m_slicerComparePath;
	if (filepath.isEmpty())
	{
		QString cmdScpPath = m_scpPath;
		if (m_simpleWay)
		{
			cmdScpPath = m_scpPath + "/SimpleTest/";
		}
		cmdList << "--scppath";
		cmdList << cmdScpPath;
		QList<QFileInfo> fileInfos = entryFileInfoList(cmdScpPath, "scp");
		if (fileInfos.size() == 0)
		{
			m_mess_text += "脚本目录为空";
			emit messageChanged();
			return;
		}
	}
	else
	{
		cmdList << "--scplist";
		cmdList << filepath;
	}
	const auto program =  m_c3dExePath;
	QDir dir(m_slicerComparePath);
	dir.removeRecursively();
	if (!dir.exists())
	{
		dir.mkdir(m_slicerComparePath);
	}
	int ret = QProcess::execute(program, cmdList);
	if (exit)
	{
		QCoreApplication::quit();
		return;
	}
	if (ret == 0)
	{
		qDebug() << "process end";
		m_errorFiles.clear();
		m_messageLines.clear();
		QString filename = m_slicerComparePath + "/compare.result";
		loadJson(filename, m_messageLines);
		QString errorResultFileName = m_slicerComparePath + "/failedlist.json";
		m_errorFiles = loadErrorList(errorResultFileName);
		for (auto line : m_messageLines)
		{
			m_mess_text += line;
		}
		emit messageChanged();	
	}
	else if (ret == -1)
	{
		m_mess_text = "启动的程序有崩溃！";
		emit messageChanged();
	}
	else if (ret == -2)
	{
		m_mess_text = "程序未启动，请检查输入的指令是否正确！";
		emit messageChanged();
	}
}

void UnitTestTool::doTestOneByOne(const int& type)
{
	m_mess_text.clear();
	QStringList cmdList;
	cmdList << "-bltest";
	switch (type)
	{
	case 0:
		cmdList << "-generate";
		m_mess_text += "Generate result:\n";
		break;
	case 1:
		cmdList << "-compare";
		m_mess_text += "Compare result:\n";
		break;
	case 2:
		cmdList << "-update";
		m_mess_text += "Update result:\n";
		break;
	default:
		cmdList << "-compare";
		break;
	}
	cmdList << "--blpath";
	cmdList << m_slicerBLPath;
	cmdList << "--cmppath";
	cmdList << m_slicerComparePath;
	cmdList << "--scppath";
	QDir dir(m_slicerComparePath);
	dir.removeRecursively();
	if (!dir.exists())
	{
		dir.mkdir(m_slicerComparePath);
	}
	QString cmdScpPath = m_scpPath;
	if (m_simpleWay)
	{
		cmdScpPath = m_scpPath + "/SimpleTest/";
	}
	QList<QFileInfo> fileInfos = entryFileInfoList(cmdScpPath, "scp");
	const auto program = m_c3dExePath;
	for (auto fileinfo : fileInfos)
	{
		cmdList << fileinfo.absoluteFilePath();
		int ret = QProcess::execute(program, cmdList);
		if (ret == 0)
		{
			qDebug() << "process end";
			m_errorFiles.clear();
			m_messageLines.clear();
			QString filename = m_slicerComparePath + "/compare.result";
			loadJson(filename, m_messageLines);
			QString errorResultFileName = m_slicerComparePath + "/failedlist.json";
			m_errorFiles = loadErrorList(errorResultFileName);
			for (auto line : m_messageLines)
			{
				m_mess_text += line;
			}
			emit messageChanged();

			
		}
		else if (ret == -1)
		{
			m_mess_text = "启动的程序有崩溃！";
			emit messageChanged();
			break;
		}
		else if (ret == -2)
		{
			m_mess_text = "程序未启动，请检查输入的指令或者exe路径是否正确！";
			emit messageChanged();
			break;
		}
		cmdList.removeLast();
	}
	
}


void UnitTestTool::scpMapping3MF(const QString& _3mfPath, const QString& scpName)
{
	QFileInfo fileInfo(_3mfPath);
	if (!fileInfo.exists())
	{
		return;
	}
	QString scpFilePath = m_scpPath + "/" + fileInfo.baseName() + ".scp";
	if (!scpName.isEmpty())
	{
		scpFilePath = m_scpPath + "/" + scpName + ".scp";
	}

	QFile file(scpFilePath);
	QString _tmp3mfPath = _3mfPath;
	if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {  // 尝试以写模式打开文件
		QTextStream out(&file);  // 创建 QTextStream 对象
		out.setCodec("utf-8");
		out << ">import \"" << _tmp3mfPath.replace(m_3mfPath, "") << "\"" << endl;  // 写入数据
		file.close();  // 关闭文件
	}
	else {
		qWarning("Could not open file for writing");
	}
}

void UnitTestTool::generateScript()
{
	 QList<QFileInfo> fileInfos =  entryFileInfoList(m_3mfPath,"3mf");
	 for (auto fileinfo : fileInfos)
	 {
		 scpMapping3MF(fileinfo.absoluteFilePath());
	 }
	 m_mess_text += "3mf映射到scp完成";
	 emit messageChanged();
}

void UnitTestTool::slicerGcode(const QString& _sourcePath, const QString& exePath, const QString& cacheDir, const QString& gcodeDir)
{
	if (!(exePath.contains("CrealityPrint.exe") || exePath.contains("orca-slicer.exe")))
	{
		m_mess_text = "应用程序选取错误";
		emit messageChanged();
		return;
	}
	int type = -1;
	if (_sourcePath.endsWith(".3mf"))
	{
		type = 0;
	}
	else if (_sourcePath.endsWith(".cxcache"))
	{
	
		type = 1;
	}
	if (type == -1)
	{
		m_mess_text = "导入的文件不是3mf或者不是引擎参数文件";
		emit messageChanged();
		return;
	}
	QString cachePath = cacheDir;
	QString gcodePath = gcodeDir;
	if (exePath.contains("CrealityPrint.exe"))
	{
		cachePath = cachePath + "/c3d/";
		gcodePath = gcodePath + "/c3d/";
		QDir dir(cachePath);
		if (!dir.exists())
			dir.mkdir(cachePath);
		QDir dir2(gcodePath);
		if (!dir2.exists())
			dir2.mkdir(gcodePath);

	}
	QStringList cmdList;
	cmdList << "-sliceCache";
	switch (type)
	{
	case 0:
		cmdList << QString::number(0);
		cmdList << _sourcePath;
		cmdList << cachePath;
		cmdList << gcodePath;
		break;
	case 1:
		cmdList << QString::number(1);
		cmdList << _sourcePath;
		cmdList << "";
		cmdList << gcodePath;
		break;
	default:
		break;
	}
	
	int ret = QProcess::execute(exePath, cmdList);
	if (ret == 0)
	{
		qDebug() << "process end";
		m_mess_text = "程序运行结束\n";
		m_mess_text += QStringLiteral("引擎数据文件目录:%1\n").arg(cachePath);
		m_mess_text += QStringLiteral("GCode 文件目录:%1\n").arg(gcodePath);
		emit messageChanged();	
	}
	else if (ret == -1)
	{
		m_mess_text = "启动的程序有崩溃！";
		emit messageChanged();
	}
	else if (ret == -2)
	{
		m_mess_text = "程序未启动，请检查输入的指令是否正确！";
		emit messageChanged();
	}
}

void UnitTestTool::runGCodeCompare(const QString&sourcePath,int appType)
{
	if (appType == 0)
	{
		slicerGcode(sourcePath, m_c3dExePath, m_slicerCacheDir, m_slicerGcodeDir);
	}
	else
	{
		slicerGcode(sourcePath, m_orcaAppPath, m_slicerCacheDir, m_slicerGcodeDir);
	}
	
}
void UnitTestTool::swichMessageShow(bool onlyError)
{
	m_mess_text = "";
	for (auto line : m_messageLines)
	{
		if (line.contains("Pass") && onlyError)continue;
		m_mess_text += line;
	}
	emit messageChanged();
}

void UnitTestTool::failedFileShow()
{
	m_mess_text = "";
	for (auto errorFile : m_errorFiles)
	{
		m_mess_text += errorFile + "\n";
	}
	emit messageChanged();
}

void UnitTestTool::processCommandLine()
{
	QStringList params = qApp->arguments();

	qDebug() << "processCommandLine  params" << params.size();
	qDebug() << "processCommandLine  params" << params;
	if (params.size() <= 1) return;
	if (params.size() == 2)
	{
		auto cmd = params[1];
		if (cmd.toLower() == "-ut3mf")
		{
			m_simpleWay = true;
			doTest(true);
		}
		else if (cmd.toLower() == "-ut3mf_runtest")
		{
#ifdef _WIN32
			_sleep(5000);
#endif
			QCoreApplication::quit();

		}
		else if (cmd.toLower() == "-runcreality")
		{
			const auto program = m_c3dExePath;
			QStringList cmdList = { "-runtest" };
			int ret = QProcess::execute(program,cmdList);
			QCoreApplication::quit();
		}
	}
}
void UnitTestTool::vldTest() {
	m_mess_text.clear();
	QStringList cmdList;

	cmdList << "-bltest";
	cmdList << "-compare";
	cmdList << "--blpath";
	cmdList << m_slicerBLPath;
	cmdList << "--cmppath";
	cmdList << m_slicerComparePath;
	cmdList << "--scppath";
	cmdList << m_scpPath;
	#ifdef _WIN32
	#if( (defined CXX_CHECK_MEMORY_LEAKS) && (defined _DEBUG) && (_MSC_VER))
		m_mess_text = "VLD正在检测中，请耐心等候！";
		emit messageChanged();
	#endif
#endif
		const auto program = m_c3dExePath; // "D:/Company_Program/c3d_V_L_D/win32-build/bin/Debug/vld_exe_test.exe";
	// m_c3dExePath;
	int ret = QProcess::execute(program, cmdList);
		if (ret == 0)
	{
		qDebug() << "process end";
#ifdef _WIN32
	#if( (defined CXX_CHECK_MEMORY_LEAKS) && (defined _DEBUG) && (_MSC_VER))
		runPythonScript();	
	#endif
#endif
	}
	else if (ret == -1)
	{
		m_mess_text = "启动的程序有崩溃！";
		emit messageChanged();
	}
	else if (ret == -2)
	{
		m_mess_text = "程序未启动，请检查输入的指令是否正确！";
		emit messageChanged();
	}
}
void UnitTestTool::runPythonScript() {
	QString vld_resultPath = QCoreApplication::applicationDirPath() + "/vld_cache.txt";
	if (!QFileInfo::exists(vld_resultPath))
	{
		const QString other_vld_resultPath = CUR_BINARY_DIR + QStringLiteral("/Creative3D/vld_cache.txt");
		if(QFileInfo::exists(other_vld_resultPath))
			QFile::copy(other_vld_resultPath, vld_resultPath);
	}
	QString appPath = QCoreApplication::applicationDirPath();

    QProcess *process = new QProcess;
    process->start("python", QStringList() << appPath + "/vld_parse.py");
    QObject::connect(process, &QProcess::readyReadStandardOutput, [=]() {
        qDebug() << process->readAllStandardOutput();
		 m_mess_text += process->readAllStandardOutput();
		emit messageChanged();
    });
    QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << "Script finished with exit code:" << exitStatus << "and status:" << exitStatus;
		m_mess_text += "\nScript finished with exit code:" + QString::number(exitStatus) + QString("and status:") + QString::number(exitStatus);
		m_mess_text += "\nvld检测内存泄漏后的文件:\n" + QCoreApplication::applicationDirPath() + "/vld_cache.txt";
		m_mess_text += "\nvld检测后的结果，分类过滤后的目录:\n" + QCoreApplication::applicationDirPath() + "/vld_result/";
		emit messageChanged();
    });
}