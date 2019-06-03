#pragma once

#define MAX_RSP_LEN							256   // 接收缓存最大长度
#define MAX_SEND_ITEMS_COUNT		10		 // 发送队列最大元素个数
#define MAX_RECEIVE_ITEMS_COUNT	10		 // 接收队列最大元素个数

// 状态对应bit位
//第一字节
#define IO_STS_BACKDOOR						0x01//后门到位
#define IO_STS_FRONTDOOR					0x04//前门到位
#define IO_STS_BANKNOTE_SENSOR		        0x10//纸币模块到位
#define IO_STS_TOKEN_COLBOX_SENSOR			0x20//Token回收箱到位
#define IO_STS_T_U_SENSOR					0x40//T-U Sensor
#define IO_STS_TOKEN_INVBOX_SENSOR			0x80//Token废票箱到位
//第二字节暂时不用
#define IO_STS_DETECT_RIGHT					0x02//
#define IO_STS_DETECT_LEFT					0x01
#define IO_STS_ASSISTENT_BTN				0x04
#define IO_STS_SHOCK_SENSOR_RIGHT           0x08  
#define IO_STS_SHOCK_SENSOR_LEFT		    0x00 //暂无
//第三字节
#define IO_STS_B_U_SENSOR					0x02//B-USensor
#define IO_STS_PERSON_NEAR_SENSOR			0x04//人体接近感应

// IO命令代码定义
typedef enum _io_command_code {
	IO_COMMAND_CODE_INIT									= 0x4C,   // L	初始化变量及I/O口
	IO_COMMAND_CODE_GET_STATUS						= 0x49,   // I		取输入口、输出口状态
	IO_COMMAND_CODE_SET_OUTPUT_ON_OFF		= 0x4F,   // O	设置单路输出状态（亮或灭）
	IO_COMMAND_CODE_SET_OUTPUT_FLASH			= 0x52,   // R	设置单路输出状态（闪烁）
	IO_COMMAND_CODE_SET_12OUTPUT					= 0x50,   // P	设置12路输出状态（亮、灭或闪烁）
	IO_COMMAND_CODE_GET_VERSION					= 0x56,   // V	取介质程序版本号
	IO_COMMAND_CODE_IO_BOARD_TYPE				= 0x54    // T	取IO板型号
} IO_COMMAND_CODE;

// IO命令数据体长度定义
typedef enum _io_command_len {
	IO_COMMAND_LEN_INIT								= 0x01,   // L   初始化变量及I/O口命令长度
	IO_COMMAND_LEN_GET_STATUS					= 0x01,   // I	取输入口、输出口状态命令长度
	IO_COMMAND_LEN_SET_OUTPUT_ON_OFF	= 0x03,   // O	设置单路输出状态（亮或灭）命令长度
	IO_COMMAND_LEN_SET_OUTPUT_FLASH		= 0x04,   // R	设置单路输出状态（闪烁）命令长度
	IO_COMMAND_LEN_SET_12OUTPUT				= 0x03,   // P	设置12路输出状态（亮、灭或闪烁）命令长度
	IO_COMMAND_LEN_GET_VERSION					= 0x01,   // V	取介质程序版本号命令长度
	IO_COMMAND_LEN_IO_BOARD_TYPE				= 0x01    // T	取IO板型号命令长度
} IO_COMMAND_LEN;

// IO输出端口号定义
typedef enum _io_output_port_no {
	IO_OUTPUT_PORT_NO_CH_INDICATOR										= 0x31,	  // 投币口指示灯
	IO_OUTPUT_PORT_NO_RETURN_CUP_INDICATOR								= 0x32,   // 出票口指示灯
	IO_OUTPUT_PORT_NO_RETURN_CUP_LIGHT									= 0x33,   // 出票口照明灯
	IO_OUTPUT_PORT_NO_CARDFFLE_INDICATOR								= 0x35,	  // 储值卡指示灯
	IO_OUTPUT_PORT_NO_BAFFLE_B											= 0x39,	  // B电磁铁
	IO_OUTPUT_PORT_NO_OPERATION_LIGHT									= 0x41,	  // 操作照明灯
	IO_OUTPUT_PORT_NO_BAFFLE_T											= 0x42,	  // T电磁铁
	IO_OUTPUT_PORT_NO_ALARM												= 0x43,   // 报警器
	IO_OUTPUT_PORT_NO_HELP_BTN_LIGHT									= 0x00,	  // 招援按钮灯(停用)
} IO_OUTPUT_PORT_NO;

// IO输出信号定义
typedef enum _io_output_value {
	IO_OUTPUT_ON								= 0x31,   // 输出开（1）
	IO_OUTPUT_OFF								= 0x30,   // 输出关（0）
	IO_OUTPUT_LIGHT_SLOW						= 0x73,	  // 慢速闪烁
	IO_OUTPUT_LIGHT_MID							= 0x6D,	  // 中速闪烁
	IO_OUTPUT_LIGHT_FAST						= 0x66,	  // 快速闪烁
	IO_OUTPUT_LIGHT_STOP						= 0x4F,	  // 单路停止闪烁
} IO_OUTPUT_VALUE;

// IO状态码定义
typedef enum _io_state_value {
	IO_STATE_SUCCEED								= 0x73,    // 成功
	IO_STATE_WARNING								= 0x77,    // 警告
	IO_STATE_ERROR									= 0x65    // 错误
} IO_STATE_VALUE;

