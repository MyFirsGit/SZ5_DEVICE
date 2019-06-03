#pragma once
#include <afx.h>
#include <vector>
#include <atlstr.h>
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcTVMOperationParam的唯一接口
#define theAFC_TVM_OPERATION CAfcTVMOperationParam::GetInstance()        

/**
 @brief   TVM运营参数类
*/

class CAfcTVMOperationParam :public CAfcParamMaster
{
public: 

	// TVM票卡读写参数
	typedef struct _tpu_param {
		WORD maxWriteErrorCnt;		// 车票连续出错最大张数	2	BIN	（预留）发票时车票连续读写错误的最大次数。默认为3张，0表示参数无效。达到该次数后，BOM停止出票。
		WORD unwriteWaitTime;		// 未写入完全的卡等待时间	2	BIN	（预留）写卡不完整后，提示重新刷卡的最大等待时间。单位：秒。默认值为5秒
		WORD unwriteRetryTimes;		// 未写入完全的卡重试次数	2	BIN	（预留）当BOM写卡不完整后，重新写卡的次数。
		_tpu_param(){
			maxWriteErrorCnt = 0;
			unwriteWaitTime = 0;
			unwriteRetryTimes = 0;
		}
	}TPU_PARAM;	

	// TVM维修登录相关参数
	typedef struct _maintain_param{ 
		WORD pwdInputTime;			//密码输入时间			2	BIN	（预留）单位秒，默认为60秒。密码输入操作的时限，超过该时间未输入密码，自动进入开机界面。
		WORD loginRetryTimes;		//登录尝试次数			2	BIN	（预留）操作员登录设备次数限制，默认为3次。
		WORD autoLogoffTime;		//无操作自动登出时间	2	BIN	（预留）单位秒，默认为120秒，0表示该值无效。
		WORD doorUnlockAlarmTime;	//维修门未关闭报警时间	2	BIN	（预留）单位秒，默认为60秒。无则不用

		_maintain_param(){
			pwdInputTime = 0;
			loginRetryTimes   = 0;
			autoLogoffTime = 0;
			doorUnlockAlarmTime = 0;
		}   
	} MAINTAIN_PARAM;

	typedef enum invalid_value {
		BANKNOTE_INVALID_VALUE = 255,
		COIN_INVALID_VALUE = 0
	} INVALUE_VALUE;

