#include "util.h"
#include "UIDefinition.h"
#pragma once

//画面显示用常量定义
//static const int MAX_ARRAY_LEN	= 9;            ///< 普通信息区域最大行数

// 子菜单按钮位置
//#define  SUBMENU_BTN1_POINT          CPoint(36,489)			///< 子菜单第一个按钮左上角坐标
//#define  SUBMENU_BTN2_POINT          CPoint(143,489)		///< 子菜单第二个按钮左上角坐标
//#define  SUBMENU_BTN3_POINT          CPoint(250,489)			///< 子菜单第三个按钮左上角坐标
//#define	SUBMENU_BTN4_POINT          CPoint(357,489)			///< 子菜单第四个按钮左上角坐标
//#define  SUBMENU_BTN5_POINT          CPoint(464,489)			///< 子菜单第五个按钮左上角坐标
//#define  PRINT_BTN_POINT             CPoint(571,489)			///< 子菜单打印按钮左上角坐标
//#define  BACK_BTN_POINT              CPoint(678,489)			///< 子菜单返回按钮左上角坐标
//#define  OK_BTN_POINT               CPoint(785,489)			///< 子菜单确定按钮左上角坐标
//#define  STANDARD_BTN_POINT         CPoint(NORMAL_BUTTON_WIDTH,NORMAL_BUTTON_HEIGHT)
//#define   SUBMENU_BTN3_RECT           CRect(SUBMENU_BTN3_POINT,SUBMENU_BTN3_POINT+STANDARD_BTN_POINT)	///< 子菜单第三个按钮大小
//#define   SUBMENU_BTN4_RECT           CRect(SUBMENU_BTN4_POINT,SUBMENU_BTN4_POINT+STANDARD_BTN_POINT)	///< 子菜单第四个按钮大小
//#define   SUBMENU_BTN5_RECT           CRect(SUBMENU_BTN5_POINT,SUBMENU_BTN5_POINT+STANDARD_BTN_POINT)	///< 子菜单第五个按钮大小
//
//static const int DATAINPUT_MSG_POINT_X           =   26;
//static const int DATAINPUT_DATATYPE_MSG_POINT_Y  =    8;
//static const int DATAINPUT_INPUTTYPE_MSG_POINT_Y =  172;
//
//static const int DEVICETEST_TESTTYPE_MSG_POINT_X     =   26;
//static const int DEVICETEST_TESTTYPE_MSG_POINT_Y     =    8;
//static const int DEVICETEST_TESTDETAIL_MSG_POINT_X   =   26;
//static const int DEVICETEST_TESTDETAIL_MSG_POINT_Y   =  264;
//
//static const int DEVICETEST_DETAILITEM_MSG_POINT_Y =  292;

// 服务模式
typedef	enum _service_mode_id{
	//0x1F00,			   // 正常服務
	//0x0000,			   // 暫停服務
}SERVICE_MODE_ID;

// 业务模式定义
typedef	enum _business_mode_id{
	//0x1F00,			   // 正常业务模式
	//0x1000,			   // 只售票
	//0x0800,			   // 只售卡
	//0x0400	,			   // 只售产品
	//0x0200	,			   // 只充值
	//0x0100,			   // 	只查询
	//0x0F00	,			   // 不售票
	//0x1700,			   // 	不售卡
	//0x1B00	,			   // 不售产品
	//0x1D00,			   // 	不充值
	//0x1E00	,			   // 不查询
	//0x1800,			   // 	售票+售卡
	//0x1400	,			   // 售票+售产品
	//0x1200,			   // 	售票+充值
	//0x1100	,			   // 售票+查询
	//0x0C00,			   // 	售卡+售产品
	//0x0A00,			   // 	售卡+充值
	//0x0900,			   // 	售卡+查询
	//0x0600,			   // 	售产品+充值
	//0x0500,			   // 	售产品+查询
	//0x0300	,			   // 充值+查询
	//0x1C00,			   // 	售票+售卡+售产品
	//0x1600	,			   // 	售票+售卡+充值
	//0x1A00,			   // 		售票+售卡+查询
	//0x1900,			   // 		售票+售产品+充值
	//0x1500,			   // 		售票+售产品+查询
	//0x1300	,			   // 	售票+充值+查询
	//0x0E00	,			   // 	售卡+售产品+充值
	//0x0D00,			   // 		售卡+售产品+查询
	//0x0700	,			   // 	售产品+充值+查询
} BUSINESS_MODE_ID;

