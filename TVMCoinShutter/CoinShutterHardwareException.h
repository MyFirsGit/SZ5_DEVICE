#pragma once
#include "afx.h"
#include "SysException.h"
#include "CoinShutterExp.h"
#include "OEDUShutterDevDef.h"

/**
 @brief		硬币接收模块异常类
*/

class CS_API CCoinShutterHardwareException :
	public CSysException
{
public:
	static const int MODULE_ID = 11802;     // 模块号

	typedef enum hardware_err_code {
		CS_ERR_NOT_INIT					= 6500,						// 自定义，未初始化错误

		// 注意：由于硬币闸门模块比较简单，且和硬币模块的联系非常密切，因此硬币闸门不创建helper，直接在硬币模块helper中使用，将硬币闸门的错误码合并到硬币模块中，当作硬币模块错误。

		//CS_ERR_SENSOR					= E_EDUSHUTTER_SENSORERR,	// 6540  信封闸门Sensor坏。
		//CS_ERR_OPEN_FAIL				= E_EDUSHUTTER_OPENFAIL,    // 6541  开门失败，门处在关的状态
		//CS_ERR_OPEN_JAM					= E_EDUSHUTTER_OPENJAM,		// 6542  开门失败，门处在半开启卡死
		//CS_ERR_CLOSE_FAIL				= E_EDUSHUTTER_CLOSEFAIL,   // 6543  关门失败，马达不转，门处在开的状态
		//CS_ERR_CLOSE_JAM				= E_EDUSHUTTER_CLOSEJAM,	// 6544  关门失败，门处在半开卡死
		//CS_ERR_DOOR_JAM					= E_EDUSHUTTER_DOORJAM,		// 6545  关门失败，门被异物顶住（全开）
		//CS_ERR_COM						= E_EDUSHUTTER_COMMERR,		// 6546  COM组件错误
		//CS_ERR_INVALID_DATA				= E_EDUSHUTTER_INVALIDDATA, // 6547  返回无效的数据
		//CS_ERR_ABNORMAL					= E_EDUSHUTTER_ABNORMAL,	// 6548  闸门处于异常状态
		//CS_ERR_TIMEOUT					= E_EDUSHUTTER_TIMEOUT,		// 6599  超时错误

	}HARDWARE_ERR;	

    CCoinShutterHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CCoinShutterHardwareException();

};
