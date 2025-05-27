#ifndef _CXCELL_LOG_HPP_
#define _CXCELL_LOG_HPP_

#include "CXCELLTaskContainer.h"
#include <ctime>
class CXCELLLog
{
	//Info
	//Debug
	//Warring
	//Error
private:
	CXCELLLog()
	{
		_taskContainer.Start();
	}

	~CXCELLLog()
	{
		_taskContainer.Close();
		if (_logFile)
		{
			Info("CELLLog fclose(_logFile)\n");
			fclose(_logFile);
			_logFile = nullptr;
		}
	}
public:
	static CXCELLLog& Instance()
	{
		static  CXCELLLog sLog;
		return sLog;
	}
	void setLogPath(const char* logPath, const char* mode)
	{
		if (_logFile)
		{
			Info("CELLLog::setLogPath _logFile != nullptr\n");
			fclose(_logFile);
			_logFile = nullptr;
		}
		_logFile = fopen(logPath, mode);
		if (_logFile)
		{
			Info("CELLLog::setLogPath success,<%s,%s>\n", logPath, mode);
		}
		else {
			Info("CELLLog::setLogPath failed,<%s,%s>\n", logPath, mode);
		}
	}
	static void Info(const char* pStr)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				auto t = system_clock::now();
				auto tNow = system_clock::to_time_t(t);
				//fprintf(pLog->_logFile, "%s", ctime(&tNow));
				std::tm* now = std::gmtime(&tNow);
				fprintf(pLog->_logFile, "%s", "Info ");
				fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
				fprintf(pLog->_logFile, "%s\n", pStr);
				fflush(pLog->_logFile);
			}
			printf("%s\n", pStr);
		});
	}
	static void Debug(const char* pStr)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				auto t = system_clock::now();
				auto tNow = system_clock::to_time_t(t);
				//fprintf(pLog->_logFile, "%s", ctime(&tNow));
				std::tm* now = std::gmtime(&tNow);
				fprintf(pLog->_logFile, "%s", "Debug ");
				fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
				fprintf(pLog->_logFile, "%s\n", pStr);
				fflush(pLog->_logFile);
			}
			printf("%s\n", pStr);
		});
	}
	static void Error(const char* pStr)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				auto t = system_clock::now();
				auto tNow = system_clock::to_time_t(t);
				//fprintf(pLog->_logFile, "%s", ctime(&tNow));
				std::tm* now = std::gmtime(&tNow);
				fprintf(pLog->_logFile, "%s", "Error ");
				fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
				fprintf(pLog->_logFile, "%s\n", pStr);
				fflush(pLog->_logFile);
			}
			printf("%s\n", pStr);
			});
	}
	template<typename ...Args>
	static void Error(const char* pformat, Args ... args)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				auto t = system_clock::now();
				auto tNow = system_clock::to_time_t(t);
				//fprintf(pLog->_logFile, "%s", ctime(&tNow));
				std::tm* now = std::gmtime(&tNow);
				fprintf(pLog->_logFile, "%s", "Info ");
				fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
				fprintf(pLog->_logFile, pformat, args...);
				fflush(pLog->_logFile);
			}
			printf(pformat, args...);
			});
	}


	template<typename ...Args>
	static void Info(const char* pformat, Args ... args)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				auto t = system_clock::now();
				auto tNow = system_clock::to_time_t(t);
				//fprintf(pLog->_logFile, "%s", ctime(&tNow));
				std::tm* now = std::gmtime(&tNow);
				fprintf(pLog->_logFile, "%s", "Info ");
				fprintf(pLog->_logFile, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
				fprintf(pLog->_logFile, pformat, args...);
				fflush(pLog->_logFile);
			}
			//printf(pformat, args...);
		});
	}
private:
	FILE* _logFile = nullptr;
	CXCELLTaskContainer _taskContainer;
};
#endif // !_CXCELL_LOG_HPP_
