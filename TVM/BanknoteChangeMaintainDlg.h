#pragma once
#include "OperationDlg.h"
#include "BanknoteChangeModuleTestSvc.h"
/**
@brief ��������
*/


class CBanknoteChangeMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteChangeMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteChangeMaintainDlg(CService* pService);
	~CBanknoteChangeMaintainDlg();

	enum { IDD = IDD_00501_EXCHANGE_BASE_DLG };

public:
	void UpdateUI();                    //ˢ�½���
protected: 
	void InitMenuUI();                  //��ʼ��Menu����
	void UpdateMenuData();              //����Menu����
	void Clear();
private:
	LABEL_GROUP_FOUR m_labelGroupFour[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};