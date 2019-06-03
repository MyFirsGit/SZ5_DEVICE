#pragma once
#include "ServiceHelper.h"
#include "THMessage.h"
#include "UpgradeHelper.h"
//@brief TH Helper

class CTHHelper : public CServiceHelper
{
public:

	CTHHelper(CService& service);    // 构造函数
	~CTHHelper(void);                // 析构函数

	void Connect();                  // TH建立连接命令
	void Disconnect();               // TH断开连接命令
	void Initialize();               // 初期化出票机
	void Test();                     // 测试函数 

	void CheckTicketBoxExist(TH_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist);
	long SetWorkMode(TH_CMD_SET_WORK_MODE command);										// 设置工作模式
	long CardOut(TH_CMD_CARD_OUT command, UINT* uiTransportNum);						// 获取票卡
	long SendCard(UINT* uiTransportNum);												// 送出票卡
	long RetractCard(UINT* uiRetractNum);												// 回收票卡
	long GetDevStatus(TH_RSP_GET_DEV_STATUS* response);									// 取设备状态
	long Reset(TH_CMD_INIT_RESET command, UINT* uiRetractNum);								// 重置
	long SetTicketBoxCountInfo(TH_BOX_TYPE inBoxID, tTIMCardNum* inCardNum);	// 设置票卡数量
	long GetTicketBoxCountInfo(TH_BOX_TYPE command, tTIMCardNum* response);	// 取票卡数量状态
	long Init(TH_CMD_INIT_RESET command, UINT* uiRetractNum);									// 初始化
	long GetVersion(TH_RSP_GET_VERSION* response);										// 获取版本号
	long TestSetting(TH_CMD_TEST_SETTING command);										// 硬件测试

	// 维护接口
	bool Repare();																		// 修复出票机
	bool TryRepareTH();																	// 尝试修复出票机
	//bool ReadTicketBoxRFID(TH_BOX_TYPE boxType,tTIMRFIDInfo* pTimRfidInfo);				// 读取RFID
	//bool WriteTicketBoxRFID(TH_BOX_TYPE boxType,tTIMRFIDInfo* pTimRfidInfo);			// 写RFID
	bool ReplaceTicketBox(TH_REPLACE_BOX_CMD_INFO);									// 票箱更换

	bool GetStaticBoxId(IN TH_BOX_TYPE boxId,OUT tTIMRFIDInfo& rfidInfo);

	static int UpdateProgram();									// 更新TH固件
private:
	long CommOpen();
	long CommClose();
	// 其他
	bool IsExceptionCode(long);                                           // 是否为需要抛异常的ID

};
