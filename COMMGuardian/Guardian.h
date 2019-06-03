// CTGuardian.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CTGuardianApp:
// See CTGuardian.cpp for the implementation of this class
//

class CTGuardianApp : public CWinApp
{
public:
	CTGuardianApp();

    INT ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTGuardianApp theApp;