#pragma once

#include "SCAPI.h"
#include "CTLOGCountInfo.h"


// BOMaudit标志
typedef enum _audit_flag
{
	OPERATER_LOGOUT  = 0x01,     // 操作员登出
	OPERATER_DAY     = 0x02,     // 操作员结算
	END_OF_DAY       = 0x03      // 日结 
}AUDIT_FLAG;

typedef enum _msg_code_transation
{
	MSG_TRANS_BOM_AR = 0x2402,//6.2.1.2BOM审计报文数据定义
	MSG_TRANS_BOM_REFOUND_TVM = 0x5001,//6.3.1BOM对TVM故障退款
	MSG_TRANS_RECYLE_INVALID_CARD = 0x5005,//6.3.5废票回收交易
	MSG_TRANS_BOM_NONBUSINESS_INCOME = 0x5007,//6.3.5BOM非营业性收支
	MSG_TRANS_NONIMMIDATELY_SURRENDER_APPLY = 0x0234,
	MSG_TRANS_NONIMMIDATELY_SURRENDER = 0x0235,
	MSG_TRANS_SIGNCARD_APPLY = 0x0237,
	MSG_TRANS_SIGNCARD_MODIFY = 0x0238,
	MSG_TRANS_REPORT_LOSS = 0x0239,
	MSG_TRANS_CANCEL_REPORT_LOSS = 0x0240,
	MSG_TRANS_ADMINISTRATION_PROCESS = 0x0241,
	MSG_TRANS_AUDIT = 0x0460,
} MSG_CODE_TRANS;

// 业务数据命令代码
typedef  enum _msg_code_bussiness{
	MSG_BUSSINESS_SOFTANDPROGRAM_UPDATE			 = 0x5102,   // 6.4.2  参数和程序更新
	MSG_BUSSINESS_SOFTANDPROGRAM_DOWNLOAD		 = 0x5103,   // 6.4.3  参数和程序下载业务数据
	MSG_BUSSINESS_DEVICE_IMPORTANT_STATUS_CHANGE = 0x5104,   // 6.4.4  设备重要状态改变	
	MSG_BUSSINESS_ISAM_LOGIN_LOGOUT              = 0x5105,   // 6.4.5  ISAM签到/签退
	MSG_BUSSINESS_OPERATING_LOG					 = 0x5106,	 // 6.4.6  操作日志
	MSG_BUSSINESS_DEVICE_ERROR					 = 0x5107,   // 6.4.7  设备故障数据
	MSG_BUSSINESS_DEVICE_REGISTER				 = 0x5108,   // 6.4.8  设备寄存器数据
	MSG_BUSSINESS_TICKETBOX_CHANGE				 = 0x510C,	 // 6.4.10 票箱更换
	MSG_BUSSINESS_DEVICE_COMPONENT				 = 0x5114,	 // 6.4.11 设备部件构成
	MSG_BUSSINESS_ACCOUNT_LOCK					 = 0x5201,   // 6.5.1  账户锁定
	MSG_BUSSINESS_UDSN_RESET					 = 0x5204,   // 6.5.2  设备流水重置
	MSG_BUSSINESS_DEVICE_COMPONENT_CHANGE		 = 0x5205,   // 6.5.3  部件更换
	MSG_BUSSINESS_MAGAZINE_INFO					 = 0x5207,   // 6.5.4  票箱库存报告
	MSG_BUSSINESS_BOM_AUDIT                      = 0x520B,   // 6.5.8  BOM审计数据
	MSG_BUSSINESS_PACKAGE_AUDIT					 = 0x520D,	 // 6.5.10 包传输审计数据	

	MSG_BUSSINESS_NONIMMIDATELY_SURRENDER_APPLY = 0x0234,
	MSG_BUSSINESS_NONIMMIDATELY_SURRENDER_APPLY_QUERY = 0x0244,
	MSG_BUSSINESS_NONIMMIDATELY_SURRENDER = 0x0235,
	MSG_BUSSINESS_SIGNCARD_APPLY_QUERY = 0x0246
}MSG_CODE_BUSSINESS;

