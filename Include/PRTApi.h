#ifndef _PRT_API_H
#define _PRT_API_H

#include "Apidef.h"

#define		PRT_ERR_LESS_PAPER			1611		//打印机少纸
#define		PRT_ERR_WARING				1612		//警告错误
#define		PRT_ERR_INVALID_CMD			1651		//无效命令
#define		PRT_ERR_NO_PAPER			1652		//打印机无纸
#define		PRT_ERR_CUT_ERROR			1653		//打印切纸错
#define		PRT_ERR_ERROR				1654		//打印机故障
#define		PRT_ERR_STUCK_PAPER			1655		//卡纸
#define		PRT_ERR_PORT_ERR			1656		//串口配置错
#define		PRT_ERR_TIME_OUT			1657		//通讯超时
#define		PRT_ERR_PARAM_ERR			1658		//传入参数错误

#pragma pack(push,1)

//错误与状态描述结构体 
typedef  struct 
{ 
	//1.基本信息
	UINT uiProviderId; //模块厂家标识。三星0x02、广电0x04、中软0x06、欧姆龙0x05；
	UINT uiModuleType; //模块类型标识 0x07打印机模块
	UINT uiType; //错误类型：0-正常；1-警告；2-故障
	UINT uiStdErrorCode[16];  //统一维护的错误代码
	UINT uiErrorCode[16];  //错误代码 （由不同模块厂家提供）  
	char acDevReturn[128]; //  硬件返回信息 
	char acReserve[128]; //  保留信息    
} tPRTDevReturn;

//模块状态结构体 

typedef  struct 
{ 
	UINT uiPaperStatus;   //打印纸状态：0-正常；1-少纸；2-无纸
	UINT uiModuleStatus;  //模块状态：0-正常；1:-故障
} tPRTModuleStatus;


#pragma pack(pop)
#ifdef __cplusplus
extern "C" {
#endif

	//初始化模块，检测模块各硬件、将模块恢复到初始状态等,设备开机后需要调用本函数
	DLLIMP int  PRT_Init(tPRTDevReturn*  pDevStatus);
	//在设备故障或从维修模式退出时（对于支持维修模式的情况），执行本函数
	DLLIMP int  PRT_Reset(tPRTDevReturn*  pDevStatus);
	//功能说明：此命令用于将输入的字符串进行打印。
	//参数说明见下表： 
	//	表248　PRT_Print参数说明 
	//	序号	参数类型	参数名称	属性	说明
	//	1	Char*	cPrintData	In	需打印的数据。必须/0结尾。
	//	2	bool	bCutPaper	In	是否切纸。true：切纸；false：不切纸。
	//	3	tPRTDevReturn *	pDevStatus 	Out	返回状态信息 
	DLLIMP int  PRT_Print(CStringArray* m_sArrPrinter,bool m_bOpenBox,bool bCutPaper,tPRTDevReturn *pDevStatus);
	//功能说明：调用此接口可获取模块当前状态。
	//	参数说明见下表： 
	//	表250　PRT_GetDevStatus ()参数说明 
	//	序号	参数类型	参数名称	属性	说明
	//	1	tPRTModuleStatus *	pModuleStatus 	Out	返回模块状态信息 
	//	2	tPRTDevReturn *	pDevStatus 	Out	返回状态信息 
	DLLIMP int  PRT_GetDevStatus(tPRTModuleStatus *pModuleStatus,tPRTDevReturn *pDevStatus);

	//功能说明：获取设备驱动版本号、固件版本号、状态信息。
	//	参数说明见下表： 
	//	表252　PRT_GetVersion ()参数说明 
	//	序号	参数类型	参数名称	属性	说明
	//	1	char	cFirmWareVersion[32]	out	返回固件版本号
	//	2	char	cDriverVersion[32]	out	返回驱动版本号
	//	3	tPRTDevReturn *	pDevStatus 	Out	返回状态信息 
	DLLIMP int  PRT_GetVersion(char cFirmWareVersion[32], char cDriverVersion[32], tPRTDevReturn *pDevStatus);

	DLLIMP int  PRT_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName);
	DLLIMP int  PRT_CommClose();

#if defined (__cplusplus)
} // end of extern "C"
#endif

#endif
