#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif


// CTxnAuditApp
// See AR.cpp for the implementation of this class
//

class CTxnAuditApp : public CWinApp
{
public:
	CTxnAuditApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
