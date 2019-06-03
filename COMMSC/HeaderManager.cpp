#include "stdafx.h"
#include "HeaderManager.h"
#include "MaintenanceInfo.h"

WORD CHeaderManager::msgNo(0x01);
NETWORK_TRANS_ID CHeaderManager::current;

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得当前NTID

@param     无 

@retval     NETWORK_TRANS_ID NTID

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
NETWORK_TRANS_ID CHeaderManager::GetCurrentID()
{
	return current;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得一个新的NTID，自增消息序列号

@param      TRANSFER_COMMAND_CODE 传输命令代码

@retval     NETWORK_TRANS_ID NTID

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
NETWORK_TRANS_ID CHeaderManager::AquireNewID(TRANSFER_COMMAND_CODE commandID)
{
	current.bCmdID = commandID;
	CTime curTime = CTime::GetCurrentTime();      // 消息时间戳
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
	current.scGroup.biGrpNum = theMAINTENANCE_INFO.GetGroupNumber();           // 设备分组编号
	current.scGroup.biEquipNum = theMAINTENANCE_INFO.GetEquNumberInGroup();       // 组内编号

	return current;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得传输命令头，不包括开始标志（F0）

@param      TRANSFER_DATA_TYPE msgType 数据传输类型
@param		TRANSFER_COMMAND_CODE commandID 传输命令代码

@retval     CMD_HEADER 传输命令头

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMD_HEADER CHeaderManager::AquireHeader(TRANSFER_DATA_TYPE msgType,TRANSFER_COMMAND_CODE commandID)
{
	CMD_HEADER header;
	header.bDataTransType = (BYTE)msgType;
	header.nwTransId = AquireNewID(commandID);
	return header;
}