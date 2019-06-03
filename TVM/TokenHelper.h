#pragma once
#include "ServiceHelper.h"
#include "TOKENMessage.h"
#include "UpgradeHelper.h"
#include "TokenCyclerRfidCommand.h"
//@brief TOKEN Helper

class CTOKENHelper : public CServiceHelper
{
public:

	CTOKENHelper(CService& service);    // 构造函数
	~CTOKENHelper(void);                // 析构函数

	void Connect();                  // TOKEN建立连接命令
	void Disconnect();               // TOKEN断开连接命令
	void Initialize();               // 初期化出票机
	void Test();                     // 测试函数 

	void CheckTicketBoxExist(TOKEN_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist,bool& isMagazineDExist);

	// 雷格特API封装
	//long SetWorkMode(TOKEN_CMD_SET_WORK_MODE command);									// 设置工作模式
	long CardOut(TOKEN_BOX_NO boxNo, UINT* uiTransportNum);									// 获取票卡
	long SendCard(UINT* uiTransportNum);													// 送出票卡
	long RetractCard(UINT* uiRetractNum);													// 回收票卡
	long CleanOut(TOKEN_BOX_NO boxNo);														// 清理票卡
	long GetDevStatus(TOKEN_RSP_GET_DEV_STATUS* response);									// 取设备状态
	long Reset();																			// 重置
	//long SetTicketBoxCountInfo(TOKEN_CMD_SET_COUNT_INFO inBoxID, tTIMCardNum* inCardNum);	// 设置票卡数量
	//long GetTicketBoxCountInfo(TOKEN_CMD_GET_COUNT_INFO command, tTIMCardNum* response);	// 取票卡数量状态
	long Init(UINT* uiRetractNum);															// 初始化
	long GetVersion(TOKEN_RSP_GET_VERSION* response);										// 获取版本号
	//long TestSetting(TOKEN_CMD_TEST_SETTING command);										// 硬件测试

	// 维护接口
	bool Repare();																			// 修复出票机
	bool TryRepareTOKEN();																	// 尝试修复出票机

	// RFID
	CString ReadTokenBoxARFID();
	CString ReadTokenBoxBRFID();
	CString ReadInvalidBoxRFID();
	CString ReadTokenBoxCollectRFID();

	static int UpdateProgram();																// 更新TOKEN固件
private:
	long CommOpen();
	long CommClose();
	// 其他
	bool IsExceptionCode(long);																// 是否为需要抛异常的ID
	// RFID
	bool ReadTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo);							// 读取RFID
	bool WriteTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo);							// 写RFID
	// 回收箱RFID
	long TokenCyclerRfidInit();//每次读写回收箱RFID前调用
	long TokenCyclerRfidRead(TOKEN_READ_WRITE_RFID* pTimRfidInfo);							// TOKEN回收箱RFID
	long TokenCyclerRfidWrite(TOKEN_READ_WRITE_RFID* pTimRfidInfo);
};
