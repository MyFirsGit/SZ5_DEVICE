#pragma once

#include <vector>
#include "Util.h"
#include "CommonDefine.h"

using namespace std;

#define MAX_SEND_ITEMS_COUNT		1		 // 发送队列最大元素个数
#define MAX_RECEIVE_ITEMS_COUNT	    1		 // 接收队列最大元素个数

namespace BHCHANGE
{
#define RES_MAX 256
}

typedef struct _box_limmit_info {
	//只设置以下三个参数
	BYTE num_out;   //出款张数，最大20
	BYTE reject;  //当前盒子拒绝最大张数，其值最大20
	BYTE pick_retries;  //重试张数，最大15
private:
	BYTE InfoConverted[5];  //上述由可读信息转换而成的发送信息
	BYTE map_readableTOsend(BYTE num)  //可读信息(1-9)到发送信息的映射关系
	{
		switch(num)
		{
		case 0: return 0x30;
		case 1: return 0xB1;
		case 2: return 0xB2;
		case 3: return 0x33;
		case 4: return 0xB4;
		case 5: return 0x35;
		case 6: return 0x36;
		case 7: return 0xB7;
		case 8: return 0xB8;
		case 9: return 0x39;
		default:
			return 0x00;
		}
	}
public:
	BYTE* toconvert()   //可读信息转换成发送信息
	{
		InfoConverted[0] = map_readableTOsend(num_out/10);
		InfoConverted[1] = map_readableTOsend(num_out%10);
		InfoConverted[2] = map_readableTOsend(reject/10);
		InfoConverted[3] = map_readableTOsend(reject%10);
		InfoConverted[4] = pick_retries;
		return InfoConverted;
	}
}BoxLimInfo,*PBoxLimInfo;

typedef struct _param_countbill {
	BoxLimInfo box[8];   //1-8号盒子,1:box[0]......8:box[8]
}ParamCountBill,*PParamCountBill;


typedef struct _box_lim_bh_lenAndthickness
{
	BYTE max_Len;		//纸币最大长度 单位：mm
	BYTE min_Len;		//纸币最小长度 单位：mm
	BYTE thickness;		//纸币厚度  单位：0.01mm
	_box_lim_bh_lenAndthickness(){
		max_Len = 0;
		min_Len = 0;
		thickness = 0;
	}
}BoxLimBHInfo,*PBoxLimBHInfo;

typedef struct _param_DevInit {
	BoxLimBHInfo box[8];   //1-8号盒子,1:box[0]......8:box[8]
}ParamDevInit,*PParamDevInit;


typedef struct _commonfeedback
{
	BYTE ErrCode[2];  //错误代码
	BYTE Version[4];   //版本信息
	BYTE ErrRegister[3];  //错误寄存器
	BYTE SensorRegister[6]; //传感器寄存器
	BYTE boxRegister[4];   //盒子寄存器
	ParamDevInit obj;    //1-8号盒子中纸币长度和厚度信息

}CommonFeedback,*PCommonFeedback;

// 解析各位数据代表意义
typedef enum _comm_byte_means_diff_{
	BYTE_IN_POS_00	= 0x01,
	BYTE_IN_POS_01	= 0x02,
	BYTE_IN_POS_02	= 0x04,
	BYTE_IN_POS_03	= 0x08,
	BYTE_IN_POS_04	= 0x10,
	BYTE_IN_POS_05	= 0x20,
	BYTE_IN_POS_06	= 0x40,
	BYTE_IN_POS_07	= 0x80,
};

