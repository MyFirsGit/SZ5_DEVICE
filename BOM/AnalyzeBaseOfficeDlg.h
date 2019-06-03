#pragma once
#include "OperationDlg.h"

// CAnalyzeBaseOfficeDlg 对话框
/**
@brief      分析基本画面
*/
class CAnalyzeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CAnalyzeBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CAnalyzeBaseOfficeDlg(CService* pService);   // 标准构造函数
	virtual ~CAnalyzeBaseOfficeDlg();

	enum { IDD = IDD_00802_ANALYZE_BASE_DLG };


protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateUI();
private:
	enum {MAX_LABEL_GROUP_COUNT = 15};
	LRESULT OnRight1GroupClick(WPARAM wParam,LPARAM lParam);
	LABEL_GROUP m_TicketInfo[MAX_LABEL_GROUP_COUNT];
	LABEL_GROUP_THREE m_historyGUIInfo[MAX_LABEL_GROUP_COUNT];
	void ShowCardInfo(CCPUCard* pCPUCard);
	void ShowCardInfo(CULCard* pULCard);
	void ShowHistory(CCPUCard* pCPUCard);
	void ShowHistory(CULCard* pULCard);
	void ShowPersonal(CCPUCard* pCPUCard);
	
};
