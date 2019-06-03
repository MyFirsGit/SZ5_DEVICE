#include "stdafx.h"
#include "SCSvcDef.h"
#include "ConnectSequence.h"
#include "Validation.h"

CConnectSequence::CConnectSequence():CSCSequence()
{
	parameterSyncSequence = new CParameterSyncSequence;
	m_status = INIT;
}

CConnectSequence::~CConnectSequence()
{
	delete parameterSyncSequence;
	parameterSyncSequence = NULL;
}

bool CConnectSequence::CanAcceptSCCommand(CSCCommand* command)
{
	if(m_status == INIT && command->GetHeader().nwTransId.bCmdID == CMD_CONNECT_REQUEST)
	{
		return true;
	}
	if(m_status == AUTHED)
	{
		return parameterSyncSequence->CanAcceptSCCommand(command);
	}
	WORD msgcode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	if(m_status == PARAMETERSYNCED &&  theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command)==MSG_BOM_ALL_STATUS_DATA)
	{
		return true;
	}
	if(m_status == MODEUPLOADED)
	{
		WORD availableMsgCode[] = {MSG_BOM_STATUS_CHANGE_DATA,MSG_OPERATION_DATA,MSG_24HOUR_OPERATION_DATA,MSG_DELAY_OPERATION_DATA,MSG_OP_END_NOTIC};
		if(!CValidation::IsInRange(msgcode,availableMsgCode))
		{
			return false;
		}
		if(msgcode == MSG_OP_END_NOTIC)
		{
			LPBYTE content = command->GetContent()->lpData+2;
			WORD noticeType = MAKEWORD(*content,*(content+1));
			if(noticeType != 0x0004)//上位同步数据完成
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void CConnectSequence::OnSCCommandComplete(CSCCommand* command)
{
	if(m_status == INIT && command->GetHeader().nwTransId.bCmdID == CMD_CONNECT_REQUEST)
	{
		m_status = AUTHED;
		return;
	}
	if(m_status == AUTHED )
	{
		parameterSyncSequence->OnSCCommandComplete(command);
		if(parameterSyncSequence->IsCompleted())
		{
			m_status = PARAMETERSYNCED;
			theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_FULLSTATUS,STATUS_TYPE_THREESTATUS,NULL);
		}
		return;
	}
	if(m_status == PARAMETERSYNCED &&  theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command) ==MSG_BOM_ALL_STATUS_DATA)
	{
		m_status = MODEUPLOADED;
		return;
	}
	WORD msgcode =  theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	if(m_status == MODEUPLOADED && msgcode == MSG_OP_END_NOTIC)
	{
		LPBYTE content = command->GetContent()->lpData+2;
		WORD noticeType = MAKEWORD(*content,*(content+1));
		if(noticeType == NOTICE_TYPE::DATA_SYNC_COMPLETE)//上位同步数据完成
		{
			m_status = COMPLETED;
			OnComplete(SP_SUCCESS);
			return ;
		}
	}
}

long CConnectSequence::ExecuteCommand()
{
	return SP_SUCCESS;
}
