#pragma once
#include "tvmforeservice.h"

#define  _MAX_ROW  15     //最大行数
#define  _MAX_COL   2     //最大列数

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")
/**
@brief      IO测试service
*/

class CIOModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CIOModuleTestSvc();
	~CIOModuleTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_IO_TEST = 1,  
		DIALOG_GROUP_IO_STATUS,
	} DIALOG_GROUP;

	//操作类型
	typedef enum _tag_operation_type
	{
		//OPERATION_TYPE_STATUS = 0,          //IO状态
		//OPERATION_TYPE_OPEN_ALARM,			//打开蜂鸣报警
		//OPERATION_TYPE_BACKLIGHT,			//打开维护面板背光
		//OPERATION_TYPE_CARD_LOCKER,			//储值卡锁卡
		//OPERATION_TYPE_CARD_SENDER,			//储值卡弹卡
		//OPERATION_TYPE_COIN_SHUTTER,		//投币口电磁铁
		//OPERATION_TYPE_PAS_SENSER,			//乘客传感器电源
		//OPERATION_TYPE_CHANGE_INDICATOR,    //找零指示灯
		//OPERATION_TYPE_CHANGE_LIGHT,		//找零照明灯
		//OPERATION_TYPE_OPEN_ALL,			//打开所有IO控制
		//OPERATION_TYPE_CLOSE_ALL,			//关闭所有IO控制
		//OPERATION_TYPE_UNKNOW,


		OPERATION_TYPE_STATUS = 0,					// IO状态
		//OPERATION_TYPE_CHANGE_INDICATOR,			// 取票找零指示灯
		//OPERATION_TYPE_CHANGE_LIGHT,				// 取票找零照明灯
		//OPERATION_TYPE_CHARGE_INDICATOR,			// 充值口指示灯
		//OPERATION_TYPE_ALARM,						// 蜂鸣器
		//OPERATION_TYPE_CH_INDICATOR,				// 硬币投币口指示灯
		//OPERATION_TYPE_RECEIPT_INDICATOR,			// 凭条口指示灯
		//OPERATION_TYPE_BAFFLE,						// 硬币电磁铁开（允许投币）
		//OPERATION_TYPE_CARDFFLE,					// 储值卡电磁铁开（允许插卡）

		IO_OUTPUT_PORT_NO_CH_INDICATOR										= 0x31,	  // 投币口指示灯
		IO_OUTPUT_PORT_NO_RETURN_CUP_INDICATOR								= 0x32,   // 出票口指示灯
		IO_OUTPUT_PORT_NO_RETURN_CUP_LIGHT									= 0x33,   // 出票口照明灯
		IO_OUTPUT_PORT_NO_CARDFFLE_INDICATOR								= 0x35,	  // 储值卡指示灯
		IO_OUTPUT_PORT_NO_BAFFLE_B											= 0x39,	  // B电磁铁
		IO_OUTPUT_PORT_NO_OPERATION_LIGHT									= 0x41,	  // 操作照明灯
		IO_OUTPUT_PORT_NO_BAFFLE_T											= 0x42,	  // T电磁铁
		IO_OUTPUT_PORT_NO_ALARM												= 0x43,   // 报警器
		IO_OUTPUT_PORT_NO_HELP_BTN_LIGHT									= 0x00,	  // 招援按钮灯(停用)


		OPERATION_TYPE_OPEN_ALL,			//打开所有IO控制
		OPERATION_TYPE_CLOSE_ALL,			//关闭所有IO控制
		OPERATION_TYPE_UNKNOW,
	}OPERATION_TYPE;

	//检测结果
	typedef struct _tag_status_data
	{
		bool bIsThisActive;
		_tag_status_data()
		{
			bIsThisActive = false;
		}
	}STATUS_DATA_INFO;
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
		STATUS_DATA_INFO IO_Status[13];
		/*_tag_data_modol(){
		for(int i = 0;i<_MAX_ROW;i++){
		menuInfo[i] = MENU_INFO();
		IO_Status[i<8 ? i : 0] = STATUS_DATA_INFO();
		}
		}	*/

	}Modol;

	Modol& GetDataModol();            //返回service的数据模型	
	void NotifyDataModelChanged();    //数据改变后，通知窗体更新数据
	volatile bool   m_bIsStopTesting; //停止检测
	
private:
	int DeviceType;													   //设备类型
	Modol           m_Modol;                                           //数据模型
	OPERATION_TYPE  m_OperationType;                                   //操作类型
	void            DoOperation();                                     //根据测试类型进行测试
	static DWORD WINAPI    BeginTestingIOStatus(LPVOID lpParameter);   //创建IO状态监测线程  
	HANDLE          m_hHandle;
	DWORD           dwThreadID;
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //处理 [Enter] 键按下消息（执行）
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Esc] 键按下消息（主菜单）
	//LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );     //处理 [0~9]   键按下消息（数字）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //处理 [F3/Ctrl]    键按下消息（后退）
	void OnStart();
};