// 控制数据---命令数据代码
typedef  enum _msg_code_control_command{
	MSG_SPECIFY_BLOCKNO_DATA               = 0x5201,   // 6.7.1.1 上传指定包编号数据命令
	MSG_SPECIFY_TIME_DATA                  = 0x5202,   // 6.7.1.2 上传指定时间数据命令
	MSG_DEBUG_DATA                         = 0x5203,   // 6.7.1.3 上传设备调试数据
	MSG_OPERATION_DATA                     = 0x5204,   // 6.7.1.4 运营模式控制命令
	MSG_DEVICE_CONTROL_COMMAND             = 0x5205,   // 6.7.1.5 设备运行控制命令
	MSG_24HOUR_OPERATION_DATA              = 0x5206,   // 6.7.1.6 24小时运营控制命令
	MSG_DELAY_OPERATION_DATA               = 0x5207,   // 6.7.1.7 延长运营时间命令
	MSG_FORCE_TIME_SYNC                    = 0x5208,   // 6.7.1.8 强制时钟同步命令
	MSG_ISSUE_RESTRICT                     = 0x520A,   // 6.7.1.9 售票限制命令
	MSG_UPLOAD_PARAMETER_ID           = 0x520C,   // 6.7.1.10 上传参数和程序版本
	MSG_UPDATE_PARAMETER                   = 0x520D,   // 6.7.1.11 参数和程序更新命令
	MSG_FORCE_LOGOUT                       = 0x5211,   // 6.7.1.12 强制退出登录
	MSG_CHANGE_PARAMETER_ID           = 0x5212,   // 6.7.1.13 参数和程序版本切换命令
	MSG_REQUEST_DEVICE_STATUS              = 0x5213,   // 6.7.1.14 设备状态查询
	MSG_AUTORUN_PARAMETER                  = 0x5215    // 6.7.1.16 自动运行参数启用控制
}MSG_CODE_CONTROL_COMMAND;

// 控制数据---请求数据代码
typedef  enum _msg_code_control_request{
	MSG_REQUEST_LOGIN_OR_LOGOUT            = 0x5101,   // 6.7.2.1 登录登出请求
	MSG_REQUEST_REFORM_PASSWORD            = 0x5103,   // 6.7.2.3 密码修改请求
	MSG_REQUEST_SEND_IMIDIATLY_PARAMETER   = 0x5107,   // 6.7.2.6 即时生效参数下发请求
	MSG_REQUEST_CONNECT_AUTH               = 0x5109,    // 6.7.2.8 连接认证请求
}MSG_CODE_CONTROL_REQUEST;

// 控制数据---通知数据代码
typedef  enum _msg_code_control_notic{
	MSG_ACCOUNT_UNLOCK_NOTIC               = 0x5502,		// 6.7.3.2 账户解锁通知
	MSG_FTP_CONFIG_NOTIC                   = 0x550C,				// 6.7.3.11 FTP配置信息通知
	MSG_OP_END_NOTIC                       = 0x550E,						// 6.7.3.13 操作结束通知
	MSG_SIGNCARD_APPLY_RESULT = 0x0247,				
	MSG_NONIMMIDATELY_SURRENDER_APPLY_RESULT  = 0x0245,
}MSG_CODE_CONTROL_NOTIC;

// 控制数据---反馈数据代码
typedef  enum _msg_code_control_reply{
	MSG_REQUEST_LOGIN_OR_LOGOUT_REPLY      = 0x5101,   // 6.7.4.1 登录登出请求反馈
	MSG_MSG_REFORM_PASSWORD_REPLY          = 0x5103,   // 6.7.4.3 密码修改请求反馈
	MSG_REQUEST_IMIDIATLY_PARAMETER_REPLY  = 0x5107,   // 6.7.4.4 即时生效参数下发请求反馈
	MSG_REQUEST_CONNECT_AUTH_REPLY         = 0x5109,   // 6.7.4.6 连接认证请求反馈
	MSG_PARAMETER_ID_DATA             = 0x5401    // 6.7.4.8 设备参数和程序版本数据
}MSG_CODE_CONTROL_REPLY;

// 状态数据代码
typedef  enum _msg_code_status{
	MSG_BOM_ALL_STATUS_DATA                = 0x3101,   // 6.6.2 BOM完整状态数据
	MSG_BOM_STATUS_CHANGE_DATA             = 0x3102,   // 6.6.7 设备变更状态数据
	MSG_BOM_ERROR_NOTIC                    = 0x3107    // 6.6.8 设备故障状态数据
}MSG_CODE_STATUS;

