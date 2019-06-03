#include "stdafx.h"
#include "SCSvcDef.h"
#include "StartOperationSequence.h"
#include "Validation.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CStartOperationSequence::CStartOperationSequence():CSCSequence()
{
	currentMsgIndex =0;
	msgSequences.push_back(MSG_DEVICE_ALL_STATUS_DATA);
	msgSequences.push_back(MSG_DEVICE_ALL_STATUS_DATA);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CStartOperationSequence::~CStartOperationSequence()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断该时序是否能够接受特定的命令
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
bool CStartOperationSequence::CanAcceptSCCommand(CSCCommand* command)
{
	if(!command->IsRemote())
	{
		return true;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      当接受的命令完成后，时序动作，一般进入下一状态
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CStartOperationSequence::OnSCCommandComplete(CSCCommand* command)
{
	WORD msgcode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	if(currentMsgIndex>=msgSequences.size()){
		throw new CInnerException(CInnerException::MODULE_ID,CInnerException::LOGIC_ERR,_T(__FILE__),__LINE__,_T("开始运营时序错误"));
	}
	if(msgcode == msgSequences[currentMsgIndex])
	{
		currentMsgIndex++;
	}
	if(currentMsgIndex==1)//第一次向SC发送开始运营开始状态（以完整状态上传，仅包含一条状态：状态ID0x010102，状态值0x05）后，需要再上一次全状态。
	{
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_DEVICE_FULLSTATUS,STATUS_TYPE_FULL,NULL);
		//theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_DEVICE_FULLSTATUS,STATUS_TYPE_FULL,NULL);

		return;
	}
	if(currentMsgIndex == 2)
	{
		OnComplete(SP_SUCCESS);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      时序完成动作。
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
long CStartOperationSequence::ExecuteCommand()
{
	//return CCloseOperationSequence;
	return SP_SUCCESS;
}
