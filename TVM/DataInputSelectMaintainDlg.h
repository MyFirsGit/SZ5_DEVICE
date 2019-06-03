#pragma once 
#include "OperationDlg.h"
#include "DataInputSvc.h"
/**
@brief  数据导入
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
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);                     //画面创建函数
	afx_msg void  OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);   //画面激活函数 
};