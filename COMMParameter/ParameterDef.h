#ifndef	__COMMDEF_H_INCLUDED_
#define	__COMMDEF_H_INCLUDED_

#if	_MSC_VER > 1000
#pragma	once
#endif	// _MSC_VER	> 1000

#endif	// __COMMDEF_H_INCLUDED_

//static const TCHAR* const BOM_PM_UPDATE_DIR		            = _T("Download\\PM\\");             // 参数更新目录



// master文件名
// 单个版本的master文件名
static const TCHAR* const SOFTVER_FILE_NAME						= _T("SoftwareVersion.mst");    // 版本管理信息
static const TCHAR* const COUNT_INFO_FILE_NAME					= _T("CountInfo.mst");          // 计数信息
static const TCHAR* const AUDIT_INFO_FILE_NAME					= _T("AuditInfo.mst");          // 授权信息
static const TCHAR* const PRESELL_TICKET_FILE_NAME				= _T("PreTicketInfo.mst");      // 预制票信息
static const TCHAR* const REGISTER_INFO_FILE_NAME	            = _T("Register.mst"); 		    // 寄存器数据信息
static const TCHAR* const PSD_ERROR_INFO_FILE_NAME	            = _T("PasswordError.mst"); 	    // 密码错误的员工信息
//static const TCHAR* const EXCHANGE_BACKUP_FILE_NAME	            = _T("ExchangeBackup.mst");     // 更换备份卡信息
//static const TCHAR* const ECT_DATA_FILE_NAME	                    = _T("ECTData.mst");            // 一卡通交易保存数据
static const TCHAR* const DEVICE_INFO_FILE_NAME	                = _T("DeviceInfo.mst");         // 设备信息
static const TCHAR* const REFUND_APPLY_INFO_FILE_NAME	        = _T("RefundApplyInfo.mst");    // 非即时退款申请信息
static const TCHAR* const NAMEDCARD_APPLY_INFO_FILE_NAME	        = _T("NamedCardApplyInfo.mst"); // 记名卡申请信息
// 两个版本的master的路径
static const TCHAR* const MASTER_CURRENT_DIR                     = _T("current\\");              // 当前版本文件路径
static const TCHAR* const MASTER_FUTURE_DIR                      = _T("future\\");               // 将来版本文件路径

// 参数文件名
static const TCHAR* const PARAM_PREFIX							= _T("Para."); 						// 参数文件前缀
static const TCHAR* const ACC_PARAM_ID_0101						= _T("Para.0101.");					// ACC运营参数
static const TCHAR* const ACC_PARAM_ID_0102						= _T("Para.0102.");					// ACC行政处理手续费参数
static const TCHAR* const ACC_PARAM_ID_0103						= _T("Para.0103.");					// ACC超时补交费用参数
static const TCHAR* const ACC_PARAM_ID_0201						= _T("Para.0201.");					// ACC设备控制参数
static const TCHAR* const ACC_PARAM_ID_0301						= _T("Para.0301.");					// ACC车站位置参数
static const TCHAR* const ACC_PARAM_ID_0302						= _T("Para.0302.");					// ACC计费站点参数
static const TCHAR* const ACC_PARAM_ID_0303						= _T("Para.0303.");					// ACC区段参数
static const TCHAR* const ACC_PARAM_ID_0401						= _T("Para.0401.");					// ACC票卡属性参数
static const TCHAR* const ACC_PARAM_ID_0601						= _T("Para.0601.");					// ACC基础票价表参数
static const TCHAR* const ACC_PARAM_ID_0602						= _T("Para.0602.");					// ACC票价级别表参数
static const TCHAR* const ACC_PARAM_ID_0603					    = _T("Para.0603.");					// ACC票价调整策略表参数
static const TCHAR* const ACC_PARAM_ID_0604						= _T("Para.0604.");					// ACC时间浮动表参数
static const TCHAR* const ACC_PARAM_ID_0605						= _T("Para.0605.");					// ACC时间类型表参数
static const TCHAR* const ACC_PARAM_ID_0606						= _T("Para.0606.");					// ACC时间类型明细参数
static const TCHAR* const ACC_PARAM_ID_0607						= _T("Para.0607.");					// ACC折扣率表参数
static const TCHAR* const ACC_PARAM_ID_0608						= _T("Para.0608.");					// ACC停运车站表
static const TCHAR* const ACC_PARAM_ID_0801						= _T("Para.0801.");					// ACC白名单参数
static const TCHAR* const ACC_PARAM_ID_0802				    	= _T("Para.0802.");					// ACC异地卡/城市代码对照参数
static const TCHAR* const ACC_PARAM_ID_0701						= _T("Para.0701.");					// ACC单条黑名单参数
static const TCHAR* const ACC_PARAM_ID_0702						= _T("Para.0702.");					// ACC区段黑名单参数
static const TCHAR* const ACC_PARAM_ID_0501						= _T("Para.0501.");					// ACC票卡发行商参数

