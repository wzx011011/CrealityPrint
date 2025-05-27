#include "logsystem.h"
//#include "cxcelllog.h"
#include "log.h"
LogSystem::LogSystem()
{
}

LogSystem::~LogSystem()
{
}

QString LogSystem::name()
{
	return "LogSystem";
}

QString LogSystem::info()
{
	return "";
}

void LogSystem::initialize()
{
#ifdef QT_NO_DEBUG
    logInit();
#endif
    //CXCELLLog::Instance().initLogSystem("./log", true);
}

void LogSystem::uninitialize()
{
}
