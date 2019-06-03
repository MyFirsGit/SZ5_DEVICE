#pragma once
#include "PassengerForeSvc.h"
#include "RWDef.h"
#include "TicketHelper.h"
#include "CAccTicketParam.h"

//static const TCHAR* const STR_ISSUE_INFO = _opl(TXT_ISSUE_INFO);

/**
@brief 售票业务Service
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
		bool  needBeginStation;				// ！参数限制：是否需要指定出发站
		bool  needEndStation;				// ！参数限制：是否需要指定目的站
		bool  needInputBeginStation;		// ！参数限制：销售时是否设置出发站
		bool  needInputEndStation;			// ！参数限制：销售时是否设置目的站
		bool  BeginStationIsCurrent;		// ！参数限制：销售时是否设置validityOrigin为当前站标志
		bool  EndStationIsCurrent;			// ！参数限制：销售时是否设置validityDestination为当前站标志
		bool  needCardDeposit;				// ！参数限制：可退款卡押金标记
		bool  needCardFee;					// ！参数限制：不可退款卡成本费标记

		BYTE ticketPiece;					// 售票张数
		BYTE issuedPiece;					// 已发售成功的张数

		//bool isCatchTicketsOk;				// 抓取票卡是否成功？
		//bool isReleaseTicketsOk;			// 放出票卡是否成功？
		//bool isReadWriteCardOk;				// 写卡是否成功？

		CACCCard* pCardInfo;						// ACC卡结构解析
		RW_READ_CARD_RESPONSE rwReadCardResponse;	// 读卡返回数据

		// Card箱信息
		CTCardCountInfo::CARD_BOX_INFO cardBoxAInfo;		// card箱A
		CTCardCountInfo::CARD_BOX_INFO cardBoxBInfo;		// card箱B
		CTCardCountInfo::CARD_BOX_INFO cardWasteBoxInfo;	// card废票箱
		//// Token箱信息
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenHopperAInfo;		// token箱A
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenHopperBInfo;		// token箱B
		//CTTokenCountInfo::TOKEN_BOX_INFO tokenWasteBoxInfo;	// token废票箱

		long refundMoney;					// 退款金额	//??

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
	void StartReceiveMoney();				// 开始收钱

	// 初始化
	void InitIssueProduct();
	vector<ACC_PRODUCT_t*>& GetIssueProduct();
	vector<WORD>& GetIssuePayment();
	void GetIssuePiece(map<int,CString>& mapPrice);
	bool CheckPayment(WORD payment, CardType_t cardtype);

	// 乘客操作
	void SetProductType(WORD type);
	void SetSubProductType(DWORD typeorprice);
	void SetBeginStation(DWORD locationNumber = LOCATION_NUMBER_UNSPECIFIED);
	void SetEndStation(DWORD locationNumber = LOCATION_NUMBER_UNSPECIFIED);
	void SetPassengerType(PassengerType_t type);
	void SetTicketPiece(WORD piece);
	void SetPayment(PaymentMethod_t paymentMethod);
	void SetTicketAmount(int nAmount);

	// 逻辑判断
	bool SupportSJT();
	bool NeedBeginSectionByPassenger();
	bool NeedEndSectionByPassenger();
	BYTE AutoCalcSaleMode();
	BYTE AutoCalcPriceMode();
	bool AutoCalcTicketPrice();
	void AutoCalcPaymentInfo();
	void DoneToNextDialog();

	// 业务处理
	void StartPayMoney();                             // 开始收钱
	bool AnalyzeTicket(long& resultCode, bool& needStopLoop);

	void StartIssueAndChange();
	bool IsBothCoinBanknoteMaxCount();
	bool IsBanknoteMaxAmount();
	bool IsBanknoteMaxCount();
	bool IsCoinMaxCount();
	bool IsCoinChageMaxCount();

	// 纸币找零数量判断
	bool IsBHChangeMaxCount();
	bool IsBHChangeMaxAmout();

	CString BanknoteShowword();
	CString CoinShowword();

	void FlowToNetPayDlg();
	void FlowToErrorDlg();
	void FlowToFinishDlg();								// 转向完成界面
	void FlowToDefaultDlg();							// 转向默认界面,无硬件动作时

public:
	// 发售车票
	bool IssueTicketProc();
	bool IssueOneTicket();
	bool IssueOneToken();
	bool IssueOneCard();
	long ReadWriteToken(RW_BUSSINESS_RESPONSE& rwBusinessResponse);
	long ReadWriteCard(RW_BUSSINESS_RESPONSE& rwBusinessResponse);

	// 收钱找零

	// 失败处理
	//void OnIssueAndChangeError();
	void OnReceiveError();
	
	
	void AcceptMoneyAndCloseGate();                      // 收纳纸币和关毕各个接收口
	


	//virtual void OnAcceptMoneyOK();
	//virtual void OnReceiveMoneyOK();
	//void OnSuccessFinish();
	//void OnErrorFinish();
	void PrinterPrintIssueRecipt();
	//void OnCancel();
	bool hasFinishPaid();

	void UpdateReceptDlg(CString strTxt);						// 更新乘客界面倒计时提示
protected:
	void OnStart();
public:
	//virtual void OnReadCard(UINT,CRWReadCard*);
	virtual void OnError(CSysException e);
	void OnError(CString errorMsg = _T(""));

	
	//bool DoChange();                                     // 找零

	RW_ISSUE_REQUEST GetIssueParameter();	

	bool ValidateCardInfo(RW_READ_CARD_RESPONSE&);
	void SaveBusinessData(RW_BUSSINESS_RESPONSE&);
	//void UpdateTHCount(SC_RSP_ISSUE_DATA& issueData);
	void PrinterPrintErrorReceipt();
	long BlockCard();
	CString GetCurrentUnitOfProduct();
	COIN_ACCEPTABLE GetAcceptCoinFaceTable();						// 获取允许接收硬币面额信息
	BRH_FACE_ACEPTED_TABLE GetAcceptBanknotesFace(tBncSetInhibitList* table = NULL);					// 获取允许接收纸币面额信息
	void PlayAudioInDlg(DWORD eventID);								// 在界面中播放语音
public:
	bool m_bServiceNewStart;											// 当前售票service是否重新启动
private:
	Model m_DataModel;
	vector<ACC_PRODUCT_t*> m_vecIssueProduct;
	vector<WORD> m_vecTokenPayment;
	vector<WORD> m_vecCardPayment;

	// 盲人购票用
	LRESULT OnBlindBeginOperation(WPARAM wParam,LPARAM lParam);		// 开始售票响应函数
	//void	OnPinPadPressed(UINT,CTVMPinPadStartInput*);			// 密码键盘回调响应
	void	PlayEventAudio(DWORD eventID,bool bStopRepeat = false);	// 播放语音接口	
	void	OnTimer(UINT nEventID);									// 计数器,计时超时后重复播放语音
	void    TurnOnPinPadAndTimer(bool bIsStartPinPad = true,bool bIsStartOutOfTimer = true);	// 启动密码键盘监控及操作超时计时器

	BLIND_OPERATION_STATUS m_operation_status;						// 盲人购票操作状态
	//UE_PINPADKEY m_PinpadInputType;									// 上次输入状态
	CIssueAudioHelper* m_pIssueAudioHelper;							// 盲人购票语音播放助理类指针对象
	vector<int> m_vPassengerInput;									// 盲人输入数字
	DWORD m_dwLastPlayedVoice;										// 上一句播放的语言，语音播放结束后再一定时间内会重复播放上一句
	std::map<int,DWORD> m_KeyValue;									// 当前对应语句的键值对应值
	bool m_bIsPinPadStop;											// 当前密码键盘是否停止了？
	int m_VoicePlayTime;
	tCHADevStatus m_CoinMoudelStatus;								// 当前硬币模块状态
	IO_RSP_STATUS_INFO m_IOMoudelStatus;							// 当前IO模块状态

	INVALIDE_TICKET_RECYCLETRADE m_invalide_ticket_trans;			// 废票交易、回收已售车票
	// 保存废票交易
	void SaveInvalidateTicketBussiness(INVALIDE_TICKET_RECYCLETRADE& inBussinessDetail);

	//bool m_bNeedClickOkButton;										// 是否需要点击按钮继续发票？
	bool m_bStartAcceptCash;											// 是否开始收款?

public:
	long SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse);
	long EmptyCardInfo();
	long FinishEPPayment(LPRW_READ_CARD_RESPONSE);

	static DWORD WINAPI IssueTicketWorker(LPVOID lpParameter);
	long CIssueSvc::StartIssueThread();
};
