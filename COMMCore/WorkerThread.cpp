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
@brief	Service��ʹ�õ��߳���                                                                   
*/

IMPLEMENT_DYNCREATE(CWorkerThread, CUtilThread)

BEGIN_MESSAGE_MAP(CWorkerThread, CUtilThread)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWorkerThread::CWorkerThread(void)
:m_worker(NULL)
,m_Interval(1)
{
}


//////////////////////////////////////////////////////////////////////////
/*
@brief    ��������  

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
@brief     ���ù����� 

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
@brief   ���ü��ʱ��:��λ�����룩   

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