// 传感器错误状态
typedef struct _comm_err_sensor_status_{
	// Error Classification register
	bool isCallingParameterError;
	bool isMediumPositionAbnormal;
	bool isNoMediumAtPosition;		// 移动介质不在位
	bool isInconsistencyDetected;	// INCND
	bool isHardwareTrouble;			// 硬件故障
	bool isCountNotComplete;		// 出币未完成
	bool isJam;						// 卡钞
	bool isDeviceNotReady;			// 纸币找零模块未准备好

	// Error detailed register
	bool isPoolSectionTrouble;		
	bool isMainHardwareTrouble;		
	bool isSensorAlarmOrMediumRemained;
	bool isShutterTrouble;			// 出口异常
	bool isNumberOfCountedBillUnmatched;
	bool isRejectBoxOverflow;		// 拒钞箱满
	bool isCashBoxNotReady;			// 找零箱未准备好

	// Error count error detailed register
	bool isBillLengthLong;
	bool isBillLenthShort;
	bool isBillTicknessError;
	bool isBillToBillSpaceTooShort;			// 
	bool isDrawingBillsFromWrongCassette;	// 出币错误

	_comm_err_sensor_status_(){
		isCallingParameterError			= false;
		isMediumPositionAbnormal		= false;
		isNoMediumAtPosition			= false;
		isInconsistencyDetected			= false;
		isHardwareTrouble				= false;
		isCountNotComplete				= false;
		isJam							= false;
		isDeviceNotReady				= false;

		
		isPoolSectionTrouble			= false;
		isMainHardwareTrouble			= false;
		isSensorAlarmOrMediumRemained	= false;
		isShutterTrouble				= false;
		isNumberOfCountedBillUnmatched	= false;
		isRejectBoxOverflow				= false;
		isCashBoxNotReady				= false;

	
		isBillLengthLong				= false;
		isBillLenthShort				= false;
		isBillTicknessError				= false;
		isBillToBillSpaceTooShort		= false;
		isDrawingBillsFromWrongCassette	= false;
	}

	// 将错误信息位转换为各位代表
	void Deserialize(LPBYTE lpByte){
		LPBYTE lpBuff = lpByte;
		// Error Classification register
		isCallingParameterError			= (*lpBuff) & BYTE_IN_POS_07;
		isMediumPositionAbnormal		= (*lpBuff) & BYTE_IN_POS_06;
		isNoMediumAtPosition			= (*lpBuff) & BYTE_IN_POS_05;
		isInconsistencyDetected			= (*lpBuff) & BYTE_IN_POS_04;
		isHardwareTrouble				= (*lpBuff) & BYTE_IN_POS_03;
		isCountNotComplete				= (*lpBuff) & BYTE_IN_POS_02;
		isJam							= (*lpBuff) & BYTE_IN_POS_01;
		isDeviceNotReady				= (*lpBuff) & BYTE_IN_POS_00;

		lpBuff++;
		// Error detailed register
		isPoolSectionTrouble			= (*lpBuff) & BYTE_IN_POS_07;
		isMainHardwareTrouble			= (*lpBuff) & BYTE_IN_POS_06;
		isSensorAlarmOrMediumRemained	= (*lpBuff) & BYTE_IN_POS_05;
		isShutterTrouble				= (*lpBuff) & BYTE_IN_POS_04;
		isNumberOfCountedBillUnmatched	= (*lpBuff) & BYTE_IN_POS_03;
		isRejectBoxOverflow				= (*lpBuff) & BYTE_IN_POS_02;
		isCashBoxNotReady				= (*lpBuff) & BYTE_IN_POS_00;

		lpBuff++;
		// Error count error detailed register
		isBillLengthLong				= (*lpBuff) & BYTE_IN_POS_07;
		isBillLenthShort				= (*lpBuff) & BYTE_IN_POS_06;
		isBillTicknessError				= (*lpBuff) & BYTE_IN_POS_04;
		isBillToBillSpaceTooShort		= (*lpBuff) & BYTE_IN_POS_03;
		isDrawingBillsFromWrongCassette	= (*lpBuff) & BYTE_IN_POS_02;
	}
}ERROR_SENSOR_STATUS;

// 传感器状态
typedef struct _comm_sensor_status_{
	// 1.Byte One
	//bool isFirstBoxExist;			// 箱子1是否在位
	//bool isSecondBoxExist;			// 箱子2是否在位

	// 2.Byte Two
	bool isMediaExitAtFrontExit;	// 出口处有异物
	bool isMediaExistAtRearExit;	// 后出口有异物
	bool isMediaExistAtRejectPath;	// 拒钞路径传感器遮挡
	bool isMediaExistAtCountPath;	// 出钞路径传感器遮挡
	bool isMediaExistAtPathSensor;	// 传输路径传感器遮挡

	// 3.Byte Three
	bool isRejectBoxAvailable;		// 废币箱是否可以用
	bool isRejectBoxNotSet;			// 废币箱是否未设置
	bool isMediaExistAtCheckSensor;	// 纸币检测传感器是否有异物
	bool isBillCheckSensorAvailable;// 纸币检测传感器是否可用
	bool isPoolSectionAtUpper;
	bool isPoolSectionAtHomePosition;// 

	// 4.Byte Four 不明确接口定义内容是否有用
	// 5.Byte Five 不明确接口定义内容是否有用
	// 6.Byte Six  不明确接口定义内容是否有用
	// 初始化
	_comm_sensor_status_(){
		//isFirstBoxExist				= false;
		//isSecondBoxExist			= false;
									
		isMediaExitAtFrontExit		= false;
		isMediaExistAtRearExit		= false;
		isMediaExistAtRejectPath	= false;
		isMediaExistAtCountPath		= false;
		isMediaExistAtPathSensor	= false;

		isRejectBoxAvailable		= false;
		isRejectBoxNotSet			= false;
		isMediaExistAtCheckSensor	= false;
		isBillCheckSensorAvailable	= false;
		isPoolSectionAtUpper		= false;
		isPoolSectionAtHomePosition	= false;
	}

	void Deserialize(LPBYTE lpByte){
		LPBYTE lpBuff = lpByte;
		
		//isFirstBoxExist			= (*lpBuff) & BYTE_IN_POS_01;
		//isSecondBoxExist			= (*lpBuff) & BYTE_IN_POS_00;
		lpBuff++;

		isMediaExitAtFrontExit		= (*lpBuff) & BYTE_IN_POS_04;
		isMediaExistAtRearExit		= (*lpBuff) & BYTE_IN_POS_03;
		isMediaExistAtRejectPath	= (*lpBuff) & BYTE_IN_POS_02;
		isMediaExistAtCountPath		= (*lpBuff) & BYTE_IN_POS_01;
		isMediaExistAtPathSensor	= (*lpBuff) & BYTE_IN_POS_00;
		lpBuff++;

		isRejectBoxAvailable		= (*lpBuff) & BYTE_IN_POS_06;
		isRejectBoxNotSet			= (*lpBuff) & BYTE_IN_POS_05;
		isMediaExistAtCheckSensor	= (*lpBuff) & BYTE_IN_POS_04;
		isBillCheckSensorAvailable	= (*lpBuff) & BYTE_IN_POS_03;
		isPoolSectionAtUpper		= (*lpBuff) & BYTE_IN_POS_01;
		isPoolSectionAtHomePosition	= (*lpBuff) & BYTE_IN_POS_00;
	}

}SENSOR_STATUS;

