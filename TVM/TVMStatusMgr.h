#pragma once
#include "svcdef.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include "scdatamsg.h"
#include "delegate.h"

// TVM状态管理唯一对象
#define theTVM_STATUS_MGR  CStatusHelper::GetInstance()

/**
@brief  TVM状态管理类
*/
class CStatusHelper
{
public:
	CStatusHelper();
	~CStatusHelper();


	delegate<void()> TVMPaymentChanged;			//支付模式代码变化事件
	delegate<void()> TVMChangeChanged;			//充值模式代码变化事件
	delegate<void()> TVMStatusDispMessageChanged;// 状态提示信息变化事件
	
    static CStatusHelper& GetInstance();          // 获取该类的对象
	
	// 1.服务状态
	void SetServiceStatus(SERVICE_STATUS );       // 设置服务状态         
	SERVICE_STATUS GetServiceStatus();            // 获取服务状态

	// 2.运行状态
	void SetRunStatus(RUN_STATUS);                // 设置运行状态
	RUN_STATUS GetRunStatus();                    // 获取运行状态

	// 3.工作模式（业务模式）
	void SetWorkMode(WORD, bool = false);		  // 设置业务模式
	WORD GetWorkMode();							  // 获取业务模式
	inline WORK_MODE GetServerWorkMode(){
		return GetDegreeWorkMode();}			  // 获取业务模式

	// 4.运营模式
	void SetOperationMode(OperationalMode_t);		// 设置运营模式
	OperationalMode_t GetOperationMode();			// 获取运营模式

	// 5.24小时运营模式
	void Set24HourMode(OPERATION_24HOURS_MODE);		//  设置24小时运营模式
	OPERATION_24HOURS_MODE Get24HourMode();			//  获取24小时运营模式

	// 6.延长运营模式
	void SetDelayTime(WORD wDelayTime);				 //  设置延长运营模式
	WORD GetDelayTime();						     //  获取延长运营模式

	// 7.连接状态
	void SetCommunicate(COMMUNICATION);             // 设置通信状态
	COMMUNICATION GetCommunicate();                 // 获取通信状态
 
	// 8.时钟状态
	void SetTimeSynStatus(TIME_STATUS);             // 设置时钟同步状态
	TIME_STATUS GetTimeSynStatus();                 // 获取时钟同步状态

	// 9.登录状态
	void SetLoginStatus(LOGIN_STATUS);              // 设置登录状态
	LOGIN_STATUS GetLoginStatus();					// 获取登录状态

	// 10.设备自动运行状态
	void  SetAutoRunStatus(DEVICE_AUTORUN);			// 设置自动运行(预留)
	DEVICE_AUTORUN GetAutoRunStatus();              // 获取自动运行

	// 11~16票箱状态
	void SetCardBox1Status(STATUS_MAGAZINE);			// 设置Card箱1安装状态
	STATUS_MAGAZINE GetCardBox1Status();				// 获取Card箱1安装状态
	void SetCardBox1NumStatus(MAGAZINE_NUM_STATUS);		// 设置Card箱1数量状态
	MAGAZINE_NUM_STATUS GetCardBox1NumStatus();			// 获取Card箱1数量状态

	void SetCardBox2Status(STATUS_MAGAZINE);			// 设置Card箱2安装状态
	STATUS_MAGAZINE GetCardBox2Status();				// 获取Card箱2安装状态
	void SetCardBox2NumStatus(MAGAZINE_NUM_STATUS);		// 设置Card箱2数量状态
	MAGAZINE_NUM_STATUS GetCardBox2NumStatus();			// 获取Card箱2数量状态

