#pragma once
#include "OperationDlg.h"
#include "DeviceStatusSvc.h"

/**
@brief ��������
*/
class CDeviceStatusDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CDeviceStatusDlg)
	DECLARE_MESSAGE_MAP()
public:
	CDeviceStatusDlg(CService* pService);
	~CDeviceStatusDlg();

	enum{IDD = IDD_20112_STATION_SET_DLG};

	enum{LABLE_GROUP_MAX_SIZE = 15};

	typedef enum _page
	{
		Main_Page = 1,
		Remain_page,
		Empty_Full_page
	}page;

	/*typedef enum _device_type
	{
	DEVICE_TYPE_TVM = 1,
	DEVICE_TYPE_AVM = 3
	}device_type;*/

	void InitlableDeviceStatusInfo();							// ��ʼ��BASE����LABEL����name1������
	void ShowCashBoxData();										// ��ʼ��BASE����LABEL����name2��name3��name4��name5������
	void ShowEmptyFullData();									// ��ʼ��BASE����LABEL����name2��name3��name4��name5�Ľ��ա���������������
	void ShowStatus();											// ��ʼ��BASE����LABEL����Status
	void SetColor(int row,CString status);						// ����״̬��ʾ����ɫ
	//void InitlableDeviceStatusInfoforAVM();						// ��ʼ��BASE����LABEL���������forAVM
protected:
	virtual void UpdateUI();									// ������ʾ��ɻص�����
private:
	int DeviceType;												// �豸����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_FIVE m_lableGroup[LABLE_GROUP_MAX_SIZE];		// ������ʾ�豸״̬��Ϣ
};