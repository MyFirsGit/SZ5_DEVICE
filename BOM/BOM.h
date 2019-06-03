#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "util.h"

/**
@brief	BOM应用程序类
*/

class CBOMApp : public CWinApp
{
	DECLARE_MESSAGE_MAP()

public:
	CBOMApp();                                  // 构造函数
	virtual BOOL InitInstance();                // 标准初期化函数
	virtual int ExitInstance();                 // 标准终止函数

public:
	CWnd* GetMainWndRec();                      // 得到乘客主窗口的指针
	CWnd* GetMainWndRecWnd();					// 得到乘客主窗口的指针
	virtual BOOL OnIdle(LONG lCount);           // 空闲函数

public:
	CWnd* m_pMainWndRec;                        // 乘客主窗口的指针
private:
	static long  ProcessUnHandleException(EXCEPTION_POINTERS* lpExceptionInfo); //

};

extern CBOMApp theApp;

/////////////////////////////////////////////////////////////////////////////
