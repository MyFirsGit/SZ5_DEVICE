#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "util.h"
#include "LanguageDialog.h"

#include "CreateDump.h"
/**
@brief	BOMӦ�ó�����
*/

class CTVMApp : public CWinApp
{
	DECLARE_MESSAGE_MAP()

public:
	CTVMApp();                                  // ���캯��
	virtual BOOL InitInstance();                // ��׼���ڻ�����
	virtual int ExitInstance();                 // ��׼��ֹ����

public:
	CWnd* GetMainWndRec();                      // �õ��˿������ڵ�ָ��
	CWnd* GetMainWndRecWnd();					// �õ��˿������ڵ�ָ��
	virtual BOOL OnIdle(LONG lCount);           // ���к���

public:
	CWnd* m_pMainWndRec;                        // �˿������ڵ�ָ��
private:
	static long  ProcessUnHandleException(EXCEPTION_POINTERS* lpExceptionInfo); //
};

extern CTVMApp theApp;

/////////////////////////////////////////////////////////////////////////////
