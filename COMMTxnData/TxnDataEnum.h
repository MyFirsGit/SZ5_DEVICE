#pragma once

// 交易数据类型
typedef enum _txn_message_kind
{
	ACC_TRANSACTION = 1,	// ACC交易数据
	ECT_TRANSACTION = 2,	// ECT交易数据
	AFC_TRANSACTION = 3		// AFC交易数据
}TXN_MESSAGE_KIND;

// 数据包类型
typedef enum _data_pekg_kind
{
	UN_DEF	 = 0x00,	// 未定义数据；
	ACC_PEKG = 0x01,	// ACC交易数据包；
	ECT_PEKG = 0x02,	// 一卡通交易数据包；
	AFC_PEKG = 0x03,	// 内部交易数据包；
	BUS_PEKG = 0x04,	// 业务数据包；
	EVN_PEKG = 0x05 	// 事件数据包
}DATA_PEKG_KIND;


// 数据类型编码
typedef	enum _data_type
{
	ACC_TRANS = 1,//ACC交易数据	
	AFC_TRANS = 2,//AFC交易数据	
	ECT_TRANS = 3,//ECT交易数据
	BUS_DATA  = 4,//业务数据
	EVEN_DATA = 5,//事件数据
	LOG_DATA  = 6,//日志数据
	UNKNOW_DATA = 7 //未知数据
} DATA_TYPE_CODE;

static const int ERROR_MSG_MAX_LEN  = 261;		// 异常最大文字数

static const int PEKG_ID_LEN		= 28;							// 数据包ID长度

static const TCHAR* const MESSAGES_DIR =  _T("messages\\");
static const TCHAR* const UNSEND_FILE_PATH		=  _T("unsend\\");
static const TCHAR* const ECT_TXN_FILE_PATH		=  _T("messages\\transaction\\ect\\");
static const TCHAR* const ACC_TXN_FILE_PATH		=  _T("messages\\transaction\\acc\\");
static const TCHAR* const AFC_TXN_FILE_PATH		=  _T("messages\\transaction\\afc\\");
static const TCHAR* const ACC_SENDED_FILE_PATH	=  _T("messages\\transaction\\acc\\history\\");
static const TCHAR* const ACC_UNTAC_FOLDER =  _T("messages\\transaction\\acc\\untac\\");
static const TCHAR* const AFC_SENDED_FILE_PATH	=  _T("messages\\transaction\\afc\\history\\");
static const TCHAR* const ECT_SENDED_FILE_PATH	=  _T("messages\\transaction\\ect\\history\\");
static const TCHAR* const BUS_TXN_FILE_PATH		=  _T("messages\\bussiness\\");
static const TCHAR* const BUS_SENDED_FILE_PATH	=  _T("messages\\bussiness\\history\\");
static const TCHAR* const EVN_TXN_FILE_PATH		=  _T("messages\\event\\");
static const TCHAR* const EVN_SENDED_FILE_PATH	=  _T("messages\\event\\history\\");

static const TCHAR* const CURRENT_ACC_FILE_NAME	 =  _T("currentAccPekg");
static const TCHAR* const CURRENT_ECT_FILE_NAME	 =  _T("currentEctPekg");
static const TCHAR* const CURRENT_AFC_FILE_NAME	 =  _T("currentAfcPekg");
static const TCHAR* const CURRENT_BUS_FILE_NAME	 =  _T("currentBusPekg");
static const TCHAR* const CURRENT_EVN_FILE_NAME	 =  _T("currentEvnPekg");
static const TCHAR* const PROTECT_FILE_NAME =  _T("unsendpackage.dll");


