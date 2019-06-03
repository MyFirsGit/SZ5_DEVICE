#pragma once
#include "SCAPI.h"
#include "CTLOGCountInfo.h"
#include "CommonDefine.h"
#include "CTBanknoteCountInfo.h"

//------------------以下是互联网取票业务消息以及结构---------------------------------------------//
typedef enum _return_code {
	RSP_CODE_OK						= 0x00,		// 成功
	RSP_CODE_ORDER_NOT_EXSIT		= 0x01,		// 订单不存在
	RSP_CODE_ORDER_IS_LOCKED		= 0x02,		// 订单已锁定
	RSP_CODE_ORDER_IS_USED			= 0x03,		// 订单已使用
	RSP_CODE_ORDER_IS_CONFIRMED		= 0x04,		// 订单已确认/解锁
	RSP_CODE_ORDER_LOCKE_FAIL		= 0x05,		// 订单锁定失败
	RSP_CODE_ORDER_CONFIRMED_FAIL	= 0x06,		// 订单确认失败
	RSP_CODE_CONNECT_FAIL			= 0x07,		// 通讯失败
	RSP_CODE_LOGIN_FAIL				= 0x08,		// 登录失败

	RSP_CODE_ORDER_REQUEST_FAILED	= 0x09,		// 订单请求失败
	RSP_CODE_GENER_ERWEIMA_FAILED	= 0x0A,		// 生成二维码失败
	RSP_CODE_ORDER_QUERY_FAILED		= 0x0B,		// 订单查询失败


	//RSP_CODE_ORDER_IS_LOCKED		= 0x02,		// 订单已锁定
	//RSP_CODE_ORDER_IS_USED			= 0x03,		// 订单已使用
	//RSP_CODE_ORDER_IS_CONFIRMED		= 0x04,		// 订单已确认/解锁
	//RSP_CODE_ORDER_LOCKE_FAIL		= 0x05,		// 订单锁定失败
	//RSP_CODE_ORDER_CONFIRMED_FAIL	= 0x06,		// 订单确认失败
	//RSP_CODE_CONNECT_FAIL			= 0x07,		// 通讯失败
	//RSP_CODE_LOGIN_FAIL				= 0x08		// 登录失败

}RETURN_CODE;
//OrderInfo（订单信息结构体）
typedef struct _orderinfo
{
	CString csOrderID;				// 订单号
	_DATE_TIME dtOrderTime;			// 下单时间
	BYTE bSaleMode;	// 售票方式 
	//0x01：按照票价售票；
	//0x02：按照起止站来售票
	BYTE bCardType;	// 卡片种类	  
	//0x01：一卡通CPU卡片；
	//0x02：一卡通M1卡片；
	//0x11：一票通CPU卡片；
	//0x12：一票通UL卡； 
	//0x13：一票通M1卡片；
	WORD wProductType;			// 车票产品种类			
	WORD wProductSubType;		// 车票产品子类型	
	DWORD dwSaleAmount;			// 售票金额		
	BYTE bPaymentMethod;		// 支付方式代码	
	//0：现金、1：储值票、2：一卡通票、3：信用卡
	WORD wBegainStation;		// 起点站站码		2	BCD	起始车站ID	0-9999，按起始站售票时候输入，其它填0.
	WORD wEndStation;			// 终点站站码		2	BCD	目的车站ID	0-9999，按起始站售票时候输入，其它填0.
	WORD wOrderPiece;			// 订单张数
	WORD wRemainPiece;			// 剩余可兑张数
	_orderinfo(){
		csOrderID=_T("");
		bSaleMode = 0;
		bCardType = 0;
		wProductSubType = 0;
		wProductType = 0;
		dwSaleAmount = 0;
		bPaymentMethod = 0;
		wBegainStation = 0;
		wEndStation = 0;
		wOrderPiece = 0;
		wRemainPiece = 0;
	}
}ORDER_INFO;
//_order_card_info（向WEBServer提交票卡信息）
typedef struct _order_card_info{
	_DATE_TIME TranDateTime;			// 交易时间
	BYTE  CardPhysicalNumber[7];		// 卡片序列号，不够7字节前补0
	CardSerialNumber_t CardLogicalNumber;	  // 车票逻辑编号
	CString strTAC;						// 交易认证码
	CString	strRWSeqNum;				// 读写器交易序列号
	WORD	wSaleAmount;				// 交易金额
	WORD	wFee;						// 
	WORD	wTickCnt;					// 卡交易序号
	DWORD	dwRWValueCnt;				// 读写器累计交易金额
	_DATE_TIME EffTime;					// 票卡生效时间
	_DATE_TIME ExpTime;					// 票卡失效时间
	BYTE SamId[6];						// SAM卡ID
	CString strSamSeqNum;				// SAM卡序列号
	WORD	wPhyType;					// 卡片物理类型
	WORD	wAppType;					// 卡片应用类型
	_order_card_info(){
		TranDateTime = _date_time();
		memset(CardPhysicalNumber,0,7);
		CardLogicalNumber = 0;
		strTAC = _T("");
		strRWSeqNum = _T("");
		EffTime = _date_time();
		ExpTime = _date_time();
		memset(SamId,0,6);
		strSamSeqNum = _T("");
		wSaleAmount = 0;
		wFee = 0;
		wTickCnt = 0;
		dwRWValueCnt = 0;
		wPhyType = 0;
		wAppType = 0;
	}
}ORDER_CARD_INFO;

//OrderProcessResult（订单执行结果结构体）
typedef struct _orderprocessresult
{
	CString csOrderID;				// 订单号
	WORD wExchangedPiece;			// 已兑换张数
	WORD wRemainPiece;				// 剩余可兑张数	
	vector<ORDER_CARD_INFO>vecCardInfo;//票卡信息

	_orderprocessresult(){
		csOrderID=_T("");
		vecCardInfo.clear();
	}
}ORDER_PROCESS_RESULT;

//--------------------------------------以下是TVM消息代码编码定义-----------------------------------------//
// TVM交易数据消息代码定义
typedef enum _msg_code_transation
{
	MSG_TRANS_RECYLE_INVALID_CARD		= 0x5005,	//  废票回收交易
	MSG_TRANS_DEVICE_IDENTIFICATION	= 0x0608,	//  ISAM卡变更（预留）
	MSG_TRANS_CD_VERSION_STATUS		= 0x0609,	//  CD版本切换（预留）
	MSG_TRANS_DEVICE_STATUS					= 0x060D,	//  设备状态变更（预留）
	MSG_TRANS_AUDIT_REGISTER					= 0x0501,	//  AR数据（预留）
} MSG_CODE_TRANS;

// TVM业务及事件数据命令代码
typedef  enum _msg_code_bussiness{
	// 业务数据
	MSG_BUSSINESS_FAILURE_REPORT					= 0x5101,   // TVM/AVM故障单
	MSG_BUSSINESS_SOFTANDPROGRAM_UPDATE				= 0x5102,   // 参数和程序更新
	MSG_BUSSINESS_SOFTANDPROGRAM_DOWNLOAD			= 0x5103,   // 参数和程序下载
	MSG_BUSSINESS_DEVICE_IMPORTANT_STATUS_CHANGE	= 0x5104,   // 设备重要状态变更（澳门项目预留）	
	MSG_BUSSINESS_ISAM_LOGIN_LOGOUT					= 0x5105,   // ISAM签到/签退（澳门项目预留）
	MSG_BUSSINESS_OPERATING_LOG						= 0x5106,   // 操作日志
	MSG_BUSSINESS_DEVICE_ERROR						= 0x5107,   // 设备故障
	MSG_BUSSINESS_DEVICE_REGISTER					= 0x5108,   // 设备寄存器
	MSG_BUSSINESS_MONEYBOX_CHANGE					= 0x5109,   // 钱箱更换
	MSG_BUSSINESS_TICKETBOX_CHANGE					= 0x510C,   // 票箱更换
	// 事件数据
	MSG_BUSSINESS_DEVICE_COMPONENT					= 0x5114,   // 设备部件构成（澳门项目预留）
	MSG_BUSSINESS_ACCOUNT_LOCK						= 0x5201,   // 账户锁定
	MSG_BUSSINESS_UDSN_RESET						= 0x5204,   // 设备流水重置
	MSG_BUSSINESS_DEVICE_COMPONENT_CHANGE			= 0x5205,   // 部件更换（澳门项目预留）
	MSG_BUSSINESS_SAM_CHANGE						= 0x520E,   // SAM更换
	MSG_BUSSINESS_MAGAZINE_INFO						= 0x5207,   // 票箱库存报告
	MSG_BUSSINESS_MONEYBOX_INFO						= 0x5208,   // 钱箱库存报告
	MSG_BUSSINESS_TVM_AUDIT							= 0x520A,   // TVM审计数据
	MSG_BUSSINESS_MONEYBOX_EMPTY_REPORT				= 0x520C,   // 钱箱清空事件
	MSG_BUSSINESS_PACKAGE_AUDIT						= 0x520D,   // 包传输审计数据	
}MSG_CODE_BUSSINESS;

// 控制数据---命令数据代码
typedef  enum _msg_code_control_command{
	MSG_SPECIFY_BLOCKNO_DATA					= 0x5201,   // 上传指定包编号数据命令
	MSG_SPECIFY_TIME_DATA						= 0x5202,   // 上传指定时间数据命令(预留)
	MSG_DEBUG_DATA								= 0x5203,   // 上传设备调试数据(预留)
	MSG_OPERATION_DATA							= 0x5204,   // 运营模式控制命令
	MSG_DEVICE_CONTROL_COMMAND					= 0x5205,   // 设备运行控制命令
	MSG_24HOUR_OPERATION_DATA					= 0x5206,   // 24小时运营控制命令
	MSG_DELAY_OPERATION_DATA					= 0x5207,   // 延长运营时间命令
	MSG_OPERATION_START_DATA					= 0x52FA,   // 运营开始命令
	MSG_OPERATION_END_DATA						= 0x52F9,   // 运营结束命令
	MSG_FORCE_TIME_SYNC							= 0x5208,   // 强制时钟同步命令
	MSG_ISSUE_RESTRICT							= 0x520A,   // 售票限制命令
	MSG_UPLOAD_PARAMETER_ID						= 0x520C,   // 上传参数和程序版本
	MSG_UPDATE_PARAMETER						= 0x520D,   // 参数和程序更新命令
	MSG_FORCE_LOGOUT							= 0x5211,   // 强制退出登录(预留)
	MSG_CHANGE_PARAMETER_ID						= 0x5212,   // 参数和程序版本切换命令(预留)
	MSG_REQUEST_DEVICE_STATUS					= 0x5213,   // 设备状态查询(预留)
	MSG_AUTORUN_PARAMETER						= 0x5215    // 自动运行参数启用控制
}MSG_CODE_CONTROL_COMMAND;

