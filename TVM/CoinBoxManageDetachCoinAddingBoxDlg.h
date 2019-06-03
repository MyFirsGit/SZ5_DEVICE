#pragma once
#include "OperationDlg.h"

/**
@brief �ӱ���ж��
*/
class CCoinBoxManageDetachAddingBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageDetachAddingBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachAddingBoxDlg(CService* pService);
	~CCoinBoxManageDetachAddingBoxDlg();

	enum { IDD = IDD_20111_MACHINE_PARAMETER_SET_DLG };

	void InitDetachCoinAddingBoxInfo();
	void ShowDetachCoinAddingBoxInfo();							// ��ʾӲ�Ҳ�����״̬
	void ShowDoDetachCoinAddingBoxInfo();						// ��ʾӲ�Ҳ�����ж��״̬
	void SetColor(int row,CString status);						// ����״̬��ʾ����ɫ
protected:
	virtual void UpdateUI();									// ������ʾ��ɻص�����

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_FOUR m_lableGroup[15];
}; 