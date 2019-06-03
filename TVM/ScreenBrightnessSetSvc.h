#pragma once
#include "TVMForeService.h"
#include "ScreenBrightnessSetMaintainDlg.h"
/**
@brief    时间设定
*/
class CScreenBrightnessSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CScreenBrightnessSetSvc();
	~CScreenBrightnessSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_SCREENBRIGHTNESS_SET = 1,                       
	} DIALOG_GROUP;

	typedef struct _tagModel
	{
		int BrightnessNumber;		// 亮度
		_tagModel()
		{
			BrightnessNumber = 5;
		}

	}Model;

	Model& GetDataModel();
	void CheckMenuNumber();														// 菜单号是否非法判断 
	void DoCheckInput();														// 对输入数据是否合理做出相应项的实际判断
	void CheckInputSave();														// 时间保存
	virtual void NotifyDataModelChanged();	
private:
	Model m_DataModel;

	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
	LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
};