// 控制数据---请求数据代码
typedef  enum _msg_code_control_request{
	MSG_REQUEST_LOGIN_OR_LOGOUT						= 0x5101,   // 登录登出请求
	MSG_REQUEST_TVM_HELP							= 0x5102,   // TVM招援请求
	MSG_REQUEST_REFORM_PASSWORD						= 0x5103,   // 密码修改请求
	MSG_REQUEST_SEND_IMIDIATLY_PARAMETER			= 0x5107,   // 即时生效参数下发请求
	MSG_REQUEST_CONNECT_AUTH						= 0x5109,   // 连接认证请求
	MSG_REQUEST_PERSONALIZATION						= 0x5110,   // 个性化信息查询请求(预留)
	MSG_REQUEST_HISTORYINFO							= 0x5111,   // 历史交易信息查询请求(预留)
}MSG_CODE_CONTROL_REQUEST;

// 控制数据---反馈数据代码
typedef  enum _msg_code_control_reply{
	MSG_REQUEST_LOGIN_OR_LOGOUT_REPLY				= 0x5101,   // 登录登出请求反馈
	MSG_REQUEST_TVM_HELP_REPLY						= 0x5102,   // TVM招援请求反馈
	MSG_MSG_REFORM_PASSWORD_REPLY					= 0x5103,   // 密码修改请求反馈
	MSG_REQUEST_IMIDIATLY_PARAMETER_REPLY			= 0x5107,   // 即时生效参数下发请求反馈
	MSG_REQUEST_CONNECT_AUTH_REPLY					= 0x5109,   // 连接认证请求反馈
	MSG_PARAMETER_ID_DATA							= 0x5401,   // 设备参数和程序版本数据
	MSG_REQUEST_PERSONALIZATION_REPLY				= 0x5110,   // 个性化信息查询请求反馈(预留)
	MSG_REQUEST_HISTORYINFO_REPLY					= 0x5111    // 历史交易信息查询请求反馈(预留)
}MSG_CODE_CONTROL_REPLY;

// 控制数据---通知数据代码
typedef  enum _msg_code_control_notic{
	MSG_ACCOUNT_UNLOCK_NOTIC						= 0x5502,	// 账户解锁通知
	MSG_FTP_CONFIG_NOTIC							= 0x550C,	// FTP配置信息通知
	MSG_OP_END_NOTIC								= 0x550E,	// 操作结束通知
}MSG_CODE_CONTROL_NOTIC;

// 状态数据代码
typedef  enum _msg_code_status{
	MSG_DEVICE_ALL_STATUS_DATA					= 0x3101,   // 设备完整状态数据
	MSG_DEVICE_STATUS_CHANGE_DATA				= 0x3102,   // 设备变更状态数据
	MSG_DEVICE_ERROR_NOTIC						= 0x3107    // 设备故障状态数据
}MSG_CODE_STATUS;

//--------------------------------------以上是TVM消息代码编码定义-----------------------------------------//

//--------------------------------------以下是TVM消息长度编码定义-----------------------------------------//
#define LEN_AFC_SYS_HEADER 10
#define  LEN_TXN_DATATYPE 17
#define LEN_PACKAGE_HEADER 28
//6.3AFC内部交易
//6.3.1BOM对TVM退款
#define  LEN_TXN_BOM_REFUND_TVM (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+19)
//6.3.2非即时退卡申请
#define  LEN_TXN_DELAY_REFUND (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+89)
//6.3.5废票回收交易
#define  LEN_TXN_INVALID_TICKET_RECYCLE (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+20)
//6.3.6BOM非营业性收支
#define  LEN_TXN_BOM_NONBUSINESS_INCOME (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+84)
#define LEN_TXN_SYSCOM_HDR 42
#define LEN_TXN_CARDCOM_HDR 20
#define LEN_TXN_CARDHOLDERCOM_HDR 16
#define LEN_TXN_SYSSECURITY_HDR 12
#define LEN_TXN_TICKETCOM_HDR 17
#define LEN_TXN_OPERATORCOM_HDR  4
#define LEN_TXN_FINCOM_HDR 13
#define LEN_TXN_SERCOM_HDR  5

#define LEN_TXN_NONIMMIDATELY_SURRENDER (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR +LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 15 +LEN_TXN_SERCOM_HDR)

#define LEN_TXN_SIGNCARD_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_MODIFY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
//#define LEN_TXN_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_CARDCOM_HDR + 32 + LEN_TXN_SYSSECURITY_HDR) // 同LEN_TXN_BLACKLIST_CARD_REQUEST
#define LEN_TXN_CANCEL_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 42+LEN_TXN_SERCOM_HDR)

#define LEN_TXN_ADMINISTRATION_PROCESS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR+ 22+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 105 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 31 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 21 + LEN_TXN_SERCOM_HDR)

//#define LEN_TXN_REPORT_LOSS_REQUEST (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_CARDCOM_HDR+32) // 见LEN_TXN_BLACKLIST_CARD_REQUEST

#define LEN_TXN_DEVICE_IDENTIFICATION	(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_CD_VERSION_STATUS		(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + 408 + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_BLACKLIST_CARD_REQUEST	(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_CARDCOM_HDR + 32 + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_DEVICE_STATUS			(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + 4 + LEN_TXN_SYSSECURITY_HDR)
#define LEN_TXN_AUDIT_REGISTER			(LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + 2408 + LEN_TXN_SYSSECURITY_HDR)

#define LEN_BUSINESS_HEADER 32
#define INX_BUSINESS_DATATYPE (LEN_BUSINESS_HEADER-7)
#define INX_BUSINESS_HEADER_LENGTH (LEN_BUSINESS_HEADER-5)
#define LEN_BUSINESS_HEADER_DATATYPE (LEN_BUSINESS_HEADER-5)
//6.4业务数据
// TVM、AVM故障单业务数据
#define LEN_BUSINESS_FAILED_DATA (LEN_BUSINESS_HEADER + 35)
//6.4.2参数和程序更新
#define LEN_BUS_PARAMETER_SOFT_UPDATE_BASE (LEN_BUSINESS_HEADER+2)
//#6.4.3参数和程序下载业务数据
#define LEN_BUS_PARAMETER_SOFT_DOWNLOAD_BASE (LEN_BUSINESS_HEADER+1)
//6.4.4设备重要状态改变
#define LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG (LEN_BUSINESS_HEADER+4)
//6.4.5ISAM签到签退
#define LEN_BUS_ISAM_LOGIN_LOGOUT (LEN_BUSINESS_HEADER+16)
//6.4.6操作日志
#define LEN_BUS_OPERATING_LOG (LEN_BUSINESS_HEADER + 68)
//6.4.7设备故障数据
#define LEN_BUS_DEVICE_ERROR (LEN_BUSINESS_HEADER + 13)
//6.4.8设备寄存器数据
#define LEN_BUS_DEVICE_REGISTER_BASE (LEN_BUSINESS_HEADER + 3)
#define LEN_BUS_DEVICE_REGISTER (LEN_BUSINESS_HEADER + 3 + 6 * 314)
//6.4.10票箱更换
#define LEN_BUS_TICKETBOX_CHANGE (LEN_BUSINESS_HEADER + 18)
//纸币箱更换
#define LEN_BUS_BANKNOTE_BOX_CHANGE (LEN_BUSINESS_HEADER + 41)
//6.4.11设备部件构成
#define COUNT_DEVICE_COMPONET 3
#define LEN_BUS_DEVICE_COMPONENT (LEN_BUSINESS_HEADER + 2 + 7*COUNT_DEVICE_COMPONET)
//6.5事件数据
//6.5.1账户锁定
#define LEN_EVN_ACCOUNT_LOCK (LEN_BUSINESS_HEADER)
//6.5.3设备流水号重置
#define LEN_EVN_UDSN_RESET (LEN_BUSINESS_HEADER + 2 + 1*9)
//6.5.4部件更换
#define LEN_EVN_DEVICE_COMPONENT_CHANGE (LEN_BUSINESS_HEADER+13)
//6.5.5票箱库存报告
#define LEN_EVN_TICKETBOX_INFO_BASE  (LEN_BUSINESS_HEADER + 3)
#define LEN_EVN_TICKETBOX_INFO (LEN_BUSINESS_HEADER + 3 + 7*6)
//AVM钱箱库存报告(纸币箱1)
#define LEN_EVN_AVM_MONEY_BOX (LEN_BUSINESS_HEADER + 4 + 1 + 4 + 1 + 1 + (1 + 2) * 6 + 1)
//TVM钱箱库存报告(纸币箱1+纸币循环找零箱4 + 纸币找零箱1 + 硬币箱2 + 循环找零2 + 回收箱2)
#define LEN_EVN_TVM_MONEY_BOX (LEN_BUSINESS_HEADER + 4 + 1 + 24 + 36 + 9 + 36 + 24)
//纸币箱清空钱箱
#define LEN_EVN_CLEAR_BANKNOTES_INFO (LEN_BUSINESS_HEADER + 4 + 1 + 3 * 6 + 5)
//纸币找零箱清空
#define LEN_EVN_CLEAR_BANKNOTES_CHANGE_BOX_INFO (LEN_BUSINESS_HEADER + 4 + 1 + 3*2 + 5)
//硬币箱清空钱箱
#define LEN_EVN_CLEAR_COINS_INFO (LEN_BUSINESS_HEADER + 4 + 1 + 3*2 + 4 + 1 + 1 + 2 + 1 + 2*3 + 1 + 2 + 1 + 2)
//TVM审计事件
#define LEN_EVN_AVM_TVM_AUDIT (LEN_BUSINESS_HEADER + 14 + 78)
//6.6状态数据
#define LEN_STATUS_HEADER 11
#define LEN_STATUS_SINGLE_MODE (LEN_STATUS_HEADER + 2 + 4 * 1) // 状态数据长度(控制模式/24小时运营/延长模式
#define LEN_STATUS_THREE (LEN_STATUS_HEADER + 2 + 4*3) // 控制模式 + 24小时运营 + 延长模式
#define LEN_STATUS_FULL (LEN_STATUS_HEADER + 2 + 4 * 16 )  // 全状态数据
#define LEN_STATUS_TVM_FULL (LEN_STATUS_HEADER + 2 + 4 * 39) // TVM全状态数据
#define LEN_STATUS_AVM_FULL (LEN_STATUS_HEADER + 2 + 4 * 26) // AVM全状态数据
#define LEN_STATUS_HAS_ERROR (LEN_STATUS_HEADER + 7) // 异常状态电文长度
#define LEN_STATUS_CHANGE (LEN_STATUS_HEADER+4) //6.6.6设备状态变更数据

