#pragma once
#include "tvmforeservice.h"
class CBalanceSvc :
	public CTVMForeService
{
	 DECLARE_MESSAGE_MAP()
public:
	CBalanceSvc(void);
	~CBalanceSvc(void);

	typedef enum _dialog_group
	{
		DIALOG_GROUP_BASE,
	} DIALOG_GROUP;

	void OnStart();
	void NotifyDataModelChanged();
	typedef enum _current
	{
		PROCESSING_DATA,	// 业结数据处理中
		PRINT_RECEPTION,	// 业结单据打印中
		SHUTTING_DOWN		// 关机处理中
	}current_state;

	typedef struct data_model
	{
		current_state	state;
	}DATA_MODEL;

	DATA_MODEL GetDataModel();
private:
	DATA_MODEL model;
	void ShutDown();
	virtual LRESULT OnKeyboardF2(WPARAM , LPARAM);							 // 处理 [F2/Alt] 键按下消息（保存）
	virtual LRESULT OnKeyboardF3(WPARAM , LPARAM);							 // 处理 [F3/Ctrl] 键按下消息（返回）
};

