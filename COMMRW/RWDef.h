#pragma once

#include "CommonDefine.h"
#include "RWException.h"

//typedef enum _rw_rsp_code {
//   RW_RSP_OK               = 0x00,         // 请求成功完成
//   // DLL自定义相关
//   RW_RSP_UNKNOWN          = 0xEF,         // 未知错误
//   RW_RSP_OVERTIME         = 0xEE,         // 通信超时错误
//   RW_RSP_USERCANCEL       = 0xED,         // 用户取消
//   RW_RSP_BADRSPCOMMAND    = 0xEC,         // 应答中的命令码与请求码不一致
//   RW_RSP_WRITECOMERROR    = 0xEB,         // 写串口失败
//// 设备类/通信类相关
//   RW_RSP_CHECKERROR       = 0x01,         // 校验和错误
//   RW_RSP_BADCOMMAND       = 0x02,         // 错误的请求
//   RW_RSP_BADPARAM         = 0x03,         // 输入参数错
//   RW_RSP_STATUSERROR      = 0x04,         // 读写器状态错
//   RW_RSP_BURSTERROR       = 0x26,         // 读写器升级异常
//   RW_RSP_READERERROR      = 0x70,         // 读写器故障

//   // 卡片类相关
//   RW_RSP_AUTHENERROR      = 0x10,         // 认证失败
//   RW_RSP_READERROR        = 0x11,         // 读卡故障
//   RW_RSP_WRITEERROR       = 0x12,         // 写卡故障	
//   RW_RSP_CARDREMOVED      = 0x13,         // 无卡或操作过程中卡移动
//   RW_RSP_NOCARD           = 0x14,         // 超时未寻到卡
//   RW_RSP_INVALIDFLAG      = 0x15,         // 错误的进出站标识
//   RW_RSP_NORECORD         = 0x16,         // 无交易记录
//   RW_RSP_ENOUGH           = 0x17,         // 卡内余额不足
//   RW_RSP_LOCKERROR        = 0x19,         // 锁卡失败
//   RW_RSP_DATADAMAGED      = 0x1F,         // 卡数据被破坏
//RW_RSP_BLACKCARD        = 0x20,			// 黑名单卡
//RW_RSP_NOUSE            = 0x21,         // 卡片未启用
//   RW_RSP_SECTAUTHERR      = 0x22,         // 扇区认证错误
//   RW_RSP_MAC1ERR          = 0x23,         // MAC1认证错误
//   RW_RSP_MAC2ERR          = 0x24,         // MAC2认证错误
//   RW_RSP_INVALID          = 0x25,         // 无效卡
//RW_RSP_PURSE_ERROR      = 0x27,			// 钱包异常
//   RW_RSP_PINERROR         = 0x61,         // 操作员PIN错误
//   RW_RSP_ANTICOLL         = 0x62,         // 在感应区域内同时出现多卡
//   RW_RSP_ERRCARD          = 0xF8,         // 指定卡座没有卡片或者卡片错误
//   // SAM卡类相关
//   RW_RSP_SAMERR           = 0xB0,         // SAM卡故障
//   RW_RSP_BMACPSAMERR      = 0xB1,         // 一卡通PSAM卡认证故障
//   RW_RSP_BMACISAMERR      = 0xB2,         // 一卡通ISAM卡认证故障
//   RW_RSP_ACCPSAMERR       = 0xB3,         // ACC PSAM卡认证故障
//   RW_RSP_ACCISAMERR       = 0xB4,         // ACC ISAM卡认证故障
//// DLL类相关
//RW_RSP_DLLCMDERROR		= 0xFC,			// 外部IC读写器命令异常
//RW_RSP_DLLERROR			= 0xFD,			// 外部IC读写器DLL异常
//RW_RSP_ABORTERROR		= 0xFE,			// 外部IC读写器超时（Abort）
//RW_RSP_CHARGEERROR      = 0xFB,			// 充值失败
//};

//typedef enum _rw_err_detail {
//    //RW_SET_PARAM_ERR            = 1,	///< 设定值：参数异常
//    //RW_FUNC_PARAM_ERR           = 2,	///< 函数参数异常
//    //RW_SEQUENCE_ERR	            = 3,	///< 函数调用顺序异常
//    //RW_FUNC_ERR_RETURN	        = 4,	///< 函数返回异常
//    //RW_LOGIC_ERR	            = 5,	///< 逻辑异常
//    //RW_OTHER_ERR	            = 6,	///< 异常发生
//
//    RW_ERR_COM_OPEN             = 3001, ///< 串口打开失败
//    //RW_ERR_BUFF                 = 3002, ///< 参数数据错误 
//    RW_ERR_CARD_CHANGED         = 3003, // 写卡前发现卡不同
//}RW_ERR_DETAIL;
//
//typedef enum _rw_first_in_flag {
//    RW_IS_FIRST_IN              = 0x01,     // 是首次进站
//    RW_NOT_FIRST_IN             = 0x00      // 不是首次进站
//}RW_FIRST_IN_FLAG;
//

