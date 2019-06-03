#ifndef __RWCOMLIBAPI_H__
#define __RWCOMLIBAPI_H__

#pragma once

#include "CommonDefine.h"
#include "RWException.h"

static const UINT RW_MSG_NO_RESPONSE = WM_USER + 201;

static const unsigned long RW_ERROR_CARD_NO_CARD = 0x0069;

//static const unsigned long RW_ERROR_CARD_NO_CARD = 0x00100001;
static const unsigned long RW_ERROR_NOT_AVALUEABLE_CARD_TYPE = 0x0050003D;

typedef enum _rw_rsp_code {
	RW_RSP_ACC_CARD_BLACKLIST_BLOCKED   = 0xA1000000,			// 一票通黑名单票卡已锁定

	//RW_RSP_ACC_CARD_BLACKLIST   = 0xA1,			// ??一票通黑名单票卡需要锁定
	//RW_RSP_ECT_CARD_BLACKLIST   = 0xA3,			// ??一卡通黑名单票卡需要锁定
/*-------------------------------------以上为旧版 部分待确认----------------------------------------------------*/
	RW_RSP_CLOSE_ERROR					= 0xFF11,		// 关闭通讯错误
	RW_RSP_SEND_ERROR					= 0xFF12,		// 发送错误
	RW_RSP_RECEIVE_TIMEOUT				= 0xFF13,		// 接收超时
	RW_RSP_RECEIVE_ERROR				= 0xFF14,		// 接收错误
	RW_RSP_DOWNLOAD_PARA_ERR			= 0xFF15,		// 下载参数错误
	RW_RSP_DOWNLOAD_SERIAL_ERR			= 0xFF16,		// 下载序列号错误
	RW_RSP_SEND_WAIT					= 0xFF17,		// 命令发送，等待
	RW_RSP_COMMAND_CANCLED				= 0xFF18,		// 命令取消
	RW_RSP_OPEN_PORT_ERR				= 0xFF19,		// 打开端口错误
	RW_RSP_DEVICE_ERR					= 0xFF20,		// 当输入的设备类型不正确时
	RW_RSP_PACKETS_ERR					= 0xFF21,		// 当获不到空闲的数据包
	RW_RSP_GET_DATA_ERR					= 0xFF22,		// 当获取不到数据时
	RW_RSP_OUTPUT_BUFFER_ERR			= 0xFF23,		// 当接口函数提供数据的输出缓存不足时
	RW_RSP_NO_DATA_RETURN				= 0xFF24,		// 当没有数据要返回时
	RW_RSP_MEMORY_ERR					= 0xFF25,		// 分配内存出错了
	RW_RSP_FILE_ERR						= 0xFF26,		// 文件错误
	RW_RSP_UPLOAD_FINAL_TRANSACTION		= 0xFF31,		// 须上传最后一交易
	RW_RSP_PRELIMINARY_INSPECTION		= 0xFF32,		// 设备需要初验(只对CPU卡可用)
	RW_RSP_INITIALl_INSPECTION_END		= 0xFF33,		// 初验结束(只对M1卡有用)
	RW_RSP_CARD_ERR						= 0xFF34,		// 卡片错误
	RW_RSP_POINTER_ERR					= 0xFF35,		// 指针错误
	RW_RSP_TIMEOUT_HANDLING				= 0xFF36,		// 超时处理
	RW_RSP_INSUFFICIENT_LENGTH_DATA		= 0xFF37,		// 接收的数据长度不够
	RW_RSP_CONNECT_SERVER_FAIL			= 0xFF38,		// 连接服务器失败
	RW_RSP_PARAMETER_ERR				= 0xFF39,		// 函数参数错误
	/*0xFF3A=$$$$
	0xFF3B=$$$$
	0xFF3C=$$$$
	0xFF3D=$$$$
	0xFF3E=$$$$
	0xFF3F=$$$$*/
	RW_RSP_COMMAND_ERR					= 0xFF40,		// 联机相应报文件中的命令码错误
	RW_RSP_COMMAND_ERROR				= 0xFF50,		// 联机相应报文件中的命令码错误
	RW_RSP_MAC2_LEN_ERR					= 0xFF52,		// 联机服务机返回的MAC2长度错误(长度不等于4)
	RW_RSP_CARD_TYPE_ERR				= 0xFF53,		// 联机读卡返回的"卡片类型"有误
	RW_RSP_NEED_MODIFY_FILE_NUM			= 0xFF54,		// 联机读卡返回的"需修改文件的个数"有误
	RW_RSP_SEND_DATA_ERR				= 0xFF61,		// 向联机服务发送数据失败
	RW_RSP_SERVICE_GET_DATA_ERR			= 0xFF62,		// 从联机服务上获取数据失败
	RW_RSP_TPU_INCORRECT_ERR			= 0xFF64,		// 联机售卡(充值)时,从TPU中读取的密钥类型不正确
	RW_RSP_DATA_LESS_THAN_32			= 0xff71,		// 接收到的读卡信息反馈数据长度小于32
	RW_RSP_DATA_LESS_32CARD_DATA		= 0xff72,		// 接收到的读卡信息反馈数据小于32+卡数据长度
	RW_RSP_DATA_LESS_32_CARD_LOG_LEN	= 0xff73,		// 接收到的读卡信息反馈数据小于32+卡数据长度+日志长度
	RW_RSP_LESS_THAN_37					= 0xff74,		// 接收到的信息长度小于37
	RW_RSP_LESS_THAN_37_UD				= 0xff75,		// 接收到的信息长度小于37+UD数据长度
	RW_RSP_LESS_THAN_37_UD_AR			= 0xff76,		// 接收到的信息长度小于37+UD数据长度+AR数据长度
	RW_RSP_CARDLEN_LESS_THAN_CARDSIZE	= 0xff77,		// 发送给TPU的Cardlen小于Cardsize
	RW_RSP_RECEIVED_PACKET_LESS_CARD	= 0xff78,		// 接收的数据包长度小于卡数据长度+其他信息长度
	RW_RSP_CYCLE_POWER_TPU				= 0xFFF4,		// 给TPU断电重启
	RW_RSP_OK							= 0x0000,		// 操作成功
	RW_RSP_OPERATION_FAILED				= 0x0001,		// 操作失败
	RW_RSP_RESULTS_UNCERTAIN			= 0x0002,		// 操作结果不确定
	RW_RSP_READ_CARD_FAILED				= 0x0057,		// 读卡失败
	RW_RSP_WRITE_CARD_FAILED			= 0x0058,		// 写卡失败
	RW_RSP_BLACK_CARD					= 0x0059,		// 黑名单卡
	RW_RSP_CARD_DATA_NOT_LEGITIMATE		= 0x005a,		// 卡片数据不合法（城市代码有误，行业代码有误，发行商代码错误，类型不匹配）
	RW_RSP_CARD_STATUS_NOT_LEGITIMATE	= 0x005b,		// 卡片状态不合法
	RW_RSP_PURSE_BEYOND_LIMIT			= 0x005c,		// 钱包超越上限
	RW_RSP_CARD_NOT_VALID				= 0x005d,		// 卡片不在有效期
	RW_RSP_ENTER_OUT_STATION_ERR		= 0x005e,		// 进出站次序错误
	RW_RSP_BALANCE_INSUFFICIENT			= 0x005f,		// 卡片余额不足
	RW_RSP_SITE_LIMIT					= 0x0060,		// 站点限制
	RW_RSP_TIMES_LIMIT					= 0x0061,		// 卡片日乘坐次数限制
	RW_RSP_CARD_TIMEOUT					= 0x0062,		// 卡片超时
	RW_RSP_CARD_BEYONG_JOURNEY			= 0x0063,		// 卡片超乘
	RW_RSP_RW_NOT_SUPPORT				= 0x0064,		// 读写器不支持此业务
	RW_RSP_SAM_ERR						= 0x0065,		// sam卡错误
	RW_RSP_DOWNLOAD_FILE_FAILED			= 0x0066,		// 下载文件失败
	RW_RSP_CLOCK_OPERATION_FAILED		= 0x0067,		// 时钟操作失败
	RW_RSP_START_RW_FAILS				= 0x0068,		// 启动读写器失败
	RW_RSP_FIND_CARD_FAILED				= 0x0069,		// 找卡失败
	RW_RSP_FILE_NOT_EXIST				= 0x006a,		// 不存在此文件
	RW_RSP_GET_REGISTER_DATA_FAILED		= 0x006b,		// 获取寄存器数据失败
	RW_RSP_COSTING_FAILURE				= 0x006c,		// 计算费用失败
	RW_RSP_SLE_SEND_NOT_LEGITIMATE		= 0x006d,		// SLE发送的命令数据不合法
	RW_RSP_RW_NOT_INITIALIZED			= 0x006e,		// 读写器未初始化
	RW_RSP_DATA_NOT_EXIST				= 0x006f,		// 数据不存在
	RW_RSP_FARE_FILE_EXCEPTIONS			= 0x0070,		// 票价文件异常
	RW_RSP_OPERATIONAL_PARM_EXCEPTIONS	= 0x0071,		// 运营控制参数异常

	//RW_RSP_OK               = 0x00,         // 成功
	RW_RSP_BLACKCARD        = 0x01,			// 黑名单锁卡
	RW_RSP_ENOUGH           = 0x04,         // 余额不足
	RW_RSP_READERROR        = 0x05,         // 读卡失败
	RW_RSP_WRITEERROR       = 0x06,         // 写卡失败
	RW_RSP_STATUSERROR      = 0x07,         // TPU状态非法
	RW_RSP_FINDERROR		= 0x10,         // 寻卡类错误
	RW_RSP_M1KAUTHENERROR	= 0x20,			// M1卡认证错误

	RW_RSP_SAMERR           = 0x30,         // SAM卡处理公共类错误
	RW_RSP_BMACPSAMERR      = 0x31,         // 一卡通PSAM卡认证故障
	RW_RSP_BMACISAMERR      = 0x32,         // 一卡通ISAM卡认证故障
	RW_RSP_ACCPSAMERR       = 0x33,         // ACC PSAM卡认证故障
	RW_RSP_ACCISAMERR       = 0x34,         // ACC ISAM卡认证故障

	RW_RSP_COMERR			= 0x40,         // 通讯接口类错误

	RW_RSP_ACCERR			= 0x50,         // 一票通业务处理类错误
	RW_RSP_ECTERR			= 0x60,         // 一卡通业务处理类错误

	RW_RSP_PARAMERR			= 0x70,			// 配置参数类错误
	RW_RSP_UDERR			= 0x80,			// UD数据类错误
	RW_RSP_NOT_AVALIABLE_STATION = 0x88,	// 目地车站不运营
	RW_RSP_HARDWAREERR		= 0x90,			// 硬件类错误

	RW_RSP_ACC_CARD_BLACKLIST   = 0xA1,			// 一票通黑名单票卡需要锁定
	RW_RSP_ECT_CARD_BLACKLIST   = 0xA3,			// 一卡通黑名单票卡需要锁定
	RW_RSP_UNKNOWERR			= 0xf0,			// 未归类错误

	RW_RSP_ISSUE_INIT_CHECK	= 0x33,			// 一卡通售卡需要初检
	RW_RSP_CHARGE_OVERTIME	= 0xB1,			// 一卡通充值需要超时处理
	RW_RSP_CHARGE_DIFF_CARD  = 0xB2         // 超时处理不是同一张卡
};

