#include "stdafx.h"
#include "LPTHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLPTHelper::CLPTHelper(CService &service):CServiceHelper(service)
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
CLPTHelper::~CLPTHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化并口

@param      none

@retval     long     0:成功  1:初始化失败 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CLPTHelper::LPTOpenLPT()
{
	if(!theBOM_INFO.IsLPTUsed()){
		return 0;
	}
	OpenLPT pLPTCmd;
	long errCode = pLPTCmd.ExecuteCommand();
	if(errCode != 0){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
    return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      读取并口状态值

@param      (o)ULONG&    uLptStatusVal 并口状态值

@retval     long  0:成功  1:读取失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CLPTHelper::LPTReadLPT(BYTE& uLptStatusVal)
{
	if(!theBOM_INFO.IsLPTUsed())
	{
		return 1;
	}
	ReadLPT pLPTCmd;
	long errCode = pLPTCmd.ExecuteCommand();
	if(errCode != 0){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	pLPTCmd.GetLPTResponse(uLptStatusVal);
	return errCode;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      关闭并口

@param      none

@retval     long  0:成功  1:读取失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CLPTHelper::LPTCloseLPT()
{
	if(!theBOM_INFO.IsLPTUsed())
	{
		return 0;
	}
	// 关闭并口
	CloseLPT pLPTCmd;
	long errCode = pLPTCmd.ExecuteCommand();
	if(errCode != 0){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return errCode;
}


//****************************************************************************
// 取得出票机门禁的状态
// function  : 取得出票机门禁的状态
// input:		
// output:		
// return:		TickboxDoorStatus  门禁状态
//
//****************************************************************************
DOORSTATUS CLPTHelper::GetTicketboxDoorStatus(){
	if(!theBOM_INFO.IsLPTUsed())
	{
		return DOORSTATUS::CLOSED;
	}
	DOORSTATUS status = DOORSTATUS::UNKNOWN;
	BYTE lptValue = 0;
	int ret = LPTReadLPT(lptValue);
	if(ret == 0)
	{
		status = lptValue == theBOM_INFO.GetClosedDoorStatusValue() ? DOORSTATUS::CLOSED : DOORSTATUS::OPENED;
	}
	// 读取失败
	else if(ret == 2){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);	
	}
	return status;
}