// 读写器程序文件名
static const TCHAR* const MID_STR_BOM_RW_PROG_APP   = _T("*4109*"); // RW应用程序
static const TCHAR* const MID_STR_BOM_RW_PROG_MAIN  = _T("*4110*"); // RW主控程序

// RW打开命令结构
typedef struct _rw_cmd_open
{
	BYTE port;                                  // 串口号
	DWORD baud;                                 // 通讯速率
	BOOL  isUsb;								// 是否使用USB接口
	_rw_cmd_open()
	{
		port = 0;
		baud = 0;
		isUsb = FALSE;
	}
} RW_CMD_OPEN, *LPRW_CMD_OPEN;

// 程序下载输入参数
typedef struct _rw_cmd_prog_firmware
{
	LPBYTE      fileData;                   // Firmware数据,从更新文件中读取
	int         size;                       // 文件的大小  
	bool        isMain;                     // 是否为基本部更新，true:Main, false:Exec

	_rw_cmd_prog_firmware(){
		fileData = NULL;
		size     = 0;
		isMain   = false;
	}
} RW_CMD_FIRMWARE,*LPRW_CMD_FIRMWARE;

// 参数下载输入参数
typedef struct _rw_cmd_param_firmware
{
	int downloadRetryCnt;			// 下载失败重试次数 
	LPBYTE      fileData;              // 参数数据,从参数文件中读取
	int         size;							// 文件的大小  
	WORD		paramId;				// 参数ID
	_rw_cmd_param_firmware(){
		downloadRetryCnt = 3;
		fileData = NULL;
		size     = 0;
		paramId = 0;
	}
} RW_PARAM_DOWNLOAD,*LPRW_PARAM_DOWNLOAD;

// 设置运营模式输入参数
typedef struct _rw_cmd_set_mode
{
	BYTE ControlCode;  //控制代码:0x01:设置模式;0x02:解除模式
	WORD ModeCode;	   //模式代码，长度为2个字节
	_rw_cmd_set_mode(){
		ControlCode = 0;
		ModeCode    = 0;
	}
} RW_CMD_SET_MODE,*LPRW_CMD_SET_MODE;

// 设置运营模式输入参数
typedef struct _rw_cmd_set_time
{
	BYTE ControlCode;  //控制代码:0x01：启动24小时运营；0x02：解除24小时运营；0x10：启动延长运营；
	ulong Time;	       //延长时间
	_rw_cmd_set_time(){
		ControlCode = 0;
		Time    = 0;
	}
} RW_CMD_SET_TIME,*LPRW_CMD_SET_TIME;

// RW初始化输入参数结构
typedef struct _rw_cmd_init {  
	DWORD ReadTimeout;				// 自动ABORT延时时间,单位ms,表示上位机发出读卡指令后，如果在X毫秒内没有检测到卡片，读卡器将自动返回到空闲状态
	DWORD DeviceId;					// 设备ID
	WORD DeviceType;				// 设备类型HardwareType_t
	DWORD StationId;				// 车站位置信息 LocationNumber_t
	DWORD AgentCode;				// 运营公司或参与方 ParticipantId_t
	_DATE OperationDate;			// 运营日期 YYYYMMDD
	_DATE CurrentDate;				// 当前日历日期 YYYYMMDD
	BYTE TestMode;					// 设备测试模式
	WORD OperationMode;				// 车站运营模式 参见EventId_t.
	DWORD OperaterID;				// 操作员的编号,  3	BCD	
	BYTE AntennaConfig;			    // 天线配置 0x00：单天线；0x01：双天线且天线A优先寻卡；0x02：双天线且天线B优先寻卡
	BYTE ChangeStationSign;
	BYTE ReaderPosition;
	WORD ErrFareCtrMax;//连续出错最大张数
	WORD WaitTimeForCardWR;//
	WORD RetryTimesForCardWR;
	BYTE YPTonlinePara[50];
	BYTE YKTonlinePara[50];

	_rw_cmd_init(){
		ReadTimeout = 5000;	
		DeviceId = 0;
		//memset(DeviceId,0,4);
		DeviceType = 0;
		StationId = 0;
		TestMode = 0;
		OperationMode = 0;
		OperaterID = 0;	
		AntennaConfig = 0;
		AgentCode = 0;
		ChangeStationSign = 0;
		ReaderPosition = 0;
		ErrFareCtrMax = 0;
		WaitTimeForCardWR = 0;
		RetryTimesForCardWR = 0;
		memset(YKTonlinePara,0x00,sizeof(YKTonlinePara));
		memset(YPTonlinePara,0x00,sizeof(YPTonlinePara));
	}
} RW_CMD_INIT, *LPRW_CMD_INIT;

