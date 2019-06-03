#ifndef _DIO_API_H
#define _DIO_API_H

#include "Apidef.h"

//错误类型
#define iTYPE_2    2
//错误代码
#define iERROR_CODE_DIO_0						0x0000	//OK
#define iERROR_CODE_DIO_OPEN_FAIL				0x2710	// 10000 打开DIO模块错误！
#define iERROR_CODE_DIO_INIT_FAIL				0x2711	// 10001 初始化DIO模块错误！
#define iERROR_CODE_DIO_RESET_FAIL				0x2712	// 10002 复位DIO模块错误！
#define iERROR_CODE_DIO_PARAM_LAMP				0x2713	// 10003 提示灯信号编号输入错误！
#define iERROR_CODE_DIO_CONFIG_LAMP				0x2714	// 10004 设置灯信号发生错误！
#define iERROR_CODE_DIO_PARAM_SIGNAL			0x2715	// 10005 开关信号编号输入错误！
#define iERROR_CODE_DIO_CONFIG_SIGNAL			0x2716	// 10006 设置开关信号发生错误！
#define iERROR_CODE_DIO_GET_SIGNAL				0x2AF8	// 10007 获取开关信号发生错误！

#pragma pack(push, 1)

typedef enum _dio_lamp_color{
	IO_LIGHT_RED	= 0x01,
	IO_LIGHT_GREEN	= 0x02,
	IO_LIGHT_BLUE	= 0x03,
	IO_LIGHT_YELLOW = 0x04,
	IO_LIGHT_WHITE	= 0x05,
	IO_LIGHT_UNKNOWN = 0xFF,
}DIO_LAMP_COLOR;

typedef enum _dio_oper_type{
	IO_CLOSE = 0x00,
	IO_OPEN	= 0x01,

}DIO_OPER_TYPE;


typedef enum _dio_signal_type{
	IO_SIGNAL_LOW						= 0x0001,
	IO_SIGNAL_EMERGENCY					= 0x0002,
	IO_SIGNAL_TPUA_RESET				= 0x0003,
	IO_SIGNAL_TPUB_RESET				= 0x0004,
	IO_SIGNAL_RECOVERY_RESET			= 0x0005,
	IO_SIGNAL_FLAP_RESET				= 0x0006,
	IO_SIGNAL_TH_RESET					= 0x0007,
	IO_SIGNAL_BH_RECEIVING_RESET		= 0x0008,
	IO_SIGNAL_BH_CHANGE_RESET			= 0x0009,
	IO_SIGNAL_CH_RESET					= 0x000A,
	IO_SIGNAL_MAINTENANCE_DOOR1			= 0x000B,
	IO_SIGNAL_MAINTENANCE_DOOR2			= 0x000C,
	IO_SIGNAL_MAINTENANCE_DOOR3			= 0x000D,
	IO_SIGNAL_MAINTENANCE_DOOR4			= 0x000E,
	IO_SIGNAL_MAINTENANCE_DOOR5			= 0x000F,
	IO_SIGNAL_MAINTENANCE_DOOR6			= 0x0010,
	IO_SIGNAL_MAINTENANCE_DOOR7			= 0x0011,
	IO_SIGNAL_MAINTENANCE_DOOR8			= 0x0012,
	IO_SIGNAL_BUZZER					= 0x0013,
	IO_SIGNAL_HUMAN_SENSOR1				= 0x3001,
	IO_SIGNAL_HUMAN_SENSOR2				= 0x3002,
	IO_SIGNAL_HUMAN_SENSOR3				= 0x3003,
	IO_SIGNAL_RECHARGE_OPEN				= 0x3004,
	IO_SIGNAL_RECHARGE_UNLOCK			= 0x3005,
	IO_SIGNAL_RECHARGE_INPLACE			= 0x3006,
	IO_SIGNAL_COIN_SLOT					= 0x3007,
	IO_SIGNAL_FAN_SWITCH				= 0x3008,
	IO_SIGNAL_HELPBUTTON				= 0x3009,
	IO_SIGNAL_SAFETY_DOOR_SWITCH		= 0x3010,
	IO_SIGNAL_SAFETY_DOOR_INPLACE		= 0x3011,
	IO_SIGNAL_BH_RECEIVING_SECURITY_DOOR = 0x3012,
	IO_SIGNAL_BH_CHANGE_SAFETY_DOOR		= 0x3013,
	IO_SIGNAL_CH_SAFETY_DOOR			= 0x3014,
	IO_SIGNAL_BH_RECEIVING_INPLACE		= 0x3015,
	IO_SIGNAL_BH_CHANGE_INPLACE			= 0x3016,
	IO_SIGNAL_CH_INPLACE				= 0x3017,
	IO_SIGNAL_TH_INPLACE				= 0x3018,
	IO_SIGNAL_PASSENGER_PRINT_INLPACE	= 0x3019,
	IO_SIGNAL_COIN_OPERATIING_LIGHT		= 0x3020,
	IO_SIGNAL_BH_RECEIVING_LIGHT		= 0x3021,
	IO_SIGNAL_RECHARGE_LIGHT			= 0x3022,
	IO_SIGNAL_PRINT_LIGHT				= 0x3023,
	IO_SIGNAL_PICKUPTICKET_INDICATOR	= 0x3024,
	IO_SIGNAL_BANKCARD_LIGHT			= 0x3025,
	IO_SIGNAL_COIN_COLECT_BOX_READY		= 0x3026,
	IO_SIGNAL_PICKUPTICKET_LITGHT		= 0x3027,
}IO_SIGNAL_TYPE;

