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
@brief      CLoggerThread构造函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CLogThread::CLogThread()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CLoggerThread析构函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CLogThread::~CLogThread()
{
	m_pRequestList.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化
@param      无
@retval     BOOL FALSE 失败  TRUE 成功
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLogThread::InitInstance()
{
	return __super::InitInstance();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      结束
@param      无
@retval     BOOL FALSE 失败  TRUE 成功
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
@brief      加入消息队列
@param      WPARAM wp 
LPARAM lp   消息对象
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CLogThread::OnRequestQueue(WPARAM wp, LPARAM lp)
{
	m_pRequestList.push_back((pair<CLog*,CString>*)lp);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      空闲时间
@param      long lCount 
@retval     BOOL   
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLogThread::OnIdle(long lCount)
{
	Sleep(1);
	// 如果消息队列为空
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

