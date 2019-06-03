#include "stdafx.h"
#include "SCSvcDef.h"
#include "CloseOperationSequence.h"
#include "Validation.h"

CCloseOperationSequence::CCloseOperationSequence():CSCSequence()
{

}

CCloseOperationSequence::~CCloseOperationSequence()
{

}

bool CCloseOperationSequence::CanAcceptSCCommand(CSCCommand* command)
{
	if(!command->IsRemote())
	{
		return true;
	}
	WORD msgcode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	WORD availableMsgCode[] = {MSG_SPECIFY_BLOCKNO_DATA,MSG_OP_END_NOTIC,MSG_DEVICE_CONTROL_COMMAND};
	if(!CValidation::IsInRange(msgcode,availableMsgCode))
	{
		return false;
	}
	return true;
}

void CCloseOperationSequence::OnSCCommandComplete(CSCCommand* command)
{
	WORD msgcode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	if(msgcode == MSG_OP_END_NOTIC)
	{
		LPBYTE content = command->GetContent()->lpData+2;
		WORD noticeType = MAKEWORD(*content,*(content+1));
		if(noticeType == NOTICE_TYPE::DAY_AUDIT_COMPLETE)//参数同步完成
		{
			theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_FULLSTATUS,STATUS_TYPE_FULL,NULL);
			//OnComplete(SP_SUCCESS);
		}
	}
	if(msgcode == MSG_DEVICE_CONTROL_COMMAND)
	{
		OnComplete(SP_SUCCESS);
	}
}

long CCloseOperationSequence::ExecuteCommand()
{
	//return CCloseOperationSequence;
	return SP_SUCCESS;
}
