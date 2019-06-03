#pragma once
#include "afx.h"
#include "SysException.h"
#include "CHLibExp.h"
#include "CAS002DevDef.h"
/**
@brief	CH硬件异常类
*/
class DLLIMP CCHHardwareException : public CSysException
{

public:
	static const int MODULE_ID = 11801;     // 模块号
	typedef enum hardware_err_code {
		// 自定义硬件错误
		ERROR_CH_COLLECT_BOX_A_NOT_OPEN			= 57,	// 硬币回收箱不到位
		//ERROR_CH_COLLECT_BOX_B_NOT_OPEN			= 58,	// 硬币回收箱B盖板未打开
		ERROR_CH_MODULE_NOT_STANDBY				= 200,	// 硬币模块没有到位
		ERROR_CH_CHANGE_BOX_A_NOT_INSTALL		= 201,	// 找零箱A没有安装
		ERROR_CH_CHANGE_BOX_B_NOT_INSTALL		= 202,	// 找零箱B没有安装
		ERROR_CH_CHANGE_CHANGE_FAILED			= 203,	// 找零失败
		ERROR_CH_BEGIN_INSERT_COMMOND_ERR		= 300,	// 开始接收硬币命令错误（硬币接收列表为空）

		// 警告性错误
		CH_ERR_BASE						= S_CAS_BASE,					// 错误码基值
		CH_ERR_WARNNING_BASE			= W_CAS_BASE,					// 17005 警告码基值
		CH_ERR_HOPPER1_COUNT			= W_CAS_MHOPPER1_COUNT_ERROR,	// 17006 Hopper1清币数量不符						(清币失败)
		CH_ERR_HOPPER2_COUNT			= W_CAS_MHOPPER2_COUNT_ERROR,	// 17007 Hopper2清币数量不符						(清币失败)
		CH_ERR_HOPPER_COUNT				= W_CAS_MHOPPER_COUNT_ERROR,	// 17008 两Hopper清币数量不符						(清币失败)

		// 严重错误
		CH_ERR_ERROR_BASE				= E_CAS_BASE,					// 严重错阈值
		CH_ERR_COMMAND					= E_CAS_COMMAND_ERROR,			// 17011 无效命令
		CH_ERR_NO_CHANGE				= E_CAS_NO_CHANGE,				// 17012 金额不足，找零失败							(建议屏蔽找零功能)
		CH_ERR_ESCROW1_MIDDLE			= E_CAS_ESCROW1_MIDDLE,			// 17013 暂存器1找不到中间位置						(建议停机维护，影响找零和收币功能)
		CH_ERR_ESCROW1_LEFT				= E_CAS_ESCROW1_LEFT,			// 17014 暂存器1找不到左边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ESCROW1_RIGHT			= E_CAS_ESCROW1_RIGHT,			// 17015 暂存器1找不到右边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ESCROW2_MIDDLE			= E_CAS_ESCROW2_MIDDLE,			// 17016 暂存器2找不到中间位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ESCROW2_LEFT				= E_CAS_ESCROW2_LEFT,			// 17017 暂存器2找不到左边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ESCROW2_RIGHT			= E_CAS_ESCROW2_RIGHT,			// 17018 暂存器2找不到右边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ROUTER1_LEFT				= E_CAS_ROUTER1_LEFT,			// 17019 换向器1找不到左边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ROUTER1_RIGHT			= E_CAS_ROUTER1_RIGHT,			// 17020 换向器1找不到右边位置  					(建议停机维护，影响找零和收币功能)
		CH_ERR_ROUTER2_LEFT				= E_CAS_ROUTER2_LEFT,			// 17021 换向器2找不到左边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ROUTER2_RIGHT			= E_CAS_ROUTER2_RIGHT,			// 17022 换向器2找不到右边位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ROUTER1_MIDDLE			= E_CAS_ESCROW1_MIDDLE,			// 17023 换向器1找不到中间位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_ROUTER2_MIDDLE			= E_CAS_ESCROW2_MIDDLE,			// 17024 换向器2找不到中间位置   					(建议停机维护，影响找零和收币功能)
		CH_ERR_HOPPER1_DISP				= E_CAS_MHOPPER1_DISP,			// 17025 A主找零器出币失败       					(建议停机维护，建议屏蔽找零功能)
		CH_ERR_HOPPER1_JAM				= E_CAS_MHOPPER1_JAM,			// 17026 A主找零器卡币           					(建议停机维护，建议屏蔽找零功能)
		CH_ERR_HOPPER2_DISP				= E_CAS_MHOPPER2_DISP,			// 17027 B主找零器出币失败       					(建议停机维护，建议屏蔽找零功能)
		CH_ERR_HOPPER2_JAM				= E_CAS_MHOPPER2_JAM,			// 17028 B主找零器卡币           					(建议停机维护，建议屏蔽找零功能)
		CH_ERR_ACCEPT_FAULT				= E_CAS_COINACCEPTOR_FAULT,		// 17033 硬币接收器故障          					(建议停机维护，建议屏蔽收币功能)

		CH_ERR_IBUTTON1					= E_CAS_IBUTTON1,				// 17034 加币箱1iButton故障/RFID故障  				(建议停机维护)
		CH_ERR_IBUTTON2					= E_CAS_IBUTTON2,				// 17035 加币箱2iButton故障/RFID故障  				(建议停机维护)
		CH_ERR_WRITE_FLASH				= E_CAS_WRITE_FLASH,			// 17036 编程FLASH失败           					(建议停机维护)
		CH_ERR_FILE_NOTFINISH			= E_CAS_FILE_NOTFINISH,			// 17037 文件未下载完            					(建议停机维护)
		CH_ERR_WRITE_LOG				= E_CAS_WRITE_LOG,				// 17038 写Log失败               					(预留)
		CH_ERR_READ_LOG					= E_CAS_READ_LOG,				// 17039 读Log失败               					(预留)
		CH_ERR_CLEAN_FAIL				= E_CAS_CLRCOIN_FAIL,			// 17040 清币失败                					(建议停机维护，不影响找零和接受硬币功能)
		CH_ERR_CAM_JAM					= E_CAS_CAM_JAM,				// 17041 凸轮堵转                					(建议停机维护)
		CH_ERR_CAM_MOTOR				= E_CAS_CAM_MOTOR,				// 17042 凸轮马达故障            					(建议停机维护)
		CH_ERR_COININ_JAM				= E_CAS_COININ_JAM,				// 17043 投币口卡币              					(建议停机维护，建议屏蔽收币功能)
		CH_ERR_EJECT_SIGNAL				= E_CAS_EJECT_SIGNAL,			// 17044 硬币接收器退币信号异常  					(建议停机维护，建议屏蔽收币功能)
		CH_ERR_ESCROW1_SENSOR			= E_CAS_ESCROW1_SENSOR,			// 17045 暂存器1传感器故障       					(建议停机维护)
		CH_ERR_SCROW2_SENSOR			= E_CAS_ESCROW2_SENSOR,			// 17046 暂存器2传感器故障       					(建议停机维护)
		CH_ERR_ROUTER1_SENSOR			= E_CAS_ROUTER1_SENSOR,			// 17047 换向器1传感器故障       					(建议停机维护)
		CH_ERR_ROUTER2_SENSOR			= E_CAS_ROUTER2_SENSOR,			// 17048 换向器2传感器故障       					(建议停机维护)
		CH_ERR_EEPROM					= E_CAS_EEPROM,					// 17049 EEPROM故障              					(建议停机维护)
		CH_ERR_COINACCEPTOR_TIMEOUT		= E_CAS_COINACCEPTOR_TIMEOUT,	// 17050 硬币接受器超时          					(建议停机维护，建议屏蔽收币功能)
		CH_ERR_COINACCEPTOR_DATA		= E_CAS_COINACCEPTOR_DATA,		// 17051 硬币接受器返回数据错    					(建议停机维护，建议屏蔽收币功能)
		CH_ERR_DEV_BUSY					= E_CAS_DEV_BUSY,				// 17052 设备忙                  
		CH_ERR_IBUTTON_ERROR			= E_CAS_IBUTTON_ERROR,			// 17053 钱箱iButton故障/RFID故障					(建议停机维护)
		CH_ERR_COINOUT_JAM				= E_CAS_COINOUT_JAM,			// 17054 投币出口卡币            					(建议停机维护，建议屏蔽收币功能)
		CH_ERR_MODULE_ERROR				= E_CAS_MODULE_ERROR,			// 17055 模块位置异常          						(建议停机维护)
		CH_ERR_HOPPER1_POS				= E_CAS_HOPPER1_POS,			// 17056 HOPPER1不到位         						(建议停机维护)
		CH_ERR_HOPPER2_POS				= E_CAS_HOPPER2_POS,			// 17057 HOPPER2不到位           					(建议停机维护)
		CH_ERR_COINBOX_POS				= E_CAS_COINBOX_POS,			// 17058 钱箱不到位              					(建议停机维护)
		CH_ERR_MODULE_POS				= E_CAS_MODULE_POS,				// 17059 模块不到位              					(建议停机维护)
		CH_ERR_IMAGE_ERROR				= E_CAS_IMAGE_ERROR,			// 17060 图像识别模块故障        					(建议停机维护)

		CH_ERR_RFID_REQUEST				= E_CAS_RFID_REQUEST,			// 17061 感应区无有效卡或/卡模块未连接				(根据业务需求决定故障等级)
		CH_ERR_RFID_AUTHEN				= E_CAS_RFID_AUTHEN,			// 17062 感应区卡认证失败							(根据业务需求决定故障等级)
		CH_ERR_RFID_PARAMETER			= E_CAS_RFID_PARAMETER,			// 17063 读写RFID时参数错							(根据业务需求决定故障等级)
		CH_ERR_READ_FILE				= E_CAS_READ_FILE,				// 17066 读文件失败									(建议停机维护)
		CH_ERR_FILE_LENGTH				= E_CAS_FILE_LENGTH,			// 17067 文件太大									(建议停机维护)
		CH_ERR_OPEN_FILE				= E_CAS_OPEN_FILE,				// 17068 文件打开失败								(建议停机维护)
		CH_ERR_UPDATEDATA_EMPTY			= E_CAS_UPDATEDATA_EMPTY,		// 17069 无法执行更新，无更新数据
		CH_ERR_COINBOXFULL				= E_CAS_COINBOXFULL,			// 17073 硬币回收钱箱满
		CH_ERR_VOLTAGE_LACK				= E_CAS_VOLTAGE_LACK,			// 17074 电压不足 
		CH_ERR_HOPPER1_INTERVENE		= E_CAS_MHOPPER1_INTERVENE,		// 17075 主找零1出币过程中传感器被干扰(当前交易出币计数不准)
		CH_ERR_HOPPER1_LACK				= E_CAS_MHOPPER1_LACK,			// 17076 主找零1出币过程中可能少出币 (当前交易出币计数不准) 
		CH_ERR_HOPPER2_INTERVENE		= E_CAS_MHOPPER2_INTERVENE,		// 17077 主找零2出币过程中传感器被干扰 (当前交易出币计数不准)
		CH_ERR_HOPPER2_LACK				= E_CAS_MHOPPER2_LACK,			// 17078 主找零2出币过程中可能少出币 (当前交易出币计数不准)

		CH_ERR_UNSUPPORT				= E_CAS_UNSUPPORT,				// 17079 设备不支持的功能							(某些接口仅针对特定型号，对于不支持的型号会返回此错误)
		CH_ERR_RTRBOX2_RFID				= E_CAS_RTRBOX2_RFID,			// 17081 回收箱2 iButton/RFID 故障
		CH_ERR_GATE_OPEN				= E_CAS_GATE_OPEN,				// 17082 闸门打开失败
		CH_ERR_GATE_CLOSE				= E_CAS_GATE_CLOSE,				// 17083 闸门关闭失败
		CH_ERR_UNKNOWN					= E_CAS_UNKNOWN,				// 17090 未知错
		CH_ERR_PARAM_ERROR				= E_CAS_PARAM_ERROR,			// 17091 参数不正确 
		CH_ERR_DEV_CFG					= E_CAS_DEV_CFG,				// 17092 读写设备配置错                
		CH_ERR_COMM_CFG					= E_CAS_COMM_CFG,				// 17093 串口配置错 
		CH_ERR_RECEIVE_NAK				= E_CAS_RECEIVE_NAK,			// 17094 收到NAK,发送数据有错    
		CH_ERR_REVDATA_ERROR			= E_CAS_REVDATA_ERROR,			// 17095 接收到的数据有误        
		CH_ERR_SENDENQ_ERROR			= E_CAS_SENDENQ_ERROR,			// 17096 发送ENQ出错             
		CH_ERR_SENDCMD_ERROR			= E_CAS_SENDCMD_ERROR,			// 17097 发送命令错              
		CH_ERR_WAITACK_TIMEOUT			= E_CAS_WAITACK_TIMEOUT,		// 17098 等待ACK超时             
		CH_ERR_WAITRSP_TIMEOUT			= E_CAS_WAITRSP_TIMEOUT,		// 17099 等待响应超时    


		// 注意：
		// 由于硬币闸门模块比较简单，且和硬币模块的联系非常密切，因此硬币闸门不创建helper，直接在硬币模块helper中使用，将硬币闸门的错误码合并到硬币模块中，当作硬币模块错误。
		// 以下是硬币闸门模块的错误码。
		CS_ERR_NOT_INIT					= 6500,		// 自定义，未初始化错误
		CS_ERR_SENSOR					= 6540,		// 6540  信封闸门Sensor坏。
		CS_ERR_OPEN_FAIL				= 6541,		// 6541  开门失败，门处在关的状态
		CS_ERR_OPEN_JAM					= 6542,		// 6542  开门失败，门处在半开启卡死
		CS_ERR_CLOSE_FAIL				= 6543,		// 6543  关门失败，马达不转，门处在开的状态
		CS_ERR_CLOSE_JAM				= 6544,		// 6544  关门失败，门处在半开卡死
		CS_ERR_DOOR_JAM					= 6545,		// 6545  关门失败，门被异物顶住（全开）
		CS_ERR_COM						= 6546,		// 6546  COM组件错误
		CS_ERR_INVALID_DATA				= 6547,		// 6547  返回无效的数据
		CS_ERR_ABNORMAL					= 6548,		// 6548  闸门处于异常状态
		CS_ERR_TIMEOUT					= 6599,		// 6599  超时错误

	}HARDWARE_ERR;	//错误详细，参考文档

	CCHHardwareException(long lDetailID, CString sFileName, long lLineNo, CString sMessage = _T(""));
	~CCHHardwareException(void);

private:
};