#pragma once
#include "TVMForeService.h"

/**
@brief 硬币自检Service
*/

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")

class CCoinModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinModuleTestSvc();
	virtual ~CCoinModuleTestSvc();

	typedef enum _dialog_group
	{
		DIALOG_GROUP_MENU = 1,    //菜单界面
		DIALOG_GROUP_OPERATION,  //操作界面
	}DIALOG_GROUP;

	//菜单编号
	enum MenuNum
	{
		MenuNum1 = 1,
		MenuNum2,
		MenuNum3,
		MenuNum4,
		MenuNum5,
		MenuNum6,
		MenuNum7,
		MenuNum8,
		MenuNum9,
		MenuNum10,
		MenuNum11,
		MenuNum12,
		MenuNum13,
		MenuNum14,

		MenuNum21 = 21,
		MenuNum22,
		MenuNum31 = 31,
		MenuNum32,
		MenuNum41 = 41,
		MenuNum42,
	};
	//创建数据模型
	enum OPERATION_TYPE
	{
		OPERATION_TYPE_RETURN_COIN = 0 ,  //退币操作
		OPERATION_TYPE_CLEAR_COIN,        //清币操作
		//OPERATION_TYPE_SAFELOCK_CTRL,     //安全扣
		//OPERATION_TYPE_READ_ID,           //读RFID
		//OPERATION_TYPE_WRITE_ID,          //写RFID
		//OPERATION_TYPE_ADDING_COIN,       //补币操作
		OPERATION_TYPE_UNKNOW,            //未知
	};

	//检测结果-菜单信息反馈
	typedef struct _tag_menu_info
	{
		CString strMenuStatus;   //成功、失败、完成等
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	//倒出硬币菜单选项情况
	typedef struct _tag_return_coin_menu_info
	{
		//bool bMenu_A1;   //Coin Cycle Change Box
		//bool bMenu_A2;   //Coin Collection Box1
		//bool bMenu_A3;   //Passenger
		//bool bMenu_A4;   //Coin Collection Box2

		//bool bMenu_B1;   //To Passenger
		//bool bMenu_B2;   //To Coin Collection Box2
		CString str_cancel;
		CString str_accept;
		_tag_return_coin_menu_info()
		{
			str_cancel = UNKNOW;
			str_accept = UNKNOW;
			/*bMenu_A1 = false;
			bMenu_A2 = false;
			bMenu_A3 = false;
			bMenu_A4 = false;

			bMenu_B1 = false;
			bMenu_B2 = false;*/
		}

	}RETURN_COIN_MENU_INFO;

	//清出硬币测试
	typedef struct _tag_clear_coin_menu_info
	{
		WORD iClsMenu_A1;
		WORD iClsMenu_A2;
		bool bClsMenu_A1;
		bool bClsMenu_A2;

		WORD iClsMenu_B1;
		WORD iClsMenu_B2;
		bool bClsMenu_B1;
		bool bClsMenu_B2;

		bool bClsMenu_A1_All ;
		bool bClsMenu_A2_All ;
		bool bClsMenu_B1_All ;
		bool bClsMenu_B2_All ;
		_tag_clear_coin_menu_info()
		{
			iClsMenu_A1 = 0;
			iClsMenu_A2 = 0;
			iClsMenu_B1 = 0;
			iClsMenu_B2 = 0;
			bClsMenu_A1 = false;
			bClsMenu_A2 = false;
			bClsMenu_B1 = false;
			bClsMenu_B2 = false;
			bClsMenu_A1_All = false;
			bClsMenu_A2_All = false;
			bClsMenu_B1_All = false;
			bClsMenu_B2_All = false;
		}
	}CLEAR_COIN_MENU_INFO;
	enum CurInputOP
	{
		NULL_OP = 0,
		MenuA1,
		MenuA2,
		MenuB1,
		MenuB2,
	};


	//加币安全扣菜单选择状态
	typedef struct _tag_safelock_ctrl_menu_info
	{
		bool bSfMenu_A1;
		bool bSfMenu_A2;

		bool bSfMenu_B1;
		bool bSfMenu_B2;
		_tag_safelock_ctrl_menu_info()
		{
			bSfMenu_A1 = false;
			bSfMenu_A2 = false;

			bSfMenu_B1 = false;
			bSfMenu_B2 = false;
		}
	}SAFELOCK_CTRL_MENU_INFO;

	//读写钱箱RFID数据
	typedef struct _tag_read_write_coin_box_id
	{
		CString strCAddingBoxA;     //加币箱1
		CString strCAddingBoxB;     //加币箱2
		CString strCCollectionBoxA; //回收箱1
		CString strCCollectionBoxB; //回收箱2

		_tag_read_write_coin_box_id()
		{
			strCAddingBoxA = UNKNOW;
			strCAddingBoxB = UNKNOW;
			strCCollectionBoxA = UNKNOW;
			strCCollectionBoxB = UNKNOW;
		}
	}READ_WRITE_COIN_BOX_ID;

	typedef struct _tag_data_modol
	{
		OPERATION_TYPE operationType;
		MENU_INFO menuInfo[14];
		RETURN_COIN_MENU_INFO returnCoinMenuInfo;
		CLEAR_COIN_MENU_INFO clearCoinMenuInfo;
		SAFELOCK_CTRL_MENU_INFO safeLockCtrlMenuInfo;
		READ_WRITE_COIN_BOX_ID rwCoinBoxId;

		_tag_data_modol()
		{
			// 初始化清空数据
			operationType = OPERATION_TYPE_UNKNOW;
			/*operationType = OPERATION_TYPE();
			returnCoinMenuInfo = RETURN_COIN_MENU_INFO();
			clearCoinMenuInfo = CLEAR_COIN_MENU_INFO();
			rwCoinBoxId = READ_WRITE_COIN_BOX_ID();
			safeLockCtrlMenuInfo = SAFELOCK_CTRL_MENU_INFO();
			for(int i = 0; i<14; i++){
			menuInfo[i] = MENU_INFO();
			}*/
		}

	}Modol;

	Modol& GetDataModol();            //返回service的数据模型
	BYTE GetHopperType(int hopetype); // 获取硬币箱硬币类型 
protected:
	//void ReflashUI();                 //刷新界面
	void NotifyDataModelChanged();    //数据改变后，通知窗体更新数据
	void DoMenuOperation();           //主菜单操作业务
	void DoOperation();               //操作子业务

	bool ResetTest();                 //重置测试
	bool OpenCoinShutterTest();       //打开硬币口测试
	bool CloseCoinShutterTest();      //关闭硬币口测试
	bool StartCoinRecvTest();         //开始接收硬币测试
	bool StopCoinRecvTest();          //停止接收硬币测试
	bool AcceptCoinTest();            //CH收纳硬币
	bool ReleaseCoinTest();           //CH硬币找零
	void ReadAmountTest();            //读金额
	void ReturnCoinTest();            //退币测试
	void ClearCoinTest();             //清币测试
	void CoinAddingTest();            //补币测试
	
	//郑州项目这些无底层接口
	//bool StopCoinAddingTest();      //结束加币测试
	//void ContorlSafeLockTest();       //安全扣控制测试
	//void ReadCoinBoxIdTest();         //读RFID测试
	//void WriteCoinBoxIdTest();        //写RFID测试
	//void ReadVersionTest();           //读版本测试

	void ClearModolData();              //返回菜单界面时清除Modol数据
private:
	bool bIsCoinShutterOpen;            //硬币口是否打开
	bool bIsCoinRecvStart;              //是否开始接收硬币
	//bool bIsCoinAddingStart;           
private:	
	Modol m_Modol;
	CurInputOP type_op;
	//消息声明
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //处理 [Enter] 键按下消息（执行）
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //处理 [F3/Ctrl]    键按下消息（后退）
	LRESULT OnKeyboardF2(WPARAM , LPARAM );                 //处理 [F2/Alt]    键按下消息（保存）
	void OnStart();											//业务开始时初始化数据
};
