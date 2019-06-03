#include "StdAfx.h"
#include "AFCLogApi.h"
const char CrLf[3]	   = {13,10,0};

CAFCLogApi CAFCLogApi::theInstance;

CAFCLogApi::CAFCLogApi(void)
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_strFileName = _T("");
	m_strTracePath = _T("");
}


CAFCLogApi::~CAFCLogApi(void)
{
	CloseLog();
	::DeleteCriticalSection(&m_hLock);
}

void CAFCLogApi::MakeFileName()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	m_strFileName.Format(_T("%s%04d%02d%02d.log"),m_strFileName,sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}


// 要写日志，必须先调用Initialize对api初始化
void CAFCLogApi::Initialize(CString path,CString filename)
{
	::InitializeCriticalSection(&m_hLock);
	m_strTracePath = path;
	m_strFileName = filename;
	MakeFileName();
	CString strFullName = _T("");
	MakeFullName(strFullName);
	
	m_hFile = ::CreateFile(strFullName.GetBuffer(strFullName.GetLength()), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	strFullName.ReleaseBuffer();
}


// 需要关闭日志，否则无法上传
void CAFCLogApi::CloseLog(void)
{
	if (m_hFile!=INVALID_HANDLE_VALUE){
		::CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}
void CAFCLogApi::MakeFullName(CString& strFullName)
{
	if (m_strTracePath.Right(1) != _T("\\") && m_strTracePath.Right(1) != _T("//"))
	{
		m_strTracePath += _T("\\");
	}
	CheckAndCreatePath();
	strFullName = m_strTracePath + m_strFileName;
}
// 写日志
void CAFCLogApi::WriteLog(CString className, CString methodName, int line, CString format, ...)
{
	try
	{
		// 固定头
		CString head = _T("");
		head.Format(_T("%-s %-s %d		"), className, methodName, line);

		// 日志内容
		CString data = _T("");
		va_list args;
		va_start(args, format);
		data.FormatV(format, args);
		va_end(args);
		WriteData(head + data + _T("\r\n"));
	}
	catch (...)
	{
	}
}
// 写日志
void CAFCLogApi::WriteData(CString& data)
{
	// 进入临界区
	::EnterCriticalSection(&m_hLock);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		CString strFullName = _T("");
		MakeFullName(strFullName);

		m_hFile = ::CreateFile(strFullName.GetBuffer(strFullName.GetLength()), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		strFullName.ReleaseBuffer();
	}
	// 操作时间
	SYSTEMTIME dtCurTime;
	::GetLocalTime(&dtCurTime);
	CString strPrefix = _T("");
	strPrefix.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d.%.3d   threadID %d"),dtCurTime.wYear,dtCurTime.wMonth,dtCurTime.wDay,
		dtCurTime.wHour,dtCurTime.wMinute,dtCurTime.wSecond,dtCurTime.wMilliseconds,GetCurrentThreadId());
	CString logData = strPrefix + _T(",") + data;
	DWORD x;
	// 找到文件末尾
	SetFilePointer(m_hFile,0,0,FILE_END);
	// 写入日志内容
	char *buf = NULL;
	USES_CONVERSION;
	buf = T2A(logData);
	WriteFile(m_hFile,buf, lstrlenA(buf), &x,0);	
	// 换行
	WriteFile(m_hFile,CrLf, lstrlenA(CrLf), &x,0);	

	// 离开临界区
	::LeaveCriticalSection( &m_hLock );

}
void CAFCLogApi::CheckAndCreatePath()
{
	if (!m_strTracePath.IsEmpty())
	{
		CStringArray pathArray;
		int nIndex = -1,lastIndex = 0;
		nIndex = m_strTracePath.Find(_T("\\"));
		while(nIndex != -1)
		{
			pathArray.Add(m_strTracePath.Mid(lastIndex,nIndex+1));
			lastIndex = nIndex+1;
			nIndex = m_strTracePath.Find(_T("\\"),lastIndex);
		}
		CString path = _T("");
		while(pathArray.GetSize()>0)
		{
			path += pathArray.GetAt(0);
			if (!PathIsDirectory(path))
			{
				::CreateDirectory(path, NULL);
			}
			pathArray.RemoveAt(0);
		}
	}
}