#define LEN_AFC_SYS_HEADER 10
#define  LEN_TXN_DATATYPE 17
#define LEN_PACKAGE_HEADER 28
//6.3AFC内部交易
//6.3.1BOM对TVM退款
#define  LEN_TXN_BOM_REFUND_TVM (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+19)
//6.3.2非即时退卡申请
#define  LEN_TXN_DELAY_REFUND (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+89)
//6.3.5废票回收交易
#define  LEN_TXN_INVALID_TICKET_RECYCLE (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+16)
//6.3.6BOM非营业性收支
#define  LEN_TXN_BOM_NONBUSINESS_INCOME (LEN_AFC_SYS_HEADER+LEN_TXN_DATATYPE+84)
#define LEN_TXN_SYSCOM_HDR 42
#define LEN_TXN_TICKETCOM_HDR 17
#define LEN_TXN_OPERATORCOM_HDR  4
#define LEN_TXN_FINCOM_HDR 13
#define LEN_TXN_SERCOM_HDR  5
#define LEN_TXN_NONIMMIDATELY_SURRENDER (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR +LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 15 +LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_MODIFY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 375+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 42+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_CANCEL_REPORT_LOSS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR+ 42+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_ADMINISTRATION_PROCESS (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR +LEN_TXN_TICKETCOM_HDR+LEN_TXN_FINCOM_HDR + LEN_TXN_OPERATORCOM_HDR+ 22+LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 105 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 31 + LEN_TXN_SERCOM_HDR)
#define LEN_TXN_SIGNCARD_APPLY_QUERY (LEN_AFC_SYS_HEADER + LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR + LEN_TXN_OPERATORCOM_HDR + 21 + LEN_TXN_SERCOM_HDR)

#define LEN_BUSINESS_HEADER 32
#define INX_BUSINESS_DATATYPE (LEN_BUSINESS_HEADER-7)
#define INX_BUSINESS_HEADER_LENGTH (LEN_BUSINESS_HEADER-5)
#define LEN_BUSINESS_HEADER_DATATYPE (LEN_BUSINESS_HEADER-5)
//6.4业务数据
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
#define LEN_EVN_TICKETBOX_INFO (LEN_BUSINESS_HEADER + 17)



//6.6状态数据
#define LEN_STATUS_HEADER 11
#define LEN_STATUS_SINGLE_MODE (LEN_STATUS_HEADER + 2 + 4 * 1) // 状态数据长度(控制模式/24小时运营/延长模式
#define LEN_STATUS_THREE (LEN_STATUS_HEADER + 2 + 4*3) // 控制模式 + 24小时运营 + 延长模式
#define LEN_STATUS_FULL (LEN_STATUS_HEADER + 2 + 4 * 24 )  // 全状态数据
#define LEN_STATUS_HAS_ERROR (LEN_STATUS_HEADER + 7) // 异常状态电文长度
#define LEN_STATUS_CHANGE (LEN_STATUS_HEADER+4) //6.6.6设备状态变更数据

#define LEN_REQ_HEADER 2
//6.7.2请求数据
//6.7.2.1登陆登出
#define LEN_REQ_LOGINLOGOFF  (LEN_REQ_HEADER+12)
//6.7.2.3密码修改
#define LEN_REQ_CHANGE_PASSWORD (LEN_REQ_HEADER+17)
//6.7.2.5参数修改
//6.7.2.6即时生效参数下发请求
#define LEN_REQ_SEND_IMMIDIATLY_PARAMETER (LEN_REQ_HEADER+6)
//6.7.2.8连接认证请求
#define  LEN_REQ_CONNECT_AUTH (LEN_REQ_HEADER+11)

//6.7.3通知数据
//6.7.3.13操作结束通知
#define  LEN_NOTICE_OPERATION_END 4