static const TCHAR* const ECT_PARAM_ID_4011						= _T("Para.4011.");					// ECT黑名单参数

static const TCHAR* const AFC_PARAM_ID_1002						= _T("Para.1002.");					// AFC模式履历参数
static const TCHAR* const AFC_PARAM_ID_1005						= _T("Para.1005.");					// AFC设备公共参数
static const TCHAR* const AFC_PARAM_ID_1006						= _T("Para.1006.");					// AFC设备操作员信息
static const TCHAR* const AFC_PARAM_ID_1007						= _T("Para.1007.");					// AFC设备权限参数
static const TCHAR* const AFC_PARAM_ID_1011						= _T("Para.1011.");					// AFC TVM运营参数
static const TCHAR* const AFC_PARAM_ID_1089						= _T("Para.1089.");					// AFC 运营时间参数
static const TCHAR* const AFC_PARAM_ID_1021						= _T("Para.1021.");					// AFC BOM运营参数



// 参数版本类型编码
typedef	enum _param_version_type
{
	CUR_VER  = 0x00,//正式当前版	
	FUT_VER	 = 0x01,//正式将来版	
	CUR_TEST = 0x10,//当前测试版
	FUT_TEST = 0x11,//将来测试版
	VER_TYPE_UNKNOW   = 0xFF //未知版	
} PARAM_VERSION_TYPE_CODE;

// BOM系统功能编码
typedef	enum _sys_function_code
{
	SYS_FUNCTION_ISSUE				= 0x14010101,	// 售票
	SYS_FUNCTION_ADJUST				= 0x14010201,	// 补票
	SYS_FUNCTION_CHARGE				= 0x14010301,	// 充值
	SYS_FUNCTION_REFUND				= 0x14010401,	// 退款
	SYS_FUNCTION_ANALYZE			= 0x14010701,	// 查询	
	SYS_FUNCTION_ACTIVATION			= 0x14010801,	// 激活
	SYS_FUNCTION_DEFER				= 0x14010901,	// 延期
	SYS_FUNCTION_OTHER				= 0x14010A01,	// 其他

	SYS_FUNCTION_REFUND_BY_METRO = 0x14010901,//地铁原因退款
	SYS_FUNCTION_EXCHANGE		= 0x14010501,//换卡
	SYS_FUNCTION_REFRESH			= 0x14010601,//激活

	SYS_FUNCTION_SYSTEM_SET			= 0x14020101,	// 系统设置	
	SYS_FUNCTION_DATA_MANAGE		= 0x14020201,	// 数据管理	
	SYS_FUNCTION_HARDWARE_TEST		= 0x14020301,	// 硬件测试	
	SYS_FUNCTION_REMOVEEXCEPTION	= 0x14020401,	// 异常解除

	// TVM 功能代码定义
	SYS_TVM_FUNCTION_MODE_SET				= 0x02010401,// 模式设定
	SYS_TVM_FUNCTION_CASH_SALE_INFO			= 0x02010301,// 交易一览、销售统计、设备状态
	SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE		= 0x02010501,// 纸币箱管理、硬币箱管理
	SYS_TVM_FUNCTION_CARD_BOX_MANAGE		= 0x02010601,// 票箱管理
	SYS_TVM_FUNCTION_BALANCE_MANAGE			= 0x02010701,// 结算处理、一键收机、凭证补打
	SYS_TVM_FUNCTION_DATA_MANAGE			= 0x02010801,// 数据管理
	SYS_TVM_FUNCTION_SYSTEM_HARDWARE_TEST	= 0x02010901,// 系统设定、硬件自检
} SYS_FUNCTION_CODE;

