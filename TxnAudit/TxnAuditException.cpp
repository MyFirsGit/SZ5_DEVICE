#include "StdAfx.h"
#include "TxnAuditException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	参数文件处理异常类
*/

//IMPLEMENT_DYNAMIC(CTxnAuditException, CSysException)

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)DETAIL_ERROR_CODE lDetailID      模块内详细编码
@param      (i)CString sFileName				异常发生时参数文件名
@param      (i)long lLineNo						异常发生时行号
@param      (i)CString sMessage					错误信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTxnAuditException::CTxnAuditException(DETAIL_ERROR_CODE lDetailID, CString sFileName, long lLineNo, CString sMessage)
:CSysException(MODULE_ID, lDetailID, sFileName, lLineNo, sMessage)
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
CTxnAuditException::~CTxnAuditException()
{
}
