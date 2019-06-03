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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionLog::CExceptionLog(void)
    :CLog(FILTER_EXCEPTION_LOG)
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
CExceptionLog::~CExceptionLog(void)
{
}

CExceptionLog&  CExceptionLog::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写日志

@param      (i)EXCEPTION_LEVEL exceptionLevel         异常级别
@param      (i)long lMTCCode                          MTCCode
@param      (i)CString strFileName                    模块名,文件名
@param      (i)long lRowNum                           行号
@param      (i)CString strError                       错误说明

@retval     无

@exception  无
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
@brief      关闭日志

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionLog::ReleaseExceptionLog(){
	__super::CloseLog();
}