// ACC参数一览
typedef struct _version_acc{
	DWORD lBusinessParam;					// ACC运营参数
	DWORD lServerFeeParam;					// ACC行政处理手续费参数
	DWORD lOverTimeAdjuestParam;			// ACC超时补交费用参数
	DWORD lDeviceControlParam;				// ACC设备控制参数
	DWORD lStationParam;					// ACC车站位置参数
	DWORD lPayStationParam;					// ACC计费站点参数
	DWORD lCardAttributeParam;				// ACC票卡属性参数
	DWORD lBasePriceParam;					// ACC基础票价表参数
	DWORD lPrieLevelParam;					// ACC票价级别表参数
	DWORD lPriceAdjuestParam;				// ACC票价调整策略表参数
	DWORD lTimeFloatParam;					// ACC时间浮动表参数
	DWORD lTimeTypeParam;					// ACC时间类型表参数
	DWORD lTimeDetailParam;					// ACC时间类型明细参数
	DWORD lDiscountParam;					// ACC折扣率表参数
	DWORD lNotServStationParam;				// ACC停运车站表
	DWORD lWriteListParam;					// ACC白名单参数
	DWORD lOtherCityCardParam;				// ACC异地卡/城市代码对照参数
	DWORD lSingleBlackListParam;			// ACC单条黑名单参数
	DWORD lSectionBlackListParam;			// ACC区段黑名单参数
	DWORD lSectionParam;					// ACC区段参数
	DWORD lissueCompanyParam;				// ACC票卡发行商参数


	_version_acc(){
		lBusinessParam= 0xFFFFFFFF;					// ACC运营参数
		lServerFeeParam= 0xFFFFFFFF;					// ACC行政处理手续费参数
		lOverTimeAdjuestParam= 0xFFFFFFFF;			    // ACC超时补交费用参数
		lDeviceControlParam= 0xFFFFFFFF;				// ACC设备控制参数
		lStationParam= 0xFFFFFFFF;					    // ACC车站位置参数
		lPayStationParam= 0xFFFFFFFF;					// ACC计费站点参数
		lCardAttributeParam= 0xFFFFFFFF;				// ACC票卡属性参数
		lBasePriceParam= 0xFFFFFFFF;					// ACC基础票价表参数
		lPrieLevelParam= 0xFFFFFFFF;					// ACC票价级别表参数
		lPriceAdjuestParam= 0xFFFFFFFF;				// ACC票价调整策略表参数
		lTimeFloatParam= 0xFFFFFFFF;					// ACC时间浮动表参数
		lTimeTypeParam= 0xFFFFFFFF;					// ACC时间类型表参数
		lTimeDetailParam= 0xFFFFFFFF;					// ACC时间类型明细参数
		lDiscountParam= 0xFFFFFFFF;					// ACC折扣率表参数
		lNotServStationParam= 0xFFFFFFFF;				// ACC停运车站表
		lWriteListParam= 0xFFFFFFFF;					// ACC白名单参数
		lOtherCityCardParam= 0xFFFFFFFF;				// ACC异地卡/城市代码对照参数
		lSingleBlackListParam= 0xFFFFFFFF;			    // ACC单条黑名单参数
		lSectionBlackListParam= 0xFFFFFFFF;		  	// ACC区段黑名单参数
		lSectionParam= 0xFFFFFFFF;						// ACC区段参数
		lissueCompanyParam= 0xFFFFFFFF;				// ACC票卡发行商
	}
}VERSION_ACC;

 //一卡通版本
