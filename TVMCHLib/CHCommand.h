#ifndef __CHCOMMANDS_H__
#define __CHCOMMANDS_H__

#pragma once
#include "afx.h"
#include "CHAApi.h"
#include "DebugLog.h"
#include "Command.h"
//#include "CHException.h"
//#include "CHHardwareException.h"

//extern CHTRS_HANDLE m_hCH;		// Ӳ��ģ����
//extern bool isCHAborted;
//extern OCAS002LogDev gOCAS002Dev;
typedef DWORD CHTRS_HANDLE;   
typedef DWORD CHTRS_RESULT;
const int CH_RES_MAX = 1500;
#ifndef theCH_TRACE
	#define theCH_TRACE CFileLog::GetInstance(_T("CH_"))
#endif


/**
 @brief  CH�����
*/
class DLLIMP CCHCommand : public CCommand
{
public:
	CCHCommand(void){} 
    ~CCHCommand(void){}
public:
	

};
CHTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer = 10000);
BOOL         IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime);


//*******************************************************************************/
// ���ؽ���-ͨ��������Ӧ����
//*******************************************************************************/
typedef struct _ch_common_response
{
	BYTE ID[2];
	BYTE Lng[2];
	BYTE Res[2];             // 0x00 ������0x80 �쳣��0xFF �����쳣��0x70 ����

	BYTE Err_Info[4];         //������Ϣ
	BYTE MechanismStatus[10]; //��е״̬��Ϣ
	WORD OneY_inHopper;       //1Ԫѭ����������Ӳ������
	WORD HalfY_inHopper;	  //5��ѭ����������Ӳ������
	WORD OneY_inPreparation;  //1Ԫ��������Ӳ������
	WORD HalfY_inPreparation; //5�Ǳ�������Ӳ������
	WORD OneY_inRecover;      //1Ԫ��������Ӳ������
	WORD HalfY_inRecover;     //5�ǻ�������Ӳ������
	BYTE Reserve1[2];
	BYTE Reserve2[2];

	// ��е״̬��ϸ��Ϣ
	// ����
	bool bIsCoinModuleStandby;	// Ӳ��ģ���Ƿ�λ��true:��λ��false��δ��λ
	// Hopper A
	bool bIsHopperAFull;		// Hopper �Ƿ���
	bool bIsHopperAEmpty;		// Hopper �Ƿ��
	// Hopper B
	bool bIsHopperBFull;		// Hopper �Ƿ���
	bool bIsHopperBEmpty;		// Hopper �Ƿ��
	// ����Ǯ��A
	bool bIsChangeBoxAInstall;	// Change BoxA�Ƿ�װ
	bool bIsChangeBoxAFull;		// Change BoxA�Ƿ���
	bool bIsChangeBoxAEmpty;	// Change BoxA�Ƿ��
	// ����Ǯ��B
	bool bIsChangeBoxBInstall;	// Change BoxB�Ƿ�װ
	bool bIsChangeBoxBFull;		// Change BoxB�Ƿ���
	bool bIsChangeBoxBEmpty;	// Change BoxB�Ƿ��

	inline int Deserialize(const BYTE* data)
	{
		memcpy(ID,data,2);
		memcpy(Lng,data + 2,2);
		memcpy(Res,data + 4,2);
		memcpy(Err_Info,data + 6,4);
		memcpy(MechanismStatus,data + 10,10);
		OneY_inHopper = (data[20]<<8)|data[21];
		HalfY_inHopper = (data[22]<<8)|data[23];
		OneY_inPreparation = (data[24]<<8)|data[25];
		HalfY_inPreparation = (data[26]<<8)|data[27];
		OneY_inRecover = (data[28]<<8)|data[29];
		HalfY_inRecover = (data[30]<<8)|data[31];
		memcpy(Reserve1,data+32,2);
		memcpy(Reserve2,data+34,2);

		bIsCoinModuleStandby = !(MechanismStatus[0] & BYTE_IN_CH_POS_03);
		bIsHopperAFull		 = (MechanismStatus[2] & BYTE_IN_CH_POS_04);
		bIsHopperAEmpty		 = (MechanismStatus[2] & BYTE_IN_CH_POS_00);

		bIsHopperBFull		 = (MechanismStatus[3] & BYTE_IN_CH_POS_04);
		bIsHopperBEmpty		 = (MechanismStatus[3] & BYTE_IN_CH_POS_00);

		bIsChangeBoxAInstall = !(MechanismStatus[4] & BYTE_IN_CH_POS_05);
		bIsChangeBoxAFull	 = (MechanismStatus[4] & BYTE_IN_CH_POS_04);
		bIsChangeBoxAEmpty	 = (MechanismStatus[4] & BYTE_IN_CH_POS_00);

		bIsChangeBoxBInstall = !(MechanismStatus[5] & BYTE_IN_CH_POS_05);
		bIsChangeBoxBFull	 = (MechanismStatus[5] & BYTE_IN_CH_POS_04);
		bIsChangeBoxBEmpty	 = (MechanismStatus[5] & BYTE_IN_CH_POS_00);

		return 36;
	}

} CH_COMMON_RSP,*LPCH_COMMON_RSP;
//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ�������״̬��ȡ���������Ӧ
//*******************************************************************************/
typedef struct _ch_spec1_response
{
	CH_COMMON_RSP Common;    //��������
	BYTE ActionStatus[2];    //����״̬
	//���½�����Ϣ
	WORD OneY_inTemporary;   //�ݴ�����1ԪӲ�ҵĸ���
	WORD HalfY_inTemporary;   //�ݴ�����5��Ӳ�ҵĸ���
	WORD num_Rejected;		  //�ܾ�����
	WORD num_Overrun;		  //��������
	bool bBusyRecycle;		  //�Զ�����æµ��
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		memcpy(ActionStatus,data+offset,2);
		bBusyRecycle = (data[offset] & 0x80)&&(data[offset+1]&0x02);
		OneY_inTemporary = (data[offset + 2]<<4)|data[offset + 2 + 1];
		HalfY_inTemporary = (data[offset + 2 + 2]<<4)|data[offset + 2 + 3];
		num_Rejected = (data[offset + 2 + 4]<<4)|data[offset + 2 + 5];
		num_Overrun = (data[offset + 2 + 6]<<4)|data[offset + 2 + 7];
		return offset+2+8;
	}

} CH_SPEC1_RSP,*LPCH_SPEC1_RSP;