// RW获取票价命令结构
typedef struct _rw_cmd_get_price
{
	BYTE CardType;          // 卡片的种类
	BYTE ProductCategory;	// 车票产品的种类
	WORD ProductType;		// 产品类型
	WORD ProductSubType;    // 车票查品的子类型（预留）
	BYTE PriceType;         // 要查询的票价类型：0x01：表示按消费票价表查询票价；0x02：表示按销售票价表查询票价；（预留） 
	BYTE PassengerType;
	_DATE_TIME Time;		// 指定查询票价时日历表使用的时间
	BYTE BegainStation[4];
	BYTE EndStation[4];	

	_rw_cmd_get_price()
	{
		CardType = 0;
		ProductCategory = 0;
		ProductType = 0;
		ProductSubType = 0;
		PriceType = 0;
		PassengerType = 0;
		memset(BegainStation,0,sizeof(BegainStation));
		memset(EndStation,0,sizeof(EndStation));
	}
} RW_CMD_TICKET_PRICE, *LPRW_CMD_TICKET_PRICE;

// RW获取票价命令反馈结构
typedef struct _rw_cmd_get_price_res
{
	DWORD TicketPrice;		// 票价
	int DetailErrorCode;			//详细错误代码

	_rw_cmd_get_price_res()
	{
		TicketPrice = 0;
		DetailErrorCode = 0;
	}
} RW_CMD_TICKET_PRICE_RES, *LPRW_CMD_TICKET_PRICE_RES;

//获取税费命令结构
typedef struct _rw_cmd_get_tax 
{
	uchar CardType;			//0x01：澳门通M1；0x02：澳门通CPU卡片卡；0x11：一票通M1卡片；0x12：一票通UL卡
	ulong SaleOrAddAmount;	//售票或充值金额

	_rw_cmd_get_tax()
	{
		CardType = 0;
		SaleOrAddAmount = 0;
	}
} RW_CMD_TICKET_TAX;

// RW获取税费命令反馈结构
typedef struct _rw_cmd_get_tax_res
{
	DWORD TicketTax;		// 税费
	int DetailErrorCode;	//详细错误代码

	_rw_cmd_get_tax_res()
	{
		TicketTax = 0;
		DetailErrorCode = 0;
	}
} RW_CMD_TICKET_TAX_RES, *LPRW_CMD_TICKET_TAX_RES;



// RW读卡请求参数
typedef struct _rw_read_card_request
{
	int readCardInterval;		// 读卡间隔时间(单位：毫秒)
	bool IsCheck;				// 合法性检查标志	true:检查；false:不检查
	BYTE FunctionCode;			// 业务功能代码		0x01：进站；0x02：出站；0x03：售卡；0x04：补票；0x05：充值；0x06：退卡；0x07：替换旧卡；0x08：替换新卡；0x09：激活；0x10：延期； 0x11：自动充值申请；0x12：抵消；0x13：解挂； 0x14：个性化；0x15：个性化信息更新旧卡；0x16：个性化信息更新新卡；0x1F：票卡初始化；0x2f：票卡发行；0x3f：重编码；0xFF：其他
	bool IsPaidArea;			// 付费区标志		true:付费区;false：非付费区。
	bool IsReadRecords;			// 交易记录读取标志	true:读取;false:不读取
	BYTE PollCardMethod;		// 天线寻卡方式		0x01:双天线同时寻卡；0x02：只有A天线寻卡；0x03：只有B天线寻卡
	_rw_read_card_request()
	{
		readCardInterval = 100;
		IsCheck = true;
		FunctionCode = 0xFF;			
		IsPaidArea = false;	
		IsReadRecords = false;
		PollCardMethod = 0;
	}
}RW_READ_CARD_REQUEST, *LPRW_READ_CARD_REQUEST;

// RW售卡命令结构
typedef struct _rw_issue_request
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE CardType;				// 卡片种类 0x11：一票通M1卡片；0x12：一票通UL卡
	BYTE ProductyCategory;		// 车票产品类型(计次，定期，钱包)
	WORD ProductType;			// 车票产品种类
	WORD ProductSubType;		// 车票产品子类型
	BYTE SaleMode;				// 售票方式
	BYTE PassengerType;         // 乘客类型
	DWORD SaleAmount;			// 售票金额
	DWORD BegainStation;		// 起点站站码	
	DWORD EndStation;			// 终点站站码
	WORD MultiRideTicketCount;  // 计次票使用次数
	WORD ValidityPeriod;        // 有效期的时间长度
	BYTE PaymentMethod;			// 支付方式
	BYTE BankCardNum[10];	    // 银行卡号
	BYTE TicketNum[8];			// 支付卡号

	_rw_issue_request()
	{
		Initialize();
	}
	void Initialize()
	{
		SerialNo = 0;			
		CardType = 0;				
		ProductyCategory = 0;	
		ProductType = 0;			
		ProductSubType = 0;		
		SaleMode = 0;			
		PassengerType = 0;       
		SaleAmount = 0;			
		BegainStation = 0;			
		EndStation = 0;			
		MultiRideTicketCount = 0;  
		ValidityPeriod = 0;
		PaymentMethod = 0;	
		memset(BankCardNum,0,10);
		memset(TicketNum,0,8);
	}
}RW_ISSUE_REQUEST, *LPRW_ISSUE_REQUEST;

