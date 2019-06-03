#pragma once
#include "TVMForeService.h"
#include "TimeSetBaseOfficeDlg.h"
/**
@brief    时间设定
*/
class CTimeSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTimeSetSvc();
	~CTimeSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_TIMESET = 1,                       
	} DIALOG_GROUP;

	typedef struct _tagModel
	{
		int MenuNumber;			// 菜单号
		BOOL MenuChange;		// 菜单变化标志
		BOOL TimeChange;		// 代码变化标志
		BOOL TimeLegal;			// 输入内容是否非法标志
		int TimeNumber;			// 代码号
		CString setYear;
		CString setMonth;
		CString setDay;
		CString setHour;
		CString setMinute;
		CString setSecond;
		_tagModel()
		{
			MenuNumber = 0;
			MenuChange = FALSE;
			TimeChange = FALSE;
			TimeLegal = TRUE;
			setYear = _T("");
			setMonth = _T("");
			setDay = _T("");
			setHour = _T("");
			setMinute = _T("");
			setSecond = _T("");
		}

	}Model;

	Model& GetDataModel();
	void CheckMenuNumber();														// 菜单号是否非法判断 
	void DoCheckInput();														// 对输入数据是否合理做出相应项的实际判断
	void CheckInputSave();														// 时间保存
	virtual void NotifyDataModelChanged();	
protected:
	void OnStart();																// Service启动后处理	
private:
	int EnterTime;																// keyboard/Enter键按下次数
	SYSTEMTIME dSystemTime;
	CString TimeDate;															// 向导栏获取输入的时间
	int ErrorFlag;																// 时间是否非法的标志
	Model m_DataModel;
	
	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Clear] 键按下消息（删除）
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
};