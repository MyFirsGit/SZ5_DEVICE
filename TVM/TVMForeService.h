#pragma once
#include "ForeService.h"
#include "tvmdialogflow.h"
#include "ECTCardInfo.h"
#include "ACCCardInfo.h"
#include "CPUCardInfo.h"

#include "RWMessage.h"
#include "CHCommand.h"
#include "BRHMessage.h"
//#include "BHCommand.h"
#include "BHChangeCommand.h"
#include "IOCommand.h"
#include "CHdef.h"
#define  INVALID_ID _T("FFFFFFFF")

class CTVMForeService:public CForeService
{
	DECLARE_MESSAGE_MAP()
public:

	CRITICAL_SECTION		m_critical_Section;
	typedef struct _tagIssuePayDetailInfo
	{
		DWORD dueAmount;
		DWORD changeAmount;

		bool hasStartPaid;                                // 支付完成标志
		bool hasFinishPaid;                               // 支付完成标志

		bool isCionOpen;
		bool isBankNoteOpen;
		bool isBankCardOpen;
		bool isWaitingEPCard;      // 是否正在读取EP卡
		bool isCionCollected;
		bool isBankNoteCollected;
		bool isCoinCharged;

		map<BankNoteAndCoinType_t,int> PayBankNoteInfo;
		map<BankNoteAndCoinType_t,int> PayCoinInfo;
		map<BankNoteAndCoinType_t,int> ChargeCionInfo;

		DWORD CHReceiveMoney;
		DWORD BHReceiveMoney;
		DWORD BankReceiveMoney;
		DWORD EPReceiveMoney;

		DWORD ChangeStockCount;
		DWORD ChangeHopper1Count;
		DWORD ChangeHopper2Count;

		BYTE BankCardNum[10];	                    // 银行卡号	
		BYTE EPCardNum[8];	                        // 储值卡号	

		_tagIssuePayDetailInfo(){
			Initialize();
		}

		void Initialize(){
			isCionOpen = false;
			isBankNoteOpen = false;
			isBankCardOpen = false;
			isWaitingEPCard = false;
			isCionCollected = false;
			isBankNoteCollected = false;
			isCoinCharged = false;
			hasFinishPaid = false;
			hasStartPaid = false;

			dueAmount = 0;
			changeAmount = 0;
			CHReceiveMoney = 0;
			BHReceiveMoney = 0;
			BankReceiveMoney = 0;
			EPReceiveMoney = 0;
			ChangeStockCount = 0;
			ChangeHopper1Count = 0;
			ChangeHopper2Count = 0;
			memset(BankCardNum,0,10);
			memset(EPCardNum,0,10);
		}

	} IssuePayDetailInfo;

	IssuePayDetailInfo issuePayDetailInfo;          // 支付详细信息

	CTVMForeService(SERVICE_ID serviceID, SERVICE_ID parentServiceID = MAIN_SVC);      // 构造函数
	~CTVMForeService();                                                                // 析构函数
	virtual void OnStart();                                                         // Service启动后处理
	virtual bool OnStop();                                                          // Service停止前处理
	void DoDialogFlow(const TCHAR* pButtonMsg);                                      // 处理画面跳转按钮消息
	UINT GetCurDialogGroupID();                                                     // 获取当前Dialog组编号
	COperationDlg* GetCurOperationDlg();												 // 获取当前Dialog;
	CReceptionDlg* GetCurReceptionDlg();												 // 获取当前Dialog;
	UINT GetPreDialogGroupID();                                                     // 获取前一组Dialog组编号
	void ShowCurDialogGroup();                                                      // 显示当前Dialog组
	void HideCurDialogGroup();                                                      // 隐藏当前Dialog组
	virtual void NotifyDataModelChanged();
	CTVMDialogFlow* GetDialogFlow();
	
	virtual void OnBusy(bool busy);
	//CString GetCompleteGuide(ResultCode_t resultCode,OPERATION_MSG operationMsg);   // 获取拼接后的文言

	// 翻页功能
	void SetPageInfo(int total, int current = 1, int per = 15);	// 设置翻页信息
	int GetPerPage() const;										// 获取每页条数
	int GetTotalPage() const;									// 获取总计页数
	int GetCurrentPage() const;									// 获取当前页码
	int GetPagePrefix();										// 获取当前页的前缀条数，例如：第3页第1条的前缀条数为每页条数*前面的页数（2）

	static BankNoteAndCoinType_t TransCoinType(int value);	  // 币值转换
	static BankNoteAndCoinType_t TransBanknoteType(int value);// 纸币值转换
	static DWORD TransCoinValue (int value);				//币值转换 BIN  0：无效，1：0.5元，2：1元
	static DWORD TransBankNoteValue(int value);				//纸币面额转换 1：1元，5：5元，10：10元，20：20元，50：50元，100：100元
	static CString ShowCoinValue (BYTE value);				//Dlg用 BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
	static CString  CashValueToString(BankNoteAndCoinType_t value);// 现金面值	转	字符串
	bool m_isSecondary;
protected:

