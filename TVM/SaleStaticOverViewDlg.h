#pragma once
#include "OperationDlg.h"
#include "DataHelper.h"
#include "resource.h"
#define UD_MAX_ROW	15

/**
@brief 维护业务基本画面
*/
class CSaleStaticOverViewDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CSaleStaticOverViewDlg)
    DECLARE_MESSAGE_MAP()

public:
	CSaleStaticOverViewDlg(CService* pService);
	~CSaleStaticOverViewDlg();

	enum { IDD = IDD_10901_SALESTATIC_OVERVIEW };

	typedef enum _UE_NOTICEID
	{
	}UE_NOTICEID;
protected:
	void UpdateUI();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	
public:
	void setlabelgroup(const LABEL_GROUP_FIVE* plabelgroup,const UINT uigroupnum);

	void showData();
private:

private:
	int DeviceType;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	LABEL_GROUP_FIVE m_labelGroup[15];
	//LABEL_GROUP m_labelgroup[15];

	
};
