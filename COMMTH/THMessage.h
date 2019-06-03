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
@brief   TH ͨѶ��������
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
@brief   TH ģ���ʼ������
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
@brief   TH ģ�鸴λ����
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
@brief   TH ��ȡ�豸�����汾��Ϣ����
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
@brief   TH ��ȡ�豸״̬����
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
@brief   TH ��ȡƱ������
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
@brief   TH �ͳ�Ʊ������
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
@brief   TH ����Ʊ������
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
@brief   TH ��ȡ����ۼ�ֵ����
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
@brief   TH ��ȡƱ��������Ϣ����
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
@brief   TH ����Ʊ��������Ϣ����
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
@brief   TH ��ȡ���к�����
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
@brief   TH �ر�ͨѶ����
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
@brief   TH ����ģ�鹤��ģʽ����
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
@brief   TH ����ģ���������
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
@brief   TH ��ȡƱ��RFID��Ϣ
*/
class TH_API CTHReadRFIDInfo : public CTHCommands
{
public:
	CTHReadRFIDInfo(BYTE bTicketBoxNo);
	~CTHReadRFIDInfo();

	long ExecuteCommand();			// ִ������
	long GetTHResponse(tTIMRFIDInfo*);
private:
	BYTE m_bTicketBoxNo;			// 0x00:����;0x01:AƱ��;0x02:BƱ��;0x03:��Ʊ�� 
	tTIMDevReturn m_TimDevReturn;	// ͨ�÷���
	tTIMRFIDInfo m_RfidInfo;		// ��ȡ��RFID��Ϣ
};

/**
@brief   TH дƱ��RFID��Ϣ
*/
class TH_API CTHWriteRFIDInfo : public CTHCommands
{
public:
	CTHWriteRFIDInfo(BYTE bTicketBoxNo,tTIMRFIDInfo* pTimRfidInfo);
	~CTHWriteRFIDInfo();

	long ExecuteCommand();			// ִ������
private:
	long ThWriteRfidInfo();			// дRFID

	BYTE m_bTicketBoxNo;			// 0x00:����;0x01:AƱ��;0x02:BƱ��;0x03:��Ʊ�� 
	tTIMRFIDInfo* m_pRfidInfo;		// RFID��Ϣ
	tTIMDevReturn m_TimDevReturn;	// ͨ�÷���
};

/**
@brief   TH ����(��װ/ж��)Ʊ��
*/
class TH_API CTHReplaceTicketBox : public CTHCommands
{
public:
	CTHReplaceTicketBox(BYTE bTicketBoxNo,BYTE bBoxMode);
	~CTHReplaceTicketBox();

	long ExecuteCommand();			// ִ������

private:
	long ReplaceTicketBox();		// ����Ʊ��

	BYTE m_bTicketBoxNo;			// 0x00:����;0x01:AƱ��;0x02:BƱ��;0x03:��Ʊ��
	BYTE m_bBoxMode;				// 0x01:ж��;0x02:��װ
	tTIMDevReturn m_TimDevReturn;	// ͨ�÷���
};

/**
@brief   TH����
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
@brief   Ʊ�����̿���
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
