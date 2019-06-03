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
@brief      ������־ǰ׺����ȡ��һ��д��־�Ķ���

@param      CString logPrefix ��־ǰ׺

@retval     CFileLog* д��־�Ķ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFileLog* CFileLog::GetInstance(CString logPrefix)
{
	if(theInstance.mapLog[logPrefix]== NULL)//û���ҵ������¼�һ����
	{
		CFileLog*	log = new CFileLog(logPrefix);
		theInstance.mapLog[logPrefix] = log;
	}
	return theInstance.mapLog[logPrefix];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFileLog::CFileLog(CString logPrefix)
:CLog(logPrefix)
{
	m_writeDebug = true;                               // �Ƿ��¼Debug��Ϣ
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief       �����Ƿ��¼Debug��Ϣ

@param       (i)bool writeDebug

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CFileLog::SetWriteDebug(bool writeDebug)
{
	m_writeDebug = writeDebug;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д��־

@param      (i)CString className                       
@param      (i)CString methodName                       
@param      (i)int line                   
@param      (i)CString format                       

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CFileLog::WriteData(CString className, CString methodName, int line, CString format, ...)
{
	try {
		// ϵͳINI�������ò���¼Debug��־����ֱ�ӷ���
		if (! m_writeDebug) {
			return;
		}

		// �̶�����
		CString head;
		head.Format(_T("%-14s %-22s %4d "), className, methodName, line);

		// ���Ӳ���
		CString data;
		va_list args;
		va_start(args, format);
		data.FormatV(format, args);
		va_end(args);

		// �����ļ�
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
		// ϵͳINI�������ò���¼Debug��־����ֱ�ӷ���
		if (! m_writeDebug) {
			return;
		}

		CString data=_T("");
		CString sHex = _T("");
		CString strLine = _T("");
		strLine.Format(_T("��%d��"), line);
		for(unsigned int i=0;i<length;i++){
			sHex.Format(format,*(lpData+i));
			data+=sHex;
		}

		// �����ļ�
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
		// ϵͳINI�������ò���¼Debug��־����ֱ�ӷ���
		if (! m_writeDebug) {
			return;
		}

		CString data=_T("");
		CString sHex = _T("");
		for(unsigned int i=0;i<length;i++){
			sHex.Format(format,*(lpData+i));
			data+=sHex;
		}

		// �����ļ�
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
@brief      �ͷ�Debug��־

@param      ��

@retval     ��

@exception  ��
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