#ifndef _CXCELL_LOG_HPP_
#define _CXCELL_LOG_HPP_

#include"cxcelltaskcontainer.h"
#include<ctime>
#include<string>
#include <typeinfo>
#include <filesystem>
#include<chrono>
#include<thread>
#include<QString>

 class CXCELLLog
{
public:
	typedef enum
	{
		INFO = 0,
		DEBUG,
		WARING,
		ERROR,
	} LOGLEVEL;
	//////init log system
	void initLogSystem(const char* pPath = "./log", bool systemLogDebug = false)
	{
		m_systemLogDebug = systemLogDebug;
		if (!std::filesystem::exists(pPath))
		{
			std::filesystem::path path(pPath);
			create_directory(path);
		}
		std::string filePath(pPath);
		/////获取当天日志文件
		char curDate[20] = { 0 };
		auto t = std::chrono::system_clock::now();
		auto tNow = std::chrono::system_clock::to_time_t(t);
		std::tm* now = std::localtime(&tNow);
		sprintf(curDate, "_%04d%02d%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
		filePath += "/creative3d";
		filePath += curDate;
		filePath += ".log";
		setLogPath(filePath.c_str());
	}
private:
	CXCELLLog()
	{
		_taskContainer.Start();
	}
	////setLogPath
	void setLogPath(const char* pLogPath, const char* mode = "ab")
	{
		if (_logFile)
		{
			Info("CELLLog::setLogPath _logFile != nullptr\n");
			fclose(_logFile);
			_logFile = nullptr;
		}
		_logFile = fopen(pLogPath, mode);
		if (_logFile)
		{
			CXCELLLog::Instance().Info("CELLLog::setLogPath success ");
		}
		else {
			CXCELLLog::Instance().Error("CELLLog::setLogPath failed ");
		}
	}
	////cur time str buf
	void getNowTimeBuf(char* nowTimeBuf)
	{
		auto t = std::chrono::system_clock::now();
		auto tNow = std::chrono::system_clock::to_time_t(t);
		std::tm* now = std::localtime(&tNow);
		sprintf(nowTimeBuf, "[%d-%02d-%02d %02d:%02d:%02d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	}

public:
	static CXCELLLog& Instance()
	{
		static  CXCELLLog sLog;
		return sLog;
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
	void CXLog(LOGLEVEL level,const char* pStr)
	{
		if (level == DEBUG && (!m_systemLogDebug)) return;
		char leveStr[8] = { 0 };
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				switch (level)
				{
				case INFO:
					sprintf((char*)leveStr,"%s","Info");
					break;
				case DEBUG:
					sprintf((char*)leveStr, "%s", "Debug");
					break;
				case WARING:
					sprintf((char*)leveStr, "%s", "Warring");
					break;
				case ERROR:
					sprintf((char*)leveStr, "%s", "Error");
					break;
				default:
					sprintf((char*)leveStr, "%s", "NONE");
					break;
				}
				fprintf(pLog->_logFile, "%s[%s]%s \n", m_nowTimeBuf, leveStr, pStr);
				fflush(pLog->_logFile);
			}
			printf("%s\n", pStr);
			});
	}
	template<typename ...Args>
	void CXLog(LOGLEVEL level, const char* pformat,Args ... args)
	{
		if (level == DEBUG && (!m_systemLogDebug)) return;
		char leveStr[8] = { 0 };
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				switch (level)
				{
				case INFO:
					sprintf((char*)leveStr, "%s", "Info");
					break;
				case DEBUG:
					sprintf((char*)leveStr, "%s", "Debug");
					break;
				case WARING:
					sprintf((char*)leveStr, "%s", "Warring");
					break;
				case ERROR:
					sprintf((char*)leveStr, "%s", "Error");
					break;
				default:
					sprintf((char*)leveStr, "%s", "NONE");
					break;
				}
				fprintf(pLog->_logFile, "%s[%s]", m_nowTimeBuf, leveStr);
				fprintf(pLog->_logFile, pformat, args...);
				fflush(pLog->_logFile);
			}
			printf(pformat, args...);
			});
	}

	//////////Info///////////////////////////////////////////////////////////////
	void Info(const char* pStr)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s%s \n", m_nowTimeBuf, "[Info]", pStr);
   			    fflush(pLog->_logFile);
			}
			printf("%s\n", pStr);
		});
	}
	void Info(QString pStr)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s%s \n", m_nowTimeBuf, "[Info]", pStr.toStdString().c_str());
				fflush(pLog->_logFile);
			}
			printf("%s\n", pStr.toStdString().c_str());
			});
	}


template<typename ...Args>
 void Info(const char* pformat,Args ... args)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s", m_nowTimeBuf, "[Info]");
				fprintf(pLog->_logFile, pformat, args...);
				fflush(pLog->_logFile);
			}
			printf(pformat, args...);
			});
	}
	//////////////////////////////////Debug////////////////////////////////////////////
void Debug(const char* pStr)
	{
	if (!m_systemLogDebug) return;
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s%s \n", m_nowTimeBuf, "[Debug]", pStr);
				fflush(pLog->_logFile);
			}
			printf("%s \n", pStr);
		});
	}

template<typename ...Args>
void Debug(const char* pformat,Args ... args)
{
	if (!m_systemLogDebug) return;
	CXCELLLog* pLog = &Instance();
	pLog->_taskContainer.AddTask([=]() {
		if (pLog->_logFile)
		{
			char m_nowTimeBuf[32] = { 0 };
			pLog->getNowTimeBuf(m_nowTimeBuf);
			fprintf(pLog->_logFile, "%s%s", m_nowTimeBuf, "[Debug]");
			fprintf(pLog->_logFile, pformat, args...);
			fflush(pLog->_logFile);
		}
		printf(pformat, args...);
		});
}
void Debug(QString pStr)
{
	if (!m_systemLogDebug) return;
	CXCELLLog* pLog = &Instance();
	pLog->_taskContainer.AddTask([=]() {
		if (pLog->_logFile)
		{
			char m_nowTimeBuf[32] = { 0 };
			pLog->getNowTimeBuf(m_nowTimeBuf);
			fprintf(pLog->_logFile, "%s%s%s \n", m_nowTimeBuf, "[Debug]", pStr.toStdString().c_str());
			fflush(pLog->_logFile);
		}
		printf("%s\n", pStr.toStdString().c_str());
		});
}


/////////////////////////////Error/////////////////////////
	void Error(const char* pStr)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s%s \n", m_nowTimeBuf, "[Error]", pStr);
				fflush(pLog->_logFile);
			}
			printf("%s\n", pStr);
			});
	}
	void Error(QString qStr)
	{
		std::string str = qStr.toStdString();
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s%s \n", m_nowTimeBuf, "[Error]", str.c_str());
				fflush(pLog->_logFile);
			}
			printf("%s\n", str.c_str());
			});
	}
	template<typename ...Args>
	void Error(const char* pformat, Args ... args)
	{
		CXCELLLog* pLog = &Instance();
		pLog->_taskContainer.AddTask([=]() {
			if (pLog->_logFile)
			{
				char m_nowTimeBuf[32] = { 0 };
				pLog->getNowTimeBuf(m_nowTimeBuf);
				fprintf(pLog->_logFile, "%s%s", m_nowTimeBuf, "[Error]");
				fprintf(pLog->_logFile, pformat, args...);
				fflush(pLog->_logFile);
			}
			printf(pformat, args...);
			});
	}
private:
	FILE* _logFile = nullptr;
	CXCELLTaskContainer _taskContainer;
	bool m_systemLogDebug = 0; ///if debug
};
#endif // !_CXCELL_LOG_HPP_