// DIO模块状态结构
struct STDioDevReturn {
	unsigned int uiProviderId;	// 模块厂家标识
	int iType;					// 错误类型：0-正常；1-警告；2-故障
	int iStdErrorCode;			//标准错误代码
	int iErrorCode;				// 错误代码
	char chErrorCode[128];   // 硬件返回信息
	char acReverse[128];   //预留信息
};
// 提示灯信号配置结构体
struct STDioLampConfig {
	unsigned short usLampType;	// 需要设置的提示灯类型
	unsigned char ucLampColor;	// 提示灯颜色:
								// 1，红灯(警告/错误灯)
								// 2，绿灯
								// 3，蓝灯
								// 4，黄灯
								// 5，白灯
	unsigned char ucOperType;	// 操作类型：0x00，关闭灯；0x01，打开灯
	unsigned char ucBlinkTimes; // 闪烁次数（操作类型为0x01时有效）：
								// 0x00，不闪烁；
								// ≥ 0x00，闪烁次数
} ;
// 开关信号配置结构体
struct STDioSignalConfig {
	unsigned short usSignalType;  // 开关信号类型
	unsigned char ucOperType;     // 操作类型：
								  // 0x00，关闭信号
								  // 0x01，打开信号
} ;
// 版本信息结构体
struct STDioVerInfo {
	unsigned char pFirmwareVerInfo[ 32 ];  //固件版本信息, 32 字节，不够后补0x00
	unsigned char pDriverVerInfo[ 32 ];    //驱动版本信息, 32 字节，不够后补0x00
	unsigned char pHardwareVerInfo[ 32 ];  //硬件版本信息, 32 字节，不够后补0x00
} ;

// 维修门DIO信息(0:维修门开; 1:维修门关)
struct STDioMaintainDoor {
	// 维修门数量
	unsigned char ucDoorNumber;
	// 维修门DIO信息
	unsigned char ucDoorSignal[ 16 ];
};

