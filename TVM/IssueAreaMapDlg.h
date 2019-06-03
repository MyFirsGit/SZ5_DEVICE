#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"
#include "CGuiParm.h"
#include "AllLineShowBox.h"

class CIssueSvc;

/**
@brief 售票路网图画面（默认待机画面）
*/
class CIssueAreaMapDlg : public CReceptionDlg
{
public:
	DECLARE_DYNAMIC(CIssueAreaMapDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueAreaMapDlg(CService*);
	~CIssueAreaMapDlg();
	
	// 对话框数据
	enum { IDD = IDD_10101_ISSUE_DLG };

	// 售票右侧功能区按钮组组件
	typedef enum _tagIssueComponents
	{
		LINE_SELECT_BUTTON			= 0x0001,	// 线路按钮
		PRICE_BUTTON				= 0x0002,	// 票价按钮
		NUMBER_BUTTON				= 0x0004,	// 张数按钮
		TOP_STATION_BUTTON			= 0x0008,	// 热点车站按钮		
		FUNC_SELECT_BUTTON			= 0x0010,	// 选择功能按钮
	};

	enum {MAX_TICKET_INFO_ROWS = 18};

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnProductTypeClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnSubProductClick(WPARAM wParam,LPARAM lParam);
	LRESULT XLineGroupButtonClick(WPARAM wParam, LPARAM lParam);

	// 当前用
	LRESULT OnLineGroupClick(WPARAM wParam,LPARAM lParam);		// 线路选择
	LRESULT OnPriceTypeClick(WPARAM wParam,LPARAM lParam);		// 票价选择
	LRESULT OnTicketNumClick(WPARAM wParam,LPARAM lParam);		// 车票数量选择
	LRESULT OnTopStationClick(WPARAM wParam,LPARAM lParam);		// 热点车站选择
	LRESULT OnFunctionClick(WPARAM wParam,LPARAM lParam);		// 功能选择

	bool IsSupportIssueCard();

protected:
	void OnClientLanguageModeChanged(LANGUAGETYPE_T&);	  //语言变化绘制函数
	//void changeUiLanguage(LANGUAGETYPE_T&);
private:
	void ClearLabelData();
private:
	CIssueSvc* pSvc;
	CAllLineShowBox* m_pAllLineDlg;
};
