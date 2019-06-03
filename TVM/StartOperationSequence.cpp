#include "stdafx.h"
#include "SCSvcDef.h"
#include "StartOperationSequence.h"
#include "Validation.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��
@param      ��    
@retval     ��
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
@brief      ��������
@param      ��    
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CStartOperationSequence::~CStartOperationSequence()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �жϸ�ʱ���Ƿ��ܹ������ض�������
@param      ��    
@retval     ��
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
@brief      �����ܵ�������ɺ�ʱ������һ�������һ״̬
@param      ��    
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CStartOperationSequence::OnSCCommandComplete(CSCCommand* command)
{
	WORD msgcode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->GetCommandMsgCode(command);
	if(currentMsgIndex>=msgSequences.size()){
		throw new CInnerException(CInnerException::MODULE_ID,CInnerException::LOGIC_ERR,_T(__FILE__),__LINE__,_T("��ʼ��Ӫʱ�����"));
	}
	if(msgcode == msgSequences[currentMsgIndex])
	{
		currentMsgIndex++;
	}
	if(currentMsgIndex==1)//��һ����SC���Ϳ�ʼ��Ӫ��ʼ״̬��������״̬�ϴ���������һ��״̬��״̬ID0x010102��״ֵ̬0x05������Ҫ����һ��ȫ״̬��
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
@brief      ʱ����ɶ�����
@param      ��    
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
long CStartOperationSequence::ExecuteCommand()
{
	//return CCloseOperationSequence;
	return SP_SUCCESS;
}
