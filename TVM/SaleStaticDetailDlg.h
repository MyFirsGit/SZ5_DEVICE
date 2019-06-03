#pragma once
#include "OperationDlg.h"
#include "DataHelper.h"
#include "resource.h"
#define UD_MAX_ROW	15

/**
@brief 维护业务基本画面
*/
class CSaleStaticDetailDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CSaleStaticDetailDlg)
    DECLARE_MESSAGE_MAP()

public:
	CSaleStaticDetailDlg(CService* pService);
	~CSaleStaticDetailDlg();

	enum { IDD = IDD_10902_SALESTATIC_DETAIL };

	typedef enum _UE_NOTICEID
	{
	}UE_NOTICEID;
protected:
	void UpdateUI();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	
public:
	void showData();
	void setlabelgroup(const LABEL_GROUP_SIX* plabelgroup,const UINT uigroupnum);
	//void setlabelgroup(const LABEL_GROUP* plabelgroup,const UINT uigroupnum);
private:

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	LABEL_GROUP_SIX m_labelGroup[15];
	//LABEL_GROUP m_labelgroup[15];
};
