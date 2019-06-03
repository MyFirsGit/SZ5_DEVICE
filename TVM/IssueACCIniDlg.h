#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"
#include "Sync.h"

class CIssueSvc;

/**
@brief ��Ʊ������Ϣ����
*/
class CIssueACCIniDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CIssueACCIniDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueACCIniDlg(CService*);
	~CIssueACCIniDlg();

	enum { IDD = IDD_10101_ISSUE_DLG };

	// �����������
	enum {
		BASE_NONE	   = 0,
		BASE_INFO_ARAE = 1,
		BASE_CASH_ARAE = 2,
	};
	typedef enum _tagIssueComponents
	{
		PASSAGETYPE = 0x0001,
		PAYTYPE = 0x0002,
		PIECE_BUTTON = 0x0004,
	};

	enum {MAX_TICKET_INFO_ROWS = 23};
public:
	void ClearLabelData();
	void ShowCoinMaxCount();	 // ��ʾӲ�������ﵽ�������
	void ShowBanknoteMaxCount(); // ��ʾֽ�������ﵽ�������
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);
	LRESULT OnPassagerTypeClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPieceClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPayTypeClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPrint(WPARAM wParam,LPARAM lParam);

	void InitializeBaseInfo();
	void InitializeFunctionBtns(long lComponents);
	void InitializeGuideArea();
	bool ValidateInput();
	void ShowGuideMsgOfPayInfo();					// ��ʾ֧����Ϣ  
	bool m_bIsShowCoinMaxCount;		// �Ƿ���ʾӲ���������
	bool m_bIsShowBanknoteMaxCount;// �Ƿ���ʾֽ���������
	bool m_bIsShowFinshedInsertMoney;// �Ƿ����ͶǮ

	bool m_bShowOkButtonAndPayMented;// �Ƿ���ʾȷ�ϰ�ť��֧����ʽ��
private:
	CIssueSvc* pSvc;
	LABEL_GROUP m_TicketInfoGroup[MAX_TICKET_INFO_ROWS];
	bool m_bConfirmPayment;
};