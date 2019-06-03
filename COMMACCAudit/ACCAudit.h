#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


// CACCAuditApp
// See AR.cpp for the implementation of this class
//

class CACCAuditApp : public CWinApp
{
public:
	CACCAuditApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
