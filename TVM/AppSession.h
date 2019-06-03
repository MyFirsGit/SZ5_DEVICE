#include "childview.h"
#include "ChildViewRec.h"
#include "servicedefinition.h"
#include "SvcDef.h"
#pragma once


#define theAPP_SESSION  CAppSession::GetInstance()

/**
  @brief 对TVM全体业务相关信息进行管理的类
*/
class CAppSession
{
public:
    static CAppSession& GetInstance();                                  // 获取该类的对象

	// 在线状态
    void SetSCConnected(bool);                                         // 设置在线状态
    bool IsSCConnected() const;                                        // 判断是否在线
    void SetHasNewParam(bool);                                          // 设置是否有新参数（从上位下载）
    bool HasNewParam() const;                                           // 判断是否有新参数（从上位下载）

	void SetPhysicalConnStatus(bool);									// 设置物理连接状态
	bool GetPhysicalConnStatus();										// 获取物理连接状态

	//void SetStartOperationFlag(bool);									// 设置开始运营标志
	//bool GetStartOperationFlag();										// 获取开始运营标志（上传设备状态用）

	void SetCloseOperationFlag(bool);									// 设置运营结束标志
	bool GetCloseOperationFlag() const;									// 获取运营结束标志

    // 初期化失败
    bool IsInitCheckError() const;                                      // 判断初期化失败
    void SetInitCheckError(bool);                                       // 设置初期化失败
//
//	bool IsInitDetailError(INIT_DETAIL_ERROR);							// 是否存在某个初期化错误
	void SetInitDetailErrorFlag(INIT_DETAIL_ERROR);						// 设置某个初期化错误
//	void InitDetailErrorInfo();											// 初期化错误信息
//	void RemoveInitDetailErrorFlag(INIT_DETAIL_ERROR);					// 移除某个初期化错误
	DWORD GetInitDetailErrorFlag();										// 获取初期化错误标志
	DWORD IsInitErrorOver();											// 是否存在需要中止的初期化错误
//
	void SetInitDetailErrorMsg(CString);								// 设置初期化详细错误文言（预留）
	CString GetInitDetailErrorMsg();									// 获取初期化详细错误文言（预留）
//
	// TH端口是否连接
	bool IsTHConnected() const;                                         // 判断TH端口是否连接
	void SetIsTHConnected(bool);                                        // 设置TH端口是否连接

	// TOKEN端口是否连接
	bool IsTOKENModuleConnected() const;                                // 判断TOKEN端口是否连接
	void SetIsTOKENModuleConnected(bool);                               // 设置TOKEN端口是否连接

	// CH端口是否连接
	bool IsCHConnected() const;                                         // 判断CH端口是否连接
	void SetIsCHConnected(bool);                                        // 设置CH端口是否连接

	bool IsIOConnected() const;											// 判断IO端口是否连接
	void SetIsIOConnected(bool);										// 设置IO端口是否连接

	bool IsBankCardConnected() const;									// 判断银行卡端口是否连接
	void SetIsBankCardConnected(bool);									// 设置银行卡端口状态

	bool IsWarningStart() const;										// 判断震动传感器是否报警
	void SetIsWarningStart(bool);										// 设置震动传感器报警状态

    // 程序第一次启动
    bool IsFirstRun() const;                                            // 判断是否程序第一次启动
    void SetIsFirstRun(bool);                                           // 设置是否程序第一次启动

