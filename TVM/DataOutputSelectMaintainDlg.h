#pragma once 
#include "OperationDlg.h"
#include "DataOutputSvc.h"

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
public:
	void UpdateUI();			//���½���Ψһ�ӿ�

protected:	
	void InitOutputUI();        //��ʼ������
	void UpdateOutputData();    //���½�������
	void Clear();               //���������ʾ
private:
	LABEL_GROUP     m_LabelGroup[_MAX_ROW];
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);                     //���洴������
	afx_msg void  OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);   //���漤���
};