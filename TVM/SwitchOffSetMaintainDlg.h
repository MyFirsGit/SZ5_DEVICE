#pragma once
#include "OperationDlg.h"

/**
@brief ��������
*/
class CSwitchOffSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CSwitchOffSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSwitchOffSetMaintainDlg(CService* pService);
	~CSwitchOffSetMaintainDlg();
	
	enum{IDD = IDD_20112_STATION_SET_DLG};

	typedef enum _device_type
	{
		TVM_TYPE = DEVICE_TYPE_TVM,
		AVM_TYPE = 3
	}device_type;

	void InitlableSwitchOffInfo();											// ��ʼ��BASE����LABEL���������
	void SetFromReadIniText();												// ��ʾ��TVM.INI��ȡ�������ļ��ڶ�Ӧ����
	void ShowCodeContent();													// ������Ϣ����ʾ��Ӧ�����õ�����
protected:
	virtual void UpdateUI();												// ������ʾ��ɻص�����
private:
	int DeviceType;															// �豸����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// ���漤��
	LABEL_GROUP_THREE m_lableGroup[15];										// ������ʾ�����á�ͣ����Ϣ
};
