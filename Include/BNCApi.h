//纸币循环模块
#ifndef _BNC_API_H
#define _BNC_API_H

#include "Apidef.h"

#define MAX_BOXNUM 6

#pragma pack(push,1)


   // error code
#define BNC_ERR_CODE_NONE               0     // No error

//　结构体说明
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
} tBncDevReturn;

// 设备详细状态
typedef struct tBncSysDevStatus
{
	bool bIsCollectBoxInstalled;
	bool bIsChangeBoxInstalled;
	tBncSysDevStatus(){
		bIsChangeBoxInstalled = true;
		bIsCollectBoxInstalled = true;
	}
};

//　初始化张数信息
typedef struct
{
	UINT uiOutNumber;//退出张数
	UINT uiRetractNumber;//回收张数
}tBncInitNumInfo;

//　钞票系列禁用信息结构体
typedef struct tBncSetInhibitItem
{
	int  iValue;    //面额，单位为分
	bool  bAccepted; //是否接收  
	tBncSetInhibitItem(){
		bAccepted = false;
	}
};
typedef struct tBncSetInhibitList
{ 
	int iNumOfItems; //现金体个数
	tBncSetInhibitItem  *psBncCashInfo;//现金数组基址指针
	tBncSetInhibitList(){
		iNumOfItems = 0;
		psBncCashInfo = NULL;
	}
};

//　现金信息结构体
typedef struct _BncCashItem
{
    unsigned int  iValue; //接收钞票面额，单位为分
    unsigned int  iCount; //钞票张数
}tBncCashItem;
typedef struct _BncCashInfo
{
    int iNumOfItems; //现金体个数
    tBncCashItem  *psBncCashInfo;
	_BncCashInfo(){
		iNumOfItems = 0;
		psBncCashInfo = NULL;
	}
}tBncCashInfo;
// 押钞反馈
typedef struct tBncEnCashInfo{
	DWORD dwCashIntoAmount;

	WORD wRecyclerAIntoCount;
	WORD wRecyclerBIntoCount;
	WORD wRecyclerCIntoCount;
	WORD wRecyclerDIntoCount;

	WORD wCashBox_CNY1_IntoCount;
	WORD wCashBox_CNY5_IntoCount;
	WORD wCashBox_CNY10_IntoCount;
	WORD wCashBox_CNY20_IntoCount;
	WORD wCashBox_CNY50_IntoCount;
	WORD wCashBox_CNY100_IntoCount;

	tBncEnCashInfo(){
		dwCashIntoAmount = 0;

		wRecyclerAIntoCount = 0;
		wRecyclerBIntoCount = 0;
		wRecyclerCIntoCount = 0;
		wRecyclerDIntoCount = 0;

		wCashBox_CNY1_IntoCount = 0;
		wCashBox_CNY5_IntoCount = 0;
		wCashBox_CNY10_IntoCount = 0;
		wCashBox_CNY20_IntoCount = 0;
		wCashBox_CNY50_IntoCount = 0;
		wCashBox_CNY100_IntoCount = 0;
	}

};//押钞反馈
//　纸币钱箱状态结构体
typedef struct 
{
	int iModuleStatus; /* 0 正常，1 需复位 2 异常 */
	int iBoxStatus;  /* 0 安装（正常）， 1 将满 2 满 3 未安装 */
	int iInStacker; /* 0 无， 1 有钞票在暂存 */
}tBncStatus;

//　设置钱箱结构体 
typedef struct
{
	UINT uiBoxType;   //容器类型：0-废币箱 1-1元纸币箱, 2-5元纸币箱, 3-10元纸币箱, 4-20元纸币箱, 5-50元纸币箱, 6-100元纸币箱*
	UINT uiBoxNo;     /**容器序号：0 ~ 6 其中： 0号废币箱 1号为5元找零箱，2号为1元找零箱 */
	UINT uiBillValue; /**单位分：容器中所能存放纸币的面额，期中0表示不能确定该容器的面额*/
	UINT uiBillNum;   /**容器中纸币的数量*/
	UINT uiBillMaxNum;/**容器中能存入纸币最大的数量*/
	UINT uiBoxStatus; /**容器的状态：0-正常；1-未安装；2-已满；3-将满；4-已空；5-将空,该状态为模块硬件检测的状态，通过计数判断的状态由上位机进行判断*/
} tBncCashBoxValueInfo;

