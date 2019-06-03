#pragma once
#include "OperationDlg.h"

/**
@brief      通信参数输设置画面
*/
class CCommunicationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCommunicationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationSetMaintainDlg(CService* pService);
	~CCommunicationSetMaintainDlg();

	enum { IDD = IDD_20114_COMMUNICATION_SET_DLG };

private:
	typedef enum _communicationinfo_edit_group 
	{
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1  =  0,
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2 = 1,
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3 = 2,
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4 = 3,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1 = 4,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2 = 5,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3 = 6,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4 = 7,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART1 = 8,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART2 = 9,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART3 = 10,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART4 = 11,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1 = 12,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2 = 13,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3 = 14,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4 = 15,
		COMMUNICATION_EDIT_GROUP_CPSPort = 16,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1 = 17,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2 = 18,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3 = 19,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4 = 20,
		COMMUNICATION_EDIT_GROUP_NTPPort = 21,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1 = 22,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2 = 23,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3 = 24,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4 = 25,
		COMMUNICATION_EDIT_GROUP_FTPPort = 26,
	} COMMUNICATION_EDIT_GROUP;

	LABEL_GROUP m_labelCommunicationinfo[LABEL_GROUP_EDIT_ROWS_9];					// 画面显示的通信参数信息

	void InitlabelCommunicationInfo();												// 初始化BASE区域LABEL里面的数据
	void InitEditText();															// 初始化BASE区域EDIT框里面的数据
	void SetCommunicateParameters();												// 把EDIT框里面数据的保存到SVC里面
	bool CheckEditText();															// 检查EDIT框里的端口数据是否合法
	void HideGUIModule(ULONG ulCompomentFlag);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);							// 画面创建函数							
	afx_msg LRESULT OnOK(WPARAM,LPARAM);											// 处理确认按钮消息
	afx_msg LRESULT OnReset(WPARAM,LPARAM);											// 处理重置按钮消息
};
