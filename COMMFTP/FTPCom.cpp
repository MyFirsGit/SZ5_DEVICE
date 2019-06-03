#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFTPManager m_Ftp;
CInternetSession* m_pintsession ;
CFtpConnection *m_pFtpConnection;

CString		m_strCurrentDirectory;
BOOL		m_bIsDirectory;
