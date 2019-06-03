#ifndef __TOKENMESSAGE_H_2011_06_02__
#define __TOKENMESSAGE_H_2011_06_02__

#include "TOKENLib.h"
#include "Command.h"
#include "TOKEN.h"
#include "TOKENCommands.h"


/**
// 雷格特API封装
/** 
@brief   TOKEN 通讯建立命令
*/
class TOKEN_API CTOKENCommOpen : public CTOKENCommands
{
public:
	CTOKENCommOpen(TOKEN_CMD_COMM_OPEN&);
	~CTOKENCommOpen(void);
	long IsValidCommand();
	long ExecuteCommand();

private:
	TOKEN_CMD_COMM_OPEN m_command;
};

/** 
@brief   TOKEN 模块初始化命令
*/
class TOKEN_API CTOKENInit : public CTOKENCommands
{
public:
	CTOKENInit(TOKEN_CMD_INIT);
	~CTOKENInit(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(UINT*);

private:
	TOKEN_CMD_INIT m_command;
	unsigned char m_ucRetractNum;
};

/** 
@brief   TOKEN 模块复位命令
*/
class TOKEN_API CTOKENReset : public CTOKENCommands
{
public:
	CTOKENReset();
	~CTOKENReset(void);
	long IsValidCommand();
	long ExecuteCommand();
private:
};

/** 
@brief   TOKEN 读取设备驱动版本信息命令
*/
class TOKEN_API CTOKENGetVersion : public CTOKENCommands
{
public:
	CTOKENGetVersion(void);
	~CTOKENGetVersion(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(TOKEN_RSP_GET_VERSION*);

private:
	TOKEN_RSP_GET_VERSION m_response;
};

/** 
@brief   TOKEN 读取设备状态命令
*/
class TOKEN_API CTOKENGetDevStatus : public CTOKENCommands
{
public:
	CTOKENGetDevStatus(void);
	~CTOKENGetDevStatus(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(TOKEN_RSP_GET_DEV_STATUS*);

private:
	TOKEN_RSP_GET_DEV_STATUS m_response;
};

/** 
@brief   TOKEN 获取票卡命令
*/
class TOKEN_API CTOKENCardOut : public CTOKENCommands
{
public:
	CTOKENCardOut(TOKEN_BOX_NO boxNo);
	~CTOKENCardOut(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(UINT*);

private:
	uchar m_boxNo;
	uchar m_ucTransportNum;
};

/** 
@brief   TOKEN 送出票卡命令
*/
class TOKEN_API CTOKENSendCard : public CTOKENCommands
{
public:
	CTOKENSendCard(void);
	~CTOKENSendCard(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(UINT*);

private:
	uchar m_ucTransportNum;
};

/** 
@brief   TOKEN 回收票卡命令
*/
class TOKEN_API CTOKENRetractCard : public CTOKENCommands
{
public:
	CTOKENRetractCard(void);
	~CTOKENRetractCard(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(UINT*);

private:
	uchar m_ucRetractNum;
};

/** 
@brief   TOKEN 清理票卡命令
*/
class TOKEN_API CTOKENCleanOut : public CTOKENCommands
{
public:
	CTOKENCleanOut(TOKEN_BOX_NO boxNo);
	~CTOKENCleanOut(void);
	long IsValidCommand();
	long ExecuteCommand();

private:
	uchar m_ucBoxNo;
};


/** 
@brief   TOKEN 读取序列号命令
*/
class TOKEN_API CTOKENGetSN : public CTOKENCommands
{
public:
	CTOKENGetSN(TOKEN_BOX_NO boxNo);
	~CTOKENGetSN(void);
	long IsValidCommand();
	long ExecuteCommand();
	void GetTOKENResponse(char*);

private:
	uchar m_boxNo;
	uchar m_boxId[4];
};


/** 
@brief   TOKEN 关闭通讯命令
*/
class TOKEN_API CTOKENCommClose : public CTOKENCommands
{
public:
	CTOKENCommClose(void);
	~CTOKENCommClose(void);
	long IsValidCommand();
	long ExecuteCommand();

private:

};


/**
@brief   TOKEN 读取票箱RFID信息
*/
class TOKEN_API CTOKENReadRFIDInfo : public CTOKENCommands
{
public:
	CTOKENReadRFIDInfo(unsigned char boxNo);
	~CTOKENReadRFIDInfo();

	long ExecuteCommand();			// 执行命令
	long GetTOKENResponse(TOKEN_READ_WRITE_RFID*);
private:
	long DoRead();
	TOKEN_READ_WRITE_RFID m_RfidInfo;		// 读取的RFID信息
};

/**
@brief   TOKEN 写票箱RFID信息
*/
class TOKEN_API CTOKENWriteRFIDInfo : public CTOKENCommands
{
public:
	CTOKENWriteRFIDInfo(TOKEN_READ_WRITE_RFID* pRfidInfo);
	~CTOKENWriteRFIDInfo();

	long ExecuteCommand();					// 执行命令
private:
	long DoWrite();							// 写RFID
	TOKEN_READ_WRITE_RFID m_RfidInfo;		// RFID信息
};


#endif