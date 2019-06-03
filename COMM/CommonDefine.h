#pragma once
#include "Util.h"

// 配置文件名定义
#define BOM_INI_FILE_NAME							_T("BOM.INI")
#define INI_FILE											_T(".INI")
#define MAINTENANCE_INI_FILE_NAME				_T("Maintenance.INI")
#define FUNCTION_INI_FILE_NAME						_T("Functioninfo.INI")
#define VERSION_INI_FILE_NAME						_T("Versioninfo.INI")
#define FARE_TABLE_INI_FILE_NAME					_T("FareTableInfo.ini")

// 日志文件名定义
// 通用日志文件名定义
static const TCHAR* const FILTER_LOGIN_LOGOFF_LOG			= _T("LoginAndLogoff_");	// 登录登出日志文件名前缀
static const TCHAR* const FILTER_STAFFOPERATION_LOG			= _T("PassengerOperation_");	// 乘客操作日志文件名前缀
static const TCHAR* const FILTER_EXCEPTION_LOG				= _T("Exception_");			// 异常日志文件名前缀
static const TCHAR* const FILTER_SC_CLIENT_LOG				= _T("SC_Client_");			// SC客户端日志文件名前缀
static const TCHAR* const FILTER_SC_CLIENT_DATA				= _T("SC_Data_");			// SC通信数据日志文件名前缀
static const TCHAR* const FILTER_SC_SVC_LOG					= _T("SC_Service");			// SC服务日志文件名前缀
static const TCHAR* const FILTER_TXN_DATA					= _T("Txn_Data_");			// 数据存储日志文件名前缀
static const TCHAR* const FILTER_DEBUG_LOG					= _T("Debug_");				// DEBUG日志文件名前缀
static const TCHAR* const FILTER_DB_SQL_LOG					= _T("DB_SQL_");			// 数据库SQL操作日志文件名前缀
static const TCHAR* const FILTER_NETWORK_DB_LOG				= _T("Network_DB_");		// 网络数据库操作日志文件名前缀
static const TCHAR* const FILTER_QRCODE_LOG					= _T("QRCode_");			// 二维码操作日志文件名前缀
static const TCHAR* const FILTER_CCS_CLIENT_LOG				= _T("CCS_Client_");		// CCS客户端日志文件名前缀
static const TCHAR* const FILTER_CCS_CLIENT_DATA			= _T("CCS_Data_");			// CCS通信数据日志文件名前缀
static const TCHAR* const FILTER_CCS_SVC_LOG				= _T("CCS_Service_");		// CSS服务日志文件名前缀
static const TCHAR* const FILTER_KEYBOARD_LOG				= _T("Keyboard_");			// TVM维护键盘日志文件名前缀
static const TCHAR* const FILTER_OPERATION_LOG				= _T("PassengerFore_Debug_");//TVM乘客调试日志文件名前缀，开发调试用，不需要将删除！！！
static const TCHAR* const FILTER_FTP_HELPER_LOG				= _T("FTP_HELPER_");		// FTP帮助日志类前缀
static const TCHAR* const FILTER_MONITOR_CONTROL_LOG= _T("MonitorControl_");//TVM监控服务控制日志
static const TCHAR* const FILTER_MONITOR_RECHARGE_RW_LOG= _T("RechargeRWMonitor_");//TVM 充值读写器监控日志
static const TCHAR* const FILTER_MONITOR_COIN_MODULE_LOG= _T("CoinModuleMonitor_");//硬币模块监控日志
static const TCHAR* const FILTER_MONITOR_BANKNOTE_MODULE_LOG= _T("BanknoteCollectionModuleMonitor_");//纸币接受模块监控日志
static const TCHAR* const FILTER_STAFF_PRINT_HISTORY		= _T("Staff\\History");
static const TCHAR* const FILTER_STAFF_PRINT_UNPRINT		= _T("Staff\\UnPrint");
static const TCHAR* const FILTER_PASSENGER_PRINT_HISTORY	= _T("Passenger\\History");
static const TCHAR* const FILTER_PASSENGER_PRINT_UNPRINT	= _T("Passenger\\UnPrint");
static const TCHAR* const FILTER_AUDIT_LOG					= _T("Audit\\");
static const TCHAR* const FILTER_LOG_ZIP_BACKUP_HISTORY		= _T("LogZipBackup\\History");
static const TCHAR* const FILTER_WEBAPI_LOG					= _T("WebServiceApi_");
static const TCHAR* const FILTER_QR_ORDER_IMAGES_RUN_PRE	= _T("QR_ORDER_INFO_");	// 当前订单二维码信息
static const TCHAR* const FILTER_UPS_LOG					= _T("UPSLog_");					// UPS日志文件名前缀

// 硬件模块日志文件名定义
static const TCHAR* const FILTER_RECEIPT_PRINTER_LOG		= _T("Receipt_Printer_");			// 凭证打印机日志文件名前缀
static const TCHAR* const FILTER_DESKTOP_RW_LOG				= _T("Desktop_RW_");				// 桌面读写器日志文件名前缀
static const TCHAR* const FILTER_TOKEN_RW_LOG					= _T("Token_RW_");					// Token处理模块用读写器日志文件名前缀
static const TCHAR* const FILTER_CARD_RW_LOG					= _T("Card_RW_");					// 票卡处理模块用读写器日志文件名前缀
static const TCHAR* const FILTER_RECHARGE_RW_LOG			= _T("Recharge_RW_");				// 充值用读写器日志文件名前缀
static const TCHAR* const FILTER_CARD_RECYCLE_RW_LOG	= _T("Card_Recycle_RW_");		// 票卡回收处理模块用读写器日志文件名前缀
static const TCHAR* const FILTER_TOKEN_RECYCLE_RW_LOG	= _T("Token_Recycle_RW_");		// Token回收处理模块用读写器日志文件名前缀
static const TCHAR* const FILTER_ENTRY_RW_LOG					= _T("Entry_RW_");					// 进站刷卡读写器日志文件名前缀
static const TCHAR* const FILTER_EXIT_RW_LOG						= _T("Exit_RW_");						// 出站刷卡读写器日志文件名前缀
static const TCHAR* const FILTER_TOKEN_ES_RW1_LOG			= _T("Token_ES_RW1_");			// Token式ES读写器1日志文件名前缀
static const TCHAR* const FILTER_TOKEN_ES_RW2_LOG			= _T("Token_ES_RW2_");			// Token式ES读写器2日志文件名前缀
static const TCHAR* const FILTER_TOKEN_ES_RW3_LOG			= _T("Token_ES_RW3_");			// Token式ES读写器3日志文件名前缀
static const TCHAR* const FILTER_CARD_ES_RW_LOG				= _T("Card_ES_RW_");				// 卡式ES用读写器日志文件名前缀
static const TCHAR* const FILTER_PM_RW_LOG						= _T("PM_RW");						// 证卡打印机用读写器日志文件名前缀

static const TCHAR* const FILTER_ES_CARD_BOX1_RFID_LOG						= _T("ES_Card_Box1_RFID_");					// ES票箱1RFID日志文件名前缀
static const TCHAR* const FILTER_ES_CARD_BOX2_RFID_LOG						= _T("ES_Card_Box2_RFID_");					// ES票箱2RFID日志文件名前缀
static const TCHAR* const FILTER_ES_CARD_BOX3_RFID_LOG						= _T("ES_Card_Box3_RFID_");					// ES票箱3RFID日志文件名前缀
static const TCHAR* const FILTER_ES_CARD_BOX4_RFID_LOG						= _T("ES_Card_Box4_RFID_");					// ES票箱4RFID日志文件名前缀
static const TCHAR* const FILTER_ES_CARD_BOX5_RFID_LOG						= _T("ES_Card_Box5_RFID_");					// ES 票箱5RFID日志文件名前缀
static const TCHAR* const FILTER_ES_TOKEN_BOX1_RFID_LOG					= _T("ES_Token_Box1_RFID_");					// ES Token箱1RFID日志文件名前缀
static const TCHAR* const FILTER_ES_TOKEN_BOX2_RFID_LOG					= _T("ES_Token_Box2_RFID_");					// ES Token箱2RFID日志文件名前缀
static const TCHAR* const FILTER_ES_TOKEN_BOX3_RFID_LOG					= _T("ES_Token_Box3_RFID_");					// ES Token箱3RFID日志文件名前缀
static const TCHAR* const FILTER_COIN_ADD_BOX1_RFID_LOG					= _T("Coin_Add_Box1_RFID_");					// 硬币补充箱1RFID日志文件名前缀
static const TCHAR* const FILTER_COIN_ADD_BOX2_RFID_LOG					= _T("Coin_Add_Box2_RFID_");					// 硬币补充箱2RFID日志文件名前缀
static const TCHAR* const FILTER_COIN_RECYCLE_BOX1_RFID_LOG			= _T("Coin_Recycle_Box1_RFID_");			// 硬币回收箱1RFID日志
static const TCHAR* const FILTER_COIN_RECYCLE_BOX2_RFID_LOG			= _T("Coin_Recycle_Box2_RFID_");			// 硬币回收箱2RFID日志
static const TCHAR* const FILTER_BANKNOTE_CHANGE_BOX1_RFID_LOG	= _T("Banknote_Change_Box1_RFID_");		// 纸币找零箱1RFID日志文件名前缀
static const TCHAR* const FILTER_BANKNOTE_CHANGE_BOX2_RFID_LOG	= _T("Banknote_Change_Box2_RFID_");		// 纸币找零箱2RFID日志文件名前缀
static const TCHAR* const FILTER_BANKNOTE_WASTE_BOX_RFID_LOG		= _T("Banknote_Waste_Box_RFID_");			// 纸币废币箱RFID日志文件名前缀
static const TCHAR* const FILTER_BANKNOTE_RECYCLE_BOX_RFID_LOG		= _T("Banknote_Recycle_Box_RFID_");		// 纸币回收箱RFID日志

static const TCHAR* const FILTER_CARD_MODULE_LOG					= _T("Card_Module_");							// 票卡处理模块日志文件名前缀
static const TCHAR* const FILTER_TOKEN_MODULE_LOG				= _T("Token_Module_");						// Token处理模块日志文件名前缀
static const TCHAR* const FILTER_BANKNOTE_MODULE_LOG			= _T("Banknote_Module_");					// 纸币处理模块日志文件名前缀
static const TCHAR* const FILTER_COIN_MODULE_LOG					= _T("Coin_Module_");							// 硬币处理模块日志文件名前缀
static const TCHAR* const FILTER_PIN_PAD_LOG  							= _T("Pin_Pad_");									// 密码键盘模块日志文件名前缀
static const TCHAR* const FILTER_BANKCARD_MODULE_LOG			= _T("Bankcard_Module_");					// 银行卡处理模块日志文件名前缀
static const TCHAR* const FILTER_IO_MODULE_LOG						= _T("IO_Module_");								// IO模块日志文件名前缀
static const TCHAR* const FILTER_STATUS_DISPLAY_LOG				= _T("Status_Diaplay_");								// IO模块日志文件名前缀
static const TCHAR* const FILTER_UPS_MODULE_LOG					= _T("UPS_Module_");							// UPS模块日志文件名前缀
static const TCHAR* const FILTER_GATE_MODULE_LOG					= _T("Gate_Module_");							// 闸门模块日志文件名前缀
static const TCHAR* const FILTER_CARD_RECYCLE_MODULELOG	= _T("Card_Recycle_Module_");				// 票卡回收模块日志文件名前缀
static const TCHAR* const FILTER_TOKEN_RECYCLE_MODULE_LOG	= _T("Token_Recycle_Module_");			// Token回收模块日志文件名前缀
static const TCHAR* const FILTER_CARD_PRINTER_LOG					= _T("Card_Printer_");							// 票卡打印机日志文件名前缀
static const TCHAR* const FILTER_SCANER_LOG								= _T("Scaner_");									// 扫描仪日志文件名前缀
static const TCHAR* const FILTER_LASERPRINTER_LOG					= _T("Laser_Printer_");							// 镭射打印机日志文件名前缀
static const TCHAR* const FILTER_CAMERA_LOG								= _T("Camera_");									// 摄像头日志文件名前缀
static const TCHAR* const FILTER_TOKEN_ES_LOG							= _T("Token_ES_");								// Token式ES日志文件名前缀
static const TCHAR* const FILTER_CARD_ES_LOG								= _T("Card_ES_");									// 卡式ES日志文件名前缀
static const TCHAR* const FILTER_PM_MODULE_LOG						= _T("PM_Module");								// PM模块日志文件名前缀
static const TCHAR* const FILTER_MAGAZINE_MODULE_LOG     	= _T("Magazine_Module_");					// 票箱模块日志文件名前缀
static const TCHAR* const FILTER_DCR4000_LOG							= _T("logDCR4000Comm_");					// DCR4000日志文件名前缀

// 目录结构定义
// 通用目录定义
static const TCHAR* const LOG_DIR									= _T("Log\\");						// 日志文件目录
static const TCHAR* const DATABASE_DIR								= _T("Database\\");					// 数据库文件目录
static const TCHAR* const MC_UPDATE_DIR								= _T("Download\\MC\\");				// 控制部程序更新目录
static const TCHAR* const PM_UPDATE_DIR								= _T("Download\\PM\\");				// 参数更新目录
static const TCHAR* const TEMPLATE_UPDATE_DIR						= _T("Download\\TEMPLATE\\");		// 收据模板更新目录
static const TCHAR* const GUI_UPDATE_DIR							= _T("Download\\GUI\\");			// GUI文件更新目录
static const TCHAR* const STATION_MAP_UPDATE_DIR					= _T("Download\\STATION_MAP\\");	// STATION_MAP文件更新目录
static const TCHAR* const VOICE_UPDATE_DIR							= _T("Download\\VOICE\\");			// 声音文件更新目录
static const TCHAR* const MC_DOWNLOAD_DIR							= _T("PG\\MC\\");					// 控制部程序下载目录
static const TCHAR* const TPU_MAIN_DOWNLOAD_DIR						= _T("PG\\TPU-MAIN\\");				// TPU主控程序下载目录
static const TCHAR* const TPU_APP_DOWNLOAD_DIR						= _T("PG\\TPU-APP\\");				// TPU应用部程序下载目录
static const TCHAR* const GUI_DOWNLOAD_DIR							= _T("PG\\GUI\\");					// GUI文件下载目录
static const TCHAR* const STATION_MAP_DOWNLOAD_DIR					= _T("PG\\STATION_MAP\\");			// 线路站点参数下载目录
static const TCHAR* const TH_DOWNLOAD_DIR							= _T("PG\\TH\\");					// TH文件下载目录
static const TCHAR* const PURCHASE_DIR								= _T("SCAudit\\");					// 收益文件目录
static const TCHAR* const UPGRADE_DATA_DESC_FILE_NAME				= _T("upgrade.description");		// 升级文件说明文件
static const TCHAR* const GUI_CURRENT_DIR							= _T("res\\gui\\");					// GUI工作目录
static const TCHAR* const TEMPLATE_CURRENT_DIR						= _T("Template\\");					// TEMPLATE工作目录
static const TCHAR* const VOICE_CURRNT_DIR							= _T("res\\tts\\audio\\");			// Voice工作目录 

// 数据和备份数据文件路径
static const TCHAR* const MASTER_DIR		= _T("master\\"); // master文件目录

// TVM用
static const TCHAR* const TVM_CARD_RW_UPDATE_DIR		= _T("Download\\CardRW\\");		// TVM 票卡读写器更新目录
static const TCHAR* const TVM_TOKEN_RW_UPDATE_DIR		= _T("Download\\TokenRW\\");	// TVM Token读写器更新目录
static const TCHAR* const TVM_CHARGE_RW_UPDATE_DIR		= _T("Download\\ChargeRW\\");	// TVM 重置读写器更新目录	
static const TCHAR* const DATABASE_FILE_NAME_OF_TVM     = _T("TVM.db");					// TVM 数据库文件
static const TCHAR* const TVM_TH_UPDATE_DIR				= _T("Download\\TH\\");			// TVM 发卡模块更新目录
// ES用
static const TCHAR* const PHOTO_MAIN_DIR								= _T("Picture\\photo\\");				// 图片文件目录
static const TCHAR* const PHOTO_TEMP_DIR								= _T("Picture\\temp\\");				// 临时图片文件目录
static const TCHAR* const CRPT_TEMP_DIR_FILE_NAME				= _T("Picture\\printerTmp.prn");	// 票卡打印机打印文件临时目录
static const TCHAR* const PM_TEST_FRONTGOUND					= _T("res\\test card-Front.bmp");	// PM打印测试正面图片
static const TCHAR* const PM_TEST_BACKGOUND						= _T("res\\test card-Bakct.bmp");	// PM打印测试背面图片

static const TCHAR* const TC_UPDATE_DIR									= _T("Download\\TC\\");				// TH程序更新目录
static const TCHAR* const IC_UPDATE_DIR									= _T("Download\\IC\\");				// 外部RW程序更新目录
static const TCHAR* const TW_UPDATE_DIR									= _T("Download\\TW\\");				// 内部RW程序更新目录
static const TCHAR* const PM_DOWNLOAD_DIR							= _T("Download\\PM\\");				// 参数下载目录

// 日志文件名
static const TCHAR* const NOTE_LOGIN_LOGOFF_LOG			= _T("登录登出日志");
static const TCHAR* const NOTE_STAFFOPERATION_LOG		= _T("站员操作日志");
static const TCHAR* const NOTE_EXCEPTION_LOG			= _T("异常日志");
static const TCHAR* const NOTE_SC_CLIENT_LOG			= _T("SC客户端日志");
static const TCHAR* const NOTE_SC_CLIENT_DATA			= _T("SC收发数据");
static const TCHAR* const NOTE_SC_SVC_LOG				= _T("SC服务日志");
static const TCHAR* const NOTE_TXN_DATA					= _T("交易数据");
static const TCHAR* const NOTE_DT_RW_LOG				= _T("外部读写器日志");
static const TCHAR* const NOTE_DT_TW_LOG				= _T("内部读写器日志");
static const TCHAR* const NOTE_TH_LOG					= _T("TH设备通信日志");
static const TCHAR* const NOTE_PRT_LOG					= _T("打印机日志");
static const TCHAR* const NOTE_DEBUG_LOG				= _T("调试日志");
static const TCHAR* const NOTE_DCR4000_LOG				= _T("DCR4000日志");

