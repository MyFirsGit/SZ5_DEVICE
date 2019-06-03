#pragma once
#include "PassengerForeSvc.h"
#include "RWDef.h"
#include "TicketHelper.h"
#include "CAccTicketParam.h"

//static const TCHAR* const STR_ISSUE_INFO = _opl(TXT_ISSUE_INFO);

/**
@brief ��Ʊҵ��Service
*/
class CIssueSvc : public CPassengerForeSvc
{
	DECLARE_MESSAGE_MAP()

public:
	CIssueSvc();
	virtual ~CIssueSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_AREA_MAP = 1,
		DIALOG_GROUP_ISSUE_WITH_CASH,
		DIALOG_GROUP_ISSUE_WITH_SVT,
		DIALOG_GROUP_ISSUE_WITH_NET,
		DIALOG_GROUP_ISSUE_WITH_CASH_FINISH,
		DIALOG_GROUP_ISSUE_WITH_CASH_ERROR,
		DIALOG_GROUP_ISSUE_WITH_SVT_FINISH,
		DIALOG_GROUP_ISSUE_WITH_SVT_ERROR,
		DIALOG_GROUP_ISSUE_WITH_NET_FINISH,
		DIALOG_GROUP_ISSUE_WITH_NET_ERROR,
	} DIALOG_GROUP;

	typedef struct _tagModel : public CommonModel
	{
		bool  needBeginStation;				// ���������ƣ��Ƿ���Ҫָ������վ
		bool  needEndStation;				// ���������ƣ��Ƿ���Ҫָ��Ŀ��վ
		bool  needInputBeginStation;		// ���������ƣ�����ʱ�Ƿ����ó���վ
		bool  needInputEndStation;			// ���������ƣ�����ʱ�Ƿ�����Ŀ��վ
		bool  BeginStationIsCurrent;		// ���������ƣ�����ʱ�Ƿ�����validityOriginΪ��ǰվ��־
		bool  EndStationIsCurrent;			// ���������ƣ�����ʱ�Ƿ�����validityDestinationΪ��ǰվ��־
		bool  needCardDeposit;				// ���������ƣ����˿Ѻ����
		bool  needCardFee;					// ���������ƣ������˿�ɱ��ѱ��

		BYTE ticketPiece;					// ��Ʊ����
		BYTE issuedPiece;					// �ѷ��۳ɹ�������

		//bool isCatchTicketsOk;				// ץȡƱ���Ƿ�ɹ���
		//bool isReleaseTicketsOk;			// �ų�Ʊ���Ƿ�ɹ���
		//bool isReadWriteCardOk;				// д���Ƿ�ɹ���

		CACCCard* pCardInfo;						// ACC���ṹ����
		RW_READ_CARD_RESPONSE rwReadCardResponse;	// ������������

		// Card����Ϣ
		CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card��A
		CTCardCountInfo::CARD_BOX_INFO cardBoxBInfo;		// card��B
		CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card��Ʊ��
		//// Token����Ϣ
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenHopperAInfo;		// token��A
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenHopperBInfo;		// token��B
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token��Ʊ��

		long refundMoney;					// �˿���	//??

		_tagModel()
		{
			needBeginStation = false;
			needEndStation = false;
			needInputBeginStation = false;
			needInputEndStation = false;
			BeginStationIsCurrent = false;
			EndStationIsCurrent = false;
			needCardDeposit = false;
			needCardFee = false;
			ticketPiece = 0;
			issuedPiece = 0;
			pCardInfo = NULL;
			refundMoney = 0;
			CommonModel();
		}
	} Model;

	Model& GetDataModel();
	virtual void NotifyDataModelChanged();
	void StartReceiveMoney();				// ��ʼ��Ǯ

	// ��ʼ��
	void InitIssueProduct();
	vector<ACC_PRODUCT_t*>& GetIssueProduct();
	vector<WORD>& GetIssuePayment();
	void GetIssuePiece(map<int,CString>& mapPrice);
	bool CheckPayment(WORD payment, CardType_t cardtype);

	// �˿Ͳ���
	void SetProductType(WORD type);
	void SetSubProductType(DWORD typeorprice);
	void SetBeginStation(DWORD locationNumber = LOCATION_NUMBER_UNSPECIFIED);
	void SetEndStation(DWORD locationNumber = LOCATION_NUMBER_UNSPECIFIED);
	void SetPassengerType(PassengerType_t type);
	void SetTicketPiece(WORD piece);
	void SetPayment(PaymentMethod_t paymentMethod);
	void SetTicketAmount(int nAmount);

	// �߼��ж�
	bool SupportSJT();
	bool NeedBeginSectionByPassenger();
	bool NeedEndSectionByPassenger();
	BYTE AutoCalcSaleMode();
	BYTE AutoCalcPriceMode();
	bool AutoCalcTicketPrice();
	void AutoCalcPaymentInfo();
	void DoneToNextDialog();

	// ҵ����
	void StartPayMoney();                             // ��ʼ��Ǯ
	bool AnalyzeTicket(long& resultCode, bool& needStopLoop);

	void StartIssueAndChange();
	bool IsBothCoinBanknoteMaxCount();
	bool IsBanknoteMaxAmount();
	bool IsBanknoteMaxCount();
	bool IsCoinMaxCount();
	bool IsCoinChageMaxCount();

	// ֽ�����������ж�
	bool IsBHChangeMaxCount();
	bool IsBHChangeMaxAmout();

	CString BanknoteShowword();
	CString CoinShowword();

	void FlowToNetPayDlg();
	void FlowToErrorDlg();
	void FlowToFinishDlg();								// ת����ɽ���
	void FlowToDefaultDlg();							// ת��Ĭ�Ͻ���,��Ӳ������ʱ