#define LEN_REQ_HEADER 2
//6.7.2请求数据
//6.7.2.1登陆登出
#define LEN_REQ_LOGINLOGOFF  (LEN_REQ_HEADER+12)
//6.7.2.3密码修改
#define LEN_REQ_CHANGE_PASSWORD (LEN_REQ_HEADER+17)

#define LEN_REQ_PERSONALIZATION (LEN_REQ_HEADER + 5 + 61 + 1 + 4 + 21 + 21 + 129 + 21 + 16 + 10 + 4 + 10 + 1 + 1 + 41 + 41 + 41 + 41 + 41 + 19 + 1)

//6.7.2.5参数修改
//6.7.2.6即时生效参数下发请求
#define LEN_REQ_SEND_IMMIDIATLY_PARAMETER (LEN_REQ_HEADER+6)
//6.7.2.8连接认证请求
#define  LEN_REQ_CONNECT_AUTH (LEN_REQ_HEADER+11)

// 招援请求
#define LEN_REQ_CALL_HELP (LEN_REQ_HEADER + 14)

//6.7.3通知数据
//6.7.3.13操作结束通知
#define  LEN_NOTICE_OPERATION_END 4

//6.7.4反馈数据
#define LEN_REPLY_PARAMETER_VERSION (10+15*40)

//--------------------------------------以上是TVM消息长度编码定义-----------------------------------------//

// 传输错误代码
typedef enum _transfer_error{
	ERR_PROTOCOL_IDENTIFY        = 0x0001,   // 协议标识错误
	ERR_PROTOCOL_VERSION         = 0x0002,   // 协议版本号错误
	ERR_DATA_FORMAT_VERSION      = 0x0003,   // 数据格式版本号错误
	ERR_PROTOCOL_COMMAND         = 0x0004,   // 协议命令错误
	ERR_DATA_TYPE                = 0x0005,   // 数据类型编码错误
	ERR_CRC_TYPE                 = 0x0006,   // 校验码算法错误
	ERR_CRC_DATA                 = 0x0007,   // 校验码错误
	ERR_BLOCK_DATA               = 0x0008,   // 分包数据错误
	ERR_ECRYPT                   = 0x0009,   // 加密算法错误
	ERR_KEY_VERSION              = 0x000A,   // 密钥版本错误
	ERR_SAME_DATAD               = 0x000B,   // 重复消息
	ERR_PACKAT_LENGTH            = 0x00FD,   // 包体长度错误
	ERR_OTHER                    = 0x00FE    // 其它错误
}TRANSFER_ERROR;

// 操作代码
typedef enum _operation_code{
	OPERATION_LOGIN          = 0x1001, // 登录(TVM)
	OPERATION_LOGOUT         = 0x1002, // 登出(TVM)
	OPERATION_INPUT_PARAM    = 0x1101, // 导入参数和程序(TVM)
	OPERATION_OUTPUT_PARAM   = 0x1102, // 导出参数和程序
	OPERATION_INPUT_DEAL     = 0x1103, // 导入交易数据
	OPERATION_OUTPUT_DEAL    = 0x1104, // 导出交易数据(TVM)
	OPERATION_INPUT_OPER     = 0x1105, // 导入业务数据
	OPERATION_OUTPUT_OPER    = 0x1106, // 导出业务数据(TVM)
	OPERATION_INPUT_DEBUG    = 0x1107, // 导入调试数据
	OPERATION_OUTPUT_DEBUG   = 0x1108, // 导出调试数据(TVM)
	OPERATION_FORCE_TIME     = 0x1411, // 强制时钟同步
	OPERATION_MANUAL_TIME    = 0x1412, // 手工时钟同步
	OPERATION_24HOUR_DELAY   = 0x1701, // 24小时运营和延长运营设置
	OPERATION_DEBUG_UPLOAD   = 0x1702, // 调试日志上传(TVM)
	OPERATION_PARAM_EDIT     = 0x1A01, // 参数编辑(编辑、发布、下发）
	OPERATION_MODE_SET       = 0x1901, // 模式设定（人工设置设备模式）
	OPERATION_MODE_NOTIC     = 0x1902, // 模式通知设定(TVM)
	OPERATION_TIME_WARNING   = 0xA101, // 时钟同步警告(TVM)
	OPERATION_TIME_ERROR     = 0xA102, // 时钟同步异常(TVM)
	OPERATION_CONNECT_STATUS = 0xA103,  // 连接状态(TVM)
	OPERATION_SERVICE_STATUS_SETTINGS     = 0xA401,// 服务模式设置(TVM)	     
	OPERATION_AISLEDIRECTION_SETTINGS    = 0xA402,// 通道类型设置	    
	OPERATION_FLAP_STATUS_SETTINGS        = 0xA403,// 闸门模式设置	    
	OPERATION_DEVICE_SETTINGS            = 0xA404,// 本机设置	(TVM)        
	OPERATION_NETWORK_SETTINGS           = 0xA405,// 通讯设置	 (TVM)       
	OPERATION_DATETIME_SETTINGS       = 0xA406,// 日期设置(TVM)	        
	OPERATION_COM_SETTINGS               = 0xA407,// 串口设置	        
	OPERATION_FLAPOPENCLOSESPEED       = 0xA408,// 扇门开关时间设置	
	OPERATION_VOLUME_SETTINGS            = 0xA409,// 设备音量设置  	  
	OPERATION_REMOVE_ERROR               = 0xA40A,// 异常解除(TVM)	        
	OPERATION_BALANCE_PROCESSING         = 0xA40B,// 业务结束(TVM)	        
	OPERATION_SHUTDOWN                  = 0xA40C,// 关机(TVM)	            
	OPERATION_RESTART                   = 0xA40D,// 重启(TVM)	            
	OPERATION_NETWORK_TEST               = 0xA501,// 网络测试(TVM)	        
	OPERATION_RW_TEST                   = 0xA502,// 读卡器测试	(TVM)      
	OPERATION_COIN_UNIT_TEST              = 0xA503,// 硬币模块测试(TVM)	    
	OPERATION_BANKNOTE_UNIT_TEST          = 0xA504,// 纸币模块测试	(TVM)    
	OPERATION_TOKEN_UNIT_TEST             = 0xA505,// TOKEN模块测试(TVM)	    
	OPERATION_CARD_UNIT_TEST         = 0xA506,// Card模块(TVM)	        
	OPERATION_KEYBOARD_TEST              = 0xA507,// 维护键盘测试	    
	OPERATION_DISPLAY_TEST               = 0xA508,// 屏幕测试	        
	OPERATION_SOUND_TEST                 = 0xA509,// 声音测试	        
	OPERATION_IO_UNIT_TEST                = 0xA50A,// IO部测试(TVM)	        
	OPERATION_TOKEN_RECYCLE_UNIT_TEST      = 0xA50B,// 回收部测试	      
	OPERATION_PASSENGER_CONTROLUNIT_TEST  = 0xA50C,// 通行逻辑测试	    
	OPERATION_BANKCARD_UNIT_TEST          = 0xA50D,// 银行卡模块测试(TVM)    
	OPERATION_PINPAD_UNIT_TEST            = 0xA50E,// 密码键盘测试(TVM)	    
	OPERATION_PRINTER_UNIT_TEST           = 0xA50F,// 打印模块测试(TVM)	    
	OPERATION_REPRINT_RECEIPT            = 0x1D01,// 凭证补打(TVM)	        
	OPERATION_CARDBOX_REPLACEMENT        = 0xA601,// Card票箱更换(TVM)	    
	OPERATION_CARDBOX_INSTALLATION       = 0xA602,// Card票箱安装(TVM)	    
	OPERATION_CARDBOX_REMOVAL            = 0xA603,// Card票箱卸载(TVM)	    
	OPERATION_TOKENBOX_REPLACEMENT       = 0xA701,// Token票箱更换(TVM)	    
	OPERATION_TOKENBOX_INSTALLATION      = 0xA702,// Token票箱安装(TVM)	    
	OPERATION_TOKENBOX_REMOVAL           = 0xA703,// Token票箱卸载(TVM)	    
	OPERATION_TOKEN_RECYCLE              = 0xA704,// Token回收(TVM)	        
	OPERATION_COINBOX_REPLACEMENT        = 0xA801,// Coin箱替换(TVM)	      
	OPERATION_COINBOX_INSTALLATION       = 0xA802,// Coin箱安装(TVM)	      
	OPERATION_COINBOX_REMOVAL            = 0xA803,// Coin箱卸载(TVM)	      
	OPERATION_COIN_RECYCLE               = 0xA804,// Coin箱回收(TVM)	   
	OPERATION_COIN_CHANGE_INSTALL		 = 0xA805,// Coin找零箱安装
	OPERATION_COIN_CHANGE_REMOVAL		 = 0xA806,// Coin找零箱卸载
	OPERATION_BANKNOTEBOX_REPLACEMENT    = 0xA901,// 纸币箱更换(TVM)	      
	OPERATION_BANKNOTEBOX_INSTALLATION   = 0xA902,// 纸币箱安装(TVM)	      
	OPERATION_BANKNOTEBOX_REMOVAL        = 0xA903,// 纸币箱卸载(TVM)	 
	OPERATION_CHANGE_BOX_INSTALLATION	 = 0xA904,	// 纸币找零箱安装
	OPERATION_CHANGE_BOX_REMOVAL		 = 0xA905,	// 纸币找零箱卸载
	OPERATION_INVALID_BOX_INSTALLATION	 = 0xA906,	// 废币回收箱安装
	OPERATION_INVALID_BOX_REMOVAL		 = 0xA907,	// 废币回收箱卸载    
}OPERATION_CODE;