	typedef struct _billbox_param{ 
		BYTE acceptSalesTicketBillValue1;			//Ticket售票接收的纸币面额1	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue2;			//Ticket售票接收的纸币面额2	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue3;			//Ticket售票接收的纸币面额3	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue4;			//Ticket售票接收的纸币面额4	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue5;			//Ticket售票接收的纸币面额5	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue6;			//Ticket售票接收的纸币面额6	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue7;			//Ticket售票接收的纸币面额7	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue8;			//Ticket售票接收的纸币面额8	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue9;			//Ticket售票接收的纸币面额9	1	BIN	单位：元。无效时为255
		BYTE acceptSalesTicketBillValue10;		    //Ticket售票接收的纸币面额10	1	BIN	单位：元。无效时为255
		WORD acceptSalesTicketMaxBillCnt;			//Ticket售票接收的最大纸币数量	2	BIN	张数。传输时转换成INTEL序。
		WORD acceptSalesTicketMaxBillAmt;			//Ticket售票接收的最大纸币总额	2	BIN　	单位：元。传输时转换成INTEL序。

		BYTE acceptChrgrBillValue1;			//充值接收的纸币面额1	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue2;			//充值接收的纸币面额2	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue3;			//充值接收的纸币面额3	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue4;			//充值接收的纸币面额4	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue5;			//充值接收的纸币面额5	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue6;			//充值接收的纸币面额6	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue7;			//充值接收的纸币面额7	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue8;			//充值接收的纸币面额8	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue9;			//充值接收的纸币面额9	1	BIN	单位：元。无效时为255
		BYTE acceptChrgrBillValue10;		//充值接收的纸币面额10	1	BIN	单位：元。无效时为255
		WORD acceptChrgrMaxBillCnt;			//充值接收的最大纸币数量	2	BIN	张数。传输时转换成INTEL序。
		WORD acceptChrgrMaxBillAmt;			//充值接收的最大纸币总额	2	BIN　	单位：元。传输时转换成INTEL序。

		BYTE salesNoCoin;			        //无硬币找零时是否允许售票	1	BIN	0x00: 不允许；0x01：允许
		BYTE acceptCoinMaxCnt;			    //接受硬币的最大数量	1	BIN	
		BYTE chargeCoinMaxCnt;			    //硬币找零的最大数量	1	BIN	
		BYTE chargeBill;			        //售票是否允许纸币找零	1	BIN	0x00: 不允许；0x01：允许
		BYTE chargeBillMaxCnt;   			//纸币找零最大纸币数量	1	BIN	
		BYTE chargeBillMaxAmt;	    		//纸币找零最大纸币总金额	1	BIN	单位：元
		BYTE IsEnableAssistanceBtn;			//招援按钮启用标识		1	HEX 0：不启用；1：启用
		WORD cancelDealTime;		     	//交易取消时间	2	BIN	单位：秒。传输时转换成INTEL序。
		BYTE showWaitScreen;			    //是否允许显示待机画面	1	BIN	
		WORD changeWaitScreenTime;			//待机画面切换时间	2	BIN	单位：秒。传输时转换成INTEL序。
		WORD willFullBillCnt;		     	//纸币钱箱将满张数	2	BIN	传输时转换成INTEL序。
		WORD alreadyFullBillCnt;			//纸币钱箱已满张数	2	BIN	传输时转换成INTEL序。
		WORD coinCollectionWillFullCoinCnt;	//硬币回收箱A将满数量	2	BIN	传输时转换成INTEL序。
		WORD coinCollectionAlreadyFullCoinCnt;//硬币回收箱A已满数量	2	BIN	传输时转换成INTEL序。
		WORD coinChangeWillEmptyCoinCnt;	//硬币找零箱A将空数量	2	BIN	传输时转换成INTEL序。
		WORD minTicketCnt;			        //TVM最少车票数量	2	BIN	少于该数量时，TVM不进行售票。传输时转换成INTEL序。
		WORD willEmptyBillBoxCnt;			//纸币找零箱将空数量	2	BIN	传输时转换成INTEL序。 

		BYTE acceptSalesCoinValue1;			//售票接收的硬币面额1	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		BYTE acceptSalesCoinValue2;			//售票接收的硬币面额2	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		BYTE acceptSalesCoinValue3;			//售票接收的硬币面额3	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		BYTE acceptSalesCoinValue4;			//售票接收的硬币面额4	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP

		BYTE acceptChrgrCoinValue1;			//充值接收的硬币面额1	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		BYTE acceptChrgrCoinValue2;			//充值接收的硬币面额2	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		BYTE acceptChrgrCoinValue3;			//充值接收的硬币面额3	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		BYTE acceptChrgrCoinValue4;			//充值接收的硬币面额4	1	BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP
		WORD BankeCardPayL;                 //银行卡付款最低限额	2	Bin	默认1000

		_billbox_param()
		{
			acceptSalesTicketBillValue1 = 0;
			acceptSalesTicketBillValue2 = 0;
			acceptSalesTicketBillValue3 = 0;
			acceptSalesTicketBillValue4 = 0;
			acceptSalesTicketBillValue5 = 0;
			acceptSalesTicketBillValue6 = 0;
			acceptSalesTicketBillValue7 = 0;
			acceptSalesTicketBillValue8 = 0;
			acceptSalesTicketBillValue9 = 0;
			acceptSalesTicketBillValue10 = 0;
			acceptSalesTicketMaxBillCnt = 0;
			acceptSalesTicketMaxBillAmt = 0;

			acceptChrgrBillValue1 = 0;
			acceptChrgrBillValue2 = 0;
			acceptChrgrBillValue3 = 0;
			acceptChrgrBillValue4 = 0;
			acceptChrgrBillValue5 = 0;
			acceptChrgrBillValue6 = 0;
			acceptChrgrBillValue7 = 0;
			acceptChrgrBillValue8 = 0;
			acceptChrgrBillValue9 = 0;
			acceptChrgrBillValue10 = 0;
			acceptChrgrMaxBillCnt = 0;
			acceptChrgrMaxBillAmt = 0;

			salesNoCoin = 0;
			acceptCoinMaxCnt = 0;
			chargeCoinMaxCnt = 0;
			chargeBill = 0;
			chargeBillMaxAmt = 0;

			cancelDealTime = 0;
			showWaitScreen = 0;
			changeWaitScreenTime = 0;
			willFullBillCnt = 0;
			alreadyFullBillCnt = 0;
			coinCollectionWillFullCoinCnt=0;
			coinCollectionAlreadyFullCoinCnt=0;
			coinChangeWillEmptyCoinCnt=0;

			minTicketCnt = 0;
			willEmptyBillBoxCnt = 0;

			acceptSalesCoinValue1 = 0;
			acceptSalesCoinValue2 = 0;
			acceptSalesCoinValue3 = 0;
			acceptSalesCoinValue4 = 0;
		}
	}BILLBOX_PARAM;   