// RW补票命令结构
typedef struct _rw_cmd_adjust
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE CardType;				// 卡片种类 0x01:一卡通S50/S70卡片;0x02:一卡通CPU卡;0x11:一票通S50/S70卡片;0x12:一票通UL卡；0x13：一票通CPU卡；
	WORD ProductType;			// 车票产品类型
	BYTE AdjustMethod;			// 补票付款方式 0x01:非卡内扣款补票;0x02:卡内扣款补票
	DWORD AdjustAmount;			// 补票金额   	
	BYTE AdjustArea;			// 补票区域 0x01:付费区补票;0x02:非付费区补票
	BYTE AdjustOperType;		// 补票操作类型 0x01:按进站选择（未刷进站）;0x02:按金额选择（余额不足，包括超时、超程、补票超时）;0x03:选择终点站，输入手续费金额（未刷出站）
	DWORD BegainStation;			// 起点站站码	
	DWORD EndStation;			// 终点站站码
	BYTE PaymentMethod;			// 支付方式
	BYTE BankCardNum[10];	    // 银行卡号
	_rw_cmd_adjust()
	{
		SerialNo = 0;
		CardType = 0;
		ProductType = 0;
		AdjustMethod = 0;
		PaymentMethod = 0;
		AdjustAmount = 0;
		AdjustArea = 0;
		AdjustOperType = 0;
		BegainStation = 0;			
		EndStation = 0;	
		memset(BankCardNum,0,10);
	}
} RW_CMD_ADJUST,*LPRW_CMD_ADJUST;

// RW充值命令结构
typedef struct _rw_cmd_charge
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE CardType;				// 卡类型
	BYTE ChargeType;			// 充值类型
	WORD ChargeProductType;     // 充值产品类型
	WORD ChargeSubProductType;	// 充值子产品类型
	DWORD ChargeAmount;			// 充值金额
	DWORD ChargeCount;			// 充值次数(计次卡用)
	BYTE PaymentMethod;			// 支付方式
	BYTE BankCardNum[10];	    // 银行卡号
	_rw_cmd_charge()
	{
		SerialNo = 0;
		PaymentMethod = 0;
		CardType = 0;
		ChargeType = 0;
		ChargeProductType = 0;
		ChargeSubProductType = 0;
		ChargeAmount = 0;
		ChargeCount = 0;
		memset(BankCardNum,0,10);
	}
} RW_CMD_CHARGE,*LPRW_CMD_CHARGE;

// RW充值超时命令结构
typedef struct _rw_cmd_chargeovertime 
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE byDo;			        // 是否时进行超时处理
	_rw_cmd_chargeovertime()
	{
		SerialNo = 0;		
		byDo = 0;
	}
} RW_CMD_CHARGEOVERTIME,*LPRW_CMD_CHARGEOVERTIME;

//  RW售票初检命令结构
typedef struct _rw_cmd_setcheck 
{
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE byDo;			        // 是否时进行初检处理
	_rw_cmd_setcheck()
	{
		byDo = 0;
	}
} RW_CMD_SETCHECK,*LPRW_CMD_SETCHECK;

//  RW售票初检命令回复结构
typedef struct _rw_resp_cmd_setcheck 
{
	byte byTpuState;            // 返回tpu的状态码
	int DetailErrCode;          // 返回tpu的错误码
	_rw_resp_cmd_setcheck()
	{
		byTpuState = 0;
		DetailErrCode =0;
	}
} RW_CMD_RESP_SETCHECK,*LPRW_CMD_RESP_SETCHECK;

typedef struct _SURRENDERPRODUCT
{
	ushort ProductType;//车票产品种类
	ushort ProductSubType;//车票产品子类型
	uchar SurrenderReasonCode;
	ulong RefundMoney;
	_SURRENDERPRODUCT()
	{
		ProductType=0;
		ProductSubType=0;
		SurrenderReasonCode=0;
		RefundMoney=0;
	}
}SURRENDERPRODUCT;

