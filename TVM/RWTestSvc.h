#pragma once
#include "TVMForeService.h"
#include "TVMRWHelper.h"
/**
@brief    读写器测试
1.充值        
2.smart-card  
3.Token       
*/

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")

class CRWTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRWTestSvc();
	~CRWTestSvc();

	typedef enum _dialog_group
	{
		DIALOG_GROUP_MAIN = 1,
		DIALOG_GROUP_BASE,
		DIALOG_GROUP_OPERATION,
	} DIALOG_GROUP;

	//Main menu
	typedef enum _tag_Main_Menu
	{
		RECHARGE_RW = 0,
		CARD_RW=1,
		//TOKEN_RW,
		UNKNOW_RW,
	}MAIN_MENU;
	//操作类型
	enum OperationType
	{
		Operation_Reset,
		Operation_ReadCard,
		Operetion_Parameter_Ver,
		Operation_RW_Info,
		Operetion_Unknow,
	};
	//参数版本数据区
	typedef struct _tag_paramter_info
	{
		CString strID;
		CString strName;
		CString strVersion;
		_tag_paramter_info()
		{
			strID      = UNKNOW;
			strName    = UNKNOW;
			strVersion = UNKNOW;
		}
	}Paramter_Info;

	//RW信息数据
	typedef struct _tag_RW_Info
	{
		CString strRWName;
		CString strRWValue; //因为显示都需要转换为CString，所以这里统一定义为CString
		_tag_RW_Info()
		{
			strRWName  = _T("R/W Device ID:");
			strRWValue = UNKNOW;
		}

	}RW_Info;

	//基本界面
	typedef struct _tag_BaseMenu_Info
	{
		int nMenuNum;
		CString strMenuStatus;
		_tag_BaseMenu_Info()
		{
			nMenuNum      = 1;
			strMenuStatus = UNKNOW;
		}

	}BaseMenu_Info;

	typedef struct _tag_Menu_Info
	{
		OperationType operation_type;          //当前操作类型
		MAIN_MENU     rwType;                  //进入的主菜单类型（充值，票卡）
		BaseMenu_Info baseMenuInfo[4];
		Paramter_Info paramterInfo[22];
		RW_Info       rwInfo[6];
		int timesstatus;							// 翻页中的当前页数

		_tag_Menu_Info()
		{
			timesstatus = 1;
			operation_type = Operetion_Unknow;
			rwType         = CARD_RW;
			/*for(int i=0;i<10;i++){
			baseMenuInfo[i<4 ? i:0] = BaseMenu_Info();
			paramterInfo[i<10?i:0] = Paramter_Info();
			rwInfo[i<4 ? i:0]		= RW_Info();
			}*/
		}
	}Modol;

	Modol& GetDataModol();//传递数据模型

	bool RWResetTest();                         //RW复位测试
	bool RWReadCardTest();                      //RW读卡测试
	void RWParameterTest();                     //RW读Parameter测试
	void RWInfoTest();                          //RWInfo测试

	void ReFlashDlg();
	void ResetModolData();                      //重置modol的数据

private:
	Modol m_Modol;
	int DeviceType;
	void DoMainMenu();    //主菜单处理
	void DoBaseMenu();    //子菜单处理
	//消息声明
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );						  //处理 [Enter] 键按下消息（执行）
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );							  //处理 [F3/Ctrl]    键按下消息（后退）
	LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F5] 键按下消息（上一页）
	LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);			// 处理 [F6] 键按下消息（下一页）

	//void OnReadCard(UINT,CRWReadCard*);                             //读卡处理回调函数
	void OnStart();
	bool m_bIsSupperUser;
};