	void SetCardWasteBoxStatus(STATUS_MAGAZINE);		// 设置Card回收箱安装状态
	STATUS_MAGAZINE GetCardWasteBoxStatus();			// 获取Card回收箱安装状态
	void SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS);	// 设置Card回收箱数量状态
	MAGAZINE_NUM_STATUS GetCardWasteBoxNumStatus();		// 获取Card回收箱数量状态


	// Token箱状态	
	void SetTokenAddBox1Status(STATUS_MAGAZINE);				// 设置Token补充箱1箱安装状态
	STATUS_MAGAZINE GetTokenAddBox1Status();					// 获取Token补充箱1箱安装状态
	void SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token补充箱1箱数量状态
	MAGAZINE_NUM_STATUS GetTokenAddBox1NumStatus();				// 获取Token补充箱1箱数量状态

	void SetTokenAddBox2Status(STATUS_MAGAZINE);				// 设置Token补充箱2箱安装状态
	STATUS_MAGAZINE GetTokenAddBox2Status();					// 获取Token补充箱2箱安装状态
	void SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token补充箱2箱数量状态
	MAGAZINE_NUM_STATUS GetTokenAddBox2NumStatus();				// 获取Token补充箱2箱数量状态

	void SetTokenHopper1Status(STATUS_MAGAZINE);				// 设置Token Hopper1箱安装状态（暂不使用）
	STATUS_MAGAZINE GetTokenHopper1Status();					// 获取Token Hopper1箱安装状态（暂不使用）
	void SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token Hopper1箱数量状态
	MAGAZINE_NUM_STATUS GetTokenHopper1NumStatus();				// 获取Token Hopper1箱数量状态

	void SetTokenHopper2Status(STATUS_MAGAZINE);				// 设置Token Hopper2箱安装状态（暂不使用）
	STATUS_MAGAZINE GetTokenHopper2Status();					// 获取Token Hopper2箱安装状态（暂不使用）
	void SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token Hopper2箱数量状态
	MAGAZINE_NUM_STATUS GetTokenHopper2NumStatus();				// 获取Token Hopper2箱数量状态

	void SetTokenWasteBoxStatus(STATUS_MAGAZINE);				// 设置Token废票箱安装状态
	STATUS_MAGAZINE GetTokenWasteBoxStatus();					// 获取Token废票箱安装状态
	void SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS);		// 设置Token废票箱数量状态
	MAGAZINE_NUM_STATUS GetTokenWasteBoxNumStatus();			// 获取Token废票箱数量状态

	void SetTokenCollectedBoxStatus(STATUS_MAGAZINE);			// 设置Token回收箱安装状态
	STATUS_MAGAZINE GetTokenCollectedBoxStatus();				// 获取Token回收箱安装状态
	void SetTokenCollectedBoxNumStatus(MAGAZINE_NUM_STATUS);	// 设置Token回收箱数量状态
	MAGAZINE_NUM_STATUS GetTokenCollectedBoxNumStatus();		// 获取Token回收箱数量状态

	// 硬币箱状态
	void SetCoinAddBox1Status(STATUS_MAGAZINE);				// 设置硬币补充箱1安装状态
	STATUS_MAGAZINE GetCoinAddBox1Status();					// 获取硬币补充箱1安装状态
	void SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS);		// 设置硬币补充箱1数量状态
	MAGAZINE_NUM_STATUS GetCoinAddBox1NumStatus();			// 获取硬币补充箱1数量状态

	void SetCoinAddBox2Status(STATUS_MAGAZINE);				// 设置硬币补充箱2安装状态
	STATUS_MAGAZINE GetCoinAddBox2Status();					// 获取硬币补充箱2安装状态
	void SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS);		// 设置硬币补充箱2数量状态
	MAGAZINE_NUM_STATUS GetCoinAddBox2NumStatus();			// 获取硬币补充箱2数量状态

	void SetCoinHopper1Status(STATUS_MAGAZINE);				// 设置硬币Hopper1箱安装状态(不使用)
	STATUS_MAGAZINE GetCoinHopper1Status();					// 获取硬币Hopper1箱安装状态(不使用)
	void SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS);		// 设置硬币Hopper1箱数量状态
	MAGAZINE_NUM_STATUS GetCoinHopper1NumStatus();			// 获取硬币Hopper1箱数量状态

	void SetCoinHopper2Status(STATUS_MAGAZINE);				// 设置硬币Hopper2箱安装状态(不使用)
	STATUS_MAGAZINE GetCoinHopper2Status();					// 获取硬币Hopper2箱安装状态(不使用)
	void SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS);		// 设置硬币Hopper2箱数量状态
	MAGAZINE_NUM_STATUS GetCoinHopper2NumStatus();			// 获取硬币Hopper2箱数量状态

	void SetCoinCircleHopperStatus(STATUS_MAGAZINE);		// 设置硬币循环找零Hopper箱安装状态（暂不使用）
	STATUS_MAGAZINE GetCoinCircleHopperStatus();			// 获取硬币循环找零Hopper箱安装状态（暂不使用）
	void SetCoinCircleHopperNumStatus(MAGAZINE_NUM_STATUS);	// 设置硬币循环找零Hopper箱数量状态（暂不使用）
	MAGAZINE_NUM_STATUS GetCoinCircleHopperNumStatus();		// 获取硬币循环找零Hopper箱数量状态（暂不使用）

	void SetCoinCollectionBox1Status(STATUS_MAGAZINE);		 // 设置硬币回收箱1安装状态
	STATUS_MAGAZINE GetCoinCollectionBox1Status();			 // 获取硬币回收箱1安装状态
	void SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS);// 设置硬币回收箱1数量状态
	MAGAZINE_NUM_STATUS GetCoinCollectionBox1NumStatus();	 // 获取硬币回收箱1数量状态

	void SetCoinCollectionBox2Status(STATUS_MAGAZINE);		 // 设置硬币回收箱2安装状态
	STATUS_MAGAZINE GetCoinCollectionBox2Status();			 // 获取硬币回收箱2安装状态
	void SetCoinCollectionBox2NumStatus(MAGAZINE_NUM_STATUS);// 设置硬币回收箱2数量状态
	MAGAZINE_NUM_STATUS GetCoinCollectionBox2NumStatus();	 // 获取硬币回收箱2数量状态

	// 纸币箱状态
	void SetBanknoteChangeBox1Status(STATUS_MAGAZINE);			// 设置纸币找零箱1安装状态
	STATUS_MAGAZINE GetBanknoteChangeBox1Status();				// 获取纸币找零箱1安装状态
	void SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS);	// 设置纸币找零箱1数量状态
	MAGAZINE_NUM_STATUS GetBanknoteChangeBox1NumStatus();		// 获取纸币找零箱1数量状态

	void SetBanknoteChangeBox2Status(STATUS_MAGAZINE);			// 设置纸币找零箱2安装状态
	STATUS_MAGAZINE GetBanknoteChangeBox2Status();				// 获取纸币找零箱2安装状态
	void SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS);	// 设置纸币找零箱2数量状态
	MAGAZINE_NUM_STATUS GetBanknoteChangeBox2NumStatus();		// 获取纸币找零箱2数量状态

	/*void SetBanknoteWasteBoxStatus(STATUS_MAGAZINE);			// 设置纸币废币箱安装状态（暂不使用）
	STATUS_MAGAZINE GetBanknoteWasteBoxStatus();				// 获取纸币废币箱安装状态（暂不使用）
	void SetBanknoteWasteBoxNumStatus(MAGAZINE_NUM_STATUS);		// 设置纸币废币箱数量状态（暂不使用）
	MAGAZINE_NUM_STATUS GetBanknoteWasteBoxNumStatus();			// 获取纸币废币箱数量状态（暂不使用）*/

	void SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS);			// 设置纸币循环鼓1数量状态
	MAGAZINE_NUM_STATUS GetBanknoteRE1NumStatus();				// 获取纸币循环鼓1数量状态
	void SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS);			// 设置纸币循环鼓2数量状态
	MAGAZINE_NUM_STATUS GetBanknoteRE2NumStatus();				// 获取纸币循环鼓2数量状态
	void SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS);			// 设置纸币循环鼓3数量状态
	MAGAZINE_NUM_STATUS GetBanknoteRE3NumStatus();				// 获取纸币循环鼓3数量状态
	void SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS);			// 设置纸币循环鼓4数量状态
	MAGAZINE_NUM_STATUS GetBanknoteRE4NumStatus();				// 获取纸币循环鼓4数量状态

	void SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE);		// 设置纸币回收箱安装状态
	STATUS_MAGAZINE GetBanknoteCollectionBoxStatus();			// 获取纸币回收箱安装状态
	void SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS);// 设置纸币回收箱数量状态
	MAGAZINE_NUM_STATUS GetBanknoteCollectionBoxNumStatus();	// 获取纸币回收箱数量状态

	// 现金安全门状态
	void SetSecurityDoorStatus(SECURITY_DOOR_STATUS);			// 设置现金安全门状态（预留）
	SECURITY_DOOR_STATUS GetSecurityDoorStatus();				// 获取现金安全门状态（预留）

	// 招援按钮
	void SetAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS);	// 设置招援按钮状态
	ASSISTANCE_BUTTON_STATUS GetAssistanceButtonStatus();		// 获取招援按钮状态

	// 读写器1状态
	void SetRW1Status(STATUS_RW);								// 设置读写器1状态
	STATUS_RW GetRW1Status();									// 获取读写器1状态

	// 读写器2状态
	void SetRW2Status(STATUS_RW);								// 设置读写器2状态
	STATUS_RW GetRW2Status();									// 获取读写器2状态

	// SAM1类型
	void SetSam1Type(SAM_STATUS);								// 设置Sam1类型
	SAM_STATUS GetSam1Type();									// 获取Sam1类型
	
	// SAM2类型
	void SetSam2Type(SAM_STATUS);								// 设置Sam2类型
	SAM_STATUS GetSam2Type();									// 获取Sam2类型

	// SAM1状态
	void SetSam1Status(SAM_AUTH_STATUS);						// 设置Sam1认证状态
	SAM_AUTH_STATUS GetSam1Status();							// 获取Sam1认证状态
	
	// SAM2状态
	void SetSam2Status(SAM_AUTH_STATUS);						// 设置Sam2认证状态
	SAM_AUTH_STATUS GetSam2Status();							// 获取Sam2认证状态

	// 打印机状态
	void SetPrintStatus(PRINTER_STATUS);						// 设置打印机状态
	PRINTER_STATUS GetPrintStatus();							// 获取打印机状态

	// 维修门状态
	void SetMaintenanceDoorStatus(DOOR_STATUS);                // 设置维修门状态
	DOOR_STATUS GetMaintenanceDoorStatus();					   // 获取维修门状态

	// 纸币回收模块状态
	void SetBanknoteModuleStatus(HARDWARE_MODULE_STATUS);		// 设置纸币模块状态
	HARDWARE_MODULE_STATUS GetBanknoteModuleStatus();			// 获取纸币模块状态

	// 纸币找零模块状态
	void SetBanknoteChangeModuleStatus(HARDWARE_MODULE_STATUS);	// 设置纸币找零模块状态
	HARDWARE_MODULE_STATUS GetBanknoteChangeModuleStatus();		// 获取纸币找零模块状态

	// 票卡发行模块状态
	void SetCardModuleStatus(HARDWARE_MODULE_STATUS);			// 设置Card模块状态
	HARDWARE_MODULE_STATUS GetCardModuleStatus();				// 获取Card模块状态

	// 硬币模块状态
	void SetCoinModuleStatus(HARDWARE_MODULE_STATUS);			 // 设置硬币模块状态
	HARDWARE_MODULE_STATUS GetCoinModuleStatus();				 // 获取硬币模块状态

	// LED模块状态
	void SetStatusDisplayStatus(HARDWARE_MODULE_STATUS);		// 设置状态显示器模块状态
	HARDWARE_MODULE_STATUS GetStatusDisplayStatus();			// 获取状态显示器模块状态
	// 触摸屏状态
	void SetTouchDisplayStatus(TOUCH_SCREEN_STATUS);			// 设置触摸屏状态（预留）
	TOUCH_SCREEN_STATUS GetTouchDisplayStatus();				// 获取触摸屏状态（预留）

	// 设备故障状态
	void SetDeviceErrStatus(long lMtcCode);						// 设置设备故障状态
	long GetDeviceErrStatus();									// 获取设备故障状态

	// 传输审计状态
	void SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS);			// 设置传输审计状态
	TRANSFER_AUDIT_STATUS GetDeviceTransferAudit();				// 取传输审计状态

	// 支付模式
	void SetPaymentMode(WORD,bool bIsDeviceControl = true);	// 设置支付模式
	WORD GetPaymentMode();										// 获取支付模式

	// 打印模式
	void SetPrintMode(WORD,bool bIsDeviceControl = true);		// 设置打印模式
	bool IsNoPrintMode();										// 是否是无打印模式
	WORD GetPrintMode();

	// 找零模式
	void SetChangeMode(WORD,bool bIsDeviceControl = true);		// 设置找零模式
	WORD GetChangeMode();										// 获取找零模式

	// 磁盘空间状态
	void SetDiskSpaceStatus(DISK_SPACE_CODE);					// 设置磁盘空间状态(暂时不用)
	DISK_SPACE_CODE GetDiskSpaceStatus();						// 获取磁盘空间状态(暂时不用)

	// 离线运行
	void SetOffLineDays(BYTE);									// 设置离线天数(暂时不用)
	BYTE GetOffLineDays();										// 获取离线天数(暂时不用)


	/*-----------------------------------分割线（以下郑州不使用）---------------------------------------------*/

	// 降级运行模式
	void SetDegradedMode(DEVICE_CONTRL_CODE);						// 设置降级运行模式
	bool IsCanAutoDegraded();										// 是否允许降级运行

	

	// Token箱状态
	/*
	void SetTokenAddBox1Status(STATUS_MAGAZINE);							// 设置Token补充箱1箱安装状态
	STATUS_MAGAZINE GetTokenAddBox1Status();								// 获取Token补充箱1箱安装状态
	void SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token补充箱1箱数量状态（暂不使用）
	MAGAZINE_NUM_STATUS GetTokenAddBox1NumStatus();				// 获取Token补充箱1箱数量状态（暂不使用）

	void SetTokenAddBox2Status(STATUS_MAGAZINE);							// 设置Token补充箱2箱安装状态
	STATUS_MAGAZINE GetTokenAddBox2Status();								// 获取Token补充箱2箱安装状态
	void SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token补充箱2箱数量状态（暂不使用）
	MAGAZINE_NUM_STATUS GetTokenAddBox2NumStatus();				// 获取Token补充箱2箱数量状态（暂不使用）

	void SetTokenHopper1Status(STATUS_MAGAZINE);							// 设置Token Hopper1箱安装状态（暂不使用）
	STATUS_MAGAZINE GetTokenHopper1Status();									// 获取Token Hopper1箱安装状态（暂不使用）
	void SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS);			// 设置Token Hopper1箱数量状态
	MAGAZINE_NUM_STATUS GetTokenHopper1NumStatus();				// 获取Token Hopper1箱数量状态

	void SetTokenHopper2Status(STATUS_MAGAZINE);							// 设置Token Hopper2箱安装状态（暂不使用）
	STATUS_MAGAZINE GetTokenHopper2Status();									// 获取Token Hopper2箱安装状态（暂不使用）
	void SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS);					// 设置Token Hopper2箱数量状态
	MAGAZINE_NUM_STATUS GetTokenHopper2NumStatus();							// 获取Token Hopper2箱数量状态

	void SetTokenWasteBoxStatus(STATUS_MAGAZINE);							// 设置Token回收箱安装状态
	STATUS_MAGAZINE GetTokenWasteBoxStatus();								// 获取Token回收箱安装状态
	void SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS);		// 设置Token回收箱数量状态
	MAGAZINE_NUM_STATUS GetTokenWasteBoxNumStatus();				// 获取Token回收箱数量状态*/

	// Card箱状态
	//void SetCardBox1Status(STATUS_MAGAZINE);									// 设置Card箱1安装状态
	//STATUS_MAGAZINE GetCardBox1Status();											// 获取Card箱1安装状态
	//void SetCardBox1NumStatus(MAGAZINE_NUM_STATUS);					// 设置Card箱1数量状态
	//MAGAZINE_NUM_STATUS GetCardBox1NumStatus();						// 获取Card箱1数量状态

	//void SetCardBox2Status(STATUS_MAGAZINE);									// 设置Card箱2安装状态
	//STATUS_MAGAZINE GetCardBox2Status();											// 获取Card箱2安装状态
	//void SetCardBox2NumStatus(MAGAZINE_NUM_STATUS);						// 设置Card箱2数量状态
	//MAGAZINE_NUM_STATUS GetCardBox2NumStatus();						// 获取Card箱2数量状态

	//void SetCardWasteBoxStatus(STATUS_MAGAZINE);							// 设置Card回收箱安装状态
	//STATUS_MAGAZINE GetCardWasteBoxStatus();									// 获取Card回收箱安装状态
	//void SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS);			// 设置Card回收箱数量状态
	//MAGAZINE_NUM_STATUS GetCardWasteBoxNumStatus();				// 获取Card回收箱数量状态

	//void SetCardRWStatus(STATUS_RW);								// 设置Card RW状态
	//STATUS_RW GetCardRWStatus();									// 获取Card RW状态

	//void SetTokenRWStatus(STATUS_RW);								// 设置Token RW状态
	//STATUS_RW GetTokenRWStatus();									// 获取Token RW状态

	
	//void SetSam3Type(SAM_STATUS);                     // 设置Sam3类型（暂不使用）
	//SAM_STATUS GetSam3Type();							// 获取Sam3类型（暂不使用）
	//void SetSam4Type(SAM_STATUS);                     // 设置Sam4类型（暂不使用）
	//SAM_STATUS GetSam4Type();							// 获取Sam4类型（暂不使用）

	
	//void SetSam3Status(SAM_STATUS);                 // 设置Sam3认证状态
	//SAM_STATUS GetSam3Status();                       // 获取Sam3认证状态
	//void SetSam4Status(SAM_STATUS);                 // 设置Sam4认证状态
	//SAM_STATUS GetSam4Status();                       // 获取Sam4认证状态	

	//void SetUPSStatus(UPS_STATUS);						// 设置UPS状态（暂不使用）
	//UPS_STATUS GetUPSStatus();								// 获取UPS状态（暂不使用）
	

	//void SetTokenModuleStatus(HARDWARE_MODULE_STATUS); // 设置Token模块状态
	//HARDWARE_MODULE_STATUS GetTokenModuleStatus();		// 获取Token模块状态	

	//void SetTiltStatus(TILT_STATUS);				// 设置倾斜传感器状态
	//TILT_STATUS GetTiltStatus();					// 获取倾斜传感器状态

	void SetAuthorizedAccessStatus(AUTHORIZED_STATUS);		// 设置授权访问状态
	AUTHORIZED_STATUS GetAuthorizedAccessStatus();			// 获取授权访问状态

