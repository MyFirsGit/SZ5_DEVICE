#pragma once
#include "OperationDlg.h"
#include "DataHelper.h"
#define UD_MAX_ROW	15

#define UE_EC	TXT_TRANSDETAIL_ECCODE
#define UE_MTC	TXT_TRANSDETAIL_MTCCODE
#define UE_EXCEPTION	TXT_TRANSDETAIL_EXCEPTION
#define UE_SOLUTION		TXT_TRANSDETAIL_SOLUTION
#define UE_TRANSACTIONCODE	TXT_TRANSDETAIL_TRANSCODE
#define UE_TRANSACTIONTIME	TXT_TRANSDETAIL_TRANSTIME
#define UE_TRANSACTIONSTATUS	TXT_TRANSDETAIL_TRANSSTATUS
#define UE_PURCHASEOPERACTION	TXT_TRANSDETAIL_PURCHASE
#define UE_PURCHASEAMOUNT	TXT_TRANSDETAIL_PURCHASE_AMOUNT
#define UE_INSERTEDAMOUNT	TXT_TRANSDETAIL_ACCEPTED_AMOUNT
#define UE_TICKETRELEASED	TXT_TRANSDETAIL_RELEASED_TICKET
#define UE_TICKETUNRELEASED	TXT_TRANSDETAIL_UNRELEASED_TICKET
#define UE_WRITESTATUS	TXT_TRANSDETAIL_WRITE_STATUS
#define UE_CHANGEAMOUNT	TXT_TRANSDETAIL_CHANGE_AMOUNT
#define UE_CHANGEUNRELEASED	TXT_TRANSDETAIL_UNRELEASED_CHANGE_AMOUNT
#define UE_BANKNOTEACCEPTED	TXT_TRANSDETAIL_ACCEPTED_BANKNOTE
#define UE_COINACCEPTED	TXT_TRANSDETAIL_ACCEPTED_COIN
#define UE_RELEASECHANGE	TXT_TRANSDETAIL_RELEASED_COIN
#define UE_TRANSINFO	 TXT_TRANSDETAIL_INFO

#define UE_SUCCESSFUL TXT_TRANSDETAIL_SUCCESSFUL
#define UE_FAILED TXT_TRANSDETAIL_FAILED
#define UE_CANCELSUCCESSFUL TXT_TRANSDETAIL_CANCELSUCCESSFUL
#define UE_CANCELFAILED TXT_TRANSDETAIL_CANCELFAILED
#define UE_ISSUETOKEN TXT_TRANSDETAIL_ISSUETOKEN
#define UE_ISSUECARD TXT_TRANSDETAIL_ISSUECARD
#define UE_ISSUEPRODUCT TXT_TRANSDETAIL_ISSUEPRODUCT
#define UE_CHARGE TXT_TRANSDETAIL_CHARGE
#define UE_EXCEPTIONTIME TXT_TRANSDETAIL_EXCEPTIONTIME
#define UE_OPERATETYPE TXT_TRANSDETAIL_OPERATETYPE
#define UE_PRODUCTTYPE TXT_TRANSDETAIL_PRODUCTTYPE
#define UE_UNITPRICE TXT_TRANSDETAIL_UNITPRICE
#define UE_TAX TXT_TRANSDETAIL_TAX
#define UE_PAYABLE TXT_TRANSDETAIL_PAYABLE
#define UE_QUANTITY TXT_TRAMSDETAIL_QUANTITY
#define UE_DEPOSIT TXT_TRANSDETAIL_DEPOSIT
#define UE_COST TXT_TRANSDETAIL_COST

/**
@brief 维护业务基本画面
*/
class CTransactionHistoryBaseDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTransactionHistoryBaseDlg)
    DECLARE_MESSAGE_MAP()

public:
	CTransactionHistoryBaseDlg(CService* pService);
	~CTransactionHistoryBaseDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };

	typedef enum _UE_TRANSTYPE
	{
		UE_ISSUETOKE=1,
		UE_ISSUECARD,
		UE_RECHARGE,
		UE_ADDPRODUCT,
		UE_UNKNOWNTRANS
	}UE_TRANSTYPE;

	typedef enum _UE_OPERATE_TYPE
	{
		UE_ACCEPTED=1,
		UE_INSERT,
		UE_CHANGE
	}UE_OPERATE_TYPE;

	typedef enum money_operation
	{
		Cash_Pay,
		EP_Pay,
		Bank_pay
	}MONEY_OPERATION;
protected:
	void UpdateUI();
	/*afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	*/
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	void setlabelgroup(const LABEL_GROUP_SEVEN* plabelgroup,const UINT uigroupnum);
	void showDetail();
	void forceRefreshBaseInfoArea(); //强制刷新基础信息区
private:

	long m_get1YuanAcceptedNoteNum(TransactionRecord& transInfo);
	long m_get5YuanAcceptedNoteNum(TransactionRecord& transInfo);
	long m_get10YuanAcceptedNoteNum(TransactionRecord& transInfo);
	long m_get20YuanAcceptedNoteNum(TransactionRecord& transInfo);
	long m_get50YuanAcceptedNoteNum(TransactionRecord& transInfo);
	long m_get100YuanAcceptedNoteNum(TransactionRecord& transInfo);

	long m_get1YuanChangedNoteNum(TransactionRecord& transInfo);
	long m_get5YuanChangedNoteNum(TransactionRecord& transInfo);
	long m_get10YuanChangedNoteNum(TransactionRecord& transInfo);
	long m_get20YuanChangedNoteNum(TransactionRecord& transInfo);
	long m_get50YuanChangedNoteNum(TransactionRecord& transInfo);
	long m_get100YuanChangedNoteNum(TransactionRecord& transInfo);

	long m_get50AVOSAcceptedCoinNum(TransactionRecord& transInfo);
	long m_get1MOPAcceptedCoinNum(TransactionRecord& transInfo);
	long m_get2MOPAcceptedCoinNum(TransactionRecord& transInfo);
	long m_get5MOPAcceptedCoinNum(TransactionRecord& transInfo);

	long m_get50AVOSReleasedCoinNum(TransactionRecord& transInfo);
	long m_get1MOPReleasedCoinNum(TransactionRecord& transInfo);
	long m_get2MOPReleasedCoinNum(TransactionRecord& transInfo);
	long m_get5MOPReleasedCoinNum(TransactionRecord& transInfo);
	
	void m_showDetail(CTransactionHistorySvc::Model& dataModel,UE_TRANSTYPE ueTransType);
	void m_enterSJTDetail(CTransactionHistorySvc::Model& dataModel);
	void m_enterRechargeDetail(CTransactionHistorySvc::Model& dataModel);
	CString GetStrPayment(TransactionRecord& transInfo);// 检测当前资金操作是否被涉及到

private:
	int DeviceType;												// 设备类型
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	LABEL_GROUP_SEVEN m_labelGroup[15];
	void ClearUI();
};
