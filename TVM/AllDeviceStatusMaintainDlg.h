#pragma once
#include "OperationDlg.h"

#include "afxwin.h"
#include "AllDeviceStatusSvc.h"
/**
@brief 硬件自检所有设备状态
*/

class CAllDeviceStatusMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CAllDeviceStatusMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CAllDeviceStatusMaintainDlg(CService* pService);      // 构造函数
	~CAllDeviceStatusMaintainDlg();                       // 析构函数

	enum { IDD = IDD_ALL_DEVICE_STATUS };       // 对话框ID

	void ShowInitRessult(int index);            // 刷新初期化结果
	void InitTitle();
private:
	void OnDraw(CDC* pDC);   // 画面绘制函数
	CString GetResultText(CAllDeviceStatusSvc::INIT_RESULT result);   // 获取初期化结果文字
	COLORREF GetResultColor(CAllDeviceStatusSvc::INIT_RESULT result); // 获取初期化结果颜色
};