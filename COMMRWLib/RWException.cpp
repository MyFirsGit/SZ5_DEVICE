#include "StdAfx.h"
#include "RWException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	�ⲿRW����쳣��
*/

//IMPLEMENT_DYNAMIC(CRWException, CSysException)

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
CRWException::CRWException(long lDetailID, CString sFileName, long lLineNo, CString sMessage)
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
CRWException::CRWException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage)
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
CRWException::~CRWException()
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
long CRWException::GetModuleID(RW_TYPE rwType)
{
	switch(rwType)	{
	case BOM_DESKTOP_RW: return CRWException::BOM_DESKTOP_RW_MODULE_ID;
	case BOM_CARD_RW: return CRWException::BOM_CARD_RW_MODULE_ID;
	case BOM_TOKEN_RW: return CRWException::BOM_TOKEN_RW_MODULE_ID;
	case TVM_RECHARGE_RW: return CRWException::TVM_RECHARGE_RW_MODULE_ID;
	case TVM_CARD_RW: return CRWException::TVM_CARD_RW_MODULE_ID;
	case TVM_TOKEN_RW: return CRWException::TVM_TOKEN_RW_MODULE_ID;
	case AGM_CARD_RECYCLE_RW: return CRWException::AGM_CARD_RECYCLE_RW_MODULE_ID;
	case AGM_TOKEN_RECYCLE_RW: return CRWException::AGM_TOKEN_RECYCLE_RW_MODULE_ID;
	case AGM_ENTRY_RW: return CRWException::AGM_ENTRY_RW_MODULE_ID;
	case AGM_EXIT_RW: return CRWException::AGM_EXIT_RW_MODULE_ID;
	case ES_TOKEN_RW1: return CRWException::ES_TOKEN_RW1_MODULE_ID;
	case ES_TOKEN_RW2: return CRWException::ES_TOKEN_RW2_MODULE_ID;
	case ES_TOKEN_RW3: return CRWException::ES_TOKEN_RW3_MODULE_ID;
	case ES_CARD_RW: return CRWException::ES_CARD_RW_MODULE_ID;
	case PM_RW: return CRWException::PM_RW_MODULE_ID;
	case TCM_RW: return CRWException::TCM_RW_MODULE_ID;
	default:
		return CRWException::BOM_DESKTOP_RW_MODULE_ID;
	}
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CRWException::~CRWException()
//{
//}