// 箱子状态
typedef struct _common_cassette_status_{
	bool isFirstBoxExist;			// 箱子1是否在位
	bool isSecondBoxExist;			// 箱子2是否在位

	bool isFirstBoxBillsLow;		// 箱子1钱将空
	bool isSecondBoxBillsLow;		// 箱子2钱将空

	_common_cassette_status_(){
		isFirstBoxExist				= false;
		isSecondBoxExist			= false;

		isFirstBoxBillsLow			= false;
		isSecondBoxBillsLow			= false;
	}

	void Deserialize(LPBYTE lpByte){
		LPBYTE lpBuff = lpByte;

		isFirstBoxExist			= !((*lpBuff) & BYTE_IN_POS_04);
		isFirstBoxBillsLow		= (*lpBuff) & BYTE_IN_POS_07;	
		lpBuff++;

		isSecondBoxExist		= !((*lpBuff) & BYTE_IN_POS_04);
		isSecondBoxBillsLow		= (*lpBuff) & BYTE_IN_POS_07;
	}
}CASSETTE_STATUS;

// 纸币找零模块状态
typedef struct _comm_device_status_info{
	BYTE errCommCode;
	BYTE errDetilCode;
	ERROR_SENSOR_STATUS errSensorStatus;
	SENSOR_STATUS		SensorStatus;
	CASSETTE_STATUS		CassetteStatus;
	// 初始化
	_comm_device_status_info(){
		errCommCode = 0x00;
		errDetilCode= 0x00;
		errSensorStatus = _comm_err_sensor_status_();
		SensorStatus	= _comm_sensor_status_();
		CassetteStatus  = _common_cassette_status_();
	}

	// 填值
	void Deserialize(LPBYTE lpByte){
		LPBYTE lpBuff = lpByte;
		lpBuff += 3;
		errCommCode = *lpBuff;
		
		lpBuff++;
		errDetilCode= *lpBuff;
		
		lpBuff += 5;
		errSensorStatus.Deserialize(lpBuff);

		lpBuff +=3;
		SensorStatus.Deserialize(lpBuff);

		lpBuff +=6;
		CassetteStatus.Deserialize(lpBuff);
	}
}DEVICE_STATUS_INFO,*PDEVICE_STATUS_INFO;

// 纸币送出数量
typedef struct _banknote_change_detail_{
	DEVICE_STATUS_INFO common_status;    // 公共返回部分
	DWORD				dwBoxASendOutCount;	 // 纸币箱A已送出的数量
	DWORD				dwBoxBSendOutCount;	 // 纸币箱B已送出的数量
	DWORD				dwBoxARejectionCount; // A箱送到废币箱数量
	DWORD				dwBoxBRejectionCount; // B箱送到废币箱数量

	_banknote_change_detail_(){
		common_status = _comm_device_status_info();
		dwBoxASendOutCount   = 0;
		dwBoxBSendOutCount   = 0;
		dwBoxBRejectionCount = 0;
		dwBoxARejectionCount = 0;
	}

	void Deserialize(LPBYTE lpByte){
		LPBYTE lpBuff = lpByte;

		common_status.Deserialize(lpBuff);
		lpBuff += 39;

		// 解析数量
		dwBoxASendOutCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 2;

		dwBoxBSendOutCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 6;

		dwBoxARejectionCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 2;

		dwBoxBRejectionCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 2;
	
	}
	
	// 转换定义的数量
	BYTE TransforHEXCodeToByte(BYTE hexByte){
		switch(hexByte){
		case 0x30: return 0x00;
		case 0xB1: return 0x01;
		case 0xB2: return 0x02;
		case 0x33: return 0x03;
		case 0xB4: return 0x04;
		case 0x35: return 0x05;
		case 0x36: return 0x06;
		case 0xB7: return 0x07;
		case 0xB8: return 0x08;
		case 0x39: return 0x09;
		default:
			return 0x00;
		}
	}
}BANKNOTE_CHANGE_DETAIL,*PBANKNOTE_CHANGE_DETAIL;