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

		bool hasStartPaid;                                // ֧����ɱ�־
		bool hasFinishPaid;                               // ֧����ɱ�־

		bool isCionOpen;
		bool isBankNoteOpen;
		bool isBankCardOpen;
		bool isWaitingEPCard;      // �Ƿ����ڶ�ȡEP��
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

		BYTE BankCardNum[10];	                    // ���п���	
		BYTE EPCardNum[8];	                        // ��ֵ����	

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

	IssuePayDetailInfo issuePayDetailInfo;          // ֧����ϸ��Ϣ

	CTVMForeService(SERVICE_ID serviceID, SERVICE_ID parentServiceID = MAIN_SVC);      // ���캯��
	~CTVMForeService();                                                                // ��������
	virtual void OnStart();                                                         // Service��������
	virtual bool OnStop();                                                          // Serviceֹͣǰ����
	void DoDialogFlow(const TCHAR* pButtonMsg);                                      // ��������ת��ť��Ϣ
	UINT GetCurDialogGroupID();                                                     // ��ȡ��ǰDialog����
	COperationDlg* GetCurOperationDlg();												 // ��ȡ��ǰDialog;
	CReceptionDlg* GetCurReceptionDlg();												 // ��ȡ��ǰDialog;
	UINT GetPreDialogGroupID();                                                     // ��ȡǰһ��Dialog����
	void ShowCurDialogGroup();                                                      // ��ʾ��ǰDialog��
	void HideCurDialogGroup();                                                      // ���ص�ǰDialog��
	virtual void NotifyDataModelChanged();
	CTVMDialogFlow* GetDialogFlow();
	
	virtual void OnBusy(bool busy);
	//CString GetCompleteGuide(ResultCode_t resultCode,OPERATION_MSG operationMsg);   // ��ȡƴ�Ӻ������

	// ��ҳ����
	void SetPageInfo(int total, int current = 1, int per = 15);	// ���÷�ҳ��Ϣ
	int GetPerPage() const;										// ��ȡÿҳ����
	int GetTotalPage() const;									// ��ȡ�ܼ�ҳ��
	int GetCurrentPage() const;									// ��ȡ��ǰҳ��
	int GetPagePrefix();										// ��ȡ��ǰҳ��ǰ׺���������磺��3ҳ��1����ǰ׺����Ϊÿҳ����*ǰ���ҳ����2��

	static BankNoteAndCoinType_t TransCoinType(int value);	  // ��ֵת��
	static BankNoteAndCoinType_t TransBanknoteType(int value);// ֽ��ֵת��
	static DWORD TransCoinValue (int value);				//��ֵת�� BIN  0����Ч��1��0.5Ԫ��2��1Ԫ
	static DWORD TransBankNoteValue(int value);				//ֽ�����ת�� 1��1Ԫ��5��5Ԫ��10��10Ԫ��20��20Ԫ��50��50Ԫ��100��100Ԫ
	static CString ShowCoinValue (BYTE value);				//Dlg�� BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
	static CString  CashValueToString(BankNoteAndCoinType_t value);// �ֽ���ֵ	ת	�ַ���
	bool m_isSecondary;
protected:

	virtual LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Clear] ��������Ϣ��ɾ����
	virtual LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Enter] ��������Ϣ��ִ�У�
	virtual LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F2/Alt] ��������Ϣ�����棩
	virtual LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F3/Ctrl] ��������Ϣ�����أ�
	virtual LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Esc] ��������Ϣ�����˵���
	virtual LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F5] ��������Ϣ����һҳ��
	virtual LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F6] ��������Ϣ����һҳ��

	//~~~~~~~~~~~~ delete detailed definition later later
	virtual void OnReadCard(UINT,CRWReadCard*);
	virtual void OnCoinInserted(UINT,CCHReadReceivs*); 
	virtual void OnCoinChanged(UINT,CCHChange*);
	virtual void OnCoinReceived(UINT,CCHAccept*);
	virtual void OnCoinReturned(UINT,CCHRefund*);
	virtual void OnBanknoteInserted(UINT,CBRHOpenAndValidate*);
	virtual void OnBanknoteReturned(UINT,CBRHCashInRollback*);
	virtual void OnBanknoteCollect(UINT,CBRHCashInEnd*);
	virtual void OnBanknoteChangeSendOut(UINT,CBRHDispenseRequest*);			// ֽ������ص�����
	//virtual void OnChargeInsertCard(UINT,CIOCardPosMonitor*);					// ��ֵ�˿Ͳ忨�ص�����

	// ҵ��㹫������
	// all delete detailed definition later
	virtual bool StartReceiveCash();                             // ��ʼ�����ֽ�ֽ�ң�Ӳ�ң�
	virtual void StartReceiveBankCard();                         // ��ʼ���п�֧��
	virtual void StartReadEPCard();                              // ��ʼ����ֵ��	
	virtual bool StartAcceptCashMoney();                         // �ؿں�ѹ��
	virtual bool StartChange();                                  // ����
	virtual bool StartReturnCash();                              // �˱�
	//?? currently there are two AcceptMoneyAndCloseGate() defined seperately at issueSvc and chargeSvc
	//?? but it is actually undefined in TVMForeService.cpp ... ...
	//virtual void AcceptMoneyAndCloseGate();                      // ����ֽ�Һ͹رϸ������տ�
	virtual bool IsReceiveEnoughMoney();							// �Ƿ����֧��
	virtual DWORD GetCoinAcceptCount();								// ��ȡӲ�ҽ��ܸ���
	virtual DWORD GetBankNoteAcceptCount();							// ��ȡֽ�ҽ��ܸ���
	virtual void CloseAllDevice();									// �ر�����Ӳ��

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
	virtual long GetCoinAcceptAmount(CH_RSP_ACCEPT_INSERT_INFO chAmountInfo);  // ��ȡӲ�ҽ��ܽ��
	virtual long GetBankNoteAcceptAmount(BRH_STATUS bh_status);                 // ��ȡֽ�ҽ��ܽ��
	virtual bool CalculateChangeCount();										// ��changeAmount����changeCount

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

	LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/);			// ���� [0~9] ��������Ϣ�����֣�
	LRESULT OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [.] ��������Ϣ��С���㣩
	//LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Esc] ��������Ϣ�����˵���
	LRESULT OnKeyboardF1(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F1] ��������Ϣ��δʹ�ã�
	LRESULT OnKeyboardF4(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F4] ��������Ϣ��δʹ�ã�

private:
	bool m_support_page;
	int m_current_page;
	int m_total_page;
	int m_per_page;
};