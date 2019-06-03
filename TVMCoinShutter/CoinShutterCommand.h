#pragma once
#include "Command.h"
#include "CoinShutterCom.h"
#include "CoinShutterException.h"
#include "CoinShutterHardwareException.h"

#include "Sync.h"
#include "DebugLog.h"

#ifndef theCoinShutter_TRACE
	#define theCoinShutter_TRACE CFileLog::GetInstance(_T("CoinShutte_"))
#endif

#define  E_NO_CS_ERROR	0

/**
 @brief  theCoinShutter_TRACE命令父类
*/
class CS_API CCoinShutterCommand : public CCommand
{
public:
	CCoinShutterCommand(void){} 
    ~CCoinShutterCommand(void){}
public:
	long ExecuteCommand() = 0;
};

/**
 @brief	原子命令——打开串口
*/
class CS_API CCSConnect : public CCoinShutterCommand
{
public:
	CCSConnect(int nPort, int nBaud);
	~CCSConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// 端口号
	int m_nBaud;						// 波特率
private:
	long ConnectFunction();

};

/**
 @brief	原子命令——关闭串口
*/
class CS_API CCSClose : public CCoinShutterCommand
{
public:
	CCSClose();
	~CCSClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	原子命令——初始化硬币模块
*/
class CS_API CCSInit : public CCoinShutterCommand
{
public:
	CCSInit();
	~CCSInit();
public:
	long ExecuteCommand();
private:
	long InitFunction();
};

/**
 @brief	复位初始化
*/
class CS_API CCSReset : public CCoinShutterCommand
{
public:
	CCSReset();
	~CCSReset();
public:
	long ExecuteCommand();
private:
	long ResetFunction();
};

/**
 @brief	获取传感器状态
*/
class CS_API CCSGetStatus: public CCoinShutterCommand
{
public:
	CCSGetStatus();
	~CCSGetStatus();
public:
	long ExecuteCommand();
	long GetResponse(tShutterStatus* pSensorStatus);
private:
	long GetStatusFunction();
	tShutterStatus m_sSensorStatus;
};

/**
 @brief	打开闸门
*/
class CS_API CCSOpenShutter : public CCoinShutterCommand
{
public:
	CCSOpenShutter();
	~CCSOpenShutter();
public:
	long ExecuteCommand();
private:
	long OpenShutterFunction();
};

/**
 @brief	关闭闸门
*/
class CS_API CCSCloseShutter : public CCoinShutterCommand
{
public:
	CCSCloseShutter();
	~CCSCloseShutter();
public:
	long ExecuteCommand();
private:
	long CloseShutterFunction();
};