// 操作级别
typedef enum _operation_level{
	OPERATION_ONE_LEVEL   = 0x01, // 普通操作
	OPERATION_TWO_LEVEL   = 0x02, // 警告信息 
	OPERATION_THREE_LEVEL = 0x03, // 重要操作
	OPERATION_FOUR_LEVEL  = 0x04, // 故障信息
	OPERATION_FIVE_LEVEL  = 0x05  // 危险操作
}OPERATION_LEVEL;

// 设备控制命令代码
typedef enum{
	DEVICE_CONTRL_CODE_SC_UNKNOW										= 0x0000, //未知
	DEVICE_CONTRL_CODE_SC_POWER_OFF										= 0x0101, //电源关闭
	DEVICE_CONTRL_CODE_SC_RESTART_TVM									= 0x0102, //重新启动
	DEVICE_CONTRL_CODE_SC_OPERATION_START								= 0x0103, //运营开始
	DEVICE_CONTRL_CODE_SC_OPERATION_END									= 0x0104, //运营结束
	DEVICE_CONTRL_CODE_SC_SLEEP											= 0x0105, //睡眠模式
	DEVICE_CONTRL_CODE_SC_WAKEUP										= 0x0106, //远程唤醒
	DEVICE_CONTRL_CODE_SC_START_SERVICE									= 0x0107, //开始服务
	DEVICE_CONTRL_CODE_SC_OUT_OF_SERVICE								= 0x0108, //暂停服务
	DEVICE_CONTRL_CODE_SC_RESTART_SOFTWARE								= 0x0109, //软件重启
	DEVICE_CONTRL_CODE_SC_DE_OPER_ON									= 0x0301, //降级运行开
	DEVICE_CONTRL_CODE_SC_DE_OPER_OFF									= 0x0302, //降级运行关
	DEVICE_CONTRL_CODE_SC_BANKNOTE_ONLY									= 0x0303, //只收纸币
	DEVICE_CONTRL_CODE_SC_COIN_ONLY										= 0x0304, //只收硬币
	DEVICE_CONTRL_CODE_SC_NO_CHANGE										= 0x0305, //无找零模式
	DEVICE_CONTRL_CODE_SC_ONLY_SVT_ISSUE								= 0x0306, //只储值卡售票模式
	DEVICE_CONTRL_CODE_SC_NOT_ISSUE										= 0x0308, //不能发售单程票模式
	DEVICE_CONTRL_CODE_SC_ONLY_ANALY									= 0x0309, //只验卡
	DEVICE_CONTRL_CODE_SC_NOT_SVT										= 0x030A, //不能接收储值卡
	DEVICE_CONTRL_CODE_SC_NOT_COIN										= 0x030B, //暂不收硬币模式
	DEVICE_CONTRL_CODE_SC_NOT_BANKNOTE									= 0x030C, //暂不收纸币模式
	DEVICE_CONTRL_CODE_SC_RECOVER_NORMAL_MODE							= 0x030D, //恢复正常模式
	DEVICE_CONTRL_CODE_SC_NO_PRINT										= 0x030E, //无打印模式//暂未定义，仅仅为了编译
	// 以下是找零模式定义
	DEVICE_CONTRL_CODE_NO_CHANGE									= 0x0800, //无找零
	DEVICE_CONTRL_CODE_NO_COIN_CHANGE								= 0x0801, //仅纸币找零，无硬币找零
	DEVICE_CONTRL_CODE_NO_BILL_CHANGE								= 0x0802, //仅硬币找零，无纸币找零
	DEVICE_CONTRL_CODE_ALL_CHANGE									= 0x0803, // 全找
	// 以下是业务模式组合定义
	DEVICE_CONTRL_CODE_BUSINESS										= 0x0600,  //  业务模式类代码
	DEVICE_CONTRL_CODE_BUSINESS_CARD								= 0x0601,  //  售卡
	DEVICE_CONTRL_CODE_BUSINESS_CHARGE								= 0x0602,  //  充值
	DEVICE_CONTRL_CODE_BUSINESS_QUERY								= 0x0604,  //  查询
	DEVICE_CONTRL_CODE_BUSINESS_ALL									= 0x0607,  //  全部
	//DEVICE_CONTRL_CODE_BUSINESS_TOKEN								= 0x0610,  //  售Token(预留)
	//DEVICE_CONTRL_CODE_BUSINESS_PRODUCT							= 0x0604,  //  售产品(预留)

	// 以下是支付模式组合定义
	DEVICE_CONTRL_CODE_PAYMENT										= 0x0700, // 支付模式类代码
	DEVICE_CONTRL_CODE_PAYMENT_COIN									= 0x0701, // 收硬幣
	DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE								= 0x0702, // 收紙幣
	DEVICE_CONTRL_CODE_PAYMENT_SVT									= 0x0704, // 收储值卡
	DEVICE_CONTRL_CODE_PAYMENT_ALL									= 0x0707, // 全收（其他为预留）
	//DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD						= 0x0710, // 信用卡
	//DEVICE_CONTRL_CODE_PAYMENT_BANKCARD							= 0x0702, // 收银行卡
	//DEVICE_CONTRL_CODE_PAYMENT_EP									= 0x0701, // 收EP
}DEVICE_CONTRL_CODE;

// 钱、票箱位置
typedef enum _magazine_palace{
						// 钱箱              票箱
	MAGAZINEA  = 0x01,  // 纸币回收			废票箱
	MAGAZINEB  = 0x02,  // 硬币找零B		B票箱
	MAGAZINEC  = 0x03,  // 硬币找零A		A票箱
	MAGAZINED  = 0x04,	// 硬币HopperB		HopperB
	MAGAZINEE  = 0x05,	// 硬币HopperA		HopperA
	MAGAZINEF  = 0x06,	// 硬币回收箱B		回收箱
	MAGAZINEG  = 0x07,	// 硬币回收箱A
	MAGAZINEH  = 0x08,	// 纸币补充箱
	MAGAZINEI  = 0x09,	// 纸币找零A
	MAGAZINEJ  = 0x0A,	// 纸币找零B
}MAGAZINE_PALACE;

// 票箱类型
typedef enum _magazine_type{
	MAGAZINE_NORMAL = 0x01, // 正常票箱	0x01
	MAGAZINE_CYCLE  = 0X02  // 废票箱	0x02
}_MAGAZINE_TYPE;

// 票箱状态
typedef enum _sc_magazine_status{
	VALID_ATTACH   = 0x01, // 正常安装；
	INVALID_ATTCH  = 0x02, // 非法安装；
	VALID_DETACH   = 0x03, // 正常卸载；
	INVALID_DETACH = 0x04  // 非法卸载
}SC_MAGAZINE_STATUS;


//--------------------------------------以下是TVM状态编码定义-----------------------------------------//
typedef struct _tagTVM_STATUS_ID
{
	BYTE b1;
	BYTE b2;
	BYTE b3;
} TVM_STATUS_ID;

namespace TVM_STATUS_IDS
{
	const static _tagTVM_STATUS_ID   SERVICE_STATUS = {0x01,0x01,0x01};			// 服务状态
	const static _tagTVM_STATUS_ID   RUN_STATUS		= {0x01,0x01,0x02};			// 运行状态
	const static _tagTVM_STATUS_ID   WORK_MODE		= {0x01,0x01,0x03};         // 工作模式
	const static _tagTVM_STATUS_ID   OPERATION_MODE = {0x01,0x01,0x04};         // 运营模式，参见7.9运营模式代码。
	const static _tagTVM_STATUS_ID   HOUR24MODE		= {0x01,0x01,0x05};         // 24小时运营模式
	const static _tagTVM_STATUS_ID   DELAY_RUN_MODE = {0x01,0x01,0x06};         // 延长运营时间
	const static _tagTVM_STATUS_ID   CONNECT_STATUS = {0x01,0x01,0x07};         // 连接状态
	const static _tagTVM_STATUS_ID   TIME_DELAY		= {0x01,0x01,0x08};         // 时钟差异
	const static _tagTVM_STATUS_ID   LOGIN_STATUS	= {0x01,0x01,0x09};         // 登录状态
	const static _tagTVM_STATUS_ID   DEVICE_AUTO_RUN_STATUS = {0x01,0x01,0x0A}; // 设备自动运行状态	
	const static _tagTVM_STATUS_ID   DEVICE_ERR_STATUS = {0x01,0x01,0x0B};      // 设备故障状态状态

	const static _tagTVM_STATUS_ID   CARD_BOX1_STATUS		= {0x06,0x01,0x01};			// 卡式票箱1安装状态
	const static _tagTVM_STATUS_ID   CARD_BOX1_COUNT		= {0x06,0x01,0x02};			// 卡式票箱1数量状态
	const static _tagTVM_STATUS_ID   CARD_BOX2_STATUS		= {0x06,0x02,0x01};			// 卡式票箱2安装状态
	const static _tagTVM_STATUS_ID   CARD_BOX2_COUNT		= {0x06,0x02,0x02};			// 卡式票箱2数量状态
	const static _tagTVM_STATUS_ID   CARD_CYCLEBOX_STATUS	= {0x07,0x01,0x01};			// 卡式废票箱安装状态
	const static _tagTVM_STATUS_ID   CARD_CYCLEBOX_COUNT	= {0x07,0x01,0x02};			// 卡式废票箱数量状态

	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX1_STATUS = {0x09,0x01,0x01};		// 硬币补充箱1安装状态
	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX1_COUNT  = {0x09,0x01,0x02};		// 硬币补充箱1数量状态
	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX2_STATUS = {0x09,0x02,0x01};		// 硬币补充箱2安装状态
	const static _tagTVM_STATUS_ID   COIN_CHANGE_BOX2_COUNT  = {0x09,0x02,0x02};		// 硬币补充箱2数量状态
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX1_STATUS  = {0x0A,0x01,0x01};		// 硬币回收箱1安装状态
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX1_COUNT   = {0x0A,0x01,0x02};		// 硬币回收箱1数量状态
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX2_STATUS  = {0x0A,0x02,0x01};		// 硬币回收箱2安装状态
	const static _tagTVM_STATUS_ID   COIN_CYCLE_BOX2_COUNT   = {0x0A,0x02,0x02};		// 硬币回收箱2数量状态
	const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX1_COUNT  = {0x0D,0x01,0x01};		// 硬币Hopper箱数量状态
	const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX2_COUNT  = {0x0D,0x02,0x01};		// 硬币Hopper箱数量状态
	//const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX1_STATUS = {0x09,0x03,0x01};		// 硬币Hopper箱安装状态（无）
	//const static _tagTVM_STATUS_ID   COIN_HOPPER_BOX2_STATUS = {0x09,0x04,0x01};		// 硬币Hopper箱安装状态（无）
	//const static _tagTVM_STATUS_ID   COIN_CYCLE_HOPPER_BOX_STATUS = {0x09,0x05,0x01};	// 硬币循环找零Hopper箱安装状态(无)
	//const static _tagTVM_STATUS_ID   COIN_CYCLE_HOPPER_BOX_COUNT = {0x0D,0x03,0x01};	// 硬币循环找零Hopper箱数量状态	

	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX1_STATUS = {0x0B,0x01,0x01};	// 纸币找零箱1安装状态
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX1_COUNT	 = {0x0B,0x01,0x02};	// 纸币找零箱1数量
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX2_STATUS = {0x0B,0x02,0x01};	// 纸币找零箱2安装状态（预留）
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_BOX2_COUNT	 = {0x0B,0x02,0x02};	// 纸币找零箱2数量（预留）
	const static _tagTVM_STATUS_ID   BANKNOTE_CYCLE_BOX_STATUS   = {0x0C,0x01,0x01};	// 纸币回收箱1安装状态
	const static _tagTVM_STATUS_ID   BANKNOTE_CYCLE_BOX_COUNT    = {0x0C,0x01,0x02};	// 纸币回收箱1数量状态