// 支付模式定义
typedef	enum _payment_mode_id{
	//0x003C	正常收款模式
	//0x0000	不收款模式
	//0x0020	只收紙幣
	//0x0010	只收硬幣
	//0x0008	只收银行卡
	//0x0004	只收EP
	//0x001C	不收紙幣
	//0x002C	不收硬幣
	//0x0034	不收銀行卡
	//0x0038	不收EP
	//0x000C	不收現金
	//0x0024	不收硬幣/銀行卡
	//0x0028	不收硬幣/EP
	//0x0014	不收紙幣/銀行卡
	//0x0018	不收紙幣/EP
	//0x0030	不收銀行卡/EP
}PAYMENT_MODE_ID;

// 找零模式
typedef	enum _change_mode_id{
	//0x0002	正常找零
	//0x0000	無找零
}CHANGE_MODE_ID;

// 打印模式
typedef	enum _print_mode_id{
	//0x0001	可打印
	//0x0000	無打印
}PRINT_MODE_ID;


// 语音ID定义
typedef enum  _audio_id{
	// 通用语音
	AUDIO_ID_WELCOME									= 4001,	//	欢迎光临，请选择画面上的按钮
	AUDIO_ID_THANKS										= 4002,	//	交易成功，谢谢使用
	AUDIO_ID_CALLING_STAFF								= 4003,	//	呼叫站员中，请稍后
	AUDIO_ID_INSERT_CASH								= 4004,	//	请投入现金
	AUDIO_ID_INSERT_BANKCARD							= 4005,	//	请插入银行卡
	AUDIO_ID_SWIPE_EP_CARD								= 4006,	//	请刷储值卡
	AUDIO_ID_SWIPE_CARD									= 4007,	//	请刷卡
	AUDIO_ID_SWIPE_CARD_AGAIN							= 4008,	//	请重新刷卡
	AUDIO_ID_INVALIDATE_CARD							= 4009,	//	该卡不能用，请到窗口处理
	AUDIO_ID_SWITCH_PAYMENT								= 4010,	//	该卡不能用，请选择其他支付方式
	AUDIO_ID_INPUT_ERROR								= 4011,	//	输入内容不合法，请确认后重新输入
	AUDIO_ID_TRANSACTION_CANCELED						= 4012,	//	交易已取消，谢谢使用
	AUDIO_ID_TRANSACTION_FAILED							= 4013,	//	交易失败，请联系工作人员	
	AUDIO_ID__WAITINGTRANSACTION						= 4014,	//  交易中,请稍候

	// 盲人专用
	AUDIO_ID_LANGUAGE_TYPE								= 4015,	//  语言种类
	AUDIO_ID_BUSINESS_TYPE								= 4016,	//	业务类型
	AUDIO_ID_TICKET_TYPE								= 4017,	//	车票种类
	AUDIO_ID_ISSUE_METHOD								= 4018,	//	购票方式
	AUDIO_ID_HOT_STATION								= 4019,	//	热点车站
	AUDIO_ID_TICKET_PRICE								= 4020,	//	车票单价
	AUDIO_ID_START_SECTION								= 4021,	//  起始区段
	AUDIO_ID_END_SECTION								= 4022,	//	目的区段
	AUDIO_ID_START_LINE									= 4023,	//	起始线路
	AUDIO_ID_END_LINE									= 4024,	//	目的线路
	AUDIO_ID_START_STATION								= 4025, //  起始车站
	AUDIO_ID_END_STATION								= 4026,	//	目的车站
	AUDIO_ID_ISSUE_QUANTITY								= 4027,	//  购票张数
	AUDIO_ID_PASSENGER_TYPE								= 4028,	//	乘客类型
	AUDIO_ID_PRODUCT_TYPE								= 4029,	//	产品类型
	AUDIO_ID_TRANSACTION_INFO							= 4030,	//	交易信息
	AUDIO_ID_PAYMENT_METHOD								= 4031, //  支付方式
	AUDIO_ID_PAYMENT_INFO								= 4032,	//	支付信息
	AUDIO_ID_PAYMENT_PASSWORD							= 4033, //  支付密码
	AUDIO_ID_CHARGE_AMOUNT								= 4034,	//	充值金额
	AUDIO_ID_TRANSACTION_OK_CANCEL						= 4035, //	请确认交易信息，然后按下确认键，返回请按取消键
	AUDIO_ID_TICKET_PEIECE								= 4041, //	张

	AUDIO_ID_PLEASE_PRESS								= 4900,	//  请按
	AUDIO_ID_PLEASE_SELECT								= 4901,	//	请选择
	AUDIO_ID_PLEASE_INPUT								= 4902,	//	请输入
	AUDIO_ID_PLEASE_CONFIRM								= 4903,	//	请确认
	AUDIO_ID_RANGE_1_TO_9								= 4904, //  范围1到9
	AUDIO_ID_OK_OR_CANCEL								= 4905,	//	然后按下确认键，返回请按取消键

	AUDIO_ID_CN											= 4001,	//	普通话
	AUDIO_ID_CTN										= 4002,	//	粤语
	AUDIO_ID_EN											= 4003,	//	英语
	AUDIO_ID_PN											= 4004,	//	葡萄牙语

	AUDIO_ID_BUY_TICKETS								= 4010, //	购票
	AUDIO_ID_BUY_CARDS									= 4011, //	购卡
	AUDIO_ID_RECHARGE									= 4012,	//	充值
	AUDIO_ID_QUERY										= 4013, //	查询

	AUDIO_ID_ONE_WAY_TICKET								= 4040,	//	单程票
	AUDIO_ID_ROUND_TRIP_TICKET							= 4041, //	往返票
	AUDIO_ID_STORED_VALUE_TICKET						= 4042, //	储值票
	AUDIO_ID_COUNT_TICKET								= 4043, //	计次票
	AUDIO_ID_PERIODICAL_TICKET							= 4044, //	定期票

	AUDIO_ID_NUM_1										= 4060,	//	一
	AUDIO_ID_NUM_2										= 4061,	//	二
	AUDIO_ID_NUM_3										= 4062,	//	三
	AUDIO_ID_NUM_4										= 4063,	//	四
	AUDIO_ID_NUM_5										= 4064,	//	五
	AUDIO_ID_NUM_6										= 4065,	//	六
	AUDIO_ID_NUM_7										= 4066,	//  七
	AUDIO_ID_NUM_8										= 4067,	//	八
	AUDIO_ID_NUM_9										= 4068,	//	九
	AUDIO_ID_NUM_10										= 4069,	//	十（ten）
	AUDIO_ID_NUM_11_EN									= 4070,	//	十一(eleven)
	AUDIO_ID_NUM_12_EN									= 4071,	//	十二(twelve)
	AUDIO_ID_NUM_3_EN									= 4072,	//	三(thir)
	AUDIO_ID_NUM_4_EN									= 4073,	//	四(four)
	AUDIO_ID_NUM_5_EN									= 4074,	//	五(fif)
	AUDIO_ID_NUM_TY										= 4075,	//	几十(ty)
	AUDIO_ID_NUM_100									= 4076,	//	百
	AUDIO_ID_NUM_1000									= 4077,	//	千
	AUDIO_ID_NUM_10000									= 4078,	//	万
	AUDIO_ID_NUM_AND									= 4079,	//	和（and）
	AUDIO_ID_NUM_POINT									= 4080, //	点
	AUDIO_ID_NUM_TEEN									= 4081, //	十（teen）
	AUDIO_ID_NUM_MINUS									= 4082, //	负
	AUDIO_ID_NUM_0										= 4083, //	零

	AUDIO_ID_UNIT_YUAN									= 4020,	//	元
	AUDIO_ID_UNIT_JIAO									= 4021,	//	角
	AUDIO_ID_UNIT_FEN									= 4022,	//	分

	AUDIO_ID_TIME_YEAR									= 4400,	//	年
	AUDIO_ID_TIME_MONTH									= 4401,	//	月
	AUDIO_ID_TIME_DAY									= 4402,	//	日
	AUDIO_ID_TIME_HOUR									= 4403,	//	时
	AUDIO_ID_TIME_MINUTE								= 4404,	//	分
	AUDIO_ID_TIME_SECOND								= 4405,	//	秒
	AUDIO_ID_UNIT_TIMES									= 4406,	//	次

	AUDIO_ID_GUANZHA									= 4200, //	关闸
	AUDIO_ID_MACHANGDONG								= 4201, //	马场东
	AUDIO_ID_HEISHAHUANPARK								= 4203, //	黑沙环公园

	AUDIO_ID_ADULT										= 4000, //	成人

	AUDIO_ID_CASH_PAYMENT								= 4420, //	现金支付
	AUDIO_ID_BANKCARD_PAYMENT							= 4421,	//	银行卡支付
	AUDIO_ID_STORED_VALUE_CARD_PAYMENT					= 4422, //	储值卡支付

	AUDIO_ID_PAYMENT_DUE								= 4430,	//	应付金额
	AUDIO_ID_PAYMENT_PAID								= 4431,	//	已付金额
	AUDIO_ID_PAYMENT_CHANGE								= 4432,	//	找零金额
	AUDIO_ID_PAYMENT_DEPOSIT							= 4433,	//	押金
	AUDIO_ID_PAYMENT_TAX								= 4434,	//	税费
	AUDIO_ID_PAYMENT_TOTAL								= 4435,	//	合计金额
	AUDIO_ID_PAYMENT_RECHARGE							= 4436, //  充值金额

	AUDIO_ID_HOT_SPOT_BUY_TICKET						= 4440,	//	热点购票
	AUDIO_ID_PRICE_BUY_TICKET							= 4441, //	票价购票
	AUDIO_ID_STATION_BUY_TICKET							= 4442,	//	站点购票

	// COMMON
	AUDIO_COMMON_PLEASE_SELECT_LANGUAGE					= 1015, //  语言选择：			普通话请按1，粤语请按2，英语请按3，葡萄牙语请按4
	AUDIO_COMMON_MSG_DEGRADE							= 1035, //  降级提示：			当前不支持打印凭证，找零，硬币支付，纸币支付，是否继续?继续请按确认键，取消请按取消键
	AUDIO_COMMON_SELECT_BUSINESS						= 1016, //	业务选择提示：		请选择业务类型，然后按下确认键,取消请按取消键，购单程票请按1，购卡请按2，充值请按3
	AUDIO_COMMON_CALL_HELP_ON							= 1003, //  招援按钮按下：		正在招援中，请稍后…（四语言）
	AUDIO_COMMON_CALL_HELP_OFF							= 1036, //  招援按钮弹起：		招援已取消，请继续操作。
	AUDIO_COMMON_INSERT_MAX_AMOUNT						= 1037, //	现金投入超限：		现金投入数量已达到最大限制。已投入的现金99,999.99MOP将被退回。请拿好您的现金，谢谢。
	AUDIO_COMMON_CHANGE_MAX_COUNT						= 1038, //  硬币找零数量超限：	硬幣找零超過最大限制。	已投入的現金99,999.99MOP將被退回。	請拿好您的現金，謝謝。
	AUDIO_COMMON_CHANGE_NOT_ENOUGH						= 1039, //  找零不足：			對不起，硬幣找零不足。	已投入的現金99,999.99MOP將被退回。	請拿好您的現金，謝謝。
	AUDIO_COMMON_MSG_NOT_SUPPORT_BUSINESS				= 1049, //  提示信息：			对不起，当前不支持此业务
	AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT				= 1051, //  提示信息：			对不起，当前不支持此支付方式!!!
	AUDIO_COMMON_MSG_NOT_SUPPORT_TICKET_TYPE			= 1052, //  提示信息：			对不起，当前不支持此票种!!!
	AUDIO_COMMON_MSG_INPUT_ERROR						= 1011,	//  提示信息：			输入有误，请重新输入
	AUDIO_COMMON_MSG_TIMEOUT_CANCEL						= 1050, //  提示信息：			交易因超时取消，请拿好您的现金，谢谢。
	AUDIO_COMMON_MSG_INPUT_AN_ERROR_STATION				= 1058,	//  提示信息：			此站为本站，不允许输入
	AUDIO_COMMON_MSG_NOT_SUPPORT_BLIND_SERVICE			= 1060,	//	提示信息：			对不起，当前不支持盲人业务。
	
	AUDIO_BUSINESS_SUCCESS								= 1002,	//  交易成功
	AUDIO_BUSINESS_FAILED								= 1013,	//  交易失败，请联系工作人员
	AUDIO_BUSINESS_CANCEL								= 1044,	//	交易取消，退款
	AUDIO_BUSINESS_PRINT_SUCCESS						= 1045,	//	打印完成
	AUDIO_BUSINESS_CANCEL_NORMAL						= 1012,	//  交易取消
	AUDIO_BUSINESS_DOING								= 1014,	//  交易中...
	AUDIO_SELECT_PAYMENT								= 1031,	//	选择支付方式
	AUDIO_INSERT_CASH									= 1004,	//	投入现金
	AUDIO_CONFORM_PAYINFO								= 1032,	//	支付确认
	AUDIO_INSERT_BANKCARD								= 1005,	//	插入银行卡
	AUDIO_INPUT_PASSWORD								= 1033,	//	输入银行卡密码


	// 盲人购卡	(CARD)
	AUDIO_CARD_SELECT_PRODUCT_TYPE						= 1017, //  票种选择
	AUDIO_CARD_SELECT_SON_PRODUCT_TYPE_SVT				= 1029, //  选择子产品:SVT
	AUDIO_CARD_SELECT_SON_PEODUCT_TYPE_MST				= 1040, //	选择子产品:MST
	AUDIO_CARD_SELECT_SON_PEODUCT_TYPE_PST				= 1041, //	选择子产品:PST
	AUDIO_CARD_SELECT_BEGIN_SECTION						= 1021, //  选择起始区段
	AUDIO_CARD_SELECT_END_SECTION						= 1022, //  选择目的区段
	//AUDIO_CARD_SELECT_PIECE								= 4104, //  选择张数（预留）
	AUDIO_CARD_CONFIRM_BUSINESS_SVT						= 1030, //  交易确认:SVT
	AUDIO_CARD_CONFIRM_BUSINESS_MST						= 1042,	//  交易确认:MST
	AUDIO_CARD_CONFIRM_BUSINESS_PST						= 1043,	//  交易确认:PST
	
	// 盲人购票
	//AUDIO_TOKEN_SELECT_PRODUCT_TYPE						= 1018, //  票种选择（预留）
	//AUDIO_TOKEN_SELECT_HOT_STATION						= 4201, //  选择热点车站（预留）
	AUDIO_TOKEN_SELECT_END_STATION						= 1026, //  选择目的车站
	//AUDIO_TOKEN_SELECT_PIECE								= 4203, //  选择张数（预留）
	//AUDIO_TOKEN_SELECT_PAYMENT							= 4204, //  选择支付方式（预留）
	AUDIO_TOKEN_CONFIRM_BUSINESS						= 1053, //  交易确认!!!
	AUDIO_TOKEN_INSERT_BANKCARD							= 1005, //  插入银行卡（预留）
	AUDIO_TOKEN_INPUT_PASSWORD							= 1033, //  输入银行卡密码（预留）
	// 盲人充值
	AUDIO_RECHARGE_FIRST_SWIPE_CARD						= 1007, //  第一次刷卡  
	AUDIO_RECHARGE_FIRST_SWIPE_CARD_FOR_AVM				= 1061,	//  AVM第一次刷卡提示

	AUDIO_RECHARGE_SELECE_PRODUCT_TYPE					= 1054, //  选择充值票种!!!
	AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE				= 1055, //  选择充值子产品!!!
	AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST			= 1056,	//	选择充值子产品（PST）
	AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST			= 1057,	//	选择充值子产品（MST）
	//AUDIO_RECHARGE_SELECT_PAYMENT							= 1031, //  选择支付方式
	AUDIO_RECHARGE_CONFIRM_BUSINESS_SVT					= 1046, //  交易确认:SVT
	AUDIO_RECHARGE_CONFIRM_BUSINESS_MST					= 1047, //  交易确认:MST
	AUDIO_RECHARGE_CONFIRM_BUSINESS_PST					= 1048, //  交易确认:PST
	//AUDIO_RECHARGE_INSERT_CASH							= 1004, //  投入现金
	//AUDIO_RECHARGE_CONFIRM_PAYMENT						= 1032, //  支付确认
	//AUDIO_RECHARGE_INSERT_BANKCARD						= 1005, //  插入银行卡
	//AUDIO_RECHARGE_INPUT_PASSWORD							= 1033, //  输入银行卡密码
	AUDIO_RECHARGE_SECOND_SWIPE_CARD					= 1008, //  第二次刷卡
	//AUDIO_RECHARGE_CHARGING								= 1014, //  充值中...
	//AUDIO_RECHARGE_CHARGE_SUCCESS							= 4211, //  充值成功
	//AUDIO_RECHARGE_CHARGE_FAILED							= 1013, //  充值失败
	//AUDIO_RECHARGE_CHARGE_CANCEL							= 1044, //  充值取消
	//AUDIO_RECHARGE_PRINT_FINISHED							= 1045, //  打印完成
	//AUDIO_RECHARGE_BUSINESS_CANCEL						= 1012, //  交易取消
	AUDIO_RECHARGE_FAILED_READ_CARD						= 1009, //  刷卡失败
} AUDIO_ID ;