//　模块容器配置信息结构体
typedef  struct
{
	tBncCashBoxValueInfo pCashBoxValueInfo[MAX_BOXNUM];
	tBncCashBoxValueInfo stCashNGBoxValueInfo;
}tBncBillBoxInfo;

//　配钞结构体/清空钱箱反馈
typedef struct
{
	UINT uiAmount;				      //总数
	UINT uiDispenseNumber[MAX_BOXNUM];//每个钱箱需要出钱的张数
	UINT uiOutNumber[MAX_BOXNUM];    //实际每个钱箱出钱的张数（返回）依次是：循环1.循环2，循环3，循环4，补充箱
	UINT uiRejectNumber[MAX_BOXNUM]; //出钞过程中每个钱箱废钞回收的张数（返回）
} tBncChangeNum;
//　审计结构体
typedef struct
{
	UINT uiNGBoxAccInNum;       //累计进入废钞箱张数
	UINT uiBoxAccOutNum;       //累计出钞张数
} tBncAuditCashNum;
//　RFID信息结构体
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
}tBncRfidInfo;
#pragma pack(pop)

/*****************************************************************************/
/*                                                                           */
// 以下接口在LIB实现，在DLL层使用，如果厂家提供dll则可仿造TIM模块实现，如果厂家
// 提供串口协议，则需封装完协议后再实现以下接口
/* # wang feng                                                               */
/*****************************************************************************/
//　通信串口打开
// 功能说明：调用此接口可打开低层串口通信。
// 参数说明见表23。
// 1	unsigned int	uiCommPort	In	串口通信号
// 2	unsigned int	uiBaudRate	In	波特率
// 返回值说明
// 1	Int	0	成功
// 2	Int	1	串口打开出错
DLLIMP int  BNC_open(unsigned int uiCommPort, unsigned int uiBaudRate);

//　初始化
// 功能说明： 初始化模块，检测模块各硬件、将模块恢复到初始状态等。
// 参数说明见下表。
// 1	tBncInitNumInfo *	pNumInfo	Out	退出和回收钱币信息
// 2	tBNDDevReturn	pDevStatus	Out	            设备返回状态
// 返回值说明见下表。
// 1	Int	0	成功
// 2	Int	1	传入参数为空
DLLIMP int BNC_Init(tBncInitNumInfo *pNumInfo, tBncDevReturn * pDevStatus);

//　通信串口关闭
//  功能说明：调用此接口可关闭低层串口通信。
// 	返回值说明
// 	1	Int	0	成功
// 	2	Int	1	关闭错误
DLLIMP int BNC_close();	

//　获取模块固件版本
DLLIMP int BNC_GetVersion(char* pVersion, tBncDevReturn * p_psStatus);
// 功能说明：用于获取控制模块内的firmware版本号（如果模块有），目前只针对GRG的纸币模块有效。
// 参数说明
// 1	char*	pVersion	Out	固件版本（如果有）
// 2	tBncDevReturn &	p_psStatus	Out	返回状态信息
// 返回值说明见表27。
// 1	Int	0	成功
// 2	Int	1	关闭错误


//　设置识别可用钞票
//  功能说明：此命令用于设置可接收的钞票，对于禁止接收的真钞会退出。
// 	参数说明见表3。
// 	1	tBncSetInhibitList *	p_psBuyTicketSetInhibitList	In	设置售票时钞票可用信息结构体数组
// 	2	tBncSetInhibitList *	p_psRechargeSetInhibitList	In	设置充值时钞票可用信息结构体数组
// 	3	tBncDevReturn *	p_psStatus	Out	保存BNA返回的逻辑错误码
// 	返回值说明
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int  BNC_SetPrevalue(tBncSetInhibitList *p_psBuyTicketSetInhibitList, tBncSetInhibitList *p_psRechargeSetInhibitList, tBncDevReturn * p_psStatus);

//　复位
// 功能说明：此命令用于清理通道中的钞票，机芯恢复正常工作。
// 参数说明
// 1	Int	iMode	In	如果通道中有钞票，可以选择的操作由 p_iResetMode 设定可以设置为：
 //0：将传输系统和暂存区的钞票压入钱箱,如果失败则报暂停服务错误；
 //1：将传输系统和暂存区的钞票退出,如果失败则报暂停服务错误；
// 默认为 0 方式
// 2	Int	iRetryTimes	In	复位重试次数
// 3	tBncInitNumInfo *	pNumInfo	Out	退出和回收钱币信息
// 4	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 返回值说明
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BNC_Reset(int iMode, int iRetryTimes, tBncInitNumInfo *pNumInfo,tBncDevReturn * p_psStatus);

			
//　获取模块状态
//  功能说明：调用此接口可获取模块当前状态，详细信息查看返回结构体。
// 	参数说明
// 	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 	返回值说明
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BNC_GetStatus(tBncDevReturn * p_psStatus);

