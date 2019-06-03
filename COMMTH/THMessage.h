#pragma once
#include "TIM.h"
#include "THExp.h"
#include "THUpdate.h"
#include "TIMApi.h"
#include "Command.h"


class TH_API CTHCommands : public CCommand
{		
public:
	CTHCommands();
	~CTHCommands();
	HINSTANCE hInst;

	long DealReturnCode(long, tTIMDevReturn*);
protected:
	virtual long ExecuteCommand();
	virtual long IsValidCommand();
private:

};
/** 
@brief   TH 通讯建立命令
*/
class TH_API CTHCommOpen : public CTHCommands
{
public:
    CTHCommOpen(TH_CMD_COMM_OPEN&);
    ~CTHCommOpen(void);
    long IsValidCommand();
    long ExecuteCommand();

private:
	TH_CMD_COMM_OPEN m_command;
};

/** 
@brief   TH 模块初始化命令
*/
class TH_API CTHInit : public CTHCommands
{
public:
    CTHInit(TH_CMD_INIT_RESET);
    ~CTHInit(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(UINT*);

private:
	TH_CMD_INIT_RESET m_command;
	UINT m_uiRetractNum;
};

/** 
@brief   TH 模块复位命令
*/
class TH_API CTHReset : public CTHCommands
{
public:
	CTHReset(TH_CMD_INIT_RESET);
    ~CTHReset(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(UINT*);
	
private:
	TH_CMD_INIT_RESET m_command;
	UINT m_uiRetractNum;
};

/** 
@brief   TH 读取设备驱动版本信息命令
*/
class TH_API CTHGetVersion : public CTHCommands
{
public:
    CTHGetVersion(void);
    ~CTHGetVersion(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(TH_RSP_GET_VERSION*);
	
private:
	TH_RSP_GET_VERSION m_response;
};

/** 
@brief   TH 读取设备状态命令
*/
class TH_API CTHGetDevStatus : public CTHCommands
{
public:
    CTHGetDevStatus(void);
    ~CTHGetDevStatus(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(TH_RSP_GET_DEV_STATUS*);
	
private:
	TH_RSP_GET_DEV_STATUS m_response;
};

/** 
@brief   TH 获取票卡命令
*/
class TH_API CTHCardOut : public CTHCommands
{
public:
    CTHCardOut(TH_CMD_CARD_OUT);
    ~CTHCardOut(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(UINT*);
	
private:
	TH_CMD_CARD_OUT m_command;
	UINT m_uiTransportNum;
	tTIMDevReturn m_tDevReturn;
	tTIMModuleStatus  m_tModuleStatus;
};

/** 
@brief   TH 送出票卡命令
*/
class TH_API CTHSendCard : public CTHCommands
{
public:
    CTHSendCard(void);
    ~CTHSendCard(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(UINT*);
	
private:
	UINT m_uiTransportNum;
};

/** 
@brief   TH 回收票卡命令
*/
class TH_API CTHRetractCard : public CTHCommands
{
public:
    CTHRetractCard(void);
    ~CTHRetractCard(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(UINT*);

private:
	UINT m_uiRetractNum;
};

/** 
@brief   TH 读取审计累计值命令
*/
class TH_API CTHGetAuditData : public CTHCommands
{
public:
    CTHGetAuditData(void);
    ~CTHGetAuditData(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(tTIMAuditCardNum*);
	
private:
	tTIMAuditCardNum m_tAuditCardNum;
	tTIMDevReturn m_tDevReturn;
};

/** 
@brief   TH 读取票箱数量信息命令
*/
class TH_API CTHGetTicketBoxCountInfo : public CTHCommands
{
public:
    CTHGetTicketBoxCountInfo(TH_BOX_TYPE);
    ~CTHGetTicketBoxCountInfo(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(tTIMCardNum*);
	
private:
	TH_BOX_TYPE m_command;
	tTIMCardNum m_response;
};

/** 
@brief   TH 设置票箱数量信息命令
*/
class TH_API CTHSetTicketBoxCountInfo : public CTHCommands
{
public:
    CTHSetTicketBoxCountInfo(TH_BOX_TYPE,tTIMCardNum*);
    ~CTHSetTicketBoxCountInfo(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(tTIMDevReturn*);

private:
	TH_BOX_TYPE m_boxID;
	tTIMCardNum m_cardNum;
	tTIMDevReturn m_DevReturn;
};

/** 
@brief   TH 读取序列号命令
*/
class TH_API CTHGetSN : public CTHCommands
{
public:
    CTHGetSN(void);
    ~CTHGetSN(void);
    long IsValidCommand();
    long ExecuteCommand();
	void GetTHResponse(TH_RSP_GET_SN*);

private:
	TH_RSP_GET_SN m_response;
	tTIMDevReturn m_DevReturn;

};

/** 
@brief   TH 关闭通讯命令
*/
class TH_API CTHCommClose : public CTHCommands
{
public:
    CTHCommClose(void);
    ~CTHCommClose(void);
    long IsValidCommand();
    long ExecuteCommand();
	
private:

};

/** 
@brief   TH 设置模块工作模式命令
*/
class TH_API CTHSetWorkMode : public CTHCommands
{
public:
    CTHSetWorkMode(TH_CMD_SET_WORK_MODE);
    ~CTHSetWorkMode(void);
    long IsValidCommand();
    long ExecuteCommand();
	
private:
		TH_CMD_SET_WORK_MODE m_command;
};

/** 
@brief   TH 设置模块测试命令
*/
class TH_API CTHTestSetting : public CTHCommands
{
public:
    CTHTestSetting(TH_CMD_TEST_SETTING);
    ~CTHTestSetting(void);
    long IsValidCommand();
    long ExecuteCommand();
	
private:
	TH_CMD_TEST_SETTING m_command;
};

/**
@brief   TH 读取票箱RFID信息
*/
class TH_API CTHReadRFIDInfo : public CTHCommands
{
public:
	CTHReadRFIDInfo(BYTE bTicketBoxNo);
	~CTHReadRFIDInfo();

