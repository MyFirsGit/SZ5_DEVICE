#pragma once
#include "OperationDlg.h"


#define _MAX_ROW 15
#define _MAX_COL 4

class COneKeySettlementDlg : public COperationDlg
{
	DECLARE_DYNAMIC(COneKeySettlementDlg)
	DECLARE_MESSAGE_MAP()

public:
	COneKeySettlementDlg(CService* pService);				// ��׼���캯��
	~COneKeySettlementDlg();

	// �Ի�������
	enum { IDD = IDD_20701_REMOVE_EXCEPTION_DLG };

public:
	void UpdateUI();                        //ˢ�½��棨�ⲿ�ӿڣ�
	void UpdateLabelData();					// ǿ����ʾ����
protected:
	void InitDialogUI();                    //��ʼ����
private:
	LABEL_GROUP_FIVE  m_LableGroupFour[_MAX_ROW];

	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // ���漤��
};