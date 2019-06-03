#include "StdAfx.h"
#include "Command.h"
#include "DebugLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCommand::CCommand(void)
{
	m_bCompleted = FALSE;
	m_bAutoDeleted = FALSE;
	m_lCompletedCode = SP_SUCCESS;
	m_ulCommandID = 0;
	m_ulTimeOut = 0;
    m_Service = NULL;
	m_Bag = new CBag();
	m_Handle = INVALID_HANDLE_VALUE;
	m_bNeedCallbackService = false;
}

CCommand::~CCommand(void)
{
	if(m_Handle!=NULL && m_Handle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}

	if(NULL != m_Bag){
		delete m_Bag;
		m_Bag = NULL;
	}

}


/*********************************************************************************	
/**
*   @brief Get command TimeOut.
*	@param None.
*	@return Command TimeOut value.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
unsigned long CCommand::GetTimeOut()
{
	return m_ulTimeOut;
}

/*********************************************************************************	
/**
*   @brief Set command TimeOut.
*	@param ulTimeOut : command TimeOut input.
*	@return None.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
void CCommand::SetTimeOut(unsigned long ulTimeOut)
{
	m_ulTimeOut = ulTimeOut;
}


/*********************************************************************************	
/**
*   @brief Check timeout.
*	@param ulStartTime : start time.
*	@return where is timeout.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
bool CCommand::IsTimeOut(unsigned long ulStartTime)
{
    //Check timeout
    DWORD dwDuration = 1000 * this->GetTimeOut(); //time for waiting
    DWORD dwCurTime = ::GetTickCount();

    if ((dwDuration > 0) && (dwCurTime > ulStartTime + dwDuration)) {
        return true;
    }
    else {
        return false;
    }
}

/*********************************************************************************	
/**
*   @brief Core execution function.
*	@param None.
*	@return Return code.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
long CCommand::Execute()
{
	long lResult = SP_SUCCESS ;
	
	if (IsValidCommandID() == FALSE)
	{
		return SP_ERR_INVALID_COMMANDID;
	}

	lResult = IsValidCommand();
	
	if ( lResult == SP_SUCCESS )
	{
		Executing.Invoke(this);
		lResult = ExecuteCommand();
	}
	return lResult;
}

void CCommand::SetNeedCallbackService(bool bNeedCallbackService)
{
	this->m_bNeedCallbackService = bNeedCallbackService;
}

bool CCommand::GetNeedCallbackService()
{
	return m_bNeedCallbackService;
}

/*********************************************************************************	
/**
*   @brief Fire complete signal. Call this function to set command completed sign.
*	@param lCompleteCode : return code.
*	@return None.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
void CCommand::OnComplete(long lCompleteCode)
{
	Completing.Invoke(this);
	SetCompleted(lCompleteCode);
	Completed.Invoke(this);
	if(m_Handle!=NULL && m_Handle!=INVALID_HANDLE_VALUE){
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("SetEvent :%d") ,(long)this);
		SetEvent(m_Handle);
	}
	else if(m_bNeedCallbackService && m_Service!=NULL)
	{
		m_Service->PostMessage(SP_REQUEST_COMPLETE,lCompleteCode,(LPARAM)this);
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("hasPost"));
	}
	else if(m_bAutoDeleted)
	{
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("Delete this :%d"),(long)this);
		delete this;
	}
	return;
}

/*********************************************************************************	
/**
*   @brief Set completed flag.
*	@param lCompleteCode : command return code.
*	@return None.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
void CCommand::SetCompleted(long lCompleteCode)
{
	m_lCompletedCode = lCompleteCode;
	m_bCompleted = TRUE;
}

/*********************************************************************************	
/**
*   @brief Get command return code.
*	@param none.
*	@return Return code.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
long CCommand::GetResultCode()
{
	return m_lCompletedCode;
}

/*********************************************************************************	
/**
*   @brief Check command completed status.
*	@param none.
*	@return TRUE if command execution completed.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
BOOL CCommand::IsCompleted()
{
	return m_bCompleted;
}


BOOL CCommand::IsValidCommandID()
{
	if ( m_ulCommandID < 0 )
		return FALSE;
	else 
		return TRUE;
}

/*********************************************************************************	
/**
*   @brief Set command ID.
*	@param ulCmdID : command ID.
*	@return None.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
void CCommand::SetCommandID(unsigned long ulCmdID)
{
	m_ulCommandID = ulCmdID;
}

/*********************************************************************************	
/**
*   @brief Set auto-deleted command.
*	@param bAutoDeleted : auto-deleted flag.
*	@return None.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
void CCommand::SetAutoDeleted(bool bAutoDeleted)
{
	m_bAutoDeleted = bAutoDeleted;
}

/*********************************************************************************	
/**
*   @brief Get auto-deleted command flag.
*	@param None.
*	@return auto-deleted flag.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
bool CCommand::GetAutoDeleted()
{
	return m_bAutoDeleted;
}

/*********************************************************************************	
/**
*   @brief Check input-data.
*	@param None.
*	@return auto-deleted flag.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/

long CCommand::IsValidCommand(void)
{
	return SP_SUCCESS;
}

/*********************************************************************************	
/**
*   @brief Get ID of this command.
*	@param None.
*	@return ID of this command.
*	@date 10/2004
*	@remarks (C) Vu Huy.
*/
/**********************************************************************************/
unsigned long CCommand::GetCommandID(void)
{
	return m_ulCommandID;
}

/*********************************************************************************	
/**
*   @brief Set Service ID.
*	@param serviceID : Service ID input.
*	@return None.
*	@date 01/2006
*	@remarks (C) Li Weifeng
*/
/**********************************************************************************/
void CCommand::SetService(CService* pService)
{
    m_Service = pService;
}

/*********************************************************************************	
/**
*   @brief Get Service ID.
*	@param None.
*	@return Service ID.
*	@date 01/2006
*	@remarks (C) Li Weifeng
*/
/**********************************************************************************/
CService* CCommand::GetService()
{
    return m_Service;
}

/*********************************************************************************	
/**
*   @brief Get a bag
*	@param None.
*	@return CBag*
*	@date 01/2006
*	@remarks (C) Li Weifeng
*/
/**********************************************************************************/
CBag* CCommand::GetBag()
{
	return m_Bag;
}


void CCommand::EnsureHandle()
{
	if(m_Handle == NULL || m_Handle== INVALID_HANDLE_VALUE)
	{
		m_Handle = CreateEvent(NULL,TRUE,FALSE,NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      等待指定的命令处理结束

@param      (i)CCommand* command     被等待的命令

@retval     long 命令响应的ErrCode

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long WaitForCommandComplete(CCommand* pRequestItem,DWORD waitForSeconds)
{
	if(pRequestItem == NULL)
	{
		return SP_ERR_INVALID_COMMANDID;
	}
	if(pRequestItem->IsCompleted())
	{
		return pRequestItem->GetResultCode();
	}
	pRequestItem->EnsureHandle();
	DWORD waitResult = WaitForSingleObject(pRequestItem->m_Handle,waitForSeconds == INFINITE ? waitForSeconds : waitForSeconds*1000);
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("ID=%d Result =%d"),(long)pRequestItem,waitResult);
	if(waitResult ==WAIT_OBJECT_0)
	{
		return pRequestItem->GetResultCode();
	}
	else
	{
		pRequestItem->OnComplete(SP_ERR_TIMEOUT);
	}
	return SP_ERR_TIMEOUT;
}