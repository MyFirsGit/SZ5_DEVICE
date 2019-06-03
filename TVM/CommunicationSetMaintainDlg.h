#pragma once
#include "OperationDlg.h"

/**
@brief      ͨ�Ų��������û���
*/
class CCommunicationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCommunicationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationSetMaintainDlg(CService* pService);
	~CCommunicationSetMaintainDlg();

	enum { IDD = IDD_20114_COMMUNICATION_SET_DLG };

	void SetFromReadIniText();														// ��ȡMaintenance.INI�����ļ�����ʾ�ڶ�Ӧ����
	void InitlabelCommunicationInfo();												// ��ʼ��BASE����LABEL���������
	void ShowIPContent();															//������Ϣ����ʾ��Ӧ�����õ�����
protected:
	virtual void UpdateUI();														// ������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);							// ���洴������							
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// ���漤��

	LABEL_GROUP m_labelGroup[15];													// ������ʾ��ͨ�Ų�����Ϣ
};