	virtual LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Clear] 键按下消息（删除）
	virtual LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Enter] 键按下消息（执行）
	virtual LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F2/Alt] 键按下消息（保存）
	virtual LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F3/Ctrl] 键按下消息（返回）
	virtual LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Esc] 键按下消息（主菜单）
	virtual LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F5] 键按下消息（上一页）
	virtual LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F6] 键按下消息（下一页）

	//~~~~~~~~~~~~ delete detailed definition later later
	virtual void OnReadCard(UINT,CRWReadCard*);
	virtual void OnCoinInserted(UINT,CCHReadReceivs*); 
	virtual void OnCoinChanged(UINT,CCHChange*);
	virtual void OnCoinReceived(UINT,CCHAccept*);
	virtual void OnCoinReturned(UINT,CCHRefund*);
	virtual void OnBanknoteInserted(UINT,CBRHOpenAndValidate*);
	virtual void OnBanknoteReturned(UINT,CBRHCashInRollback*);
	virtual void OnBanknoteCollect(UINT,CBRHCashInEnd*);
	virtual void OnBanknoteChangeSendOut(UINT,CBRHDispenseRequest*);			// 纸币找零回调函数
	//virtual void OnChargeInsertCard(UINT,CIOCardPosMonitor*);					// 充值乘客插卡回调函数

	// 业务层公共函数
	// all delete detailed definition later
	virtual bool StartReceiveCash();                             // 开始接受现金（纸币，硬币）
	virtual void StartReceiveBankCard();                         // 开始银行卡支付
	virtual void StartReadEPCard();                              // 开始读储值卡	
	virtual bool StartAcceptCashMoney();                         // 关口和压钞
	virtual bool StartChange();                                  // 找零
	virtual bool StartReturnCash();                              // 退币
	//?? currently there are two AcceptMoneyAndCloseGate() defined seperately at issueSvc and chargeSvc
	//?? but it is actually undefined in TVMForeService.cpp ... ...
	//virtual void AcceptMoneyAndCloseGate();                      // 收纳纸币和关毕各个接收口
	virtual bool IsReceiveEnoughMoney();							// 是否完成支付
	virtual DWORD GetCoinAcceptCount();								// 获取硬币接受个数
	virtual DWORD GetBankNoteAcceptCount();							// 获取纸币接受个数
	virtual void CloseAllDevice();									// 关闭所有硬件

	virtual void OnError(CSysException e);

	// all delete later
	virtual void OnOverMoneyCountLimit();
	virtual void OnOverMoneyAmountLimit();
	virtual void OnOverChangeCountLimit();
	virtual void OnAcceptMoneyOK();	
	virtual void OnReceiveMoneyOK();	
	virtual void OnReturnCashOK();	

	// all delete later
	//virtual void RenewDbWhileCoinInsert(CH_RSP_ACCEPT_INSERT_INFO chAmountInfo);
	virtual void RenewDbWhileBanknoteInsert(BRH_STATUS bh_status);
	virtual bool IsBothCoinBanknoteMaxCount();
	virtual bool IsBanknoteMaxCount();
	virtual bool IsBanknoteMaxAmount();
	
private:

	// delete later
	virtual long GetCoinAcceptAmount(CH_RSP_ACCEPT_INSERT_INFO chAmountInfo);  // 获取硬币接受金额
	virtual long GetBankNoteAcceptAmount(BRH_STATUS bh_status);                 // 获取纸币接受金额
	virtual bool CalculateChangeCount();										// 从changeAmount计算changeCount

	void OnReadCardEx(UINT,CCommand*);
	void OnCoinInsertedEx(UINT,CCommand*);
	void OnCoinChangedEx(UINT,CCommand*);
	void OnCoinReceivedEx(UINT,CCommand*);
	void OnCoinReturnedEx(UINT,CCommand*);
	void OnBanknoteInsertedEx(UINT,CCommand*);
	void OnBanknoteReturnedAllEx(UINT,CCommand*);
	void OnBanknoteReturnedBuffEx(UINT,CCommand*);
	void OnBanknoteCollectEx(UINT,CCommand*);
	void OnBanknoteChangeSendOutEx(UINT,CCommand*);
	void OnChargeCardInsertEx(UINT,CCommand*);

	LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/);			// 处理 [0~9] 键按下消息（数字）
	LRESULT OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [.] 键按下消息（小数点）
	//LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Esc] 键按下消息（主菜单）
	LRESULT OnKeyboardF1(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F1] 键按下消息（未使用）
	LRESULT OnKeyboardF4(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F4] 键按下消息（未使用）

private:
	bool m_support_page;
	int m_current_page;
	int m_total_page;
	int m_per_page;
};