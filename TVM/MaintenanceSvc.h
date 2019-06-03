#pragma once
#include "TVMForeService.h"

/**
@brief 维护业务Service
*/
class CMaintenanceSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CMaintenanceSvc();					// 构造函数
	virtual ~CMaintenanceSvc();			// 析构函数

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1
	} DIALOG_GROUP;

	// SVC数据模型
	typedef struct _tagModel
	{
		vector<Menu_Item*> firstlist;
		Menu_Item* secondlist;
		_tagModel()
		{
			secondlist = NULL;
			firstlist.clear();
		}
	}Model;

	Model& GetDataModel();

	void InitData();
	void ExecuteService();
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/ );				// 处理 [Esc] 键按下消息
	void SetReturn(){m_isReturn = true;};
private:
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	void OnStart();
	Model m_DataModel;
	bool m_isReturn;
};