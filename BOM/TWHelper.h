#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"

/**
@brief  TW������
*/

class CTWHelper : public CServiceHelper
{
public:

	CTWHelper(CService& service);                                                 // ���캯��
	~CTWHelper();                                                                 // ��������

	// TW���нӿ� 
	void InitCheck(bool reset = true);                                            // ��ʼ����д���Լ�
	void CheckDeviceExchange();													  // ��鲿���Ƿ����
	long Close();
	void InitData();															  // ��¼ʱ��ʼ����д��                     
	static long SetTime(_DATE_TIME,_DATE_TIME&);								  // ����TPUʱ��
	bool RepareIni();                                                             // �޸���д��(��ʼ���Լ죬ά����ʹ��)
	bool Repare();																  // �޸���д��(��ͨҵ������)
	void ReadCard(bool IsPaidArea = false);										  // ����
	long TryReadCard(RW_READ_CARD_RESPONSE&,bool IsPaidArea = false);								//ͬ����һ�ο�
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

	// ҵ������
	void ReadCard(RW_READ_CARD_REQUEST);
	long TryReadCard(RW_READ_CARD_REQUEST,RW_READ_CARD_RESPONSE&);
	long Issue(RW_ISSUE_REQUEST,RW_BUSSINESS_RESPONSE&);
	long Block(RW_CMD_BLOCK,RW_BUSSINESS_RESPONSE&);

	bool IsTWHasException();

	int SaveTWUDData(WORD txnLen,LPBYTE txnBuf);								    // �����д�����ص�UD���ݵ����׹�����
	void SaveTWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType=0);		// �����д�����ص�AR���ݵ�AR������
	CString GetTpuErrorInfo(int iError);                                           // ���ݴ��������Ҵ�����Ϣ
	long GetStatus(RW_RSP_STATUS&);

	long GetTime(_DATE_TIME&);
	long GetTPUInfo(RW_RSP_TPU_INFO&);
	long GetParameterInfo(VERSION_ACC&,VERSION_ECT&,VERSION_AFC&);
private:
	// ͨ������
	long Open();
	long Abort(RW_RSP_STATUS&);
	long Stop(RW_RSP_STATUS&);
	long GetErrCode(int&);

	// ��������
	long InitTW(RW_CMD_INIT,RW_RSP_STATUS&);
	long DownloadParamter(RW_PARAM_DOWNLOAD);
	long GetUDData(RW_BUSSINESS_RESPONSE&);
	static long TWFirmwareDownload(RW_CMD_FIRMWARE);            

	// ����
	static bool IsExceptionCode(long errCode);                                           // �Ƿ�Ϊ��Ҫ���쳣��ID

	int GetAndSaveUDData();														  // ��ȡ������UD����

	CString gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode);             // ���ݴ��������ļ��в��Ҵ�����Ϣ

	static RWID s_TWID;
};