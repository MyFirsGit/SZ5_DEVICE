#pragma once
#include "tvmforeservice.h"
class CVersionInfoSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CVersionInfoSvc(void);
	~CVersionInfoSvc(void);
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAINPUT_SELECT = 1,           
	} DIALOG_GROUP;

	//数据模型
	typedef struct _tagParameterInputStruct
	{
		CString	name;					//参数名称
		DWORD localCurrentVersion;		//本地当前版本
		DWORD localFutureVersion;		//本地当前将来版
		DWORD importCurrentVersion;		//导入的当前版本
		DWORD importFutureVersion;		//导入的将来版本
		CString importCurrentTo;		//导入当前版到的位置
		CString importCurrentFrom;		//从哪里导入当前版
		CString importFutureTo;			//导入将来版到的位置
		CString importFutureFrom;		//从哪里导入将来版
		_tagParameterInputStruct()
		{
			name = _T("");
			localCurrentVersion = -1;
			localFutureVersion = -1;
			importCurrentVersion = -1;
			importFutureVersion = -1;
			importCurrentTo =  _T("");
			importCurrentFrom =  _T("");
			importFutureTo = _T("");
			importFutureFrom = _T("");
		}
	} ParameterInputStruct;

	map<WORD,ParameterInputStruct>& GetDataModel();     // 获取数据模型
protected:
	void OnStart();
	bool ImportParameter();								// 导入参数
	bool AuthUSB();										// 验证USB
	void NotifyDataModelChanged();						// 更新界面显示
private:	
	map<WORD,ParameterInputStruct> m_DataModel;
	void InitDataModel();
	void GetImportInfo();
	//LRESULT OnKeyboardEnter(WPARAM , LPARAM );	// 键盘处理函数[确认]
	LRESULT OnKeyboardF3(WPARAM , LPARAM );     // 键盘处理函数[F3/Ctrl]
};