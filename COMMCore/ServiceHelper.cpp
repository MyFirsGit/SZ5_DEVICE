#include "StdAfx.h"
#include "ServiceHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief Service Helper
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CServiceHelper::CServiceHelper(CService& service)
    :m_service(service)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CServiceHelper::~CServiceHelper(void)
{
}
