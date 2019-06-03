#include "stdafx.h"
#include "ExceptionLog.h"
#include "InnerException.h"
#include "LogException.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExceptionLog CExceptionLog::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionLog::CExceptionLog(void)
    :CLog(FILTER_EXCEPTION_LOG)
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
CExceptionLog::~CExceptionLog(void)
{
}

CExceptionLog&  CExceptionLog::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д��־

@param      (i)EXCEPTION_LEVEL exceptionLevel         �쳣����
@param      (i)long lMTCCode                          MTCCode
@param      (i)CString strFileName                    ģ����,�ļ���
@param      (i)long lRowNum                           �к�
@param      (i)CString strError                       ����˵��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionLog::WriteData(EXCEPTION_LEVEL exceptionLevel, long lMTCCode, CString strFileName, long lRowNum, CString strError)
{
    try {
        CString sLevel = _T("");
        CString strRowNum = _T("");
        CString strMTCCode = _T("");

        if (exceptionLevel == LEVEL_WARNING) {
            sLevel = _T("W");
        }
        else if (exceptionLevel == LEVEL_ERROR) {
            sLevel = _T("E");
        }

        strMTCCode.Format(_T("%08X"), lMTCCode);
        strRowNum.Format(_T("%d"), lRowNum);

        CString sData = _T("");
        sData = sLevel + _T(",") + strMTCCode + _T(",") + strFileName + _T(",") + strRowNum + _T(",") + strError;

        __super::WriteData(sData);

    }    
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ر���־

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionLog::ReleaseExceptionLog(){
	__super::CloseLog();
}