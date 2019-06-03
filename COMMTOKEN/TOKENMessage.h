#ifndef __TOKENMESSAGE_H_2011_06_02__
#define __TOKENMESSAGE_H_2011_06_02__

#include "TOKENLib.h"
#include "Command.h"
#include "TOKEN.h"
#include "TOKENCommands.h"


/**
// �׸���API��װ
/** 
@brief   TOKEN ͨѶ��������
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
@brief   TOKEN ģ���ʼ������
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
@brief   TOKEN ģ�鸴λ����
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
@brief   TOKEN ��ȡ�豸�����汾��Ϣ����
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
@brief   TOKEN ��ȡ�豸״̬����
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
@brief   TOKEN ��ȡƱ������
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
@brief   TOKEN �ͳ�Ʊ������
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
@brief   TOKEN ����Ʊ������
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
@brief   TOKEN ����Ʊ������
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
@brief   TOKEN ��ȡ���к�����
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
@brief   TOKEN �ر�ͨѶ����
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
@brief   TOKEN ��ȡƱ��RFID��Ϣ
*/
class TOKEN_API CTOKENReadRFIDInfo : public CTOKENCommands
{
public:
	CTOKENReadRFIDInfo(unsigned char boxNo);
	~CTOKENReadRFIDInfo();

	long ExecuteCommand();			// ִ������
	long GetTOKENResponse(TOKEN_READ_WRITE_RFID*);
private:
	long DoRead();
	TOKEN_READ_WRITE_RFID m_RfidInfo;		// ��ȡ��RFID��Ϣ
};

/**
@brief   TOKEN дƱ��RFID��Ϣ
*/
class TOKEN_API CTOKENWriteRFIDInfo : public CTOKENCommands
{
public:
	CTOKENWriteRFIDInfo(TOKEN_READ_WRITE_RFID* pRfidInfo);
	~CTOKENWriteRFIDInfo();

	long ExecuteCommand();					// ִ������
private:
	long DoWrite();							// дRFID
	TOKEN_READ_WRITE_RFID m_RfidInfo;		// RFID��Ϣ
};


#endif