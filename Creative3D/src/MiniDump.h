#if _WINDOWS || WINDOWS || WIN32 || WIN64
#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tchar.h>
#pragma comment(lib, "dbghelp.lib")

#ifdef UNICODE
#define TSprintf	wsprintf
#else
#define TSprintf	sprintf
#endif


class MiniDump
{
private:
	MiniDump();
	~MiniDump();

public:
	// 程序崩溃时是否启动自动生成dump文件;
	// 只需要在main函数开始处调用该函数即可;
	static void EnableAutoDump(std::string path, bool bEnable = true);
	static void test();

private:

	static LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);

	static void CreateDumpFile(TCHAR* lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);
};
#endif
