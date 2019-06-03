#pragma once
#include "IOExp.h"
#include "DIOApi.h"
#include "IOdef.h"
#include "Command.h"
/** 
@brief   
*/
class IO_API CDIOCommOpen : public CCommand
{
public:
	CDIOCommOpen(int nNum,tSerialConfigInfo& command);
	~CDIOCommOpen(void);
	long IsValidCommand();
	long ExecuteCommand(void);

private:
	tSerialConfigInfo m_command;
	int m_Num;
};

/** 
@brief   
*/
class IO_API CDIOCommClose : public CCommand
{
public:
	CDIOCommClose();
	~CDIOCommClose();
	long IsValidCommand();
	long ExecuteCommand();

private:
};

/** 
@brief   
*/
class IO_API CDIOCommInit : public CCommand
{
public:
	CDIOCommInit(void);
	~CDIOCommInit();
	long IsValidCommand();
	long ExecuteCommand();
	void GetIOResponse(STDioDevReturn* response);

private:
	STDioDevReturn m_DioReturn;
};

/** 
@brief   
*/
class IO_API CDIOSetLampSignal : public CCommand
{
public:
	CDIOSetLampSignal(IO_SIGNAL_TYPE usLampType, DIO_LAMP_COLOR ucLampColor,DIO_OPER_TYPE ucOperType, unsigned char ucBlinkTimes = 0x00);
	~CDIOSetLampSignal();
	long IsValidCommand();
	long ExecuteCommand();
	void GetIOResponse(STDioDevReturn* response);

private:
	STDioLampConfig m_DioLampConfig;
	STDioDevReturn m_DioReturn;
};

/** 
@brief   
*/
class IO_API CDIOSetSwitchSignal : public CCommand
{
public:
	CDIOSetSwitchSignal(IO_SIGNAL_TYPE usSignalType, DIO_OPER_TYPE ucOperType,UCHAR ucSignalCnt);
	~CDIOSetSwitchSignal();
	long IsValidCommand();
	long ExecuteCommand();
	void GetIOResponse(STDioDevReturn* response);

private:
	STDioSignalConfig m_DioSignalConfig;
	STDioDevReturn m_DioReturn;
	UCHAR m_ucSignalCnt;
};

/** 
@brief   
*/
class IO_API CDIOCommGetStatus : public CCommand
{
public:
	CDIOCommGetStatus();
	~CDIOCommGetStatus();
	long IsValidCommand();
	long ExecuteCommand();
	void GetIOResponse(IO_RSP_STATUS_INFO* response);

private:
	IO_RSP_STATUS_INFO m_status;
};


class IO_API CDIOGetVersion : public CCommand
{
public:
	CDIOGetVersion(void);
	~CDIOGetVersion(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetDIOResponse(STDioVerInfo* response);

private:
	STDioVerInfo m_DioVerInfo;
	STDioDevReturn m_DioReturn;
};