//RW DLL ����ͷ�ļ�
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"		// ������

/** 
@brief   RW DLL �����ļ�
*/
class CRWApp : public CWinApp
{
public:
	CRWApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};
