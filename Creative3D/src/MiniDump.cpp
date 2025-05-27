
#if _WINDOWS || WINDOWS || WIN32 || WIN64
#include "MiniDump.h"
#include "interface/gadatainterface.h"
#include "kernel/kernelui.h"

static std::string s_path;
MiniDump::MiniDump()
{
}


MiniDump::~MiniDump()
{
}

void MiniDump::EnableAutoDump(std::string path, bool bEnable)
{
	s_path = path;
	if (bEnable)
	{
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER) ApplicationCrashHandler);
	}
}

LONG MiniDump::ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	/*if (IsDebuggerPresent())
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}*/

	TCHAR szDumpDir[MAX_PATH] = { 0 };
	TCHAR szDumpFile[MAX_PATH] = { 0 };
	TCHAR szMsg[MAX_PATH] = { 0 };
	SYSTEMTIME	stTime = { 0 };
	// 构建dump文件路径;
	GetLocalTime(&stTime);
	::GetCurrentDirectory(MAX_PATH, szDumpDir);

	QString version = getKernelUI()->currentVersion();

	sprintf(szDumpFile, ("%s/%04d%02d%02d_%02d%02d%02d_Creality Print %s.dmp"), s_path.data(),
		stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond,
		version.toStdString().c_str());

	creative_kernel::sendDataToGA("Creality Print", "Crash");

	

	// 创建dump文件;
	CreateDumpFile(szDumpFile, pException);

	// 弹出一个错误对话框或者提示上传， 并退出程序;
	sprintf(szMsg,  ("I'm so sorry, but the program crashed.\r\ndump file : %s"), szDumpFile);
	FatalAppExit(1, szMsg);

	return EXCEPTION_EXECUTE_HANDLER;
}

void MiniDump::CreateDumpFile(TCHAR* strPath, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件;
	HANDLE hDumpFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Dump信息;
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}

void MiniDump::test()
{
}
#endif