// RW退款命令结构
typedef struct _rw_cmd_refund
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE CardPHType;
	BYTE CardAppType;
	BYTE ReasonCode;
	DWORD RefundAmount;
	DWORD DepositAmount;
	BYTE ReceptionNum[10];
	_rw_cmd_refund()
	{
		SerialNo = 0;
		CardPHType = 0;
		CardAppType = 0;
		ReasonCode = 0;
		RefundAmount = 0;
		DepositAmount = 0;
		memset(ReceptionNum,0,10);
	}
}RW_CMD_REFUND,*LPRW_CMD_REFUND;

// RW锁卡命令结构
typedef struct _rw_cmd_block
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	_rw_cmd_block()
	{
		SerialNo = 0;	
	}
}RW_CMD_BLOCK,*LPRW_CMD_BLOCK;

// RW抵消命令结构
typedef struct _rw_cmd_counteract
{
	DWORD SerialNo;				// 交易流水号
	_DATE_TIME TranDateTime;	// 交易时间
	BYTE CardType;	  		    // 卡片种类，0x01:一卡通S50/S70卡片；0x02:一卡通CPU卡；0x11:一票通S50/S70卡片；0x12:一票通UL卡
	WORD ProductyType;			// 产品种类
	WORD ReasonCode;			// 抵消原因代码
	_rw_cmd_counteract()
	{
		SerialNo = 0;	
		CardType = 0;
		ProductyType = 0;
		ReasonCode = 0;
	}
}RW_CMD_COUNTERACT,*LPRW_CMD_COUNTERACT;

// RW EP支付命令结构
typedef struct _rw_cmd_consume{
	DWORD		SerialNo;		// 交易流水号
	_DATE_TIME	TranDateTime;	// 交易时间
	DWORD		ConsumeAmount;  // 交易金额
	_rw_cmd_consume(){
		SerialNo = 0;
		ConsumeAmount = 0;
	}
}RW_CMD_CONSUME;

//////////////////////////////////////////////以下为读写器反馈数据结构定义/////////////////////////////////////////
// RW返回状态定义
typedef BYTE RW_RSP_STATUS; 

// RW获取TPU信息命令应答内容结构
typedef struct _rw_rsp_tpu_info
{
	BYTE DeviceCode[4];
	BYTE hardwareVersion[2];                               // 硬件版本
	BYTE softwareVersionApp[6];                            // 应用部软件版本
	BYTE softwareVersionMain[4];                           // 基本部软件版本
	BYTE accPSAMid[6];
	BYTE accISAMid[6];
	BYTE bmacPSAMid[6];
	BYTE bmacISAMid[6];

	_rw_rsp_tpu_info()
	{
		memset(DeviceCode,0,6);
		memset(hardwareVersion,0,2);
		memset(softwareVersionApp,0,6);
		memset(softwareVersionMain,0,4);
		memset(accPSAMid,0,6);
		memset(accISAMid,0,6);
		memset(bmacPSAMid,0,6);
		memset(bmacISAMid,0,6);
	}
} RW_RSP_TPU_INFO, *LPRW_RSP_TPU_INFO;

// 获取参数版本反馈结构
typedef struct _rw_rsp_param_info
{
	WORD			ParaCount;                  // 参数数量
	DAT::PARAINFO	ParamData[25];                  // 参数数据指针，读写器返回

	_rw_rsp_param_info(){
		ParaCount = 0;
		memset(ParamData,0,sizeof(ParamData));
	}
} RW_RSP_PARAM_INFO,*LPRW_RSP_PARAM_INFO;

// RW读卡反馈产品信息结构
typedef struct _RECHARGEPRODUCT
{
	WORD ProductType;//产品类型
	WORD ProductSubType;//产品子类型
	ulong RefoundMoney;//可退金额
	bool RechargeMark;//是否可充值   false：不可充值；true：可充值
}RECHARGEPRODUCT;