// 功能代码
typedef enum FUNCTION_CODE{
	FUNCTION_CODE_ENTER				= 0x01,//进站；
	FUNCTION_CODE_EXIT				= 0x02,//出站；
	FUNCTION_CODE_ISSUE				= 0x03,//售卡;
	FUNCTION_CODE_ADJUST			= 0x04,//补票;
	FUNCTION_CODE_CHARGE			= 0x05,//充值;
	FUNCTION_CODE_REFUND			= 0x06,//退卡退资;
	FUNCTION_CODE_EXCHANGE_OLD		= 0x07,//替换旧卡；
	FUNCTION_CODE_EXCHANGE_NEW		= 0x08,//替换新卡；0
	FUNCTION_CODE_REFRESH_AUTO		= 0x09,//激活；
	FUNCTION_CODE_DEFER				= 0x10,//延期；
	FUNCTION_CODE_REPORT_LOST		= 0x11,//挂失；
	FUNCTION_CODE_CONTERACT			= 0x12,//抵消；
	FUNCTION_CODE_LOST_CANCEL		= 0x13,//解锁；
	FUNCTION_CODE_REFUND_MONEY		= 0x14,//退资；
	FUNCTION_CODE_MAINTENANCE_DOOR	= 0x5F,//TVM后维护门读卡
	FUNCTION_CODE_SVT_ISSUE			= 0x6F,//TVM储值卡购票
	FUNCTION_CODE_OTHER				= 0xFF //其他业务	
};


