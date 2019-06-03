#include "BOMForeService.h"
#include "ReceptionStaticInfo.h"
#pragma once

static const TCHAR* const WM_TO_REFUND_APPLY_DLG				=  _T("WM_TO_REFUND_APPLY_DLG");                // �˿����뻭��
static const TCHAR* const WM_TO_REFUND_APPLY_NOT_DLG			=  _T("WM_TO_REFUND_APPLY_NOT_DLG");			// ���벻�ɻ���
static const TCHAR* const WM_TO_REFUND_APPLY_FINISH_DLG      =  _T("WM_TO_REFUND_APPLY_FINISH_DLG");			// ������ɻ���
static const TCHAR* const WM_TO_REFUND_BASE_APPLY_DLG		=  _T("WM_TO_REFUND_BASE_APPLY_DLG");                 // �˿��������
static const TCHAR* const WM_TO_REFUND_FINISH_APPLY_DLG		=  _T("WM_TO_REFUND_FINISH_APPLY_DLG");				// �˿���ɻ���
static const TCHAR* const WM_TO_REFUND_QUERY_FINISH_DLG		=  _T("WM_TO_REFUND_QUERY_FINISH_DLG");				//��ѯ��ɻ���

class CRefundApplySvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _dialog_group{
		DIALOG_GROUP_WAIT = 1,                       // �ȴ���������
		DIALOG_GROUP_APPLY,                          // �˿����뻭��
		DIALOG_GROUP_APPLY_NOT,						 // ���벻�ɻ���
		DIALOG_GROUP_APPLY_FINISH,                   // �����������
		DIALOG_GROUP_QUERY,							 // �˿������ѯ������
		DIALOG_GROUP_QUERY_FINISH,					 // �˿������ѯ��ɻ�����
		DIALOG_GROUP_BASE,                           // �˿��������
		DIALOG_GROUP_FINISH,                         // �˿��������
	} DIALOG_GROUP;
	


	typedef struct _tagModel
	{
		bool isQuerySvc;													// �Ƿ����Ĳ�ѯ

		BYTE cardPHType;
		CString strTicketNum;													// Ʊ�濨��
		CString strTicketType_CN;                                                // Ʊ��������
		CString strTicketType_EN;                                                // Ʊ��Ӣ����
		CString strValidDate;                                                    // Ʊ����Ч��
		long ticketBalance;													// �������
		long ticketDeposit;													// Ѻ��
		long refundBalance;
		long refundDeposit;
		CString strUserName;														// ����
		Sex_t sexType;														// �Ա�
		CERTIFICATE_TYPE certificateType;										// ֤������
		CString strCertificateNumber;											// ֤������
		WORD cityCode;														//���д���
		WORD industryCode;													//��ҵ����
		TicketComHdr_t ticketComHdr;											//Ʊ��������

		_DATE_TIME transTime;													// ����ʱ��	
		RefundApplyType applyType;														// ��������
		RefundReasonType refundReason;													// �˿�ԭ��
		RefundApplyReasonType applyReason;														// ����ԭ��
		BYTE cardType;														// ��Ƭ����
		BYTE cardAppType;														// ��Ʊ��Ʒ����
		CString strSuspendBillNum;												// ��ʧ���ݺ�
		CString strApplyBillNum;													// �����վݺ�

		_tagModel(){

			strTicketNum = "";
			strTicketType_CN = "";
			strTicketType_EN = "";
			strValidDate = "";

			ticketBalance = 0;
			ticketDeposit = 0;
			refundBalance = 0;
			refundDeposit = 0;
			sexType = SEX_MALE;
			certificateType = CERTIFICATE_ID_CARD_MAINLAND;
			
			applyType = APPLY_TYPE_GOOD_CARD;
			refundReason = REFUND_REASON_PASSAGER;
			applyReason = APPLY_REASON_BAD_CARD;
			cardType = 0;
			cardAppType = 0;
			strSuspendBillNum = "";
			strApplyBillNum = "";
		}
	}Model;

	CRefundApplySvc();
	~CRefundApplySvc();
	void SetIsQuery(bool isQuery);
	long RefundApply();														// �˿�����
	BYTE QueryRefundApplyResult();											// ��ѯ�˿�������
	long Refund();                                                         // �˿�
	CString GetTpuErrorInfo(int errCode);									// ���TPU������Ϣ
	void PrinterPrint(bool isApply);                                        // ��ӡ
	bool CheckOperatorIdAndPwd(int nOperatorID,CString sPwd);				// ��֤����ԱID������
	
	Model& GetDataModel();

protected:
	void OnStart();
	
private:
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard);
	long BlockCard(RW_BUSSINESS_RESPONSE& response);
	void SaveSCData(char flag);                                                      // ����SC����

	Model m_DataModel;
};