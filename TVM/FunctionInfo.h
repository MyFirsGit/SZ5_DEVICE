#pragma once

#define theFunction_INFO CFunctionInfo::GetInstance()       // 功能配置唯一接口

/**
@brief FunctionInfo.INI的信息管理，
*/
class CFunctionInfo
{
public:

    static CFunctionInfo&  GetInstance()
    {
        return theInstance;
    }

    int Initialize();                              // 初期化

	// === TVM功能配置 ===
	int  GetACCAuthorizeManner();	                    // ACC认证方式（0：不认证；1：内部模式；2：外部模式）  
	int  GetECTAuthorizeManner();                      // ECT认证方式（0：不认证；1：内部模式；2：外部模式）
	bool IsTpuParamterZipFlag();                        // TPU参数压缩标志（0：不压缩；1：需要压缩）
	bool IsUseFounderStaffID();							// 是否启用默认员工
	int GetFounderStaffID();								// 获取默认员工ID
	bool IsFounderStaffID(int userID);					// 是否是默认员工
	CString GetFounderStaffPassword();				// 获取默认员工密码
	bool IsFounderOperation();                          // 是否启用高级功能（0：不启用；1：启用）
	bool GetUSBAuthFlag();							   // USB认证标志（0：不认证；1：需要认证）
	bool IsStaffDefaultService();						   // 操作员登录后默认业务（Service ID）
	bool IsDeleteParameterHead();						// 是否删除参数协议头和尾部
	bool IsShowInitProcess();							// 是否显示初始化细节过程
	bool IsEquipmentAutomaticRunningOn();				// 设备自动运行状态是否开启？
	bool IsShowNearlyFullEmptyParaValue();				// 是否显示设备空及将空参数值
	int  GetPrintLastReceiptCount();					// 取最近的打印失败的条数
	bool IsShowPaymentMethod();							// 是否显示支付方式区域
	bool IsNeedPassengerClickOkButton();				// 是否需要用户点击确认按钮
	bool IsAutoDegradeWhenCashDoorOpen();				// 是否检查现金门状态？
	int GetLoadMaxTransactionHistory();					// 最大可加载交易历史记录条数
	bool IsBanknoteChangeBoxIDRead();					// 纸币找零模块RFID是否使用？
	//=== 交易数据 ===
	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();

	void SetLimitedDaysOfSendData(int nDays);

	// === 线路配置（0:不可用，1：可用） ===
	bool    IsLineEnabled(DWORD lineCode);           // 判断指定线路按钮是否可用
    
	// === 车站配置（0:不可用，1：可用） ===
	bool    IsStationEnabled(WORD stationCode);    // 判断指定车站按钮是否可见

	// === 菜单按钮配置===
	bool IsMenuEnabled(WORD menuID);				// 按钮是否可用
	bool IsMenuVisibled(WORD menuID);				// 按钮是否可见

	//=== 可售票种配置(0:不可售，1：可售) ===
	bool IsTicketIssueEnabled(WORD TicketTypeId);														// 判断车票是否可发售
	bool IsPayTypeEnable(WORD ServiceID,CardType_t TicketType,int PayTypeId);					// 判断付款方式是否可用

	//=== 售票张数 ===
	vector<int> GetTicketCounts();																					// 获取车票销售张数按钮信息

	//=== 售票金额（按金额购票）===
	vector<int> GetTicketAmount();


	int GetTicketMaxAmount() const;
	void SetTicketMaxAmount(int )const ;

	//=====RFID设置=========
	bool IsCardNormalBoxRFIDWriteable();
	bool IsCardWasteBoxRFIDWriteable();
	bool IsCoinChangeBoxRFIDWriteable();
	bool IsCoinRecycleBoxRFIDWriteable();
	bool IsBanknoteChangeBoxRFIDWriteable();
	bool IsBanknoteRecycleBoxRFIDWriteable();
	bool IsTokenNormalBoxRFIDWriteable();
	bool IsTokenWasteBoxRFIDWriteable();

	bool IsAutoReadRfidWithoutManual();					// 是否自动读取RFID

	//=====警报设定=========

