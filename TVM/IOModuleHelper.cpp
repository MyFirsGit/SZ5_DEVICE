#include "stdafx.h"
#include "Sync.h"
#include "IOModuleHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleHelper::CIOModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleHelper::~CIOModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief				�򿪲���ʼ��IO

@param			none

@retval			none 

@exception		none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::Initialize()
{
	SYNC(IOMGR,_T("IOMGR"));
	this->OpenIO();
	this->InitializeIO();
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				�ر�IOͨ��

@param			none

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseIO()
{
	if (!theAPP_SESSION.IsIOConnected())
	{
		return ;
	}
	CDIOCommClose IOCmd;
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
	else
	{		
		theAPP_SESSION.SetIsIOConnected(false);
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				��ȡIO״̬

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::GetIOStatus(IO_RSP_STATUS_INFO& ioStatus)
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOCommGetStatus IOCmd;
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
	IOCmd.GetIOResponse(&ioStatus);
}
// ����Ͷ�ҿ�ָʾ��
void CIOModuleHelper::OpenCHIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_COIN_OPERATIING_LIGHT,IO_LIGHT_WHITE,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}
// �ر�Ͷ�ҿ�ָʾ��
void CIOModuleHelper::CloseCHIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_COIN_OPERATIING_LIGHT,IO_LIGHT_WHITE,IO_CLOSE);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}
// ����ֽ��Ͷ�ҿ�ָʾ��
void CIOModuleHelper::OpenBHIndicator()
{
	//SYNC(IOMGR,_T("IOMGR"));
	//CDIOSetLampSignal IOCmd(IO_SIGNAL_BH_RECEIVING_LIGHT,IO_LIGHT_WHITE,IO_OPEN);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}

}
// �ر�ֽ��Ͷ�ҿ�ָʾ��
void CIOModuleHelper::CloseBHIndicator()
{	
	//SYNC(IOMGR,_T("IOMGR"));
	//CDIOSetLampSignal IOCmd(IO_SIGNAL_BH_RECEIVING_LIGHT,IO_LIGHT_WHITE,IO_CLOSE);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}

}

// ������ֵ��ָʾ��
void CIOModuleHelper::OpenBKCardIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_RECHARGE_LIGHT,IO_LIGHT_WHITE,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}

}
// �رմ�ֵ��ָʾ��
void CIOModuleHelper::CloseBKCardIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_RECHARGE_LIGHT,IO_LIGHT_WHITE,IO_CLOSE);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}

}

/////////////////////////////////////////////////////////////////////////
/**
@brief				����ȡƱ����ָʾ��

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenReturnCupIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_PICKUPTICKET_INDICATOR,IO_LIGHT_WHITE,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
	// ���������ָʾ�Ƴɹ���ִ������ָʾ�ƹرռ�ʱ��
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_INDICATOR,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				Ϩ��ȡƱ����ָʾ��

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseReturnCupIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_PICKUPTICKET_INDICATOR,IO_LIGHT_WHITE,IO_CLOSE);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				����ȡƱ����������

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenReturnLight()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_PICKUPTICKET_LITGHT,IO_LIGHT_WHITE,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
	// ��������������Ƴɹ���ִ�����������ƹرռ�ʱ��
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_LIGHT,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				Ϩ��ȡƱ����������

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseReturnLight()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_PICKUPTICKET_LITGHT,IO_LIGHT_WHITE,IO_CLOSE);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				��������������

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenOperatorLight()
{
	//SYNC(IOMGR,_T("IOMGR"));
	//CIOSetOneSlotOutput IOCmd(IO_OUTPUT_PORT_NO_OPERATION_LIGHT,IO_OUTPUT_ON);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}
	//// ��������������Ƴɹ���ִ�����������ƹرռ�ʱ��
	//theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_LIGHT,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				Ϩ�����������

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO״̬ 

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseOperatorLight()
{
	//SYNC(IOMGR,_T("IOMGR"));
	//CIOSetOneSlotOutput IOCmd(IO_OUTPUT_PORT_NO_OPERATION_LIGHT,IO_OUTPUT_OFF);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}
}
// ��Ͷ�ҿ�
void CIOModuleHelper::OpenCoinShutter()
{

}
// �ر�Ͷ�ҿ�
void CIOModuleHelper::CloseCoinShutter()
{

}
void CIOModuleHelper::ReturnRechargeCard()										// ��ֵ������
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetSwitchSignal IOCmd(IO_SIGNAL_RECHARGE_UNLOCK,IO_OPEN,1);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}

}
/////////////////////////////////////////////////////////////////////////
/**
@brief				�򿪷�������

@param			none

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenAlarm()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_BUZZER,IO_LIGHT_UNKNOWN,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				�رշ�������

@param			none

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseAlarm()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_BUZZER,IO_LIGHT_UNKNOWN,IO_CLOSE);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				�޸�IO�쳣

@param			none

@retval			none

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::RepareIO()
{
	SYNC(IOMGR,_T("IOMGR"));
	this->CloseIO();
	this->Initialize();
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				��IO����

@param			none

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenIO()
{
	//�������
	if (theAPP_SESSION.IsIOConnected())
	{
		return;
	}
	tSerialConfigInfo connectParam;
	connectParam.chSerialPort           = theTVM_SETTING.GetIOComPort();
	connectParam.nIoSpeed				= theTVM_SETTING.GetIOBaudRate();
	connectParam.nParity = 0;
	connectParam.nDatabits = 8;
	connectParam.nStopbits = 1;
	CDIOCommOpen IOCmd(1,connectParam);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		theAPP_SESSION.SetIsIOConnected(false);
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
	else
	{
		if (!theAPP_SESSION.IsIOConnected())
		{
			theAPP_SESSION.SetIsIOConnected(true);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				��ʼ��IO

@param			none

@retval			long  0:�ɹ�  ��0:ʧ��

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::InitializeIO()
{
	CDIOCommInit IOCmd;
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ԯ��ť��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenHelpButtonLight(){
	//SYNC(IOMGR,_T("IOMGR"));
	//CIOSetOneSlotOutput IOCmd(IO_OUTPUT_PORT_NO_HELP_BTN_LIGHT,IO_OUTPUT_ON);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ر���Ԯ��ť��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseHelpButtonLight(){
	//SYNC(IOMGR,_T("IOMGR"));
	//CIOSetOneSlotOutput IOCmd(IO_OUTPUT_PORT_NO_HELP_BTN_LIGHT,IO_OUTPUT_OFF);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ȡƱ����ָʾ������˸��ʽ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenReturnCupIndicatorFlash(){
	//SYNC(IOMGR,_T("IOMGR"));
	//CIOSetOneOutputFlash IoCmd(IO_OUTPUT_PORT_NO_RETURN_CUP_INDICATOR,IO_OUTPUT_LIGHT_FAST);
	//long errCode = IoCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}
	//// ���������ָʾ�Ƴɹ���ִ������ָʾ�ƹرռ�ʱ��
	//theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_INDICATOR,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������IO

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenAll(){
	SYNC(IOMGR,_T("IOMGR"));

	this->OpenCHIndicator();
	this->OpenChargeIndicator();
	this->OpenReturnCupIndicator();
	this->OpenReturnLight();
	this->OpenAlarm();
	this->OpenBKCardIndicator();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ϩ������IO

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::CloseAll(){
	SYNC(IOMGR,_T("IOMGR"));
	
	this->CloseCHIndicator();
	this->CloseChargeIndicator();
	this->CloseReturnCupIndicator();
	this->CloseReturnLight();
	this->CloseAlarm();
	this->CloseBKCardIndicator();
}