typedef struct _version_ect{
	DWORD lBlacklistParam;  // 一卡通黑名单参数类
	_version_ect(){
		lBlacklistParam = 0xFFFFFFFF;
	}
}VERSION_ECT;

// AFC系统参数
typedef struct _version_afc{
	DWORD lAccessLevelParam;   // 设备权限参数类
	DWORD lOperationParam;  // 运营参数类
	DWORD lDeviceCommonParam;  // 设备公共参数类
	DWORD lStaffPwdParam;      // 操作员参数类
	DWORD lDeviceRunTimeParam; // 设备运行时间参数
	//DWORD lOpratorCodeTable;   // 运营商代码映射表
	DWORD lModeHistoryParam;   // 模式履历
	_version_afc(){
		lAccessLevelParam = 0xFFFFFFFF;
		lOperationParam = 0xFFFFFFFF;
		lDeviceCommonParam = 0xFFFFFFFF;
		lStaffPwdParam = 0xFFFFFFFF;
		//lDeviceRunTimeParam = 0xFFFFFFFF;
		lModeHistoryParam = 0xFFFFFFFF;
	}
}VERSION_AFC;

// 参数及程序ID
typedef enum _parameter_id{
	AFC_MODEHISTORY_ID           = 0x1002,  // 模式履历
	AFC_DEVICECOMMONPARAM_ID     = 0x1005,  // 设备公共参数
	AFC_STAFFPWDPARAM_ID         = 0x1006,  // 设备操作员信息
	AFC_ACCESSLEVELPARAM_ID      = 0x1007,  // 设备权限参数
	AFC_TVMOPERATIONPARAM_ID     = 0x1011,  // TVM运营参数
	AFC_TCMOPERATIONPARAM_ID     = 0x1041,  // TCM运营参数
	AFC_BOMOPERATIONPARAM_ID     = 0x1021,  // BOM运营参数
	AFC_DEVICERUNTIMEPARAM_ID    = 0x1089,  // 设备运行时间参数

	ACC_BUSINESS_ID						= 0x0101,					// ACC运营参数
	ACC_SERVICEFEE_ID					= 0x0102,					// ACC行政处理手续费参数
	ACC_OVERTIME_UPDATE_ID				= 0x0103,					// ACC超时补交费用参数
	ACC_DEVICE_ID						= 0x0201,					// ACC设备控制参数
	ACC_STATION_ID						= 0x0301,					// ACC车站位置参数
	ACC_CHARGE_STATION_ID				= 0x0302,					// ACC计费站点参数
	ACC_SECTION_ID						= 0x0303,					// ACC区段参数
	ACC_CARDATTRIBUTE_ID				= 0x0401,					// ACC票卡属性参数
	ACC_ISSUE_COMPANY_ID			    = 0x0501,					// ACC票卡发行商参数
	ACC_BASE_PRICE_TABLE_ID				= 0x0601,					// ACC基础票价表参数
	ACC_PRICE_LEVEL_TABLE_ID			= 0x0602,					// ACC票价级别表参数
	ACC_PRICE_ADJUEST_TABLE_ID			= 0x0603,					// ACC票价调整策略表参数
	ACC_TIME_FLOATING_TABLE_ID			= 0x0604,					// ACC时间浮动表参数
	ACC_TIME_TYPE_TABLE_ID				= 0x0605,					// ACC时间类型表参数
	ACC_TTIME_TYPE_DETAIL_ID			= 0x0606,					// ACC时间类型明细参数
	ACC_DISSCOUNT_TABLE_ID				= 0x0607,					// ACC折扣率表参数
	ACC_NOT_SERVICE_STATION_ID			= 0x0608,					// ACC停运车站表
	ACC_SINGLE_BLACK_LIST_ID			= 0x0701,					// ACC单条黑名单参数
	ACC_SECTION_BLACK_LIST_ID			= 0x0702,					// ACC区段黑名单参数
	ACC_WRITE_LIST_ID					= 0x0801,					// ACC白名单参数
	ACC_OTHER_CITY_CARD_ID				= 0x0802,					// ACC异地卡/城市代码对照参数

	AFC_TVMPROGRAM_ID				= 0x4101,	// TVM应用程序
	AFC_BOMPROGRAM_ID				= 0x4102,   // BOM应用程序
	AFC_TCMPROGRAM_ID				= 0x4104,   // TCM应用程序(EQM)
	AFC_TPUPROGRAM_ID				= 0x4109,   // TPU程序
	//AFC_TVMGUI_ID					= ACC_STATION_MAP_ID,// GUI界面 
	//AFC_TPUMAINPROGRAM_ID        = 0x4110,// TPU Main程序
	AFC_THPROGRAM_ID				= 0x4110, // TH应用程序(BOM)
	AFC_TVMTHPROG_ID				= 0x4111, // TH应用程序（TVM）
	PARAM_UNKNOW					= 0x0000   
}PARAMETER_ID;

