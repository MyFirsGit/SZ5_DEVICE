#pragma once
#include "OperationDlg.h"

/**
@brief      车站信息输入画面
*/
class CStationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CStationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStationSetMaintainDlg(CService* pService);                             // 标准构造函数
	~CStationSetMaintainDlg();
	enum { IDD = IDD_20112_STATION_SET_DLG };                               // 对话框数据

private:
	typedef enum _stationinfo_edit_group {                                  // 车站代码EDIT对应的EDITGROUP
		STATIONINFO_EDIT_GROUP_LINECODE = 0,
		STATIONINFO_EDIT_GROUP_STATIONCODE  =  1,
		STATIONINFO_EDIT_GROUP_MACHINECODE  =  2,
		STATIONINFO_EDIT_GROUP_GROUPNUMBER  =  3,
		STATIONINFO_EDIT_GROUP_EQUNOINGROUP =  4
	} STATIONINFO_EDIT_GROUP;

	LABEL_GROUP m_labelStationinfo[LABEL_GROUP_EDIT_ROWS_5];                // 画面显示的车站信息

	void InitlabelStationInfo();											// 初始化BASE区域LABEL里面的数据
	void InitEditText();													// 初始化BASE区域EDIT框里面的数据
	void SetStationInfo();													// 设置车站信息
	int CheckInputText();													// 检查EDIT框里的数据是否合法
	void HideGUIModule(ULONG ulCompomentFlag);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// 处理确认按钮消息
	afx_msg LRESULT OnReset(WPARAM,LPARAM);									// 处理重置按钮消息
};