	const static _tagTVM_STATUS_ID   SECURITY_DOOR_STATUS	= {0x0E,0x01,0x01};			// 现金安全门状态
	const static _tagTVM_STATUS_ID   TOUCH_SCREEN_STATUS	= {0x0F,0x01,0x01};			// 触摸屏状态

	//const static _tagTVM_STATUS_ID   HELP_BUTTON_STATUS		= {0x11,0x01,0x01};			// 招援按钮状态

	const static _tagTVM_STATUS_ID   RW1_STATUS				= {0x13,0x01,0x01};			// 读写器1状态
	const static _tagTVM_STATUS_ID	 RW2_STATUS				= {0x13,0x02,0x01};			// 读写器2状态（预留）


	const static _tagTVM_STATUS_ID   RW_SAM1_TYPE			= {0x14,0x01,0x01};			// SAM1类型（01: LRT SAM Normal； 02: Macau Pass SAM Normal； 03: LRT SAM Failure； 04: Macau Pass SAM Failure）
	const static _tagTVM_STATUS_ID	 RW_SAM2_TYPE			= {0x14,0x02,0x01};			// SAM2类型
	const static _tagTVM_STATUS_ID	 RW_SAM_1_STATUS		= {0x14,0x01,0x02};			// SAM1状态
	const static _tagTVM_STATUS_ID	 RW_SAM_2_STATUS		= {0x14,0x02,0x02};			// SAM2状态
	const static _tagTVM_STATUS_ID   PRINTER_STATUS			= {0x15,0x01,0x01};			// 打印机
	
	const static _tagTVM_STATUS_ID   DOOR_STATUS			= {0x1C,0x01,0x01};			// 维修门综合状态
	const static _tagTVM_STATUS_ID   BANKNOTE_ACCEPT_MODULE_STATUS = {0x1E,0x01,0x01};  // 纸币接收模块状态
	const static _tagTVM_STATUS_ID	 BANKNOTE_CHANGE_MODULE_STATUS = {0x1F,0x01,0x01};	// 纸币找零模块状态
	const static _tagTVM_STATUS_ID   CARD_MODULE_STATUS		= {0x20,0x01,0x01};			// 卡式发行模块
	const static _tagTVM_STATUS_ID   COIN_MODULE_STATUS		= {0x21,0x01,0x01};			// 硬币模块状态
	const static _tagTVM_STATUS_ID   STATUS_DISPLAY_STATUS  = {0x23,0x01,0x01};			// 状态显示器模块	
	const static _tagTVM_STATUS_ID   DEVICE_TRANSFER_AUDIT = {0x01,0x01,0x0C};			// 传输审计状态


	/*const static _tagTVM_STATUS_ID   PAYMENT_MODE_STATUS = {0x01,0x01,0x0D};    // 支付模式状态
	const static _tagTVM_STATUS_ID   PRINT_MODE_STATUS = {0x01,0x01,0x0E};      // 打印模式状态
	const static _tagTVM_STATUS_ID   CHANGE_MODE_STATUS = {0x01,0x01,0x0F};     // 找零模式状态
	const static _tagTVM_STATUS_ID   DISC_SPACE_STATUS = {0x04,0x01,0x01};      // 磁盘空间状态*/

	const static _tagTVM_STATUS_ID   TOKEN_BOX1_STATUS = {0x06,0x01,0x01};	// Token式票箱1安装状态
	const static _tagTVM_STATUS_ID   TOKEN_BOX1_COUNT =  {0x06,0x01,0x02};		// Token式票箱1数量状态
	const static _tagTVM_STATUS_ID   TOKEN_BOX2_STATUS = {0x06,0x02,0x01};		// Token式票箱2安装状态
	const static _tagTVM_STATUS_ID   TOKEN_BOX2_COUNT =  {0x06,0x02,0x02};		// Token式票箱2数量状态
	const static _tagTVM_STATUS_ID   TOKEN_CYCLEBOX_STATUS = {0x07,0x01,0x01};  // Token式废票箱安装状态
	const static _tagTVM_STATUS_ID   TOKEN_CYCLEBOX_COUNT = {0x07,0x01,0x02};   // Token式废票箱数量状态
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER1_STATUS = {0x07,0x03,0x01};	// Token式票箱1安装状态（无）
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER1_COUNT = {0x0D,0x04,0x02};	// Token式票箱1数量状态
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER2_STATUS = {0x07,0x04,0x01};	// Token式票箱2安装状态(无)
	const static _tagTVM_STATUS_ID   TOKEN_HOPPER2_COUNT = {0x0D,0x05,0x01};	// Token式票箱2数量状态
	const static _tagTVM_STATUS_ID   TOKEN_COLLBOX_STATUS = {0x07,0x02,0x01};  // Token式回收箱安装状态
	const static _tagTVM_STATUS_ID   TOKEN_COLLEBOX_COUNT = {0x07,0x02,0x02};   // Token式回收箱数量状态

	/*
	const static _tagTVM_STATUS_ID   TILT_STATUS = {0x24,0x01,0x01};           // 振动传感器状态
	const static _tagTVM_STATUS_ID   AUTHORIZED_STATUS = {0x40,0xEF,0xEC};           // 授权访问状态
	const static _tagTVM_STATUS_ID   OFFLINE_DAYS = {0x40,0xEF,0xEA};           // 脱机天数*/
}

// 设备服务状态编码定义
typedef enum{
	IN_SERVICE    = 0x00, // 设备正常服务
	WARN_SERVICE  = 0x01, // 设备警告服务
	OUTOF_SERVICE = 0x02, // 设备暂停服务
	ERROR_SERVICE = 0x03, // 设备故障关闭
	MANT_SERVICE  = 0x04, // 设备维修关闭
	SLEEP_SERVICE = 0x05, // 设备睡眠模式
	STOP_SERVICE  = 0x06, // 设备停止服务（运营结束）
	EMER_SERVICE  = 0x07  // 设备紧急服务
}SERVICE_STATUS;

// 运行状态编码定义 
typedef enum{
	RUNING							= 0x00,  // 正常运行中
	CLOSEING						= 0x01,  // 业务终了中
	START_RUNING				    = 0x02,  // 再启动中
	SHUTDOWNING						= 0x03,  // 关机中
	WARNING							= 0x04,  // 设备报警
	OPERATION_STARTING				= 0x05   // 设备开始运营
}RUN_STATUS;

// 工作模式状态编码定义
//typedef enum{
//	WORK_MODE_NONE					= 0x00,			// 无业务
//	WORK_MODE_ISSUE					= 0x01,			// 只售票
//	WORK_MODE_RECHARGE				= 0x02,			// 只充值
//	WORK_MODE_ALL					= 0x03			// 所有业务
//	//WORK_MODE_ISSUE_CARD			= 0x08,			// 只售卡
//	//WORK_MODE_ISSUE_PRODUCT		= 0x04,			// 只售产品	
//	//WORK_MODE_QUERY				= 0x01,			// 只查询
//}WORK_MODE;

// 运营模式状态编码定义 mzy
typedef enum{
	OPERATION_MODE_STATUS_UNKNOW     				= 0xFFFF,	// 未知模式
	OPERATION_MODE_STATUS_NORMAL_MODE				= 0x01,		// 正常模式
	OPERATION_MODE_STATUS_CLOSE_MODE			    = 0x02,		// 关闭模式
	OPERATION_MODE_STATUS_EMERGENCY_MODE			= 0x06,		// 紧急模式
	OPERATION_MODE_STATUS_FARE_FREE_MODE		    = 0x07		// 时间免检模式
}OPERATION_MODE;

// 24小时运营模式状态编码定义
typedef enum{
	OPERATION_24HOURS_OFF				= 0x00,		// 非24小时运营
	OPERATION_24HOURS_ON				= 0x01 		// 24小时运营
}OPERATION_24HOURS_MODE;

// 延长运营模式状态编码定义
typedef enum{
	OPERATION_DELAY_OFF			= 0x00,		// 非延长运营
	OPERATION_DELAY_ON			= 0x01 		// 延长运营
}OPERATION_DELAY_MODE;

// 通讯状态编码定义
typedef enum{
	DISCONNECT = 0x00,// 通讯中断
	CONNECTED  = 0x01,// 通讯正常
	CONNECTING = 0x02 // 通讯恢复中
}COMMUNICATION;

// 时钟差异状态编码定义
typedef enum{
	VALID_TIME = 0x00, // 时钟正常
	WARN_TIME  = 0x01, // 时钟警告
	ERROR_TIME = 0x02  // 时钟故障
}TIME_STATUS;

// 登录状态编码定义
typedef enum{
	LOGIN   = 0x00,	// 有人登录	登录成功后的状态
	LOGOUT  = 0x01,	// 无人登录	退出登录后的状态
	LOCKED  = 0x02	// 账户锁定	账户锁定后的状态，有人登录成功后状态改变
}LOGIN_STATUS;

// 设备故障状态编码定义
typedef enum{
	DEVICE_NO_ERROR			= 0x00, // 无故障
	DEVICE_ERROR			= 0x01  // 有故障
}DEVICE_ERR_CODE;

