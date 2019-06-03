#ifndef __TOKENDEF_H_2011_06_02__
#define __TOKENDEF_H_2011_06_02__

#include "CommonDefine.h"
#include "TOKENLib.h"

#ifdef TOKEN_EXPORTS
#define TOKEN_API __declspec(dllexport)
#else
#define TOKEN_API __declspec(dllimport)
#endif



typedef enum _th_box_no_{
	TOKEN_BOX_ALL	= 0x00,
	TOKEN_BOX_A		= 0x01,
	TOKEN_BOX_B		= 0x02,
	TOKEN_BOX_C		= 0x03,
}TOKEN_BOX_NO;

typedef enum _th_box_rfid_no_{
	TOKEN_BOX_RFID_A	= 0x04,
	TOKEN_BOX_RFID_B	= 0x03,
	TOKEN_BOX_RFID_C	= 0x06,
}TOKEN_BOX_RFID_NO;

///** 
//@brief      传感器定义
//*/
//typedef enum TH_CMD_SENSOR
//{
//	SENSOR01_ID = 61000,	// 61000 (0:正常;1:触发) 票卡出口检测传感器
//	SENSOR02_ID = 61001,    // 61001 (0:正常;1:触发) 读写区检测传感器
//	SENSOR07_ID = 61006,    // 61006 (0:到位;1:移除) 票箱 A 到位检测传感器
//	SENSOR08_ID = 61007,    // 61007 (0:打开;1:关闭) 票箱 A 顶盖检测传感器（塑料票箱未使用）
//	SENSOR09_ID = 61008,    // 61008 (0:正常;1:将空) 票箱 A 将空传感器   
//	SENSOR10_ID = 61009,    // 61009 (0:正常;1:已空) 票箱 A 空传感器
//	SENSOR13_ID = 61012,    // 61012 (0:到位;1:移除) 票箱 B 到位检测传感器
//	SENSOR14_ID = 61013,    // 61013 (0:打开;1:关闭) 票箱 B 顶盖检测传感器（塑料票箱未使用） 
//	SENSOR15_ID = 61014,    // 61014 (0:正常;1:将空) 票箱 B 将空传感器
//	SENSOR16_ID = 61015,    // 61015 (0:正常;1:已空) 票箱 B 空传感器
//	SENSOR19_ID = 61018,    // 61018 (0:到位;1:移除) 废票箱到位检测传感器
//	SENSOR20_ID = 61019,    // 61019 (0:打开;1:关闭) 废票箱顶盖检测传感器（塑料票箱未使用）
//	SENSOR21_ID = 61020,    // 61020 (0:正常;1:已满) 废票箱满检测传感器
//};


/** 
@brief        通讯建立参数结构
@param     (i)UINT uiCommPort    端口号
@param     (i)UINT uiBaudRate    波特率
*/
typedef struct _tem_token_cmd_comm_open
{
	UINT uiCommPort;    // 端口号
	UINT uiBaudRate;    // 波特率

	_tem_token_cmd_comm_open(){
		uiCommPort = 0;
		uiBaudRate = 0;
	}
} TOKEN_CMD_COMM_OPEN, *LPTOKEN_CMD_COMM_OPEN;

/** 
@brief      模块初始化参数结构-命令
*/
typedef enum TOKEN_CMD_INIT
{
	CMD_INIT_CLEAN			= 1,    // 通道清票
	CMD_INIT_NOT_CLEAN		= 2,    // 通道不清票
};

/** 
@brief      读取设备状态参数结构
*/
typedef struct _token_rsp_get_dev_status
{
	// 返回模块的状态信息
	bool bBoxANearEmpty;        // 票箱A将空
	bool bBoxAExist;			// 票箱A存在
	bool bBoxBNearEmpty;        // 票箱B将空
	bool bBoxBExist;			// 票箱B存在
	bool bBoxAEmpty;			// 票箱A空
	bool bBoxBEmpty;			// 票箱B空
	bool bCardInReadArea;       // 有空在读写区
	// --------------
	bool bHopperAException;     // HopperA是否故障
	bool bHopperBException;     // HopperB是否故障
	bool bHasCardAtTemporaryA;	// A暂存区是否有票（有暂存区模块专用）
	bool bHasCardAtTemporaryB;	// B暂存区是否有票（有暂存区模块专用）

	_token_rsp_get_dev_status()
	{
		bBoxANearEmpty			= false;
		bBoxAExist				= false;
		bBoxBNearEmpty			= false;
		bBoxBExist				= false;
		bBoxAEmpty				= false;
		bBoxBEmpty				= false;
		bCardInReadArea			= false;
		bHopperAException		= false;
		bHopperBException		= false;
		bHasCardAtTemporaryA	= false;
		bHasCardAtTemporaryB	= false;
	}
} TOKEN_RSP_GET_DEV_STATUS, *LPTOKEN_RSP_GET_DEV_STATUS;;


/** 
@brief      读取设备驱动版本信息
*/
typedef struct _token_rsp_get_version
{
	// 返回模块的状态信息
	unsigned char ucModNum[8];			// 模块号
	unsigned char ucFwVer[7];

	_token_rsp_get_version(){
		memset(ucModNum, 0, sizeof(ucModNum));
		memset(ucFwVer, 0, sizeof(ucFwVer));
	}
} TOKEN_RSP_GET_VERSION, *LPTOKEN_RSP_GET_VERSION;


typedef struct _token_rfid_data
{
	unsigned char uBoxNo[8+1];
	_token_rfid_data(){
		memset(&uBoxNo, 0, sizeof(uBoxNo));
	}
} TOKEN_RFID_DATA, *LPTOKEN_RFID_DATA;

// 设置RFID
typedef struct _tem_token_read_write_rfid
{
	unsigned char ucRfidNo;//0x04 = A 箱 0x03 = B 箱 0x05 = 清票箱 0x06 = 废票箱
	TOKEN_RFID_DATA rfidData;

	_tem_token_read_write_rfid(){
		ucRfidNo = 0;
		memset(&rfidData, 0, sizeof(rfidData));
	}
} TOKEN_READ_WRITE_RFID, *LPTOKEN_READ_WRITE_RFID;

#endif
