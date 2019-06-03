#include "bomforeservice.h"
#include "ReceptionStaticInfo.h"

#pragma once

static const TCHAR* const WM_TO_CHARGE_BASE_DLG       =  _T("WM_TO_CHARGE_BASE_DLG");                           // ��ֵ��������
static const TCHAR* const WM_TO_CHARGE_NOT_DLG		 =  _T("WM_TO_CHARGE_NOT_DLG");                            // ��ֵ���ܻ���
static const TCHAR* const WM_TO_CHARGE_FINISH_DLG     =  _T("WM_TO_CHARGE_FINISH_DLG");
static const TCHAR* const WM_BASE_TO_CHARGE_NOT_DLG   =  _T("WM_BASE_TO_CHARGE_NOT_DLG");

/**
@brief      ��ֵҵ��Service
*/
class CChargeSvc :
    public CBOMForeService
{
    DECLARE_MESSAGE_MAP()

public:
	CChargeSvc();
	~CChargeSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_BASE,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_CHARGE_NOT
	} DIALOG_GROUP;
	
	typedef struct _tagModel
	{
		TicketType_t ticketType;		//Ʊ������
		ProductCategory_t ticketCategory;	//Ʊ������
		CARD_MEDIUM_TYPE cardPHType;	//Ʊ����������
		BYTE cardType;					//
		CString strTicketNumber;		//��Ʊ���
		CString strTicketName;			//��Ʊ����
		CString strTicketEnName;		//��ƱӢ������
		_DATE_TIME validDate;			//��Ч��
		long balance;					//Ʊ�����
		long deposit;					//Ѻ��
		long chargeValue;				//��ֵ���
		long valueAfterCharge;			//��ֵ����
		long due;						//Ӧ��
		long paid;						//ʵ��
		long change;					//����

		_DATE_TIME transTime;

		_tagModel()
		{
			strTicketNumber = "";
			strTicketName = "";
			strTicketEnName = "";
			balance = 0;
			deposit = 0;
			chargeValue = 0;
			valueAfterCharge = 0;
			due = 0;
			paid = 0;
			change = 0;
		}

	}Model;

	long DoCardCharge();                                                    // CPU��ֵ����
	long ChargeOverTime(bool bcheck);                                       // һ��ͨ��ֵ��ֵ��ʱ����
	Model& GetDataModel();

	void NotifyDataModelChanged();
	void OnError(CString strMessage);
	CString GetTpuErrorInfo(int errCode);
private:
	
	void OnReadCard(UINT errCode, CRWReadCard* rwReadCard);
	bool SaveCardInfo(LPRW_READ_CARD_RESPONSE);
	long BlockCard(RW_BUSSINESS_RESPONSE response);
	long DoCardPurseCharge();                                               // CPU��ֵ����(��ֵ/�ƴ�)
	void SaveSCData();                                                      // ����SC�������
	void PrinterPrint();                                                    // ��Ǯ�䲢��ӡ��Ϣ
	

	Model m_DataModel;														
};