// 传输审计状态
typedef enum{
	TRANSFER_AUDIT_STATUS_WARNING		= 0x00,	// 警告
	TRANSFER_AUDIT_STATUS_NORMAL		= 0x01,	// 正常	
	TRANSFER_AUDIT_STATUS_CLOSE			= 0x02	// 结束	
}TRANSFER_AUDIT_STATUS;

// 磁盘空间状态编码定义
typedef enum{
	DISK_SPACE_NORMAL			= 0x00, // 正常
	DISK_SPACE_FULL				= 0x01,  // 满
	DISK_SPACE_NEAR_FULL		= 0x02  // 将满
}DISK_SPACE_CODE;

// 票箱/钱箱安装状态编码定义
typedef enum{
	STATUS_MAGAZINE_SET								= 0x00,		// 已安装
	STATUS_MAGAZINE_INVALIDE_ATACH					= 0x01,		// 非法安装
	STATUS_MAGAZINE_UNSET							= 0x02,    // 未安装
	STATUS_MAGAZINE_UNREADY							= 0x03,    // 不到位
}STATUS_MAGAZINE;

// 票箱/钱箱/Hopper箱数量状态编码定义
typedef enum{
	MAG_NUM_NORMAL			    = 0x00,	// 正常
	MAG_NUM_EMPTY				= 0x01,	// 空
	MAG_NUM_NEAR_EMPTY			= 0x02,	// 将空
	MAG_NUM_FULL				= 0x03,	// 满
	MAG_NUM_NEAR_FULL			= 0x04	// 将满
}MAGAZINE_NUM_STATUS;

// 现金安全门状态（暂不使用）
typedef enum{
	SECURITY_DOOR_STATUS_NORMAL_OPEN		= 0x00,	// 正常打开
	SECURITY_DOOR_STATUS_NORMAL_CLOSE		= 0x01,	// 正常关闭
	SECURITY_DOOR_STATUS_INVALIDE_OPEN		= 0x02	// 异常打开
}SECURITY_DOOR_STATUS;					

// 触摸屏状态（暂不使用）
typedef enum{
	TOUCH_SCREEN_STATUS_NORMAL			= 0x00,	// 正常
	TOUCH_SCREEN_STATUS_ERROR			= 0x01,	// 故障
}TOUCH_SCREEN_STATUS;					

// 招援按钮状态
typedef enum{
	ASSISTANCE_BUTTON_STATUS_UNPRESSED		= 0x00,	// 按下
	ASSISTANCE_BUTTON_STATUS_PRESSED		= 0x01,	// 未按下
}ASSISTANCE_BUTTON_STATUS;			

// 读写器状态编码定义
typedef enum{
	RW_NORMAL			= 0x00,		// 读写器正常
	RW_ERROR			= 0x01,		// 读写器故障
	RW_LOSS				= 0x02,		// 读写器丢失
	RW_SAM_ERROR		= 0x03		// 读写器SAM错误
}STATUS_RW;

// SAM卡状态编码定义
typedef enum{
	SAM_STATUS_LRT_NORMAL					= 0x01,		// LRT SAM卡正常
	SAM_STATUS_LRT_FAILURE					= 0x02,		// LRT SAM卡异常
	SAM_STATUS_MACAUPASS_NORMAL				= 0x03,		// 澳门通SAM卡正常
	SAM_STATUS_MACAUPASS_FAILURE			= 0x04		// 澳门通SAM卡异常
}SAM_STATUS;

// 打印机状态编码定义
typedef enum{
	PRINTER_NORMAL   = 0x00,  // 打印机正常工作
	PRINTER_ERR      = 0x01,  // 打印机通信故障
	PRINTER_SHUTDOWN = 0x02,  // 打印机断电
	PRINTER_NOPAPER  = 0x03	  // 打印机缺纸
}PRINTER_STATUS;

// UPS状态编码定义
typedef enum{
	UPS_STATUS_OPEN			= 0x00,  // UPS开
	UPS_STATUS_CLOSE			= 0x01,  // UPS关
	UPS_STATUS_WARNING	= 0x02   // UPS电量低
}UPS_STATUS;

// 维修门状态编码定义
typedef enum{
	DOOR_NORMAL_OPEN  = 0x00, // 维修门正常打开
	DOOR_NORMAL_CLOSE = 0x01, // 维修门正常关闭
	DOOR_ILLEGLE_OPEN = 0x02, // 维修门非法打开
	DOOR_CLOSE_UNLOCK = 0x03  // 维修门关闭未锁
}DOOR_STATUS;

// 纸币/硬币/卡式发行/Token式发行/状态显示器模块状态编码定义
typedef enum{
	HARDWARE_MODULE_NORMAL				= 0x00,  // 设备正常
	HARDWARE_MODULE_UNSET				= 0x01,  // 设备不到位
	HARDWARE_MODULE_NO_CONNECT			= 0x02,  // 设备通信失败
	HARDWARE_MODULE_FAILURE				= 0x03,  // 设备故障
	HARDWARE_MODULE_JAM					= 0x04	 // 设备卡币或卡票
}HARDWARE_MODULE_STATUS;

// 振动传感器状态编码定义
typedef enum{
	TILT_STATUS_OFF		= 0x00,  // 无振动
	TILT_STATUS_ON     = 0x01   // 有振动
}TILT_STATUS;

// 授权访问状态编码定义
typedef enum{
	AUTHORIZED_STATUS_NORMAL			= 0x00,  // 正常
	AUTHORIZED_STATUS_ABNORMAL     = 0x01   // 维修门常开报警
}AUTHORIZED_STATUS;

// 设备自动运行状态编码定义
typedef enum{
	AUTORUN_STOP  = 0x00, // 设备自动运行状态停用
	AUTORUN_START = 0x01  // 设备自动运行状态启用
}DEVICE_AUTORUN;

// SAM卡认证状态（预留）
typedef enum{
	ISAM_UNAUTH = 0x00, // ISAM状态未验证
	ISAM_AUTHED = 0x01  // ISAM状态已验证
}SAM_AUTH_STATUS;

//--------------------------------------以上是TVM状态编码定义-----------------------------------------//

//--------------------------------------以下是自定义编码-----------------------------------------//
// SAM位置编码定义
typedef enum{
	RECHARGE_RW_YPT_ISAM		= 0x01,// 充值读写器一票通ISAM
	RECHARGE_RW_YPT_PSAM		= 0x02,// 充值读写器一票通PSAM
	RECHARGE_RW_YKT_ISAM		= 0x03,// 充值读写器一卡通ISAM	
	RECHARGE_RW_YKT_PSAM		= 0x04,// 充值读写器一卡通PSAM
	CARD_RW_YPT_ISAM				= 0x05,// 票卡读写器一票通ISAM
	CARD_RW_YPT_PSAM				= 0x06,// 票卡读写器一票通PSAM
	CARD_RW_YKT_ISAM				= 0x07,// 票卡读写器一卡通ISAM
	CARD_RW_YKT_PSAM				= 0x08,// 票卡读写器一卡通PSAM
	TOKEN_RW_YPT_ISAM				= 0x09,// Token读写器一票通ISAM
	TOKEN_RW_YPT_PSAM			= 0x0A,// Token读写器一票通PSAM
	TOKEN_RW_YKT_ISAM				= 0x0B,// Token读写器一卡通ISAM
	TOKEN_RW_YKT_PSAM			= 0x0C// Token读写器一卡通PSAM
}SAM_POSITION;

// SAM卡签到签退状态（预留）
typedef enum _auth_type{
	LOGON_IN        = 0x01,		// 签到
	LOGON_OUT       = 0x02,		// 签退
	DISCONNECT_AUTH = 0x03       // 离线认证
}AUTH_TYPE;

// 当前生效CD版本
typedef enum _cd_version{
	VERSION_FIRST = 1,
	VERSION_SECOND = 2
}CD_VERSION;

typedef enum{
	SRC_NOT_BUT_NEW_HAVE = 0,	//更换票箱之前没有票箱，之后安装上新的票箱
	SRC_HAVE_BUT_NEW_NOT,		//更换票箱之前存在票箱，之后没有安装上新的票箱， 即卸载票箱，未安装新票箱。
	SRC_NEW_SAME,				//更换票箱之前存在票箱，之后安装上票箱与原来的一样, 即票箱没有更换
	SRC_NEW_DIFFER,				//更换票箱之前存在票箱，之后安装上票箱与原来的不一样， 即更换了票箱。
	SRC_NEW_ALL_NOT,			//更换前后都不存在票箱。
	SRC_NEW_DIFFER_NUM			//更换前后，数量不一致
} OP_BOX_STATUS;

typedef enum{
	UPLOAD_COMPLETE    = 0x0001, // 文件上传结束
	PARAMETER_COMPLETE = 0x0002, // 参数更新完成
	DAY_AUDIT_COMPLETE = 0x0003, // 日传输审计结束
	DATA_SYNC_COMPLETE = 0x0004  // 上位同步数据完成
}NOTICE_TYPE;


//--------------------------------------以下是交易数据体结构定义-----------------------------------------//
// ACC设备寄存器数据
typedef struct _txn_audit_register{
	AfcSysHdr_t afcSysHdr;			// AFC系统专业头
	SysComHdr_t sysComHdr;			// 系统公共头
	_txn_audit_register(){
	}
}TXN_AUDIT_REGISTER;

// CD版本切换
typedef struct _txn_event_cd_version_status{
	AfcSysHdr_t afcSysHdr;			// AFC系统专业头
	SysComHdr_t sysComHdr;			// 系统公共头
	DWORD cdGenerationId;
	DWORD numberOfEntries;
	DWORD system_params;              
	DWORD service_provider_params;
	DWORD card_actionlist;
	DWORD product_actionlist;
	DWORD sam_hotlist;
	DWORD calendars;
	DWORD fare_tables;
	DWORD fare_patterns;
	DWORD fare_codes;
	DWORD day_types;
	DWORD location_names;
	DWORD product_type_params;
	DWORD participant_params;
	DWORD mifare_params;
	DWORD high_security_blacklist;
	DWORD locations;
	DWORD business_rules;
	DWORD card_batch_withdraw_list;
	_txn_event_cd_version_status(){
		cdGenerationId = 0;
		numberOfEntries = 0;
		system_params = 0;
		service_provider_params = 0;
		card_actionlist = 0;
		product_actionlist = 0;
		sam_hotlist = 0;
		calendars = 0;
		fare_tables = 0;
		fare_patterns = 0;
		fare_codes = 0;
		day_types = 0;
		location_names = 0;
		product_type_params = 0;
		participant_params = 0;
		mifare_params = 0;
		high_security_blacklist = 0;
		locations = 0;
		business_rules = 0;
		card_batch_withdraw_list = 0;
	}
}TXN_EVENT_CD_VERSION_STATUS;