	// TVM票箱参数
	typedef struct _magazine_param{ 
		WORD TicketNearEmptyValue;		    //Ticket票箱将空张数		2	BIN	（预留）票箱将空张数阀值
		WORD TicketEmptyValue;				//Ticket票箱已空张数		2	BIN	（预留）票箱已空张数阀值
		WORD TicketNearFullValue;			//Ticket票箱将满张数		2	BIN	（预留）票箱将满张数阀值
		WORD TicketFullValue;				//Ticket票箱已满张数		2	BIN	（预留）票箱已满张数阀值
		WORD TicketInvalidNearFullValue;	//废票箱将满张数			2	BIN	（预留）废票箱将满张数阀值
		WORD TicketInvalidFullValue;		//废票箱已满张数			2	BIN	（预留）废票箱已满张数阀值
		_magazine_param(){
			TicketNearEmptyValue = 0;		    //Ticket票箱将空张数		2	BIN	（预留）票箱将空张数阀值
			TicketEmptyValue = 0;				//Ticket票箱已空张数		2	BIN	（预留）票箱已空张数阀值
			TicketNearFullValue = 0;			//Ticket票箱将满张数		2	BIN	（预留）票箱将满张数阀值
			TicketFullValue = 0;				//Ticket票箱已满张数		2	BIN	（预留）票箱已满张数阀值
			TicketInvalidNearFullValue = 0;		//废票箱将满张数	        2	BIN	（预留）废票箱将满张数阀值
			TicketInvalidFullValue = 0;			//废票箱已满张数	        2	BIN	（预留）废票箱已满张数阀值

		}   
	} MAGAZINE_PARAM;

	// TVM最近可查最小交易限制
	typedef struct _txn_lookup_param{ 
		BYTE minCount;				//可查询交易数量下限	1	HEX	（预留）可查最小交易数量。默认30条。
		BYTE minTime;				//可查询交易时间下限	1	HEX	（预留）单位：分钟。默认10分钟
		BYTE logSaveDay;			//日志保留天数			1	BIN	单位：天。默认：10天。
		BYTE maxOffLineDay;			//最大离线天数			1	BIN 单位：天。默认：7天。
		BYTE expiredTransKeepDay;	//已上传交易保留天数	1	BIN 单位：天。默认：10天。	
		_txn_lookup_param(){
			minCount = 0;
			minTime = 0;
			logSaveDay = 0;
			maxOffLineDay = 0;
			expiredTransKeepDay = 0;
		}
	} TXN_LOOKUP_PARAM;

	// TVM 功能设置参数 
	typedef struct _function_param{ 
		BYTE MaxOfflineBusinessTime;	  //最大连续不离线服务时间	1	BIN	单位：天，默认七天
		BYTE PrintSwitch;				  //打印按钮开关	1	BIN	是否允许乘客端显示打印按钮
		BYTE QueryFunctionSwitch;		  //查询功能开关	1	BIN	是否允许查询功能
		WORD VersionPic;                  //待机界面图片版本	    2	BIN
		CString FilePathPic;              //待机界面图片路径	40	ASCII	
		_function_param(){
			MaxOfflineBusinessTime = 0;			 
			PrintSwitch = 0;
			QueryFunctionSwitch = 0;			 
			VersionPic = 0;
			FilePathPic = "";
		}   
	} FUNCTION_PARAM;

