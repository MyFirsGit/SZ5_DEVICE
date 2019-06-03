//纸币找零模块
#ifndef _BND_API_H
#define _BND_API_H

#include "Apidef.h"

#pragma pack(push,1)

//　返回状态结构体 
typedef  struct 
{
	UINT uiType;	/*错误类型：0-正常；1-警告；2-故障*/	
	UINT uiErrorType;/*故障类型：0-正常；1-命令类错误；2-硬件故障；3-通道异常；4-钱箱故障*/
	int  iHandle;	/*处理方法：0-不处理 1-初始化 2-重发命令 3-停机休息 4-软件和硬件重新配置*/
	int  iStdErrorCode;     /*统一维护的错误代码*/		
	UINT uiErrorCode;/*错误代码（由不同模块厂家提供）*/
	char cReverse[128]; /*预留信息*/
} tBNDDevReturn;

//　初始化张数信息
typedef struct
{
	UINT uiOutNumber;
	UINT uiRetractNumber;
}tBndInitNumInfo;

#ifndef MAX_BOXNUM
#define MAX_BOXNUM 6
#endif
//　钱箱状态结构体
typedef struct
{
	unsigned char uzBoxStatus[MAX_BOXNUM];
	unsigned char ucNgBoxStatus;
}tBndBoxStatus;

//　设置钱箱结构体 
typedef struct
{
	UINT uiBoxType;   //容器类型：0-废币箱 1-1元纸币箱, 2-5元纸币箱, 3-10元纸币箱, 4-20元纸币箱, 5-50元纸币箱, 6-100元纸币箱*
	UINT uiBoxNo;     /**容器序号：0 ~ 6 其中： 0号废币箱 1号为5元找零箱，2号为1元找零箱 */
	UINT uiBillValue; /**单位分：容器中所能存放纸币的面额，期中0表示不能确定该容器的面额*/
	UINT uiBillNum;   /**容器中纸币的数量*/
	UINT uiBillMaxNum;/**容器中能存入纸币最大的数量*/
	UINT uiBoxStatus; /**容器的状态：0-正常；1-未安装；2-已满；3-将满；4-以空；5-将空,该状态为模块硬件检测的状态，通过计数判断的状态由上位机进行判断*/
} tBndCashBoxValueInfo;
//　模块容器配置信息结构体
typedef  struct
{
	tBndCashBoxValueInfo pCashBoxValueInfo[MAX_BOXNUM];
	tBndCashBoxValueInfo stCashNGBoxValueInfo;;
}tBNDBillBoxInfo;

//　配钞结构体
typedef struct
{
	UINT uiAmount;				      //总数
	UINT uiDispenseNumber[MAX_BOXNUM];//每个钱箱需要出钱的张数
	UINT uiOutNumber[MAX_BOXNUM];    //实际每个钱箱出钱的张数（返回）
	UINT uiRejectNumber[MAX_BOXNUM]; //出钞过程中每个钱箱废钞回收的张数（返回）
} tBNDChangeNum;
//　审计结构体
typedef struct
{
	UINT uiNGBoxAccInNum;       //累计进入废钞箱张数
	UINT uiBoxAccOutNum;       //累计出钞张数
}tBndAuditCashNum;

//　Rfid信息结构体
typedef struct
{
	unsigned char uzCashBoxID[4]; // 钱箱编号
	unsigned char uzOperateID[3]; // 操作员ID
	unsigned char uzDeviceID[4];// 设备ID
	unsigned char ucCashBoxStatus;// 钱箱位置状态
	unsigned char ucCashBoxOperateStatus;// 操作后钱箱状态
	unsigned char ucBillType;// 币种代码
	unsigned char usBillCnt;// 数量
	unsigned int  uiBillAmount;// 纸币总金额
	unsigned char ucStationCode[2];// 车站编码
	unsigned char ucInstallPosit;// 安装位置
	unsigned int  uiBlockOperateTime;// 块操作时间
	unsigned char ucOperateTime[7];// 最后操作时间
	unsigned short usCheckField;// 校验字段
}tBndRfidInfo;

#pragma pack(pop)

/*****************************************************************************/
/*                                                                           */
// 以下接口在LIB实现，在DLL层使用，如果厂家提供dll则可仿造TIM模块实现，如果厂家
// 提供串口协议，则需封装完协议后再实现以下接口
/* # wang feng                                                               */
/*****************************************************************************/

//　通信串口打开
// 	功能说明：调用此接口可打开低层串口通信。
// 	参数说明见下表。
// 	1	unsigned int	uiCommPort	In	串口通信号
// 	2	unsigned int	uiBaudRate	In	波特率
// 	3	tBNDDevReturn	pDevStatus	Out	设备返回状态
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	串口打开出错
DLLIMP int BND_CommOpen(UINT uiCommPort, UINT uiBaudRate, tBNDDevReturn *pDevStatus);

//　通信串口关闭
// 	功能说明：调用此接口可关闭低层串口通信。
// 	参数说明参见下表
// 	1	tBNDDevReturn	pDevStatus	Out	设备返回状态
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	关闭错误
DLLIMP int BND_CloseComm(tBNDDevReturn *pDevStatus);	


//　初始化
// 	功能说明： 初始化模块，检测模块各硬件、将模块恢复到初始状态等。
// 	参数说明见下表。
// 	1	tBndInitNumInfo *	pNumInfo	Out	退出和回收钱币信息
// 	2	tBNDDevReturn	pDevStatus	Out	            设备返回状态
// 
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int  BND_Init(tBndInitNumInfo *pNumInfo, tBNDDevReturn * pDevStatus); 

