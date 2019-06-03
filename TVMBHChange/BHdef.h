#pragma once

#include <vector>
#include "Util.h"
#include "CommonDefine.h"

using namespace std;

#define MAX_SEND_ITEMS_COUNT		1		 // ���Ͷ������Ԫ�ظ���
#define MAX_RECEIVE_ITEMS_COUNT	    1		 // ���ն������Ԫ�ظ���

namespace BHCHANGE
{
#define RES_MAX 256
}

typedef struct _box_limmit_info {
	//ֻ����������������
	BYTE num_out;   //�������������20
	BYTE reject;  //��ǰ���Ӿܾ������������ֵ���20
	BYTE pick_retries;  //�������������15
private:
	BYTE InfoConverted[5];  //�����ɿɶ���Ϣת�����ɵķ�����Ϣ
	BYTE map_readableTOsend(BYTE num)  //�ɶ���Ϣ(1-9)��������Ϣ��ӳ���ϵ
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
	BYTE* toconvert()   //�ɶ���Ϣת���ɷ�����Ϣ
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
	BoxLimInfo box[8];   //1-8�ź���,1:box[0]......8:box[8]
}ParamCountBill,*PParamCountBill;


typedef struct _box_lim_bh_lenAndthickness
{
	BYTE max_Len;		//ֽ����󳤶� ��λ��mm
	BYTE min_Len;		//ֽ����С���� ��λ��mm
	BYTE thickness;		//ֽ�Һ��  ��λ��0.01mm
	_box_lim_bh_lenAndthickness(){
		max_Len = 0;
		min_Len = 0;
		thickness = 0;
	}
}BoxLimBHInfo,*PBoxLimBHInfo;

typedef struct _param_DevInit {
	BoxLimBHInfo box[8];   //1-8�ź���,1:box[0]......8:box[8]
}ParamDevInit,*PParamDevInit;


typedef struct _commonfeedback
{
	BYTE ErrCode[2];  //�������
	BYTE Version[4];   //�汾��Ϣ
	BYTE ErrRegister[3];  //����Ĵ���
	BYTE SensorRegister[6]; //�������Ĵ���
	BYTE boxRegister[4];   //���ӼĴ���
	ParamDevInit obj;    //1-8�ź�����ֽ�ҳ��Ⱥͺ����Ϣ

}CommonFeedback,*PCommonFeedback;

// ������λ���ݴ�������
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

// ����������״̬
typedef struct _comm_err_sensor_status_{
	// Error Classification register
	bool isCallingParameterError;
	bool isMediumPositionAbnormal;
	bool isNoMediumAtPosition;		// �ƶ����ʲ���λ
	bool isInconsistencyDetected;	// INCND
	bool isHardwareTrouble;			// Ӳ������
	bool isCountNotComplete;		// ����δ���
	bool isJam;						// ����
	bool isDeviceNotReady;			// ֽ������ģ��δ׼����

	// Error detailed register
	bool isPoolSectionTrouble;		
	bool isMainHardwareTrouble;		
	bool isSensorAlarmOrMediumRemained;
	bool isShutterTrouble;			// �����쳣
	bool isNumberOfCountedBillUnmatched;
	bool isRejectBoxOverflow;		// �ܳ�����
	bool isCashBoxNotReady;			// ������δ׼����

	// Error count error detailed register
	bool isBillLengthLong;
	bool isBillLenthShort;
	bool isBillTicknessError;
	bool isBillToBillSpaceTooShort;			// 
	bool isDrawingBillsFromWrongCassette;	// ���Ҵ���

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

	// ��������Ϣλת��Ϊ��λ����
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

// ������״̬
typedef struct _comm_sensor_status_{
	// 1.Byte One
	//bool isFirstBoxExist;			// ����1�Ƿ���λ
	//bool isSecondBoxExist;			// ����2�Ƿ���λ

	// 2.Byte Two
	bool isMediaExitAtFrontExit;	// ���ڴ�������
	bool isMediaExistAtRearExit;	// �����������
	bool isMediaExistAtRejectPath;	// �ܳ�·���������ڵ�
	bool isMediaExistAtCountPath;	// ����·���������ڵ�
	bool isMediaExistAtPathSensor;	// ����·���������ڵ�

	// 3.Byte Three
	bool isRejectBoxAvailable;		// �ϱ����Ƿ������
	bool isRejectBoxNotSet;			// �ϱ����Ƿ�δ����
	bool isMediaExistAtCheckSensor;	// ֽ�Ҽ�⴫�����Ƿ�������
	bool isBillCheckSensorAvailable;// ֽ�Ҽ�⴫�����Ƿ����
	bool isPoolSectionAtUpper;
	bool isPoolSectionAtHomePosition;// 

	// 4.Byte Four ����ȷ�ӿڶ��������Ƿ�����
	// 5.Byte Five ����ȷ�ӿڶ��������Ƿ�����
	// 6.Byte Six  ����ȷ�ӿڶ��������Ƿ�����
	// ��ʼ��
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

// ����״̬
typedef struct _common_cassette_status_{
	bool isFirstBoxExist;			// ����1�Ƿ���λ
	bool isSecondBoxExist;			// ����2�Ƿ���λ

	bool isFirstBoxBillsLow;		// ����1Ǯ����
	bool isSecondBoxBillsLow;		// ����2Ǯ����

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

// ֽ������ģ��״̬
typedef struct _comm_device_status_info{
	BYTE errCommCode;
	BYTE errDetilCode;
	ERROR_SENSOR_STATUS errSensorStatus;
	SENSOR_STATUS		SensorStatus;
	CASSETTE_STATUS		CassetteStatus;
	// ��ʼ��
	_comm_device_status_info(){
		errCommCode = 0x00;
		errDetilCode= 0x00;
		errSensorStatus = _comm_err_sensor_status_();
		SensorStatus	= _comm_sensor_status_();
		CassetteStatus  = _common_cassette_status_();
	}

	// ��ֵ
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

// ֽ���ͳ�����
typedef struct _banknote_change_detail_{
	DEVICE_STATUS_INFO common_status;    // �������ز���
	DWORD				dwBoxASendOutCount;	 // ֽ����A���ͳ�������
	DWORD				dwBoxBSendOutCount;	 // ֽ����B���ͳ�������
	DWORD				dwBoxARejectionCount; // A���͵��ϱ�������
	DWORD				dwBoxBRejectionCount; // B���͵��ϱ�������

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

		// ��������
		dwBoxASendOutCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 2;

		dwBoxBSendOutCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 6;

		dwBoxARejectionCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 2;

		dwBoxBRejectionCount = MAKEWORD(TransforHEXCodeToByte(*(lpBuff + 1)),TransforHEXCodeToByte(*lpBuff));
		lpBuff += 2;
	
	}
	
	// ת�����������
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