// 获取模块详细状态
DLLIMP int BNC_GetSysDevStatus(tBncSysDevStatus* p_psSysStatus);

typedef struct _TAcceptorState
{
	BYTE  CurrentOCCode;
	char  CurrentCode[3];
	char  SequenceNr;
	BYTE  NrOfDocumentInintermediateStorage;
	char  ResolutionMode;
	BYTE  ValidationMode;
	BYTE  WorkMode;
}TAcceptorState;
// 结构体返回的状态信息;因此在硬件返回信息字符串中内容格式如下:
// [CurrentCode],[CurrentOCCode],[NrOfDocumentInintermediateStorage],[ResolutionMode],[SequenceNr] ,[ ValidationMode] ,[WorkMode] 

//　读取审计信息
// 	功能说明：读取设备的审计信息。
// 	参数说明见下表。
// 	1	tBncAuditCashNum *	pBncAuditCashNum	Out	容器审计信息
// 	2	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BNC_GetAuditData(tBncAuditCashNum *pBncAuditCashNum, tBncDevReturn *pDevStatus);

//　读取容器配置信息
// 	函数定义：
// 	功能说明：读取钱箱结构体信息。
// 	参数说明见下表。
// 	1	tBncBillBoxInfo *	pBillBoxInfo	Out	返回钱箱结构体信息
// 	2	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值见表80。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BNC_GetBillBoxInfo(tBncBillBoxInfo *pBillBoxInfo, tBncDevReturn *pDevStatus);

//　设置容器配置信息
// 	函数定义：
// 	功能说明：设置容器配置信息（设置纸币数量）
// 	参数说明见下表。
// 	1	UNIT	uiSlotID	In	槽位ID
// 	2	tBncBillBoxInfo *	pBillBoxInfo	In	容器配置信息
// 	3	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BNC_SetBillBoxInfo(UINT uiSlotID, tBncBillBoxInfo *pBillBoxInfo, tBncDevReturn *pDevStatus);


//　开始接收钞票
// 	功能说明：机芯进入预备存款状态，指示灯开始闪烁提示用户投入纸币。实现验钞，退钞，钞票压入暂存区等功能。
// 	参数说明见下表
// 	1	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 	返回值说明见下表
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
// 	备注：可在子线程或主线程中启动，使纸币模块进入等待接收纸币的工作状态，程序将进入阻塞，除非成功接收纸币或在另外线程中调用BNC_Stop可以使该函数退出运行。
DLLIMP int BNC_Start(tBncDevReturn * p_psStatus);

//　停止接收钞票
// 	功能说明：机芯停止接收钞票，指示灯熄灭。
// 	参数说明见下表
// 	1	tBncDevReturn *	p_psStatus	Out	保存BNA返回的逻辑错误码
//      tBncCashInfo *p_psCashInfo  Out 保存已接收钞票数据
// 	返回值说明见下表
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BNC_Stop(tBncEnCashInfo *p_psCashInfo,tBncDevReturn * p_psStatus);

//　获取识别钞票信息
// 	功能说明：获取机芯接收的钞票信息。
// 	参数说明见下表
// 	1	tBncCashInfo *	p_psCashInfo	Out	返回钞票信息结构体，临时钱箱的信息
// 	2	BYTE&	p_byOperation	Out	最后一张处理纸币的状态 
// 		0:没有钞票插入; 
// 		1:无法识别的钞票退出到退钞口; 
// 		2:用户由退钞口把钞票（不能识别或者取消操作退出的）取走; 
// 		3:钞票放入又被强行拽出; 
// 		4:钞票被拉紧; 
// 		5:真钞处理完毕; 
// 		6:钞票拉紧并且超时被迫停止接收; 
// 		7:退钞口有钞票未取走超时; 
// 		8:入钞口至暂存区有钞票正在处理; 
// 		9:钱箱已经取走; 
// 		10:钱箱已满; 
// 		11:机芯进出口长时间阻塞(技术故障); 
// 		12:未知错误导致暂停服务。
// 	3	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 
// 	返回值说明见下表
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
// 
// 	备注：可在BNC_Start，执行成功后，调用该函数，获取临时钱箱与最后一张纸币的处理信息。
// 	e)	对于G&D的纸币模块，p_byOperation可以获取0，1，3，5；
// 	f)	对于MEI的纸币模块，p_byOperation可以获取0，1，3，5；
// 	g)	对于G&D的纸币模块，p_byOperation可以获取0，1，5，6，7，9，10；
// 	h)	对于钱箱已满，钱箱已取走，机芯故障，未知错误等其他错误，可以通过BNA_ GetStatu的硬件返回信息来判断获取。
DLLIMP int  BNC_GetCashInfo(tBncCashInfo *p_psCashInfo, BYTE& p_byOperation, tBncDevReturn *p_psStatus);

