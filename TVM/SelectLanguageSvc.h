#pragma once
#include "TVMForeService.h"

/**
@brief 维护业务Service
*/
class CSelectLanguageSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSelectLanguageSvc();					// 构造函数
	virtual ~CSelectLanguageSvc();			// 析构函数

	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1
	} DIALOG_GROUP;

	// SVC数据模型
	typedef struct _tagModel
	{
		_tagModel()
		{
		}
	}Model;

	Model& GetDataModel();
	void NotifyDataModelChanged();


private:
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息

	Model m_DataModel;
};