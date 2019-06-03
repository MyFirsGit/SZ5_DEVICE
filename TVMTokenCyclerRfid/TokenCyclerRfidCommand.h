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
 @brief  theTokenCyclerRfid_TRACE�����
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
 @brief	ԭ���������������
*/
class CS_API CTCRfidConnect : public CTokenCyclerRfidCommand
{
public:
	CTCRfidConnect(int nPort, int nBaud);
	~CTCRfidConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// �˿ں�
	int m_nBaud;						// ������
private:
	long ConnectFunction();

};

/**
 @brief	ԭ��������Ͽ�����
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
 @brief	ԭ���������д����ǰ׼��
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
 @brief	��RFID
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
 @brief	дRFID
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