// DIO模块串口配置结构体
struct tSerialConfigInfo {
	int chSerialPort;	// 串口号
	int nIoSpeed;		// 波特率
	int nStopbits;		// 停止位
	int nDatabits;		// 数据位
	int nParity;		// 校验位
};
typedef enum _tem_io_rsp_code {
	IO_RSP_OK = 0x0000,    // 操作成功
	IO_RSP_NG = 0x0001,    // 操作失败
}IO_RSP_CODE;
// 取IO状态返回结果定义
typedef struct _io_rsp_status_info
{
	bool isFrontDoorOpen;									// 前维护门是否打开
	bool isBackDoorOpen;									// 后维护门是否到位
	bool isBanknoteModeReady;								// 纸币模块是否到位
	bool isCoinModeReady;									// 硬币模块是否到位
	bool isTokenColBoxReady;								// Token回收箱到位
	bool isTokenInvBoxReady;								// Token废票箱到位
	bool isLeftPersonActive;								// 左侧是否有人
	bool isRightPersonActive;								// 右侧是否有人
	bool isLeftShockActive;									// 左振动传感器是否触发
	bool isRightShockActive;								// 右振动传感器是否触发
	bool isAssittentButtonPressed;							// 招援按钮是否按下
	bool isCardModeReady;									// 票卡模块是否到位
	bool isCoinboxReady;									// 硬币回收箱是否到位
	bool isPrintModeReady;									// 乘客打印机是否到位
	bool isRechargeCardReady;								// 充值卡到位
	_io_rsp_status_info()
	{
		isFrontDoorOpen = false;							// 前维护门是否打开
		isBackDoorOpen = true;
		isBanknoteModeReady= true;							// 纸币模块是否到位
		isCoinModeReady = true;
		isTokenColBoxReady = false;
		isTokenInvBoxReady = false;
		isLeftPersonActive= false;							// 左侧是否有人
		isRightPersonActive= false;							// 右侧是否有人
		isLeftShockActive = false;							// 左振动传感器是否触发
		isRightShockActive= false;							// 右振动传感器是否触发
		isAssittentButtonPressed= false;					// 招援按钮是否按下
		isCardModeReady= true;								// 票卡模块是否到位
		isCoinboxReady = true;
		isPrintModeReady = true;
		isRechargeCardReady = true;
	}
} IO_RSP_STATUS_INFO, *LPIO_RSP_IO_INFO;
#pragma pack(pop)

extern "C" {
	// >>>> ======= 标准接口定义 =======
	/**
	* DIO模块打开
	*  uiCommPort - 串口号(非串口的DIO模块无意义)
	*  uiBaudRate - 串口通讯的波特率(非串口的DIO模块无意义)
	* 返回值:
	*  0 - 执行成功; 其他 - 执行失败;
	*/
	DLLIMP int DIO_Open( int nNum, tSerialConfigInfo* pSerialConfigInfo );
	/**
	 * 初始化模块，检查模块是否可用
	 *  tReturn - 返回的模块状态信息
	 * 返回值:
	 *  0 - 执行成功; 其他 - 执行失败;
	 */
	DLLIMP int DIO_Init( STDioDevReturn& tReturn );
	/**
	 * DIO模块关闭
	 * 返回值:
	 *  0 - 执行成功; 其他 - 执行失败;
	 */
	DLLIMP int DIO_Close();

	/**
	 * 设置提示灯信号
	 *  pLampConfig - 需要设置的灯信息
	 *  ucLampCnt - 需要设置提示灯的数量（tDioLampConfig的个数）
	 *  tReturn - 返回的模块状态信息
	 * 返回值:
	 *  0 - 执行成功; 其他 - 执行失败;
	 */
	DLLIMP int DIO_SetLampSignal( STDioLampConfig* pLampConfig, unsigned char ucLampCnt, STDioDevReturn& tReturn );
	/**
	 * 设置提示灯信号
	 *  pSignalConfig - 需要设置的开关信号
	 *  ucSignalCnt - 需要设置的开关信号的数量（tDioSignalConfig的个数）
	 *  tReturn - 返回的模块状态信息
	 * 返回值:
	 *  0 - 执行成功; 其他 - 执行失败;
	 */
	DLLIMP int DIO_SetSwitchSignal( STDioSignalConfig* pSignalConfig, unsigned char ucSignalCnt, STDioDevReturn& tReturn );
	/**
	 * 设置提示灯信号
	 *  pSignalConfig - 需要返回的开关信号配置结构（需要调用者开辟返回数据内存空间）
	 *  ucSignalCnt - 返回开关信号的数量（返回tDioSignalConfig的个数，个数不超过传入的数量；当
						传入值为0时，返回所有的，调用方应保障调用前为pSignalConfig申请足够的空间）
	 *  tReturn - 返回的模块状态信息
	 * 返回值:
	 *  0 - 执行成功; 其他 - 执行失败;
	 */
	DLLIMP int DIO_GetSwitchSignal( STDioSignalConfig* pSignalConfig, unsigned char& ucSignalCnt, STDioDevReturn& tReturn );
	/**
	 * 获取模块固件版本
	 *  tVersion - 模块固件版本信息
	 *  tReturn - 返回的模块状态信息
	 * 返回值:
	 *  0 - 执行成功; 其他 - 执行失败;
	 */
	DLLIMP int DIO_GetVersion( STDioVerInfo& tVersion, STDioDevReturn& tReturn );
	// <<<<
}



#endif
