#pragma once
#include "bomforeservice.h"
#include "RWDef.h"
#include "TicketHelper.h"

//售票业务Service
static const TCHAR* const STR_ISSUE_INFO = _T("售票信息");

class CIssueSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	typedef struct _tagModel
	{
		TicketType_t ticketType;						//车票类型
		CARD_MEDIUM_TYPE ticketPHType;					//车票物理类型
		ProductCategory_t cardCategory;					//发售的车票种类，钱包，计次，定期
		CTicketHelper::IssueWay issueWay;				//通过外部读写器，还是内部读写器发售
		bool isPresell;									//是否是预售
		int piece;										//需要发售的张数
		int issueSuccessPiece;							//发售成功的张数
		long deposit;									//押金
		long paid;										//已付金额
		long due;										//应付金额
		long change;									//找零
		long dueOfSuccess;								//发售成功应付金额
		long changeOfSuccess;							//发售成功找零
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;	//发售车票的票卡属性
		RW_ISSUE_REQUEST rwIssueRequest;				//售票数据模型
		RW_READ_CARD_RESPONSE rwReadCardResponse;		//读卡返回数据
		CCPUCard* pCPUCard;								//CPU卡结构解析
		CULCard* pULCard;								//UL卡结构解析
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