//6.7.4反馈数据
#define LEN_REPLY_PARAMETER_VERSION (10+15*40)

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
	OPERATION_LOGIN          = 0x1001, // 登录
	OPERATION_LOGOUT         = 0x1002, // 登出
	OPERATION_INPUT_PARAM    = 0x1101, // 导入参数和程序
	OPERATION_OUTPUT_PARAM   = 0x1102, // 导出参数和程序
	OPERATION_INPUT_DEAL     = 0x1103, // 导入交易数据
	OPERATION_OUTPUT_DEAL    = 0x1104, // 导出交易数据
	OPERATION_INPUT_OPER     = 0x1105, // 导入业务数据
	OPERATION_OUTPUT_OPER    = 0x1106, // 导出业务数据
	OPERATION_INPUT_DEBUG    = 0x1107, // 导入调试数据
	OPERATION_OUTPUT_DEBUG   = 0x1108, // 导出调试数据
	OPERATION_FORCE_TIME     = 0x1411, // 强制时钟同步
	OPERATION_MANUAL_TIME    = 0x1412, // 手工时钟同步
	OPERATION_24HOUR_DELAY   = 0x1701, // 24小时运营和延长运营设置
	OPERATION_DEBUG_UPLOAD   = 0x1702, // 调试日志上传
	OPERATION_PARAM_EDIT     = 0x1A01, // 参数编辑(编辑、发布、下发）
	OPERATION_MODE_SET       = 0x1901, // 模式设定（人工设置设备模式）
	OPERATION_MODE_NOTIC     = 0x1902, // 模式通知设定
	OPERATION_TIME_WARNING   = 0xA101, // 时钟同步警告
	OPERATION_TIME_ERROR     = 0xA102, // 时钟同步异常
	OPERATION_CONNECT_STATUS = 0xA103  // 连接状态
}OPERATION_CODE;

// 操作级别
typedef enum _operation_level{
	OPERATION_ONE_LEVEL   = 0x01, // 普通操作
	OPERATION_TWO_LEVEL   = 0x02, // 警告信息 
	OPERATION_THREE_LEVEL = 0x03, // 重要操作
	OPERATION_FOUR_LEVEL  = 0x04, // 故障信息
	OPERATION_FIVE_LEVEL  = 0x05  // 危险操作
}OPERATION_LEVEL;



// 票箱位置
typedef enum _magazine_palace{
	MAGAZINEA  = 0x01,  // A票箱
	MAGAZINEB  = 0x02,   // B票箱
	MAGAZINEC  = 0x03   // 废票箱
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
	VALID_DETACH   = 0x03, // 正常卸下；
	INVALID_DETACH = 0x04  // 非法卸下
}SC_MAGAZINE_STATUS;


// 设备服务状态编码
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

// 运行模式 
typedef enum{
	RUNING       = 0x00, // 正常运行中
	CLOSEING     = 0x01, // 业务终了中
	START_RUNING = 0x02, // 再启动中
	SHUTDOWNING  = 0x03, // 关机中
	WARNING      = 0x04  // 设备报警
}RUN_STATUS;

// 工作模式
typedef enum{
	ISSUE_MODE        = 0x01, // BOM售票模式
	ADJUST_MODE       = 0x02, // BOM补票模式
	ISSUE_ADJUST_MODE = 0x03  // BOM售补票模式
}WORK_MODE;

// 通讯状态
typedef enum{
	DISCONNECT = 0x00,// 通讯中断
	CONNECTED  = 0x01,// 通讯正常
	CONNECTING = 0x02 // 通讯恢复中
}COMMUNICATION;

// 时钟差异
typedef enum{
	VALID_TIME = 0x00, // 时钟正常
	WARN_TIME  = 0x01, // 时钟警告
	ERROR_TIME = 0x02  // 时钟故障
}TIME_STATUS;

// 登录状态
typedef enum{
	LOGIN   = 0x00,	// 有人登录	登录成功后的状态
	LOGOUT  = 0x01,	// 无人登录	退出登录后的状态
	LOCKED  = 0x02	// 账户锁定	账户锁定后的状态，有人登录成功后状态改变
}LOGIN_STATUS;

// 票箱状态
typedef enum{
	STATUS_MAGAZINE_SET = 0x00,           // 票箱安装
	STATUS_MAGAZINE_INVALIDE_ATACH = 0x01,// 票箱非法安装
	STATUS_MAGAZINE_UNSET = 0x02,         // 票箱未安装（卸下）
}STATUS_MAGAZINE;

