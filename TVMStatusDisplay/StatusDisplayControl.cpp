#include "stdafx.h"
#include "StatusDisplayControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

/**
 @brief      ���캯��
                                                                 
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayControl::CStatusDisplayControl(void )
{
}
//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������
                                                                 
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

CStatusDisplayControl::~CStatusDisplayControl(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��

@param      (i) bool bWithSense
(i) int iIdleTimer

@retval     long      

@exception  ��
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
@brief      �رտ���

@param      ��

@retval     long      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayControl::Shutdown(void)
{
	CControl::Shutdown();
	//m_pRWLibControl->Shutdown();
	return 0;
}
