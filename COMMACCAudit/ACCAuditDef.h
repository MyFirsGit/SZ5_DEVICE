#pragma once



#define AR_MANAGE_ORIGINAL_FILE_LEN					4*3						    // 文件初始长度
#define AR_MANAGE_DEVICE_TYPE_LEN					4							// 设备类型长度
#define AR_MANAGE_REVOLVE_TIMES_LEN					4							// 循环次数长度
#define AR_MANAGE_REVOLVE_BODY_LEN                  40							// 循环体长度
#define AR_MANAGE_REVOLVE_TIMES_POS                 8							// 循环次数开始位置
#define AR_MANAGE_TICKET_TYPE_LEN					4							// 车票类型长度
#define AR_MANAGE_TICKET_POSITION_POS			    12							// 车票类型开始位置
#define AR_MANAGE_UPDATE_MESSAGE_LEN				4							// 更新数据长度
#define AR_MANAGE_UPDATE_MESSAGE_POS				12							// 更新数据开始位置

#define AR_MANAGE_TAG_LEN											4
#define AR_MANAGE_COMM_HEAD_LEN                     24							//审计通用报文头长度


static const TCHAR* const AR_MANAGE_FOLDER			= _T("AR\\");					// 审计文件目录名
static const TCHAR* const AR_MANAGE_FILE_NAME		= _T("DeviceAR.dat");			// 审计文件名
static const TCHAR* const AR_MANAGE_ACC_FILE_NAME	= _T("ACCAR.dat");
static const TCHAR* const AR_MANAGE_SERIAL_NAME		= _T("SERIAL.dat");				// 交易流水号暂存文件
static const TCHAR* const AR_MANAGE_DEV_NAME		= _T("DeviceReg.dat");			// 设备寄存器记录名
static const TCHAR* const AR_MANAGE_BOM_FILE_NAME = _T("BomAR.dat");		       // 审计文件名

static const int AR_ERROR_MSG_MAX_LEN  = 261;		// 异常最大文字数

typedef enum registerId{
		LUCKCOUNT     =5,	//累积锁卡次数
		SELLCOUNT     =6,	//累积发售张数
		SELLAMOUNT    =7,	//累积发售金额（分）
		REFOUNDCOUNT  =8,	//累积退票次数
		REFOUNDAMOUNT =9,	//累积退票金额（分）
		UPDATACOUNT   =10,	//累积更新次数
		UPDATAAMOUNT  =11,	//累积更新金额（分）
		CHARGECOUNT   =12,	//累积充值次数
		CHARGEAMOUNT  =13	//累积充值金额（分）
}REGISTERID;
// 设备类型编码
typedef enum _ar_reason
{
	AR_BUSINESS_DAY_START = 0x02,		//运营日开始
	AR_BUSINESS_DAY_END = 0x01,			//运营日结束
	AR_BUSINESS_DAY_SHUTDOWN = 0x03,	//运营日没结束关机
} DEVICE_AR_REASON;