#include "stdafx.h"
#include "LogThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CLogThread, CUtilThread)

BEGIN_MESSAGE_MAP(CLogThread, CUtilThread)
	ON_THREAD_MESSAGE(WM_USER_LOG_ENQUEUE,OnRequestQueue)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      CLoggerThread���캯��
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CLogThread::CLogThread()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CLoggerThread��������
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CLogThread::~CLogThread()
{
	m_pRequestList.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��
@param      ��
@retval     BOOL FALSE ʧ��  TRUE �ɹ�
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLogThread::InitInstance()
{
	return __super::InitInstance();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����
@param      ��
@retval     BOOL FALSE ʧ��  TRUE �ɹ�
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLogThread::ExitInstance()
{
	while(!m_pRequestList.empty())
	{
		try
		{
			pair<CLog*,CString>* writeLogTask = m_pRequestList.front();
			writeLogTask->first->WriteDataImp(writeLogTask->second);
			m_pRequestList.pop_front();
			delete writeLogTask;
		}
		catch (...) {
		}
	}
	return __super::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����
@param      WPARAM wp 
LPARAM lp   ��Ϣ����
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CLogThread::OnRequestQueue(WPARAM wp, LPARAM lp)
{
	m_pRequestList.push_back((pair<CLog*,CString>*)lp);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ʱ��
@param      long lCount 
@retval     BOOL   
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLogThread::OnIdle(long lCount)
{
	Sleep(1);
	// �����Ϣ����Ϊ��
	if (m_pRequestList.empty())
	{
		return TRUE;
	}
	try
	{
		pair<CLog*,CString>* writeLogTask = m_pRequestList.front();
		writeLogTask->first->WriteDataImp(writeLogTask->second);
		m_pRequestList.pop_front();
		delete writeLogTask;
	}
	catch (...) {
	}

	return TRUE;
}