// 设备状态
typedef struct _txn_event_device_status
{
	AfcSysHdr_t afcSysHdr;			// AFC系统专业头
	SysComHdr_t sysComHdr;			// 系统公共头
	BYTE deviceStatus;				// 设备状态  1：In service；2：Out of service；255：未定义。
	_txn_event_device_status(){
		deviceStatus = 0;
	}
}TXN_EVENT_DEVICE_STATUS;

// 设备标识
typedef struct _txn_event_device_identification
{
	AfcSysHdr_t afcSysHdr;			// AFC系统专业头
	SysComHdr_t sysComHdr;			// 系统公共头
	_txn_event_device_identification(){
	}
}TXN_EVENT_DEVICE_IDENTIFICATION;

// 废票回收交易
typedef struct _invalid_ticket_recycletrade{
	TransComHdr_t transComHdr;
	DWORD cardSerialNo;			// 4 BIN 卡序列号
	BYTE cardPhysicalType;      // 1 BIN 卡物理类型
	DWORD issueId;			    // 4 BIN 卡发行商
	WORD productType;			// 2 BIN 车票产品类型
	BYTE reson;                 // 1 BIN 废卡产生原因
	CCTLOGCountInfo::TICKETBOX_ID magazineId;     // 4 1BCD+1BIN+2BIN 票箱ID
	_invalid_ticket_recycletrade()
	{
		cardSerialNo = 0;
		cardPhysicalType = 0;
		issueId = 0;
	}
}INVALIDE_TICKET_RECYCLETRADE;

// 操作员密码修改请求
typedef struct _change_staffpassword
{
	DWORD StaffID;        // 待修改密码的员工ID
	CString sOldPassword;    // 旧密码
	CString sNewPassword;    // 修改后密码
	BYTE ciperOldPwd[6];
	BYTE ciperNewPwd[6];
	_change_staffpassword()
	{
		StaffID = 0;
		sOldPassword = "";
		sNewPassword = "";
		memset(ciperOldPwd, 0, 6);
		memset(ciperNewPwd, 0, 6);
	}
}CHANGE_STAFFPASSWORD;

// 参数和程序更新（业务数据）
typedef struct _parameter_update{
	WORD parameterId;  // 参数和程序ID	2	BCD	按从小到大的顺序排列
	WORD providerId;   // 设备供应商ID 	2	BIN	传输时需要转换成INTEL序。 对程序有效，对参数无效，无意义时填全0
	WORD softwareNo;   // 软件批次号 	2	BIN	传输时需要转换成INTEL序。对程序有效，对参数无效，无意义时填全0
	BYTE versionType;  // 版本类型	1	BIN	0x00正式参数当前版本；0x01正式参数将来版本；0x10测试参数当前版本；	0x11测试参数将来版本
	DWORD oldVersion;  // 旧版本号	4	BIN	传输时需要转换成INTEL序。 切换前的参数/程序文件的版本号
	DWORD oldStartDate;// 生效日期	4	BCD	旧版本的生效日期，YYYYMMDD
	DWORD newVersion;  // 新版本号	4	BIN	传输时需要转换成INTEL序。 切换后参数/程序文件的版本号
	DWORD newStartDate;// 生效日期	4	BCD	新版本的生效日期，YYYYMMDD
	_parameter_update()
	{
		parameterId = 0;
		providerId = 0;
		softwareNo = 0;
		versionType = 0;
		oldVersion = 0;
		oldStartDate = 0;
		newVersion = 0;
		newStartDate = 0;
	}
}PARAMETER_UPDATE;

// 参数和程序下载（业务数据）
typedef struct _parameter_download{
	WORD parameterId;    // 2 BCD 参数和程序ID 按从小到大的顺序排列
	WORD providerId;     // 2 BIN 设备供应商ID
	WORD softwareNo;     // 2 BIN 软件批次号
	BYTE versionType;    // 1 BIN 版本类型 0x00正式参数当前版本；0x01正式参数将来版本；0x10测试参数当前版本；	0x11测试参数将来版本
	DWORD version;       // 4 BIN 版本
	_DATE_TIME DateTime; // 7 BCD 下载时间
	BYTE file[20];       // 20 ASCII 文件名称
	BYTE result;         // 1 BIN 下载结果 0x00下载成功；0x01下载失败；0x02无法解析；0x03其它错误

	_parameter_download(){
		parameterId = 0;
		providerId = 0;
		softwareNo = 0;
		versionType = 0;
		version = 0;
		result = 0;
		memset(file, 0x00, 20);
	}
}PARAMETER_DOWNLOAD;


// 操作日志
typedef struct _operator_log{
	CString csUserID;
	OPERATION_CODE operCode;    // 操作代码	2  1BIN+1BIN
	OPERATION_LEVEL operLevel;	// 操作级别	1  BIN
	BYTE result;	            // 操作结果	1  BIN
	BYTE operDetail[32];	    // 操作内容	32 ASCII
	BYTE operTip[32];	        // 备注	    32 ASCII
	_operator_log(){
		csUserID = "";
		result  = 0;
		memset(operDetail, 0x00, 32);
		memset(operTip, 0x00, 32);
	}
}OPERATOR_LOG;

// 设备故障数据
typedef struct _device_errordata{
	BYTE partType;					  // 1 BIN 设备部件类型
	BYTE  bDeviceType;            // 1 BIN 部件类型                          
	BYTE devicePartID[3];         // 3 BIN 部件ID
	WORD deviceDeviderId;         // 2 BIN 设备供应商ID
	BYTE maintanCode[4];	          // 4 1BIN+1BIN+1BIN+1BIN 故障代码
	BYTE ecode[2];	                  // 2 1BIN+1BIN 维修代码
	_device_errordata()
	{
		memset(devicePartID, 0, 3);
		memset(maintanCode, 0, 4);
		memset(ecode, 0, 2);
	}
}DEVICE_ERRORDATA;

// 票箱交换
typedef struct _magazine_change{
	TICKETBOX_OPERATION_TYPE operType;             // 1 BIN 操作类型
	CCTLOGCountInfo::TICKETBOX_ID magazineId;              // 4 1BCD+1BIN+2BIN 票箱ID
	MAGAZINE_PALACE magazne;			 // 1 BIN 票箱位置ID
	BYTE  cardType;						 // 1 BIN 票卡物理类型
	DWORD providerId;					 // 4 BIN 票卡发行商ID
	WORD cardProductType;				 // 2 BIN 车票产品种类
	BYTE preProperty;					 // 1 BIN 预赋值属性
	BYTE childproductId;                 // 1 BIN 衍生产品ID
	WORD CardCount;                      // 2 BIN 票卡数量
	SC_MAGAZINE_STATUS magazineStatus;      // 1 BIN 票箱状态
	_magazine_change()
	{
		cardType = 0;
		providerId = 0;
		cardProductType = 0;
		preProperty = 0;
		childproductId = 0;
		CardCount = 0;
	}
}MAGAZINE_CHANGE;

// TVM/AVM故障单业务数据
typedef struct _tvm_avm_business_failed_data{
	DEVICE_ID	DeviceID;
	_DATE_TIME	DateTime;
	DWORD		DeviceAfcNum;
	DWORD		TransactionNum;
	DWORD		RefoundAmount;
	BYTE		FailedReason[12];
	
	_tvm_avm_business_failed_data(){
		DeviceID = _device_id();
		DateTime = _date_time();
		DeviceAfcNum = 0;
		TransactionNum = 0;
		RefoundAmount = 0;
		memset(FailedReason,0,sizeof(FailedReason));
	}

}BUSINESS_FAILED_DATA;

// 钱箱ID（纸币，硬币）
typedef struct cash_box_id
{
	BYTE  btLineCode;
	BYTE  btType;
	WORD  wNum;
	cash_box_id()
	{
		btLineCode  = 0xFF;
		btType      = 0xFF;
		wNum       = 0xFFFF;
	}
	void Serialze(LPBYTE lpData) const
	{
		if(btLineCode!=0xFF)
		{
			int2BCD(btLineCode,(char*)lpData,1);
		}
		else
		{
			*lpData = btLineCode;
		}
		lpData++;
		*lpData = btType;
		lpData++;
		memcpy(lpData,&wNum,2);
		lpData+=2;
	}
	void Deserialize(LPBYTE lpData)
	{
		if(*lpData == 0xFF)
		{
			btLineCode = *lpData;
		}
		else
		{
			btLineCode = BCD2int((char*)lpData,1);
		}
		lpData++;
		btType = *lpData;
		lpData++;
		memcpy(&wNum,lpData,2);
		lpData+=2;
	}
	CString ToString()
	{
		CString str;
		str.Format(_T("%02X%02X%04X"),(BYTE)char2BCD(btLineCode),btType,wNum);
		return str;
	}
	bool operator == (const cash_box_id& another) const
	{
		if(btLineCode != another.btLineCode) return false;
		if(btType != another.btType) return false;
		if(wNum != another.wNum) return false;
		return true;
	}
} CASH_BOX_ID;//
////钱箱位置
//typedef enum  cash_box_position
//{
//	BANKNOTE_COLL_POS		=  0x01, 
//	COIN_CHANGE_BOX_A_POS,
//	COIN_CHANGE_BOX_B_POS,
//	COIN_HOPPER_BOX_POS,
//	COIN_COLL_BOX_POS,
///*	BANKNOTE_INVALID_POS,*/
//	BANKNOTE_CHANGE_BOX_POS,
//	BANKNOTE_RECYCLE_BOX_A_POS,
//	BANKNOTE_RECYCLE_BOX_B_POS,
//	BANKNOTE_RECYCLE_BOX_C_POS,
//	BANKNOTE_RECYCLE_BOX_D_POS,
//}CASH_BOX_POSITION;

//钱箱状态
//typedef enum _cash_box_state
//{
//	NORMAL_INSTALL    = 0x01, //正常安装
//	ILLEGAL_INSTALL   = 0x02, //非法安装
//	NORMAL_UNINSTALL  = 0x03, //正常卸载
//	ILLEGAL_UNINSTALL = 0x04, //非法卸载
//	UNKNOW_STATE      = 0xFF  //未知状态
//}CASH_BOX_STATE;

