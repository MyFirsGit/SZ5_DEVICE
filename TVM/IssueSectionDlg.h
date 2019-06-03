#pragma once
#include "OperationDlg.h"
#include "IssueSvc.h"
#include "ButtonGroup.h"
/**
@brief      售票区段选择画面
*/

//区段的编码0x11FF.;
#define LINE_AREA_CODE	0x11FF 

class CIssueSvc;

class CIssueSectionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CIssueSectionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueSectionDlg(CService*);
	~CIssueSectionDlg();

	enum { IDD = IDD_00102_ISSUE_CONFIRM_DLG };

	enum {MAX_TICKET_INFO_ROWS = 13};

	typedef enum _tagIssueComponents
	{
		SUB_PRODUCT_BUTTON	 = 0x0001,
		BEGIN_SECTION_BUTTON = 0x0002,
		END_SECTION_BUTTON   = 0x0004,
	};

public:
	void ClearLabelData();
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);
	LRESULT OnSelectSubProduct(WPARAM wParam,LPARAM lParam);     // 选择子产品按钮消息
	LRESULT OnBeginSectionClick(WPARAM wParam,LPARAM lParam);	 // 选择开始车站
	LRESULT OnEndSectionClick(WPARAM wParam,LPARAM lParam);		 // 选择结束车站

	void InitializeBaseInfo();									 // 初始化基本信息区
	void InitializeComponents(long lComponents);				 // 初始化按钮组
	void ShowGuideMsg();
	
private:
	CIssueSvc* pSvc;
	LABEL_GROUP m_TicketInfoGroup[MAX_TICKET_INFO_ROWS];
};