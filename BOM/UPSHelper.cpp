#include "stdafx.h"
#include "UPSHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSHelper::CUPSHelper(CService &service):CServiceHelper(service)
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
CUPSHelper::~CUPSHelper()
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
long CUPSHelper::UPSOpen()
{
	int port = theBOM_INFO.GetUPSCommPort();
	int baud = theBOM_INFO.GetUPSBaudRate();
	CUPSOpen pUPSCmd(port, baud);
	long errCode = pUPSCmd.ExecuteCommand();
	if(errCode != UPS_SUCCESSED){
		throw CUPSException(CUPSException::ERROR_UPS_OPEN_COM, _T(__FILE__), __LINE__);
	}
	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      读取并口状态值

@param      (o)_tagUPSBatteryInfo& info 读取UPS信息

@retval     long  0:成功  1:读取失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CUPSHelper::UPSRead(_tagUPSBatteryInfo &info)
{
	CUPSRead pUPSCmd;
	long errCode = pUPSCmd.ExecuteCommand();
	if(errCode != UPS_SUCCESSED){
		throw CUPSException(CUPSException::ERROR_UPS_READ_COM, _T(__FILE__), __LINE__);
	}
	pUPSCmd.GetUPSInfo(info);
	return errCode;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      关闭并口

@param      none

@retval     long     0:成功  1:初始化失败 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CUPSHelper::UPSClose()
{
	// 关闭并口
	CUPSClose pUPSCmd;
	long errCode = pUPSCmd.ExecuteCommand();
	if(errCode != UPS_SUCCESSED){
		throw CUPSException(CUPSException::ERROR_UPS_CLOSE_COM, _T(__FILE__), __LINE__);
	}
	return errCode;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      获取剩余电量百分比

@param      none

@retval     DWORD     剩余电量百分比

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CUPSHelper::GetUPSBatteryCapacity()
{
	_tagUPSBatteryInfo mUPSInfo;
	long errCode = UPSRead(mUPSInfo);
	return mUPSInfo.dBatteryCapacity;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      获取剩余时间

@param      none

@retval     DWORD     剩余时间(单位：分钟)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CUPSHelper::GetUPSLeftStandbyTime()
{
	_tagUPSBatteryInfo mUPSInfo;
	long errCode = UPSRead(mUPSInfo);
	return mUPSInfo.dwLeftStandbyTime/60;
}