static const TCHAR* const FILTER_RW_LOG					= _T("RW_");						// RW日志文件名前缀
static const TCHAR* const FILTER_TW_LOG					= _T("TW_");						// TW日志文件名前缀
static const TCHAR* const FILTER_TH_LOG					= _T("ThTrace_");					// TH日志文件名前缀
static const TCHAR* const FILTER_BRH_LOG                = _T("BrhTrace_");                  // TH日志文件名前缀  20180531
static const TCHAR* const FILTER_TOKEN_LOG				= _T("TokenTrace_");				// Token日志文件名前缀
static const TCHAR* const FILTER_PRT_LOG				= _T("PRT_");                       // 凭证打印机
static const TCHAR* const FILTER_DIO_LOG				= _T("DioTrace_");					// IO日志文件名前缀


static const TCHAR* const BOM_TRACE_DIR					= _T("log\\");						// 日志文件目录
static const TCHAR* const BOM_MC_UPDATE_DIR				= _T("Download\\MC\\");				// 控制部程序更新目录
static const TCHAR* const BOM_TC_UPDATE_DIR				= _T("Download\\TC\\");				// TH程序更新目录
static const TCHAR* const BOM_IC_UPDATE_DIR				= _T("Download\\IC\\");				// 外部RW程序更新目录
static const TCHAR* const BOM_TW_UPDATE_DIR				= _T("Download\\TW\\");				// 内部RW程序更新目录
static const TCHAR* const BOM_PM_UPDATE_DIR				= _T("Download\\PM\\");				// 参数更新目录
static const TCHAR* const BOM_MC_DOWNLOAD_DIR			= _T("PG\\MC\\");					// 控制部程序下载目录
static const TCHAR* const BOM_TPU_MAIN_DOWNLOAD_DIR		= _T("PG\\TPU-MAIN\\");				// TPU主控程序下载目录
static const TCHAR* const BOM_TPU_APP_DOWNLOAD_DIR		= _T("PG\\TPU-APP\\");				// TPU应用部程序下载目录
static const TCHAR* const BOM_TH_APP_DOWNLOAD_DIR		= _T("PG\\TH-APP\\");				// TH应用部程序下载目录
static const TCHAR* const BOM_PM_DOWNLOAD_DIR			= _T("Download\\PM\\");				// 参数下载目录
static const TCHAR* const BOM_PURCHASE_DIR				= _T("SCAudit\\");					// 收益文件目录

static const TCHAR* const TCM_TRACE_DIR					= _T("log\\");						// 日志文件目录
static const TCHAR* const TCM_MC_UPDATE_DIR				= _T("Download\\MC\\");				// 控制部程序更新目录
static const TCHAR* const TCM_IC_UPDATE_DIR				= _T("Download\\IC\\");				// 外部RW程序更新目录
static const TCHAR* const TCM_PM_UPDATE_DIR				= _T("Download\\PM\\");				// 参数更新目录
static const TCHAR* const TCM_MC_DOWNLOAD_DIR			= _T("PG\\MC\\");					// 控制部程序下载目录
static const TCHAR* const TCM_TPU_MAIN_DOWNLOAD_DIR		= _T("PG\\TPU-MAIN\\");				// TPU主控程序下载目录
static const TCHAR* const TCM_TPU_APP_DOWNLOAD_DIR		= _T("PG\\TPU-APP\\");				// TPU应用部程序下载目录
static const TCHAR* const TCM_PM_DOWNLOAD_DIR			= _T("Download\\PM\\");				// 参数下载目录
static const TCHAR* const TCM_PURCHASE_DIR				= _T("SCAudit\\");					// 收益文件目录

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// ES 定义
//////////stati const TCHAR* const PHOTO_MAIN_DIR                = _T("picture\\photo");			   // 图片文件目录
//////////static const TCHAR* const PHOTO_TEMP_DIR                = _T("picture\\temp");			   // 临时图片文件目录

//////////static const TCHAR* const PM_TEST_FRONTGOUND			= _T("card background\\test card-Front.bmp");	// PM打印测试正面图片
//////////static const TCHAR* const PM_TEST_BACKGOUND				= _T("card background\\test card-Bakct.bmp");	// PM打印测试背面图片

static const TCHAR* const DATABASE_FILE_NAME         = _T("ES.db");
static const TCHAR* const ES_MC_UPDATE_DIR			 = _T("Download\\MC\\");               // 控制部程序更新目录
static const TCHAR* const ES_RW1_UPDATE_DIR			 = _T("Download\\ESRW1\\");            // ES读写器1更新目录
static const TCHAR* const ES_RW2_UPDATE_DIR			 = _T("Download\\ESRW2\\");            // ES读写器2更新目录
static const TCHAR* const ES_RW3_UPDATE_DIR			 = _T("Download\\ESRW3\\");            // ES读写器3更新目录
static const TCHAR* const PM_RW_UPDATE_DIR			 = _T("Download\\PMRW\\");             // PM读写器更新目录
static const TCHAR* const CARD_RW_UPDATE_DIR         = _T("Download\\CARDRW\\");           // 卡式读写器更新目录
static const TCHAR* const ES_PM_UPDATE_DIR			 = _T("Download\\PM\\");               // 参数更新目录
static const TCHAR* const ES_BLANK_CARD_LIST_DIR     = _T("blankcardlist\\");					// 白卡清单存放路径
static const TCHAR* const ES_MC_DOWNLOAD_DIR		 = _T("PG\\MC\\");					   // 控制部程序下载目录
static const TCHAR* const ES_TPU_MAIN_DOWNLOAD_DIR	 = _T("PG\\TPU-MAIN\\");			   // TPU主控程序下载目录
static const TCHAR* const ES_TPU_APP_DOWNLOAD_DIR	 = _T("PG\\TPU-APP\\");				   // TPU应用部程序下载目录
static const TCHAR* const ES_PM_DOWNLOAD_DIR		 = _T("Download\\PM\\");			   // 参数下载目录
//////////static const TCHAR* const UPGRADE_DATA_DESC_FILE_NAME = _T("upgrade.description");		   // 升级文件说明文件



////////////////////定义常量与枚举///////////////////////////////
// 系统初值中定义的类型
typedef void*               CardBlacklistStruct_t;      // 卡黑名单数据
typedef void*               ProductBlacklistStruct_t;   // 车票黑名单数据
typedef void*               PayloadDetailsStruct_t;     // 清单有效荷载数据
typedef void*               SamBlacklistStruct_t;       // 含有SAM 丢失的开始和结束时间
typedef void*               FeedbackIdStruct_t;         // 反馈定义包括声音、文字和灯光指示。注意：不是这个结构的所有参数都总是有效的。
typedef void*               SoundIdStruct_t;            // 设定设备声音属性。
typedef void*               TimeZoneInfoStruct_t;       // 时区信息。
typedef void*               PayloadDetailsStruct_t;     // 清单有效荷载数据
typedef void*               SamBlacklistStruct_t;       // 含有SAM 丢失的开始和结束时间
typedef void*               SoundIdStruct_t;            // 设定设备声音属性。
typedef void*               FareCodeId_t;               // 未定义

// 核心基本类型
typedef char                Byte_t;
typedef unsigned long       DateC19_t;                  // 天数从1799年12月31日算起。0=“未设定”。1=1800年1月1日
typedef unsigned short      DateC20_t;                  // 天数从1899年12月31日算起。0=“未设定”。1=1900年1月1日
typedef char                ExceptionID_t[3];
typedef short               S16_t;
typedef long                S32_t;
typedef __int64             S64_t;         
typedef char                S8_t;         	
typedef unsigned short      U16_t;        	
typedef unsigned long	    U32_t;        
typedef unsigned __int64    U64_t;                  
typedef unsigned char	    U8_t;         
typedef U16_t               Time_Days_t;   	
typedef S32_t       	    Time_t;     

// 系统基本类型
typedef U8_t                ActionSequenceNumber_t;     // 用于排序的二进制值。类型属性的含义取决于子系统的使用。
typedef char                Address_t[41];              // 表示一个具体的街道地址。
typedef U32_t               ApplicationSerialNumber_t;  // 票卡上标识应用的号码。
typedef U32_t               AuditRegisterValue_t;       // 此类数据旨在存储计数器型或累加器型审核数据的值.
typedef U16_t               CalendarId_t;               // 日历标识。
typedef U16_t               CardBatchNumber_t;          // 票卡批号在票卡成批发行时写在票卡上的编号。
typedef char                CardholderName_t[61];       // ASCII 字符，用于标识持卡人的编号。	
typedef char                CardholderNum_t[19];   	    // 持卡人的唯一标识符
typedef U32_t               CardholderSerialNumber_t;   // 标识持卡人。如果值为0xFFFFFFFF ，则表示申请尚未个人化。
typedef char                CardholderTitle_t[5];       // 持卡人的头衔，如先生、女士、教授等。
typedef U32_t               CardIssuerId_t;             // 根据ISO/IEC 7812 域注册卡发行人: 000,000 到 999,999
typedef U32_t               CardSerialNumber_t;         // 以二进制值标识实际票卡。       
typedef char	            CartonId_t[10];             // 标识存有一张票卡的票箱的ASCII 字符
typedef U32_t               CdAttributeId_t;            // 判定BEI-00040 所说的CD 属性。
typedef char                CdAttributeName_t[51];      // CD属性的名称
typedef U16_t               CdPayloadDataVersion_t;     // 该类型指有效负载内的数据，当数据发生改变时增加。
typedef U16_t               CdPayloadFormatVersion_t;   // 有效负载的结构版本,当有效负载发生结构性改变（新属性、删除的属性、查找的附加级）时增加
typedef char                CdPayloadName_t[12];        // 唯一的CD 有效荷载的名称（12 字符）
typedef U16_t               CdSetVersion_t;             // CD集的版本。 
typedef char                Char8_t;                    // 用于一般系统使用的不带符号的字符类型。    	
typedef U8_t                CounterEight_t;             // 用于一般用途的八位计数器。
typedef U16_t               CounterSixteen_t;   	    // 通用的16 位计数器
typedef U16_t               DayTypeId_t;                  // 标识日子类型（如周末、假日，等）并在项目内可配置的位字段。
typedef U8_t                DeviceFeedbackId_t;         // 决定设备音频和／或视频输出。 
typedef U32_t               DeviceId_t;                 // 此号码在AFC 系统中唯一标识设备。此类型包含下列几个类型：HardwareType_t（占用最有效的16 位）HardwareSequenceNumber_t（占用最低有效的16 位）  	
typedef U16_t               Duration_t;   	            // 通过单位或值来表示持续时间。DurationUnit_t 占用最有效的4 位。DurationValue_t 占用最低有效的12 位。
typedef U32_t               DurationValue_t;            // 指出某事物的持续期间（例如，产品的持续期）。
typedef char                EmailAddress_t[129];        // 含电子邮件地址的使用类型
typedef U32_t               Fare_t;                     // 用来指定一个费用值。
typedef U16_t               FareCode_t;                 // 用来在费用表中确定某行的费用代码。 
typedef U16_t               FareCodeTableId_t;          // 标识将要用于费用计算的费用代码表。	
typedef U16_t               FarePatternId_t;   	        // 标识费用模式。
typedef U8_t                FareSetId_t;                // 费用设置标识符。
typedef U16_t               FareTableId_t;              // 费用表的唯一标识。
typedef U16_t               GraphicId_t;                // 用于辩识票卡上的图案的一个编号
typedef U16_t               HardwareSequenceNumber_t;   // 通过为硬件分配的顺序号标识AFC 系统中所用的硬件。
typedef U16_t               JourneyType_t;              // 标识费用情节或资助人旅行中所涉及的情节。 
typedef U16_t               KeyVersion_t;               // 用来确保文件安全的密钥的版本。
typedef U16_t               KeyNumber_t;                // 为加密系统使用而保留的密钥码。标识某个安全性密钥（不是其版本）。
typedef U16_t               LifeCycleCounter_t;         // 标识票卡“循环寿命”的二进制值。  	
typedef U32_t               LocationCode_t;             // 标识位置代码的二进制值。
typedef U32_t               LocationNumber_t;           // 通过类型和代码标识位置。“LocationType_t”储存在“LocationNumber_t”的最有效字节中。
typedef U32_t               StationNumber_t;            // 通过类型和代码标识位置。“LocationType_t”储存在“StationNumber_t”的最有效字节中。
typedef U16_t               NameRepresentationType_t;   // 用于包含关于编码类型(NameEncodingType_t)和名称特定变化(NameVariantType_t)的信息。
typedef U32_t               ParticipantId_t;            // 标识方案中的参与者。 
typedef U8_t                PassengerType_t;            // 这是对享受服务的人、物体或动物的分类。	
typedef char                Password_t[16];   	        // 用于一般口令储存的以Null 为终结符的ASCII 字符。
typedef char                PasswordClue_t[21];         // 储存口令线索的ASCII 字符，此口令线索可用来提醒人们他们的密码。
typedef U16_t               Percent_t;                  // 确定精确到2 个小数位的百分比(0..100)。
typedef U32_t               ProductIssuerId_t;          // 产品发行商标识
typedef U16_t               ProductSerialNumber_t;      // 这是一个在票卡的应用中标识产品的号码。
typedef U16_t               ProductType_t;              // 二进制值。确定了被用来产生交易的产品。这些都是对特定的项目而言的
typedef U16_t               ProductTypeVariant_t;       // 标识特定ProductType_t的变量
typedef U16_t               Ptsn_t;                     // 二进制值。一个产品情况特定顺序号码
typedef U16_t               ReceiptNum_t;               // 一般用途的收据号码。 
typedef U8_t                RefundLevel_t;              // 用来表示某个延期的退款程序的进行程度的一个二进制值。
typedef U8_t                Rides_t;                    // 乘坐次数 ，适用基于乘坐次数的产品
typedef U8_t                RoleType_t;                 // 定义操作员的角色。不对类进行列举。其值由CD 定义。
typedef U32_t               SamId_t;                    // 密码摸块（即SAM）的唯一标识
typedef U32_t               ShiftSequenceNumber_t;      // 标识特定操作员的轮班的号码，由SAM 维护。
typedef char                StaffCodeNumber_t[10];      // 表示员工编码数
typedef char                StaffId_t[10];              // ASCII 字符，在某个应用供应商范围内标识的某个工作人员（此时，此应用供应商通常是服务供应商）。
typedef char                TelephoneNumber_t[21];      // 储存电话号码（一般用途）的ASCII 字符。
typedef U8_t                TimeCode_t;                 // 时间期限的参考（通过CD 解释） 
typedef U8_t                TxnMac_t[8];                // 有关交易的安全信息。
typedef U8_t                UD_Subtype_t;               // 在数据源的源或类型中标识消息。
typedef U8_t                UD_Type_t;                  // 确定数据源内的类或源。
typedef U32_t               UDSNum_t;                   // 用量数据序列号。
typedef U16_t               ValidityDistance_t;         // 产品有效的地区或千米的数目。有效性单位由产品发行人确定。
typedef S32_t               ValueOneUnit_t;             // 现行货币最小单位表示的货币价值。
typedef U16_t               Version_t;                  // 二进制值，写入票卡的密钥的“SecurityKeyTable”的版本
typedef U8_t                Years_t;                    // 整年值的计数器。

/////////////////////////////// 以下为票卡交易处理相关数据定义///////////////////////////
//固定基本类型
typedef unsigned long        KeyTableVersionNumber_t;                // 加载进加密模块的最后一个密钥表的版本号码。
typedef unsigned long        SoftwareVersion_t;                      // 软件的版本号码。
typedef unsigned short       SystemReferenceId_t;                    // BinaryValue 二进制值
// 类型定义
typedef Time_t              TimeT;                      // 日期/时间。表示从1970 年1 月1 日 00：00 起的秒数（范围大约为68 年），0 = 未指定， 
// 1 = 1970 年1 月1 日00：01。注：与time_t 相同，除了数据类型是未标记的
typedef unsigned long       TimeM26_t;                  // 日期/时间。表示从2006 年1 月1 日 00：00 起的分钟数（范围大约为32 年），0 = 未指定， 
// 1 = 2006 年1 月1 日00：01。
typedef unsigned short      CardBaseDateTime_t;     // 定义Ultralight 卡初始化时的开始的日期/时间如下：
													// 0= 2006 年1 月1 日 00：00
													// 1= 2006 年2 月1 日 00：00
													// 2= 2006 年3 月1 日 00：00
typedef unsigned short      TimeOfDayMinutes_t;         // 定义今天从午夜开始的分钟数。范围：0..1440
typedef unsigned short      DateC26_t;                  // 日期。表示从2006 年1 月1 日起的分钟数（范围大约为179 年），0 = 未指定，1 =2006 年1 月2 日。
typedef unsigned short      CardLocation_t;             // 存储在卡上映射的位置代码。通过一个映射表映射到StationNumber_t，该表允许有4094 个位置，0 = 保留，0xFFF = 未指定。
typedef unsigned char       CardParticipant_t;          // 存储在卡上映射的参与方ID。通过一个映射表映射到ParticipantId_t，该表允许有253个参与方，0 = 保留，0xFF = 未指定。
typedef unsigned long       DeviceMode_t;               // 说明当前运营模式或设备模式的编码，其中：
														// 1  = 正常模式
														// 2  = 关闭模式
														// 3  = 列车故障模式
														// 4  = 进站免检模式
														// 5  = 出站免检模式
														// 6  = 紧急放行模式
														// 7  = 正常 ＋ 时间免检模式
														// 8  = 正常 ＋ 日期免检模式
														// 9  = 正常 ＋ 车费免检模式
														// 10 = 正常 ＋ 时间 ＋ 日期免检模式
														// 11 = 正常 ＋ 时间 ＋ 车费免检模式
														// 12 = 正常 ＋ 日期 ＋ 车费免检模式
														// 13 = 正常 ＋ 时间 ＋ 日期 ＋ 车费免检模式
														// 14 = 进站 ＋ 时间免检模式
														// 15 = 进站 ＋ 日期免检模式
														// 16 = 进站 ＋ 车费免检模式
														// 17 = 进站 ＋ 时间 ＋ 日期免检模式
														// 18 = 进站 ＋ 时间 ＋ 车费免检模式
														// 19 = 进站 ＋ 日期 ＋车费免检模式
														// 20 = 进站 ＋ 时间 ＋ 日期 ＋车费免检模式
														// 21 = 出站 ＋ 时间免检模式
														// 22 = 出站 ＋ 日期免检模式
														// 23 = 出站 ＋ 车费免检模式
														// 24 = 出站 ＋ 时间 ＋ 日期免检模式
														// 25 = 出站 ＋ 时间 ＋ 车费免检模式
														// 26 = 出站 ＋ 日期 ＋车费免检模式
														// 27 = 出站 ＋ 时间 ＋ 日期 ＋车费免检模式
														// 28 = 进站 ＋ 出站 ＋时间免检模式
														// 29 = 进站 ＋ 出站 ＋日期免检模式
														// 30 = 进站 ＋ 出站 ＋车费免检模式
														// 31 = 进站 ＋ 出站 ＋时间 ＋ 日期免检模式
														// 32 = 进站 ＋ 出站 ＋时间 ＋ 车费免检模式
														// 33 = 进站 ＋ 出站 ＋日期 ＋车费免检模式
														// 34 = 进站 ＋ 出站 ＋时间 ＋ 日期 ＋车费免检模式
