#ifndef __CHCOMMANDS_H__
#define __CHCOMMANDS_H__

#pragma once
#include "afx.h"
#include "CHAApi.h"
#include "DebugLog.h"
#include "Command.h"
//#include "CHException.h"
//#include "CHHardwareException.h"

//extern CHTRS_HANDLE m_hCH;		// 硬币模块句柄
//extern bool isCHAborted;
//extern OCAS002LogDev gOCAS002Dev;
typedef DWORD CHTRS_HANDLE;   
typedef DWORD CHTRS_RESULT;
const int CH_RES_MAX = 1500;
#ifndef theCH_TRACE
	#define theCH_TRACE CFileLog::GetInstance(_T("CH_"))
#endif


/**
 @brief  CH命令父类
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
// 返回解析-通用命令响应数据
//*******************************************************************************/
typedef struct _ch_common_response
{
	BYTE ID[2];
	BYTE Lng[2];
	BYTE Res[2];             // 0x00 正常，0x80 异常，0xFF 命令异常，0x70 警告

	BYTE Err_Info[4];         //错误信息
	BYTE MechanismStatus[10]; //机械状态信息
	WORD OneY_inHopper;       //1元循环找零箱中硬币数量
	WORD HalfY_inHopper;	  //5角循环找零箱中硬币数量
	WORD OneY_inPreparation;  //1元备用箱中硬币数量
	WORD HalfY_inPreparation; //5角备用箱中硬币数量
	WORD OneY_inRecover;      //1元回收箱中硬币数量
	WORD HalfY_inRecover;     //5角回收箱中硬币数量
	BYTE Reserve1[2];
	BYTE Reserve2[2];

	// 机械状态详细信息
	// 整机
	bool bIsCoinModuleStandby;	// 硬币模块是否到位？true:到位；false：未到位
	// Hopper A
	bool bIsHopperAFull;		// Hopper 是否满
	bool bIsHopperAEmpty;		// Hopper 是否空
	// Hopper B
	bool bIsHopperBFull;		// Hopper 是否满
	bool bIsHopperBEmpty;		// Hopper 是否空
	// 备用钱箱A
	bool bIsChangeBoxAInstall;	// Change BoxA是否安装
	bool bIsChangeBoxAFull;		// Change BoxA是否满
	bool bIsChangeBoxAEmpty;	// Change BoxA是否空
	// 备用钱箱B
	bool bIsChangeBoxBInstall;	// Change BoxB是否安装
	bool bIsChangeBoxBFull;		// Change BoxB是否满
	bool bIsChangeBoxBEmpty;	// Change BoxB是否空

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
// 返回解析-命令响应数据，包括“状态读取”命令的响应
//*******************************************************************************/
typedef struct _ch_spec1_response
{
	CH_COMMON_RSP Common;    //基本反馈
	BYTE ActionStatus[2];    //动作状态
	//以下进币信息
	WORD OneY_inTemporary;   //暂存箱中1元硬币的个数
	WORD HalfY_inTemporary;   //暂存箱中5角硬币的个数
	WORD num_Rejected;		  //拒绝个数
	WORD num_Overrun;		  //超出个数
	bool bBusyRecycle;		  //自动回收忙碌中
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
// 返回解析-命令响应数据，包括“出币计数”命令的响应
//*******************************************************************************/
typedef struct _ch_spec4_response
{
	CH_COMMON_RSP Common;    //基本反馈
	BYTE CoinComeout_Info[8];   //出币信息
	WORD wOneYuanCount;			// 出币一元数量
	WORD wHalfYuanCount;		// 出币五角数量
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
// 返回解析-命令响应数据，包括“回收”命令的响应
//*******************************************************************************/
typedef struct _ch_spec5_response
{
	CH_COMMON_RSP Common;    //基本反馈
	// 实际回收数量
	WORD wOneHopperCount;		// 左边(HOPPER B)
	WORD wHalfHopperCount;		// 右边(HOPPER A)
	WORD wOneChangeBoxCount;	// 右边(找零箱 A)
	WORD wHalfChangeBoxCount;	// 左边(找零箱 B)

//	BYTE num_inRecover[8];   //回收库回收个数
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
// Datawrite/Datalight命令参数
//*******************************************************************************/
typedef struct _ch_DatalightInfo  
{
	WORD OneY_inHopper;							   //1元Hopper箱中的硬币数量 
	WORD HalfY_inHopper;                           //5角Hopper箱中的硬币数量 
	WORD OneY_preparation;                         //1元备用箱中的硬币数量
	WORD HalfY_preparation;                        //5角备用箱中的硬币数量
	WORD OneY_inRecover;						   //1元回收箱中的硬币数量
	WORD HalfY_inRecover;                          //5角回收箱中的硬币数量
	WORD Reserve1_inRecover;                       //回收箱保留字段1
	WORD Reserve2_inRecover;                       //回收箱保留字段2
	WORD OneY_inHopper_preInterruption;               //触发1元Hopper箱中断预报的硬币数量
	WORD HalfY_inHopper_preInterruption;              //触发5角Hopper箱中断预报的硬币数量
	WORD OneY_preparation_preInterruption;        //触发1元备用箱中断预报的硬币数量
	WORD HalfY_preparation_preInterruption;       //触发5角备用箱中断预报的硬币数量
	WORD OneY_inHopper_Interruption;			   //触发1元Hopper箱预报的硬币数量
	WORD HalfY_inHopper_Interruption;              //触发5角Hopper箱预报的硬币数量
	WORD OneY_preparation_Interruption;				//触发1元备用箱预报的硬币数量
	WORD HalfY_preparation_Interruption;            //触发5角备用箱中断预报的硬币数量
	WORD OneY_maxthreshold_inRecover;				//回收箱1元满载阈值
	WORD HalfY_maxthreshold_inRecover;				//回收箱5角满载阈值
	WORD Reserve1_maxthreshold_inRecover;           
	WORD Reserve2_maxthreshold_inRecover;
	WORD OneY_max_inRecover;                        //回收箱1元满载数
	WORD HalfY_max_inRecover;                       //回收箱5角满载数
	WORD Reserve1_max_inRecover;
	WORD Reserve2_max_inRecover;
	BYTE Year;										//年数去掉千、百位的结果的BCD码，例如：2015->15->0x15
	BYTE Month;										//BCD码
	BYTE Day;										//BCD码
	BYTE Hour;										//BCD码
	BYTE Minute;									//BCD码
	BYTE Second;									//BCD码

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
	BYTE byMHopper1Num; // 主找零器2找零数	
	BYTE byMHopper2Num; // 主找零器1找零数	
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
