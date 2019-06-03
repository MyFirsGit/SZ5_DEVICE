// TVMBH.h : main header file for the TVMBH DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTVMBHApp
// See TVMBH.cpp for the implementation of this class
//

class CTVMBHApp : public CWinApp
{
public:
	CTVMBHApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};