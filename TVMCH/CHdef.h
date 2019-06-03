#pragma once

#include <vector>
#include "Util.h"
#include "CommonDefine.h"
#include "CHAApi.h"
using namespace std;

#define E_NO_CH_ERROR 0 
#define MAX_SEND_ITEMS_COUNT		10		 // 发送队列最大元素个数
#define MAX_RECEIVE_ITEMS_COUNT	    10		 // 接收队列最大元素个数
const int CH_RES_MAX = 1500;			// 纸币接收模块的回复不超过1500字节

// 解析各位数据代表意义
typedef enum _CH_BOX_NO {
	Undefine,
	Hopper,//循环箱
	Preparation, //备用箱
	Recover, //回收箱
} CH_BOX_NO;






// 通用-硬币信息
typedef struct _ch_common_response_coininfo
{
	WORD num_OneY_inHopper;        //1元Hopper箱计数
	WORD num_HalfY_inHopper;        //5角Hopper箱计数
	WORD num_OneY_inPreparation;        //1元备用箱计数
	WORD num_HalfY_inPreparation;        //5角备用箱计数
	WORD num_OneY_inRecover;        //1元回收箱计数
	WORD num_HalfY_inRecover;        //5角回收箱计数
	WORD num_OneY_inTemporary;        //1元暂存箱计数
	WORD num_HalfY_inTemporary;        //5角暂存箱计数



	_ch_common_response_coininfo(){
		num_OneY_inHopper        = 0;
		num_HalfY_inHopper        = 0;
		num_OneY_inPreparation        = 0;
		num_HalfY_inPreparation        = 0;
		num_OneY_inRecover        = 0;
		num_HalfY_inRecover        = 0;
		num_OneY_inTemporary        = 0;
		num_HalfY_inTemporary        = 0;

	}
	void Serialize(LPBYTE lpByte)
	{
		char* buf = (char*)lpByte;
		*(unsigned short*)buf = num_OneY_inHopper;                buf += sizeof num_OneY_inHopper;
		*(unsigned short*)buf = num_HalfY_inHopper;                buf += sizeof num_HalfY_inHopper;
		*(unsigned short*)buf = num_OneY_inPreparation;                 buf += sizeof num_OneY_inPreparation;   
		*(unsigned short*)buf = num_OneY_inPreparation;                 buf += sizeof num_OneY_inPreparation;   
		*(unsigned short*)buf = num_OneY_inRecover;                buf += sizeof num_OneY_inRecover;
		*(unsigned short*)buf = num_HalfY_inRecover;                buf += sizeof num_HalfY_inRecover;
		*(unsigned short*)buf = num_OneY_inTemporary;                 buf += sizeof num_OneY_inTemporary;   
		*(unsigned short*)buf = num_OneY_inTemporary;                 buf += sizeof num_OneY_inTemporary;   

	}
	inline int Deserialize(const char* lpByte)
	{
		const char* cp = lpByte;  
		num_OneY_inHopper       = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inHopper;
		num_HalfY_inHopper       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inHopper;
		num_OneY_inRecover      = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inRecover;
		num_HalfY_inRecover       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inRecover;
		num_OneY_inPreparation       = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inPreparation;
		num_HalfY_inPreparation       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inPreparation;
		num_OneY_inTemporary       = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inTemporary;
		num_HalfY_inTemporary       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inTemporary;
		return (cp - lpByte);
	}
} CH_COMMON_RSP_COININFO,*LPCH_COMMON_RSP_STATUS;