// RW读卡反馈信息
typedef struct _rw_read_card_response
{
	int DetailErrorCode;			//详细错误代码
	BYTE CardPHType;				//车票物理类型 0:预留	1:Ultralight  2:Mifare1K  3:CPU票卡
	WORD CardInfoLen;				//卡片信息的长度
	BYTE PaidAreaAdjustCode;		//付费区补票建议代码
	BYTE UnPaidAreaAdjustCode;		//非服费区补票建议代码
	DWORD AdjustAmount;				//建议补票金额

	BYTE ProductCount;//产品数量
	RECHARGEPRODUCT RechargeProduct[10];
	BYTE SalableProductType;        //可售产品类型
	//按BIT位解析，从低到高位依次代表钱包，计次，定期
	//钱包：00000001 =0x01
	//计次：00000010 = 0x02
	//钱包+计次 = 00000011 = 0x03
	//定期：00000100 = 0x04
	//钱包+定期 = 00000101 = 0x05
	//钱包+计次+定期 = 00000111 = 0x07

	BYTE SalableProductCount;       //可售产品数量
	BYTE AdjustPaymentMethod;		//补票时支付方式
	DWORD ReturnCardFee;            //建议退票手续费金额
	bool  isBigMoneyCard;			//是否为大额卡 true:是 false:否

	BYTE CardInfo[RW_MSG_CARD_INFO_LEN];	//卡信息,根据不同卡类型返回不同卡信息
	BYTE LogCount;							//交易记录数量
	BYTE LogInfo[256];		//交易记录数据



	_rw_read_card_response()
	{
		DetailErrorCode = 0;
		CardPHType = 0;
		CardInfoLen = 0;
		PaidAreaAdjustCode = 0;	
		UnPaidAreaAdjustCode = 0;	
		ProductCount = 0;
		AdjustAmount = 0;		
		ReturnCardFee = 0;			
		isBigMoneyCard = false;	
		memset(RechargeProduct,0,sizeof(RECHARGEPRODUCT)*10); 
		SalableProductType = 0;
		SalableProductCount = 0;
		memset(CardInfo,0,RW_MSG_CARD_INFO_LEN); 
		LogCount = 0;
		memset(LogInfo,0,256);
	}
} RW_READ_CARD_RESPONSE, *LPRW_READ_CARD_RESPONSE;



// RW业务反馈通用结构
typedef struct _rw_bussiness_response
{
	int  DetailErrorCode;			// 详细错误代码
	BYTE SerialAddnum;			    // UDSN或一卡通交易流水号累加值
	DAT::TRANRESULT TranResult;		// TPU业务处理后返回的结果数据(同售票)
	WORD UDlen;						// UD数据长度 长度为0表示下面的UD数据不存在
	BYTE UDData[BUF_SIZE];			// UD数据
	WORD ARLen;						// AR数据长度 长度为0表示下面的AR数据不存在
	DAT::ARDATA ARData;				// AR数据
	_rw_bussiness_response()
	{
		SerialAddnum = 0;	
		UDlen = 0;					
		memset(UDData,0,sizeof(UDData));	
		ARData.ARnum = 0;	
		memset(ARData.ARdata,0,sizeof(ARData.ARdata));
	}
} RW_BUSSINESS_RESPONSE, *LPRW_BUSSINESS_RESPONSE;

// RW激活数据结构体
typedef struct _rw_cmd_active
{
	_DATE_TIME TranDateTime;            // 交易发生的日期时间，BCD码，格式YYMMDDHHmm
	uchar Category;                 // 卡片种类，描述卡片的应用大类别：0x01：一卡通S50/S70卡片；0x02：一卡通CPU卡；0x11：一票通S50/S70卡片；0x12：一票通UL卡
	ushort ProductyCategory;        // 车票产品类型	
	_rw_cmd_active()
	{
		Category = 0;
		ProductyCategory = 0;	
	}
} RW_CMD_ACTIVE, *LPRW_CMD_ACTIVE;

// RW延期数据结构体
typedef struct _rw_cmd_defer
{
	DWORD SerialNo;						// 交易流水号
	_DATE_TIME TranDateTime;            // 交易发生的日期时间，BCD码，格式YYMMDDHHmm
	DWORD ServiceFee;					// 手续费
	WORD DeferTime;						// 有效期顺延的长度，以天为单位
	_rw_cmd_defer()
	{
		SerialNo = 0;
		ServiceFee = 0;
		DeferTime = 0;
	}
} RW_CMD_DEFER, *LPRW_CMD_DEFER;

//  RW替换数据结构体
typedef struct _rw_cmd_replace_card
{

	_DATE_TIME TranDateTime;             
	DWORD Serialnum;			    // UDSN或一卡通交易流水号	
	_rw_cmd_replace_card()
	{
		Serialnum = 0;	
	}
} RW_CMD_REPLACE_CARD, *LPRW_CMD_REPLACE_CARD;

//  RW无卡/坏卡替换数据结构体
typedef struct _rw_cmd_no_card_replace
{
	typedef struct _ProductDetailInfo
	{
		WORD ProductType;
		WORD ProductSubType;
		BYTE ProductCategory;
		_DATE_TIME BeginTime;
		BYTE TypeOfExpiryDate;
		WORD DurationOfExpiryDate;
		BYTE ActivedFlag;
		BYTE ProductStatus;
		BYTE ProductIssuerId;
		DWORD Balance;
		WORD TransactionSerialNo;
		WORD BeginSection;
		WORD EndSection;
		DWORD SamId;
		WORD CountNumber;
		BYTE PriorityOfUse;
	}ProductDetailInfo;

	typedef struct _ProductInfo
	{
		BYTE ProductCount;
		ProductDetailInfo m_ProductDetailInfo[3];
		WORD CompanyId;						
		DWORD StaffId;
		char PassengerName[8];//???
		BYTE CertificateType;
		BYTE CertificateNum[9];
		_DATE PassengerBirth;
		BYTE TravelStastus;
		BYTE LastUseProduct;
		WORD EntryStation;
		_DATE_TIME EntryTime;
		WORD LastUseStation;
		_DATE_TIME LastUseTime;
	}ProductInfo;

	DWORD Serialnum;					// 
	_DATE_TIME TranDateTime;
	ProductInfo transData;
}RW_CMD_NO_CARD_REPLACE;

