#if !defined(AFX_COMMON_H__B959A317_B089_4978_8207_9B9D63D2CB1C__INCLUDED_)
#define AFX_COMMON_H__B959A317_B089_4978_8207_9B9D63D2CB1C__INCLUDED_

#include <afx.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "afxinet.h"
#include "FTPManager.h"

extern CFTPManager	  m_Ftp;
extern CInternetSession* m_pintsession;
extern CFtpConnection *m_pFtpConnection;

extern CString		m_strCurrentDirectory;
extern BOOL			m_bIsDirectory;

#endif