#ifndef __TOKENCOMMANDS_H__
#define __TOKENCOMMANDS_H__

#pragma once
#include "StdAfx.h"
#include "TOKENLIBExp.h"
#include "TOKENCOMLIB_DEF.h"
#include "Command.h"
#include "ToIMApi.h"


//function:		通讯建立
//return:		0：执行成功；1：执行失败
extern "C" typedef int( *pToIM_CommOpen)(const char *sCommPort, uint uiBaudRate);


//function:		关闭通讯
//return:		0：执行成功；1：执行失败
extern "C" typedef int( *pToIM_CommClose)(void);


//function:		模块初始化（设备开机或复位后需要调用本函数）
//parameter:	ucAcionMode(IN 1：通道清票；2：通道不清票)
//				ucErrorCode(OUT 返回错误码)	
//				xModuleStatus(OUT 返回设备状态)	
//				ucTokenNum(OUT 回收卡数量)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_Init)(uchar ucActionMode,uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar &ucTokenNum);


//function:		读取设备状态（获取读写区是否有卡，票箱状态、票箱数量以及各传感器实时状态）
//parameter:	ucErrorCode(OUT 返回错误码)	
//				xModuleStatus(OUT 返回设备状态)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_GetDevStatus)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus);


//function:		token传送至天线区
//parameter:	ucBoxNo(IN 1：A票箱；2：B票箱)
//				ucErrorCode(OUT 返回错误码)	
//				xModuleStatus(OUT 返回设备状态)	
//				ucTokenNum(OUT 获取的token数量)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_TokenToAntenna)(uchar ucBoxNo, uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar & ucTokenNum);


//function:		token传送至出票口
//parameter:	ucErrorCode(OUT 返回错误码)	
//				xModuleStatus(OUT 返回设备状态)	
//				ucTokenNum(OUT 本次操作发送的卡片数)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_TokenToOutlet)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar & ucTokenNum);


//function:		token传送至回收箱
//parameter:	ucErrorCode(OUT 返回错误码)	
//				xModuleStatus(OUT 返回设备状态)	
//				ucTokenNum(OUT 本次回收的卡片数)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_TokenRetrieve)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleStatus & xModuleStatus, uchar & ucTokenNum);


//function:		模块复位（重新启动发售模块的控制程序，在设备故障或者从维修模式退出时（对于支持维修模式的情况下），执行本函数）
//parameter:	ucErrorCode(返回错误码)	
//return:		0：执行成功；1：执行失败
//ps:			's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *poTIM_Reset)(uchar &ucErrorCode);


//function:		读取设备驱动版本信息（获取设备驱动版本号、固件版本号(如发售模块的 ID 等)、状态信息）
//parameter:	ucErrorCode(返回错误码)	
//				xModuleInfo（驱动信息结构）
//return:		0：执行成功；1：执行失败
//ps:			's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_ReadVersion)(uchar & ucErrorCode, TOKEN_NAMESPACE::ModuleInfo & xModuleInfo);


//function:		清空票箱（对设备内的token进行清空）
//parameter:	ucBoxNo(IN 1：A票箱；2：B票箱)
//				ucErrorCode(OUT 返回错误码)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_CleanOut)(uchar cBoxNo, uchar & ucErrorCode);


//function:		停止清空票箱
//parameter:	ucBoxNo(IN 1：A票箱；2：B票箱)
//				ucErrorCode(OUT 返回错误码)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_StopCleanOut)(uchar cBoxNo, uchar & ucErrorCode);


//function:		读取票箱清空数量
//parameter:	ucBoxNo(IN 1：A票箱；2：B票箱)
//				ucErrorCode(OUT 返回错误码)	
//				usCoNum(OUT 清除票箱Token卡数量）
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_GetNumOfCleanOut)(uchar cBoxNo, uchar & ucErrorCode,ushort & usCoNum);


//function:		设置块RFID信息
//parameter:	ucRfidNo(IN RFID端口号 0x04：A票箱；0x03：B票箱；0x05：清票箱；0x06：废票箱)
//				ucBlockNo(IN Block号 取值范围为： 8,9,10 / 12,13,14 / 16,17,18 / 20,21,22 / 24,25,26 / 28,29,30 / 32,33,34 / 36,37,38 / 40,41,42 / 44,45,46 / 48,49,50 / 52,53,54 / 56,57,58 / 60,61,62)
//				xBlockData(IN 块结构体)
//				ucErrorCode(OUT 返回错误码)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_RfidWriteBlock)(uchar ucRfidNo, uchar ucBlockNo,TOKEN_NAMESPACE::BlockData xBlockData, uchar &ucErrorCode);


//function:		读取块RFID信息
//parameter:	ucRfidNo(IN RFID端口号 0x04：A票箱；0x03：B票箱；0x05：清票箱；0x06：废票箱)
//				ucBlockNo(IN Block号 取值范围为： 8,9,10 / 12,13,14 / 16,17,18 / 20,21,22 / 24,25,26 / 28,29,30 / 32,33,34 / 36,37,38 / 40,41,42 / 44,45,46 / 48,49,50 / 52,53,54 / 56,57,58 / 60,61,62)
//				xBlockData(IN 块结构体)
//				ucErrorCode(OUT 返回错误码)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_RfidReadBlock)(uchar ucRfidNo, uchar ucBlockNo, uchar &ucErrorCode,TOKEN_NAMESPACE::BlockData &xBlockData);


