#include "StdAfx.h"
#include <windows.h> 
#include <stdio.h> 
#include "UC_LOG.h"

const char CrLf[3]	   = {13,10,0};

//TH��־��ӡ��ʵ��

UC_LOG::UC_LOG ()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_dwLineCount = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��TH��־�ļ�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::OpenLog() 
{
    char strFullName[255] = {0};
	// ȡ����־�ļ���
    MakeFileName(m_strFileName,sizeof(strFullName));
	// ȡ����־�ļ�ȫ·��
    MakeFullName(strFullName,sizeof(strFullName));

    //������־�ļ�
    m_hFile = ::CreateFileA(strFullName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��TH��־�ļ�

@param      (i)const char* tracePath  ��־����Ŀ¼�ַ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::Initialize(const char* tracePath) 
{
	// ������־���Ŀ¼
    strcpy_s(m_strTracePath, tracePath);

	// ��ʼ���ٽ�������
	::InitializeCriticalSection(&m_hLock);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
UC_LOG::~UC_LOG ()
{
	// �ر���־�ļ�
	CloseLog();
	// ɾ���ٽ�������
    ::DeleteCriticalSection(&m_hLock);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �ر���־�ļ�

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
@brief     ��ʽ�����ݲ�д����־�ļ�

@param     (i)const char *fmt

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::printf(const char *fmt, ...)
{
	// ��ʽ���ַ���
	char buffer[1024*50];
	va_list arglist;
	va_start(arglist,fmt);
	vsprintf_s(buffer,fmt,arglist);
	va_end(arglist);

	print(buffer);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��16���Ƶ���ʽ��buffer���ݰ��ֽ�д����־��

@param     (i)const BYTE* btContent Ҫд����ڴ���
@param     (i)const UINT uilen �ڴ�����С

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
#define _T_UD_SIZE 1024*50
void UC_LOG::printhex(const BYTE* btContent,const UINT uilen)
{
	// ��ʽ���ַ���
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
@brief     д����־����

@param     (i)const char *text ��־����

@retval    none

@exception none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::print(const char *text)
{
	// �����ٽ���
	::EnterCriticalSection( &m_hLock );

    // �ļ�δ�򿪣����ȴ�
    if (m_hFile == INVALID_HANDLE_VALUE) {
        OpenLog();
    }

	// ��ʽ��ʱ��	
	char  timebuf[256];
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf_s(timebuf, "[%02d/%02d/%02d %02d:%02d:%02d.%03d]",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

	// ȡ����־�ļ���
    char szName[256];
    MakeFileName(szName,sizeof(szName));
	// ��־���������뵱ǰ���ڲ�ͬ�򴴽��µ���־�ļ�
    if (strcmp(m_strFileName, szName)) {
        CloseLog();
        OpenLog();
    }

	DWORD x;
	// �ҵ��ļ�ĩβ
	SetFilePointer(m_hFile,0,0,FILE_END);
	// д��ʱ��
	WriteFile(m_hFile,timebuf, lstrlenA(timebuf),&x,0);	
	// д����־����
	WriteFile(m_hFile,text, lstrlenA(text), &x,0);	
	// ����
	WriteFile(m_hFile,CrLf, lstrlenA(CrLf), &x,0);	

	// �뿪�ٽ���
	::LeaveCriticalSection( &m_hLock );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����־�ļ�

@param      none        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void UC_LOG::ClearLog() 
{
    // �ļ�δ��
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return;
    }

	// �����־�ļ�
    char strFullName[255] = {0};
    strcpy_s(strFullName, m_strTracePath);
	strcat_s(strFullName,sizeof(strFullName),m_strFileName);
    m_hFile = ::CreateFileA(strFullName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��TH��־ȫ·�����ļ���

@param      (o)char* strFullName      TH��־ȫ·�����ļ����ַ���        

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
@brief      ȡ��TH��־�ļ���

@param      (o)char* fileName          �ļ����ַ���        

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