// RW记名卡信息修改命令
typedef struct _rw_cmd_update_signcard_info
{
	DWORD SerialNo;						// 交易流水号
	_DATE_TIME TranDateTime;			// 交易时间
	BYTE CardholderType;				// 持卡人类型标识
	BYTE StaffType;						// 持卡人职工标识
	BYTE Name[20];						// 持卡人姓名
	BYTE CertificateNo[32];				// 持卡人证件号码
	BYTE CertificateType;				// 持卡人证件类型
	WORD Deposit;						// 押金金额（分）
	BYTE SexType;						// 持卡人性别
	BYTE AppVer;						// 应用版本
	_DATE_TIME CertificateValidDate;	// 证件有效期
	BYTE Tel[10];						// 持卡人电话	10 	BCD	
	BYTE Email[60];						// 持卡人Email	60	ASCII
	BYTE Addr1[128];					// 持卡人地址1	128	ASCII
	BYTE Addr2[128];					// 持卡人地址2	128	ASCII
	_rw_cmd_update_signcard_info()
	{
		SerialNo = 0;
		CardholderType = 0;
		StaffType = 0;
		memset(Name,0,20);
		memset(CertificateNo,0,32);
		CertificateType = 0;
		Deposit = 0;
		SexType = 0;
		AppVer = 0;
		memset(Tel,0,10);
		memset(Email,0,60);
		memset(Addr1,0,128);
		memset(Addr2,0,128);
	}
}RW_CMD_UPDATE_SIGNCARD_INFO,*LPRW_CMD_UPDATE_SIGNCARD_INFO;

// RW个性化信息命令
typedef struct _rw_cmd_personalize_info
{
	DWORD SerialNo;						// 交易流水号		4	BIN ACC定义的唯一的交易序号。从1开始，依次累加，范围1～4294967295。达到最大值后从1开始。传输时转换成INTEL序。
	_DATE_TIME TranDateTime;			// 交易时间			7	BCD	交易发生的日期时间
	BYTE OperationType;					// 操作类型			1	BIN 1：个性化（原卡更新）交易；2：个性化更新-锁旧卡（操作类型为2时6至16字段无意义）；3：个性化更新-写新卡
	BYTE CorporationCode[2];				// 公司编码			2	BCD	0001～9999
	BYTE StaffNumber[4];					// 员工编号			4	BCD	00000001 ～ 99999999
	BYTE CardHolderName[8];				// 持卡人姓名		8	ASCII	最多4汉字
	BYTE CertificateType;				// 证件类型			1	BIN	1，身份证；2，其他
	BYTE CertificateID[9];				// 证件编号			9	BCD	身份证号码
	_DATE Birthday;				// 生日				4	BCD	YYYYMMDD
	BYTE PassengerLevel;				// 乘客级别（预留）	1	BIN	固定0
	DWORD PaymentAmount;				// 支付金额			4	BIN	
	BYTE PaymentMethod;					// 支付方式			1	BIN	参见《Macau-06 System Initial Values .doc》的PaymentMethod_t的定义。
	BYTE BankCarkNumber[10];			// 银行卡号			10	BCD	支付银行卡卡号
	_rw_cmd_personalize_info()
	{
		SerialNo = 0;
		OperationType = 0;
		memset(CorporationCode, 0, sizeof(CorporationCode));
		memset(StaffNumber, 0, sizeof(StaffNumber));
		memset(CardHolderName,0,61);
		CertificateType = 0;
		memset(CertificateID,0,18);
		PassengerLevel = 0;
		PaymentAmount = 0;
		PaymentMethod = 0;
		memset(BankCarkNumber,0,10);
	}
}RW_CMD_PERSONALIZE_INFO,*LPRW_CMD_PERSONALIZE_INFO;

// 计算一票通MAC结构体
typedef struct _rw_cmd_calculate_mac
{
	WORD keyVersion;           // 计算MAC需要的密钥版本
	BYTE macCalculate[20];     // 计算MAC需要的Hash数据
	_rw_cmd_calculate_mac(){
		keyVersion = 0;
		memset(macCalculate,0,20);
	}
}RW_CMD_CALCULATE_MAC,*LPRW_CMD_CALCULATE_MAC;