//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ����������Ҽ������������Ӧ
//*******************************************************************************/
typedef struct _ch_spec4_response
{
	CH_COMMON_RSP Common;    //��������
	BYTE CoinComeout_Info[8];   //������Ϣ
	WORD wOneYuanCount;			// ����һԪ����
	WORD wHalfYuanCount;		// �����������
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		memcpy(CoinComeout_Info,data+offset,8);

		wOneYuanCount = MAKEWORD(CoinComeout_Info[1],CoinComeout_Info[0]);
		wHalfYuanCount = MAKEWORD(CoinComeout_Info[3],CoinComeout_Info[2]);
		return offset+8;
	}

} CH_SPEC4_RSP,*LPCH_SPEC4_RSP;
//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ����������ա��������Ӧ
//*******************************************************************************/
typedef struct _ch_spec5_response
{
	CH_COMMON_RSP Common;    //��������
	// ʵ�ʻ�������
	WORD wOneHopperCount;		// ���(HOPPER B)
	WORD wHalfHopperCount;		// �ұ�(HOPPER A)
	WORD wOneChangeBoxCount;	// �ұ�(������ A)
	WORD wHalfChangeBoxCount;	// ���(������ B)

//	BYTE num_inRecover[8];   //���տ���ո���
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		//memcpy(num_inRecover,data+offset,8);
		wOneHopperCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		wHalfHopperCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		wOneChangeBoxCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		wHalfChangeBoxCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		return offset;
	}

} CH_SPEC5_RSP,*LPCH_SPEC5_RSP;

