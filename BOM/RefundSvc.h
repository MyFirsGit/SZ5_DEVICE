#include "BOMForeService.h"
#include "ReceptionStaticInfo.h"
#pragma once

static const TCHAR* const WM_TO_REFUND_BASE_DLG				=  _T("WM_TO_REFUND_BASE_DLG");                           // �˿��������
static const TCHAR* const WM_TO_REFUND_NOT_DLG				=  _T("WM_TO_REFUND_NOT_DLG");                            // �˿�ܻ���
static const TCHAR* const WM_TO_REFUND_FINISH_DLG			=  _T("WM_TO_REFUND_FINISH_DLG");						  // �˿���ɻ���
static const TCHAR* const WM_BASE_TO_REFUND_NOT_DLG			=  _T("WM_BASE_TO_REFUND_NOT_DLG");

class CRefundSvc : public CBOMForeService
{
    DECLARE_MESSAGE_MAP()
public:
	typedef enum _dialog_group{
		DIALOG_GROUP_WAIT = 1,                       // �ȴ���������
		DIALOG_GROUP_BASE,                           // �˿��������
		DIALOG_GROUP_REFUND_NOT,                     // �˿�ɻ���
		DIALOG_GROUP_FINISH,                         // �˿��������
	} DIALOG_GROUP;
	typedef struct _tagModel
	{
		BYTE refundReason;													// �˿�ԭ��
		BYTE cardPHType;
		BYTE cardType;														// ��Ƭ����
		BYTE cardAppType;														// ��Ʊ��Ʒ����
		ProductCategory_t cardCategory;											// ��Ƭ��Ʒ����
		CString strTicketNum;													// Ʊ�濨��
		CString strTicketType_CN;                                                // Ʊ��������
		CString strTicketType_EN;                                                // Ʊ��Ӣ����
		CString strValidDate;                                                    // Ʊ����Ч��
		long ticketBalance;													// �������
		long deposit;															// Ѻ��
		long refundDeposit;													// �˿�Ѻ��
		long refundValue;                                                      // �˿���
		_DATE_TIME transTime;

		CCPUCard* pCPUCard;
		CULCard* pULCard;

		_tagModel()
		{
			refundReason = 0;
			cardPHType = 0;
			cardType = 0;
			cardAppType = 0;
			cardCategory = PURSE;
			strTicketNum = "";
			strTicketType_CN = "";
			strTicketType_EN = "";
			strValidDate = "";
			ticketBalance = 0;
			deposit = 0;
			refundDeposit = 0;
			refundValue = 0;
			pCPUCard = new CCPUCard;
			pULCard = new CULCard;
		}
		~_tagModel()
		{
			delete pCPUCard;
			pCPUCard = NULL;
			delete pULCard;
			pULCard = NULL;
		}

	}Model;
	CRefundSvc();
	~CRefundSvc();

	bool CheckOperatorIdAndPwd(int nOperatorID,CString sPwd);				// ��֤����ԱID������
	
	Model& GetDataModel();
	
	void OnOK();
private:
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard);
	long BlockCard(RW_BUSSINESS_RESPONSE response);
	void SaveSCData();                                                      // ����SC����
	void OnError(CString msg,CString receiptMsg = _T(""),CString receiptENMsg = _T(""));
	void PrinterPrint();													// ��ӡ
	Model m_DataModel;

	
};