typedef U8_t                CardPaymentMethod_t;
typedef U8_t                LogTransactionType_t;
typedef U8_t                JourneyStatus_t;
typedef bool                CardLanguageId_t;
typedef U8_t                LiteCardStatus_t;
typedef U8_t                S1kCardStatus_t;
typedef U8_t                S1kProductStatus_t;
typedef unsigned char       ValidityDurationType_t;     // 规定一个计次/定期产品的ValidityDuration 是否基于天数、小时数或者分钟数存储，0.1=保留，2 =分钟数，3 =小时数，4＝天数，5＝周数，6＝日历月数。    

// 交易日志数字
static const unsigned char  LOG_TRANSACTION_TYPE_MULTIRIDE_USE_ENTRY            = 17;
static const unsigned char  LOG_TRANSACTION_TYPE_MULTIRIDE_USE_EXIT             = 18;
static const unsigned char  LOG_TRANSACTION_TYPE_PASS_USE_ENTRY                 = 17;
static const unsigned char  LOG_TRANSACTION_TYPE_PASS_USE_EXIT                  = 18;

// 日期时间
static const unsigned long  DATE_C_19_MAXIMUM                                   = 0xFFFFFFFF;
static const unsigned char  DATE_C_19_UNSPECIFIED                               = 0;
static const unsigned char  MINS_255                                            = 0xFF;
static const unsigned short DATE_C_20_MAXIMUM                                   = 0xFFFF;
static const unsigned char  TIME_T_UNSPECIFIED                                  = 0;
static const unsigned char  DATE_C_20_UNSPECIFIED                               = 0;
static const unsigned char  TIME_M_26_UNSPECIFIED                               = 0;
// 未实现
static const unsigned short DAYS_BETWEEN_DATEC20_AND_DATEC26                    = 38717;

//概要
static const short          NUMBER_OF_AUDIT_REGISTERS                           = 300;
static const unsigned long  UD_FORMAT_VERSION                                   = 0x00020000;
static const unsigned long  LOCATION_NUMBER_UNSPECIFIED                         = 0x00000000;
static const unsigned long  VALUEONEUNIT_UNSPECIFIED                            = 0xFFFFFFFF;
static const unsigned char  PASSENGER_TYPE_UNSPECIFIED                          = 0xFF;
static const unsigned char  RIDES_UNSPECIFIED                                   = 0xFF;
static const unsigned short DURATION_UNSPECIFIED                                = 0xFFFF;

//有效期列举
static const unsigned char  VALIDITY_DURATION_UNIT_MIN                          = 0;
static const unsigned char  VALIDITY_DURATION_UNIT_HOUR                         = 1;
static const unsigned char  VALIDITY_DURATION_UNIT_DAY                          = 2;
static const unsigned char  VALIDITY_DURATION_UNIT_CALENDAR_MONTH               = 3;
static const unsigned char  VALIDITY_DURATION_UNIT_UNSPECIFIED                  = 0xFF;

//票卡列举
static const unsigned short CARD_VERSION_UNINITIALISED                          = 0;
static const unsigned short APPLICATION_SERIAL_NUMBER                           = 1;
static const unsigned short APPLICATION_STATUS_CODE_NOT_BLOCKED                 = 1;
static const unsigned char  AUTOLOAD_PRODUCT_ID_UNSPECIFIED                     = 0xFF;
static const unsigned short MIN_TRANSACTION_SEQUENCE_NUMBER                     = 0;
static const unsigned short REFUND_LEVEL_UNSPECIFIED                            = 0;
static const unsigned short RECEIPT_NUMBER_UNSPECIFIED                          = 0;
static const TCHAR* const    CARDHOLDER_NUMBER_UNSPECIFIED                       = _T("000000000000000000");
static const TCHAR* const    CARDHOLDER_NAME_UNSPECIFIED                         = _T("");
static const unsigned short MOC_INDUSTRY_CODE                                   = 5320;
static const unsigned char  CARD_LIFECYCLE_INITIAL_VALUE                        = 0;
static const unsigned short MAX_CARD_BASE_DATE_TIME                             = 511;

//行程产品ID 列举
static const unsigned char  JOURNEY_PRODUCT_ID_PURSE                            = 0;
static const unsigned char  JOURNEY_PRODUCT_ID_PASS_MULTIRIDE                   = 1;
static const unsigned char  JOURNEY_PRODUCT_ID_UNSPECIFIED                      = 0xFF;

// 票卡旅程状态编码
static const unsigned char  JOURNEY_NONE                                        = 0;
static const unsigned char  JOURNEY_CHECKED_IN                                  = 1;
static const unsigned char  JOURNEY_CHECKED_OUT                                 = 2;
static const unsigned char  JOURNEY_CHECKED_OUT_TRAIN_FAULT                     = 3;
static const unsigned char  JOURNEY_CHECKED_IN_COMPENSATION_FARE_PAID           = 4;

// 销售常量
static const unsigned char  MAX_NUMBER_PRODUCTS_FOR_CARD_ENQUIRY                = 10;
static const unsigned char  REFUND_LEVEL_PROCESS_STARTED                        = 1;
static const unsigned char  MAX_NUMBER_PRODUCT_HISTORY_FOR_CARD_ENQUIRY         = 10;

// 其他常量
static const U16_t KEY_VERSION_UNSPECIFIED                                      = 0xFFFF;
static const U16_t KEY_NUMBER_UNSPECIFIED                                       = 0xFFFF;
static const U32_t UNSPECIFIED_CARD_PARTICIPANT                                 = 0xFFFFFFFF;
static const U16_t LIFE_CYCLE_COUNTER_UNSPECIFIED                               = 0xFFFF;
static const U32_t CARD_SERIAL_NUMBER_UNSPECIFIED                               = 0xFFFFFFFF;
static const DateC20_t      UNSPECIFIED_DATEC20                                 = 0xFFFF;
static const U32_t          UDSNUM_UNSPECIFIED                                  = 0xFFFFFFFF;

// CD版本无效的判断值
static const unsigned long UNAVALIBLE_VERSION     = 0xFFFFFFFF;


///////////////////////////////// 从这里开始是系统编码规则的定义///////////////////////////////////////////

// 发行商编码定义
typedef enum _tagISSUANCER_CODE{
	ACC = 0x04,		// ACC
	ECT = 0x01		// YKT
} ISSUANCER_CODE;
// 业务状态
typedef enum _service_state {
	INIT						= 0x00,         // 初始中（系统初始化）
	OUT_OF						= 0x01,         // 中止中 （招援时）
	READY						= 0x02,         // 等待中 （等待乘客操作或操作员操作）
	RECEPT						= 0x03,         // 操作中 （乘客操作）
	COUNTING					= 0x04,			// 计数中  检测到有现金投入时）
	ISSUEING					= 0x05,         // 设备运用中（发票处理中等硬件动作执行的时候）
	CANCELING					= 0x06,			// 返回中 （用户取消操作或计数超时时）
	MAINTENANCE					= 0x07,         // 维护中 （操作员登录后）
	ABNORMAL					= 0x08,         // 异常中 （TVM发生异常后）
	REST						= 0x09,         // 待机中 （广告播放时）
	FINISH						= 0x0A,         // 结束	  （售票或充值完成后）
	WAIT_MAINTENANCE			= 0x0B,         // 等待维护中（维护门开，但未登录）
} SERVICE_STATE;

// 设备类型
typedef enum _afc_device_type{
	DEVICE_TYPE_TVM					=	0x16,		// 自动售票机 
	DEVICE_TYPE_TOM					=	0x14,		// 半自动售票机
	DEVICE_TYPE_AVM					=	0x19,		// 自动充值机
	DEVICE_TYPE_VM					=	0x04,		// 代理半自动售票机
	DEVICE_TOM						=	DEVICE_TYPE_TOM,
	DEVICE_VM						=	DEVICE_TYPE_VM,	//为了兼容郑州项目
	DEVICE_TYPE_AG					=	0x1F,		// 自动检票机
	DEVICE_TYPE_HT					=	0x23,		// 手持检票机
	DEVICE_TYPE_ES					=	0x3D,		// 编码分拣机
	DEVICE_TYPE_PM					=	0x3E,		// 个性化打印机
	DEVICE_TYPE_CCS_SERVER			=	0x62,		// 线路中心服务器
	DEVICE_TYPE_SC_SERVER			=	0x5d,		// 车站中心服务器
	DEVICE_TYPE_CCS_WS				=	0x81,		// 线路中心工作站
	DEVICE_TYPE_SC_WS				= 0x82,		// 车站中心工作站
	DEVICE_TYPE_ROUTE				=	0x51,		// 路由器
	DEVICE_TYPE_SWITCH				=	0x52,		// 交换机
	DEVICE_TYPE_UNKNOW				=	0xFF		// 未定义
}AFC_DEVICE_TYPE;

// 设备子类型
typedef enum _device_subtype{
	DEVICE_SUBTYPE_AGM_NORMAL		=	0x01,	// 通用闸机
	DEVICE_SUBTYPE_AGM_WIDE			=	0x02,	// 宽通道闸机
	
	DEVICE_SUBTYPE_ES_CARD			=	0x01,	// 卡式ES
	DEVICE_SUBTYPE_ES_TOKEN			=	0x02,	// TOKEN式ES
	DEVICE_SUBTYPE_PM				=	0x04,	// PM
	DEVICE_SUBTYPE_ES_CARD_PM		=	0x05,	// 卡式ES & PM
	DEVICE_SUBTYPE_ES_TOKEN_PM		=	0x06,	// TOKEN式ES & PM
	DEVICE_SUBTYPE_UNKNOW			=	0x00 	// 未定义
}DEVICE_SUBTYPE;

//  语言编码定义（ ISO 639-2 ）
typedef enum _languageType{  
	MODE_UNKNOWN						= 0,		//无语言集
	LANGUAGE_ID_CHINESE				= 2245,	// 简体中文
	LANGUAGE_ID_ENGLISH				= 3751,	// 英语
	LANGUAGE_ID_PORTUGUESE			= 11224,// 葡萄牙语
	LANGUAGE_ID_CANTONESE			= 17451,// 粤语
	LANGUAGE_ID_UNSPECIFIED			= 65535,	// 未定义
	CHINESE=LANGUAGE_ID_CHINESE,
	ENGLISH=LANGUAGE_ID_ENGLISH,
	PORTUGUESE=LANGUAGE_ID_PORTUGUESE,
	TRADITIONAL_CHINESE=LANGUAGE_ID_CANTONESE
}LANGUAGETYPE_T;

// 发送接收模式定义
typedef enum _send_mode{
	MODE_SEND		= 1,
	MODE_RECEIVE    = 2
}SEND_MODE;


// 票箱指定类型
typedef	enum Magazine_type
{
	MAGAZINE_NONE =	0x0000,			   // 不指定票箱
	MAGAZINE_A	  =	0x0001,			   // 票箱A出票
	MAGAZINE_B	  =	0x0002,			   // 票箱B出票
	MAGAZINE_R	  =	0x0003,			   // 回收箱
} MAGAZINE_TYPE;

// acc车票状态
//typedef enum acc_ticket_statue
//{
//	INIT_T = 1,			        //初始化
//	SALE_T = 2,					//已售卖 
//	ENTER_T = 3,				//已进站
//	ENTER_UPDATE_T = 4,			//进站类更新
//	EXIT_T = 5,					//已出站 
//	EXIT_UPDATE_T= 6,			//出站类更新
//	REFUND_T = 7,				//已退款
//	OVER_T = 8,					//已注 
//	UNKNOW_T = 9,				//未知
//}ACC_TICKET_STATUE;
// ECT车票状态
//typedef enum _ect_ticket_statue
//{
//	UNUSETICKET = 1,			        //未启用
//	USE = 2,					//启用 
//	STOP = 3,	    			//停用
//	BLACKET = 4,				//黑名单卡
//	RECYCLED = 5,				//回收 
//	
//}ECT_TICKET_STATUE;

// 收支类型定义
typedef enum business_type{		
	BUSINESS_TYPE_NONE = 0,        // 默认
	BUSINESS_TYPE_INCOME = 1,	   // 收入
	BUSINESS_TYPE_EXPENSE = 2,     // 支出
}BUSINESS_TYPE;	

// 数据导出数据类型定义
typedef	enum _export_datatype
{
	PAM_TYPE = 0x01,
	TXN_TYPE = 0x02,
	BUS_TYPE = 0x03,
	UNKNOW_TYPE   = 0xFF //未知
} EXPORT_DATA_TYPE;

// 1.系统编码规则 的定义结束
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 从这里开始是出票机相关的定义



//////////////////////////////////////////////////////////////////////////
// 从这里开始是	3.IC R/W I/F式样书 的定义
// SAM槽编号
typedef	enum _slot_id
{
	SLOT1 =	0x00,									// 一卡通PSAM卡号
	SLOT2 =	0x01,									// 一卡通ISAM卡号
	SLOT3 =	0x02,									// 地铁PSAM卡号
	SLOT4 =	0x03									// 地铁ISAM卡号
} SLOT_ID;

// SAM种类定义
typedef enum _sam_type {
	RW_ACC_ISAM = 0x01,
	RW_ACC_PSAM = 0x02,
	TH_ACC_ISAM = 0x03,
	TH_ACC_PSAM = 0x04,
	RW_ECT_ISAM = 0x05,
	RW_ECT_PSAM = 0x06,
	TH_ECT_ISAM = 0x07,
	TH_ECT_PSAM = 0x08
}SAM_TYPE;

// SIM ID信息
typedef struct _sam_id_info{
	BYTE rw_acc_psam_id[6];
	BYTE rw_acc_isam_id[6];
	BYTE rw_ect_psam_id[6];
	BYTE rw_ect_isam_id[6];
	BYTE th_acc_psam_id[6];
	BYTE th_acc_isam_id[6];
	BYTE th_ect_psam_id[6];
	BYTE th_ect_isam_id[6];

	_sam_id_info(){
		memset(rw_acc_psam_id, 0, 6);
		memset(rw_acc_isam_id, 0, 6);
		memset(rw_ect_psam_id, 0, 6);
		memset(rw_ect_isam_id, 0, 6);
		memset(th_acc_psam_id, 0, 6);
		memset(th_acc_isam_id, 0, 6);
		memset(th_ect_psam_id, 0, 6);
		memset(th_ect_isam_id, 0, 6);
	}
}SAM_ID_INFO;

// 设备部件类型
typedef enum _device_component{
	BOM_DEVICE_APPLI		 = 0x20, // BOM主控部	    0x20
	BOM_DEVICE_CUSTOMER_LCD  = 0x21, // BOM乘客显示屏	0x21
	BOM_DEVICE_OFFICER_LCD	 = 0x22, // BOM售票显示屏	0x22
	BOM_DEVICE_TICKET_MODULE = 0x23, // BOM车票处理模块	0x23
	BOM_DEVICE_PRINTER       = 0x24, // BOM单据打印机	0x24
	BOM_DEVICE_CASH_BOX      = 0x25, // BOM现金模块	    0x25
	BOM_DEVICE_OUT_TPU       = 0x26, // BOM桌面TPU	    0x26
	BOM_DEVICE_INNER_TPU     = 0x27, // BOM内部TPU	    0x27
	BOM_DEVICE_CAMERA        = 0x28, // BOM摄像头       0x28
	BOM_DEVICE_CARDPRINTER   = 0x29, // BOM票卡打印机   0x29
	BOM_DEVICE_SCANNER       = 0x2A, // BOM扫描仪       0x28
	BOM_DEVICE_LASERPRINTER  = 0x2B, // BOM镭射打印机   0x29

	TCM_DEVICE_APPLI		 = 0x20, // TCM主控部	    0x20
	TCM_DEVICE_CUSTOMER_LCD  = 0x21, // TCM乘客显示屏	0x21
	TCM_DEVICE_OFFICER_LCD	 = 0x22, // TCM售票显示屏	0x22
	TCM_DEVICE_TICKET_MODULE = 0x23, // TCM车票处理模块	0x23
	TCM_DEVICE_PRINTER       = 0x24, // TCM单据打印机	0x24
	TCM_DEVICE_CASH_BOX      = 0x25, // TCM现金模块	    0x25
	TCM_DEVICE_OUT_TPU       = 0x26, // TCM桌面TPU	    0x26
	TCM_DEVICE_INNER_TPU     = 0x27, // TCM内部TPU	    0x27
	TCM_DEVICE_CAMERA        = 0x28, // TCM摄像头       0x28
	TCM_DEVICE_CARDPRINTER   = 0x29, // TCM票卡打印机   0x29
	TCM_DEVICE_SCANNER       = 0x2A, // TCM扫描仪       0x28
	TCM_DEVICE_LASERPRINTER  = 0x2B, // TCM镭射打印机   0x29

	ACC_PSAM_RW				 = 0xA1, // 外部读写器一票通PSAM
	ACC_ISAM_RW				 = 0xA2, // 外部读写器一票通ISAM
	ECT_PSAM_RW				 = 0xA3, // 外部读写器一卡通PSAM
	ECT_ISAM_RW				 = 0xA4, // 外部读写器一卡通ISAM
	DEVICE_COMPOENT_UNKNOW = 0XFF
}DEVICE_COMPONENT;
// 更新建议
typedef enum _update_suggest{
	NO_UPDATE = 0x00,                // 该车票不需要补票
	ENTER_STATION_UPDATE_81 =  0x81,   // 进站更新(本站20分钟内进站)
	ENTER_STATION_UPDATE_82 =  0x82,           // 请发行新票
	ENTER_STATION_UPDATE_83 = 0x83,    // 进站更新(本站20分钟外进站)
	ENTER_STATION_UPDATE_84 = 0x84,    // 进站更新(非本站进站)
	ENTER_STATION_UPDATE_85 = 0x85,      // 补延迟出站(紧急模式)
	ENTER_STATION_UPDATE_86 =  0x86,   // 进站更新(紧急模式出站)
	ENTER_STATION_UPDATE_87 =  0x87,   // 进站更新(列车故障模式发售)
	ENTER_STATION_UPDATE_88 =  0x88,   // 进站更新(列车故障模式出站)
	OUT_STATION_UPDATE_21 = 0x21,    // 补延迟进站，无进站标志，默认发售车站为进站车站
	OUT_STATION_UPDATE_22 = 0x22,    // 补延迟进站，无进站标志，需要按照乘客描述或工作人员判断选择进站车站，对车票写入进站码
	OUT_STATION_UPDATE_23 = 0x23,    // 补延迟进站(进站免检模式)
	OUT_STATION_UPDATE_24 = 0x24,        // 超时更新
	OUT_STATION_UPDATE_25 = 0x25,    // 补延迟进站
	OUT_STATION_UPDATE_26 = 0x26,        // 超程补票
	OUT_STATION_UPDATE_27 = 0x27,        // 超程补票->发行出站票
	OUT_STATION_UPDATE_28 = 0x28, // 超程/超时补票
	OUT_STATION_UPDATE_29 = 0x29,      // 发行出站票
	OUT_STATION_UPDATE_2A =  0x2A            // 发行出站票
}UPDATE_SUGGEST;
typedef	enum test_no
{
	TEST_NUM_LED                = 0x1100,	// // LED test
	TEST_NUM_MOTOR				= 0x1300,	// Motor test
	TEST_NUM_RESET				= 0x1400,	// Reset test
	TEST_NUM_RW					= 0x1600,	// R/W test
	TEST_NUM_CONT_CSC_ISSUE		= 0x1700,	// Continuous CSC issue	test
	TEST_NUM_SENSOR_SWITCH		= 0x2000,	// Sensor/Switch Test
	TEST_NUM_UNKNOWN			= 0xFFFF	// unknown
} TEST_NO;

