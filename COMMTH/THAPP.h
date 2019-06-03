//TH DLL 的主头文件
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"		// 主符号

/** 
@brief   TH DLL 的主文件
*/
class CTHApp : public CWinApp
{
public:
	CTHApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};