	// 刷卡开后维修门
	int GetCardOpenMainDoorOperatorID() const;                      // 获取刷卡开后维修门的员工ID
	void SetCardOpenMainDoorOperatorID(int operatorID);                  // 设置刷卡开后维修门的员工ID

//    // 有无未送数据
//    bool HasDeliverData() const;                                        // 判断是否有未送数据
//    void SetHasDeliverData(bool);                                       // 设置是否有未送数据
//    // 版本信息电文
//    void SaveVersion(const VARIABLE_DATA& binData);                      // 保存电文信息
//    void GetVersion(VARIABLE_DATA& binData);                             // 获取电文信息
    void SetHasProgUpdate(bool);                                        // 设置是否有程序更新
    bool HasProgUpdate();                                               // 判断是否有程序更新

//    void SetIsDownload(bool);                                           // 设置是否正在FTP中
//    bool GetIsDownload() const;                                         // 获取是否正在FTP中
//
//    void SetIsUpdateParam(bool);                                        // 设置是否正在下载参数中
//    bool GetIsUpdateParam() const;                                      // 获取是否正在下载参数中
//
//    void SetIsWorkingHourChange(bool);                                  // 设置是否运营日切换
//    bool GetIsWorkingHourChange() const;                                // 获取是否运营日切换

	// SAM信息
    void SetSAMInfo(SAM_TYPE ,const LPBYTE);							// 设置SAM信息
    SAM_ID_INFO GetSAMInfo();											// 获取SAM信息

	// 工作处理类型
	CLOSE_START_FLAG GetCloseStartFlag() const;                         // 获取工作处理类型
	void SetCloseStartFlag(const CLOSE_START_FLAG);                     // 设置工作处理类型 

	// 站员信息
	bool IsSuperUser();
	USER_INFO GetUserInfo() const;                                      // 获取站员信息
	void SetUserInfo(const USER_INFO&);                                 // 设置站员信息
	bool HasUserLogin();			 									// 是否有站员登录？
	void SetUserLoginStatus(bool bIsLogin);

    // 车站信息
    CString GetStationName(LANGUAGETYPE_T ) const;                      // 获取当站车站名称
    void SetStationName(const CString, const CString, const CString);   // 设置当站车站名称

    void ShowReceptionGuide(DWORD msgID );                              // 显示乘客画面向导信息
	void ShowReceptionGuide(CString recpMsgCn, CString recpMsgEn ,CString guidePoMsg=_T(""));  // 显示乘客画面向导信息

    void ShowAllGuide(DWORD officeMsgID,DWORD msgID);                   // 显示站员和乘客画面向导信息
	void ShowAllGuide(CString officeMsg, CString recpMsgCn, CString recpMsgEn, CString guidePoMsg=_T(""));                   // 显示站员和乘客画面向导信息

    CChildView* GetChildView();                                         // 获取ChildView
    CChildViewRec* GetChildViewRec();                                   // 获取顾客视窗ChildViewRec
    CString GetServiceName(SERVICE_ID serviceID,LANGUAGETYPE_T languageType = LANGUAGE_ID_UNSPECIFIED);                     // 获取业务名称（中文）
    void ParseGuideMsg(CString originalGuideMsg, CString& guideMsg, COLORREF& color);                                               // 解析向导信息

//    bool HasARToSend();                                                 // 是否有AR数据需要发送
//    void SetARToSend(bool need);                                        // 设置是否有AR数据需要发送
//	  void SetRePrintStaffID(CString);									// 设置前次打印员工ID
//	  void SetRePrintLoginTime(_DATE_TIME);								// 设置前次打印登录时间
//	  void SetRePrintGroupID(CString);									// 设置前次打印班ID
//    CString GetRePrintStaffID();										// 取得前次打印员工ID
//	  _DATE_TIME GetRePrintLoginTime();									// 取得前次打印登录时间
//	  CString GetRePrintGroupID();										// 取得前次打印班ID
	delegate<void(LANGUAGETYPE_T&)> ClientLanguageModeChanged;
	delegate<void(LANGUAGETYPE_T&)> OperatorLanguageModeChanged;
	static LANGUAGETYPE_T GetCurrentClientLanguageType();
	void SetCurrentClientLanguageType(LANGUAGETYPE_T languageType = LANGUAGE_ID_CHINESE);
	static LANGUAGETYPE_T GetCurrentOperatorLanguageType();
	void SetCurrentOperatorLanguageType(LANGUAGETYPE_T languageType = LANGUAGE_ID_CHINESE);