//　复位
// 	功能说明：复位。
// 	参数说明见下表。
// 	1	tBndInitNumInfo*	pNumInfo	Out	退出和回收钱币张数
// 	2	tBNDDevReturn	pDevStatus	Out	设备返回状态
// 
// 	返回值说明见下表。
DLLIMP int BND_Reset(tBndInitNumInfo *pNumInfo, tBNDDevReturn *pDevStatus);

//　获取模块状态
// 	功能说明：调用此接口可获取模块当前状态，详细信息查看返回结构体。
// 	参数说明见下表。
// 	1	tBndBoxStatus	pBndBoxStatus	Out	返回钱箱状态
// 	2	tBNDDevReturn *	tBNDDevReturn	Out	保存BND返回的逻辑错误码
// 
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BND_GetDevStatus(tBndBoxStatus *pBndBoxStatus, tBNDDevReturn * tBNDDevReturn);

//　读取审计信息
// 	功能说明：读取设备的审计信息。
// 	参数说明见下表。
// 	1	tBndAuditCashNum *	pBndAuditCashNum	Out	容器配置信息
// 	2	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_GetAuditData(tBndAuditCashNum *pBndAuditCashNum, tBNDDevReturn *pDevStatus);

//　读取容器配置信息
// 	功能说明：读取钱箱结构体信息。
// 	参数说明见下表。
// 	1	tBNDBillBoxInfo *	pBillBoxInfo	Out	返回钱箱结构体信息
// 	2	tBNDDevReturn *	pDevStatus	Out	返回状态信息

// 	返回值见表
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_GetBillBoxInfo(tBNDBillBoxInfo *pBillBoxInfo, tBNDDevReturn *pDevStatus);


//　设置容器配置信息
// 	功能说明：设置容器配置信息（设置纸币数量）
// 	参数说明见下表。
// 	1	UNIT	uiSlotID	In	槽位ID
// 	2	tBNDBillBoxInfo *	pBillBoxInfo	In	容器配置信息
// 	3	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_SetBillBoxInfo(UINT uiSlotID, tBNDBillBoxInfo *pBillBoxInfo, tBNDDevReturn *pDevStatus);


//　找零配钞
// 	功能说明：找零配钞
// 	参数说明见下表。
// 	1	tBNDChangeNum *	pChangeNum	In	找零的纸币面额及数量信息
// 	2	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_ChangeBill(tBNDChangeNum *pChangeNum, tBNDDevReturn *pDevStatus);

//　开始更换纸币找零箱
// 	功能说明：此接口用于更换纸币找零箱。
// 	参数说明见下表。
// 	表28　BND_StartReplaceBox()参数说明
// 	序号	参数类型	参数名称	输入/输出	说明
// 	1	UNIT	uiBoxType	In	指定纸币容器的类型
// 	2	UINT	uiBoxNo	In	制定纸币容器的序号
// 	3	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值见下表。
// 	表29　BND_StartReplaceBox()返回值说明
// 	序号	类型	说明
// 	1	int	0：执行成功；1：执行失败。
DLLIMP int BND_StartReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBNDDevReturn *pDevStatus);

//　停止更换纸币找零箱
// 	功能说明：停止更换纸币找零箱。
// 	参数说明见下表。
// 	1	UNIT	uiBoxType	In	指定纸币容器类型
// 	2	UNIT	uiBoxNo	In	指定纸币容器序号
// 	2	tBNDDevReturn *	p_psStatus	Out	保存BND返回的逻辑错误码
// 
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BND_StopReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBNDDevReturn *pDevStatus);

//　读取序列号
// 	功能说明：此命令用于模块的序列号。
// 	参数说明见下表。
// 	1	Unsigned char*	pSn	Out	模块的序列号，长度32
// 	2	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_GetSerialNumber(unsigned char * pSn, tBNDDevReturn * pDevStatus);

//　读取固件编号
// 	功能说明：此命令用于模块的固件编号。
// 	参数说明见下表。
// 	1	Unsigned char*	pVersion	Out	模块的固件编号
// 	2	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_GetFWVerdion (unsigned char * pVersion, tBNDDevReturn * pDevStatus);

//　取消找零
// 	功能说明：取消找零操作。
// 	参数说明见下表。
// 	1	tBNDDevReturn *	p_psStatus	Out	保存BND返回的逻辑错误码
// 
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int  BND_Cancel (tBNDDevReturn *p_psStatus);

//　设置钱箱RFID信息
// 	功能说明：写入钱箱RFID信息。
// 	参数说明见下表。
// 	1	UNIT	uiSLotID	In	槽位ID
// 	2	tBndRfidInfo *	pRFIDInfo	In	设置RFID信息
// 	3	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_WriteRFIDInfo(UINT uiSLotID, tBndRfidInfo* pRFIDInfo, tBNDDevReturn * pDevStatus);

//　读取钱箱RFID信息
// 	功能说明：读取钱箱RFID信息。
// 	参数说明见下表。
// 	1	UNIT	uiSLotID	In	槽位ID
// 	2	tBndRfidInfo *	pRFIDInfo	Out	设置RFID信息
// 	3	tBNDDevReturn *	pDevStatus	Out	返回状态信息
// 
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BND_ReadRFIDInfo(UINT uiSLotID, tBndRfidInfo* pRFIDInfo, tBNDDevReturn * pDevStatus);

#endif