//function:		设置扇区RFID信息
//parameter:	ucRfidNo(IN RFID端口号 0x04：A票箱；0x03：B票箱；0x05：清票箱；0x06：废票箱)
//				ucSectorNo(IN Sector号： 取值范围为： 2~15 )
//				xSectorData(IN 写入数据（变长）)
//				ucLen(IN 写入数据的长度，最多为48字节)
//				ucErrorCode(OUT 返回错误码)	
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_RfidWriteSector)(uchar ucRfidNo, uchar ucSectorNo, uchar ucLen,TOKEN_NAMESPACE::SectorData xSectorData,uchar &ucErrorCode);


//function:		读取扇区RFID信息
//parameter:	ucRfidNo(IN RFID端口号 0x04：A票箱；0x03：B票箱；0x05：清票箱；0x06：废票箱)
//				ucSectorNo(IN Sector号： 取值范围为： 2~15 )
//				ucErrorCode(OUT 返回错误码)	
//				xSectorData(IN 读出数据（固定48字节）)
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_RfidReadSector)(uchar ucRfidNo, uchar ucSectorNo, uchar &ucErrorCode,TOKEN_NAMESPACE::SectorData &xSectorData);


//function:		读取RFID卡的物理编号
//parameter:	ucRfidNo(IN RFID端口号 0x04：A票箱；0x03：B票箱；0x05：清票箱；0x06：废票箱)
//				ucErrorCode(OUT 返回错误码)	
//				uiRfidID(OUT 读取物理编号： 读取到的 S50 卡的物理编号（ 4 字节）)
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_RfidReadID)(uchar ucRfidNo, uchar &ucErrorCode, uchar* uiRfidID);


//function:		传感器检查
//parameter:	ucErrorCode(OUT 返回错误码)	
//				sensorData(OUT 3个字节传感器信息）
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_SensorCheck)(uchar &ucErrorCode, char *sensorData);


//function:		读取Hopper版本
//parameter:	ucErrorCode(OUT 返回错误码)	
//				xModuleInfo(OUT Hopper版本32个字节 见结构体说明 ）
//return:		's'：执行成功；'e'：执行失败；'w'：警告
extern "C" typedef int( *pToIM_GetHopperVersion)(uchar &ucErrorCode, TOKEN_NAMESPACE::HopperModuleInfo &xModuleInfo);


/** 
@brief   TOKEN command 类
*/
class TOKENCOMLIB_DECLSPEC CTOKENCommands : public CCommand
{		
public:
	CTOKENCommands();
	~CTOKENCommands();

	// 雷格特API封装
	virtual long CommOpen(UINT uiCommPort, UINT uiBaudRate);
	virtual long CommClose(); 
	virtual long Init(uchar ucActionMode, uchar &ucRetriveNum);
	virtual long Reset(); 
	virtual long GetVersion(TOKEN_NAMESPACE::ModuleInfo & xModuleInfo);
	virtual long GetDevStatus(TOKEN_NAMESPACE::ModuleStatus& xModuleStatus);

	virtual long CardOut(uchar ucBoxNo, uchar & ucTokenNum);
	virtual long SendCard(uchar & ucTokenNum);
	virtual long RetractCard(uchar & ucTokenNum);

	virtual long CleanOut(uchar cBoxNo);
	virtual long StopCleanOut(uchar cBoxNo);
	virtual long GetCleanOutOfNum(uchar cBoxNo, ushort &usCoNum);

	virtual long RfidWriteBlock(uchar ucRfidNo, uchar ucBlockNo,TOKEN_NAMESPACE::BlockData xBlockData);
	virtual long RfidReadBlock(uchar ucRfidNo, uchar ucBlockNo,TOKEN_NAMESPACE::BlockData& xBlockData);
	virtual long RfidWriteSector(uchar ucRfidNo, uchar ucSectorNo, uchar ucLen, TOKEN_NAMESPACE::SectorData xSectorData);
	virtual long RfidReadSector(uchar ucRfidNo, uchar ucSectorNo, TOKEN_NAMESPACE::SectorData xSectorData);
	virtual long RfidReadID(uchar ucRfidNo, uchar* uiRfidID);

	virtual long SensorCheck(char* sSensorData);

	virtual long GetHopperVersion(TOKEN_NAMESPACE::HopperModuleInfo& xModuleInfo);

public:
	long DealReturnCode(long retCode, uchar ucErrorCode);

	HINSTANCE hInst;
	pToIM_CommOpen TIM_CommOpen;
	pToIM_CommClose TIM_CommClose;
	pToIM_Init TIM_Init;
	poTIM_Reset TIM_Reset;
	pToIM_ReadVersion TIM_GetVersion;
	pToIM_GetDevStatus TIM_GetDevStatus;
	pToIM_TokenToAntenna TIM_CardOut;
	pToIM_TokenToOutlet TIM_SendCard;
	pToIM_TokenRetrieve TIM_RetractCard;
	pToIM_CleanOut TIM_CleanOut;
	pToIM_StopCleanOut TIM_StopCleanOut;
	pToIM_GetNumOfCleanOut TIM_GetNumOfCleanOut;
	pToIM_RfidWriteBlock TIM_RfidWriteBlock;
	pToIM_RfidReadBlock TIM_RfidReadBlock;
	pToIM_RfidWriteSector TIM_RfidWriteSector;
	pToIM_RfidReadSector TIM_RfidReadSector;
	pToIM_RfidReadID TIM_RfidReadID;
	pToIM_SensorCheck TIM_SensorCheck;
	pToIM_GetHopperVersion TIM_GetHopperVersion;

protected:
	virtual long ExecuteCommand();
	virtual long IsValidCommand();

};
#endif