// 票箱数量状态
typedef enum{
	MAG_NUM_NORMAL     = 0x00,// 票箱正常
	MAG_NUM_EMPTY      = 0x01,// 票箱空
	MAG_NUM_NEAR_EMPTY = 0x02,// 票箱将空
	MAG_NUM_FULL       = 0x03,// 票箱满
	MAG_NUM_NEAR_FULL  = 0x04 // 票箱将满
}MAGAZINE_NUM_STATUS;

// 读写器状态
typedef enum{
	RW_NORMAL    = 0x00, // 读写器正常
	RW_ERROR     = 0x01, // 读写器故障
	RW_LOSS      = 0x02, // 读写器丢失
	RW_SAM_ERROR = 0x03  // 读写器SAM错误
}STATUS_RW;

// SAM类型
typedef enum{
	YPT_ISAM = 0x01,// 一票通ISAM
	YPT_PSAM = 0x02,// 一票通PSAM
	YKT_ISAM = 0x03,// 一卡通ISAM
	YKT_PSAM = 0x04	// 一卡通PSAM
}SAM_TYPE_;

// SAM位置
typedef enum{
	RW_YPT_ISAM = 0x01,// 外部读写器一票通ISAM
	RW_YPT_PSAM = 0x02,// 外部读写器一票通PSAM
	RW_YKT_ISAM = 0x03,// 外部读写器一卡通ISAM	
	RW_YKT_PSAM = 0x04,// 外部读写器一卡通PSAM
	TH_YPT_ISAM = 0x05,// 内部读写器一票通ISAM
	TH_YPT_PSAM = 0x06,// 内部读写器一票通PSAM
	TH_YKT_ISAM = 0x07,// 内部读写器一卡通ISAM
	TH_YKT_PSAM = 0x08,// 内部读写器一卡通PSAM
}SAM_POS_;

// SAM状态
typedef enum{
	ISAM_UNAUTH = 0x00, // ISAM状态未验证
	ISAM_AUTHED = 0x01  // ISAM状态已验证
}SAM_STATUS;

// 设备自动运行状态
typedef enum{
	AUTORUN_STOP  = 0x00, // 设备自动运行状态停用
	AUTORUN_START = 0x01  // 设备自动运行状态启用
}DEVICE_AUTORUN;

// 维修门状态
typedef enum{
	DOOR_NORMAL_OPEN  = 0x00, // 维修门正常打开
	DOOR_NORMAL_CLOSE = 0x01, // 维修门正常关闭
	DOOR_ILLEGLE_OPEN = 0x02, // 维修门非法打开
	DOOR_CLOSE_UNLOCK = 0x03  // 维修门关闭未锁
}DOOR_STATUS;

// 打印机状态
typedef enum{
	PRINTER_NORMAL   = 0x00,  // 打印机正常工作
	PRINTER_ERR      = 0x01,  // 打印机通信故障
	PRINTER_SHUTDOWN = 0x02,  // 打印机断电
	PRINTER_NOPAPER  = 0x03	  // 打印机缺纸
}PRINTER_STATUS;

typedef enum{
	TH_MODULE_NORMAL   = 0x00, // 模块正常
	TH_MODULE_ERR      = 0x01, // 模块故障
	TH_MODULE_SHUTDOWN = 0x02, // 模块连接断开
	TH_MODULE_JAM      = 0x03  // 模块卡票
}TH_MODULE_STATUS;

typedef enum{
	OPENED  = 0x01,	// 门打开着
	CLOSED  = 0x02,	// 门关闭着
	UNKNOWN = 0xFF	// 未知
} DOORSTATUS;

typedef enum _box_no {
	TICKET_BOX1 = 1,                // 票箱A  
	TICKET_BOX2                     // 票箱B
} BOX_NO;

