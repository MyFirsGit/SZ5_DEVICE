//TH DLL ����ͷ�ļ�
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"		// ������

/** 
@brief   TH DLL �����ļ�
*/
class CTHApp : public CWinApp
{
public:
	CTHApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};