	long ExecuteCommand();			// 执行命令
	long GetTHResponse(tTIMRFIDInfo*);
private:
	BYTE m_bTicketBoxNo;			// 0x00:所有;0x01:A票箱;0x02:B票箱;0x03:废票箱 
	tTIMDevReturn m_TimDevReturn;	// 通用反馈
	tTIMRFIDInfo m_RfidInfo;		// 读取的RFID信息
};

/**
@brief   TH 写票箱RFID信息
*/
class TH_API CTHWriteRFIDInfo : public CTHCommands
{
public:
	CTHWriteRFIDInfo(BYTE bTicketBoxNo,tTIMRFIDInfo* pTimRfidInfo);
	~CTHWriteRFIDInfo();

	long ExecuteCommand();			// 执行命令
private:
	long ThWriteRfidInfo();			// 写RFID

	BYTE m_bTicketBoxNo;			// 0x00:所有;0x01:A票箱;0x02:B票箱;0x03:废票箱 
	tTIMRFIDInfo* m_pRfidInfo;		// RFID信息
	tTIMDevReturn m_TimDevReturn;	// 通用反馈
};

/**
@brief   TH 更换(安装/卸下)票箱
*/
class TH_API CTHReplaceTicketBox : public CTHCommands
{
public:
	CTHReplaceTicketBox(BYTE bTicketBoxNo,BYTE bBoxMode);
	~CTHReplaceTicketBox();

	long ExecuteCommand();			// 执行命令

private:
	long ReplaceTicketBox();		// 更换票箱

	BYTE m_bTicketBoxNo;			// 0x00:所有;0x01:A票箱;0x02:B票箱;0x03:废票箱
	BYTE m_bBoxMode;				// 0x01:卸下;0x02:安装
	tTIMDevReturn m_TimDevReturn;	// 通用反馈
};

/**
@brief   TH更新
*/
class TH_API THUpdateCommand:public CCommand
{
public:
	THUpdateCommand(DWORD port,DWORD speed, DWORD updateBaudRate,char* pFilePath);
	~THUpdateCommand();
protected:
	long ExecuteCommand();

private:
	DWORD m_dPort;
	DWORD m_dSpeed;
	DWORD m_dUpdateSpeed;
	char*m_sFilePath;
};
/**
@brief   票箱托盘控制
*/
class TH_API THTrayContral:public CCommand
{
public:
	THTrayContral(int nTrayType,int nTrayCmd);
	~THTrayContral();
protected:
	long ExecuteCommand();

private:
	int m_nTrayType;
	int m_nTrayCmd;
};
