#pragma once
#include "BHExp.h"
#include "Command.h"
#include "BHdef.h"
#include "BHCom.h"
#include "BHChangeLib.h"
#include "BHChangeException.h"
#include "BHChangeHardwareException.h"


using namespace BHCHANGE;
/**
 @brief  BH�����
*/
class BH_API CBHChangeCommand : public CCommand
{
public:
	CBHChangeCommand(void);   
    ~CBHChangeCommand(void);
public:
	BOOL         IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime);
	BHTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer = 10000);
	BHTRS_RESULT SendOneCommand_NoRrcv(DWORD nSendLength,BYTE* pSendBuffer);
};

/**
 @brief	ԭ��������򿪴���
*/
class BH_API CBHChangeConnect : public CBHChangeCommand
{
public:
	CBHChangeConnect(int nPort, int nBaud, int nSize);
	~CBHChangeConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// �˿ں�
	int m_nBaud;						// ������
	int m_nSize;						// �ֳ�
	BHCHANGETRS_CONNECT_PARAM m_connectParam;	// ���Ӳ���
private:
	long ConnectFunction();
};

/**
 @brief	ԭ��������رմ���
*/
class BH_API CBHChangeClose : public CBHChangeCommand
{
public:
	CBHChangeClose();
	~CBHChangeClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};


/**
 @brief  �豸��ʼ������
*/
class BH_API CBHChangeDevInit : public CBHChangeCommand
{
public:
	CBHChangeDevInit(BYTE FrontShutterAction,BYTE RearShutterAction,ParamDevInit& ref,BOOL isEnhanced = TRUE);
	~CBHChangeDevInit();
public:
	long ExecuteCommand();
private:
	long DevInitFunction();
	// �ֲ�����
	BYTE m_FontShutterAction,m_RearShutterAction;
	BOOL m_isEnhanced;
	PParamDevInit ptrParaDev;
	// ��������
	BYTE m_RecvBuffer[RES_MAX];
	DWORD m_RecvLen;
};

/**
 @brief  ��ȡ�豸״̬��Ϣ
*/
class BH_API CBHChangeGetStatusInfo : public CBHChangeCommand
{
public:
	CBHChangeGetStatusInfo(/*BOOL isEnhanced = TRUE*/);
	~CBHChangeGetStatusInfo();
public:
	long ExecuteCommand();
	void GetResponse(PDEVICE_STATUS_INFO response);
private:
	long GetStatusInfoFunction();
	// ��������
	BYTE m_RecvBuffer[RES_MAX];
	DWORD m_RecvLen;
	DEVICE_STATUS_INFO m_response;
};


/**
 @brief  bill transportation (�ο�����:��Ʊ����)
*/
class BH_API CBHChangeBillTransportation : public CBHChangeCommand
{
public:
	CBHChangeBillTransportation();
	~CBHChangeBillTransportation();
public:
	long ExecuteCommand();
private:
	long BillTransportationFunction();

	BYTE RecvBuffer[RES_MAX];
	DWORD RecvLen;
};

/**
@brief SendOutBill ֽ���ͳ�
*/
class BH_API CBHChangeSendOutBill : public CBHChangeCommand{
public:
	CBHChangeSendOutBill(int nCountBoxA,int nCountBoxB);
	~CBHChangeSendOutBill();

	long ExecuteCommand();
	void GetChangeResponse(PBANKNOTE_CHANGE_DETAIL rsp);
private:
	// Send Content
	long SendOutBill();
	int  m_nCountBoxA; // ֽ��������A��������
	int  m_nCountBoxB; // ֽ��������B��������
	BYTE TransforIntToHEXCode(int nCount);	// ת������Ϊֽ������ģ��Ҫ������

	long BillTransportation();
	// Recv Content
	BYTE m_RecvBuff[RES_MAX];
	DWORD m_RecvLen;
	BANKNOTE_CHANGE_DETAIL m_changeDetail;
};