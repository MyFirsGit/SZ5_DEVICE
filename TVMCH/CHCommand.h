#pragma once
#include "CHExp.h"
#include "Command.h"
#include "CHdef.h"
#include "CHLib.h"
#include "CHCom.h"
#include "Sync.h"
#include "DebugLog.h"
#include "CHException.h"
#include "CHHardwareException.h"
#include "CHAApi.h"

#ifndef theCH_TRACE
	#define theCH_TRACE CFileLog::GetInstance(_T("CH_"))
#endif


/**
 @brief  CH�����
*/
class CH_API CCHCommand : public CCommand
{
public:
	CCHCommand(void){} 
    ~CCHCommand(void){}
public:
	long ExecuteCommand() = 0;
};

/**
 @brief	ԭ��������򿪴���
*/
class CH_API CCHConnect : public CCommand
{
public:
	CCHConnect(int nPort, int nBaud, int nSize);
	~CCHConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// �˿ں�
	int m_nBaud;						// ������
	int m_nSize;						// �ֳ�
private:
	long ConnectFunction();

};

/**
 @brief	ԭ��������رմ���
*/
class CH_API CCHClose : public CCommand
{
public:
	CCHClose();
	~CCHClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	ԭ���������ʼ��Ӳ��ģ��
*/
class CH_API CCHInit : public CCommand
{
public:
	CCHInit(BYTE ctl);
	~CCHInit();
public:
	long ExecuteCommand();
private:
	long InitFunction();
	BYTE m_ctl;
};

/**
 @brief	��λ��ʼ��
*/
class CH_API CCHReset : public CCHCommand
{
public:
	CCHReset(BYTE XinCtl);
	~CCHReset();
public:
	long ExecuteCommand();
private:
	long ResetFunction();
	BYTE m_Ctl;
};

/**
 @brief	״̬��ȡ
*/
class CH_API CCHGetStatus: public CCHCommand
{
public:
	CCHGetStatus();
	~CCHGetStatus();
public:
	long ExecuteCommand();
	long GetResponse(tCHADevStatus* pRes);
private:
	long GetStatusFunction();
	tCHADevStatus m_status;
};

/**
 @brief	����Ӳ��ģ���������Ӳ����
*/
class CH_API CCHSetCoinNum: public CCHCommand
{
public:
	CCHSetCoinNum(tCoinBoxInfo numInfo);
	~CCHSetCoinNum();
public:
	long ExecuteCommand();
private:
	long SetNumFunction();
	tCoinBoxInfo m_numInfo;
};

/**
 @brief	��ʼ����
*/
class CH_API CCHStartReceive: public CCHCommand
{
public:
	CCHStartReceive();
	~CCHStartReceive();
public:
	long ExecuteCommand();
private:
	long StartReceiveFunction();
};

// ��ȡ���յ���Ӳ����Ϣ
class CH_API CCHReadCoinNum: public CCHCommand
{
public:
	CCHReadCoinNum();
	~CCHReadCoinNum();
public:
	long ExecuteCommand();
	long GetResponse(tCHACoinValueInfo* pRes);
private:
	long ReadCoinNumFunction();
	tCHACoinValueInfo m_res;//���ηֱ��ʾ�� 1 Ԫ��5 �ǡ��˱ҵ���Ϣ��
};


CH_API void CH_EnableInsert();
CH_API void CH_DisableInsert();
// ���ƶ�ȡ���յ���Ӳ����Ϣ
class CH_API CCHReadReceivs : public CCHCommand
{
public:
	CCHReadReceivs ();
	~CCHReadReceivs ();
public:
	long ExecuteCommand();
	void GetCHResponse(tCHACoinValueInfo* rsp);
private:
	long GetCH_Insert_Info();
	bool Ch_Insert();
	tCHACoinValueInfo  m_res;//���ηֱ��ʾ�� 1 Ԫ��5 �ǡ��˱ҵ���Ϣ��
};

/**
 @brief	���þ���Ӳ������
*/
class CH_API CCHSetRejectType : public CCHCommand
{
public:
	CCHSetRejectType (COIN_ACCEPTABLE& acceptTable);
	~CCHSetRejectType ();
public:
	long ExecuteCommand();
private:
	long SetFunction();
	COIN_ACCEPTABLE m_acceptTable;
};

/**
 @brief	ֹͣ����Ӳ��
*/
class CH_API CCHEndReceive : public CCHCommand
{
public:
	CCHEndReceive ();
	~CCHEndReceive ();
public:
	long ExecuteCommand();
private:
	long EndFunction();
};

/**
 @brief	ֹͣ��ȡ������Ϣ
*/
class CH_API CHStopReadRV: public CCHCommand
{
public:
	CHStopReadRV(){
	}
	~CHStopReadRV(){
	}
public:
	long ExecuteCommand(void){
		CH_DisableInsert();
		long lRet = 0;
		OnComplete(lRet);
		return lRet;
	}
};

/**
 @brief	�˻�Ӳ��
*/
class CH_API CCHRefund: public CCHCommand
{
public:
	CCHRefund(UINT coinnum);
	~CCHRefund();
public:
	long ExecuteCommand();
private:
	long RefundFunction();
	UINT m_returncoinnum;
};

/**
 @brief	����Ӳ��
*/
class CH_API CCHAccept: public CCHCommand
{
public:
	CCHAccept();
	~CCHAccept();
public:
	long ExecuteCommand();
	long GetResponse(tCHACoinValueInfo* rsp);
private:
	long AcceptFunction();
	BYTE m_bHopperAccept;
	tCHACoinValueInfo m_rsp;
};

/**
 @brief	����
*/

class CH_API CCHChange:public CCHCommand
{
public:
	CCHChange(WORD num_OneY,WORD num_HalfY);
	~CCHChange();
public:
	long ExecuteCommand();
	long GetResponse(tCHAChangeNum* rsp);
private:
	long ChangeFunction();
	WORD m_num_OneY; 
	WORD m_num_HalfY;
	tCHAChangeNum changeSet;

};


/**
 @brief	����Ӳ��
*/
class CH_API CCHClearCoin: public CCHCommand
{
public:
	CCHClearCoin();
	~CCHClearCoin();
public:
	long ExecuteCommand();
	long GetResponse (tCoinBoxInfo*);
private:
	long ClearFunction();
	tCoinBoxInfo CoinBoxInfo;
};


/**
 @brief	��RFID
*/
class CH_API CCHReadRfid: public CCHCommand
{
public:
	CCHReadRfid(int boxNo);//:0x01 = �ӱ���2  0x02 = �ӱ���1  0x03 = ������1 
	~CCHReadRfid();
public:
	long ExecuteCommand();
	long GetResponse(BYTE* rfidData);
private:
	long ReadFunction();
	int m_boxNo;
	BYTE m_RfidData[64];
};


/**
 @brief	дRFID
*/
class CH_API CCHWriteRfid: public CCHCommand
{
public:
	CCHWriteRfid(int boxNo, BYTE* rfidData);//:0x01 �ӱ���2; 0x02 �ӱ���1; 0x03 ������ 
	~CCHWriteRfid();
public:
	long ExecuteCommand();
private:
	long WriteFunction();
	int m_boxNo;
	BYTE m_RfidData[64];
};