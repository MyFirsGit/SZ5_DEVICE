#pragma once 
#include "OperationDlg.h"

/**
@brief  �������-�������ѡ��
*/
class  CDataOutputSelectMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CDataOutputSelectMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDataOutputSelectMaintainDlg(CService* pService) ;
	~CDataOutputSelectMaintainDlg() ;
	enum { IDD = IDD_20131_DATAOUTPUT_SELECT_DLG };

private:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);						// ����ȷ�ϰ�ť��Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// ���崴������
};