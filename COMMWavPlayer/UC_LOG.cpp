#include "StdAfx.h"
#include <windows.h> 
#include <stdio.h> 
#include "UC_LOG.h"

const char CrLf[3]	   = {13,10,0};

//TH日志打印类实现

UC_LOG::UC_LOG ()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_dwLineCount = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打开TH日志文件

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::OpenLog() 
{
    char strFullName[255] = {0};
	// 取得日志文件名
    MakeFileName(m_strFileName,sizeof(strFullName));
	// 取得日志文件全路径
    MakeFullName(strFullName,sizeof(strFullName));

    //创建日志文件
    m_hFile = ::CreateFileA(strFullName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化TH日志文件

@param      (i)const char* tracePath  日志保存目录字符串

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::Initialize(const char* tracePath) 
{
	// 保存日志存放目录
    strcpy_s(m_strTracePath, tracePath);

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
UC_LOG::~UC_LOG ()
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
void UC_LOG::CloseLog ()
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
void UC_LOG::printf(const char *fmt, ...)
{
	// 格式化字符串
	char buffer[1024*50];
	va_list arglist;
	va_start(arglist,fmt);
	vsprintf_s(buffer,fmt,arglist);
	va_end(arglist);

	print(buffer);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     以16进制的形式将buffer内容按字节写到日志中

@param     (i)const BYTE* btContent 要写入的内存区
@param     (i)const UINT uilen 内存区大小

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
#define _T_UD_SIZE 1024*50
void UC_LOG::printhex(const BYTE* btContent,const UINT uilen)
{
	// 格式化字符串
	char buffer[_T_UD_SIZE+2];	
	memset(buffer,0,_T_UD_SIZE+2);
	
	UINT uiloop=0;
	_T_UD_SIZE/3>uilen?uiloop=uilen:uiloop=_T_UD_SIZE/3;
    for (UINT i = 0; i < uiloop; i++)
    {
        sprintf_s(buffer+ i * 3, sizeof(buffer+ i * 3),"%02x ", *(btContent+i) & 0xFF);
    }
	if(uiloop<uilen)
	{
		buffer[_T_UD_SIZE]='>';
		buffer[_T_UD_SIZE+1]='\0';
	}else
	{
		//buffer[uiloop]='\0';
	}
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
void UC_LOG::print(const char *text)
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
	sprintf_s(timebuf, "[%02d/%02d/%02d %02d:%02d:%02d.%03d]",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

	// 取得日志文件名
    char szName[256];
    MakeFileName(szName,sizeof(szName));
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
void UC_LOG::ClearLog() 
{
    // 文件未打开
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return;
    }

	// 清空日志文件
    char strFullName[255] = {0};
    strcpy_s(strFullName, m_strTracePath);
	strcat_s(strFullName,sizeof(strFullName),m_strFileName);
    m_hFile = ::CreateFileA(strFullName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得TH日志全路径及文件名

@param      (o)char* strFullName      TH日志全路径及文件名字符串        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::MakeFullName(char* strFullName,const UINT uisize) 
{
    strcpy_s(strFullName, uisize,m_strTracePath);
    strcat_s(strFullName, uisize,m_strFileName);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得TH日志文件名

@param      (o)char* fileName          文件名字符串        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::MakeFileName(char* fileName,const UINT uisize) 
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf_s(fileName, uisize,"WAVPLAYER_%04d%02d%02d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}