// 3.IC	R/W	I/F式样书 的定义结束
//////////////////////////////////////////////////////////////////////////

// 票卡打印机状态定义
typedef enum _pmStatus{
	PM_STATUS_UNKNOW  = 0x00, // 未知状态
	PM_STATUS_OFF	  = 0x50, // 关闭状态
	PM_STATUS_ON	  = 0xD0, // 上电状态
	PM_STATUS_READY   = 0x18  // 就绪状态
}PM_STATUS;

//////////////////////////////////////////////////////////////////////////
// 从这里开始是	4.CPS I/F式样书	的定义
// 5.1进站设备状态TK_TRANS_EntryEQState
//typedef	enum _TK_TRANS_EntryEQState
//{
//	NO_WORK	  =	0x00,									// 无意义
//	NORMAL	  =	0x01,									// 正式
//	TEST	  =	0x02,									// 测试
//	MAINTAIN  =	0x03,									// 维修
//	NO_USE    = 0xFF
//} TK_TRANS_EntryEQState;


// 初期化错误
typedef	enum _init_detail_error {
	INI_ERROR						= 0x00000001,			// 初期化INI文件错误
	HOME_FOLDER_ERROR				= 0x00000002,			// 创建数据主目录错误
	BACKUP_FOLDER_ERROR				= 0x00000004,			// 创建数据备份目录错误
	OPERATION_PARAMETERA_ERROR		= 0x00000008,			// Operation parameter A错误
	DEVICE_TXNDATA_ERROR					= 0x00000010,			// 初期化交易数据错误
	DEVICE_MASTER_ERROR					= 0x00000020,			// 非参数类master错误
	INIT_TRACE_ERROR				= 0x00000040,			// 初期化trace有错
	INIT_DATACOUNT_ERROR			= 0x00000080,			// 初期化集计错误
	NTP_ERROR						= 0x00000100,			// NTP同步本地时间错误
	PARAMETER_ERROR					= 0x00000200,			// 参数错误（不含非参数类master）
	VFARE_ERROR						= 0x00000400,			// 票价表错误
	INIT_RW_ERROR					= 0x00000800,			// 连接读写器错误
	INIT_TH_ERROR					= 0x00001000,			// 检测并初始化出票机错误
	INIT_PRINT_ERROR				= 0x00002000,			// 初期化打印机错误
	INIT_CAMERA_ERROR		        = 0x00003000,           // 初期化摄像头错误
	BOX_CHANGE_ERROR				= 0x00004000,			// 票箱更换检查错误
	ADD_NEW_DATACOUNT_ERROR			= 0x00008000,			// 创建一个新日期集计文件
	ACC_ACCREDIT_ERROR				= 0x00010000,			// ACC一票通ISAM认证错误
	OVER_TIME_DATA_ERROR			= 0x00020000,			// 存在超过期限未送数据错误
	BMAC_ACCREDIT_ERROR				= 0x00040000,			// 一卡通认证错误
	INI_DATABASE_ERROR				= 0x00080000,			// 数据库初始化错误
	INIT_STATUS_DISPLAY_ERROR				= 0x00100000,			// 状态显示屏初始化错误
} INIT_DETAIL_ERROR;

// 5.69	//	operator login status
typedef	enum _login_off_flag
{
	LOG_IN  = 0x00,				  // login
	LOG_OFF = 0x01				  // logoff
} LOGIN_OFF_FLAG;


// 5.86	Magazine data
typedef	enum _operation_type
{
	ATTACH			= 0x01,				  // ATTACH
	DETACH			= 0x02,				  // DETACH
	UNUSE			= 0xFF				  // unuse
} TICKETBOX_OPERATION_TYPE;
								  

// 5.14Magazine	1 status
typedef	enum _magazine1_status
{
	MAG_NORMAL	        = 0x00,		// normal
	MAG_NEAR_EMPTY      = 0x01,		// near	empty
	MAG_EMPTY		    = 0x02		// empty
} MAGAZINE1_STATUS;

// 设备供应商ID
static const WORD  FOUNDER_ID = 0x0011;

// Collection_Box
typedef	enum _collection_box
{
	LESS_FULL			  =	0x00,		  //  less than	nearly full
	NEARLY_FULL			  =	0x03,		  //  nearly full		 
	IS_FULL				  =	0x04		  //  full					   
} COLLECTION_BOX;											  

// IN_OFF_SVC
typedef	enum _in_off_sev
{
	IN_SVC				  =	0x00,		   //  In service
	IN_SVC_WARNING		  =	0x01,		   //  In service (with	warning)						 
	OUT_SVC_NORMAL		  =	0x02,		   //  Out of service (under normal	conditions)			 
	OUT_SVC_ERROR		  =	0x03,		   //  Out of service (under error conditions)			 
	OUT_SVC_MAINTENANCE	  =	0x04		   //  Out of service (under maintenance)					 
} IN_OFF_SVC;

// Close_Start_flag
typedef	enum _close_start_flag
{
	CS_NORMAL 	            = 0x00,			// Normal
	CS_CLOSE			    = 0x01,			// Closing sequence
	CS_RESTART				= 0x02,			// Restart sequence
	CS_SHUTDOWN			    = 0x03,				// shut	down
	CS_EXIT				= 0x04,			//退出程序不关机
	CS_EXITBOM				=CS_EXIT,
	CS_EXITTCM				= CS_EXIT			//退出TCM程序不关机
} CLOSE_START_FLAG;

// 版本的的数据结构
typedef struct _SC_VER_INFO
{
	TCHAR type[10];                  // TH型式
	TCHAR version[2];                // TH版本
	ULONG sum;                      // SUM值

	_SC_VER_INFO()
	{
		memset(type, 0x00, 10);
		memset(version, 0x00, 2);
		sum = 0;
	}
} SC_VER_INFO, *LPSC_VER_INFO;	//郑州新加

// TH模式定义
typedef	enum 
{
	TH_MODE_NORMAL		= 0x00,				// 正常模式
	TH_MODE_ABNORMAL	= 0x01				// 缩退模式
} TH_MODE;

// TH设备设置状态分类
typedef	enum set_mode
{
	SET		= 0x01,				  // 设置
	UN_SET	= 0x00				  // 未设置
} SET_MODE;

//Is_Authentication
typedef	enum _is_authentication
{
	AUTHENTICATED		 = 0x01,			//	Authenticated
	UNAUTHENTICATED		 = 0x02				//	Unauthenticated
} IS_AUTHENTICATION;
// 4.CPS I/F式样书 的定义结束
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 从这里开始是	5.车票encode式样书 的定义


// 5.车票encode式样书 的定义结束
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 从这里开始是	6.一卡通式样书 的定义
// 5.7 Card_Trans_Status



// 允许退卡标识
typedef	enum _refund_type
{ 
	REFUND_CARD     = 0x01,				// 退卡
	REFUND_MONEY    = 0x03				// 退资
} REFUND_TYPE;


// 设备测试状态
typedef	enum _equip_status
{
	STATUS_NORMAL =	0x00,			// 正常状态
	NORMAL_TEST	  =	0x01,			// 上次正常设备操作，本次测试设备操作
	TEST_NORMAL	  =	0x10,			// 上次测试设备操作，本次正常设备操作
	TEST_TEST	  =	0x11			// 上次测试设备操作，本次测试设备操作	
} EQUIP_STATUS;


// 一卡通交易类型
typedef	enum _ykt_trans_type
{
	//YKT_TRTYPE_FREE	                = 0x00,             // 不扣款
	//YKT_TRTYPE_ISSUE	            = 0x01,             // 售卡
	//YKT_TRTYPE_CHARGE	            = 0x02,             // 充值
	//YKT_TRTYPE_CONSUME	            = 0x06,             // 消费
	//YKT_TRTYPE_UPDATE	            = 0x07,             // 修复
	//YKT_TRTYPE_ADJUST	            = 0x08,             // 补票
	//YKT_TRTYPE_CASH	                = 0x09,             // 现金
	//YKT_TRTYPE_BLOCK	            = 0x0A,             // 锁卡
	//YKT_TRTYPE_REFUND	            = 0x0E,             // 退资
	//YKT_TRTYPE_SURRENDER	        = 0x0F,             // 退卡
	//YKT_TRTYPE_EXCHANGE             = 0x10,             // 换卡
	//YKT_TRTYPE_VALUE_SHIFT1         = 0x11,             // 移资
	//YKT_TRTYPE_VALUE_SHIFT2         = 0x12,             // 损换移资
	//YKT_TRTYPE_INNER_DISCOUNT       = 0x16,             // 联乘优惠交易(行业内)
	//YKT_TRTYPE_DISCOUNT             = 0x26              // 联乘优惠交易(行业间)
} YKT_TRANS_TYPE;

//// 一票通票状态
//typedef	enum _ypt_ticket_status
//{
//	NORMAL_TICKET	 = 0x00,				// 正常乘车类车票
//	TEST_MAINTENENCE = 0x01					// 维护测试类车票
//} YPT_TICKET_STATUS;
// 6.一卡通式样书 的定义结束


//////////////////////////////////////////////////////////////////////////
////定义票卡某个应用的处理状态。
//typedef enum _processingStatus_t{  
//	PROCESSING_STATUS_RECEIVED                  = 1,        // 已经收到应用。
//	PROCESSING_STATUS_APPROVED                  = 2,        // 已经批准应用。
//	PROCESSING_STATUS_REJECTED                  = 3,        // 已经拒绝应用。
//	PROCESSING_STATUS_UNSPECIFIED               = 255       // 无设定值。
//}ProcessingStatus_t;
//
////确定那些可通过黑名单对产品采取的操作。
//typedef enum _productActionCode_t{  
//	PRODUCT_ACTION_CODE_BLOCK                   = 1,        // 锁定产品。
//	PRODUCT_ACTION_CODE_UNBLOCK                 = 2,        // 解锁产品。
//	PRODUCT_ACTION_CODE_UNSPECIFIED             = 15        // 未指定的动作代码
//}ProductActionCode_t;

////某项目从票卡上被移走的原因。
//typedef enum _removalReason_t{  
//	REMOVAL_REASON_INVALID                   = 1,       // 无效的移走理由。
//	REMOVAL_REASON_GARBAGE_COLLECTED         = 2,       // 当产品从卡上被移走时，发送进“TxnProductDelete_t” UD
//	REMOVAL_REASON_UNSPECIFIED               = 255      // 未指定。
//}RemovalReason_t;


//////////////////////////////////////////////////////////////////////////
// 从这里开始是	7.其他 的定义


//销售统计
typedef	enum _SALESTAT_KIND
{
	CURRENT_DATE = 1,	  //全天
	STAFF_ONCE	 = 2	  //个人
} SALESTAT_KIND;

// 7.其他 的定义结束

typedef enum _status_type{
	STATUS_TYPE_FULL = 0x00,
	STATUS_TYPE_THREESTATUS = 0x01,
}STATUS_TYPE;

// 此项说明票卡/应用/产品帐户的状态。
typedef enum _accountState_t{
	ACCOUNT_STATE_UNINITIALISED         = 1,     // 未初始化。
	ACCOUNT_STATE_INITIALISED           = 2,     // 帐户被初始化。
	ACCOUNT_STATE_ISSUED                = 3,     // 帐户已被发行。
	ACCOUNT_STATE_REPLACED              = 4,     // 已发生替换。
	ACCOUNT_STATE_REFUND_IN_PROGRESS    = 5,     // 正在对帐户进行退款。
	ACCOUNT_STATE_REFUNDED              = 6,     // 帐户已被退款。
	ACCOUNT_STATE_CLOSED_OFF            = 7,     // 帐户被关闭。
	ACCOUNT_STATE_REVERSED              = 8,     // 帐户被回滚。
	ACCOUNT_STATE_UNSPECIFIED           = 255    // 未设置值。
} AccountState_t;

//定义M1票卡的状态
typedef enum _cardStatusCode_t_M1{    
	CARD_STATUS_CODE_NOT_BLOCKED                    = 1,    // 此卡未被锁定。
	CARD_STATUS_CODE_BLOCKED_OTHER                  = 2,    // 此卡已被锁定，由于其他未列明的原因。
	CARD_STATUS_CODE_BLOCKED_SURRENDERED_CARD       = 3,    // 此卡被锁定，因为持卡人退卡或闸机回收票卡。
	//PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM       = 4,    // 产品因为在黑名单SAM 上使用而锁定。
	CARD_STATUS_CODE_BLOCKED_FAULTY_CARD            = 5,    // 坏卡锁定。
	CARD_STATUS_CODE_BLOCKED_NOT_ISSUED             = 6,    // 此卡已被锁定，因为它还未被发行。
	//PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT   = 8,    // 产品被锁定，因为银行坏帐。
	//PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED = 9,    // 产品被锁定，因为银行帐户已关闭。
	//PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT  = 10,   // 产品被锁定，因为超期。
	CARD_STATUS_CODE_BLOCKED_LOST_STOLEN            = 11,   // 票卡因遗失／被盗而被锁定。
	CARD_STATUS_CODE_BLOCKED_CARD_BATCH             = 12,   // 成批锁定。
	CARD_STATUS_CODE_BLOCKED_OVER_TIME              = 13,   // 因为生命周期计数到达允许的限制值而被锁定。
	CARD_STATUS_CODE_BLOCKED_INACTIVE               = 14,   // 因为12 个月未用，该卡被锁定。
	CARD_STATUS_CODE_UNSPECIFIED                    = 15,    // 无设定值。
	CARD_STATUS_CODE_INITIALISED_NOT_YET_ISSUED     = 110    // 此卡没有初始化
}CardStatusCode_t_M1;

typedef enum _productStatus_t_M1
{
	PRODUCT_STATUS_CODE_NOT_BLOCKED                 = 1,    //产品未被锁定
	PRODUCT_STATUS_CODE_BLOCKED_OTHER               = 2,    //产品因未列明的其他原因被锁定
	PRODUCT_STATUS_CODE_BLOCKED_HOTLISTED_SAM       = 4,    //产品因在有列入黑名单的SAM的设备上使用而锁定
	PRODUCT_STATUS_CODE_BLOCKED_BAD_AUTOLOAD_DEBT   = 8,    //产品因坏帐被锁定
	PRODUCT_STATUS_CODE_BLOCKED_BANK_ACCOUNT_CLOSED = 9,    //产品被锁定，因为银行帐户已关闭
	PRODUCT_STATUS_CODE_BLOCKED_PRODUCT_OVER_LIMIT  = 10,   //产品被锁定，因为超期
	PRODUCT_STATUS_CODE_UNSPECIFIED                 = 15    //无设定值

}ProductStatus_t_M1;
//定义UL票卡的状态
typedef enum _cardStatusCode_t_UL{
	CARD_STATUS_CODE_UNSEND					    	= 0,    // 未售
	CARD_STATUS_CODE_SEND					        = 3,    // 已售
	CARD_STATUS_RECYCLE		                        = 7,    // 回收	
	CARD_STATUS_BLACKLIST_BLOCK                      = 9,    // 黑名单锁定
}CardStatusCode_t_UL;

// 卡片种类 0x01：一卡通CPU卡片；0x02：一卡通M1卡片；
//			0x11：一票通CPU卡片；0x12：一票通UL卡； 0x13：一票通M1卡片
typedef enum _cardType_t{
	CARD_TYPE_ACC_UNDEFINE       = 0xFF,
	CARD_TYPE_ACC_UL             = 0x00,
	CARD_TYPE_ACC_M1             = 0x01, 
	CARD_TYPE_ACC_M4			 = 0x02,
	CARD_TYPE_ACC_CPU            = 0x04,
	CARD_TYPE_ECT_M1,       
	CARD_TYPE_ECT_CPU,     
	//沈阳卡片种类定义
	CARD_TYPE_SY_ECT			= 0x01,//一卡通
	CARD_TYPE_SY_ECT_CPU		= 0x02,//一卡通CPU卡
	CARD_TYPE_SY_EPT			= 0x11,//一票通
	CARD_TYPE_SY_EPT_UL			= 0x12,//一票通UL卡
	CARD_TYPE_SY_EPT_CPU		= 0x13,//一票通CPU卡
}CardType_t;

// 手续费
typedef enum _cardholderFeeType_t{
	CARDHOLDER_FEE_TYPE_REPLACEMENT      = 2,     // Fee TCHARged for replacement.
	CARDHOLDER_FEE_TYPE_REFUND           = 5,     // Fee TCHARged for a refund.
	CARDHOLDER_FEE_TYPE_PERSONALISATION  = 13,    // Fee TCHARged for personalisation.
	CARDHOLDER_FEE_TYPE_UNSPECIFIED      = 255    // No value set.无设定值
}CardholderFeeType_t;

