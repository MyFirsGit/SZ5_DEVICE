#pragma once
#include "Command.h"
#include "zlg500B.h"
#include "TokenCyclerRfidExp.h"
#include "TokenCyclerRfidException.h"
#include "TokenCyclerRfidHardwareException.h"

#include "Sync.h"
#include "DebugLog.h"

#ifndef theTokenCyclerRfid_TRACE
	#define theTokenCyclerRfid_TRACE CFileLog::GetInstance(_T("TokenCyclerRfid_"))
#endif

#define  E_NO_CS_ERROR	0
extern bool m_bTokenCyclerRfidConnected;
/**
 @brief  theTokenCyclerRfid_TRACE命令父类
*/
class CS_API CTokenCyclerRfidCommand : public CCommand
{
public:
	CTokenCyclerRfidCommand(void){} 
    ~CTokenCyclerRfidCommand(void){}
public:
	long ExecuteCommand() = 0;
};


/**
 @brief	原子命令——建立连接
*/
class CS_API CTCRfidConnect : public CTokenCyclerRfidCommand
{
public:
	CTCRfidConnect(int nPort, int nBaud);
	~CTCRfidConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// 端口号
	int m_nBaud;						// 波特率
private:
	long ConnectFunction();

};

/**
 @brief	原子命令——断开连接
*/
class CS_API CTCRfidClose : public CTokenCyclerRfidCommand
{
public:
	CTCRfidClose();
	~CTCRfidClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	原子命令——读写操作前准备
*/
class CS_API CTCRfidInit : public CTokenCyclerRfidCommand
{
public:
	CTCRfidInit();
	~CTCRfidInit();
public:
	long ExecuteCommand();
private:
	long InitFunction();
};

/**
 @brief	读RFID
*/
class CS_API CTCRfidRead : public CTokenCyclerRfidCommand
{
public:
	CTCRfidRead();
	~CTCRfidRead();
public:
	long ExecuteCommand();
	long GetResponse(BYTE* rfidData);
private:
	long ReadFunction();
private:
	BYTE m_rfidData[16];
};


/**
 @brief	写RFID
*/
class CS_API CTCRfidWrite : public CTokenCyclerRfidCommand
{
public:
	CTCRfidWrite(BYTE* rfidData);
	~CTCRfidWrite();
public:
	long ExecuteCommand();
private:
	long WriteFunction();
private:
	BYTE m_rfidData[16];
};