//*******************************************************************************/
// Datawrite/Datalight�������
//*******************************************************************************/
typedef struct _ch_DatalightInfo  
{
	WORD OneY_inHopper;							   //1ԪHopper���е�Ӳ������ 
	WORD HalfY_inHopper;                           //5��Hopper���е�Ӳ������ 
	WORD OneY_preparation;                         //1Ԫ�������е�Ӳ������
	WORD HalfY_preparation;                        //5�Ǳ������е�Ӳ������
	WORD OneY_inRecover;						   //1Ԫ�������е�Ӳ������
	WORD HalfY_inRecover;                          //5�ǻ������е�Ӳ������
	WORD Reserve1_inRecover;                       //�����䱣���ֶ�1
	WORD Reserve2_inRecover;                       //�����䱣���ֶ�2
	WORD OneY_inHopper_preInterruption;               //����1ԪHopper���ж�Ԥ����Ӳ������
	WORD HalfY_inHopper_preInterruption;              //����5��Hopper���ж�Ԥ����Ӳ������
	WORD OneY_preparation_preInterruption;        //����1Ԫ�������ж�Ԥ����Ӳ������
	WORD HalfY_preparation_preInterruption;       //����5�Ǳ������ж�Ԥ����Ӳ������
	WORD OneY_inHopper_Interruption;			   //����1ԪHopper��Ԥ����Ӳ������
	WORD HalfY_inHopper_Interruption;              //����5��Hopper��Ԥ����Ӳ������
	WORD OneY_preparation_Interruption;				//����1Ԫ������Ԥ����Ӳ������
	WORD HalfY_preparation_Interruption;            //����5�Ǳ������ж�Ԥ����Ӳ������
	WORD OneY_maxthreshold_inRecover;				//������1Ԫ������ֵ
	WORD HalfY_maxthreshold_inRecover;				//������5��������ֵ
	WORD Reserve1_maxthreshold_inRecover;           
	WORD Reserve2_maxthreshold_inRecover;
	WORD OneY_max_inRecover;                        //������1Ԫ������
	WORD HalfY_max_inRecover;                       //������5��������
	WORD Reserve1_max_inRecover;
	WORD Reserve2_max_inRecover;
	BYTE Year;										//����ȥ��ǧ����λ�Ľ����BCD�룬���磺2015->15->0x15
	BYTE Month;										//BCD��
	BYTE Day;										//BCD��
	BYTE Hour;										//BCD��
	BYTE Minute;									//BCD��
	BYTE Second;									//BCD��

	_ch_DatalightInfo()
	{
		OneY_inHopper=0;
		HalfY_inHopper=0;
		OneY_preparation=0;
		HalfY_preparation=0;
		OneY_inRecover=0;
		HalfY_inRecover=0;
		Reserve1_inRecover=0;
		Reserve2_inRecover=0;
		OneY_inHopper_preInterruption=20;
		HalfY_inHopper_preInterruption=20;
		OneY_preparation_preInterruption=20;
		HalfY_preparation_preInterruption=20;
		OneY_inHopper_Interruption=10;
		HalfY_inHopper_Interruption=10;
		OneY_preparation_Interruption=10;
		HalfY_preparation_Interruption=10;
		OneY_maxthreshold_inRecover=200;
		HalfY_maxthreshold_inRecover=200;
		Reserve1_maxthreshold_inRecover=200;           
		Reserve2_maxthreshold_inRecover=200;
		OneY_max_inRecover=0;
		HalfY_max_inRecover=0;
		Reserve1_max_inRecover=0;
		Reserve2_max_inRecover=0;
		Year=0;
		Month=0;
		Day=0;
		Hour=0;
		Minute=0;
		Second=0;
	}
}DatalightInfo,*pDatalightInfo;



#if 0
typedef struct
{
UINT uiProviderId;
UINT uiModuleType;
UINT uiType;
UINT uiErrorType[16];
UINT uiErrorCode[16];
char cReverse[8];
} tCHADevReturn;

typedef struct
{
UINT uiBoxType;
UINT uiBoxNo;
UINT uiCoinValue;
UINT uiCoinNum;
UINT uiCoinMaxNum;
UINT uiBoxStatus;
} tCoinBoxValueInfo;

typedef struct
{
UINT uiCoinBoxNum;
tCoinBoxValueInfo* pCoinBoxValueInfo;
} tCHACoinBoxInfo;


typedef struct
{
tCoinBoxValueInfo stCoinBoxValueInfo[2];
UINT uiEjectCoinNum;
} tCHACoinValueInfo;

typedef struct
{
UINT uiCoinValue;
UINT uiCoinNum;
} tCoinChangeInfo;
/*
typedef struct
{
UINT uiCoinTypeNum;
tCoinChangeInfo* pCoinChangeInfo;
} tCHAChangeNum;
*/

typedef struct 
{
	BYTE byMHopper1Num; // ��������2������	
	BYTE byMHopper2Num; // ��������1������	
}tCHAChangeNum;

typedef struct
{
UINT uiSensorID;
UINT uiStatus;
} SensorStatus;

typedef struct
{
UINT uiSensorNum;
SensorStatus* pSensorStatus;
} tCHASensorStatus;
#endif

#endif
