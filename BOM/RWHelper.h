#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"

/**
@brief  RW������
*/

class CRWHelper : public CServiceHelper
{
public:
	
	CRWHelper(CService& service);                                                 // ���캯��
	~CRWHelper();                                                                 // ��������

	// RW���нӿ� 
	void InitCheck(bool reset = true);                                            // ��ʼ����д���Լ�
	void CheckDeviceExchange();													  // ��鲿���Ƿ����
	long Close();
    void InitData();															  // ��¼ʱ��ʼ����д��                     
	static long SetTime(_DATE_TIME,_DATE_TIME&);								  // ����TPUʱ��
	bool RepareIni();                                                             // �޸���д��(��ʼ���Լ죬ά����ʹ��)
	bool Repare();																  // �޸���д��(��ͨҵ������)
	void ReadCard(bool IsPaidArea = false);										  // ����
	long Abort(bool checkStatus = true);                                          // ��ֹ��д��
	long StopReadCard();														  // ֹͣ��������
	bool SynchronismParam();													  // ͬ������������
	bool UpdateParamter(CParamHelper::VEC_PARAM_ID);								  // ���¶�д������
	static int  UpdateProgram(bool isMain);										  // ���¶�д������
	static long SetOperationMode(RW_CMD_SET_MODE,RW_RSP_STATUS&);				  // ������Ӫģʽ
	static long SetBusinessTime(RW_CMD_SET_TIME,RW_RSP_STATUS&);				  // ����TPU 24Сʱ��Ӫ���ӳ���Ӫʱ��
	long GetMac(BYTE*,BYTE*);													  // ����MACֵ
	long GetTicketPrice(RW_CMD_TICKET_PRICE,RW_CMD_TICKET_PRICE_RES&);							  // ��ȡƱ��
	static long Reset(RW_RSP_STATUS&);

	long GetStatus(RW_RSP_STATUS&);
	long GetTime(_DATE_TIME&);
	long GetTPUInfo(RW_RSP_TPU_INFO&);
	long GetParameterInfo(VERSION_ACC&,VERSION_ECT&,VERSION_AFC&);

	// ҵ������
	void ReadCard(RW_READ_CARD_REQUEST);
	long Issue(RW_ISSUE_REQUEST,RW_BUSSINESS_RESPONSE&);
	long Adjust(RW_CMD_ADJUST,RW_BUSSINESS_RESPONSE&);
	long Charge(RW_CMD_CHARGE,RW_BUSSINESS_RESPONSE&);
	long Refund(RW_CMD_REFUND,RW_BUSSINESS_RESPONSE&);
	long Block(RW_CMD_BLOCK,RW_BUSSINESS_RESPONSE&);
	long Counteract(RW_CMD_COUNTERACT,RW_BUSSINESS_RESPONSE&);  
	long Refresh(RW_CMD_ACTIVE,RW_BUSSINESS_RESPONSE&);
	long Defer(RW_CMD_DEFER,RW_BUSSINESS_RESPONSE&);
	long ReplaceOldCard(RW_CMD_REPLACE_CARD&,RW_BUSSINESS_RESPONSE&);
	long ReplaceNewCard(RW_CMD_REPLACE_CARD&,RW_BUSSINESS_RESPONSE&);
	long ChargeOverTimeProcess(RW_CMD_CHARGEOVERTIME chargeOverTime, RW_BUSSINESS_RESPONSE& response);
	long IssueInitCheckProcess(RW_CMD_SETCHECK chargeInput,RW_CMD_RESP_SETCHECK &response);
	long UpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO,RW_BUSSINESS_RESPONSE&);
	bool IsRWHasException();

	int SaveRWUDData(WORD txnLen,LPBYTE txnBuf);								    // �����д�����ص�UD���ݵ����׹�����
	void SaveRWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType=0);		// �����д�����ص�AR���ݵ�AR������
	long GetErrCode(int&);
	CString GetTpuErrorInfo(int iError);                                            // ���ݴ��������Ҵ�����Ϣ
private:
	// ͨ������
	long Open();
	long Abort(RW_RSP_STATUS&);
	long Stop(RW_RSP_STATUS&);
	
	// ��������
	long InitRW(RW_CMD_INIT,RW_RSP_STATUS&);
	long DownloadParamter(RW_PARAM_DOWNLOAD);
	long GetUDData(RW_BUSSINESS_RESPONSE&);
    static long RWFirmwareDownload(RW_CMD_FIRMWARE);            
	
	// ����
	static bool IsExceptionCode(long errCode);                                           // �Ƿ�Ϊ��Ҫ���쳣��ID

	int GetAndSaveUDData();														  // ��ȡ������UD����

	CString gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode);             // ���ݴ��������ļ��в��Ҵ�����Ϣ

	static RWID s_RWID;
	friend class CBOMForeService;
};