#pragma once
#include "BOMForeService.h"

/**
@brief    数据导入
*/
class CDataInputSvc: public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
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
			name = "";
			localCurrentVersion = -1;
			localFutureVersion = -1;
			importCurrentVersion = -1;
			importFutureVersion = -1;
			importCurrentTo = "";
			importCurrentFrom = "";
			importFutureTo = "";
			importFutureFrom = "";
		}
	} ParameterInputStruct;

	CDataInputSvc();
	~CDataInputSvc();
	bool AuthUSB();										// 验证USB
	map<WORD,ParameterInputStruct>& GetDataModel();
	bool ImportParameter();								// 导入参数

	int m_ErrorCode;

protected:
	void OnStart();

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAINPUT_SELECT = 1,           
	} DIALOG_GROUP;
	map<WORD,ParameterInputStruct> m_DataModel;
	void InitDataModel();
	void GetImportInfo();
};