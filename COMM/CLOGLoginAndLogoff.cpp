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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLOGLoginAndLogoff::CLOGLoginAndLogoff(void)
:CLog(FILTER_LOGIN_LOGOFF_LOG)
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
CLOGLoginAndLogoff::~CLOGLoginAndLogoff(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д��־

@param      (i)OPERATION_DIFFERENCE nOperationDifference           ��¼����         1:��¼,2:�ǳ�
@param      (i)unsigned long strUserID                             Ա��ID
@param      (i)STAFF_AUTHORITY nAuthority                          Ա��Ȩ��         1:��ͨվԱ,2:ά��Ա
@param      (i)unsigned char groupNo                               ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLOGLoginAndLogoff::WriteData(LOGINOUT_TYPE loginoutType,CString strUserID,STAFF_AUTHORITY nAuthority)
{
	try{
		CString strOperationDifference = _T("");
		CString strAuthority = _T("");

		// ��½����
		strOperationDifference= loginoutType == LOG_IN ? _T("LOGIN IN"):_T("LOGIN OFF");

		// Ա��Ȩ��
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
@brief      �ر���־����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLOGLoginAndLogoff::ReleaseLoginLog(){
	__super::CloseLog();
}