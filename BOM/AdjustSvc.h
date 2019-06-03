#pragma once
#include "bomforeservice.h"
#include "ReceptionStaticInfo.h"
#include "CommonDefine.h"

using namespace std;

static const TCHAR* const WM_ADJUST_WAIT_TO_INIT = _T("WM_ADJUST_WAIT_TO_INIT");
static const TCHAR* const WM_ADJUST_INIT_TO_FINISH	= _T("WM_ADJUST_INIT_TO_FINISH");
static const TCHAR* const WM_ADJUST_WAIT_TO_FINISHT	= _T("WM_ADJUST_WAIT_TO_FINISHT");
static const TCHAR* const WM_WAIT_TO_ERROR = _T("WM_WAIT_TO_ERROR");
static const TCHAR* const WM_INI_TO_ERROR = _T("WM_INI_TO_ERROR");
static const TCHAR* const STR_ADJUST_INFO = _T("��Ʊ��Ϣ");


class CAdjustSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	// �������ID
	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT=1,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR,
	} DIALOG_GROUP;
	typedef struct _tagModel
	{
		bool isPaidArea;					//�Ƿ��Ǹ�����

		TicketType_t ticketType;			//Ʊ������
		CString strTicketTypeName;			//��Ʊ��������
		CString strTicketTypeEnName;		//��Ʊ����Ӣ������
		ProductCategory_t ticketCategory;		//Ʊ������
		CARD_MEDIUM_TYPE cardPHType;		//Ʊ����������
		CString strTicketNumber;			//Ʊ����
		BYTE journeyStatus;					//�ó�״̬
		WORD saleStation;					//������վ
		_DATE_TIME saleTime;				//����ʱ��
		WORD enterStation;					//��վ��վ
		_DATE_TIME enterDateTime;			//��վʱ��
		WORD exitStation;					//��վ��վ
		_DATE_TIME exitDateTime;			//��վʱ��
		long balance;						//Ʊ�����
		
		long adjustAmount;					//��Ʊ���
		long actualAdjustAmount;			//ʵ�ʲ�Ʊ���
		BYTE adjustCode;					//��Ʊ�������
		CString strAdjustGuide;				//��Ʊ��������
		CString strAdjustGuideOfReceipt;		//�˿Ͳ�Ʊ��������
		CString strAdjustEnGuideOfReceipt;		//�˿Ͳ�ƱӢ�Ľ�������
		_PAYED_METHOD payMethod;				//��Ʊ��ʽ
		BYTE overTimeReason;					//��ʱʱ����ʱԭ�� 0��δ��ʱ�� 1���˿�ԭ��2������ԭ��

		bool canSelectStation;					//�Ƿ����ѡ��վ��
		int	 availablePayMethod;				//����ѡ��Ʊ��ʽ 1���ֽ�2�����ڣ�3���ֽ�Ϳ���

		long due;							//Ӧ��
		long paid;							//ʵ��
		long change;						//����

		_tagModel()
		{
			isPaidArea = true;
			ticketType = TICKET_SINGLE_RIDE;
			strTicketTypeName = "";	
			strTicketTypeEnName = "";
			ticketCategory = PURSE;	
			cardPHType = ULTRALIGHT;
			strTicketNumber="";
			journeyStatus = 0;
			saleStation=0;			
			enterStation=0;	
			exitStation=0;
			balance=0;
			adjustAmount=0;		
			actualAdjustAmount = 0;
			adjustCode=0;					
			strAdjustGuide="";
			strAdjustGuideOfReceipt="";
			strAdjustEnGuideOfReceipt="";
			payMethod=BY_CASH;				
			overTimeReason = 0;

			canSelectStation = false;					
			availablePayMethod = 0x01;

			due=0;							//Ӧ��
			paid=0;							//ʵ��
			change=0;						//����
		}

	}Model;
	void SetIsPaidArea(bool isPaidArea);
	void OnOK();
	Model& GetDataModel();
	void NotifyDataModelChanged();
	CAdjustSvc();
	~CAdjustSvc();

protected:
	void OnStart();
	void OnReadCard(UINT,CRWReadCard*);
	long BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response);
	void PrinterPrint();

private:
	
	bool IsAdjustCodeValid();
	bool IsNeedAdust();

	void OnCardError(CString errorMsg);
	void Adjust();
	bool SaveCardInfo(LPRW_READ_CARD_RESPONSE);
	void SaveBusinessData(LPRW_BUSSINESS_RESPONSE);

	
	Model m_DataModel;

};
