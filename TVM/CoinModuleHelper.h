#pragma once
#include "ServiceHelper.h"
#include "CHCommand.h"
#include "CHException.h"
#include "CHHardwareException.h"
#include "CHdef.h"
#include "CHAApi.h"
/**
@brief  硬币模块控制类
*/

class CCoinModuleHelper
{
public:
    CCoinModuleHelper();                // 构造函数
    ~CCoinModuleHelper(void);          // 析构函数
	
		typedef enum _box_no
	{
		hopper1 = 1,															
		hopper2,
		small_hopper,
		collection_boxA,
		collection_boxB,
		temporary1,
		temporary2

	}Box_No;
    BOOL IsExceptionCode(long errCode);	
	
	// 基本接口
	long CH_Open();
	long CH_Close();
	long CH_Init();
	long CH_InitModule();
	long CH_GetStatus(tCHADevStatus* pStatus);
	long CH_Repare();
	// 设置硬币模块各部件的硬币数
	long CH_SetCoinNum();

	// 接受、找零接口
	long CH_StartReceive();													// 开始接收
	long CH_StartReceive(CService*);										// 开始接收，并且异步读取接收信息
	long CH_StartReceive(CService*, COIN_ACCEPTABLE& coinFaceTable);		// 开始接收（指定接收面额），并且异步读取接收信息
	long CH_GetInsertInfo(tCHACoinValueInfo& rsp);								// 获取接收硬币信息
	long CH_GetInsertInfoAsync(CService* pCallbackSvc);						// 获取接收硬币信息，直到接收到硬币或停止接收为止（异步调用）
	long CH_StopInsertInfo();												// 停止获取接收硬币信息
	long CH_EndReceive();													// 停止接收硬币
	long CH_Refund();														// 退回接收的硬币
	long CH_RefundAsync(CService* pCallbackSvc);							// 退回接收的硬币（异步调用）

	long CH_Accept(bool bHopperAccept = true);	
	long CH_AcceptAsync(CService* pCallbackSvc, bool bHopperAccept = true);					

	long CH_ClearCoin(tCoinBoxInfo *tBoxInfo);


	long CH_Change(WORD num_OneY, WORD num_HalfY, tCHAChangeNum* rsp);
	long CH_ChangeAsync(CService* pCallbackSvc,WORD num_OneY, WORD num_HalfY);
	long CoinShutter_Open();																			// ó2±òí?è??ú3?′ò?a
	long CoinShutter_Close();																			// ó2±òí?è??ú3?1?±?
    long GetShutterStatus();																			// ó2±òí?è??ú×′ì?

	// RFID
	CString CH_ReadRfidData(int boxNo);	// 箱子编号：0x01 = 加币箱2  0x02 = 加币箱1  0x03 = 回收箱1  0x04 = 回收箱2
	long CH_WriteRfidData(int boxNo, BYTE* rfidData);// 箱子编号：0x01 = 加币箱2  0x02 = 加币箱1  0x03 = 回收箱1  0x04 = 回收箱2

	// 其他接口
    void CoinDataWrite(tCHADevStatus& coin_data);														// CH??êy?é±¨D′è??üá?
    //void CoinDataRead(tCHADevStatus&);	
	long CH_OpenShutter();
	long CH_CloseShutter();
	long CH_RepareShutter();
	//long CH_GetShutterStatus();

private:
	bool m_bMonitor;
	tCHADevStatus m_dataLight;
	int m_nSortTrayRunCount;								// 分捡盘转动计数	
};
