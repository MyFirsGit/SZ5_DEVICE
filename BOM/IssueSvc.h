#pragma once
#include "bomforeservice.h"
#include "RWDef.h"
#include "TicketHelper.h"

//��Ʊҵ��Service
static const TCHAR* const STR_ISSUE_INFO = _T("��Ʊ��Ϣ");

class CIssueSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	typedef struct _tagModel
	{
		TicketType_t ticketType;						//��Ʊ����
		CARD_MEDIUM_TYPE ticketPHType;					//��Ʊ��������
		ProductCategory_t cardCategory;					//���۵ĳ�Ʊ���࣬Ǯ�����ƴΣ�����
		CTicketHelper::IssueWay issueWay;				//ͨ���ⲿ��д���������ڲ���д������
		bool isPresell;									//�Ƿ���Ԥ��
		int piece;										//��Ҫ���۵�����
		int issueSuccessPiece;							//���۳ɹ�������
		long deposit;									//Ѻ��
		long paid;										//�Ѹ����
		long due;										//Ӧ�����
		long change;									//����
		long dueOfSuccess;								//���۳ɹ�Ӧ�����
		long changeOfSuccess;							//���۳ɹ�����
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;	//���۳�Ʊ��Ʊ������
		RW_ISSUE_REQUEST rwIssueRequest;				//��Ʊ����ģ��
		RW_READ_CARD_RESPONSE rwReadCardResponse;		//������������
		CCPUCard* pCPUCard;								//CPU���ṹ����
		CULCard* pULCard;								//UL���ṹ����
		_tagModel()
		{
			ticketType = TICKET_SINGLE_RIDE;
			ticketPHType = RESERVE;
			cardCategory = PURSE;
			issueWay = CTicketHelper::BY_INNER_TPU;
			isPresell = false;
			piece = 1;
			issueSuccessPiece = 0;
			paid = 0;
			due = 0;
			change = 0;
			dueOfSuccess = 0;
			changeOfSuccess = 0;
			pCPUCard = NULL;
			pULCard = NULL;
		}
	} Model;

	typedef enum _dialog_group {
		DIALOG_GROUP_WELCOME = 0,
		DIALOG_GROUP_WAIT,
		DIALOG_GROUP_INIT,
		DIALOG_GROUP_CONFIRM,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_ERROR
	} DIALOG_GROUP;
	
	CIssueSvc();
	virtual ~CIssueSvc();
	static const  CStringArray& GetAvailablePieceList();
	static const CStringArray& GetAvailableChargeValueList();
	
	Model& GetDataModel();
	virtual void NotifyDataModelChanged();
	void SetTicketType(TicketType_t&);
	void SetIsPreSell(bool);
	void OnOK();
	bool GetMaxPrice(long&);
protected:
	void OnStart();
private:
	
	void PrinterPrint();
	long BlockCard();
	long IssueCheck(bool bcheck);
	virtual void OnReadCard(UINT,CRWReadCard*);
	bool OnFindCard(RW_READ_CARD_RESPONSE&);
	void OnError(CString errorMsg);
	void SaveBusinessData(RW_BUSSINESS_RESPONSE&);
	void StartReadCard();

	void SetIssueParameter();
	bool RWIssueOneCard();
	bool TWIssueOneCard();
	long TWSaleCardFunc(RW_BUSSINESS_RESPONSE& rwBusinessResponse);
	void UpdateTHCount(BYTE boxACount,BYTE boxBCount,BYTE collectionCount);
	void SyncTicketCountToTH();
	static CStringArray s_AvailablePieceList;
	static CStringArray s_AvailableChargeValueList;

	Model m_DataModel;
	bool fromTicketboxA;
	vector<long>	m_IssuedTickets;
};