typedef enum _cdPayloadId_t{     
	CD_PAYLOAD_ID_SYSTEM_PARAMS                      = 1,       // 对于所有服务提供商而言，用于整个系统范围内参数值适用于所有设备类型的参数的CD 实体。
	CD_PAYLOAD_ID_SERVICE_PROVIDER_PARAMS            = 2,       // 用于整理参数值已针对特定服务提供商进行了自定义的参数的CD 实体，但此值适用于属于此服务提供商的所有设备类型。
	CD_PAYLOAD_ID_CARD_ACTIONLIST                    = 6,       // 用来对个别票卡包括采取行动（包括列入黑名单）的CD 实体，适用于将被阻止正常使用的票卡范围。
	CD_PAYLOAD_ID_PRODUCT_ACTIONLIST                 = 8,       // 适用于包含票卡产品细节的产品黑名单的CD 实体，且此票卡产品要求票卡接口设备采取特别行动。
	CD_PAYLOAD_ID_SAM_HOTLIST                        = 9,       // CD 实体包含要求票卡接口设备采取特别行动的加密模块标识符清单。特定情况下，它将列出丢失或盗取的加密模块，此加密模块有可能以欺骗手段对乘客票卡添加价值。
	CD_PAYLOAD_ID_CALENDARS                          = 17,      // 通过指定参考日期，用于确定特定天数的日子类型特征的CD 实体。
	CD_PAYLOAD_ID_FARE_TABLES                        = 18,      // 费用表是货币费用价值的主要区分符。它们将储存代表将在费用财务交易中被收取的金额的货币价值。
	CD_PAYLOAD_ID_FARE_PATTERNS                      = 19,      // 费用模式是货币费用价值解决费用或产品价格计算中乘客类型依赖性的主要方式。
	CD_PAYLOAD_ID_FARE_CODES                         = 20,      // 费用代码表允许将标准或特征的集合转换进能用来确定相关费用表的费用价值的费用代码。
	CD_PAYLOAD_ID_DAY_TYPES                          = 47,      // 时间段CD 目标提供识别一天中具有商业规则意义的时间段集合的机制。
	CD_PAYLOAD_ID_LOCATION_NAMES                     = 63,      // 本有效负载包含一张位置类别名字表格。
	CD_PAYLOAD_ID_PRODUCT_TYPE_PARAMS                = 64 ,     // 通过详述无需存于本票卡上的额外属性，本CD有效负载扩展了票卡模型
	CD_PAYLOAD_ID_PARTICIPANT_PARAMS                 = 73,      // 此CD 目标包含参与方特有的参数。
	CD_PAYLOAD_ID_NAMES                              = 75,      // 此CD 实体包含用于显示目的的各种列举类型的名称。
	CD_PAYLOAD_ID_MIFARE_PARAMS                      = 83,      // 由票卡模型使用，用于帮助管理票卡的处理。
	CD_PAYLOAD_ID_ATTRIBUTE_DEFINITIONS              = 84,      // 定义CdAttributeId_t的定义。
	CD_PAYLOAD_ID_HIGH_SECURITY_BLACKLIST            = 85,      // 用于通知车站管理人员在北京铁路网系统中存在特殊卡。
	CD_PAYLOAD_ID_LOCATIONS                          = 86,      // 标识位置和可能与之有关的属性。
	CD_PAYLOAD_ID_DEVICE_PARAMS                      = 87,      // 定义适用于LCS 设备的参数
	CD_PAYLOAD_ID_MANIFEST                           = 88,      // 在LCS CD XML 文件中，声明包含关于其后的有效数据的必要信息。
	CD_PAYLOAD_ID_BUSINESS_RULES                     = 89,      // 此有效数据用于帮助处理各种AFC系统要求。
	CD_PAYLOAD_ID_TIME_CODES                         = 90,      // 包含把标识与一天的特定时段（例如高峰期）相关联的时间代码。
	CD_PAYLOAD_ID_PASSENGER_TYPE_PARAMS              = 91,      // 添加旅客类型所需的附加信息。
	CD_PAYLOAD_ID_CARD_BATCH_WITHDRAW_LIST           = 92       // 根据CardBatchNumber_t和票卡的基本日期从流通中删除票卡。
}CdPayloadId_t;

//确定设备状态。
typedef enum _deviceStatus_t{  
	DEVICE_STATUS_IN_SERVICE            = 1,     // 设备通过SAM 安装测试，准备工作的状态。
	DEVICE_STATUS_OUT_OF_SERVICE        = 2,     // 此状态显示设备不工作，多数情况下这发生在下班后设备关闭时。
	DEVICE_STATUS_UNSPECIFIED           = 255    // 未设定值。
}DeviceStatus_t;

//决定各种付费方法。
typedef enum _paymentMethod_t{  
	PAYMENT_METHOD_CASH							= 1,		// 现金
	PAYMENT_METHOD_ACC_CARD						= 2,		// ACC储值卡
	PAYMENT_METHOD_YKT_CARD						= 3,		// YKT储值卡
	PAYMENT_METHOD_CRIDIT_CARD					= 4,		// 信用卡支付
	PAYMENT_METHOD_WITH_NET						= 5,		// 网络支付
	PAYMENT_METHOD_WITH_ZHIFUBAO				= 6,		// 扫码支付（20171208,支付宝和微信合一）
	PAYMENT_METHOD_WITH_WEIXIN					= 7,		// 微信支付(已无效)
	PAYMENT_METHOD_BY_CARD						= 252,		// 储值卡（无效）
	PAYMENT_METHOD_BANK_CARD					= 253,		// 银行卡（无效）
	PAYMENT_METHOD_NONE                         = 254,      // 无设定值
	PAYMENT_METHOD_UNSPECIFIED					= 255		// 无设定值
}PaymentMethod_t;

//票卡被更换的原因。
typedef enum _replacementReason_t{  
	REPLACEMENT_REASON_CARD_FAULTY                          = 1,    // 故障票卡
	REPLACEMENT_REASON_NEW_PERSONALISED_CARD_AVAILABLE      = 3,    // 有新的、个人化的票卡。
	REPLACEMENT_REASON_CARD_EXPIRED                         = 4,    // 票卡已经过期。
	REPLACEMENT_REASON_CARD_STOLEN                          = 5,    // 由于卡被盗窃，换卡
	REPLACEMENT_REASON_UNSPECIFIED                          = 255   // 无设定值。
}ReplacementReason_t;

//交易被撤销的原因。
typedef enum _reversalReasonCode_t{  
	REVERSAL_REASON_CODE_INVALID             = 1,       // 采取的行为无效。
	REVERSAL_REASON_CODE_UNSPECIFIED         = 255      // 无设定值。
}ReversalReasonCode_t;



//指定详细的排序和搜索的位图标志。
typedef enum _sortingFlags_t{  
	SORTING_FLAGS_UNSORTED_EXACT_MATCH           = 0x00,     // 显示数据需要进行线形搜索，即没有采用整理命令。
	SORTING_FLAGS_ASCENDING_EXACT_MATCH          = 0x01,     // 显示数据能进行二进制搜索。
	SORTING_FLAGS_ASCENDING_BEST_FIT_HIGH        = 0x02,     // 查找最适合的值大于提供的搜索键值.
	SORTING_FLAGS_ASCENDING_BEST_FIT_LOW         = 0x04      // 查找最合适的值小于提供的搜索键值
}SortingFlags_t;

//确定退回卡的可能原因。
typedef enum _surrenderReason_t{  
	SURRENDER_REASON_CARDHOLDER_REQUEST                 = 1,    // 来自持卡人的请求。
	SURRENDER_REASON_FAULTY_CARD                        = 2,    // 票卡有故障。
	SURRENDER_REASON_EXPIRED_CARD                       = 3,    // 票卡已过期。
	SURRENDER_REASON_PREVIOUSLY_BLOCKED                 = 4,    // 票卡被锁定。
	SURRENDER_REASON_CARD_PRINTING_PROBLEM              = 5,    // 票卡不能印刷。
	SURRENDER_REASON_AUTOMATICALLY_EXPIRED_CARD         = 6,    // 票卡自动过期。
	SURRENDER_REASON_AUTOMATICALLY_EXPIRED_PRODUCT      = 7,    // 票卡上的某个产品自动过期。
	SURRENDER_REASON_TICKET_EXCHANGE                    = 8,    // 发生了车票交换。
	SURRENDER_REASON_UNSPECIFIED                        = 255   // 无设定值。
}SurrenderReason_t;

//列举确认当前票存的状态，通常用于对一批车票的报告。
typedef enum _ticketStockStatus_t{  
	TICKET_STOCK_STATUS_OK                      = 1,    // 正常状态。
	TICKET_STOCK_STATUS_CARD_BATCH_WITHDRAWAL   = 2,    // 表示批量回收。
	TICKET_STOCK_STATUS_DAMAGED                 = 3,    // 表示坏票。
	TICKET_STOCK_STATUS_MAX_LIFECYCLE_REACHED   = 4,    // 表示该车票达到了其最大寿命次数。
	TICKET_STOCK_STATUS_FAULTY                  = 5,    // 表示该车票有故障。
	TICKET_STOCK_STATUS_OTHER                   = 254,  // 其它状态原因。。
	TICKET_STOCK_STATUS_UNSPECIFIED             = 255   // 说明未提供车票状态的原因。
} TicketStockStatus_t;

//标识交易的状态。
typedef enum _transactionStat_t{  
	TRANSACTION_STAT_CARD_UPDATE_NOT_CONFIRMED          = 0x01 ,    // 当交易未被确认时，这个位将设为“1”；当交易已被确认时，这个位将设为“0”。
	// 对于所有非票卡交易（即与票卡无关的交易），这个位将设为“0”
	// 未确认的交易是指：设备（指与票卡一起参与交易的设备）无法确信是否所有交易数据都被写入票卡中。
	// 如果在票卡完成交易数据写入后，但在它向设备反馈之前，票卡就被从RF 区域移走,这种情况就发生了。
	TRANSACTION_STAT_TEST_CARD                          = 0x02,     // 当使用测试卡时设置为“1”；当未使用测试卡时设置为“0”。如果没有使用票卡，也设置为“0”。
}TransactionStat_t;


// 流水号重置原因
typedef enum _udsn_reset_reason
{
	UNDIFINED     = 0x00, // 未定义
	MAXVALUERESET = 0x01, // 到达最大值后自动重置； 
	MAINTANRESET  = 0x02  // 维修重置
}UDSN_RESET_REASON;

// 流水号类型
typedef enum _udsn_type{
	ACC_UDSN = 0x01, // ACC交易流水号；
	ECT_UDSN = 0x02, // 一卡通交易流水号；
	AFC_UDSN = 0x03, // AFC数据流水号；
	UNDIFINE = 0x00  // 未定义
}UDSN_TYPE;

// 流水号重置
typedef struct _udsn_reset{
	UDSN_TYPE udsn_type;  // 1 BIN 设备流水号类型
	DWORD beforeResetNo;  // 4 BIN 重置前设备流水号
	DWORD afterResetNo;   // 4 BIN 重置后设备流水号
}UDSN_RESET;



//定义计数器类型
typedef enum _counter_type{
	COUNTER_TYPE_EVENT = 0x00,
	COUNTER_TYPE_TRANS_ACC = 0x01,
	COUNTER_TYPE_TRANS_ECT = 0x02,
	COUNTER_TYPE_TRANS_AFC = 0x03
} COUNTER_TYPE;

//定义卖票方式（根据TPU接口文档）
typedef enum _issue_sale_mode
{
	BY_UNDEFINE = 0x00,				// 未定义
	BY_PRICE = 0x01,				// 表示按照票价来售票
	BY_BEGIN_END_STATION = 0x02,	// 表示按照起止站来售票
} ISSUE_SALE_MODE;

typedef enum _price_mode
{
	BY_CONSUME_PRICE = 0x01,		// 消费票价
	BY_SALE_PRICE = 0x02,			// 销售票价
} PRICE_MODE;

// 业务状态
//typedef enum _service_state {
//	INIT        = 0x00,         // 初始中
//	OUT_OF      = 0x01,         // 中止中
//	READY       = 0x02,         // 等待中
//	RECEPT      = 0x03,         // 操作中
//	ISSUEING    = 0x05,         // 设备运用中
//	MAINTENANCE = 0x07,         // 维护中
//	ABNORMAL    = 0x08,         // 异常中
//	REST        = 0x09,         // 休息中
//	FINISH      = 0x0A,         // 结束
//} SERVICE_STATE;

// 线路编码，采用单字节编码。
//typedef U8_t  LineID_t;
// 车站编码，采用两字节编码，高字节为线路代码，低字节为线路内车站索引代码。
//typedef U16_t StationID_t;

////票种基本类型（大类分类）
//enum TicketFamily_t{
//	TicketFamily_SJT      = 1,/*单程票  */
//	TicketFamily_SV       = 2,/*地铁发行计次票  */
//	TicketFamily_YktCsc   = 3 /*一卡通储值票 */
//};

typedef struct _NW_TRANS_ID_GRP
{
	BYTE biGrpNum;        // 组号
	BYTE biEquipNum;      // 组内编号

	_NW_TRANS_ID_GRP() {
		biGrpNum   = 0;
		biEquipNum = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		*lpByte=biGrpNum;
		lpByte++;
		*lpByte = biEquipNum;
		lpByte++;
	}
	void Deserialize(LPBYTE lpByte)
	{
		biGrpNum = *lpByte;
		lpByte++;
		biEquipNum = *lpByte;
		lpByte++;
	}
}NW_TRANS_ID_GRP;

// 设备ID
typedef struct _device_id{
	BYTE bAfcDeviceType;           // 1 BCD 设备类型
	BYTE bLineId;        // 1 BCD 线路ID
	BYTE bStationId;            // 1 BCD 车站ID
	BYTE bDeviceSeqNoInStation; // 1 BIN 车站内设备序号
	_device_id(){
		bLineId = 0;
		bStationId = 0;
		bAfcDeviceType = 0;
		bDeviceSeqNoInStation = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		*lpByte = bAfcDeviceType;
		lpByte++;
		*lpByte = bLineId;
		lpByte++;
		*lpByte = bStationId;
		lpByte++;
		int2BCD(bDeviceSeqNoInStation, (char*)lpByte, 1);
		lpByte++;
	}
	void Deserialize(LPBYTE lpByte)
	{
		bAfcDeviceType = *lpByte;
		lpByte++;

		bLineId = *lpByte;
		lpByte++;
		bStationId = *lpByte;
		lpByte++;
		bDeviceSeqNoInStation = BCD2int((char*)lpByte, 1);
		lpByte++;
	}
	CString ToString()
	{
		CString s;
		s.Format(_T("%.2X%.2X%.2X%.2d"),bAfcDeviceType,bLineId,bStationId,bDeviceSeqNoInStation);
		return s;
	}
}DEVICE_ID;

typedef struct _accdeviceid_t
{
	BYTE lineCode;				//BCD线路代码
	WORD stationCode;			//2BCD站点编码
	BYTE deviceType;			//1BCD ACC设备类型
	WORD wSerialNo;					//2BCD设备序号
	CString ToString()
	{
		CString s;
		s.Format(_T("%.2X%.2X%.2X%.2X%.4d"),lineCode,HIBYTE(stationCode),LOBYTE(stationCode),deviceType,wSerialNo);
		return s;
	}
	void Serialize(LPBYTE lpMsg)
	{
		*lpMsg = lineCode;
		lpMsg+=1;
		*lpMsg = HIBYTE(stationCode);
		lpMsg++;
		*lpMsg = LOBYTE(stationCode);
		lpMsg++;
		*lpMsg = deviceType;
		lpMsg+=1;
		int2BCD(wSerialNo,(char*)lpMsg,2);
		lpMsg+=2;
	}
}AccDeviceID_t;
//票种明细类型,即车票类型 
typedef U8_t  TicketType_t;

//票种明细类型,即车票类型 
typedef enum _ProductType
{
	SJT				= 0x62,		// 单程票（0100-单程票&预赋值单程票&出站票，0103-纪念单程票，0104-付费出站票）	
	SVT				= 0x02,		// 储值票（0200-普通储值票，0202-老人储值票，0203-纪念储值票，0204-记名储值票）
	MST				= 0x03,		// 计次票（0300-普通乘次票，0305-限次乘次票）
	PST				= 0x04,		// 定期票（0400-一日票，0401-三日票，0402-七日票）
	NMT				= 0x05,		// 免费票（0500-免费票，0501-地铁免费票）
	EPT				= 0x07,		// 员工票（0700-EP1，0701-EP2，0703-EP34）
	TET				= 0x0C,		// 团体票（0C00-团体票）
	UNKNOWPRODUCT	= 0xFF,	// 未定义
}ProductType;
// 车票种类编码（BOM）
typedef	enum _ticket_kind {
	TICKET_SINGLE_RIDE = 98,				//单程票
	TICKET_PRE_MAKE = 97,					//预制票
	TICKET_ROUND = 96,							//往返票
	TICKET_WELFARE = 95,							//福利票
	TICKET_BAGGAGE = 94,							//行李票
	TICKET_EXIT_ONLY = 93,					//出站票

	TICKET_ONE_DAY = 5,						//一日票

	TICKET_STORE_VALUE = 51,			//普通储值票
	TICKET_STORE_VALUE_NAMED = 52,//记名普通储值票
	TICKET_STUDENT = 53,						//学生票
	TICKET_OLD_STORE_VALUE = 4,	//老年储值票
	TICKET_YKT_STORE_VALUE = 1,		// 一卡通储值票:
	TICKET_OLD_FREE =7,							//老年免费票

	TICKET_COMMEMORATIC_STORE_VALUE = 37,	//定值纪念票

	TICKET_STORE_MULTIRIDE = 31,			//普通计次票（CPU）
	TICKET_COMMEMORATIC_MULTIRIDE = 40,//纪念计次票

	TICKET_FIX_PERIOD = 34,				//普通定期票
	TICKET_COMMEMORATIC_PERIOD = 15,	//定期纪念票（UL）


	TICKET_EMPLOYEE_3 = 8,				//员工票EP3
	TICKET_EMPLOYEE_2 = 9,				//员工票EP2
	TICKET_EMPLOYEE_1 = 10,				//员工票EP1
	TICKET_COMMEMORATIC_PURSE = 65,				//纪念储值卡
	TICKET_STATION_WORK = 89,		//车站工作票

	TICKET_FOREIGN_PERSON_1 = 81,			//外服票FP1
	TICKET_FOREIGN_PERSON_2 = 82,			//外服票FP2
	TICKET_FOREIGN_PERSON_3 = 83,			//外服票FP3

	TICKET_PAPER			= 254,			//地铁纸票

} TICKET_KIND;
//票卡逻辑号
typedef U64_t TicketLogicalID_t;
// 线路编码，采用单字节编码。
typedef U8_t  LineID_t;
// 车站编码，采用两字节编码，高字节为线路代码，低字节为线路内车站索引代码。
typedef U16_t StationID_t;

