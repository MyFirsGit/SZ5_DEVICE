#pragma once
#include "TVMForeService.h"
#pragma warning(disable : 4482)

/**
@brief 纸币测试Service
*/
#define  _MAX_ROW 15                 //最大行数
#define  _MAX_COL  4                 //最大列数

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")

class CBanknoteAcceptModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteAcceptModuleTestSvc();
	virtual ~CBanknoteAcceptModuleTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_BANKNOTE   =  1,                                // 纸币测试画面组
	} DIALOG_GROUP;

	//Operation type
	typedef enum _tag_operation_type
	{
		OPERATION_TYPE_RESET = 0,									// 重置
		OPERATION_TYPE_START_IDENTIFY,								// 开始接收纸币
		OPERATION_TYPE_STOP_IDENTIFY,								// 停止接收纸币		
		OPERATION_TYPE_RETURN_CASH,									// 退钞										
		OPERATION_TYPE_ACCEPT,										// 压钞
		OPERATION_TYPE_CHANGE,										// 找零纸币
		OPERATION_TYPE_UNKNOW,
		OPERATION_TYPE_F3,
		OPERATION_TYPE_ESC
	}OPERATION_TYPE;

	//检测结果-菜单信息反馈
	typedef struct _tag_menu_info
	{
		CString strMenuStatus;     //成功、失败、完成等
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	typedef struct _tag_data_modol
	{
		MENU_INFO menuInfo[5];
		BankNoteAndCoinType_t bh_face;
		_tag_data_modol(){
			bh_face = VALUE_UNKNOWN;
			/*for(int i = 0; i<5; i++){
			menuInfo[i] = MENU_INFO();
			}*/
		}
	}Modol;

	Modol& GetDataModol();            //返回service的数据模型
	void NotifyDataModelChanged();    //数据改变后，通知窗体更新数据
	void OnBanknoteInserted(UINT,CBRHOpenAndValidate*);			// 纸币识别金额
private:
	Modol m_Modol;
	OPERATION_TYPE  m_OperationType;                                   //操作类型
	void            DoOperation();                                     //根据测试类型进行测试
	bool			m_bIsBanknotesInserted;							   //判断纸币是否插入
	BOOL			m_bIsReceiving;									   //判断是否正在接收
	BRH_STATUS       bhRspType; //纸币识别

	//消息声明
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              // 处理 [Enter] 键按下消息（执行）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					// 处理 [F3/Ctrl]    键按下消息
	LRESULT OnKeyboardEsc(WPARAM , LPARAM );				// 处理 [ESC]   键按下消息
	void OnStart();
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
};
