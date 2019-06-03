// CTFTPLIB.h : main header file for the CTFTPLIB DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CCTFTPLIBApp
// See CTFTPLIB.cpp for the implementation of this class
//

class CCTFTPLIBApp : public CWinApp
{
public:
	CCTFTPLIBApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
