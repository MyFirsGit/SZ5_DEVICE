#include "StdAfx.h"
#include "BackService.h"
#include "DebugLog.h"
#include "CoreAPI.h"
#include "timermgr.h"

/**
@brief	后台服务基类
*/

BEGIN_MESSAGE_MAP(CBackService, CService)
	ON_SERVICE_MESSAGE(ST_START_TIMER,OnStartTimer)
	ON_SERVICE_MESSAGE(ST_STOP_TIMER,OnStopTimer)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      UINT serviceID            Service ID
@param      UINT parentServiceID      上级Service ID
@param      bool hasThread                  是否包含线程（在后台运行）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBackService::CBackService(UINT serviceID, UINT parentServiceID, bool hasThread)
    :CService(serviceID, parentServiceID, true)
{
    m_pServiceThread = NULL;                            // Service线程

    // 若在后台运行，则创建Service线程
    if (hasThread) {
        m_pServiceThread = (CServiceThread*) CUtilThread::StartThread(RUNTIME_CLASS(CServiceThread), "CServiceThread");
        m_pServiceThread->SetService(this);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBackService::~CBackService()
{
    // 若在后台运行，则关闭Service线程
    if (m_pServiceThread != NULL) {
		// 关机序列中的4个常驻service追加此log
		CUtilThread::ShutdownThread(m_pServiceThread,3*1000);
        m_pServiceThread = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service停止前处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CBackService::OnStop()
{
    MSG msg;

    // 等待后台线程空闲
    while (m_pServiceThread != NULL && m_pServiceThread->IsBusy()) {
        while (m_pServiceThread->IsBusy() && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            ::DispatchMessage(&msg);
        }
    }

    return true;
}


CServiceThread* CBackService::GetServiceThread()
{
	return m_pServiceThread;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送消息

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBackService::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if(m_pServiceThread!=NULL)
	{
		return m_pServiceThread->SendThreadServiceMsg(Msg,wParam,lParam);
	}
	return SP_ERR_INTERNAL_ERROR;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      邮寄消息

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CBackService::PostMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	// 若Service包含线程, 则邮寄到线程
	if (m_pServiceThread != NULL) {
		return m_pServiceThread->PostThreadServiceMsg(message,wParam,lParam);
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动定时器

@param      (i)UINT timerID 
@param      (i)int seconds  等待时间(秒)

@retval     bool true:成功 false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CBackService::StartTimer(UINT timerID, int seconds)
{
	PostMessage(ST_START_TIMER,timerID,seconds);
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      停止定时器

@param      (i)UINT timerID 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBackService::StopTimer(UINT timerID)
{
	PostMessage(ST_STOP_TIMER,timerID,NULL);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开启计时器消息响应函数（计时器在当前服务的Thread中开启）

@param     WPARAM wParam TimerID
@param	LPARAM lParam Interval

@retval     LRESULT

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBackService::OnStartTimer(WPARAM wParam,LPARAM lParam)
{
	__super::StartTimer(wParam, lParam);
	return TRUE;
}


/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBackService::OnStopTimer(WPARAM wParam,LPARAM lParam)
{
	__super::StopTimer(wParam);
	return TRUE;
}