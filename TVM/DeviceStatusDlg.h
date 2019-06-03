#pragma once
#include "OperationDlg.h"
#include "DeviceStatusSvc.h"

/**
@brief 基本画面
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

	void InitlableDeviceStatusInfo();							// 初始化BASE区域LABEL里面name1的数据
	void ShowCashBoxData();										// 初始化BASE区域LABEL里面name2、name3、name4、name5的数据
	void ShowEmptyFullData();									// 初始化BASE区域LABEL里面name2、name3、name4、name5的将空、将满、已满数据
	void ShowStatus();											// 初始化BASE区域LABEL里面Status
	void SetColor(int row,CString status);						// 设置状态显示的颜色
	//void InitlableDeviceStatusInfoforAVM();						// 初始化BASE区域LABEL里面的数据forAVM
protected:
	virtual void UpdateUI();									// 更新显示完成回调函数
private:
	int DeviceType;												// 设备类型
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_FIVE m_lableGroup[LABLE_GROUP_MAX_SIZE];		// 画面显示设备状态信息
};