// 卡物理类型
typedef	enum _card_medium_type
{
	RESERVE		= 0xFF,									// 预留
	ULTRALIGHT	= 0x00,									// Ultralight
	MIFARE1K	= 0x01,									// Mifare1K
	MIFARE4K    = 0x02,									// Mifare 4K
	CPU_TICKET	= 0x04,									// CPU票卡

	CARD_MEDIUM_TYPE_ULTRALIGHT		= CARD_TYPE_SY_EPT_UL/*ULTRALIGHT*/, // Mifare Ultralight
	CARD_MEDIUM_TYPE_MIFARE1K		= MIFARE1K, // Mifare One 1K卡
	CARD_MEDIUM_TYPE_MIFARE4K		= MIFARE4K, // Mifare One 4K卡
	CARD_MEDIUM_TYPE_CPU			= CARD_TYPE_SY_EPT_CPU/*CPU_TICKET*/ // CPU卡
} CARD_MEDIUM_TYPE;
//旅客分类级别。  
typedef enum _classificationLevel_t{  
	CLASSIFICATION_LEVEL_NORMOL                     = 0,        // 普通
	CLASSIFICATION_LEVEL_VIP                        = 1,        // VIP
	CLASSIFICATION_LEVEL_UNSPECIFIED                = 2         // 未设定值。
}ClassificationLevel_t;

//系统中所有相关产品的种类
typedef enum _productCategory_t{
	//UNDEFINE = 0x00,		    //未定义
	//PURSE = 0x01,				//钱包
	//PERIOD = 0x02,				//定期
	//MULTIRIDE = 0x03,			//计次	

	UNDEFINE = 0x00,		    //未定义
	PURSE = 0x01,				//储值卡钱包
	MULTIRIDE = 0x02,			//乘次钱包
	PERIOD = 0x03,				//定期钱包
	PURSE_FIRST = 0x04,			//储值钱包优先（储值钱包和乘次钱包双产品）
	MULTIRIDE_FIRST = 0x05,		//乘次钱包优先（储值钱包和乘次钱包双产品）	
}ProductCategory_t;

// ACC票卡测试模式 
typedef enum _accCard_test_status
{
	FORMAL_CARD = 0x00,					//0:正常使用
	TEST_CARD = 0x01,				    //1:测试或培训
} ACC_CARD_TEST_STATUS;

typedef enum _acc_Card_active_status
{
	UNACTIVED = 0x00,					//0:首次车费支付时激活 
	ACTIVED   = 0x01,					//1:已激活
}ACC_CARD_ACTIVE_STATUS;

//旅程状态
typedef	enum _acc_card_journey_status
{
	//TICKET_UNUSED				    = 0x00,				   //  产品未使用	
	//UNUSED=TICKET_UNUSED				    ,
	//INSTATION						= 0x01,				   //  正常进站																 
	//OUTSTATION						= 0x02,				   //  正常出站	
	//BEFOREOUT_IN					= 0x03,				   //  上次列车故障模式出站
	//STATUS_ADJUST_TIMEOUT           = 0x04,                //  上次补票标志

	TICKET_INIT						= 2,				   //  初始化	
	TICKET_SJT_ISSUE				= 3,				   //  单程票发售
	TICKET_WRITE_OFF				= 4,				   //  注销																 
	TICKET_REENCODE					= 5,				   //  重编码
	YKT_DISABLED_UPDATE_FOR_IN		= 10,				   //  一卡通残盲卡进站更新				
	YKT_DISABLED_UPDATE_FOR_OUT		= 11,				   //  一卡通残盲卡出站更新
	TICKET_UPDATE_FOR_IN			= 12,				   //  更新（为了进闸）
	TICKET_UPDATE_FOR_OUT           = 13,				   //  更新（为了出闸）
	TICKET_UPDATE_FOR_OTHER			= 14,				   //  其它更新
	TICKET_IN						= 15,				   //  进闸				
	TICKET_OUT						= 16,				   //  出闸
	YKT_DISABLED_IN					= 17,				   //  一卡通残盲卡进闸				
	YKT_DISABLED_OUT				= 18,				   //  一卡通残盲卡出闸
	REFOUND_TIKCET		            = 21,                  //  退卡
	TICKET_ES_ASSIGNMENT            = 31,                  //  在E/S预赋值
	TICKET_ASSIGNMENT_ZERO			= 32,                  //  预赋值清零
	TICKET_ET_FOR_EXIST				= 33,                  //  出站票（为了出闸）
	TICKET_BEFOREOUT_IN				= 34,	               //  列车故障时出闸

} ACC_CARD_JOURNEY_STATUS;

// 个性化栏位标志
typedef enum _personal_area_flag{
	PERSONAL_AREA_UNKNOWN			= 0,	// 未知内容
	PERSONAL_AREA_PASSAGER_TYPE		= 1,	// 乘客类型
	PERSONAL_AREA_PASSAGER_NAME		= 2,	// 姓名
	PERSONAL_AREA_PASSAGER_GENDER	= 3,	// 性别
	PERSONAL_AREA_PASSAGER_BIRTHDAY	= 4,	// 生日
	PERSONAL_AREA_CERTIFICATE_TYPE	= 5,	// 证件类型
	PERSONAL_AREA_CERTIFICATE_ID	= 6,	// 证件编号
	PERSONAL_AREA_PASSAGER_PHOTO	= 7,	// 员工照片
	PERSONAL_AREA_FOREGOUND			= 8,	// 正面背景
	PERSONAL_AREA_BACKGOUND			= 9,	// 反面背景
} PERSONAL_AREA_FLAG;

// 个性化栏位信息
typedef struct _personal_area{
	PERSONAL_AREA_FLAG flag;		// 栏位标志
	CString txtAreaContent;			// 栏位内容
	CString txtFont;				// 文本字体
	int nSize;						// 文本大小
	COLORREF color;					// 文本颜色
	int x;							// 横坐标
	int y;							// 纵坐标

	_personal_area()
	{
		flag = PERSONAL_AREA_UNKNOWN;
		txtAreaContent = _T("");
		txtFont = _T("");
		nSize = 0;
		color = 0;
		x = 0;
		y = 0;
	}
} PERSONAL_AREA;

// 个性化类型
typedef enum _personalize_type{
	TYPE_PERSONAL = 0,			// 个性化类型
	TYPE_PERSONAL_UPDATE = 1,	// 个性化更新类型
} PERSONALIZE_TYPE;

// 性别标识
typedef enum _sex_type{  
	SEX_MALE  = 1,			// 男
	SEX_FEMALE = 2,			// 女
	SEX_UNSPECIFIED = 255	// 无设定值。
} Sex_t;

// 证件类型
typedef	enum _certificate_type{
	CERTIFICATE_ID_CARD_MAINLAND = 1,	// 身份证（大陆）
	CERTIFICATE_ID_CARD_MACAO = 2,		// 身份证（澳门）
	CERTIFICATE_ID_CARD_HONGKONG = 3,	// 身份证（香港）
	CERTIFICATE_ID_CARD = 0,           // 0—身份证 
	CERTIFICATE_MILITARY_OFFICER = 1,   // 1—军官证 
	CERTIFICATE_POST_CARD = 2,          // 2—护照 
	CERTIFICATE_STUDENT_CARD = 3,       // 3—学生证
	CERTIFICATE_UNSPECIFIED = 255,      // 未定义
} CERTIFICATE_TYPE;

typedef enum _payed_method{
	BY_CASH  = 0x01,                   // 现金支付补票金额
	BY_CARD  = 0x03                    // 卡内扣除补票金额             
}_PAYED_METHOD;

//退款理由，同时适用于被退或归还的票卡。
typedef enum _refundReason_t{  
	REFUND_REASON_TICKET_UNUSED_FULL_REFUND_LESS_HANDLING_CHARGE                =1,     // 对未使用的车票减去手续费后全额退款
	REFUND_REASON_TICKET_USED_COMPLEMENTARY_FULL_REFUND                         =2,     // 对使用过的车票不收取任何费用全额退款
	REFUND_REASON_TICKET_PARTLY_USED_PROPORTIONAL_REFUND_LESS_HANDLING_CHARGE   =3,     // 对使用过的车票减去手续费后全额退款
	REFUND_REASON_UNSPECIFIED                                                   =255    // 未指定。
}RefundReason_t;
typedef enum _adjust_type
{
	ADJUST_KIND = 0x01,						//补票
	UPDATE_KIND = 0x02,						//更新
}ADJUST_TYPE;


// 交易类型
typedef enum _transaction_type{
	UNDEFINED           = 0,		// 没有
	CARD_KEYS_UPDATE	= 1,		// 卡密钥更新	
	ISSUE				= 2,		// 发行
	DELETE1		        = 3,		// 删除
	ADD_VALUE			= 4,		// 加值
	USE					= 5,		// 使用
	REFUND				= 6,		// 退款
	REBATE				= 7,		// 折扣
	SUBCHARGE			= 8,		// 补费
	BLOCK				= 9,		// 锁定
	UNBLOCK				= 10,		// 解锁
	AUTOLOAD_ENABLE		= 11,		// 自动加值功能生效
	AUTOLOAD_DISABLE	= 12,		// 自动加值功能失效
	AUTOLOAD_UPDATE		= 13,		// 自动加值更新
	REVERSE_ADD_VALUE	= 14,		// 加值操作备份
	PERSONALISE			= 15,		// 个人化
	USE_ON_ENTRY		= 16,		// 进站使用
	USE_ON_EXIT			= 17,		// 出站使用
	CONSUME				= 31,		// 消费
}TRANSACTION_TYPE;
// RW回调命令编号
typedef enum RW_CMD_ID{
	RW_COMMAND_ID_READ_CARD = 0x1000,               // 读卡命令
};

// 语言ID名称映射记录体
typedef struct _language_map{
	WORD languageID;
	CString languageName;
	_language_map()
	{
		languageID = 0;
		languageName = "";
	}
} LANGUAGE_MAP;

// 设备ID
typedef struct _TPU_device_id{
	BYTE bAfcDeviceType;        // 1 BIN AFC设备类型
	WORD wAfcStationID;         // 2 BCD AFC车站ID 
	BYTE bDeviceSeqNoInStation; // 1 BIN 车站内设备序号
	_TPU_device_id(){
		bAfcDeviceType = 0;
		wAfcStationID = 0;
		bDeviceSeqNoInStation = 0;
	}
	void Serialize(LPBYTE lpByte)
	{
		
		*lpByte = HIBYTE(wAfcStationID);
		lpByte++;
		*lpByte = LOBYTE(wAfcStationID);
		lpByte++;
		*lpByte=bAfcDeviceType;
		lpByte++;
		*lpByte = bDeviceSeqNoInStation;
		lpByte++;
	}
	void Deserialize(LPBYTE lpByte)
	{
		
		wAfcStationID = MAKEWORD(*(lpByte+1),*lpByte);
		lpByte+=2;
		bAfcDeviceType = *lpByte;
		lpByte++;
		bDeviceSeqNoInStation = *lpByte;
		lpByte++;
	}
	CString ToString()
	{
		CString s;
		s.Format(_T("%.2X%.2X%.2d%.2X"),HIBYTE(wAfcStationID),LOBYTE(wAfcStationID),bAfcDeviceType,bDeviceSeqNoInStation);
		return s;
	}
}TPU_DEVICE_ID;
typedef enum _tagApplyType
{
	APPLY_TYPE_GOOD_CARD = 0,					// 好卡申请
	APPLY_TYPE_BAD_CARD = 1,					// 坏卡申请
	APPLY_TYPE_SUSPEND = 2,						// 挂失申请
}RefundApplyType;
typedef enum _tagRefundReasonType
{
	REFUND_REASON_PASSAGER = 0,					// 乘客
	REFUND_REASON_SUBWAY = 1,					// 地铁
}RefundReasonType;

typedef enum _tagRefundApplyReasonType
{
	APPLY_REASON_BAD_CARD = 0,					// 票卡损坏
	APPLY_REASON_LARGE_MONEY = 1,				// 大额卡
	APPLY_REASON_OTHER = 2,						// 其他
}RefundApplyReasonType;

//////////// 其他常量
//////////static const U16_t KEY_VERSION_UNSPECIFIED                                      = 0xFFFF;
//////////static const U16_t KEY_NUMBER_UNSPECIFIED                                       = 0xFFFF;
//////////static const U32_t UNSPECIFIED_CARD_PARTICIPANT                                 = 0xFFFFFFFF;
//////////static const U16_t LIFE_CYCLE_COUNTER_UNSPECIFIED                               = 0xFFFF;
//////////static const U32_t CARD_SERIAL_NUMBER_UNSPECIFIED                               = 0xFFFFFFFF;
//////////static const DateC20_t      UNSPECIFIED_DATEC20                                 = 0xFFFF;
//////////static const U32_t          UDSNUM_UNSPECIFIED                                  = 0xFFFFFFFF;
//////////// CD版本无效的判断值
//////////static const unsigned long UNAVALIBLE_VERSION     = 0xFFFFFFFF;

// 定义操作员ID及密码长度
static const int STAFFID_LEN   = 6;
static const int STAFFPWD_LEN   = 6;

// 站员权限
typedef enum _staff_authority
{
	NONE_AUTHORITY      = 0,            // 无权限
	COMMON_AUTHORITY    = 1,            // 普通业务权限
	MAINTAIN_AUTHORITY  = 2,            // 维护业务权限
	ALL_AUTHORITY       = 3,            // 全部业务权限（普通和普通）
} STAFF_AUTHORITY;

typedef enum _ForceLogoutReason
{
	FORCE_LOGOUT_SERVER_REQUEST = 0x01,	//上位发强制登出请求
	FORCE_LOGOUT_DEVICE_CONTROL = 0x02,//设备运行控制强制登出
	FORCE_LOGOUT_EMERGENCY_MODE = 0x03, //紧急模式强制登出
	FORCE_LOGOUT_NEWPARAMETER = 0x04,		//新参数及程序下载
	FORCE_LOGOUT_DEVICE_CONTROL_SHUTDOWN = 0x05,		//设备运行控制关机命令
} FORCELOGOUTREASON;

// 票卡状态
typedef enum _ticket_status{
	TICTKET_STATUS_UNINITIALIZED	= 0x00,	// 0:未初始化（当为0时，号段表有效）
	TICTKET_STATUS_INITIALIZED		= 0x01,	// 1:已初始化
}TICTKET_STATUS;

typedef enum _authorization_param{
	taskSetting = 0x3D010101,
	taskManagement = 0x3D010201,
	templateSetting = 0x3D010301,
	templateManagement = 0x3D010401,
	otherService = 0x3D010601,
	managementServive = 0x3D020101,
} AUTHORIZATION_PARAM;

// 设备模式定义
typedef enum _device_mode{
	TEST_MODE		= 0x00,		// 正常模式
	NORMAL_MODE     = 0x01		// 测试模式
}DEVICE_MODE;

// 挂失解挂
typedef enum _suspend_and_resmue_type{
	BUSINESS_SUSPEND_TYPE = 1,					// 挂失
	BUSINESS_RESMUE_TYPE = 2,					// 解挂
	BUSINESS_SUSPEND_RESMUE_UNSPECIFIED = 255,	// 未知类型
} SUSPEND_RESMUE_TYPE;

// 交易类型（ACC M1K）
typedef enum _acc_m1k_transaction_type{
	ACC_M1K_TRANS_TYPE_ISSUE_TICKET			= 1,  // 售卡
	ACC_M1K_TRANS_TYPE_ISSUE_EXIT			= 3,  // 发售出站票
	ACC_M1K_TRANS_TYPE_ISSUE_LOVE			= 4,  // 爱心票
	ACC_M1K_TRANS_TYPE_ISSUE_BAGGAGE		= 5,  // 发售行李票
	ACC_M1K_TRANS_TYPE_INITIALIZATION		= 8,  // 初始化
	ACC_M1K_TRANS_TYPE_PREEASSIGNMENT		= 9,  // 预赋值
	ACC_M1K_TRANS_TYPE_CASH_RECHARGE		= 11, // 现金充值
	ACC_M1K_TRANS_TYPE_ADJUST_TICKET		= 15, // 补票
	ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_START = 21, // 地铁消费起始
	ACC_M1K_TRANS_TYPE_SUBWAY_CONSUME_END	= 22, // 地铁消费结束
	ACC_M1K_TRANS_TYPE_SVT_BUY_TICKET		= 41, // 储值卡购票
	ACC_M1K_TRANS_TYPE_FOLLOW_ENTER			= 51, // 跟踪进站
	ACC_M1K_TRANS_TYPE_FOLLOW_EXIT			= 52, // 跟踪出站
	ACC_M1K_TRANS_TYPE_CANCELLATION			= 66, // 注销
	ACC_M1K_TRANS_TYPE_COUNTERACT			= 67, // 抵消
	ACC_M1K_TRANS_TYPE_INSTANT_REFUND_CARD	= 71, // 即时退卡
	ACC_M1K_TRANS_TYPE_LOCK					= 77, // 锁卡
	ACC_M1K_TRANS_TYPE_UNLOCK				= 78, // 解锁
	ACC_M1K_TRANS_TYPE_REVERSE				= 79, // 冲正
	ACC_M1K_TRANS_TYPE_OVERTIME				= 83, // 滞留超时处理
	ACC_M1K_TRANS_TYPE_UNEXIT				= 84, // 无出站处理
	ACC_M1K_TRANS_TYPE_UNENTER				= 89, // 地铁无入站处理
	ACC_M1K_TRANS_TYPE_EXTENSION_VALIDITY	= 90, // 有效期更改
	ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_FINE	= 91, // 行政罚款
	ACC_M1K_TRANS_TYPE_ADMINISTRATIVE_PAY	= 92, // 行政支付
	ACC_M1K_TRANS_TYPE_ACTIVATION			= 96, // 激活
	ACC_M1K_TRANS_TYPE_ISSUE_GROUP			= 97, // 售团体票
}ACC_M1K_TRANSACTION_TYPE;

