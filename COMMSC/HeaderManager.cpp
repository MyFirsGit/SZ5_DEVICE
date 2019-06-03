#include "stdafx.h"
#include "HeaderManager.h"
#include "MaintenanceInfo.h"

WORD CHeaderManager::msgNo(0x01);
NETWORK_TRANS_ID CHeaderManager::current;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�õ�ǰNTID

@param     �� 

@retval     NETWORK_TRANS_ID NTID

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
NETWORK_TRANS_ID CHeaderManager::GetCurrentID()
{
	return current;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ȡ��һ���µ�NTID��������Ϣ���к�

@param      TRANSFER_COMMAND_CODE �����������

@retval     NETWORK_TRANS_ID NTID

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
NETWORK_TRANS_ID CHeaderManager::AquireNewID(TRANSFER_COMMAND_CODE commandID)
{
	current.bCmdID = commandID;
	CTime curTime = CTime::GetCurrentTime();      // ��Ϣʱ���
	current.dtTimeStamp.wYear = curTime.GetYear();
	current.dtTimeStamp.biMonth = curTime.GetMonth();
	current.dtTimeStamp.biDay = curTime.GetDay();
	current.dtTimeStamp.biHour = curTime.GetHour();
	current.dtTimeStamp.biMinute = curTime.GetMinute();
	current.dtTimeStamp.biSecond = curTime.GetSecond();
	current.wMsgNo = msgNo;
	if(msgNo == 0xFFFF)
	{
		msgNo = 0x01;
	}
	else
	{
		msgNo++;
	}
	current.lDeviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	current.scGroup.biGrpNum = theMAINTENANCE_INFO.GetGroupNumber();           // �豸������
	current.scGroup.biEquipNum = theMAINTENANCE_INFO.GetEquNumberInGroup();       // ���ڱ��

	return current;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ô�������ͷ����������ʼ��־��F0��

@param      TRANSFER_DATA_TYPE msgType ���ݴ�������
@param		TRANSFER_COMMAND_CODE commandID �����������

@retval     CMD_HEADER ��������ͷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMD_HEADER CHeaderManager::AquireHeader(TRANSFER_DATA_TYPE msgType,TRANSFER_COMMAND_CODE commandID)
{
	CMD_HEADER header;
	header.bDataTransType = (BYTE)msgType;
	header.nwTransId = AquireNewID(commandID);
	return header;
}