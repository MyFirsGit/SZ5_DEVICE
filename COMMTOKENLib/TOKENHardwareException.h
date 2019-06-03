#pragma once
#include "afx.h"
#include "SysException.h"
#include "TOKENLIBExp.h"
/**
@brief	外部读写器硬件异常类
*/

class TOKENCOMLIB_DECLSPEC CTOKENHardwareException :
    public CSysException
{
public:
    static const int MODULE_ID = 12501;     // 模块号

    CTOKENHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
    ~CTOKENHardwareException(void);

	typedef enum _detail_error_code {
		TOKEN_ERR_OK						= 0x00,	// 命令执行正确
		TOKEN_ERR_NO_TOKEN_AT_READ_AREA		= 0x01,	// 无token在读卡位置警告
		TOKEN_ERR_HAS_TOKEN_AT_READ_AREA	= 0x03,	// 有币在读卡位置警告
		TOKEN_ERR_INVALID_PARAMETER			= 0x31,	// 参数
		TOKEN_ERR_BOX_A_EMPTY				= 0x3c,	// A票箱为空
		TOKEN_ERR_BOX_B_EMPTY				= 0x3d,	// B票箱为空
		TOKEN_ERR_JAM_AT_OUTLET				= 0x40,	// 有token在出口位置堵塞
		TOKEN_ERR_BOX_A_CARDOUT				= 0x42,	// 票箱A不出票错
		TOKEN_ERR_BOX_B_CARDOUT				= 0x43,	// 票箱B不出票错
		TOKEN_ERR_DEVICE_BUSY				= 0x4a,	// 设备忙
		TOKEN_ERR_CLEAN_FAILED				= 0x4b,	// 清空失败
		TOKEN_ERR_MOTOR_RUN					= 0x50,	// 电机转动失败
		TOKEN_ERR_READER_SENSOR				= 0x61,	// 天线区传感器故障
		TOKEN_ERR_OUTLET_SENSOR				= 0x62,	// 出币口传感器故障
		TOKEN_ERR_RETRIEVE_SENSOR			= 0x63,	// 回收口传感器故障
		TOKEN_ERR_MAGNET1					= 0x64,	// 电磁铁1故障
		TOKEN_ERR_MAGNET2					= 0x65,	// 电磁铁2故障
		TOKEN_ERR_HOPPER1					= 0x66,	// Hopper1串口故障
		TOKEN_ERR_HOPPER2					= 0x67,	// Hopper2串口故障
		TOKEN_ERR_MAGNET3					= 0x68,	// 电磁铁3故障
		TOKEN_ERR_MAGNET4					= 0x69,	// 电磁铁4故障
		TOKEN_ERR_MAGNET_SENSOR				= 0x6a,	// 电机定位传感器故障
		TOKEN_ERR_HOPPER1_OUT_SENSOR		= 0x6b,	// Hopper1出币传感器故障
		TOKEN_ERR_HOPPER2_OUT_SENSOR		= 0x6c,	// Hopper2出币传感器故障
		TOKEN_ERR_HOPPER1_CLEAN_SENSOR		= 0x6d,	// Hopper1清币传感器故障
		TOKEN_ERR_HOPPER2_CLEAN_SENSOR		= 0x6e,	// Hopper2清币传感器故障

		TOKEN_ERR_RFID_CARD_NOT_EXIST		= 0xA1,	//无法检测到卡箱的RFID卡
		TOKEN_ERR_RFID_CARD_AUTHENT			= 0xA2,	// 卡箱RFID卡认证失败
		TOKEN_ERR_RFID_CARD_OPERATOR_PARAMTER = 0xA3,// 读写卡箱RFID卡参数错误

		TOKEN_ERR_COMM						= 0xFE,	// 串口错误
		TOKEN_ERR_ACK						= 0xFD,	// ACK错误
		TOKEN_ERR_CMD						= 0xFC,	// Cmd错误
	} DETAIL_ERROR_CODE;

private:
};
