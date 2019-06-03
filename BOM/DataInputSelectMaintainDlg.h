#pragma once 
#include "OperationDlg.h"

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

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���崴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// ����ȷ�ϰ�ť��Ϣ
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