/*******************************************************************************
// 	接收硬币信息-返回解析
*******************************************************************************/	
typedef struct _ch_common_response_accept_coin_info
{
	unsigned short escrow1;
	unsigned short escrow2_05;
	unsigned short reject;


	_ch_common_response_accept_coin_info()
	{
		escrow1   =0;    //暂存1-1元个数
		escrow2_05=0;    //暂存2-0.5元个数
		reject    =0;    //本次交易退币

	}
	inline int Deserialize(const char* lpByte)
	{
		const char* cp = lpByte;

		escrow1   = (cp[0]<<8)|cp[1];               cp  += sizeof escrow1;
		escrow2_05= (cp[0]<<8)|cp[1];               cp  += sizeof escrow2_05;
		reject    = (cp[0]<<8)|cp[1];                cp  += sizeof reject;
		return (cp - lpByte);
	}
} CH_COMMON_RSP_ACCEPT_COIN_INFO;


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
// 返回解析-命令响应数据，包括“禁止投币”命令的响应
//*******************************************************************************/
typedef struct _ch_spec2_response
{
	CH_COMMON_RSP Common;			//基本反馈
	//BYTE CoinInserted_Info[8];	//进币信息
	WORD wOneYuanInCome;			// 一元接收数量
	WORD wHalfYuanInCome;			// 五角接收数量
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		//memcpy(CoinInserted_Info,data+offset,8);
		wOneYuanInCome = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;
		wHalfYuanInCome = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;
		return offset+4;
	}

} CH_SPEC2_RSP,*LPCH_SPEC2_RSP;

//*******************************************************************************/
// 返回解析-命令响应数据，包括“参数设定”命令的响应
//*******************************************************************************/
typedef struct _ch_spec3_response
{
	CH_COMMON_RSP Common;    //基本反馈
	BYTE SeriesNO[2];   //串行编号
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		memcpy(SeriesNO,data+offset,2);

		return offset+2;
	}

} CH_SPEC3_RSP,*LPCH_SPEC3_RSP;


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




//zhengxianle add for now
//*******************************************************************************/
//返回头
//*******************************************************************************/
typedef struct _ch_common_response_head {
public:
	enum {
		RESP_INITIAL              =0x81,//1.初始化           
		RESP_STATUS_READ          =0x82,//2.状态读取
		RESP_ENABLE               =0x83,//3.开始接收硬币                        
		RESP_DISABLE              =0x84,//4.结束接收硬币
		RESP_GET_INSERT_INFO      =0x85,//5.读取接收金额
		RESP_ACCEPT               =0x86,//6.倒出硬币
		RESP_RELEASE              =0x87,//7.找零
		RESP_DATAWRITE            =0x88,//8.设置金额               
		RESP_SAVE                 =0x89,//9.清币   	       
		RESP_VERSION_SENSE        =0x95,//10.读版本号
		RESP_HOPPER_START         =0x96,//11.开始加币                  
		RESP_HOPPER_END           =0x97,//12.结束加币    
		RESP_RFID_EN_READ         =0xE1,//13.读取钱箱SN 号（RFID）
		RESP_RFID_EN_SET          =0xE2,//13.读取钱箱SN 号（RFID）
		RESP_RFID_SET             =0xE3,//14.写iButton数据                        
		RESP_RFID_READ            =0xE4,//15.读iButton数据          
		RESP_COMMON               =0x99, 
		CNTL_SAFE_LOCK            =0x9A,//16.加币安全扣
	};
	unsigned char   resp;              // 1byte(Bynary)
	unsigned char   rslt;              // 1byte(Bynary)
	unsigned char   mtc;               // 1byte(Bynary)
	_ch_common_response_head()
	{
		resp = 0x00; 
		rslt = 0x00; 
		mtc  = 0x00;

	}

	inline int Deserialize(const char* lpByte)
	{
		/*const char* cp = lpByte;
		resp   = *(unsigned char*)cp; cp += sizeof resp;
		rslt   = *(unsigned char*)cp; cp += sizeof rslt;
		mtc    = *(unsigned char*)cp; cp += sizeof mtc;
		return (cp - lpByte); */
	}

}CH_COMMON_RSP_HEAD;
//*******************************************************************************/
//返回解析-读取接受金额命令响应数据-4.8.5读取接受金额
//*******************************************************************************/
typedef struct _ch_response_insert_info
{

	CH_COMMON_RSP_HEAD                       head;
	CH_COMMON_RSP_ACCEPT_COIN_INFO    insert_info;
	inline int Deserialize(const char* data)
	{

		/*const char* bufp = data;
		int sz = head.Deserialize(bufp);         bufp += sz;
		sz = insert_info.Deserialize(bufp);  bufp += sz;

		return int(bufp-data);*/
		return 0;
	}
} CH_RSP_ACCEPT_INSERT_INFO;