	static LANGUAGETYPE_T GetCurrentBlindPlayVoiceLanguageType();
	// ////////////////////////////////////////////////////////////////////////////////////////////////
	// 维护框架部分

	// 操作员向导栏信息
	void ShowOfficeGuide(DWORD id);					// 设置站员向导栏信息（用于设置提示文言）
	void ShowOfficeGuide(CString msg);				// 设置站员向导栏信息（用于设置提示文言）
	void SetGuideWhenDelete(CString msg);			// 设置删除为空时的向导栏信息
	CString GetGuideWhenDelete();					// 获取删除为空时的向导栏信息

	// 操作员维护键盘
	void SetKeyboard(DWORD keys);					// 启用维护键盘指定按键，枚举KEYBOARD可以按位与
	void DiableKeyboard(DWORD keys);				// 禁用维护键盘指定按键
	void SetMaxInputLength(int length);				// 设置最大输入位数（不包括前缀）
	void SetPrefixToGuide(CString prefix);			// 设置输入文言的前缀
	CString GetPrefixGuide();						// 获取输入文言前缀
	void AppendOneNumToGuide(int number);			// 追加单个数字到输入内容
	void AppendOneDotToGuide();						// 追加单个圆点“.”到输入内容
	bool DeleteOneCharFromGuide();					// 删除单个字符（数字或圆点）
	void DeleteAllInput();							// 删除所有输入文言
	int		GetInputToInt();						// 获取向导栏的输入内容
	CString GetInputToString();						// 获取向导栏的输入内容
	bool	IsEmptyPrefix();						// 判断前缀是否为空

	void ReturnMaintainMenu();						// 返回维护业务菜单（1级菜单或2级菜单）
	void SetMaintainMenu(SERVICE_ID serviceID);
	SERVICE_ID GetMaintainMenu();
	SERVICE_ID ResetReturnSeviceID();				// 重置返回维护业务菜单的ID
	SERVICE_ID GetReturnSeviceID(){return m_return_svcid;};
	bool IsReturnMaintainMenu(bool bSetStatus = false);	// 是否返回维护业务？

	// 操作员菜单导航栏
	void SetMenuLevel(SERVICE_ID serviceID);		// 设置菜单导航级别
	void AppendMenuLevel(CString level);			// 追加菜单导航级别

	void ShowStatusWaringMsg(LANGUAGE_MSG strMsg,bool bIsShow = true);// 显示告警信息给站员

	bool IsBlindManOperation();						// 判断是否是盲人购票
	void SetBlindManStatus(bool);					// 设置是否是盲人购票？

	bool IsManualCloseOperation();						// 判断是否是人工业务结束？
	void SetManualCloseOperation(bool isManual = true);	// 设置是否人工业务结束

	void ShowMessageToPassenger(LANGUAGE_MSG );			// 显示信息到乘客界面右上角	
	LANGUAGE_MSG GetMessageShowPassenger();				// 取需要显示到乘客界面右上角的信息

	void SetReceiptPrintType(const RECEIPT_PRINT_TYPE& );	// 设置凭证打印类型
	RECEIPT_PRINT_TYPE GetReceiptPrintType();				// 获取凭证打印类型

	bool IsCoinNeedSyncMasterFile();						// 硬币模块是否应该同步计数
	void SetCoinNeedSyncMasterFile(bool bNeed = false);		// 设置硬币模是否应该同步计数

	void SetRechargeServerEnabled(bool bEnabled);			// 充值服务器不可用 
	bool IsRechargeServerEnabled();							// 充值服务器是否可用 

private:

    bool m_isSCConnected;                                              // 是否在线
	bool m_isPhysicalConnStatus;										// 建立连接时序
	bool m_isStartOperation;											// 是否正在运营开始时序
    bool m_hasNewParam;                                                 // 是否有新参数（从上位下载）
    bool m_hasProgUpdate;                                               // 是否有程序更新
    bool m_isFirstRun;                                                  // 是否第一次运行程序
      bool m_InitCheckError;                                              // 初期检查失败
      DWORD m_initDetailErrorFlag;										// 初期化详细错误标志
	CString m_initDetailErrorMsg;										// 初期化详细错误文言
//    bool m_isDownload;                                                  // 是否正在FTP中
//    bool m_needSendAR;                                                  // 是否有AR数据需要发送
	bool m_isTHConnected;                                               // TH端口是否连接
	bool m_isTOKENConnected;                                            // TOKEN端口是否连接
	bool m_isCHConnected;                                               // CH端口是否连接
	bool m_isIOConnected;												// IO端口是否连接
	bool m_isBankCardConnected;											// 银行卡是否连接
	bool m_isWarningStart;												// 是否触发震动传感器报警？
	bool m_isDisableOrBlindOperation;									// 是否是盲人购票？
	bool m_isManualCloseOperation;										// 是否是人工做业务结束？
	bool m_isCloseOperationFlag;										// 业务结束标志
//    bool m_isUpdateParameter;                                           // ACC参数是否正在更新
//    bool m_isWorkingHourChange;                                         // 是否运营日切换
	CLOSE_START_FLAG  m_closeStartFlag;                                 // 日终处理标志
	USER_INFO         m_userInfo;                                       // 站员信息
	bool			  m_bIsUserLogin;									// 是否有站员登录？
    CString           m_stationNameCN;                                  // 车站名称（中文）
    CString           m_stationNameEN;                                  // 车站名称（英文）
	CString           m_stationNamePN;                                  // 车站名称（葡萄牙文）
    SAM_ID_INFO   m_SAMInfo;                                   // SAMid是否改变
	bool m_bIsReturnMainMenu;											// 是否返回主菜单？

	bool m_bRechargeServerEnabled;										// 充值服务器是否可用
//
//	CString m_printStaffID;												// 前次员工ID
//	_DATE_TIME m_printLoginTime;										// 前次员工登录时间
//	CString m_printGroupID;												// 前次班ID

   static LANGUAGETYPE_T m_currentRecpLanguage;                                // 當前乘客界面語言
   static LANGUAGETYPE_T m_currentOperatorLanguage;                            // 當前操作員界面語言
   static LANGUAGETYPE_T m_currentBlindPlayVoiceLanguage;					   // 当前盲人购票语音
   int m_guide_max;							// 文言栏最大输入数量，例如：只能输入2位数字
   CString m_guide_prefix;					// 文言栏前缀部分，例如：“Line Code: ”
   CString m_guide_input;					// 文言栏输入部分，例如：“0x08”
   CString m_guide_delete;					// 删除为空时的文言
   SERVICE_ID m_return_svcid;				// 返回到维护业务菜单之前的业务ID
   SERVICE_ID m_maintance_svcid;			// 二次登陆验证保存已选业务ID
   LANGUAGE_MSG m_EmShowMessage;			// 显示给乘客的信息

   RECEIPT_PRINT_TYPE m_receipt_print_type; // 凭证打印的类型
   bool m_bNeedSyncMasterFile;				// 是否需要同步计数

public:
   UINT GetBNRRecyclerNearEmpty(int recyclerNo);
   void SetBNRRecyclerNearEmpty(int recyclerNo, UINT nearEmptyNum);
private:
   int m_openMainDoorOperatorID;
   UINT m_uBNRRecyclerNearEmpty_A;			// 纸币模块循环鼓A将空数（保存循环鼓低水位阀值，用于纸币模块业务中自动补币时软件数调整）
   UINT m_uBNRRecyclerNearEmpty_B;			// 纸币模块循环鼓A将空数
   UINT m_uBNRRecyclerNearEmpty_C;			// 纸币模块循环鼓A将空数
   UINT m_uBNRRecyclerNearEmpty_D;			// 纸币模块循环鼓A将空数
private:
	static CAppSession theInstance;                                     // 该类的唯一接口
    CAppSession();
    ~CAppSession(){};
    CAppSession(const CAppSession&);
    CAppSession& operator=(const CAppSession&);
};