private:
	CStatusHelper(const CStatusHelper&);
	CStatusHelper& operator=(const CStatusHelper&);
	void OnExceptionItemNotify(CSysException&,bool);
	CSCDataMsg* MakeStatusChangeCommand(TVM_STATUS_ID statusID,BYTE bValue);
	CSCDataMsg* MakeErrorStatusCommand(long lMtcCode);

	static CStatusHelper						theInstance;
	SERVICE_STATUS							m_sServiceStatus;			// 服务状态
	RUN_STATUS								m_sRunStatus;				// 运行状态
	WORD									m_sWorkMode;				// 工作模式
	OperationalMode_t						m_OperationMode;			// 运营模式，参见7.9运营模式代码。
	OPERATION_24HOURS_MODE				m_s24HourMode;					// 24小时运营模式
	WORD							m_sDelayTime;						// 延长运营时间									
	COMMUNICATION						m_sConnectStatus;				// 连接状态
	TIME_STATUS								m_sTimeStatus;				// 时钟差异
	LOGIN_STATUS								m_sLoginStatus;			// 登录状态
	DEVICE_AUTORUN						m_sAutorunStatus;				// 设备自动运行状态
	long								m_lDeviceErrStatus;				// 设备故障状态
	TRANSFER_AUDIT_STATUS            m_sTransferAudit;					// 传输审计状态
	WORD											m_sPaymentMode;		// 支付模式
	WORD											m_sPrintMode;		// 打印模式
	WORD											m_sChangeMode;		// 找零模式
	DISK_SPACE_CODE						m_sDiskSpaceStatus;				// 磁盘空间状态

	STATUS_MAGAZINE						m_sTokenBox1Status;					// Token补充箱1安装状态
	MAGAZINE_NUM_STATUS			m_sTokenBox1NumStatus;			// Token补充箱1数量状态（暂不使用）
	STATUS_MAGAZINE						m_sTokenBox2Status;					// Token补充箱2安装状态
	MAGAZINE_NUM_STATUS			m_sTokenBox2NumStatus;			// Token补充箱2数量状态（暂不使用）
	STATUS_MAGAZINE						m_sTokenHopper1Status;				// Token Hopper箱1安装状态（暂不使用）
	MAGAZINE_NUM_STATUS			m_sTokenHopper1NumStatus;		// Token Hopper箱1数量状态
	STATUS_MAGAZINE						m_sTokenHopper2Status;				// Token Hopper箱2安装状态（暂不使用）
	MAGAZINE_NUM_STATUS			m_sTokenHopper2NumStatus;		// Token Hopper箱2数量状态
	STATUS_MAGAZINE						m_sTokenWasteBoxStatus;				// Token废票箱安装状态
	MAGAZINE_NUM_STATUS			m_sTokenWasteBoxNumStatus;		// Token废票箱数量状态
	STATUS_MAGAZINE						m_sTokenCollectedBoxStatus;			// Token废票箱安装状态
	MAGAZINE_NUM_STATUS			m_sTokenCollectedBoxNumStatus;	// Token废票箱数量状态


	STATUS_MAGAZINE						m_sCardBox1Status;					// Card箱1安装状态
	MAGAZINE_NUM_STATUS			m_sCardBox1NumStatus;			// Card箱1数量状态
	STATUS_MAGAZINE						m_sCardBox2Status;					// Card箱2安装状态
	MAGAZINE_NUM_STATUS			m_sCardBox2NumStatus;			// Card箱2数量状态
	STATUS_MAGAZINE						m_sCardWasteBoxStatus;			// Card废票箱安装状态
	MAGAZINE_NUM_STATUS			m_sCardWasteBoxNumStatus;	// Card废票箱数量状态

	STATUS_MAGAZINE						m_sCoinBox1Status;					// Coin补充箱1安装状态
	MAGAZINE_NUM_STATUS			m_sCoinBox1NumStatus;						// Coin补充箱1数量状态
	STATUS_MAGAZINE						m_sCoinBox2Status;					// Coin补充箱2安装状态
	MAGAZINE_NUM_STATUS			m_sCoinBox2NumStatus;						// Coin补充箱2数量状态
	STATUS_MAGAZINE						m_sCoinHopper1Status;				// Coin Hopper箱1安装状态（暂不使用）
	MAGAZINE_NUM_STATUS			m_sCoinHopper1NumStatus;					// Coin Hopper箱1数量状态
	STATUS_MAGAZINE						m_sCoinHopper2Status;				// Coin Hopper箱2安装状态（暂不使用）
	MAGAZINE_NUM_STATUS			m_sCoinHopper2NumStatus;					// Coin Hopper箱2数量状态
	STATUS_MAGAZINE						m_sCoinCircleHopperStatus;			// Coin 循环找零Hopper箱安装状态（暂不使用）
	MAGAZINE_NUM_STATUS			m_sCoinCircleHopperNumStatus;				// Coin 循环找零Hopper箱数量状态
	STATUS_MAGAZINE						m_sCoinCollectionBox1Status;		// Coin回收箱1安装状态
	MAGAZINE_NUM_STATUS			m_sCoinCollectionBox1NumStatus;				// Coin回收箱1数量状态
	STATUS_MAGAZINE						m_sCoinCollectionBox2Status;		// Coin回收箱2安装状态
	MAGAZINE_NUM_STATUS			m_sCoinCollectionBox2NumStatus;				// Coin回收箱2数量状态

	STATUS_MAGAZINE				m_sBanknoteChangeBox1Status;				// 纸币找零箱1安装状态
	MAGAZINE_NUM_STATUS			m_sBanknoteChangeBox1NumStatus;				// 纸币找零箱1数量状态
	STATUS_MAGAZINE				m_sBanknoteChangeBox2Status;				// 纸币找零箱2安装状态
	MAGAZINE_NUM_STATUS			m_sBanknoteChangeBox2NumStatus;				// 纸币找零箱2数量状态

	MAGAZINE_NUM_STATUS			m_sBanknoteRE1NumStatus;					// 纸币循环鼓1数量状态
	MAGAZINE_NUM_STATUS			m_sBanknoteRE2NumStatus;					// 纸币循环鼓2数量状态
	MAGAZINE_NUM_STATUS			m_sBanknoteRE3NumStatus;					// 纸币循环鼓3数量状态
	MAGAZINE_NUM_STATUS			m_sBanknoteRE4NumStatus;					// 纸币循环鼓4数量状态

	STATUS_MAGAZINE				m_sBanknoteCollectionBoxStatus;				// 纸币回收箱安装状态
	MAGAZINE_NUM_STATUS			m_sBanknoteCollectionBoxNumStatus;			// 纸币回收箱数量状态

	SECURITY_DOOR_STATUS				m_sSecurityDoorStatus;				// 现金安全门状态（暂不使用）
	TOUCH_SCREEN_STATUS				m_sTouchScreenStatus;					// 触摸屏状态（暂不使用）
	ASSISTANCE_BUTTON_STATUS		m_sAssistanceButtonStatus;				// 招援按钮状态
	
	STATUS_RW									m_sRechargeRWStatus;		// 充值读写器状态
	STATUS_RW									m_sCardRWStatus;			// Card读写器状态
	STATUS_RW									m_sTokenRWStatus;			// Token读写器状态

	SAM_STATUS								m_sSam1Type;				// SAM1类型(充值读写器一票通 ISAM)（暂不使用）
	SAM_STATUS								m_sSam2Type;				// SAM2类型(Card读写器一票通 ISAM)（暂不使用）
	SAM_STATUS								m_sSam3Type;				// SAM3类型(Token读写器一票通 ISAM)（暂不使用）

	SAM_AUTH_STATUS							m_sSAM1Status;				// SAM1状态(充值读写器一票通 ISAM)
	SAM_AUTH_STATUS							m_sSAM2Status;					// SAM2状态(Card读写器一票通 ISAM)
	SAM_STATUS								m_sTokenRWYPTISAMStatus;				// SAM3状态(Token读写器一票通 ISAM)
	SAM_STATUS								m_sRechargeRWYKTISAMStatus;				// SAM4状态(充值读写器一卡通ISAM)(暂不使用)

	PRINTER_STATUS							m_sPrintStatus;						// 打印机状态
	UPS_STATUS									m_sUPSStatus;						// UPS状态
	DOOR_STATUS								m_sMaintenanceDoorStatus;	// 维修门状态

	HARDWARE_MODULE_STATUS     m_sBanknoteModuleStatus;		 // 纸币模块状态
	HARDWARE_MODULE_STATUS	   m_sBanknoteChangeModuleStatus;// 纸币找零模块状态
	HARDWARE_MODULE_STATUS     m_sTokenModuleStatus;		 // Token模块状态
	HARDWARE_MODULE_STATUS     m_sCardModuleStatus;			 // Card模块状态
	HARDWARE_MODULE_STATUS	    m_sCoinModuleStatus;	 	 // 硬币模块状态
	HARDWARE_MODULE_STATUS	    m_sStatusDisplayStatus;		 // 状态显示器状态
	TILT_STATUS									m_sTiltStatus;					// 倾斜传感器状态(震动传感器状态)
	BYTE										m_sOffLineDays;					// 离线天数
	AUTHORIZED_STATUS					m_sAuthorizedAccessStatus; // 授权访问状态 
	
	// 运行模式定义
	DEVICE_CONTRL_CODE				m_sDegradedMode;				// 降级运行模式
	//CGetIni  *pINI;
	WORD GetStatusFromINI(CString strSection);

	bool IsSCConnected();

public:
	// 工作模式
	WORK_MODE GetDegreeWorkMode();
};