// 硬币模块回调命令编号
typedef enum COIN_MODULE_CMD_ID{
	COIN_MODULE_COMMAND_ID_READ_COIN_AMOUNT = 0x2000,		   // 读取接受金额命令
	COIN_MODULE_COMMAND_ID_CHANGE			= 0x2001,		   // 找零命令
	COIN_MODULE_COMMAND_ID_ACCEPT			= 0x2002,		   // 收纳硬币命令
	COIN_MODULE_COMMAND_ID_RETURN			= 0x2003		   // 退回硬币命令
};

// 纸币接收模块回调命令编号
typedef enum BANKNOTE_MODULE_CMD_ID{
	BANKNOTE_MODULE_COMMAND_ID_INSERT			= 0x3000,				// 识别纸币命令
	BANKNOTE_MODULE_COMMAND_ID_COLLECT			= 0x3001,				// 收纳纸币（压钱箱）命令
	BANKNOTE_MODULE_COMMAND_ID_RETURN_ALL		= 0x3002,				// 退回缓冲区和暂存区纸币命令
	BANKNOTE_MODULE_COMMAND_ID_RETURN_BUFF		= 0x3003,				// 退回缓冲区纸币命令
	BANKNOTE_MODULE_COMMAND_ID_ACCEPT			= 0x3004					// 压暂存
};

