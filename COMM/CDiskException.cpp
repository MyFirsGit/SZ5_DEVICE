#include  "stdafx.h"
#include "CDiskException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDiskException::CDiskException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage)
:CSysException(MODULE_ID, lDetailID, sFileName, lLineNo, sMessage)
{
}

CDiskException::~CDiskException()
{
}