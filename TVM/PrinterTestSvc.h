#pragma once
#include "tvmforeservice.h"
#define  _MAX_ROW  15     //最大行数
#define  _MAX_COL   2     //最大列数

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")
/**
@brief    凭证打印机-硬件自检业务
*/
class CPrinterTestSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestSvc();
	~CPrinterTestSvc();

	//窗口
	typedef enum _dialog_group 
	{
		DIALOG_GROUP_PRINT   =  1,                
	} DIALOG_GROUP;

	//操作类型
	typedef enum _tag_operation_type
	{
		OPERATION_TYPE_RESET = 0,
		OPERATION_TYPE_TEST_PRINT,
		OPERATION_TYPE_UNKNOW,
	}OPERATION_TYPE;

	//菜单数据
	typedef struct _tag_menu_info 
	{
		CString strMenuStatus;
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	typedef struct _tag_data_modol
	{
		MENU_INFO menuInfo[_MAX_ROW];
		/*_tag_data_modol(){
		for(int i=0;i<_MAX_ROW;i++){
		menuInfo[i] = MENU_INFO();
		}
		}*/
	}Modol;

	Modol& GetDataModol();             //返回service的数据模型	
	void NotifyDataModelChanged();     //数据改变后，通知窗体更新数据

private:
	Modol m_Modol;                                //数据模型
	OPERATION_TYPE  m_OperationType;              //操作类型
	void            DoOperation();               //根据测试类型进行测试
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //处理 [Enter] 键按下消息（执行）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					//处理 [F3/Ctrl]	   键按下消息（返回）
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
	void OnStart();
};