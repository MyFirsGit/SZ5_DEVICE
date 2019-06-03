#pragma once

#include "BOMForeService.h"
#include "TVMFailureRefundBaseOfficeDlg.h"
#include "ReceptionDlg.h"
#include "TVMFailureRefundReceptionDlg.h"

static const char			FAILURE_INFO_SIZE			=	3;			// վԱ������ʾ��Ϣ������
//static const char			MAX_RIGHT_LEN_7				=	7;			// �������������Ϣ�����������������ַ���Ϊ7
//static const char			MAX_RIGHT_LEN_5				=	5;			// �������������Ϣ�����������������ַ���Ϊ5
static const char			RECEPTION_INFO_SIZE			=	4;			// ������Ϣ����ʾ����

/************************************************************************/
/*   5.80 BOM Refund TVM ticket                                         */
/************************************************************************/
typedef struct _brefundt_ticket 
{   
	CString TVM_ERROR_CODE;                 // TVM Error code
	_DATE_TIME ErrorDate;                   // Date and Time of the Error
	WORD Transaction_amount;                // Transaction_amount

	_brefundt_ticket(){
		TVM_ERROR_CODE = "";
		Transaction_amount = 0;
	}
}BREFUNDT_TICKET,*LPBREFUNDT_TRANS;


class CTVMFailureRefundSvc : public CBOMForeService
{
public:
	CTVMFailureRefundSvc();
	~CTVMFailureRefundSvc();

	void InitFailureInfo();												// ��ʼ��������Ϣ���ݹ���
	PLABEL_GROUP GetFailureInfo();										// ȡ�ù�����Ϣ
	void GetReceptionInfo(MSG_NAME_GROUP*&);							// ȡ�ó˿ͻ�����Ϣ
	void SetFailureInfo(unsigned char ,const CString&);					// ���ù�����Ϣ
	void SetRefundMoney(DWORD);											// �����˿���

	void PrintServiceInfo(CString strDeviceNo,CString strTicketNo);										// ��ӡ����
	void RefreshReceptionOperArea();									// ˢ�³˿ͻ��������
	void DoDetailInfo(int amount);												// һ����ϸ����
	void SetFailureDate(_DATE_TIME);									// ���ù��Ϸ�������
	void SetErrorCode(CString);											// ���ý��ױ��
	void SetReceptionOperInfo();										// ���ó˿ͻ�����Ϣ
	void SetMoneyUnit();												// ���ý�λ

private:
	LABEL_GROUP m_failureInfo[FAILURE_INFO_SIZE];						// ������ʾ�Ĺ�����Ϣ
	MSG_NAME_GROUP m_receptionOperInfo[RECEPTION_INFO_SIZE];			// �˿ͻ����������	
	_DATE_TIME m_date;													// ���Ϸ�������
	CString m_errorCode;												// ���ױ��
	DWORD m_refundMoney;												// �˿����Ϊ��λ��
	DWORD m_transactionNum;												// ��ӡƾ֤��

	CECTCard m_ECTCardInfo;	                                            // һ��ͨ��Ϣ�ṹ��

	long m_ChargePrice;                                                 // Ӧ�ս��

	typedef enum _dialog_group{
		DIALOG_GROUP_BASE	= 1,
		DIALOG_GROUP_OVER
	} DIALOG_GROUP;

};
