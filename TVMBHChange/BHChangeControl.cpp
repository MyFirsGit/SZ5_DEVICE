#include "stdafx.h"
#include "BHChangeControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��
                                                                 
 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeControl::CBHChangeControl()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������
                                                                 
 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeControl::~CBHChangeControl()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ��

 @param		(i) bool bWithSense
(i) int iIdleTimer

@retval     long      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeControl::Initialize()
{
	CControl::Initialize();
	//m_pRWLibControl->Initialize();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�رտ���

 @param		��

 @retval	long      

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeControl::Shutdown()
{
	CControl::Shutdown();
	//m_pRWLibControl->Shutdown();
	return 0;
}