// 个性化业务类型
typedef enum _personal_business_type{
	BUSINESS_PERSONAL_TYPE = 0,			// 个性化类型
	BUSINESS_PERSONAL_UPDATE_TYPE = 1,	// 个性化更新类型
	BUSINESS_UNSPECIFIED = 255,			// 未知类型
} PERSONAL_BUSINESS_TYPE;


// RFID模式定义
typedef	enum{
	RFID_MODE_READONLY		= 0x00,				// 只读模式
	RFID_MODE_WRITABLE	= 0x01				    // 可写模式
} RFID_MODE;

// 交易类型（ACC CPU）
typedef enum _acc_cpu_transaction_type{
	ACC_CPU_TRANS_TYPE_RECHARGE				= 2, // 充值
	ACC_CPU_TRANS_TYPE_CONSUME				= 6, // 消费
	ACC_CPU_TRANS_TYPE_COMPLEX_CONSUME		= 9, // 复合消费
}ACC_CPU_TRANSACTION_TYPE;

// 票箱状态
enum {
	MAGAZINEBOX_COUNT = 12,			// 票箱状态数	           
	MAGASENSOR_COUNT = 12,			// 传感器状态数
	MAGAALERM_COUNT = 12,			// 报警状态数
	MAGAVALID_COUNT = 5,			// 有效状态数
};

// 乘客类型
typedef enum _passenger_type{
	PASSENGER_UNDEFINED			= 0x00,		// 预留
	PASSENGER_ADULT				= 0x01,		// 成人
	PASSENGER_CHILD				= 0x02,		// 儿童
	PASSENGER_OLD_PEOPLE		= 0x03,		// 老年人
	PASSENGER_STUDENT			= 0x04,		// 学生
	PASSENGER_SOLDIER			= 0x05,		// 军人
	PASSENGER_DISABLED_PEOPLE	= 0x06,		// 残障人
}PASSENGER_TYPE;

//////////////////////////////////////////////////////////////////////////


////////////////////////读写器相关定义////////////////////////////
//////////// SAM槽编号
//////////typedef	enum _slot_id{
//////////	SLOT1 =	0x00,									// 一卡通PSAM卡号
//////////	SLOT2 =	0x01,									// 一卡通ISAM卡号
//////////	SLOT3 =	0x02,									// 地铁PSAM卡号
//////////	SLOT4 =	0x03									// 地铁ISAM卡号
//////////} SLOT_ID;

//////////// SAM种类定义
//////////typedef enum _sam_type {
//////////	RW_ACC_ISAM = 0x01,
//////////	RW_ACC_PSAM = 0x02,
//////////	TH_ACC_ISAM = 0x03,
//////////	TH_ACC_PSAM = 0x04,
//////////	RW_ECT_ISAM = 0x05,
//////////	RW_ECT_PSAM = 0x06,
//////////	TH_ECT_ISAM = 0x07,
//////////	TH_ECT_PSAM = 0x08
//////////}SAM_TYPE;

// 读写器类型
typedef enum _rw_type{
	BOM_DESKTOP_RW					= 0x01,    // BOM桌面读写器
	BOM_CARD_RW						= 0x02,    // BOM票卡模块用读写器
	BOM_TOKEN_RW						= 0x03,    // BOM Token模块用读写器
	TVM_RECHARGE_RW				= 0x04,    // TVM充值用读写器
	TVM_CARD_RW						= 0x05,    // TVM票卡模块用读写器
	TVM_TOKEN_RW						= 0x06,    // TVM Token模块用读写器
	AGM_CARD_RECYCLE_RW		= 0x07,    // AGM 票卡回收读写器
	AGM_TOKEN_RECYCLE_RW		= 0x08,    // AGM Token回收读写器
	AGM_ENTRY_RW						= 0x09,    // AGM进站刷卡读写器
	AGM_EXIT_RW							= 0x10,    // AGM出站刷卡读写器
	ES_TOKEN_RW1						= 0x11,	// Token ES读写器1
	ES_TOKEN_RW2						= 0x12,	// Token ES读写器2
	ES_TOKEN_RW3						= 0x13,	// Token ES读写器3
	ES_CARD_RW							= 0x14,	// ES Card读写器
	PM_RW									= 0x15,	// PM读写器
	TCM_RW									= 0x16 	// TCM读写器
}RW_TYPE;

typedef enum _rw_update_flag{
	SUCCEEDED    = 0x00, // 成功
	FAILURE      = 0x01  // 失败
}RW_UPDATE_FLAG;


// 卡物理类型
typedef	enum _es_card_medium_type
{
	TICKET_MEDIUM_TYPE_RESERVE	= 0xFF,									// 预留
	TICKET_MEDIUM_TYPE_CARD		= 0x01,									// Card
	TICKET_MEDIUM_TYPE_TOKEN    = 0x02,									// Token
} TICKET_MEDIUM_TYPE;

typedef enum _sortType_t{
	ID_SORT_TYPE_SATYPE					               = 0 , //密钥版本               
	ID_SORT_TYPE_CARD_VER				               = 1 , //卡批次号               
	ID_SORT_TYPE_CARD_NUM				               = 2 , //逻辑卡号               
	ID_SORT_TYPE_MEDIA_TYPE							   = 3 , //介质类型               
	ID_SORT_TYPE_PRO_TYPE				               = 4 , //产品类型               
	ID_SORT_TYPE_PRO_CARATARY						   = 5 , //产品类别               
	ID_SORT_TYPE_TEST_MODE 							   = 6 , //测试模式               
	ID_SORT_TYPE_INI_MONTH					           = 7 , //初始化月               
	//ID_SORT_TYPE_CARD_COMPANY						   = 8 , //卡片封装厂             
	ID_SORT_TYPE_PRE_ISSUE_AMOUNT                      = 8 , //预赋金额               
	ID_SORT_TYPE_PRE_ISSUE                             = 9, //预赋次数               
	ID_SORT_TYPE_CARD_STATUE                           = 10, //卡状态                 
}SortType_t;

typedef	enum _ulCard_trac_type
{
	MONTHPAK_STROE    = 0x01,                                     // "月票钱包 圈存";
	PUBPAK_STROE      = 0x02,                                     // "公共钱包 圈存";
	MONTHPAK_CONSUMER = 0x05,                                     // "月票钱包 消费";
	PUBPAK_CONSUMER   = 0x06,                                     // "公共钱包 消费";
	COM_CONSUMER      = 0x09                                      // "复合消费";

} ULCARD_TRAC_TYPE;

typedef	enum _cpuCard_trac_type
{
	LOACLPAK_CONSUMER = 0x01,                                     // "本地钱包消费";
	UPDATE_TRAC       = 0x62,                                     // "轨道交通更新交易";
	REFOUND_TRAC      = 0x8A,                                     // "退卡";
	LUCK_TRAC         = 0x99,                                     // "锁卡交易";
	ENTRE_TRAC        = 0x41,                                     // "轨道交通进站";
	OUT_TRAC          = 0x81,                                     // "轨道交通出站";
	CHARGE_TRAC       = 0x88,                                     // "充值";
} _CPUCARD_TRAC_TYPE;


// 运营模式定义
typedef enum _operationalMode_t{
	//NORMAL_SERVICE_MODE				= 0x8300,			// 正常服务模式
	//EMERGENCY_MODE					= 0x8301,			// 紧急放行模式
	//ENTRY_EXIT_STATION_FREE_MODE	= 0x8302,			// 进站/出站次序免检模式
	//RIDE_TIME_FREE_MODE				= 0x8308,			// 乘车时间免检模式
	//TICKET_DATE_FREE_MODE			= 0x8304,			// 车票日期免检模式
	//TRAIN_SERVICE_DISRUPTION_MODE	= 0x8310,			// 列车故障模式
	//FARE_FREE_MODE					= 0x8320,			// 车费免检模式
	////FREE_ENTRY_STATION_MODE     	= 0x8340,			// 进站免检模式

	// TVM
	NORMAL_SERVICE_MODE				= 0x8300,			// 正常服务模式
	TRAIN_SERVICE_DISRUPTION_MODE	= 0x8301,			// 列车故障模式
	ENTRY_EXIT_STATION_FREE_MODE	= 0x8302,			// 进站/出站次序免检模式（TVM不处理）
	TICKET_DATE_FREE_MODE			= 0x8304,			// 车票日期免检模式（TVM不处理）
	FARE_FREE_MODE					= 0x8308,			// 费用免检模式（TVM不处理）
	TIME_FREE_MODE					= 0x8310,			// 时间免检模式（TVM不处理）
	EMERGENCY_MODE					= 0x8320,			// 紧急放行模式
	//BOM
	DEVICE_MODE_NORMAL = 1,									// Normal正常
	DEVICE_MODE_CLOSED = 2,									// Closed关闭
	DEVICE_MODE_TRAIN_FAULT = 3,							// Train fault列车故障
	DEVICE_MODE_CHECK_FREE_ENTRY = 4,						// Check-free entry进站免检
	DEVICE_MODE_CHECK_FREE_EXIT = 5,						// Check-free exit出站免检
	DEVICE_MODE_EMERGENCY_EXIT = 6,							// Emergency exit紧急模式
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME = 7,					// Normal + Check-free time正常+时间免检
	DEVICE_MODE_NORMAL_CHECK_FREE_DATE = 8,					// Normal + Check-free date正常+日期免检
	DEVICE_MODE_NORMAL_CHECK_FREE_FARE = 9,					// Normal + Check-free fare正常+费用免检
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE = 10,			// Normal + Check-free time + date正常+时间免检
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME_FARE = 11,			// Normal + Check-free time + fare正常+时间免检+费用免检
	DEVICE_MODE_NORMAL_CHECK_FREE_DATE_FARE = 12,			// Normal + Check-free date + fare正常+日期免检+费用免检
	DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE_FARE = 13,		// Normal + Check-free time + date + fare正常+时间免检+日期免检
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME = 14,					// Check-free entry + time进站免检+时间免检
	DEVICE_MODE_CHECK_FREE_ENTRY_DATE = 15,					// Check-free entry + date进站免检+日期免检
	DEVICE_MODE_CHECK_FREE_ENTRY_FARE = 16,					// Check-free entry + fare进站免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE = 17,			// Check-free entry + time + date进站免检+时间免检+日期免检
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME_FARE = 18,			// Check-free entry + time + fare进站免检+时间免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_DATE_FARE = 19,			// Check-free entry + date + fare进站免检+日期免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE_FARE = 20,		// Check-free entry + time + date + fare进站免检+时间免检+日期免检+费用免检
	DEVICE_MODE_CHECK_FREE_EXIT_TIME = 21,					// Check-free exit + time出站免检+时间免检
	DEVICE_MODE_CHECK_FREE_EXIT_DATE = 22,					// Check-free exit + date出站免检+日期免检
	DEVICE_MODE_CHECK_FREE_EXIT_FARE = 23,					// Check-free exit + fare出站免检+费用免检
	DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE = 24,				// Check-free exit + time + date出站免检+时间免检+日期免检
	DEVICE_MODE_CHECK_FREE_EXIT_TIME_FARE = 25,				// Check-free exit + time + fare出站免检+时间免检+费用免检
	DEVICE_MODE_CHECK_FREE_EXIT_DATE_FARE = 26,				// Check-free exit + date + fare出站免检+日期免检+费用免检
	DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE_FARE = 27,		// Check-free exit + time + date + fare出站免检+时间免检+日期免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME = 28,			//Check-free entry + exit + time进站免检+时间免检+出站免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE = 29,			//Check-free entry + exit + date进站免检+出站免检+日期免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_FARE = 30,			//Check-free entry + exit + fare进站免检+出站免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE = 31,		//Check-free entry + exit + time + date进站免检+出站免检+时间免检+日期免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_FARE = 32,		//Check-free entry + exit + time + fare进站免检+出站免检+时间免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE_FARE = 33,		//Check-free entry + exit + date + fare进站免检+出站免检+日期免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE_FARE = 34,	//Check-free entry + exit + time + date + fare进站免检+出站免检+时间免检+日期免检+费用免检
	DEVICE_MODE_CHECK_FREE_ENTRY_EXIT = 35					//Check-free entry + exit进站免检+出站免检	
}OperationalMode_t;


// 站员信息
typedef struct _user_info
{
	CString sUserID;                    // 用户ID
	STAFF_AUTHORITY allAuthority;       // 用户全部权限
	STAFF_AUTHORITY curAuthority;       // 用户当前权限
	_DATE_TIME loginTime;               // 登录时间
	CString sPwd;
	_user_info() {                      // 构造函数
		Initialize();
	}

	void Initialize() {                 // 初期化
		sUserID = _T("");
		allAuthority = NONE_AUTHORITY;
		curAuthority = NONE_AUTHORITY;
		sPwd = _T("");
	}
} USER_INFO, *LPUSER_INFO;

// 票卡号码段信息
typedef struct _ticket_no_range
{
	DWORD nBeginNo;
	DWORD nEndNo;
	_ticket_no_range()
	{
		nBeginNo = 0;
		nEndNo = 0;
	}
} TICKET_NO_RANGE;

// 任务设置光标定位
typedef enum _task_control_focus 
{
	FOCUS_TASK_NAME_EDIT		= 1,
	FOCUS_PLAN_QUANTITY_EDIT	= 2,
	FOCUS_BEGIN_NO_EDIT			= 3,
	FOCUS_END_NO_EDIT			= 4,
	FOCUS_AMOUNT_EDIT			= 5,
	FOCUS_VALID_RIDES_EDIT		= 6
} TASK_CONTROL_FOCUS;

// 任务类型
typedef enum _task_type {
	TASK_TYPE_UNKNOW = 0,		// 未知任务
	TASK_TYPE_INITIALIZATION,	// 初始化任务
	TASK_TYPE_PRE_ISSUE,		// 预赋值任务
	TASK_TYPE_RE_ENCODE,		// 重编码任务
	TASK_TYPE_SORT,				// 分拣任务
	TASK_TYPE_PERSONALIZE,		// 个性化任务
	TASK_TYPE_CANCEL,           // 注销任务制定
} TASK_TYPE;

// 模板类型
typedef enum _template_type {
	TEMPLATE_TYPE_UNKNOW = 0,		// 未知类型
	TEMPLATE_TYPE_SORT = 10,		// 分拣模板
	TEMPLATE_TYPE_PERSONALIZE = 11,	// 个性化模板
} TEMPLATE_TYPE;

// 读写器操作类型
typedef enum _rw_operation_type {	
	OPERATION_TYPE_UNKNOW = 0,					// 0：未知操作类型
	OPERATION_TYPE_INITIALIZATION = 1,			// 1：初始化
	OPERATION_TYPE_PRE_ISSUE,					// 2：预赋值
	OPERATION_TYPE_REENCODE,					// 3：重编码
	OPERATION_TYPE_INITIALIZATION_REENCODE,		// 4：初始化+重编码
	OPERATION_TYPE_REENCODE_PRE_ISSUE,			// 5：重编码+预赋值
	OPERATION_TYPE_CANCEL,						// 6：注销（预留）
}RW_OPERATION_TYPE;

// ES2票箱类别
typedef enum _es_magazine_type{
	ES_MAGAZINE_NORMAL =0,	// 正常票箱
	ES_MAGAZINE_WASTE,		// 废票箱
}ES_MAGAZINE_TYPE;

// ES2票箱编号
typedef enum _es_magazine_number{
	ES_MAGAZINE_ALL = 0,			// 所有票箱
	ES_MAGAZINE_1 = 1,				// 票箱1
	ES_MAGAZINE_2 = 2,				// 票箱2
	ES_MAGAZINE_3 = 3,				// 票箱3
	ES_MAGAZINE_4 = 4,				// 票箱4
	ES_MAGAZINE_5 = 5,				// 票箱5
}ES_MAGAZINE_NO;

// ES传感器工位
typedef enum _es2_sensor_site{	
	ES2_SENSOR_SITE_SEND_CARD = 0,	// 工位0：发卡工位
	ES2_SENSOR_SITE_RW1,			// 工位1：读写器1工位
	ES2_SENSOR_SITE_RW2,			// 工位2：读写器2工位（暂不使用）
	ES2_SENSOR_SITE_MAGAZINE1,		// 工位3：票箱1工位
	ES2_SENSOR_SITE_MAGAZINE2,		// 工位4：票箱2工位
	ES2_SENSOR_SITE_MAGAZINE3,		// 工位5：票箱3工位
	ES2_SENSOR_SITE_MAGAZINE4,		// 工位6：票箱4工位
	ES2_SENSOR_SITE_MAGAZINE5,		// 工位7：票箱5工位
}ES2_SENSOR_SITE;

//////////// 个性化栏位标志
//////////typedef enum _personal_area_flag{
//////////	PERSONAL_AREA_UNKNOWN		= 0,	// 未知内容
//////////	PERSONAL_AREA_PERSON_ID		= 1,	// 员工编号
//////////	PERSONAL_AREA_PERSON_NAME	= 2,	// 员工姓名
//////////	PERSONAL_AREA_PERSON_BIRTHDAY= 3,	// 员工生日
//////////	PERSONAL_AREA_PAPER_TYPE	= 4,	// 证件类型
//////////	PERSONAL_AREA_PAPER_ID		= 5,	// 证件编号
//////////	PERSONAL_AREA_COMPANY_NAME	= 6,	// 公司名称
//////////	PERSONAL_AREA_FOREGOUND		= 7,	// 正面背景
//////////	PERSONAL_AREA_BACKGOUND		= 8,	// 反面背景
//////////	PERSONAL_AREA_PERSON_PHOTO	= 9		// 员工照片
//////////} PERSONAL_AREA_FLAG;

//////////// 个性化栏位信息
//////////typedef struct _personal_area{
//////////	PERSONAL_AREA_FLAG flag;		// 栏位标志
//////////	CString txtAreaContent;			// 栏位内容
//////////	CString txtFont;				// 文本字体
//////////	int nSize;						// 文本大小
//////////	COLORREF color;					// 文本颜色
//////////	int x;							// 横坐标
//////////	int y;							// 纵坐标
//////////
//////////	_personal_area()
//////////	{
//////////		flag = PERSONAL_AREA_UNKNOWN;
//////////		txtAreaContent = _T("");
//////////		txtFont = _T("");
//////////		nSize = 0;
//////////		color = 0;
//////////		x = 0;
//////////		y = 0;
//////////	}
//////////} PERSONAL_AREA;

