#include "stdafx.h"
#include "./PrinterControl.h"

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

CPrinterControl::CPrinterControl(void )
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

CPrinterControl::~CPrinterControl(void)
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
long CPrinterControl::Initialize()
{
	//CControl::Initialize();
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
long CPrinterControl::Shutdown(void)
{
	//CControl::Shutdown();
	//m_pRWLibControl->Shutdown();
	return 0;
}
