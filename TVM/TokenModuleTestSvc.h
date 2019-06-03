#pragma once
#include "TVMForeService.h"

/**
@brief 硬币自检Service
*/

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")

class CTokenModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTokenModuleTestSvc();
	virtual ~CTokenModuleTestSvc();

	typedef enum _dialog_group
	{
		DIALOG_GROUP_MENU = 1,    //菜单界面
		DIALOG_GROUP_OPERATION,  //操作界面
	}DIALOG_GROUP;

	enum _menu_num
	{
		Menu_Code_1  = 1,

		//Menu_Code_7  =7,
		//Menu_Code_8,
		//Menu_Code_9,

		Menu_Code_21 = 21,
		Menu_Code_22,

		Menu_Code_3 = 3,

		Menu_Code_4 = 4,
		//Menu_Code_41 = 41,
		//Menu_Code_42,

		Menu_Code_51 = 51,
		Menu_Code_52,

		//Menu_Code_61 = 61,
		//Menu_Code_62,

	};
	//创建数据模型
	enum OPERATION_TYPE
	{
		OPERATION_TYPE_READ_ID = 0,       //读RFID
		OPERATION_TYPE_WRITE_ID,          //写RFID
		OPERATION_TYPE_UNKNOW,            //未知操作
	};
	
	//检测结果
	typedef struct _tag_menu_info
	{
		CString strMenuStatus;   //成功、失败、完成等
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	//RFID信息
	typedef struct _tag_box_id
	{
		CString strRFIDBox1;
		CString strRFIDBox2;
		_tag_box_id()
		{
			strRFIDBox1 = UNKNOW;
			strRFIDBox2 = UNKNOW;
		}

	}TOKEN_BOX_ID;

	//Token数据模型
	typedef struct _tag_data_modol
	{
		MENU_INFO       MenuInfo[9];
		TOKEN_BOX_ID    box_id;
		OPERATION_TYPE  operationType;

		_tag_data_modol()
		{
			operationType = OPERATION_TYPE_UNKNOW; 
			box_id = TOKEN_BOX_ID();
			for(int i=0;i<9;i++){
				MenuInfo[i] = MENU_INFO();
			}
		}
	}Modol;

	Modol& GetDataModol();            //返回service的数据模型
	void NotifyDataModelChanged();    //数据改变后，通知窗体更新数据

protected:
	
	//操作选项
	void DoMenuOperation();             //主菜单操作业务
	void DoOperation();                 //操作子业务
	//操作
	bool ResetTest();                        //重置测试	
	bool CatchTokenTest(bool bIsLeft);       //抓取Token测试
	bool PutoutTokenTest();					 //放出Token测试
	bool RecycleTokenBoxTest(bool bIsLeft);  //回收Token测试
	bool ClearTokenBoxTest(bool bIsLeft);    //清空Token测试
	bool DetachTokenBoxTest(bool bIsLeft);   //卸载Token箱测试
	void ReadTokenBoxIdTest();               //读RFID测试
	void WriteTokenBoxIdTest();              //写RFID测试
	void ReadVersionTest();                  //读版本测试
	
	void ClearModolData();                  //返回菜单界面时清除Modol数据
private:	
	Modol m_Modol;
	bool m_bIsCatchLeftToken;          //是否抓起左边的Token
	bool m_bIsCatchRightToken;         //是否抓起右边的Token
	//消息声明
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //处理 [Enter] 键按下消息（执行）
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //处理 [F3]    键按下消息（后退）
	LRESULT OnKeyboardF2(WPARAM , LPARAM );                 //处理 [F2]    键按下消息（保存）
	void OnStart();
};