// 纸币找零模块回调命令
typedef enum BANKNOTE_CHANGE_CALLBACK_ID{
	BANKNOTE_CHANGE_COMMAND_ID_SEND_OUT			= 0x6000,				// 纸币送出
};

// 银行卡模块回调命令编号
typedef enum BANKCARD_MODULE_CMD_ID{
	BANKCARD_COMMAND_ID_READ_STATE		= 0x4000,               // 读取状态命令
	BANKCARD_COMMAND_ID_RETURN			= 0x4001             // 退回银行卡命令
};

// PINPAD卡模块回调命令编号
typedef enum PINPAD_CMD_ID{
	PINPAD_COMMAND_ID_READ_READ_STATE = 0x5000             // 读取状态
};

// 乘客插卡事件回调命令编号
typedef enum CHARGE_INSERT_CARD_CMD_ID{
	CHARGE_INSERT_CARD_CMD_ID_STATUS  = 0x7000,				// 读取卡插入状态
};

//// PINPAD键盘类型
//typedef enum _pinpad_type_id{
//	KEY_0		= 0,			// 按键0
//	KEY_1		= 1,			// 按键1
//	KEY_2		= 2,			// 按键2
//	KEY_3		= 3,			// 按键3
//	KEY_4		= 4,			// 按键4
//	KEY_5		= 5,			// 按键5
//	KEY_6		= 6,			// 按键6
//	KEY_7		= 7,			// 按键7
//	KEY_8		= 8,			// 按键8
//	KEY_9		= 9,			// 按键9
//	KEY_CANCEL	= 10,			// 取消键
//	KEY_ENTER	= 11,			// 确认件
//	KEY_CLEAR	= 12,			// 清除键
//	KEY_SPACE	= 13,			// 空白键
//	KEY_LEFT	= 14,			// 左按键
//	KEY_RIGHT	= 15,			// 右按键
//}PINPAD_TYPE;

