#pragma once
#include "OperationDlg.h"

/**
@brief  ��Ļ�����趨
*/
class  CScreenBrightnessSetDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CScreenBrightnessSetDlg)
	DECLARE_MESSAGE_MAP()

public:
	CScreenBrightnessSetDlg(CService*pService) ;
	~CScreenBrightnessSetDlg() ;

	enum { IDD = IDD_01801_TIMESET_BASE_DLG };

	void InitlabelScreenBrightnessInfo();					// ��ʼ��BASE����LABEL���������
	void ShowCodeContent();									// ������Ϣ����ʾ��Ӧ�����õ�����
protected:
	virtual void UpdateUI();								// ������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // ���漤��
	LABEL_GROUP m_labelGroup[15];							// ������ʾ��ʱ����Ϣ
};