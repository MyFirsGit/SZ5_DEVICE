// LPT.h : main header file for the LPT DLL
//

#pragma once
#include "LPTHardwareException.h"
#include "LPTExp.h"

#ifndef __AFXWIN_H__
	#error _T("include 'stdafx.h' before including this file for PCH")
#endif

#include "resource.h"		// main symbols


// CLPTApp
// See LPT.cpp for the implementation of this class
//

class CLPTApp : public CWinApp
{
public:
	CLPTApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#ifdef WIN_DLL
#define WIN_API _declspec(dllexport)
#else
#define WIN_API _declspec(dllimport)
#endif


extern "C"
{
	WIN_API bool _stdcall InitializeWinIo();
	WIN_API void _stdcall ShutdownWinIo();
	WIN_API PBYTE _stdcall MapPhysToLin(PBYTE pbPhysAddr, DWORD dwPhysSize, HANDLE *pPhysicalMemoryHandle);
	WIN_API bool _stdcall UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PBYTE pbLinAddr);
	WIN_API bool _stdcall GetPhysLong(PBYTE pbPhysAddr, PDWORD pdwPhysVal);
	WIN_API bool _stdcall SetPhysLong(PBYTE pbPhysAddr, DWORD dwPhysVal);
	WIN_API bool _stdcall GetPortVal(WORD wPortAddr, PDWORD pdwPortVal, BYTE bSize);
	WIN_API bool _stdcall SetPortVal(WORD wPortAddr, DWORD dwPortVal, BYTE bSize);
	WIN_API bool _stdcall InstallWinIoDriver(PSTR pszWinIoDriverPath, bool IsDemandLoaded = false);
	WIN_API bool _stdcall RemoveWinIoDriver();
}

#include "Command.h"
const WORD LPT_PORT_NO = (WORD)0x379;
const int LPT_SIZE = 1;

/**
 @brief  并口命令--打开并口
*/

class LPT_API OpenLPT : public CCommand
{
public:
	OpenLPT();   
    ~OpenLPT(void);
public:
	long ExecuteCommand(void);        //执行命令
private:
	long ConnectFunction();            //打开并口
};

/**
@brief  并口命令--读并口
*/
class LPT_API ReadLPT : public CCommand
{
public:
	ReadLPT(WORD port =LPT_PORT_NO ,int size = LPT_SIZE);   
	~ReadLPT(void);
public:
	WORD port;
	int size;
	BYTE m_response;
	long ExecuteCommand(void);        //执行命令
	void GetLPTResponse(BYTE&);
private:
	long ReadFunction();            //读并口
};

/**
@brief  并口命令--关闭并口
*/
class LPT_API CloseLPT : public CCommand
{
public:
	CloseLPT();   
	~CloseLPT(void);
public:
	long ExecuteCommand(void);        //执行命令
private:
	void CloseFunction();            //关闭并口
};
