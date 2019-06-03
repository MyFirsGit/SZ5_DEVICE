#pragma once
#include "TVMForeService.h"

#define _MAX_ROW  15        //定义最大行
#define _MAX_COL   4        //定义最大列

/**
@brief      异常解除业务类
*/
class CRemoveExceptionSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionSvc();
	~CRemoveExceptionSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_REMOVE_EXCEPTION = 1
	} DIALOG_GROUP;

	typedef struct _tag_data_modol
	{
		vector<CSysException> m_ListException;
		vector<CSysException>::size_type m_ExceptionNum;
		_tag_data_modol(){
			m_ListException.clear();
			m_ExceptionNum = 0;
		}
	}Modol;

	Modol& GetDataModol();            // 获取数据模型
	void NotifyDataModelChanged();    // 数据改变后，通知窗体更新数据
	void InitExceptionData();         // 初始化异常数据
private:
	Modol m_modol;

	//消息
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );			                //确认键
	LRESULT OnKeyboardF3(WPARAM , LPARAM );								//返回
	void OnStart();
};
