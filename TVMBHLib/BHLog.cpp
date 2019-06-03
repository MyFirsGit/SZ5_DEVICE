#include <windows.h> 
#include <stdio.h> 
#include "BHLog.h"

const char CrLf[3]	   = {13,10,0};

//BH日志打印类实现

CBHLog::CBHLog ()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_dwLineCount = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打开BH日志文件

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::OpenLog() 
{
    char strFullName[255] = {0};
	// 取得日志文件名
    MakeFileName(m_strFileName);
	// 取得日志文件全路径
    MakeFullName(strFullName);

    //创建日志文件
    m_hFile = ::CreateFileA(strFullName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BH日志文件

@param      (i)const char* tracePath  日志保存目录字符串

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::Initialize(const char* tracePath) 
{
	// 保存日志存放目录
    strcpy(m_strTracePath, tracePath);

	// 初始化临界区对象
	::InitializeCriticalSection(&m_hLock);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBHLog::~CBHLog ()
{
	// 关闭日志文件
	CloseLog();
	// 删除临界区对象
    ::DeleteCriticalSection(&m_hLock);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     关闭日志文件

@param     none

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::CloseLog ()
{	
	if (m_hFile!=INVALID_HANDLE_VALUE){
		::CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     格式化数据并写入日志文件

@param     (i)const char *fmt

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::printf(const char *fmt, ...)
{
	// 格式化字符串
	char buffer[1024*50];
	va_list arglist;
	va_start(arglist,fmt);
	vsprintf(buffer,fmt,arglist);
	va_end(arglist);

	print(buffer);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     写入日志数据

@param     (i)const char *text 日志数据

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::print(const char *text)
{
	// 进入临界区
	::EnterCriticalSection( &m_hLock );

    // 文件未打开，则先打开
    if (m_hFile == INVALID_HANDLE_VALUE) {
        OpenLog();
    }

	// 格式化时间	
	char  timebuf[256];
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(timebuf, "%02d/%02d/%02d %02d:%02d:%02d.%03d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

	// 取得日志文件名
    char szName[256];
    MakeFileName(szName);
	// 日志创建日期与当前日期不同则创建新的日志文件
    if (strcmp(m_strFileName, szName)) {
        CloseLog();
        OpenLog();
    }

	DWORD x;
	// 找到文件末尾
	SetFilePointer(m_hFile,0,0,FILE_END);
	// 写入时间
	WriteFile(m_hFile,timebuf, lstrlenA(timebuf),&x,0);	
	// 写入日志内容
	WriteFile(m_hFile,text, lstrlenA(text), &x,0);	
	// 换行
	WriteFile(m_hFile,CrLf, lstrlenA(CrLf), &x,0);	

	// 离开临界区
	::LeaveCriticalSection( &m_hLock );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清除日志文件

@param      none        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::ClearLog() 
{
    // 文件未打开
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return;
    }

	// 清空日志文件
    char strFullName[255] = {0};
    strcpy(strFullName, m_strTracePath);
    m_hFile = ::CreateFileA(strcat(strFullName,m_strFileName), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得BH日志全路径及文件名

@param      (o)char* strFullName      BH日志全路径及文件名字符串        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::MakeFullName(char* strFullName) 
{
    strcpy(strFullName, m_strTracePath);
    strcat(strFullName, m_strFileName);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得BH日志文件名

@param      (o)char* fileName          文件名字符串        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBHLog::MakeFileName(char* fileName) 
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf(fileName, "BHTrace_%04d%02d%02d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}
