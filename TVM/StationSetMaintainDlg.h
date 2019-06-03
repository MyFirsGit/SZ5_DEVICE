#pragma once
#include "OperationDlg.h"
#include "StationSetSvc.h"

/**
@brief      ��վ��Ϣ���뻭��
*/
class CStationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CStationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStationSetMaintainDlg(CService* pService);                             // ��׼���캯��
	~CStationSetMaintainDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };                               // �Ի�������

	void InitlabelStationInfo();											// ��ʼ��BASE����LABEL���������
	void SetFromReadIniText();												// ��ʾ��Maintenance.INI��ȡ�������ļ��ڶ�Ӧ����
	void ShowCodeContent();													// ������Ϣ����ʾ��Ӧ�����õ�����
protected:
	virtual void UpdateUI();												// ������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// ���漤��
	LABEL_GROUP m_lableGroup[15];											// ������ʾ�ĳ�վ��Ϣ
};
