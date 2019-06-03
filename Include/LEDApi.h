#ifndef _LED_API_H_
#define _LED_API_H_
#pragma once
/*****************************************************************************/
/*                                                                           */
/*  Include Files                                                            */
/*                                                                           */
/*****************************************************************************/
// apidef.h中定义了接口的导入/导出
//#include "Apidef.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __UINT
#define __UINT
typedef unsigned int UINT;
#endif

#ifndef __BYTE
#define __BYTE
typedef unsigned char BYTE;
#endif

#ifndef uchar
#define uchar	unsigned char	// 1 byte
#endif

#define				LED_NO_ERROR				1800		// 状态正常
#define				LED_ERR_INVALID_CMD			1851		// 无效命令
#define				LED_ERR_HARDWARE_ERR		1852		// 故障
#define				LED_ERR_COMPORT_ERR			1853		// 串口配置错误
#define				LED_ERR_TIME_OUT			1854		// 通讯超时
#define				LED_ERR_INVALID_PARA		1855		// 传入参数错误

//#pragma pack(push,1)

//11.3.1　错误与状态描述结构体 
typedef  struct 
{
	//1.基本信息
	UINT uiProviderId; //模块厂家标识。三星0x02、广电0x04、中软0x06、欧姆龙0x05；
	UINT uiModuleType; //模块类型标识 0x09 LED模块
	UINT uiType; //错误类型：0-正常；1-警告；2-故障
	UINT uiStdErrorCode[16];  //统一维护的错误代码
	UINT uiErrorCode[16];  //错误代码 （由不同模块厂家提供）  
	char acDevReturn[128]; //  硬件返回信息 
	char acReserve[128]; //  保留信息
} tLEDDevReturn;

//11.3.2　模块状态结构体 
typedef  struct 
{
	UINT uiModuleStatus;  //模块状态：0-正常；1:-故障
} tLEDModuleStatus;


//11.3.4　显示内容结构体 
typedef  struct 
{
	UINT 		nSerialNO;	//序号
	char	   	cCNText[256];		// 需要显示的中文
	char	   	cENText[256];		// 需要显示的英文
	UINT		iFontColor;// 字体颜色。0 ~ 6 分别对应 红、绿、黄、蓝、紫、青、白等颜色。
	UINT		iStayTime;// 停留时间。停留时间，0 ~ 9，0-最短，9-最长，默认值为5。如果中文/英文一屏可以显示时，此参数有效，同时居中显示。其他情况可采用滚动播放。如果中文/英文可一屏显示，中英文切换时采用立即消失。滚动播放时采用从由右向左方向滚动播放。
} tLED_Download_Text_Para;
//11.3.3　下载内容结构体 
typedef  struct 
{
	UINT 		uiTextNum;		//需要显示的记录条数
	tLED_Download_Text_Para* pLED_Download_Text_Para;
} tLED_Download_Para;

#pragma pack(pop)
	//11.4.1　通信建立
	 int  LED_CommOpen(UINT uiCommPort, UINT uiBaudRate);
	//11.4.2　初始化
	 int  LED_Init(tLEDDevReturn*  pDevStatus);
	//11.4.3　模块复位
	 int  LED_Reset(tLEDDevReturn*  pDevStatus);
	//11.4.4　下载显示信息
	//此命令用于将需要显示的信息下载到LED控制板上或保存在内存变量中
	 int  LED_Download(tLED_Download_Text_Para pLED_Download_Para,tLEDDevReturn *pDevStatus);
	//11.4.5　显示信息
	//此命令用于根据选择的序号将保存在LED控制板上或内存变量中的字符串内容进行显示。
	//要求显示内容切换时，需立即显示
	//UINT	nSerialNO	In	需显示的数据编号。与tLED_Download_Text_Para:: nSerialNO对应。
	 int  LED_Display(UINT nSerialNO, tLEDDevReturn * pDevStatus);
	//11.4.6　获取模块状态
	 int  LED_GetDevStatus(tLEDModuleStatus *pModuleStatus,tLEDDevReturn *pDevStatus);
	//11.4.7　获取模块驱动版本信息
	 int LED_GetVersion(char* cFirmWareVersion, char* cDriverVersion, tLEDDevReturn *pDevStatus);
	//11.4.8　通信关闭
	 int LED_CommClose();
#if defined (__cplusplus)
} // end of extern "C"
#endif




#endif