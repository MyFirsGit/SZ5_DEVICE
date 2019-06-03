#pragma once
#include "OperationDlg.h"
#include "CoinAddingBoxSetSvc.h"

/**
@brief      ������������û���
*/
class CCoinAddingBoxMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinAddingBoxMaintainDlg)
	DECLARE_MESSAGE_MAP()
public:
	CCoinAddingBoxMaintainDlg(CService* pService);
	~CCoinAddingBoxMaintainDlg();
	
	enum{IDD = IDD_20112_STATION_SET_DLG};
	
	void InitlabelAddingBoxInfo();											//��ʼ�����������label��Ϣ
	void SetFromReadIniText();												//��ʾ��TVM.INI��ȡ�������ļ��ڶ�Ӧ����
	void ShowGuideDisplay();												// ������ʾ��ʾ����
	void ShowCodeContent();													//������Ϣ����ʾ��Ӧ�����õ�����
protected:
	virtual void UpdateUI();												//������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// ���漤��
	LABEL_GROUP_THREE m_labelGroup[15];										// ������ʾ�ĳ�վ��Ϣ
};