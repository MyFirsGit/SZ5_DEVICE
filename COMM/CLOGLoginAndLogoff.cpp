#include "stdafx.h"
#include "CLOGLoginAndLogoff.h"
#include "InnerException.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLOGLoginAndLogoff CLOGLoginAndLogoff::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLOGLoginAndLogoff::CLOGLoginAndLogoff(void)
:CLog(FILTER_LOGIN_LOGOFF_LOG)
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
CLOGLoginAndLogoff::~CLOGLoginAndLogoff(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写日志

@param      (i)OPERATION_DIFFERENCE nOperationDifference           登录区分         1:登录,2:登出
@param      (i)unsigned long strUserID                             员工ID
@param      (i)STAFF_AUTHORITY nAuthority                          员工权限         1:普通站员,2:维护员
@param      (i)unsigned char groupNo                               班次

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLOGLoginAndLogoff::WriteData(LOGINOUT_TYPE loginoutType,CString strUserID,STAFF_AUTHORITY nAuthority)
{
	try{
		CString strOperationDifference = _T("");
		CString strAuthority = _T("");

		// 登陆区分
		strOperationDifference= loginoutType == LOG_IN ? _T("LOGIN IN"):_T("LOGIN OFF");

		// 员工权限
		strAuthority=nAuthority == COMMON_AUTHORITY ? _T("LOGIN COMMON") :_T("LOGIN MAINTAIN");
		
		CString sData = _T("");
		sData = strOperationDifference + _T(",") + strUserID + _T(",") + strAuthority ;

		__super::WriteData(sData);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CLogException::OTHER_ERR, _T(__FILE__) , __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      关闭日志连接

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLOGLoginAndLogoff::ReleaseLoginLog(){
	__super::CloseLog();
}