// 两个版本的master的路径
static const TCHAR* const PARAM_ACC_CURRENT_DIR                     = _T("parameters\\current\\");     // ACC当前参数文件路径
static const TCHAR* const PARAM_ACC_FUTURE_DIR                      = _T("parameters\\future\\");      // ACC将来参数文件路径
static const TCHAR* const PARAM_ACC_BACKUP_DIR                      = _T("parameters\\backup\\");      // ACC当前参数文件路径
static const TCHAR* const PARAM_ECT_CURRENT_DIR					   = _T("parameters\\current\\");     // ECT当前参数文件路径
static const TCHAR* const PARAM_ECT_FUTURE_DIR					   = _T("parameters\\future\\");      // ECT将来参数文件路径
static const TCHAR* const PARAM_ECT_BACKUP_DIR					   = _T("parameters\\backup\\");      // ECT将来参数文件路径
static const TCHAR* const PARAM_AFC_CURRENT_DIR					   = _T("parameters\\current\\");     // AFC当前参数文件路径
static const TCHAR* const PARAM_AFC_FUTURE_DIR					   = _T("parameters\\future\\");      // AFC将来参数文件路径
static const TCHAR* const PARAM_AFC_BACKUP_DIR					   = _T("parameters\\backup\\");      // AFC将来参数文件路径

// 文件路径
static const TCHAR* const DEVICE_MASTER_DIR         = _T("master\\");                   // master文件目录
static const TCHAR* const DEVICE_MESSAGE_DIR        = _T("messages\\");                  // message文件目录

// master文件名
// 单个版本的master文件名
static const TCHAR* const PEKG_AUDIT_FILE_NAME					= _T("PekgAuditInfo.mst");      // 包传输审计信息

#define BOM_INI_SECTION_APPLICATION					_T("APPLICATION")		// BOM.ini里的Section

#define BOM_INI_KEY_DATAPATH						_T("DataPath")   		// 主数据目录（绝对路径）
#define BOM_INI_KEY_DATAPATH_BKP					_T("BackupPath ")		// 备份数据目录（绝对路径）




// 澳门新版参数 系统基本类型


// 
//typedef enum _cardholderFeeType_t{
//	CARDHOLDER_FEE_TYPE_REPLACEMENT      = 2,     // Fee TCHARged for replacement.
//	CARDHOLDER_FEE_TYPE_REFUND           = 5,     // Fee TCHARged for a refund.
//	CARDHOLDER_FEE_TYPE_PERSONALISATION  = 13,    // Fee TCHARged for personalisation.
//	CARDHOLDER_FEE_TYPE_UNSPECIFIED      = 255    // No value set.无设定值
//}CardholderFeeType_t;

