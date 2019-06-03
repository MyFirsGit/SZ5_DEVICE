#include "stdafx.h"
#include "StatusDisplayControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

/**
 @brief      构造函数
                                                                 
 @param      (i)无

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayControl::CStatusDisplayControl(void )
{
}
//////////////////////////////////////////////////////////////////////////
/**
 @brief      析构函数
                                                                 
 @param      (i)无

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayControl::~CStatusDisplayControl(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化

@param      (i) bool bWithSense
(i) int iIdleTimer

@retval     long      

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayControl::Initialize()
{
	CControl::Initialize();
	//m_pRWLibControl->Initialize();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭控制

@param      无

@retval     long      

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayControl::Shutdown(void)
{
	CControl::Shutdown();
	//m_pRWLibControl->Shutdown();
	return 0;
}