//钱币代码 + 钱币数量
typedef struct cash_type_count 
{
	BankNoteAndCoinType_t cash_type; //币种代码 1BIN
	WORD                  cash_count;//币种数量 2BIN
	cash_type_count(){
		cash_type  = VALUE_UNKNOWN;
		cash_count = 0;
	}
}CASH_TYPE_COUNT;

//钱箱操作类型
typedef enum  _handle_type
{
	INSTALL     = 0x01,//安装
	UNINSTALL   = 0x02,//卸载
	ADD         = 0x03,//补充
	BOX_RECYCLE     = 0x04 //回收
}BOX_HANDLE_TYPE;

// 4.2.16   钱箱更换业务数据
typedef struct cashbox_change
{
	BOX_HANDLE_TYPE       handleType;  //1 操作类型 1BIN
	BYTE COIN_BANKNOTE_BOX_ID[4];  //4 钱箱ID   1BCD+1BIN+2BIN
	BYTE       position;  //1 钱箱位置 1BIN
	SC_MAGAZINE_STATUS         State;  //1 钱箱状态 1BIN
	BYTE                     btCount;  //1 币种总数
	vector<CASH_TYPE_COUNT>  vecCash;  //币种代码+币种数量

	cashbox_change()
	{
		
	}
	
	// 硬币Hopper箱ID
	void SetHopperBoxID(BYTE btLineCode, BYTE btType, BYTE bStationCode, BYTE wNum)
	{
		COIN_BANKNOTE_BOX_ID[0] = btLineCode;
		COIN_BANKNOTE_BOX_ID[1] = btType;
		COIN_BANKNOTE_BOX_ID[2] = bStationCode;
		COIN_BANKNOTE_BOX_ID[3] = wNum;
	}

	//拼凑钱箱ID
	void SetBoxID(BYTE btLineCode, BYTE btType,BYTE bNumInGrop, BYTE bNum)
	{
		COIN_BANKNOTE_BOX_ID[0] = /*char2BCD*/(btLineCode);
		COIN_BANKNOTE_BOX_ID[1] = /*char2BCD*/(btType);
		COIN_BANKNOTE_BOX_ID[2] = /*char2BCD*/(bNumInGrop);	//(wNum >>0x08) & 0x00ff;
		COIN_BANKNOTE_BOX_ID[3] = /*char2BCD*/(bNum);	//wNum & 0x00ff;
	}
	void Serialze(LPBYTE lpData) const
	{
		*lpData = COIN_BANKNOTE_BOX_ID[0];
		lpData++;
		*lpData = COIN_BANKNOTE_BOX_ID[1];
		lpData++;
		*lpData = COIN_BANKNOTE_BOX_ID[2];
		lpData++;
		*lpData = COIN_BANKNOTE_BOX_ID[3];
		lpData++;

	}

	//处理纸币箱空箱安装
	void BanNoteBox_Initial()
	{
		CASH_TYPE_COUNT cash;
		cash.cash_type = Banknote1Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote5Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote10Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote20Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote50Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Banknote100Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
	}

	//空硬币箱
	void CoinBox_Initial()
	{
		CASH_TYPE_COUNT cash;
		cash.cash_type = CoinHalfYuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Coin1Yuan;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		/*cash.cash_type = Coin5MOP;
		cash.cash_count = 0;
		vecCash.push_back(cash);
		cash.cash_type = Coin50AVOS;
		cash.cash_count = 0;
		vecCash.push_back(cash);*/
	}

	//获取币种总数量
	BYTE GetSumCurrency()
	{
		int nSum = 0;
		vector<CASH_TYPE_COUNT> ::iterator iter = vecCash.begin();
		while (iter != vecCash.end())
		{
			if((*iter).cash_count > 0)
			{
				nSum++;
				iter++;
			}
			else{
				vecCash.erase(iter);// 数量为空不上报。
			}			
		}
		//LPBYTE lpSum;
		//memcpy(lpSum,&nSum,1);
		BYTE  bSum = (BYTE) (0xff & nSum);
		return bSum;
	}

}CASHBOX_CHANGE;


// ISAM签到、签退
typedef struct _isam_auth
{
	BYTE samid[6];        // 6 BCD ISAM卡号
	DWORD participantId;  // 4 BIN 参与方ID
	AUTH_TYPE auth_type;  // 1 BIN 动作状态
	BYTE authResult;      // 1 BIN 认证结果
	DWORD amount;         // 4 BIN 授权额度
	_DATE_TIME datetime;  // 7 BCD 授权失效时间
	_isam_auth()
	{
		memset(samid, 0x00, sizeof samid);
		participantId = 0;
		authResult = 0;
		amount = 0;
	}
}ISAM_AUTH;

typedef struct _Personalization
{
	char cardHolderTitle[5];	//持卡人头衔
	char cardHolderName[61];	//持卡人姓名
	BYTE languagePreference;	//持卡人选定的写入票卡的语言
	_DATE dateOfBirth;			//生日
	char cardholderPhoneDay[21];//持卡人日间电话
	char cardholderPhoneHome[21];//持卡人家庭电话
	char cardholderEmail[129];	//持卡人电子邮件
	char cardholderClue[21];	//持卡人密码提示
	char cardholderPassword[16];//持卡人密码
	char staffID[30];			//持卡人操作员工
	BYTE sex;					//性别 1:Male 2:Female
	BYTE photePrinted;			//标明卡面是否已印制照片 1:已印制; 2:未印制
	char address1[129];			//详细地址1
	char address2[129];			//详细地址2
	char cardholderNumber[19];	//持卡人提供的个人身份
	BYTE cardholderNumberType;	//定义持卡使用的个人身份类型	
	DWORD cardSerialNumber;	//持卡人提供的个人身份逻辑卡号	

	_Personalization()
	{
		memset(cardHolderTitle,0,sizeof(cardHolderTitle));
		languagePreference = 0;
		memset(cardHolderName,0,sizeof(cardHolderName));
		memset(cardholderPhoneDay,0,sizeof(cardholderPhoneDay));
		memset(cardholderPhoneHome,0,sizeof(cardholderPhoneHome));
		memset(cardholderEmail,0,sizeof(cardholderEmail));
		memset(cardholderClue,0,sizeof(cardholderClue));
		memset(cardholderPassword,0,sizeof(cardholderPassword));
		memset(staffID,0,sizeof(staffID));
		sex = 0;
		photePrinted = 0;
		memset(address1,0,sizeof(address1));
		memset(address2,0,sizeof(address2));
		memset(cardholderNumber,0,sizeof(cardholderNumber));
		cardholderNumberType = 0;
		cardSerialNumber = 0;
	}

}Personalization,PersonalizationQuery;

typedef struct _HistoryTranceInfo
{
	_DATE_TIME TransactionDateTime;
	WORD TransactionStation;
	DWORD TransactionDevice;
	BYTE TransactionType;
	DWORD TransactionValue;
	DWORD RemainingValue;
	BYTE PaymentMethod;
	DWORD PaymentValue;

	_HistoryTranceInfo()
	{
		TransactionStation = 0;
		TransactionDevice = 0;
		TransactionType = 0;
		TransactionValue = 0;
		RemainingValue = 0;
		PaymentMethod = 0;
		PaymentValue = 0;
	}
	bool operator < (const _HistoryTranceInfo& another) const
	{
		if(TransactionDateTime < another.TransactionDateTime) return true;
		return false;
	}
	bool operator > (const _HistoryTranceInfo & another) const
	{
		if(TransactionDateTime > another.TransactionDateTime) return true;		
		return false;
	}
	bool operator == (const _HistoryTranceInfo& another) const
	{
		if(TransactionDateTime == another.TransactionDateTime) return true;		
		return false;
	}
}HistoryTranceInfo;

typedef struct _HistoryProductInfo
{
	DWORD cardIssuerId;	        // 发卡方ID
	DWORD cardSerialNumber;	    // 逻辑卡号
	WORD ProductType;           // 产品类型
	WORD ProductVariantType;    // 产品子类型
	DWORD TransactionCount;
    vector<HistoryTranceInfo> TransactionRecord;

	_HistoryProductInfo()
	{
		cardIssuerId = 0;
		cardSerialNumber = 0;
		ProductType = 0;
		ProductVariantType = 0;
		TransactionCount = 0;
		TransactionRecord.clear();
	}
}HistoryProductInfo;

typedef struct _HistoryQueryProductType
{
	WORD ProductType;           // 产品类型
	WORD ProductVariantType;    // 产品子类型

	_HistoryQueryProductType()
	{
		ProductType = 0;
		ProductVariantType = 0;
	}
}HistoryQueryProductType;

typedef struct _HistoryInfoQuery
{
	DWORD cardIssuerId;	        // 发卡方ID
	DWORD cardSerialNumber;	    // 逻辑卡号
	_DATE_TIME StartDate;		// 7 BCD 交易起始日期
	_DATE_TIME EndDate;			// 7 BCD 交易结束日期，	EndDate – Start <=31天
	DWORD ProdutCount;			// 票卡产品个数
    _HistoryQueryProductType ProductInfo[10];

	_HistoryInfoQuery()
	{
		cardIssuerId = 0;
		cardSerialNumber = 0;
		ProdutCount = 0;
	}
}HistoryInfoQuery;

// 部件组成
typedef struct _component{
	BYTE  bDeviceType;   // 1 BIN 设备部件类型
	DWORD dwCompoentID;  // 3 BIN 部件序号
}COMPONENT;


// 工作模式定义
typedef enum _work_mode_{
	WORK_MODE_BANKNOTE_ONLY			= 0x20,//只收纸币模式
	WORK_MODE_COIN_ONLY				= 0x21,//只收硬币模式
	WORK_MODE_NO_CHANGE				= 0x22,//无找零模式
	WORK_MODE_SVT_ONLY				= 0x23,//只储值卡售票模式
	WORK_MODE_NO_ISSUE				= 0x24,//无售票模式
	WORK_MODE_ANALY_ONLY			= 0x25,//只验卡模式
	WORK_MODE_NO_SVT				= 0x26,//不能接收储值卡模式
	WORK_MODE_NO_COIN_ACCEPT		= 0x27,//暂不收硬币模式
	WORK_MODE_NO_BANKNOTE_ACCEPT	= 0x28,//暂不收纸币模式
	WORK_MODE_NORMALLY				= 0x2F,//正常模式
}WORK_MODE;
