#pragma once 
#include "OperationDlg.h"

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

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 窗体创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// 处理确认按钮消息
	afx_msg LRESULT XButtonClick(WPARAM,LPARAM);
	void InitImportInfo();
	void ShowImportData();

	CXButton* m_btnPageUp;
	CXButton* m_btnPageDown;
	LABEL_GROUP_THREE m_LabelDataImportInfo[11];
	map<WORD,CDataInputSvc::ParameterInputStruct>::size_type m_DataNum;
	int m_TotalPageNum;
	int m_CurrentPageNum;
};