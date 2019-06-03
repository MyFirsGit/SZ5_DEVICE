// Common.h : main header file for the Common DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCommonApp
// See Common.cpp for the implementation of this class
//

class CCommonApp : public CWinApp
{
public:
	CCommonApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