	CString IsCollectOverFile();                         // 回收动作结束报警音路径                             
	int IsCollectOverBuzzerTime();					 // 回收动作结束报警音播放时长（单位：秒）  
	int IsCollectOverBuzzerInterval();				 // 回收动作结束报警音播放时间间隔（单位：秒）
	CString IsExceptionOccurFile();				     // 异常发生报警音路径                             
	int IsExceptionOccurBuzzerTime();				 // 异常发生报警音播放时长（单位：秒）  
	int IsExceptionOccurBuzzerInterval();			 // 异常发生报警音播放时间间隔（单位：秒）
	CString IsOperationMissFile();                    // 异常发生报警音路径                             
	int IsOperationMissBuzzerTime();				 // 异常发生报警音播放时长（单位：秒）  
	int IsOperationMissBuzzerInterval();			 // 异常发生报警音播放时间间隔（单位：秒）
	CString IsDemotionOccurFile();                  // 自动降级发生报警音路径                             
	int IsDemotionOccurBuzzerTime();			 // 自动降级发生报警音播放时长（单位：秒）  
	int IsDemotionOccurBuzzerInterval();		 // 自动降级发生报警音播放时间间隔（单位：秒）

	//=====获取LED显示文言=========
	CString getModeLanguageCN(WORD wdModeCode);
	CString getModeLanguageEN(WORD wdModeCode);
	CString getModeLanguagePN(WORD wdModeCode);

	CString getModeLanguage(WORD);
	CString getModeLanguage(WORD ,int languageId);
	CString getLedWorkMode();

	void getLedContentFormat(CStringList& strList);

	//=====获取售票票箱选择=======
	int GetAvailableMagazineBox();					// 获取可使用的售票票箱

	//=====乘客业务启动默认业务异常解除次数=======
	int GetTimesRepareException();					// 乘客业务启动默认业务异常解除次数

	//=====获取操作日志级别及名称 (操作级别代码:操作日志内容中文:操作日志内容英文:操作日志内容葡文)=========
	CString GetOperationLogLevelContent(WORD Key,int index);
	//=====获取状态显示屏运行状态=============
	CString getCNWorkMode();
	CString getENWorkMode();

	// [TVM_WEB_API]
	//CString GetWebApiMobileHost();					 // 获取 WEB_API WenHost
	CString GetWebApiHost();                         // 获取 WEB_API Host
	int     GetWebApiPort();                         // 获取 WEB_API Port
	//CString GetWebApiPwd();							 // 获取 WEB_API 密码
	CString GetWebApiLoginUrl();					 // 获取 WEB_API  登录地址
	CString GetWebApiLockUrl();						 // 获取 WEB_API  查询地址
	/*CString GetWebApiLockUrl();						 // 获取 WEB_API  锁定地址
	CString GetWebApiConfirmUrl();					 // 获取 WEB_API  提交地址
	CString GetWebApiGetOrderUrl();					 // 获取 WEB_API  取票码获取订单号地址
	CString GetWebApiTVMStateUrl();					 // 获取 WEB_API  查询TVM状态地址
	CString GetWebApiUpdateStateUrl();				 // 获取 WEB_API  修改TVM状态*/
private:
    static CFunctionInfo theInstance;
    CGetIni* m_functionIni;

	map<DWORD,bool> mapLineEnable;
	map<WORD,bool> mapStationEnable;
	map<WORD,CString> m_mapPayType;
	vector<int> m_vTicketCounts;
	vector<int> m_vTicketAmount;
	int         m_accAuthorizeManner;
	int         m_ectAuthorizeManner;

	// [TVM_WEB_API]
	//CString m_strWebMobileHost;					// 移动端Host地址
	CString m_WebApiHost;                         // WEB_API Host
	int     m_WebApiPort;                       // WEB_API Port
	//CString m_WebApiPwd;						 // WEB_API 密码
	CString m_WebApiLoginUrl;					 // WEB_API  登录地址
	//CString m_WebApiQueryUrl;					 // WEB_API  查询地址
	CString m_WebApiLockUrl;					 // WEB_API  锁定地址
	//CString m_WebApiConfirmUrl;					 // WEB_API  提交地址
	//bool	m_bWebApiUpdated;					 // WEB 地址是否更新
	//CString m_WebApiGetOrderUrl;					 // WEB_API  取票码获取订单号地址
	//CString m_WebApiTVMStateUrl;					 // WEB_API  查询TVM状态地址
	//CString m_WebApiUpdateStateUrl;					 // WEB_API  修改TVM状态地址
    // 隐藏构造函数
    CFunctionInfo();
    ~CFunctionInfo();
    CFunctionInfo(const CFunctionInfo&);
    CFunctionInfo& operator=(const CFunctionInfo&);
};