typedef enum 
{
	SRC_NOT_BUT_NEW_HAVE = 0,	//更换票箱之前没有票箱，之后安装上新的票箱
	SRC_HAVE_BUT_NEW_NOT,		//更换票箱之前存在票箱，之后没有安装上新的票箱， 即卸下票箱，未安装新票箱。
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


typedef struct _tagBOM_STATUS_ID
{
	BYTE b1;
	BYTE b2;
	BYTE b3;
} BOM_STATUS_ID;

namespace BOM_STATUS_IDS
{
	const static _tagBOM_STATUS_ID SERVICE_STATUS = {0x01,0x01,0x01};// 服务状态
	const static _tagBOM_STATUS_ID  RUN_STATUS = {0x01,0x01,0x02};             // 运行状态
	const static _tagBOM_STATUS_ID   WORK_MODE = {0x01,0x01,0x03};              // 工作模式
	const static _tagBOM_STATUS_ID   RUN_MODE = {0x01,0x01,0x04};               // 运营模式，参见7.9运营模式代码。
	const static _tagBOM_STATUS_ID   HOUR24MODE = {0x01,0x01,0x05};             // 24小时运营模式
	const static _tagBOM_STATUS_ID   DELAY_RUN_MODE = {0x01,0x01,0x06};         // 延长运营时间
	const static _tagBOM_STATUS_ID   CONNECT_STATUS = {0x01,0x01,0x07};         // 连接状态
	const static _tagBOM_STATUS_ID   TIME_DELAY = {0x01,0x01,0x08};             // 时钟差异
	const static _tagBOM_STATUS_ID   LOGIN_STATUS = {0x01,0x01,0x09};           // 登录状态
	const static _tagBOM_STATUS_ID   DEVICE_AUTO_RUN_STATUS = {0x01,0x01,0x0A}; // 设备自动运行状态
	const static _tagBOM_STATUS_ID   MAGAZINEA_STATUS = {0x06,0x01,0x01};    	// 票箱1安装状态
	const static _tagBOM_STATUS_ID   MAGAZINEA_COUNT = {0x06,0x01,0x02};        // 票箱1数量状态
	const static _tagBOM_STATUS_ID   MAGAZINEB_STATUS = {0x06,0x02,0x01};    	// 票箱2安装状态
	const static _tagBOM_STATUS_ID   MAGAZINEB_COUNT = {0x06,0x02,0x02};        // 票箱2数量状态
	const static _tagBOM_STATUS_ID   CYCLEBOX_STATUS = {0x07,0x01,0x01};        // 废票箱安装状态
	const static _tagBOM_STATUS_ID   CYCLEBOX_COUNT = {0x07,0x01,0x02};         // 废票箱数量状态
	const static _tagBOM_STATUS_ID   RW1_STATUS = {0x13,0x01,0x01};             // 读写器1状态
	const static _tagBOM_STATUS_ID   RW2_STATUS = {0x13,0x02,0x01};             // 读写器2状态
	const static _tagBOM_STATUS_ID   SAM1_STATUS = {0x14,0x02,0x01};            // SAM1类型
	const static _tagBOM_STATUS_ID   SAM2_STATUS = {0x14,0x04,0x01};            // SAM2类型
	const static _tagBOM_STATUS_ID   SAM3_STATUS = {0x14,0x08,0x01};            // SAM3类型
	const static _tagBOM_STATUS_ID   ISAM1_STATUS = {0x14,0x02,0x02};           // ISAM1状态
	const static _tagBOM_STATUS_ID   ISAM2_STATUS = {0x14,0x04,0x02};           // ISAM2状态
	const static _tagBOM_STATUS_ID   ISAM3_STATUS = {0x14,0x08,0x02};           // ISAM3状态
	const static _tagBOM_STATUS_ID   DOOR_STATUS = {0x1C,0x01,0x01};            // 维修门综合状态
	const static _tagBOM_STATUS_ID   PRINTER_STATUS = {0x15,0x01,0x01};         // 打印机
	const static _tagBOM_STATUS_ID   TH_MODULE_STATUS = {0x20,0x01,0x01};       // TH发票模块
}


typedef struct _ticketcomhdr_t
{
	WORD issuerID;											//2BCD车票发行商编码
	BYTE ticketApplicationSerialNo[8];		//8HEX票卡应用序列号
	BYTE ticketType;											//1BCD票卡类型
	BYTE ticketSubType;									//1BCD票卡子类型
	BYTE ticketStatus;										//1BCD票卡状态
	WORD ticketChargeTransSerialNo;		//2HEX票卡充值交易序号
	WORD ticketConsumeTransSerialNo;	//2HEX票卡消费交易序号
	_ticketcomhdr_t()
	{
		issuerID = 0;
		memset(ticketApplicationSerialNo,0,sizeof ticketApplicationSerialNo);
		ticketType = 0;
		ticketSubType = 0;
		ticketStatus = 0;
		ticketChargeTransSerialNo = 0;
		ticketConsumeTransSerialNo = 0;
	}
} TicketComHdr_t;

typedef struct _operatorcomhdr_t
{
	BYTE operatorID[3];				//3BCD操作员代码
	BYTE bomShiftID;					//BOM班次序号
	_operatorcomhdr_t()
	{
		memset(operatorID,0,sizeof operatorID);
	}
} OperatorComHdr_t;

typedef struct _financecomhdr_t
{
	BYTE payMethod;	//支付方式
	BYTE ticketNum[8];	//支付票卡逻辑卡号
	long amount;		//交易发生金额
	_financecomhdr_t()
	{
		payMethod = 0x01;
		memset(ticketNum,0,sizeof ticketNum);
		amount = 0;
	}

} FinanceComHdr_t;


typedef struct _nonimmidatelysurrenderapply
{
	Sex_t gender;							//持卡人性别
	BYTE name[20];							//ASC持卡人姓名
	CERTIFICATE_TYPE certificateType;		//BCD证件类型
	BYTE certificateID[20];					//ASC证件编码
	BYTE billNum[10];						//用于领款的收据编号
	BYTE nouseReason;						//不使用票卡原因
	BYTE applyType;							//退卡申请类型
	BYTE reason;							//票卡钱包退款原因
	BYTE suspendBillNum[10];				//挂失单据编号
	WORD cityCode;							//城市代码
	WORD industryCode;						//行业代码
	TicketComHdr_t ticketComHdr;			//票卡公共域
	_nonimmidatelysurrenderapply()
	{
		gender = SEX_MALE;
		memset(name,0,sizeof name);
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
		memset(billNum,0,sizeof billNum);
		nouseReason = 0;
		applyType = 0;
		reason = 0;
		memset(suspendBillNum,0,sizeof(suspendBillNum));
		cityCode = 0;
		industryCode = 0;
	}
}NonImmidatelySurrenderApply;


typedef struct _querynonimmidatelysurrenderapply
{
	CERTIFICATE_TYPE certificateType;		//BCD证件类型
	BYTE certificateID[20];					//ASC证件编码
	BYTE billNum[10];						//BCD收据编号
	WORD cityCode;							//城市代码
	WORD industryCode;						//行业代码
	TicketComHdr_t ticketComHdr;			//票卡公共域
	_querynonimmidatelysurrenderapply()
	{
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
		memset(billNum,0,sizeof billNum);
		cityCode = 0;
		industryCode = 0;
	}
}QueryNonImmidatelySurrenderApply;

typedef struct _nonimmidatelysurrender
{
	BYTE cardNum[8];				//票卡应用序列号
	DWORD transAmount;				//交易发生金额
	BYTE refundReason;				//退款原因
	DWORD depositAmount;			//退押金额
	BYTE billNum[10];				//收据号
	_nonimmidatelysurrender()
	{
		memset(cardNum,0,sizeof(cardNum));
		transAmount = 0;
		refundReason = 0;
		depositAmount = 0;
		memset(billNum,0,sizeof(billNum));
	}
}NonImmidatelySurrender;

typedef struct _signcardapply
{
	Sex_t gender;										//持卡人性别
	BYTE name[20];							//ASC持卡人姓名
	CERTIFICATE_TYPE certificateType;							//BCD证件类型
	BYTE certificateID[20];							//ASC证件编码
	_DATE_TIME cardValidity ;		//证件有效期
	BYTE phoneNumber[10];			//BCD电话号码
	BYTE email[60];							//ASCEmail
	BYTE address1[128];					//ASC地址1
	BYTE address2[128];					//ASC地址2

	_signcardapply()
	{
		gender = SEX_MALE;
		memset(name,0,sizeof name);
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
		memset(phoneNumber,0,sizeof phoneNumber);
		memset(email,0,sizeof email);
		memset(address1,0,sizeof address1);
		memset(address2,0,sizeof address2);
	}
} SignCardApply;

typedef SignCardApply SignCardModify;

typedef struct _querysigncardapply
{
	CERTIFICATE_TYPE certificateType;							//BCD证件类型
	BYTE certificateID[20];							//ASC证件编码
	_querysigncardapply()
	{
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
	}
} QuerySignCardApply;

typedef struct _reportloss
{
	BYTE gender;										//持卡人性别
	BYTE name[20];							//ASC持卡人姓名
	CERTIFICATE_TYPE certificateType;							//BCD证件类型
	BYTE certificateID[20];							//ASC证件编码
	_reportloss()
	{
		gender = 0x01;
		memset(name,0,sizeof name);
		certificateType = CERTIFICATE_ID_CARD_MAINLAND;
		memset(certificateID,0,sizeof certificateID);
	}
} ReportLoss;

typedef ReportLoss CancelReportLoss;

typedef enum _administrationprocesstype
{
	TVM_STUCK_CASH = 50,				//TVM卡币
	TVM_STUCK_TICKET = 51,			//TVM卡票
	TVM_INVALID_TICKET = 52,		//TVM发售无效票
	TVM_LESS_CHANGE = 53,				//TVM少找霍
	OTHER = 99											//其他 
}AdministrationProcessType;

typedef struct _administrationprocess
{
	AdministrationProcessType type;
	BYTE incomeOrOutcome; //01收入，02支出
	BYTE memo[20];					//备注
	_administrationprocess()
	{
		type = TVM_STUCK_CASH;
		incomeOrOutcome = 0x01;
		memset(memo,0,sizeof memo);
	}
}AdministrationProcess;

// 当前生效CD版本
typedef enum _cd_version{
	VERSION_FIRST = 1,
	VERSION_SECOND = 2
}CD_VERSION;

// 6.3.1	BOM对TVM故障退款
typedef struct _bom_refund_tvm{
	TransComHdr_t transComHdr;
	BYTE bRefoundNo[15];	      // 15 1BIN+2BCD+1BIN+7BCD+4BIN 退款凭证编号
	DWORD dwRefoundAmount;	      // 4 BIN BOM退款金额
	_bom_refund_tvm(){
		dwRefoundAmount = 0;
	};

}BOM_REFUND_TVM;


// 6.3.5	废票回收交易
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

//6.3.6 BOM非营业性收支
typedef struct _tagBOM_NONBUSINESS_INCOME
{
	TransComHdr_t transComHdr;
	BYTE baReason[20];			  //20 ASCII收支原因
	DWORD dwAmount;			//4 BIN 收支金额，转化为INTEL序
	BYTE baMemo[60];				 //60 ASCII备注
};

// 6.3.6	回收已售车票交易
typedef struct _recycle_issued_tickettrade{
	AfcSysHdr_t afcSysHdr;      // AFC系统公共头
	DEVICE_ID deviceid;         // 4 BIN 设备ID
	DWORD dwAfcSerialNo;		// 4 BIN AFC数据流水号	
	_DATE_TIME dateTime;		// 7 BCD 数据产生时间
	BYTE bMsgType;		        // 1 BIN 数据类型
	BYTE bMsgSubType;           // 1 BIN 数据子类型
	DWORD issueId;			    // 4 BIN 卡发行商
	WORD productType;			// 2 BIN 车票产品类型
	DWORD cardSerialNo;			// 4 BIN 卡序列号
	WORD lifecycle;             // 2 BIN 原交易的卡生命周期
	DWORD udsnNo;				// 4 BIN 原交易的交易流水号
	DWORD tradeValue;			// 4 BIN 原交易金额
	BYTE  tradeType;			// 1 BIN 原交易的交易类型
	BYTE  tradeSubType;			// 1 BIN 原交易的交易子类型
	_recycle_issued_tickettrade()
	{
		dwAfcSerialNo = 0;
		bMsgType = 0x50;
		bMsgSubType = 0x06;
		issueId  = 0;
		productType = 0;
		cardSerialNo = 0;
		lifecycle = 0;
		udsnNo = 0;
		tradeValue = 0;
		tradeType = 0;
		tradeSubType = 0;
	}
}RECYCLE_ISSUED_TICKETTRADE;

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
	DEVICE_COMPONENT partType;    // 1 BIN 设备部件类型
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

// 部件组成
typedef struct _component{
	BYTE  bDeviceType;   // 1 BIN 设备部件类型
	DWORD dwCompoentID;  // 3 BIN 部件序号
}COMPONENT;
