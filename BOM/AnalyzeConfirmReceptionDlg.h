#pragma once
#include "ReceptionDlg.h"
#include "ReceptionStaticInfo.h"

// CAnalyzeConfirmReceptionDlg 对话框

/**
@brief      分析确认画面
*/
class CAnalyzeConfirmReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CAnalyzeConfirmReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CAnalyzeConfirmReceptionDlg(CService* pService);   // 标准构造函数
	virtual ~CAnalyzeConfirmReceptionDlg();
	
	void SetReadCardResponse(LPRW_READ_CARD_RESPONSE);

// 对话框数据
	enum { IDD = IDD_10802_ANALYZE_CONFIRM_DLG };
  
protected:
	void UpdateUI(); 
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
