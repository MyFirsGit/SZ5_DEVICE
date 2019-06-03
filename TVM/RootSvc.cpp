#include "StdAfx.h"
#include "RootSvc.h"
#include "ServiceDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRootSvc::CRootSvc(void)
    :CBackService(ROOT_SVC, UNKNOWN_SVC, false)
{
}

CRootSvc::~CRootSvc(void)
{
}