// 员工证件类型
typedef enum _person_paper_type{
	PAPER_IDENTITY_CARD_MAINLAND = 1,		// 身份证(大陆)
	PAPER_IDENTITY_CARD_MACAU = 2,			// 身份证(澳门)
	PAPER_IDENTITY_CARD_HONGKONG = 3,		// 身份证(香港)
	PAPER_UNKNOW		= 4					// 其他
} PERSON_PAPER_TYPE;

// 任务状态
typedef  enum _task_status {	

	TASK_STATUS_UNKONW		= 0x00,	// 未知状态
	TASK_STATUS_UNEXCUTE	= 0x01,	// 未执行
	TASK_STATUS_UNCOMPLETE	= 0x02,	// 未完成
	TASK_STATUS_COMPLETED	= 0x03,	// 已完成
}TaskStatus;



// 任务结束类型
typedef  enum _task_end_type {		
	TASK_END_NORMAL			= 1,	// 任务正常结束
	TASK_END_UNNORMAL		= 2,	// 任务异常结束
}TASK_END_TYPE;

// 分拣模板中计算符号类别
typedef  enum _sort_compute_type {	
	COMPUTE_TYPE_EQUAL			= 1,	// 等于
	COMPUTE_TYPE_GREATER_THAN	= 2,	// 大于
	COMPUTE_TYPE_LESS_THAN		= 3,	// 小于
	COMPUTE_TYPE_NO_EQUAL		= 4,	// 不等于
	COMPUTE_TYPE_NOT_LESS_THAN	= 5,	// 大于等于
	COMPUTE_TYPE_NOT_MORE_THAN	= 6,	// 小于等于	
}SORT_COMPUTE_TYPE;

// ES模块
typedef enum _ES_module_number{		
	ES_MODULE_NUMBER_0 = 0,				// 全部模块
	ES_MODULE_NUMBER_1 = 1,				// 模块1
	ES_MODULE_NUMBER_2 = 2,				// 模块2
	ES_MODULE_NUMBER_3 = 3,				// 模块3
} ES_MODULE_NUMBER;

// 分拣信息结构体
typedef struct _sort_info{	
	SortType_t sortType;			// 分拣项名称
	SORT_COMPUTE_TYPE computeType;	// 分拣条件
	CString computeValus;			// 分拣条件选项值
	_sort_info()
	{
		sortType = ID_SORT_TYPE_CARD_VER;
		computeValus = _T("");
		computeType = COMPUTE_TYPE_EQUAL;			
	}
}SORT_INFO;

#define WM_USER_TASK_END (WM_USER+5000)					// 任务结束消息

// 语言对应内容结构体
typedef struct _language_content{
	WORD languageType;			// 语言种类代码
	CString txtContent;			// 对应内容	
	_language_content()
	{
		languageType = 0;
		txtContent = _T("");
	}
} LANGUAGE_CONTENT;



// 票卡状态
typedef enum _ticket_logic_status{		
	TICKET_STATUS_UNKNOW = 0,			// 未知状态
	TICKET_STATUS_NORMAL = 1,			// 正常
	TICKET_STATUS_ERROR = 2,			// 异常
}TICKET_LOGIC_STATUS;

#define  NORMAL_MAGAZINE_NO_MAX_COUNT	4	// 正常票箱数量
#define  MAGAZINE_MAX_COUNT				5	// 票箱总数量
#define  RE_TRY_COUNT					3	// 重试最大次数
#define  BOX_BUFFER_LEN					5	// 票箱信息数组长度

// 无底图打印文字信息
typedef struct _print_text_info {
	CString txtContent;		// 文本内容
	bool bBold;				// 是否粗体
	int nSize;				// 字体大小
	int x;					// 横坐标
	int y;					// 纵坐标
	_print_text_info()
	{
		txtContent = _T("");
		bBold = false;
		nSize = 0;
		x = 0;
		y = 0;
	}
} PRINT_TEXT_INFO;


// 票箱状态
typedef enum _magazine_operation_type{	
	MAGAZINE_OPERATION_UNKNOW = 0,			// 未知
	MAGAZINE_OPERATION_UNINSTALL = 1,		// 卸载
	MAGAZINE_OPERATION_INSTALL = 2,			// 安装
}MAGAZINE_OPERATION_TYPE;

// 设备操作类型
typedef enum _device_operation_type
{
	DEVICE_RUN = 0x01,				// 运行
	DEVICE_SUSPEND = 0x02,			// 暂停
	DEVICE_CONTINUE = 0x03,			// 继续
	DEVICE_END = 0x04,				// 结束
	DEVICE_CHANGEMagazine = 0x05,	// 更换票箱
	DEVICE_RESERVE = 0x06			// 预留
}DEVICE_OPERATION_TYPE;


// 电子标签编号
typedef enum _rfid_no{
	RFID = 0,
	RFID_1 = 1,
	RFID_2 = 2,
	RFID_3 = 3,
	RFID_4 = 4,
} RFID_NO;

typedef struct _rfid_param{	
	HANDLE	hCommHandle;	// 串口句柄	
	RFID_NO rfidId;			// 电子标签编号
	CString rfidName;		// 电子标签名字
	_rfid_param()
	{	
		hCommHandle = INVALID_HANDLE_VALUE;
		rfidId = RFID;
		rfidName = _T("RFID");
	}
}RFID_PARAM;



// RFID数据结构
typedef struct _SC_RFID_DATA
{
	BYTE byBoxID[4];					// 		0	票箱编号	4	1BIN+1BIN+2BIN	最后2字节按Intel序存储 ,静态区域数据
	//----------以下为动态区域数据----------------------------
	BYTE byOperatorID[3];				// 		0	操作员ID	3	BCD	
	BYTE byDeviceID[4];					// 		3	设备ID	1BIN+2BCD+1BIN	
	BYTE byBoxLocaStatusBeforeOperate;	// 		7	票箱位置状态	1	BIN	参见注1.
	BYTE byBoxLocaStatusAfterOperate;	// 		8	操作后票箱状态	1	BIN	参见注2
	BYTE byTicketIssuerID[4];			// 		9	票卡发行商ID	4	BIN	按Intel序存储
	BYTE byTicketPHType;				// 		13	票卡物理类型	1	BIN	
	short sTicketProductType;			// 		14	车票产品类型	2	BIN	按Intel序存储
	BYTE byAttributeOfPrepWrite;		// 		16	预赋值属性	1	BIN	参见注3
	BYTE bySeqOfDerivativeProduct;		// 		17	衍生产品序号	1	BIN	参见注4
	short sTicketNumber;				// 		18	票卡数量	2	BIN	票箱内的票卡数量，按Intel序存储
	BYTE byStationCode[2];				// 		20	车站编码	2	BCD	
	BYTE byPosition;					// 		22	安装位置	1	BIN	描述票箱在设备中的安装位置
	BYTE byBlockFlag[4];				// 		23	块操作标记	4	BIN	块之间滚动交替写入标志。按Intel序存储
	BYTE byOperateTime[7];				// 		27	最后操作时间	7	BCD	RFID最后一次写入时间
	BYTE byCrc[2];						// 		34	校验字段	2	BIN	对块A第0到34字节的CRC16校验值,按Intel序存储。校验初值是0xFFFF。
	BYTE byReserve[12];					// 		35	预留字段	12	BIN	

	_SC_RFID_DATA(){
		Initialize();
	}

	void Initialize(){
		memset(byBoxID,0xFF,sizeof(byBoxID));
		memset(byOperatorID,0,sizeof(byOperatorID));
		memset(byDeviceID,0,sizeof(byDeviceID));
		memset(&byBoxLocaStatusBeforeOperate,0,sizeof(byBoxLocaStatusBeforeOperate));
		memset(&byBoxLocaStatusAfterOperate,0,sizeof(byBoxLocaStatusAfterOperate));
		memset(byTicketIssuerID,0,sizeof(byTicketIssuerID));
		memset(&byTicketPHType,0,sizeof(byTicketPHType));
		memset(&sTicketProductType,0,sizeof(sTicketProductType));
		memset(&byAttributeOfPrepWrite,0,sizeof(byAttributeOfPrepWrite));
		memset(&bySeqOfDerivativeProduct,0,sizeof(bySeqOfDerivativeProduct));
		memset(&sTicketNumber,0,sizeof(sTicketNumber));
		memset(byStationCode,0,sizeof(byStationCode));
		memset(&byPosition,0,sizeof(byPosition));
		memset(byBlockFlag,0,sizeof(byBlockFlag));
		memset(byOperateTime,0,sizeof(byOperateTime));
		memset(byCrc,0,sizeof(byCrc));
		memset(byReserve,0,sizeof(byReserve));
	}
} SC_RFID_DATA, * LPSC_RFID_DATA;


//////////// 票卡打印机状态定义
//////////typedef enum _pmStatus{
//////////	PM_STATUS_UNKNOW  = 0x00, // 未知状态
//////////	PM_STATUS_OFF	  = 0x50, // 关闭状态
//////////	PM_STATUS_ON	  = 0xD0, // 上电状态
//////////	PM_STATUS_READY   = 0x18  // 就绪状态
//////////}PM_STATUS;


// 预赋值方式
typedef enum _pre_issue_mothed{
	PREISSUE_MOTHED_PRE_ISSUE			= 0, // 预赋值
	PREISSUE_MOTHED_REENCODE_PRE_ISSUE	= 1, // 重编码+预赋值
}PRE_ISSUE_MOTHED;


// 重编码方式
typedef enum _re_encode_mothed{
	PREISSUE_MOTHED_REENCODE				= 0, // 重编码
	PREISSUE_MOTHED_INITIALIZATION_REENCODE	= 1, // 初始化+重编码
}RE_ENCODE_MOTHED;


// 设备提供商
typedef enum _device_provider{
	DEVICE_PROVIDER_DAT     = 0x000F, // DAT
	DEVICE_PROVIDER_FOUNDER = 0x0011  // 方正
}DEVICE_PROVIDER;

// 参数版本类型
typedef enum _parameter_ver_type
{
	NOMAL_CUR     = 0x00,  // 正式参数当前版本
	NORMAL_FUTURE = 0x01,  // 正式参数将来版本
	TEST_CUR      = 0x10,  // 测试参数当前版本
	TEST_FUTURE   = 0x11   // 测试参数将来版本
}PARAMETER_VER_TYPE;

// 登录登出类型
typedef enum _longin_type{
	LOGIN_ON  = 0x00,
	LOGIN_OFF = 0x01
}LOGIN_TYPE;

// AR发送原因
typedef enum _ar_reson{
	AR_IN_SERVICE    = 0x01,  //运营开始
	AR_OUTOF_SERVICE = 0x02,  // 运营结束
	AR_TIME_ON       = 0x03   // 到达指定时间间隔
}AR_REASON;

// 币种类型编码定义
typedef enum _banknoteAndCoinType_t{
	VALUE_UNKNOWN	= 0x00,
	CoinHalfYuan	= 0x10,
	Coin1Yuan		= 0x11,
	
	Banknote1Yuan	= 0x31,
	Banknote5Yuan	= 0x33,
	Banknote10Yuan	= 0x34,
	Banknote20Yuan	= 0x35,
	Banknote50Yuan	= 0x36,
	Banknote100Yuan	= 0x37,
}BankNoteAndCoinType_t;

// 钱箱种类定义
typedef enum _box_type_def_{
	UNDEFINE_BOX,
	COIN_HOPPER_1,
	COIN_HOPPER_2,
	COIN_CHANGE_BOX_1,
	COIN_CHANGE_BOX_2,
	COIN_COLL_BOX_1_1,
	COIN_COLL_BOX_1_5,
	COIN_COLL_BOX_2,
	BNR_CASH_BOX,
	BNR_CHANGE_BOX,
	BNR_RECYCLER_BOX_3,
	BNR_RECYCLER_BOX_4,
	BNR_RECYCLER_BOX_5,
	BNR_RECYCLER_BOX_6,
}MONEY_BOX_TYPE;


//--------------------------------------以下是通信协议头数据定义-----------------------------------------//
// AfcSysHdr_t AFC系统专用头结构体定义 10 BIN
typedef struct _afcsyshdr_t 
{
	BYTE OperatorID[3];             // 3 BCD 操作员ID
	_DATE DateTime;                 // 4 BCD 操作时间
	WORD DataLen;                   // 2 BIN 消息长度
	BYTE Reserve;                   // 1 BIN 预留
	_afcsyshdr_t(){
		memset(OperatorID, 0x00, sizeof OperatorID);
		DataLen = 0;
		Reserve = 0;
	}
}AfcSysHdr_t;

// SysComHdr_t 系统公共头结构体定义 56 BIN
typedef struct _syscomhdr_t {
	_DATE_TIME transTime;				//交易时间
	WORD resourceProviderID;			//资源提供商ID
	BYTE transType;								//交易类型
	BYTE transSubType;						//交易子类型
	WORD cityCode;								//城市代码
	WORD industryCode;						//行业代码
	BYTE lineCode;								//线路代码
	WORD stationCode;							//站点代码
	BYTE deviceType;							//设备类型
	AccDeviceID_t deviceID;				//设备代码
	DWORD transSerialNo;					//交易序列号
	BYTE dataType;								//数据类型
	WORD parameterVersion;				//参数类型
	BYTE samID[6];								//SAM卡终端机编号
	DWORD samTransSerialNo;			//SAM卡交易序列号
	_syscomhdr_t()
	{
		memset(samID,0,sizeof samID);
	}
}SysComHdr_t;

// SysCardCom_t 票卡公共域结构体定义 20 BIN
typedef struct _syscardcom_t {
	DWORD cardIssuerId;				// 4 BIN 发行票卡的发行人的唯一ID
	DWORD cardSerialNumber;         // 4 BIN 票卡的序号
	DWORD cardType;                 // 4 BIN 票卡的类型
	DWORD cardLifeCycleCount;       // 4 BIN 票卡的当前使用周期计数
	DWORD cardActionSequenceNumber; // 4 BIN 操作表序列号（如果有）
	_syscardcom_t(){
		cardIssuerId = 0;
		cardSerialNumber = 0;
		cardType = 0;
		cardLifeCycleCount = 0;
		cardActionSequenceNumber = 0;
	}
}SysCardCom_t;

// SysCardholderCom_t 持卡人公共头结构体定义 16 BIN
typedef struct _syscardholdercom_t {
	DWORD cardholderSerialNum;		// 4 BIN 在特定发行人范围内识别持卡人的专用号码
	DWORD cardholderIssuerId;		// 4 BIN 持卡人的发行人。（固定为ACC=0xffffffff）
	DWORD companyId;				// 4 BIN ACC 定义公司标识符。对员工票而言，这里指的是运营商编码。
	//		这是参与方的标识码。
	//		0..255 = 为ACC 以及运营商预留。
	//		256..65535 = 其他公司可用（从卡中取得）
	DWORD classificationLevel;		// 4 BIN 乘客级别1:VIP;255:未设
	_syscardholdercom_t(){
		cardholderSerialNum = 0;
		cardholderIssuerId = 0;
		companyId = 0;
		classificationLevel = 0;
	}
}SysCardholderCom_t;

// SysSecurityHdr_t 安全头结构体定义 12 BIN
typedef struct _syssecurityhdr_t {
	BYTE txnMac[4];					// 4 BIN MAC
	BYTE keyVersion;				// 1 BIN 用来产生交易MAC 的密钥版本
	_syssecurityhdr_t(){
		memset(txnMac, 0x00, sizeof txnMac);
		keyVersion = 0;
	}
}SysSecurityHdr_t;

// 内部交易数据公共域结构体定义	17 BIN
typedef struct _transComHdr_t
{
	DEVICE_ID deviceid;				// 1BIN+2BCD+1BIN 设备ID
	DWORD dwAfcSerialNo;			// 4 BIN AFC交易流水号
	_DATE_TIME dtTransDataTime;		// 7 BCD 交易产生时间
	BYTE bMsgType;					// 1 BIN 数据类型
	BYTE bMsgSubType;				// 1 BIN 数据子类型
	_transComHdr_t(){
		dwAfcSerialNo = 0;
		bMsgType = 0;
		bMsgSubType = 0;
	}
} TransComHdr_t;

// 业务数据专用头 32 BIN
typedef struct _opercomhdr_t{
	BYTE operatorID[3];				// 3 BCD 操作员ID
	_DATE dataTime;					// 4 BCD 发生日期
	BYTE localType;					// 1 BIN 位置类型
	WORD localAreaID;				// 2 BCD 位置ID
	DEVICE_ID deviceID;				// 4 1BIN+2BCD+1BIN 设备ID
	DWORD afcSeqNo;					// 4 BIN AFC数据流水号
	_DATE_TIME operTime;			// 7 BCD 业务数据产生时间
	BYTE msgType;					// 1 BIN 数据类型
	BYTE msgSubType;				// 1 BIN 数据子类型
	WORD msgLen;					// 2 BIN 消息长度
	//BYTE reserve[3];				// 3 预留
	_opercomhdr_t(){
		memset(operatorID, 0x00, sizeof operatorID);
		localType = 0;
		localAreaID = 0;
		afcSeqNo = 0;
		msgType = 0;
		msgSubType = 0;
		msgLen = 0;
		//memset(reserve, 0x00, sizeof reserve);
	}
}OperComHdr_t;

// 状态数据头 11 BIN
typedef struct _statusComHdr_t
{
	WORD wStatusCode;				// 2 BIN 数据类别代码
	WORD wStationCode;				// 2 BCD 车站ID
	DEVICE_ID deviceID;				// 4 1BIN+2BCD+1BIN 设备ID
	char operatorID[3];				// 3 BCD 操作员ID
	_statusComHdr_t(){
		wStatusCode = 0;
		wStationCode = 0;
		memset(operatorID,0x00,sizeof operatorID);
	}
} StatusComHdr_t;

//--------------------------------------以上是通信协议头数据定义-----------------------------------------//

// 预赋值属性
typedef enum _pre_pay_method_{
	PRE_PAY_CARD	= 0x01,		// 预赋值卡
	YKT_EMPTY_CARD	= 0x02,		// 一卡通白卡
	ACC_INIT_CARD	= 0x03,		// ACC初始化卡
}PRE_PAY_METHOD;

// 天线定义
typedef enum _AntennaMark__mothod_{
	ANTENNA_MARK_ALL	= 0x01,		// 双天线(A和B)同时寻卡
	ANTENNA_MARK_A		= 0x02,		// 只有A天线寻卡
	ANTENNA_MARK_B		= 0x03,		// 只有B天线寻卡
	ANTENNA_MARK_C		= 0x04,		// 只有C天线寻卡（供TVM后维护门读卡使用）
}ANTENNA_MARK_MOTHOD;
