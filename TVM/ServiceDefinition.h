#pragma  once

// 各业务及服务ID定义
typedef	enum _service_id 
{
	UNKNOWN_SVC								= -1,	// 未定业务

	// 乘客业务ID（1 - 99）
	ISSUE_SVC								= 10,	// 售票
	BLIND_ISSUE_SVC							= 11,	// 盲人售票
	CHARGE_SVC								= 20,	// 充值
	BLIND_CHARGE_SVC						= 21,	// 盲人充值
	ISSUE_PRODUCT_SVC						= 30,	// 售产品
	BLIND_ISSUE_PRODUCT_SVC					= 31,	// 盲人售产品
	ANALYZE_SVC								= 40,	// 查询
	ISSUE_USE_SVT_SVC						= 41,	// 储值票购票
	LANGUAGE_SWITH_SVC						= 42,	// 语言切换
	CALL_HELP_SVC							= 50,	// 招援
	ADJUST_SVC								= 60,	// 补票（@#保留）
	REFUND_SVC								= 70,	// 退款（@#保留）
	EXCHANGE_SVC							= 80,	// 换卡（@#保留）
	STANDBY_SVC								= 90,	// 待机
	SVT_ISSUE_SVC							= 91,	// 储值卡购票
	// 维护业务ID（100 - 999）
	MAINTENANCE_SVC							= 300,	// 维护业务
	TRANSACTION_HISTORY_SVC					= 310,	// 交易一览
	DEVICE_STATUS_SVC						= 320,	// 设备状态 
	SALE_STATIC_SVC							= 330,	// 销售统计
	CARD_BOX_MANAGE_SVC						= 340,	// Card箱管理
	TOKEN_BOX_MANAGE_SVC					= 350,	// Token箱管理
	BANKNOTE_BOX_MANAGE_SVC					= 360,	// 纸币箱管理
	INSTALL_BANKNOTE_COLLECTION_BOX_SVC					=361,	// 纸币回收箱安装
	REMOVE_BANKNOTE_COLLECTION_BOX_SVC					=362,	// 纸币回收箱卸载
	INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC				=363,	// 纸币找零箱安装
	REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC				=364,	// 纸币找零箱卸载
	BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC				=365,	// 纸币模块回收纸币

	COIN_BOX_MANAGE_SVC						= 370,	// 硬币箱管理
		ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC	= 371,	// 硬币专用找零箱加币
		ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC		= 372,	// 硬币循环找零箱
		ATTACH_COLLECTION_COIN_BOX_SVC					= 373,	// 安装硬币回收箱
		DETACH_COLLECTION_COIN_BOX_SVC					= 374,	// 卸载硬币回收箱
		DETACH_ADDING_COIN_BOX_SVC						= 375,	// 卸载硬币补充箱
		COLLECTION_COIN_SVC								= 376,	// 回收硬币
		INSTALL_COIN_CHANGE_BOX_SVC						= 377,	// 安装硬币找零箱
		REMOVE_COIN_CHANGE_BOX_SVC						= 378,	// 卸载硬币找零箱


	OPERATION_CLOSE_SVC						= 380,	// 业务结束
	RUN_MODE_SETTING_SVC					= 400,	// 运行模式设定
		SERVICE_MODE_SETTING_SVC			= 401,	// 服务模式设定
		PAYMENT_MODE_SETTING_SVC			= 402,	// 支付模式设定
		CHANGE_MODE_SETTING_SVC				= 403,	// 找零模式设定
		PRINT_MODE_SETTING_SVC				= 404,	// 打印模式设定
	RECEIPT_REPRINT_SVC						= 410,	// 凭证补打
	SYSTEM_SET_SVC							= 420,	// 系统设定
		STATION_SET_SVC						= 421,	// 机站设置
		COMMUNICATION_SET_SVC				= 422,	// 通信参数设置
		SYSTEM_TIME_SET_SVC					= 423,	// 系统时间设定
		SWITCH_OFF_SVC						= 424,	// 切离设定
		MODE_SWITCH_SVC						= 425,	// 模式切换设置（@#保留）
		UDSN_SET_SVC						= 426,	// UDSN设定（@#保留）
		RFID_MODE_SET_SVC					= 427,	// RFID模式设定（@#保留）
		SCREEN_BRIGHTNESS_SET				= 428,  // 屏幕亮度设定
		DEVICE_COMPORT_SET_SVC				= 429,	// 串口号与波特率设置
	DATA_MANAGE_SVC							= 440,	// 数据管理
		DATA_INPUT_SVC						= 431,	// 数据导入
		DATA_OUTPUT_SVC						= 432,	// 数据导出
		VERSION_INFO_SVC					= 433,	// 版本信息
	DEVICE_TEST_SVC							= 700,	// 硬件自检
		RW_TEST_SVC							= 710,	// 读写器测试
		COIN_MODULE_TEST_SVC				= 720,	// 硬币模块测试
		BANKNOTE_ACCEPT_MODULE_TEST_SVC		= 730,	// 纸币接收模块测试
		BANKNOTE_CHANGE_MODULE_TEST_SVC		= 740,	// 纸币找零模块测试
		CARD_MODULE_TEST_SVC				= 750,	// 发卡模块测试
		TOKEN_MODULE_TEST_SVC				= 760,	// Token模块测试
		RECEIPT_RINTER_TEST_SVC				= 770,	// 凭证打印机测试
		PIN_PAD_TEST_SVC					= 780,	// 密码键盘模块测试
		BANKCARD_MODULE_TEST_SVC			= 790,	// 银行卡处理模块测试
		IO_MODULE_TEST_SVC					= 800,	// IO模块测试
		NETWORK_TEST_SVC					= 810,	// 网络测试
		STATUS_DISPLAY_TEST_SVC				= 820,	// 状态显示屏测试（@#保留）
		UPS_TEST_SVC						= 830,	// UPS测试（@#保留）
		ALL_DEVICE_STATUS_SVC				= 840,	// 所有硬件状态（硬件自检）
	REMOVE_EXCEPTION_SVC					= 900,	// 异常解除
	ONE_KEY_SETTLEMENT_SVC					= 910,	// 一键收机

	// 主程序程序内部控制业务ID（1000 -	1999）
	ROOT_SVC								= 1000,	// 根
	MAIN_SVC								= 1010,	// 主控
	INIT_SVC								= 1050,	// 初始化
	LOGIN_SVC								= 1060,	// 登录
	LOGOFF_SVC								= 1070,	// 登出
	END_SVC									= 1080,	// 业务结束
	SHUTDOWN_RESTART_SVC					= 1090,	// 关机重启业务
		SHUTDOWN_SVC						= 1100,	// 关机
		RESTART_SVC							= 1110,	// 重启动
	EXIT_SVC								= 1120,	// 退出程序
	WAIT_MAINTENANCE_SVC					= 1130,	// 等待维护操作（语言选择）

	// 后台服务ID（2000 -	9999）
	SC_SVC									= 2000,
		TRANSMISSION_INTERVAL_SVC			= 2020,	// 定期传送
		BUSINESS_INTERVAL_SVC				= 2030,	// 业务数据定期传送
		EVENT_MSG_SVC						= 2040,	// 事件数据处理服务
		STATUS_MGR_SVC						= 2070,	// 状态管理服务
	MONITOR_SVC								= 3000,
		MONITOR_RECHARGE_RW_SVC				= 3010,	// 充值读写器状态监控
		MONITOR_CARD_RW__SVC				= 3020,	// 票卡读写器状态监控
		MONITOR_TOKEN_RW__SVC				= 3030,	// Token读写器状态监控
		MONITOR_CARD_ISSUE_MODULE_SVC		= 3040,	// 发卡模块状态监控
		MONITOR_UPS_SVC						= 3050,	// UPS监控
		MONITOR_IO_SVC						= 3060,	// IO监控
		MONITOR_TOKEN_MODULE_SVC			= 3070,	// TOKEN模块监控
		MONITOR_COIN_MODULE_SVC				= 3080,	// 硬币模块监控
		MONITOR_BANKNOTE_ACCEPT_MODULE_SVC	= 3090,	// 纸币接收模块监控
		MONITOR_BANKNOTE_CHANGE_MODULE_SVC	= 3100,	// 纸币找零模块监控
		MONITOR_PINPAD_MODULE_SVC			= 3110,	// 密码键盘模块监控
		MONITOR_BANKCARD_MODULE_SVC			= 3120,	// 银行卡处理模块监控
		MONITOR_PRINTER_MODULE_SVC			= 3130,	// 打印机模块监控
	CONFIRM_ALIVE_SVC						= 4000,	// 确认存活服务

} SERVICE_ID;


