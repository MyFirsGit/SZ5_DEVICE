#include "StdAfx.h"
#include "RWHardwareException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	�ⲿ��д��Ӳ���쳣��
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)long lDetailID,      ģ������ϸ����
@param      (i)CString sFileName    �쳣����ʱ�ļ���
@param      (i)long lLineNo         �쳣����ʱ�к�
@param      (i)CString sMessage     ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWHardwareException::CRWHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage)
    :CSysException(MODULE_ID, lDetailID, sFileName, lLineNo, sMessage)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)long lDetailID,      ģ������ϸ����
@param      (i)CString sFileName    �쳣����ʱ�ļ���
@param      (i)long lLineNo         �쳣����ʱ�к�
@param      (i)CString sMessage     ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWHardwareException::CRWHardwareException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage)
:CSysException(GetModuleID(rwType), lDetailID, sFileName, lLineNo, sMessage)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWHardwareException::~CRWHardwareException()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���ݶ�д������ȡ��ģ��ID   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CRWHardwareException::GetModuleID(RW_TYPE rwType)
{
	switch(rwType)
	{
	case BOM_DESKTOP_RW: return CRWHardwareException::BOM_DESKTOP_RW_MODULE_ID;
	case BOM_CARD_RW: return CRWHardwareException::BOM_CARD_RW_MODULE_ID;
	case BOM_TOKEN_RW: return CRWHardwareException::BOM_TOKEN_RW_MODULE_ID;
	case TVM_RECHARGE_RW: return CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID;
	case TVM_CARD_RW: return CRWHardwareException::TVM_CARD_RW_MODULE_ID;
	case TVM_TOKEN_RW: return CRWHardwareException::TVM_TOKEN_RW_MODULE_ID;
	case AGM_CARD_RECYCLE_RW: return CRWHardwareException::AGM_CARD_RECYCLE_RW_MODULE_ID;
	case AGM_TOKEN_RECYCLE_RW: return CRWHardwareException::AGM_TOKEN_RECYCLE_RW_MODULE_ID;
	case AGM_ENTRY_RW: return CRWHardwareException::AGM_ENTRY_RW_MODULE_ID;
	case AGM_EXIT_RW: return CRWHardwareException::AGM_EXIT_RW_MODULE_ID;
	case ES_TOKEN_RW1: return CRWHardwareException::ES_TOKEN_RW1_MODULE_ID;
	case ES_TOKEN_RW2: return CRWHardwareException::ES_TOKEN_RW2_MODULE_ID;
	case ES_TOKEN_RW3: return CRWHardwareException::ES_TOKEN_RW3_MODULE_ID;
	case ES_CARD_RW: return CRWHardwareException::ES_CARD_RW_MODULE_ID;
	case PM_RW: return CRWHardwareException::PM_RW_MODULE_ID;
	case TCM_RW: return CRWHardwareException::TCM_RW_MODULE_ID;
	default:
		return CRWHardwareException::BOM_DESKTOP_RW_MODULE_ID;
	}
}