// 参数代码
typedef enum PARAM_TPU_CODE{
	TICKET_PRICE_PARA  = 0x08,    //票价参数
	BLACK_LIST_PARA    = 0x10,    //卡黑名单参数
	RE_APP_PARA        = 0x11,    //应用程序文件
	SERV_CONTROL_PARA  = 0x14,    //运营控制参数文件
	MODE_HISTORY_PARA  = 0x15,    //模式履历
	STATION_NUM_PARA   = 0x17,    //运营点参数
};

static const unsigned long RW_MSG_CARD_INFO_LEN = 512;      // 读卡信息的长度
static const unsigned long RW_MSG_LOG_INFO_LEN  = 512;     // 卡日志信息的长度

static const unsigned long RW_SIM_M1_CARD_TYPE = 1;		// 模拟ECT卡  
// 通用数据文件名定义
static const TCHAR* const TPU_INFO_FILE_NAME				= _T("C:\\RWSimData\\TpuInfoDataFile.dat");				// Tpu信息数据文件
static const TCHAR* const PARAM_DATA_FILE_NAME				= _T("C:\\RWSimData\\ParamVersionDataFile.dat");		// 参数版本数据文件
static const TCHAR* const UD_DATA_FILE_NAME					= _T("C:\\RWSimData\\UDDataFile.dat");					// UD数据文件

