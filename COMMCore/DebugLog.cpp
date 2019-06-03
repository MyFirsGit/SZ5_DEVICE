#include "stdafx.h"
#include "InnerException.h"
#include "LogException.h"
#include <atlsimpstr.h>
#include <atlstr.h>
#include <map>
#include "DebugLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CFileLog CFileLog::theInstance(_T("FILELOG"));

//////////////////////////////////////////////////////////////////////////
/**
@brief      根据日志前缀名，取得一个写日志的对象

@param      CString logPrefix 日志前缀

@retval     CFileLog* 写日志的对象

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFileLog* CFileLog::GetInstance(CString logPrefix)
{
	if(theInstance.mapLog[logPrefix]== NULL)//没有找到，则新加一个。
	{
		CFileLog*	log = new CFileLog(logPrefix);
		theInstance.mapLog[logPrefix] = log;
	}
	return theInstance.mapLog[logPrefix];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFileLog::CFileLog(CString logPrefix)
:CLog(logPrefix)
{
	m_writeDebug = true;                               // 是否记录Debug消息
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFileLog::~CFileLog(void)
{
	for(map<CString,CFileLog*>::iterator iterLog = mapLog.begin();iterLog!=mapLog.end();iterLog++)
	{
		if(iterLog->second)
		{
			delete iterLog->second;
		}
	}
	mapLog.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置是否记录Debug消息

@param       (i)bool writeDebug

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CFileLog::SetWriteDebug(bool writeDebug)
{
	m_writeDebug = writeDebug;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写日志

@param      (i)CString className                       
@param      (i)CString methodName                       
@param      (i)int line                   
@param      (i)CString format                       

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CFileLog::WriteData(CString className, CString methodName, int line, CString format, ...)
{
	try {
		// 系统INI中若设置不记录Debug日志，则直接返回
		if (! m_writeDebug) {
			return;
		}

		// 固定数据
		CString head;
		head.Format(_T("%-14s %-22s %4d "), className, methodName, line);

		// 附加参数
		CString data;
		va_list args;
		va_start(args, format);
		data.FormatV(format, args);
		va_end(args);

		// 记入文件
		__super::WriteData(head + data+_T("\r\n"));
	}    
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


void CFileLog::WriteData(int line, CString prefix,LPBYTE lpData,UINT length,CString format)
{
	try {
		// 系统INI中若设置不记录Debug日志，则直接返回
		if (! m_writeDebug) {
			return;
		}

		CString data=_T("");
		CString sHex = _T("");
		CString strLine = _T("");
		strLine.Format(_T("（%d）"), line);
		for(unsigned int i=0;i<length;i++){
			sHex.Format(format,*(lpData+i));
			data+=sHex;
		}

		// 记入文件
		__super::WriteData(strLine + prefix + data+_T("\r\n"));
	}    
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

void CFileLog::WriteData(CString prefix,LPBYTE lpData,UINT length,CString format)
{
	try {
		// 系统INI中若设置不记录Debug日志，则直接返回
		if (! m_writeDebug) {
			return;
		}

		CString data=_T("");
		CString sHex = _T("");
		for(unsigned int i=0;i<length;i++){
			sHex.Format(format,*(lpData+i));
			data+=sHex;
		}

		// 记入文件
		__super::WriteData(prefix + data+_T("\r\n"));
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
@brief      释放Debug日志

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CFileLog::ReleaseDebugLog(){
	for(map<CString,CFileLog*>::iterator iterLog = theInstance.mapLog.begin();iterLog!=theInstance.mapLog.end();iterLog++)
	{
		if(iterLog->second)
		{
			//delete iterLog->second;
			//iterLog->second = NULL;
			(iterLog->second)->m_writeDebug = false;
			(iterLog->second)->CloseLog();
		}
	}
	//theInstance.mapLog.clear();
};