	// 磁盘空间报警参数
	typedef enum _disk_type_{
		DISK_TYPE_SYSTEM = 1,							// 系统盘
		DISK_TYPE_DATA,									// 数据盘
		DISK_TYPE_LOG,									// 日志盘
	}DISK_TYPE;

	typedef struct _disk_warn_setting_{
		DISK_TYPE diskType;								// BIN	1	磁盘类别ID	1：系统盘；2：数据盘；3：日志盘。
		WORD	  wDiskNearFullPersent;					// BIN	2	磁盘将满报警阀值（百分比）
		WORD	  wDiskNearFullValue;					// BIN	2	磁盘将满报警阀值（容量）单位：MB
		WORD	  wDiskFullPersent;						// BIN	2	磁盘已满故障阀值（百分比）
		WORD	  wDiskFullValue;						// BIN	2	磁盘已满故障阀值（容量）单位：MB

		_disk_warn_setting_(){
			diskType = DISK_TYPE_SYSTEM;
			wDiskNearFullPersent	= 0;
			wDiskNearFullValue		= 0;
			wDiskFullPersent		= 0;
			wDiskFullValue			= 0;
		}
	}DISK_WARN_SETTING;

	// 共有接口
    void GetTpuParam(TPU_PARAM&);						// 获得TVM票卡读写参数
	void GetMaintainParam(MAINTAIN_PARAM&);				// 获得TVM维修登录相关参数
	void GetMagazineParam(MAGAZINE_PARAM&);				// 获得TVM票箱参数
	void GetBillboxParam(BILLBOX_PARAM&);				// 获得TVM钱箱参数
	void GetTxnLookupParam(TXN_LOOKUP_PARAM&);			// 获得TVM最近可查最小交易限制
	WORD GetAutoLogoffTime();							// 获得自动登出等待时间参数
	WORD GetPwdInputTime();                             // 获得密码输入时间参数
	WORD GetPwdInputMaxTimes();							// 获取密码登入尝试次数
	WORD GetDoorUnlockAlarmTime();                      // 维修门未关闭报警时间
	const vector<BYTE>& GetAllPaymentID();				// （已废方便调试）
	DWORD GetMaxSellValue();							//（已废方便调试）
	BYTE GetCombinePayFlag();							// （已废方便调试）获得合并支付启用标志 1：true  0：false  启用时，允许购买双产品并合并支付

	bool SupportSaleWithoutCoinChange();				// 没有硬币找零时是否允许售票
	bool SupportBHChangeWithSale();						// 售票时是否允许纸币找零
	WORD GetBHChangeMaxPiece();							// 纸币找零最大张数
	WORD GetBHChangeMaxAmount();						// 纸币找零最大额度
	bool IsEnableAssistanceBtn();						// 招援按钮是否启用
	// 日志保留天数
	WORD GetMaxLogSaveDays();							// 取日志保留天数（单位：天）
	// 脱机运营最大天数
	WORD GetOfflineWorkDays();							// 获取脱机运营最大天数（单位：天）
	// 已上传交易保留天数
	WORD GetSendedMsgSaveDays();						// 获取已上传交易保留天数（单位：天）

	// 显示待机画面？
	bool IsReadyToRestUsed();										// 待机中到等待中是否使用

	// 纸币接口可以合成一种，同时只会使用一个
	void GetAcceptBanknoteFaceOfIssueCard(vector<WORD>& vecFace);	// 获取售Card时可接收的纸币面值（以MOP为单位）
	WORD GetAcceptBanknoteMaxPieceOfIssueCard();					// 获取售Card时可接收的纸币最大张数
	WORD GetAcceptBanknoteMaxAmoutOfIssueCard();					// 获取售Card时可接收的纸币最大总额
	void GetAcceptCoinFaceOfIssue(vector<WORD>& vecFace);			// 获取售票时可接收的硬币面值（以AVOS为单位）
	WORD GetAcceptCoinMaxPieceOfIssue();							// 获取售票时可接收的硬币最大枚数
	WORD GetChageCoinMaxPieceOfIssue();								// 获取售票时可找零的硬币最大枚数

