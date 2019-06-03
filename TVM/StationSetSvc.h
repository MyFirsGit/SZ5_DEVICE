#pragma once
#include "TVMForeService.h"
#include "StationSetMaintainDlg.h"
#define MAX_SMMENU_NUM 5		// 车站设置界面菜单的最大编号
#define MAX_LineCode_NUM 255	// 线路代码的最大编号

/**
@brief    保守业务 - 车站设置业务
*/
class CStationSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CStationSetSvc();
	~CStationSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_STATION_SET	= 1,                       
	} DIALOG_GROUP;

	// SVC数据模型
	typedef struct _StationModel
	{
		int MenuNumber;			// 菜单号
		BOOL MenuChange;		// 菜单变化标志(菜单号由1变到2，这种变化置MenuChange为TRUE)
		BOOL CodeChange;		// 代码变化标志(线路代码的发生了改变，置CodeChange为TRUE)
		BOOL CodeLegal;			// 输入内容是否非法标志
		CString sLineCode;
		CString sStationCode;
		CString sMachineCode;
		CString sGroupNumber;
		CString sEquNumberInGroup;
		_StationModel(){
			MenuNumber = 0;
			MenuChange = FALSE;
			CodeChange = FALSE;
			CodeLegal = TRUE;
			sLineCode = _T("");
			sStationCode = _T("");
			sMachineCode = _T("");
			sGroupNumber = _T("");
			sEquNumberInGroup = _T("");
		}

	}Model;

	typedef struct _StationWrite
	{
		int LineIndex;
		int StationIndex;
		int MachineIndex;
		int GroupIndex;
		int EquNumberIndex;
		_StationWrite(){
			LineIndex = 0;
			StationIndex = 0;
			MachineIndex = 0;
			GroupIndex = 0;
			EquNumberIndex = 0;
		}
	}NumberIndex;

	Model& GetDataModel();
	void CheckMenuNumber();														// 菜单号是否非法判断
	void DoCheckInput();														// 对输入数据是否合理做出相应项的实际判断
	void ReadIniText();															// 读取Maintenance.INI配置文件
	void InputGuideMsg();														// 对GuideMsg分别赋值

	virtual void NotifyDataModelChanged();	
protected:
	void OnStart();																// Service启动后处理
private:
	int EnterTime;																// keyboard/Enter键按下次数
	CString GuideMsg;															// 向导栏提示
	Model m_DataModel;
	NumberIndex n_NubIndex;

	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Clear] 键按下消息（删除）
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
};



//====================================================================================================================
/* 编写思路：从选择菜单到进行相应设置，需要Enter按键按下两次，在Enter按键得消息响应函数里记录Enter按键按下次数，即
   CStationSetSvc成员变量int EnterTime的值。在第一次按下Enter按键里处理输入菜单编号是否合理，在第二次按下Enter按键
   里处理输入合理菜单编号所代表的菜单项的设定值是否合理。															*/
//====================================================================================================================