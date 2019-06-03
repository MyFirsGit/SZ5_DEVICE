#include "stdafx.h"
#include "StatusDisplaydef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COMM_HANDLE g_hStatusDisplay=NULL;

HANDLE g_hShowComplete=::CreateEvent(NULL,FALSE,FALSE,NULL);

void CALLBACK OnGetResponseCmd()
{
	::SetEvent(g_hShowComplete);
}