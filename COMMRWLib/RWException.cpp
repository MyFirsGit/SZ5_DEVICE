#include "StdAfx.h"
#include "RWException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	外部RW软件异常类
*/

//IMPLEMENT_DYNAMIC(CRWException, CSysException)

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)long lDetailID,      模块内详细编码
@param      (i)CString sFileName    异常发生时文件名
@param      (i)long lLineNo         异常发生时行号
@param      (i)CString sMessage     错误信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWException::CRWException(long lDetailID, CString sFileName, long lLineNo, CString sMessage)
    :CSysException(MODULE_ID, lDetailID, sFileName, lLineNo, sMessage)
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)long lDetailID,      模块内详细编码
@param      (i)CString sFileName    异常发生时文件名
@param      (i)long lLineNo         异常发生时行号
@param      (i)CString sMessage     错误信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWException::CRWException(RW_TYPE rwType,long lDetailID, CString sFileName, long lLineNo, CString sMessage)
:CSysException(GetModuleID(rwType), lDetailID, sFileName, lLineNo, sMessage)
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
CRWException::~CRWException()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   根据读写器类型取得模块ID   

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
//@brief      析构函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CRWException::~CRWException()
//{
//}
