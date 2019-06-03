#pragma once 
#include "OperationDlg.h"
#include "DataInputSvc.h"
/**
@brief  ���ݵ���
*/
class  CDataInputSelectMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CDataInputSelectMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDataInputSelectMaintainDlg(CService* pService) ;
	~CDataInputSelectMaintainDlg() ;

	enum { IDD = IDD_20121_DATAINPUT_SELECT_DLG };

public:
	void UpdateUI();

protected:
	void InitDataInputUI();
	void UpdateDataInputUI();
	void Clear();
private:
	LABEL_GROUP_THREE  m_LableThree[_MAX_ROW];
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);                     //���洴������
	afx_msg void  OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);   //���漤��� 
};