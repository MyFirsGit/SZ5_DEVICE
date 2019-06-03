#pragma once

#include "PassengerForeSvc.h"
#include "ReceptionStaticInfo.h"

/**
@brief      ��ֵҵ��Service
*/
class CProductIssueSvc : public CPassengerForeSvc
{
     DECLARE_MESSAGE_MAP()

public:

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_CHOOSE_LOCATION,
		DIALOG_GROUP_CHOOSE_PAYMENT,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;

	// SVC����ģ��
	struct DataModel : public CommonModel
	{
		BYTE SalableProductCount;					// ���۲�Ʒ����
		BYTE SalableProductType;					// ���۲�Ʒ����
	};

	CProductIssueSvc();
	virtual ~CProductIssueSvc();

	DataModel& GetDataModel();

	map<WORD,CString> GetAvailableProductNames();
	map<WORD,CString> GetAvailableSubproductNames();
	map<DWORD,CString> GetAvailableLocationNames();

	// ҵ�������˳������
	long CalculatePrice();								// ��ȡ��ƷƱ��
	void DlgNextPage();
	bool DoCardIssueProduct();							// TPU���²�Ʒ������ֵ/����/�ƴ�)
	void PrinterPrintReceipt();

	bool IsBothCoinBanknoteMaxCount();
	bool IsBanknoteMaxCount();
	bool IsBanknoteMaxAmount();
	bool IsCoinMaxCount();
//	bool POSPayment();												// ���п�֧��
//	bool POSCancle();												// ���п�����
//	bool IssueProductCard();										// ��Ӳ�Ʒ����
	void ReturnFinishDlg();											// ���ص���ɽ���

protected:

	// �������Ĺ��캯���У���*m_pDataModel��PassengerForeSvc���*m_pCommonModelָ��ͬһ��struct��
	// ���������������methodʱ��ʹ��*m_pDataModel���̳еĸ���struct������������ӡ�
	// ��ʹ��PassengerForeSvc���methodʱ��ͨ��*m_pCommonModel���Կɲ���DataModel��CommonModel�Ĳ��֡�
	DataModel *m_pDataModel;

	// ҵ�������˳������
	long SaveCardInfo(LPRW_READ_CARD_RESPONSE);			// ���泵Ʊ���ݵ�DataModel
	void OnSuccessFinish();
	void SaveSCData();									// ����SC����
	void WriteDBData(Data::TransactionRecord &dbInfo);	// д�����ݿ����ݣ��������ڲ�ͬ�������ر�ִ�У�
	void FlowToFinishDlg();								// ��OnSuccessFinishʹ�õ�һ��������װ��������ã�
	void FlowToErrorDlg();								// ��OnErrorFinish��OnCancel�͸��ֳ���ʹ�õ�һ��������װ��������ã�

	// ҵ��������쳣����
	void PrinterPrintErrorReceipt();
	bool ConfirmCard(LPRW_READ_CARD_RESPONSE);			//�ж�ˢ����Ϣ�Ƿ�һ�£�������ã�

private:

	// ����
	void FilterSalableProductList(vector<CAccTicketParam::TICKET_TYPE>& vecProduct); // ɸѡ���Գ�ֵ�Ĳ�Ʒ�б�
	bool IsCoinChageMaxCount();
	COIN_ACCEPTABLE GetAcceptCoinFaceTable();						// ��ȡ�������Ӳ�������Ϣ
	BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL);					// ��ȡ�������ֽ�������Ϣ
	BOOL m_IsTheSameCard;
	vector<CACCCard::ACCCARD_PRODUCT_INFO> m_vecReadCard; 
	BOOL isSubProductExist(WORD productType,WORD subProductType);	// �жϲ�ƷProductType���Ƿ����subProduct��Ʒ
	friend class CProductIssueReceptionDlg;
protected:
	virtual void OnStart();
};