public:
	// ���۳�Ʊ
	bool IssueTicketProc();
	bool IssueOneTicket();
	bool IssueOneToken();
	bool IssueOneCard();
	long ReadWriteToken(RW_BUSSINESS_RESPONSE& rwBusinessResponse);
	long ReadWriteCard(RW_BUSSINESS_RESPONSE& rwBusinessResponse);

	// ��Ǯ����

	// ʧ�ܴ���
	//void OnIssueAndChangeError();
	void OnReceiveError();
	
	
	void AcceptMoneyAndCloseGate();                      // ����ֽ�Һ͹رϸ������տ�
	


	//virtual void OnAcceptMoneyOK();
	//virtual void OnReceiveMoneyOK();
	//void OnSuccessFinish();
	//void OnErrorFinish();
	void PrinterPrintIssueRecipt();
	//void OnCancel();
	bool hasFinishPaid();

	void UpdateReceptDlg(CString strTxt);						// ���³˿ͽ��浹��ʱ��ʾ
protected:
	void OnStart();
public:
	//virtual void OnReadCard(UINT,CRWReadCard*);
	virtual void OnError(CSysException e);
	void OnError(CString errorMsg = _T(""));

	
	//bool DoChange();                                     // ����

	RW_ISSUE_REQUEST GetIssueParameter();	

	bool ValidateCardInfo(RW_READ_CARD_RESPONSE&);
	void SaveBusinessData(RW_BUSSINESS_RESPONSE&);
	//void UpdateTHCount(SC_RSP_ISSUE_DATA& issueData);
	void PrinterPrintErrorReceipt();
	long BlockCard();
	CString GetCurrentUnitOfProduct();
	COIN_ACCEPTABLE GetAcceptCoinFaceTable();						// ��ȡ�������Ӳ�������Ϣ
	BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL);					// ��ȡ�������ֽ�������Ϣ
	void PlayAudioInDlg(DWORD eventID);								// �ڽ����в�������
public:
	bool m_bServiceNewStart;											// ��ǰ��Ʊservice�Ƿ���������
private:
	Model m_DataModel;
	vector<ACC_PRODUCT_t*> m_vecIssueProduct;
	vector<WORD> m_vecTokenPayment;
	vector<WORD> m_vecCardPayment;

	// ä�˹�Ʊ��
	LRESULT OnBlindBeginOperation(WPARAM wParam,LPARAM lParam);		// ��ʼ��Ʊ��Ӧ����
	//void	OnPinPadPressed(UINT,CTVMPinPadStartInput*);			// ������̻ص���Ӧ
	void	PlayEventAudio(DWORD eventID,bool bStopRepeat = false);	// ���������ӿ�	
	void	OnTimer(UINT nEventID);									// ������,��ʱ��ʱ���ظ���������
	void    TurnOnPinPadAndTimer(bool bIsStartPinPad = true,bool bIsStartOutOfTimer = true);	// ����������̼�ؼ�������ʱ��ʱ��

	BLIND_OPERATION_STATUS m_operation_status;						// ä�˹�Ʊ����״̬
	//UE_PINPADKEY m_PinpadInputType;									// �ϴ�����״̬
	CIssueAudioHelper* m_pIssueAudioHelper;							// ä�˹�Ʊ��������������ָ�����
	vector<int> m_vPassengerInput;									// ä����������
	DWORD m_dwLastPlayedVoice;										// ��һ�䲥�ŵ����ԣ��������Ž�������һ��ʱ���ڻ��ظ�������һ��
	std::map<int,DWORD> m_KeyValue;									// ��ǰ��Ӧ���ļ�ֵ��Ӧֵ
	bool m_bIsPinPadStop;											// ��ǰ��������Ƿ�ֹͣ�ˣ�
	int m_VoicePlayTime;
	tCHADevStatus m_CoinMoudelStatus;								// ��ǰӲ��ģ��״̬
	IO_RSP_STATUS_INFO m_IOMoudelStatus;							// ��ǰIOģ��״̬

	INVALIDE_TICKET_RECYCLETRADE m_invalide_ticket_trans;			// ��Ʊ���ס��������۳�Ʊ
	// �����Ʊ����
	void SaveInvalidateTicketBussiness(INVALIDE_TICKET_RECYCLETRADE& inBussinessDetail);

	//bool m_bNeedClickOkButton;										// �Ƿ���Ҫ�����ť������Ʊ��
	bool m_bStartAcceptCash;											// �Ƿ�ʼ�տ�?

public:
	long SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse);
	long EmptyCardInfo();
	long FinishEPPayment(LPRW_READ_CARD_RESPONSE);

	static DWORD WINAPI IssueTicketWorker(LPVOID lpParameter);
	long CIssueSvc::StartIssueThread();
};
