#pragma once
#include "OperationDlg.h"

/**
@brief 基本画面
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

	void InitlableSwitchOffInfo();											// 初始化BASE区域LABEL里面的数据
	void SetFromReadIniText();												// 显示从TVM.INI读取的配置文件在对应项上
	void ShowCodeContent();													// 基本信息区显示对应项设置的数据
protected:
	virtual void UpdateUI();												// 更新显示完成回调函数
private:
	int DeviceType;															// 设备类型
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// 画面激活
	LABEL_GROUP_THREE m_lableGroup[15];										// 画面显示的启用、停用信息
};