// ES或PM写卡信息
typedef struct _rw_cmd_es_pm_write_card_info{
	DWORD DealNo;						// 交易流水号
	_DATE_TIME DealTime;				// 交易时间
	BYTE OperitionMode;					// 操作类型
	//一下字段用于初始化，重编码或注销
	BYTE FormVisionNO;					// 格式版本号
	BYTE ManufID;						// 厂商ID
	BYTE KeyEdition;					// 密钥版本
	WORD CityCode;						// 城市代码
	WORD TradeCode;						// 行业代码
	BYTE SupplyTime[3];					// 供货时间
	WORD SupplyDatch;					// 供货批次
	DWORD CardLogicalNumber;			// 逻辑卡号	4	BIN	车票逻辑编号 
	BYTE Reserve1[4];					// 预留

	//以下字段用于预赋值
	BYTE ReleaseID;						// 产品发行商ID
	BYTE CardType;						// 卡片物理类型
	BYTE ProductCategory;				// 产品类别
	BYTE ProductType;					// 产品类型
	BYTE ProductSubtype;				// 产品子类型
	BYTE PassengerType;					// 乘客类型
	DWORD CardAmount;					// 卡内金额
	DWORD CardTimes;					// 卡内次数
	BYTE ValidityPeriod;				// 有效期间
	BYTE ValidityPeriodUnit;			// 有效期间单位
	DWORD CardDeposit;					// 押金
	DWORD BookingAmount;				// 售票金额
	_DATE_TIME VolidStartTime;			// 有效开始日期
	BYTE Reserve2[8];					// 预留
	// 以下用于个性化交易
	WORD CorporationCode;				// 公司编码
	DWORD PersonalNO;					// 员工编号
	BYTE CardHolderName[8];				// 持卡人姓名
	BYTE CredentialsNOType;				// 证件号码类型
	BYTE CredentialsNO[9];				// 证件号码
	DWORD Birthday;						// 生日
	BYTE PassengerLevel;				// 乘客级别
	BYTE Reserve[8];					// 预留
	_rw_cmd_es_pm_write_card_info()
	{
		DealNo = 0;						// 交易流水号
		OperitionMode = 0;					// 操作类型
		//一下字段用于初始化，重编码或注销
		FormVisionNO = 0;					// 格式版本号
		ManufID = 0;						// 厂商ID
		KeyEdition = 0;					// 密钥版本
		CityCode = 0;						// 城市代码
		TradeCode = 0;						// 行业代码
		memset(SupplyTime,0x00,3);
		SupplyDatch = 0;					// 供货批次
		CardLogicalNumber = 0;				// 逻辑编号
		memset(Reserve1,0xff,4);
		//以下字段用于预赋值
		ReleaseID = 0;						// 产品发行商ID
		CardType = 0;						// 卡片物理类型
		ProductCategory = 0;				// 产品类别
		ProductType = 0;					// 产品类型
		ProductSubtype = 0;				// 产品子类型
		PassengerType = 0;					// 乘客类型
		CardAmount = 0;					// 卡内金额
		CardTimes = 0;					// 卡内次数
		ValidityPeriod = 0;				// 有效期间
		ValidityPeriodUnit = 0;			// 有效期间单位
		CardDeposit = 0;					// 押金
		BookingAmount = 0;				// 售票金额
		memset(Reserve2,0xff,8);
		// 以下用于个性化交易
		CorporationCode = 0;				// 公司编码
		PersonalNO = 0;					// 员工编号
		memset(CardHolderName,0,8);
		CredentialsNOType = 0;				// 证件号码类型
		memset(CredentialsNO,0,9);
		Birthday = 0;						// 生日
		PassengerLevel = 0;				// 乘客级别
		memset(Reserve,0xff,8);
	}
}RW_CMD_ES_PM_WRITE_CARD_INFO;

//发卡母卡外部认证数据
typedef struct _rw_authissue_card_master_card_param
{
	BYTE ICMC_Slot;//发行卡母卡的卡槽
	BYTE ICMCT_Slot;//发行卡母卡传输卡的卡槽
	BYTE ICMCT_PIN_Len;//PIN码的长度
	BYTE ICMCT_PIN_buff[2];//发行卡母卡传输卡的PIN码
}AuthIssueCardMasterCardParam;

// SAM 卡位置定义
typedef enum _Sam_card_pos_{
	SAM_TYPE_NONE = 0,		// 无SAM卡		0x00
	SAM_TYPE_YPT_ISAM,		// 一票通ISAM	0x01
	SAM_TYPE_YPT_PASM,		// 一票通PSAM	0x02
	SAM_TYPE_YKT_ISAM,		// 一卡通ISAM	0x03
	SAM_TYPE_YKT_PSAM,		// 一卡通PSAM	0x04
}SAM_POS_DEF;