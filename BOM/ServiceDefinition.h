#pragma  once

// 各业务及Lib的ID
typedef	enum _service_id {
	UNKNOWN_SVC					= -1,		// 未定业务

	// 主业务ID（1 - 99）
	ISSUE_SVC					= 10,		// 售票
	CHARGE_SVC				= 20,		// 充值
	//ADDTIMES_SVC				= 30,		// 充次
	ADJUST_SVC					= 40,		// 补票
	ANALYZE_SVC					= 50,		// 分析
	REFUND_SVC					= 60,		// 退款
		REFUND_BASE_SVC			= 61,
		REFUND_APPLY_SVC		= 62,
		REFUND_QUERY_SVC		= 63,
//	SURRENDER_SVC				= 70,		// 退票
	EXCHANGE_SVC	            = 70,		// 替换
	PREISSUE_SVC				= 80,		// 预售
	COUNTERACT_SVC				= 90,		// 抵销
	
	// 其他业务ID（100 - 200）
	OTHERS_SVC					= 99,		// 其他
		TVMFAILURE_REFUND_SVC		= 100,		// TVM故障退款
		NOBUSINESS_INCOME_SVC		= 110,		// 非营业收支
		REFRESH_SVC					= 120,		// 年检
		SIGNCARD_APPLY_SVC			= 130,		// 记名卡申请
		SIGNCARD_REPORT_LOSS_SVC	= 140,		// 记名卡挂失
		SIGNCARD_CANCEL_REPORT_LOSS_SVC = 150,	//记名卡解挂
		CARD_SUSPEND_AND_RESUME_SVC = 160,		//挂失解挂
		TICKETBOX_EXCHANGE_SVC = 170,						//票箱更换
		ADMINISTRATION_PROCESS_SVC = 180,
		LOCK_SVC = 190,
		DEFER_SVC = 200,
		RECEIPT_PRINT_SVC = 210,						// 打印单据
	// 辅助业务ID（300 - 499）
	ASSITANT_SVC				= 299,		// 辅助
		SALE_STATIC_SVC				= 300,		// 销售统计

	// 维护业务ID（500 - 999）
	SYSTEM_SET_SVC				= 500,		// 系统设定
	STATION_SET_SVC				= 510,		// 机站设置
	COMMUNICATION_SET_SVC		= 520,		// 通信参数设置
	MODE_SWITCH_SVC				= 530,		// 模式切换设置
	SYSTEM_TIME_SET_SVC			= 540,		// 系统时间设定
	UDSN_SET_SVC				= 550,		// UDSN设定
	TH_MODE_SET_SVC				= 560,		// TH模式设定
	RFID_MODE_SET_SVC			= 570,		// RFID模式设定
	DATA_MANAGE_SVC				= 600,		// 数据管理
		DATA_INPUT_SVC			= 610,		// 数据导入
		DATA_OUTPUT_SVC			= 620,		// 数据导出
	DEVICE_TEST_SVC				= 700,		// 硬件自检
		OUTRW_TEST_SVC			= 710,		// 读写器测试
		PRINTER_TEST_SVC        = 720,		// 打印机测试
		MONEYBOX_TEST_SVC		= 730,		// 钱箱测试
		NETWORK_TEST_SVC		= 740,		// 网络测试
		UPS_INFO_SVC			= 750,		// UPS信息
		TH_TEST_SVC				= 760,		// 出票机测试
	REMOVE_EXCEPTION_SVC        = 800,		// 异常解除

	// 主程序程序内部控制业务ID（1000 -	9989）
	ROOT_SVC					= 1000,		// 根
	MAIN_SVC					= 1010,		// 主控
	INIT_SVC					= 1050,		// 初始化
	LOGIN_SVC					= 1060,		// 登录
	LOGOFF_SVC					= 1070,		// 登出
	END_SVC						= 1080,		// 业务结束
	SHUTDOWN_SVC				= 1090,		// 关机
	EXIT_BOM_SVC				= 1100,		//退出BOM程序

	// 重启动
	RESTART_SVC                 = 1180,     // 重启动

	// 初期化异常解除
	REMOVE_INIT_EXCEPTION_SVC   = 1190,		// 初期化异常解除
	
	SC_SVC						= 2000,
		TRANSMISSION_INTERVAL_SVC	= 2020,		// 定期传送
		BUSINESS_INTERVAL_SVC       = 2030,     // 业务数据定期传送
		EVENT_MSG_SVC               = 2040,     // 事件数据处理服务
		STATUS_MGR_SVC              = 2070,     // 状态管理服务
	MONITOR_SVC	= 3000,
		MONITOR_RW_SVC  = 3010, //外部读写器监控
		MONITOR_TW_SVC  = 3020, //内部读写器监控
		MONITOR_TH_SVC  = 3030, //出票机监控
		MONITOR_UPS_SVC = 3040, //UPS监控
		MONITOR_LPT_SVC = 3050, //出票机门禁监控
	CONFIRM_ALIVE_SVC = 4000,		//确认存活服务

} SERVICE_ID;


