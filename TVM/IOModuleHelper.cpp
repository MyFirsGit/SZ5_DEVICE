#include "stdafx.h"
#include "Sync.h"
#include "IOModuleHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleHelper::CIOModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleHelper::~CIOModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief				打开并初始化IO

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
@brief				关闭IO通信

@param			none

@retval			long  0:成功  非0:失败

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
@brief				获取IO状态

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
// 点亮投币口指示灯
void CIOModuleHelper::OpenCHIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_COIN_OPERATIING_LIGHT,IO_LIGHT_WHITE,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}
// 关闭投币口指示灯
void CIOModuleHelper::CloseCHIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_COIN_OPERATIING_LIGHT,IO_LIGHT_WHITE,IO_CLOSE);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}
}
// 点亮纸币投币口指示灯
void CIOModuleHelper::OpenBHIndicator()
{
	//SYNC(IOMGR,_T("IOMGR"));
	//CDIOSetLampSignal IOCmd(IO_SIGNAL_BH_RECEIVING_LIGHT,IO_LIGHT_WHITE,IO_OPEN);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}

}
// 关闭纸币投币口指示灯
void CIOModuleHelper::CloseBHIndicator()
{	
	//SYNC(IOMGR,_T("IOMGR"));
	//CDIOSetLampSignal IOCmd(IO_SIGNAL_BH_RECEIVING_LIGHT,IO_LIGHT_WHITE,IO_CLOSE);
	//long errCode = IOCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}

}

// 点亮储值卡指示灯
void CIOModuleHelper::OpenBKCardIndicator()
{
	SYNC(IOMGR,_T("IOMGR"));
	CDIOSetLampSignal IOCmd(IO_SIGNAL_RECHARGE_LIGHT,IO_LIGHT_WHITE,IO_OPEN);
	long errCode = IOCmd.ExecuteCommand();
	if(errCode != 0){
		throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	}

}
// 关闭储值卡指示灯
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
@brief				点亮取票找零指示灯

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
	// 如果打开找零指示灯成功，执行找零指示灯关闭计时器
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_INDICATOR,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				熄灭取票找零指示灯

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
@brief				点亮取票找零照明灯

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
	// 如果打开找零照明灯成功，执行找零照明灯关闭计时器
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_LIGHT,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				熄灭取票找零照明灯

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
@brief				点亮操作照明灯

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
	//// 如果打开找零照明灯成功，执行找零照明灯关闭计时器
	//theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_LIGHT,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief				熄灭操作照明灯

@param			(o)IO_RSP_STATUS_INFO& ioStatus  IO状态 

@retval			long  0:成功  非0:失败

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
// 打开投币口
void CIOModuleHelper::OpenCoinShutter()
{

}
// 关闭投币口
void CIOModuleHelper::CloseCoinShutter()
{

}
void CIOModuleHelper::ReturnRechargeCard()										// 充值卡弹卡
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
@brief				打开蜂鸣报警

@param			none

@retval			long  0:成功  非0:失败

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
@brief				关闭蜂鸣报警

@param			none

@retval			long  0:成功  非0:失败

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
@brief				修复IO异常

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
@brief				打开IO串口

@param			none

@retval			long  0:成功  非0:失败

@exception		CIOException 
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenIO()
{
	//如果连接
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
@brief				初始化IO

@param			none

@retval			long  0:成功  非0:失败

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
@brief      打开招援按钮灯

@param      无

@retval     无

@exception  无
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
@brief      关闭招援按钮灯

@param      无

@retval     无

@exception  无
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
@brief      点亮取票找零指示灯已闪烁方式

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleHelper::OpenReturnCupIndicatorFlash(){
	//SYNC(IOMGR,_T("IOMGR"));
	//CIOSetOneOutputFlash IoCmd(IO_OUTPUT_PORT_NO_RETURN_CUP_INDICATOR,IO_OUTPUT_LIGHT_FAST);
	//long errCode = IoCmd.ExecuteCommand();
	//if(errCode != 0){
	//	throw CIOException((CIOException::DETAIL_ERROR_CODE)errCode,_T(__FILE__),__LINE__);
	//}
	//// 如果打开找零指示灯成功，执行找零指示灯关闭计时器
	//theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_RETURN_INDICATOR,NULL,NULL);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      点亮所有IO

@param      无

@retval     无

@exception  无
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
@brief      熄灭所有IO

@param      无

@retval     无

@exception  无
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