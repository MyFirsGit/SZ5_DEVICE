#include "StdAfx.h"
#include "Control.h"
#include "CoreCommon.h"
#include "RequestHandler.h"
#include "SysException.h"
#include "ForeService.h"
#include "InnerException.h"
#include "DebugLog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

const static int PAUSE_TIME = 10;           // 等待命令响应时，暂停时间（毫秒）

CControl::CControl(void)
{
    //m_pCRequestManager = NULL;
	m_pRequestHandler = NULL;
}

CControl::~CControl(void)
{

}

/*********************************************************************************	
/**
*   @brief Initialize.
*	@param None.
*	@return SP_SUCCESS if successful.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
long CControl::Initialize()
{
  
    //m_iTimer = iIdleTimer;
	m_pRequestHandler = (CRequestHandler*)CUtilThread::StartThread(RUNTIME_CLASS(CRequestHandler), "CRequestHandler");
	if(m_pRequestHandler == NULL)
	{
		return SP_ERR_INTERNAL_ERROR;
	}
	return SP_SUCCESS;
}

/*********************************************************************************	
/**
*   @brief UnInitialize.
*	@param None.
*	@return SP_SUCCESS if successful.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
long CControl::Shutdown()
{
    long hResult = SP_SUCCESS;
	if(m_pRequestHandler!=NULL)
	{
		    if (CUtilThread::ShutdownThread(m_pRequestHandler,3*1000) != 0) {
		        hResult = SP_ERR_INTERNAL_ERROR;
		    }
		    m_pRequestHandler = NULL;
	}
    return hResult;
}

/*********************************************************************************	
/**
*   @brief Idle sensing.
*	@param None.
*	@return SP_SUCCESS if successful.
*	@date 11/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
//long CControl::DoIdleSense(void)
//{
//    return SP_SUCCESS;
//}

/************************************************************************************
/*
*	@brief Send the request to CRequestManager to process the request from 
*	application.
*	@param pRequestItem: The request item will be sent to CRequestMannger
*	@param dwManagerReqID: The process event ID that will be sent to CRequestMannger
*	@return 
*		- SP_SUCCESS: Success
*		- SP_ERR_INTERNAL_ERROR: There is unknow internal error.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/***********************************************************************************/
HRESULT CControl::DoCommand(CCommand *pRequestItem, long lExecuteMode)
{
    long hResult = BeforeDoCommand.Invoke(pRequestItem);
	if(hResult!=SP_SUCCESS)
	{
		return hResult;
	}
    if (pRequestItem == NULL) {
        return SP_ERR_INTERNAL_ERROR;
    }
	if(lExecuteMode == SP_EXECMODE_SYNC)
	{
		return pRequestItem->Execute();
	}
    if (m_pRequestHandler== NULL || ( !m_pRequestHandler->PostThreadMessage(REQUEST_QUEUE, 0, (LPARAM)pRequestItem )) ){
        hResult = SP_ERR_INTERNAL_ERROR;
    }
    return hResult;
}

/*********************************************************************************	
/**
*   @brief Set Idle Timer.
*	@param iTimer : IdleSense Timer ( seconds unit ).
*	@return None.
*	@date 11/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
//void CControl::SetIdleTimer(int iTimer)
//{
//    m_iTimer = iTimer;
//}

/*********************************************************************************	
/**
*   @brief Get Idle Timer.
*	@param None.
*	@return Timer of IdleSense ( seconds unit ).
*	@date 11/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
//int CControl::GetIdleTimer()
//{
//    return m_iTimer;
//}

/*********************************************************************************	
/**
*   @brief Set IdleMode.
*	@param None.
*	@return None.
*	@date 11/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
//void CControl::SetIdleMode(bool bIdleMode)
//{
//    m_pCRequestManager->SetIdleMode(bIdleMode);
//
//    if (TRUE == bIdleMode) {
//        m_pCRequestManager->SetController(this);
//    }
//    else {
//        m_pCRequestManager->SetController(NULL);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置是否暂停RequestManager的MainRequestHandler

@param      (i)bool isPause     true:暂停, false:继续

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CControl::SetPause(bool isPause)
{
    //m_pCRequestManager->SetPause(isPause);
	m_pRequestHandler->SetPause(isPause);
}


