#pragma once
#include "OperationDlg.h"

/**
@brief ��������
*/
class CCoinBoxManageAttachCoinCollectionBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageAttachCoinCollectionBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAttachCoinCollectionBoxDlg(CService* pService);
	~CCoinBoxManageAttachCoinCollectionBoxDlg();

	enum { IDD = IDD_20115_COMMUNICATION_SET_END_DLG };

	void InitAttachCoinCollectionBoxInfo();
	void ShowAttachCoinCollectionBoxInfo();					// ��ʾӲ�һ�����״̬
	void ShowDoAttachCoinCollectionBoxInfo();				// ��ʾӲ�һ����䰲װ״̬
	void SetColor(int row,CString status);					// ����״̬��ʾ����ɫ
protected:
	virtual void UpdateUI();								// ������ʾ��ɻص�����

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_FOUR m_lableGroup[15];
};