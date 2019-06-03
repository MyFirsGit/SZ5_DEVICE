#include <windows.h> 
#include <stdio.h> 
#include "CHLog.h"

const char CrLf[3]	   = {13,10,0};

//CH��־��ӡ��ʵ��

CCHLog::CCHLog ()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_dwLineCount = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��CH��־�ļ�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCHLog::OpenLog() 
{
    char strFullName[255] = {0};
	// ȡ����־�ļ���
    MakeFileName(m_strFileName);
	// ȡ����־�ļ�ȫ·��
    MakeFullName(strFullName);

    //������־�ļ�
    m_hFile = ::CreateFileA(strFullName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��CH��־�ļ�

@param      (i)const char* tracePath  ��־����Ŀ¼�ַ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCHLog::Initialize(const char* tracePath) 
{
	// ������־���Ŀ¼
    strcpy(m_strTracePath, tracePath);

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
CCHLog::~CCHLog ()
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
void CCHLog::CloseLog ()
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
void CCHLog::printf(const char *fmt, ...)
{
	// ��ʽ���ַ���
	char buffer[1024*50];
	va_list arglist;
	va_start(arglist,fmt);
	vsprintf(buffer,fmt,arglist);
	va_end(arglist);

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
void CCHLog::print(const char *text)
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
	sprintf(timebuf, "%02d/%02d/%02d %02d:%02d:%02d.%03d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);

	// ȡ����־�ļ���
    char szName[256];
    MakeFileName(szName);
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
void CCHLog::ClearLog() 
{
    // �ļ�δ��
    if (m_hFile == INVALID_HANDLE_VALUE) {
        return;
    }

	// �����־�ļ�
    char strFullName[255] = {0};
    strcpy(strFullName, m_strTracePath);
    m_hFile = ::CreateFileA(strcat(strFullName,m_strFileName), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��CH��־ȫ·�����ļ���

@param      (o)char* strFullName      CH��־ȫ·�����ļ����ַ���        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCHLog::MakeFullName(char* strFullName) 
{
    strcpy(strFullName, m_strTracePath);
    strcat(strFullName, m_strFileName);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��CH��־�ļ���

@param      (o)char* fileName          �ļ����ַ���        

@retval     none       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCHLog::MakeFileName(char* fileName) 
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf(fileName, "CHTrace_%04d%02d%02d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}