//　压箱
// 	功能说明：将暂存器的纸币全部压入钱箱。
// 	参数说明见下表
// 	1	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 	返回值说明见下表
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int  BNC_Encash(tBncDevReturn *p_psStatus);

//　退钞
// 	功能说明：将暂存器的纸币全部退出。
// 	参数说明见下表
// 	1	tBncDevReturn *	p_psStatus	Out	保存BNA返回的逻辑错误码
// 	返回值说明见下表
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int  BNC_Refund(tBncDevReturn *p_psStatus);


//　获取钱箱ID
// 	功能说明：调用此接口可获取钱箱ID，详细信息查看返回结构体。
// 	参数说明见下表
// 	1	char*	p_BoxID	Out	获取钱箱ID
// 	2	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 
// 	返回值说明见下表
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BNC_GetBoxID(char * p_BoxID, tBncDevReturn * p_psStatus);

//　找零配钞
// 	功能说明：找零配钞
// 	参数说明见下表。
// 	1	tBncChangeNum *	pChangeNum	In/Out	找零的纸币面额及数量信息
// 	2	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BNC_ChangeBill(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus);

//　取消操作
// 	功能说明：取消操作。
// 	参数说明见下表。
// 	1	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int  BNC_Cancel (tBncDevReturn *p_psStatus);

//　开始更换纸币找零箱
// 	功能说明：此接口用于更换纸币找零箱。
// 	参数说明见下表。
// 	1	UNIT	uiBoxType	In	指定纸币容器的类型容器类型：0-废币箱 1-1元纸币箱, 2-5元纸币箱, 3-10
// 	元纸币箱, 4-20元纸币箱, 5-50元纸币箱, 6-100元纸币箱
// 	2	UINT	uiBoxNo	In	制定纸币容器的序号0 ~ 6 其中： 0号废币箱 1号为5元找零箱，2号为1元找零箱
// 	3	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值见下表。
// 	1	int	0：执行成功；1：执行失败。
DLLIMP int BNC_StartReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBncDevReturn *pDevStatus);

//  停止更换纸币找零箱
// 	功能说明：停止更换纸币找零箱。
// 	参数说明见下表。
// 	1	UNIT	uiBoxType	In	指定纸币容器类型
// 	2	UNIT	uiBoxNo	In	指定纸币容器序号
// 	2	tBncDevReturn *	p_psStatus	Out	保存BNC返回的逻辑错误码
// 
// 	返回值说明见下表。
// 	1	Int	0	成功
// 	2	Int	1	传入参数为空
DLLIMP int BNC_StopReplaceBox(UINT uiBoxType, UINT uiBoxNo, tBncDevReturn *pDevStatus);

//　设置钱箱RFID信息
// 	功能说明：写入钱箱RFID信息。
// 	参数说明见下表。
// 	1	UNIT	uiSLotID	In	槽位ID
// 	2	tBncRfidInfo *	pRFIDInfo	In	设置RFID信息
// 	3	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BNC_WriteRFIDInfo(UINT uiSLotID, tBncRfidInfo * pRFIDInfo, tBncDevReturn * pDevStatus);

//　读取钱箱RFID信息
// 	功能说明：读取钱箱RFID信息。
// 	参数说明见下表。
// 	1	UNIT	uiSLotID	In	槽位ID
// 	2	tBncRfidInfo *	pRFIDInfo	Out	设置RFID信息
// 	3	tBncDevReturn *	pDevStatus	Out	返回状态信息
// 	返回值说明见下表。
// 	1	int	0：执行成功；1，执行失败。
DLLIMP int BNC_ReadRFIDInfo(UINT uiSLotID, tBncRfidInfo* pRFIDInfo, tBncDevReturn * pDevStatus);

//   清空钱箱
//	OUT tBncChangeNum* pBoxInfo
DLLIMP int BNC_Empty(tBncChangeNum* pBoxInfo);
#endif