// 盲人操作状态定义
typedef enum _blind_operator_status_{
	STATUS_BLIND_NORMAL					= 0x01,	// 正常
	STATUS_BLIND_SELECT_LANGUAGE		= 0x02, // 语言选择
	STATUS_BLIND_GEDRADE_MSG			= 0x03, // 降级判断
	STATUS_BLIND_SELECT_BUSINESS		= 0x04, // 选择业务
	STATUS_BLIND_CALL_HELP				= 0x05, // 招援按钮按下
	STATUS_BLIND_CALL_HELP_END			= 0x06, // 招援按钮弹起
	STATUS_BLIND_INSERT_CASH_MAX		= 0x07, // 投入现金超限
	STATUS_BLIND_CHANGE_MAX_COUNT		= 0x08, // 找零数量超限
	STATUS_BLIND_CHANGE_NOT_ENOUGHT		= 0x09, // 找零不足

	STATUS_BLIND_SELECT_PRODUCT			= 0x0A, // 选择产品
	STATUS_BLIND_SELECT_SUB_PRODUCT		= 0x0B, // 选择子产品
	STATUS_BLIND_SELECT_BEGIN_SECTION	= 0x0C, // 选择起始区段
	STATUS_BLIND_SELECT_END_SECTION		= 0x0D, // 选择目的区段
	STATUS_BLIND_SELECT_PRICE			= 0x0E, // 选择张数
	STATUS_BLIND_SELECT_PAYMENT			= 0x0F, // 选择支付方式
	STATUS_BLIND_CONFIRM_BUSINESS		= 0x11, // 确认交易信息
	STATUS_BLIND_INSERT_CASH			= 0x12, // 投入现金
	STATUS_BLIND_CONFIRM_PAYMENT		= 0x13, // 确认支付信息
	STATUS_BLIND_INSERT_BANKCARD		= 0x14, // 插入银行卡
	STATUS_BLIND_INPUT_PASSWORD			= 0x15, // 输入银行卡密码
	STATUS_BLIND_ISSUING				= 0x16, // 交易中...
	STATUS_BLIND_BUSINESS_SUCCESS		= 0x17, // 交易成功
	STATUS_BLIND_BUSINESS_FAILED		= 0x18, // 交易失败
	STATUS_BLIND_BUSINESS_CANCEL		= 0x19, // 交易取消
	STATUS_BLIND_PRINT_SUCCESS			= 0x1A, // 打印成功
	STATUS_BLIND_CANCEL					= 0x1B, // 取消交易
	STATUS_BLIND_FIRST_SWIPE_CARD		= 0x1C, // 第一次刷卡
	STATUS_BLIND_SECOND_SWIPE_CARD		= 0x1D, // 第二次刷卡
	STATUS_BLIND_SELECT_END_STATION		= 0x1E, // 选择目的车站
	STATUS_BLIND_SELECT_HOT_STATION		= 0x1F, // 选择热点车站
	STATUS_BLIND_OPERATION_OUT_OF_TIME	= 0x20,	// 操作超时
	STATUS_BLIND_READ_CARD_FAILED		= 0x21,	// 读卡失败

	// 当前业务状态
	STATUS_BLIND_BUY_TOKEN				= 0x30,	// 购票
	STATUS_BLIND_BUY_CARD				= 0x31,	// 购卡
	STATUS_BLIND_RECHARGE				= 0x32,	// 充值
	STATUS_BLIND_BUY_PRODUCT			= 0x33,	// 购产品

	STATUS_BLIND_ON_RETURN				= 0x34,
}BLIND_OPERATION_STATUS;