// 一卡通数据文件名定义
static const TCHAR* const M1_CARD_FILE_NAME				= _T("C:\\RWSimData\\M1\\M1CardDataFile.dat");			// M1票卡数据文件
static const TCHAR* const ACC_M1_CHARGE_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\ChargeUDDataFile.dat");		// 充值UD数据文件
static const TCHAR* const ACC_M1_CHARGE_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\ChargeARDataFile.dat");		// 充值AR数据文件
static const TCHAR* const ACC_M1_ISSUE_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\IssueUDDataFile.dat");			// 售票UD数据文件
static const TCHAR* const ACC_M1_ISSUE_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\IssueARDataFile.dat");			// 售票AR数据文件
static const TCHAR* const ACC_M1_BLOCK_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\BlockUDDataFile.dat");			// 锁卡UD数据文件
static const TCHAR* const ACC_M1_BLOCK_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\BlockARDataFile.dat");			// 锁卡AR数据文件
static const TCHAR* const ACC_M1_ADJUST_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\AdjustUDDataFile.dat");		// 补票UD数据文件
static const TCHAR* const ACC_M1_ADJUST_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\M1\\AdjustARDataFile.dat");		// 补票AR数据文件

// ACC数据文件名定义
static const TCHAR* const UL_CARD_FILE_NAME					= _T("C:\\RWSimData\\UL\\ULCardDataFile.dat");			// UL票卡数据文件
static const TCHAR* const ACC_ISSUE_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\IssueUDDataFile.dat");			// 售票UD数据文件
static const TCHAR* const ACC_ISSUE_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\IssueARDataFile.dat");			// 售票AR数据文件
static const TCHAR* const ACC_ADJUST_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\AdjustUDDataFile.dat");		// 补票UD数据文件
static const TCHAR* const ACC_ADJUST_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\AdjustARDataFile.dat");		// 补票AR数据文件
static const TCHAR* const ACC_REFUND_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\RefundUDDataFile.dat");		// 退款UD数据文件
static const TCHAR* const ACC_REFUND_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\RefundARDataFile.dat");		// 退款AR数据文件
static const TCHAR* const ACC_BLOCK_UD_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\BlockUDDataFile.dat");			// 锁卡UD数据文件
static const TCHAR* const ACC_BLOCK_AR_DATA_FILE_NAME		= _T("C:\\RWSimData\\UL\\BlockARDataFile.dat");			// 锁卡AR数据文件
static const TCHAR* const ACC_COUNTERACT_UD_DATA_FILE_NAME	= _T("C:\\RWSimData\\UL\\CounteractUDDataFile.dat");	// 抵消UD数据文件
static const TCHAR* const ACC_COUNTERACT_AR_DATA_FILE_NAME	= _T("C:\\RWSimData\\UL\\CounteractARDataFile.dat");	// 抵消AR数据文件

#endif