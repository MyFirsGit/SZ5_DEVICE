// Core.h : main header file for the Core DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCoreApp
// See Core.cpp for the implementation of this class
//

class CCoreApp : public CWinApp
{
public:
	CCoreApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