/*
	凭条类型定义
*/
typedef enum _receipt_print_type_{
	PRINT_TYPE_NONE,					// 无
	PRINT_TYPE_BALANCE,					// 结算
	PRINT_TYPE_CH_CHANGE_INSTALL,		// 硬币找零箱安装
	PRINT_TYPE_CH_CHANGE_UNINSTALL,		// 硬币找零箱卸下
	PRINT_TYPE_CH_COLLBOX_INSTALL,		// 硬币回收箱安装
	PRINT_TYPE_CH_COLLBOX_UNINSTALL,	// 硬币回收箱卸下
	PRINT_TYPE_CH_COLLECTION,			// 硬币回收
	PRINT_TYPE_CH_SUPPLY,				// 硬币补充
	PRINT_TYPE_BH_INSTALL,				// 纸币回收箱安装
	PRINT_TYPE_BH_UNINSTALL,			// 纸币回收箱卸下
	PRINT_TYPE_BH_CHANGE_INSTALL,		// 纸币找零箱安装
	PRINT_TYPE_BH_CHANGE_UNINSTALL,		// 纸币找零箱卸下
	PRINT_TYPE_TH_INSTALL,				// 票箱安装
	PRINT_TYPE_TH_UNINSTALL,			// 票箱卸下
	PRINT_TYPE_ISSUE_ERROR,				// 售票故障单
	PRINT_TYPE_CHARGE_ERROR,			// 充值（预留）
}RECEIPT_PRINT_TYPE;