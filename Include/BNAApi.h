//纸币接收模块
#ifndef _BNA_API_H
#define _BNA_API_H

#include "Apidef.h"


#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1)
//　错误与状态描述结构体 
typedef  struct 
{
	//基本信息
	UINT uiProviderId; //模块厂家标识
	int iType;   //错误类型：0-正常；1-警告；2-故障
	int iStdErrorCode;  //统一维护的错误代码
	//2.模块的原始信息，供上位程序显示维护用 
	int iErrorCode;  //错误代码 （由不同模块厂家提供）  
	char acDevReturn[128]; //  硬件返回信息 
	char acReserve[128]; //  保留信息    
} tBNASTDDevReturn;	
//　钞票系列禁用信息结构体
typedef struct _SetInhibitItem
{
	int  iValue;    //面额，单位为分
	bool  bAccepted; //是否接收  
}tBNASetInhibitItem;
typedef struct  _SetInhibitList
{ 
	int iNumOfItems; //现金体个数
	tBNASetInhibitItem  *psBnaCashInfo;//现金数组基址指针
} tBNASetInhibitList;
//　现金信息结构体
typedef struct _BnaCashItem
{
	unsigned int  iValue; //接收钞票面额，单位为分
	unsigned int  iCount; //钞票张数
}tBNACashItem;
typedef struct _BnaCashInfo
{
	int iNumOfItems; //现金体个数
	tBNACashItem  *psBnaCashInfo;
}tBNACashInfo;
//　纸币接收钱箱状态结构体
typedef struct _BnaStatus
{
	int iModuleStatus; /* 0 正常，1 需复位 2 异常 */
	int iBoxStatus;  /* 0 安装（正常）， 1 将满 2 满 3 未安装 */
	int iInStacker; /* 0 无， 1 有钞票在暂存 */
}tBNAStatus;

#pragma pack(pop)

/*****************************************************************************/
/*                                                                           */
// 以下接口在LIB实现，在DLL层使用，如果厂家提供dll则可仿造TIM模块实现，如果厂家
// 提供串口协议，则需封装完协议后再实现以下接口
/* # wang feng                                                               */
/*****************************************************************************/
//　初始化
DLLIMP int BNA_Init(tBNASTDDevReturn*  p_psStatus);
//　设置识别可用钞票
DLLIMP int BNA_SetPrevalue(tBNASetInhibitList *p_psBuyTicketSetInhibitList, tBNASetInhibitList *p_psRechargeSetInhibitList, tBNASTDDevReturn * p_psStatus);
//　复位
DLLIMP int BNA_Reset(int iMode, int iRetryTimes, BYTE * p_byNotesRefunded, BYTE * p_byNotesEncashed,tBNASTDDevReturn* p_psStatus);
//　开始接收钞票
DLLIMP int BNA_Start(tBNASTDDevReturn * p_psStatus);
//　停止接收钞票
DLLIMP int BNA_Stop(tBNASTDDevReturn * p_psStatus);
//  获取识别钞票信息
DLLIMP int BNA_GetCashInfo(tBNACashInfo *p_psCashInfo, BYTE& p_byOperation, tBNASTDDevReturn *p_psStatus);
//　压箱
DLLIMP int BNA_Encash(tBNASTDDevReturn *p_psStatus);
//退钞
DLLIMP int BNA_Refund(tBNASTDDevReturn *p_psStatus);
//  获取模块状态
DLLIMP int BNA_GetStatus(tBNAStatus *p_psBnaStatus,tBNASTDDevReturn * p_psStatus);
//	获取钱箱ID
DLLIMP int BNA_GetBoxID(char * p_BoxID, tBNASTDDevReturn * p_psStatus);
//　通信串口打开
DLLIMP int BNA_open(unsigned int uiCommPort, unsigned int uiBaudRate);
//　通信串口关闭
DLLIMP int BNA_close();	
//　获取模块固件版本
DLLIMP int BNA_GetVersion(char* pVersion, tBNASTDDevReturn * p_psStatus);

#if defined (__cplusplus)
} // end of extern "C"
#endif

#endif



