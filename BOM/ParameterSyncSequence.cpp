#include "stdafx.h"
#include "SCSvcDef.h"
#include "ParameterSyncSequence.h"
#include "Validation.h"

CParameterSyncSequence::CParameterSyncSequence():CSCSequence()
{
}

CParameterSyncSequence::~CParameterSyncSequence()
{

}

bool CParameterSyncSequence::CanAcceptSCCommand(CSCCommand* command)
{
	WORD msgcode =  theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	WORD availableMsgCode[] = {
		MSG_UPLOAD_PARAMETER_ID,
		MSG_PARAMETER_ID_DATA,
		MSG_FTP_CONFIG_NOTIC,
		MSG_UPDATE_PARAMETER,
		MSG_REQUEST_SEND_IMIDIATLY_PARAMETER,
		MSG_OP_END_NOTIC
	};
	if(!CValidation::IsInRange(msgcode,availableMsgCode))
	{
		return false;
	}
	if(msgcode == MSG_OP_END_NOTIC)
	{
		LPBYTE content = command->GetContent()->lpData+2;
		WORD noticeType = MAKEWORD(*content,*(content+1));
		if(noticeType != 0x0002)//参数同步完成
		{
			return false;
		}
	}
	return true;
}

void CParameterSyncSequence::OnSCCommandComplete(CSCCommand* command)
{
	WORD msgcode =  theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	if(msgcode == MSG_OP_END_NOTIC)
	{
		LPBYTE content = command->GetContent()->lpData+2;
		WORD noticeType = MAKEWORD(*content,*(content+1));
		if(noticeType == NOTICE_TYPE::PARAMETER_COMPLETE)//参数同步完成
		{
			OnComplete(SP_SUCCESS);
		}
	}
}

long CParameterSyncSequence::ExecuteCommand()
{
	return SP_SUCCESS;
}
