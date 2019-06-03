#pragma once
#include "tvmforeservice.h"

#define  _MAX_ROW  15     //最大行数
#define  _MAX_COL   5     //最大列数

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")
/**
@brief      Smart-Card测试service
*/

class CSmardCardTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CSmardCardTestSvc();
	~CSmardCardTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_TH_TEST = 1,
		DIALOG_GROUP_MOTOR_TEST,
		DIALOG_GROUP_READ_RFID_TEST,
		//DIALOG_GROUP_WRITE_RFID_TEST,
		DIALOG_GROUP_SENSOR_TEST,
	} DIALOG_GROUP;


	//菜单选项
	enum MENU_CODE
	{
		MENU_CODE_1 = 1,
		MENU_CODE_21 = 21,
		MENU_CODE_22,
		MENU_CODE_3 = 3,
		MENU_CODE_41 = 41,
		MENU_CODE_42,
		MENU_CODE_43,
		MENU_CODE_44,
		MENU_CODE_5=5,
		//MENU_CODE_6,
		MENU_CODE_61=61,
		MENU_CODE_62,
		MENU_CODE_63,
		MENU_CODE_64,
		MENU_CODE_7=7,
		MENU_CODE_8,
		MENU_CODE_9,
		MENU_CODE_10,
		MENU_CODE_11,
	};
	//操作类型
	enum OPERATION_TYPE
	{
		OPERATION_TYPE_RETSET = 0 ,			//重置
		OPERATION_TYPE_CATCH_CARD,			//抓取票卡  [1]A   [2]B
		OPERATION_TYPE_ISSUE_CARD,			//放出票卡
		OPERATION_TYPE_MOVE_BOX,			//升降票箱测试[1]A   [2]B
		OPERATION_TYPE_READ_RFID,			//读票箱RFID
		//OPERATION_TYPE_WRITE_RFID,			//写票箱RFID
		OPERATION_TYPE_ISSUE_TEST,			//发票测试命令[1]A   [2]B
		OPERATION_TYPE_MOVE_MOTOR_TEST,		//票箱电机测试[1]A   [2]B
		OPERATION_TYPE_MOTOR_TEST,			//多种电机测试    【跳转页面】
		OPERATION_TYPE_SENSOR_TEST,			//传感器测试
		OPERATION_TYPE_ELECTROMAGNET_TEST,	//电磁铁测试
		OPERATION_TYPE_CYCLE_TEMP_TICKS,	//缓冲区票卡回收
		OPERATION_TYPE_CYCLE_TEST,			//票卡回收测试
		OPERATION_TYPE_UNKNOW,				//未知
	};

	//菜单数据
	typedef struct _tag_menu_info 
	{
		CString strMenuStatus;
		CString motorStatus;
		CString read_rfidStatus;
		CString write_rfidStatus;
		CString writeID;
		CString writeCount;
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
			motorStatus = UNKNOW;
			read_rfidStatus = UNKNOW;
			write_rfidStatus = UNKNOW;
			writeID = UNKNOW;
			writeCount = UNKNOW;
		}
	}MENU_INFO;

	enum CurInputOP
	{
		NULL_OP = 0,
		MenuA,
		MenuB,
		MenuC,
		MenuAB,
		NumA,
		NumB,
		NumC,
	};

	typedef struct _tag_data_modol
	{
		//BYTE INFS6Covered;          // 1:6 号INFS 遮挡
		//BYTE INFS5Covered;          // 1:5 号INFS 遮挡
		//BYTE INFS4Covered;          // 1:4 号INFS 遮挡
		//BYTE INFS3Covered;          // 1:3 号INFS 遮挡
		//BYTE INFS2Covered;          // 1:2 号INFS 遮挡
		//BYTE INFS1Covered;          // 1:1 号INFS 遮挡
		TH_RSP_GET_DEV_STATUS cardSenserStatus;
		bool bCardSenserTestSuccess;
		MENU_INFO menuInfo[_MAX_ROW];

		CString strPreBoxID;
		CString strPreInvalidID;

		_tag_data_modol(){
			strPreBoxID.Format(_T("%02d01"),theMAINTENANCE_INFO.GetLineCode());
			strPreInvalidID.Format(_T("%02d02"),theMAINTENANCE_INFO.GetLineCode());
			cardSenserStatus = _th_rsp_get_dev_status();
			bCardSenserTestSuccess = false;
		}
	}Modol;

	Modol& GetDataModol();            //返回service的数据模型	
	void NotifyDataModelChanged();    //数据改变后，通知窗体更新数据

private:
	Modol m_Modol;                          //数据模型
	OPERATION_TYPE  m_OperationType;        //操作类型
	CurInputOP type_op;
	//WORD       m_wSmartCardFirstCommond;  //测试类型编号
	BYTE       m_wSmartCardSecondCommand;   //详细测试类型
	BYTE		m_bActionType;				// 票箱上升下降测试类型
	bool		m_bIsCardSendOut;			// 票卡发送是否送出？
	bool		m_bIsCardCatch;				// 是否已经抓到票卡？
	//TEST_SPEED m_eTransferSpeedSet;       //传送测试速度
	//BYTE       m_bIssueData[16];          //发票时，传给TH的16位BYTEBYTE
	BOOL       isMotorStart;
	BYTE       testType;             //电机的测试类型
	void       DoOperation();        //根据测试类型进行测试
	void       DoMotorTestOperation();  //根据测试类型进行测试
	void       DoReadRFIDTestOperation();  //根据测试类型进行测试
	void       DoWriteRFIDTestOperation();  //根据测试类型进行测试
	void       DoSensorTestOperation();  //根据测试类型进行测试
	void       ReadRFID();      //读票箱RFID
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// 处理 [Esc] 键按下消息（主菜单）
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //处理 [Enter] 键按下消息（执行）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					//处理 [F3/Ctrl]    键按下消息（返回）
	void OnStart();
	tTIMRFIDInfo m_rfidInfo;
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
};
