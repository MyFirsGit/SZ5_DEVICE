#include "StdAfx.h"
#include "CreateDump.h"
#include <atlbase.h>
#include <atlstr.h>
#include <strsafe.h>
#include <DbgHelp.h>
#pragma comment(lib,"DbgHelp.lib")
#define GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS        (0x00000004)
#define MiniDumpWithThreadInfo 0x1000

/*
* zhengxiale:收集系统出现异常信息，写入文件E:\Log目录下
*/
typedef BOOL (WINAPI *PGetModuleHandleEx)( DWORD dwFlags, LPCTSTR lpModuleName, HMODULE *phModule );

VOID CreateDump(struct _EXCEPTION_POINTERS *pExceptionPointers) 
{
	//收集信息
	CStringW strBuild;
	strBuild.Format(L"Build: %s %s", __DATE__, __TIME__);
	CString strError;
	HMODULE hModule;
	WCHAR szModuleName[MAX_PATH] = {0};

	PGetModuleHandleEx pFun = (PGetModuleHandleEx)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "GetModuleHandleExW");
	if ( !pFun ) {
		return;
	}

	pFun(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)pExceptionPointers->ExceptionRecord->ExceptionAddress, &hModule);
	GetModuleFileName(hModule, szModuleName, ARRAYSIZE(szModuleName));
	strError.Format(L"%s %d , %d ,%d.", szModuleName,pExceptionPointers->ExceptionRecord->ExceptionCode, pExceptionPointers->ExceptionRecord->ExceptionFlags, pExceptionPointers->ExceptionRecord->ExceptionAddress);

	//生成 mini crash dump
	BOOL bMiniDumpSuccessful;
	WCHAR szPath[MAX_PATH]; 
	WCHAR szFileName[MAX_PATH]; 
	WCHAR* szAppName = L"DumpFile";
	WCHAR* szVersion = L"v1.0";
	DWORD dwBufferSize = MAX_PATH;
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;
	GetLocalTime( &stLocalTime );
	//GetTempPath( dwBufferSize, szPath );
	
	//StringCchPrintf( szFileName, MAX_PATH, L"%s%s", szPath, szAppName );
	//CreateDirectory( szFileName, NULL );
	StringCchPrintf( szFileName, MAX_PATH, L"%s//TVM-V%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
		theTVM_INFO.GetLogPath(),theVERSION_INI.GetMCVersionNoString(), 
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
		GetCurrentProcessId(), GetCurrentThreadId());
	hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	MINIDUMP_USER_STREAM UserStream[2];
	MINIDUMP_USER_STREAM_INFORMATION UserInfo;
	UserInfo.UserStreamCount = 1;
	UserInfo.UserStreamArray = UserStream;
	UserStream[0].Type = CommentStreamW;
	UserStream[0].BufferSize = strBuild.GetLength()*sizeof(WCHAR);
	UserStream[0].Buffer = strBuild.GetBuffer();
	UserStream[1].Type = CommentStreamW;
	UserStream[1].BufferSize = strError.GetLength()*sizeof(WCHAR);
	UserStream[1].Buffer = strError.GetBuffer();

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	MINIDUMP_TYPE MiniDumpWithDataSegs = (MINIDUMP_TYPE)(MiniDumpNormal 
		| MiniDumpWithHandleData 
		| MiniDumpWithUnloadedModules 
		| MiniDumpWithIndirectlyReferencedMemory 
		| MiniDumpScanMemory 
		| MiniDumpWithProcessThreadData 
		| MiniDumpWithThreadInfo);

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

	return;
}
