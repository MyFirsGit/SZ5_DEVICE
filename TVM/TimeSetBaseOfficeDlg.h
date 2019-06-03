#pragma once
#include "OperationDlg.h"

/**
@brief  ʱ���趨
*/
class  CTimeSetBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CTimeSetBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTimeSetBaseOfficeDlg(CService*pService) ;
	~CTimeSetBaseOfficeDlg() ;

	enum { IDD = IDD_01801_TIMESET_BASE_DLG };

	void InitlabelTimeInfo();								// ��ʼ��BASE����LABEL���������
	void ShowCodeContent();									// ������Ϣ����ʾ��Ӧ�����õ�����										
protected:
	virtual void UpdateUI();								// ������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// ���漤��
	LABEL_GROUP m_labelGroup[15];							// ������ʾ��ʱ����Ϣ
};