	// 充值
	void GetAcceptBanknotesFaceOfRecharge(vector<WORD>& vecFace);	// 获取充值时可接收的纸币的面值
	WORD GetAcceptBanknotesMaxPieceOfRecharge();					// 获取充值时可接收的纸币的最大数量
	WORD GetAcceptBanknotesMaxAmountOfRecharge();					// 获取充值时可接收的纸币最大总额
	void GetAcceptCoinFaceOfRecharge(vector<WORD>& vecFace);		// 获取充值时可接收的硬币的面值
	WORD GetAcceptCoinMaxPieceOfRecharge();							// 获取充值时可接收的硬币的最大数量
	WORD GetChageCoinMaxPieceOfRecharge();							// 获取充值时可找零的硬币的最大枚数

	WORD ConvertCoinFace(BYTE code);								// 将参数中的硬币面值代码转为以分为单位的面值
	bool SupportCoinFaceOfIssue(WORD face);							// 是否支持指定面值的硬币（以AVOS为单位）

	// 功能设置
	bool SupportPrintSwitch();										// 是否显示打印按钮

	// 硬币枚数状态
	WORD GetChangeCoinBoxNearEmptyCount();								// 获取找零箱A 将空枚数
	WORD GetRecycleBoxNearFullCount();									// 获取回收箱A 将满枚数
	WORD GetRecycleBoxFullCount();										// 获取回收箱A 已满枚数

	// 纸币数量状态
	WORD GetBanknotesNearFullCount();										// 获取纸币箱将满数量
	WORD GetBanknotesFullConut();											// 获取纸币箱已满数量
	WORD GetBanknoteChangeNearEmptyCount();									// 纸币找零箱将空数量

	// 票箱数量状态
	// Ticket 箱
	WORD GetTicketNearEmptyCount();											// 获取Ticket将空张数
	WORD GetTicketEmptyCount();												// 获取Ticket已空张数
	// Ticket 废票箱
	WORD GetTicketInvalidNearFullCount();									// 获取Ticket废票箱将满张数
	WORD GetTicketInvalidFullCount();										// 获取Ticket废票箱已满张数

	WORD GetCancelDealTime();												// 获取取消交易时间
	WORD GetReadyToRestTime();												// 获取等待中到待机中时间

	// 磁盘报警配置
	WORD GetSystemDiskNearFullPersent();									// 获取系统盘将满阀值百分比
	WORD GetSystemDiskNearFullValue();										// 获取系统盘将满阀值（MB）
	WORD GetSystemDiskFullPersent();										// 获取系统盘已满故障值百分比
	WORD GetSystemDiskFullValue();											// 获取系统盘已满故障值（MB）

	WORD GetDataDiskNearFullPersent();										// 获取数据盘将满阀值百分比
	WORD GetDataDiskNearFullValue();										// 获取数据盘将满阀值（MB）
	WORD GetDataDiskFullPersent();											// 获取数据盘已满故障值百分比
	WORD GetDataDiskFullValue();											// 获取数据盘已满故障值（MB）

	WORD GetLogDiskNearFullPersent();										// 获取日志盘将满阀值百分比
	WORD GetLogDiskNearFullValue();											// 获取日志盘将满阀值（MB）
	WORD GetLogDiskFullPersent();											// 获取日志盘已满故障值百分比
	WORD GetLogDiskFullValue();												// 获取日志盘已满故障值（MB）

    static CAfcTVMOperationParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[6];					// 分段信息

    // 私有成员变量
    TPU_PARAM				m_tpu_param;			// TVM票卡读写参数
	MAINTAIN_PARAM			m_maintain_param;		// TVM维修登录相关参数
	BILLBOX_PARAM			m_billbox_param;        // TVM钱箱参数
	MAGAZINE_PARAM			m_magazine_param;		// TVM票箱参数
	TXN_LOOKUP_PARAM		m_txn_lookup_param;		// TVM最近可查最小交易限制
	FUNCTION_PARAM			m_function_param;       // TVM 功能设置参数	
	vector<DISK_WARN_SETTING>	m_vecDiskWaringSet; // TVM磁盘空间报警参数

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAfcTVMOperationParam(void);
    ~CAfcTVMOperationParam();
	CAfcTVMOperationParam(const CAfcTVMOperationParam&);
	CAfcTVMOperationParam& operator=(const CAfcTVMOperationParam&);

    static  CAfcTVMOperationParam theInstance;
};
