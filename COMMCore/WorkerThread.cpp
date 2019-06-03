#include "StdAfx.h"
#include "WorkerThread.h"
#include "coreapi.h"

#include "InnerException.h"
#include "Command.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	Service中使用的线程类                                                                   
*/

IMPLEMENT_DYNCREATE(CWorkerThread, CUtilThread)

BEGIN_MESSAGE_MAP(CWorkerThread, CUtilThread)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWorkerThread::CWorkerThread(void)
:m_worker(NULL)
,m_Interval(1)
{
}


//////////////////////////////////////////////////////////////////////////
/*
@brief    析构函数  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CWorkerThread::~CWorkerThread()
{
	
}


//////////////////////////////////////////////////////////////////////////
/*
@brief     设置工作项 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CWorkerThread::SetWorker(CCommand* worker)
{
	m_worker = worker;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   设置间隔时间:单位（毫秒）   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CWorkerThread::SetInterval(UINT interval)
{
	ASSERT(interval>=1);
	m_Interval = interval;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CWorkerThread::OnIdle(LONG lCount)
{
	Sleep(m_Interval);
	if(m_worker!=NULL)
	{
		m_worker->Execute();
	}
	return TRUE;
}