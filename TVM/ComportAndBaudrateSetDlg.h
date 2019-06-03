#pragma once
#include "OperationDlg.h"

const int const MAX_LEN = 15;		// �������ֵ

/**
@brief      ͨ�Ų��������û���
*/
class CComportAndBaudrateSetDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CComportAndBaudrateSetDlg)
	DECLARE_MESSAGE_MAP()

public:
	CComportAndBaudrateSetDlg(CService* pService);
	~CComportAndBaudrateSetDlg();

	enum { IDD = IDD_20601_OUT_DEVICE_MANEGE_DLG };

	
protected:
	virtual void UpdateUI();														// ������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);							// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// ���漤��

	LABEL_GROUP_THREE m_labelGroupThree[MAX_LEN];										// ������ʾ��ͨ�Ų�����Ϣ

	void ShowComportAndBaudRate();													// ��ʾ��������

	void ClearLableData();															// ������ʾ����
};
