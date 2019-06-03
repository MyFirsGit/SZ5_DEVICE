#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"

/**
@brief  发卡模块用读写器控制类
*/

class CTVMRWHelper 
{
public:

	CTVMRWHelper(RW_TYPE rwType);                                  // 构造函数
	~CTVMRWHelper();                                                                 // 析构函数

	// TW共有接口 
	void InitCheck(bool reset = true);                                            // 初始化读写器自检
	void CheckDeviceExchange();													  // 检查部件是否更换
	long Close();
	void InitData();															  // 登录时初始化读写器                     
	long SetTime(_DATE_TIME,_DATE_TIME&);								          // 设置TPU时钟
	bool RepareIni();                                                             // 修复读写器(初始化自检，维护中使用)
	bool Repare();																  // 修复读写器(普通业务中用)
	void ReadCard(SERVICE_ID serviceId,bool IsPaidArea = false,bool IsEpPayment = false);			  // 读卡
	long TryReadCard(SERVICE_ID serviceId,RW_READ_CARD_RESPONSE&,bool IsPaidArea = false);								//同步读一次卡
	long Abort(bool checkStatus = true);                                          // 中止读写器
	long StopReadCard();														  // 停止读卡命令
	bool SynchronismParam();													  // 同步读卡器参数
	bool UpdateParamter(CParamHelper::VEC_PARAM_ID);							  // 更新读写器参数
	int  UpdateProgram(bool isMain);										      // 更新读写器程序
	long SetOperationMode(RW_CMD_SET_MODE,RW_RSP_STATUS&);						  // 设置运营模式
	long SetBusinessTime(RW_CMD_SET_TIME,RW_RSP_STATUS&);						  // 设置TPU 24小时运营或延长运营时间
	long GetMac(BYTE*,BYTE*);													  // 计算MAC值
	long GetTicketPrice(RW_CMD_TICKET_PRICE,RW_CMD_TICKET_PRICE_RES&);			  // 获取票价
	long GetTicketPriceTax(RW_CMD_TICKET_TAX,RW_CMD_TICKET_TAX_RES&);			  // 获取税费
	long Reset(RW_RSP_STATUS&);

	// 业务命令
	void ReadCard(RW_READ_CARD_REQUEST,CService* pService);
	long TryReadCard(RW_READ_CARD_REQUEST,RW_READ_CARD_RESPONSE*);
	long Issue(RW_ISSUE_REQUEST,RW_BUSSINESS_RESPONSE&);
	long Adjust(RW_CMD_ADJUST,RW_BUSSINESS_RESPONSE&);
	long Charge(RW_CMD_CHARGE,RW_BUSSINESS_RESPONSE&);
	//long Refund(RW_CMD_REFUND,RW_BUSSINESS_RESPONSE&);
	long Block(RW_CMD_BLOCK,RW_BUSSINESS_RESPONSE&);
	long ConsumeEP(RW_CMD_CONSUME,RW_BUSSINESS_RESPONSE&);				// EP支付业务命令
	//long Counteract(RW_CMD_COUNTERACT,RW_BUSSINESS_RESPONSE&);  
	//long Refresh(RW_CMD_ACTIVE,RW_BUSSINESS_RESPONSE&);
	//long Defer(RW_CMD_DEFER,RW_BUSSINESS_RESPONSE&);
	//long ReplaceOldCard(RW_CMD_REPLACE_CARD&,RW_BUSSINESS_RESPONSE&);
	//long ReplaceNewCard(RW_CMD_REPLACE_CARD&,RW_BUSSINESS_RESPONSE&);
	//long ChargeOverTimeProcess(RW_CMD_CHARGEOVERTIME chargeOverTime, RW_BUSSINESS_RESPONSE& response);
	//long IssueInitCheckProcess(RW_CMD_SETCHECK chargeInput,RW_CMD_RESP_SETCHECK &response);
	//long UpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO,RW_BUSSINESS_RESPONSE&);
	//long Personalize(RW_CMD_PERSONALIZE_INFO,RW_BUSSINESS_RESPONSE&);				// 个性化命令（个性化和个性化更新（不换卡））
	//long PersonalizeUpdateOldCard(RW_CMD_PERSONALIZE_INFO,RW_BUSSINESS_RESPONSE&);	// 个性化更新锁旧卡命令
	//long PersonalizeUpdateNewCard(RW_CMD_PERSONALIZE_INFO,RW_BUSSINESS_RESPONSE&);	// 个性化更新写新卡命令
	bool IsRWHasException();


	bool IsTWHasException();

	int SaveRWUDData(WORD txnLen,LPBYTE txnBuf);								    // 保存读写器返回的UD数据到交易管理中
	void SaveRWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType=0);		// 保存读写器返回的AR数据到AR管理中
	CString GetTpuErrorInfo(int iError);                                           // 根据错误代码查找错误信息
	long GetStatus(RW_RSP_STATUS&);

	long GetTime(_DATE_TIME&);
	long GetTPUInfo(RW_RSP_TPU_INFO&);
	long GetParameterInfo(VERSION_ACC&,VERSION_ECT&,VERSION_AFC&);

	// 充值服务器检测
	long DoRechargeServerMoniter();
private:
	// 通用命令
	long Open();
	long Abort(RW_RSP_STATUS&);
	long Stop(RW_RSP_STATUS&);
	long GetErrCode(int&);

	// 功能命令
	long InitTW(RW_CMD_INIT,RW_RSP_STATUS&);
	long DownloadParamter(RW_PARAM_DOWNLOAD);
	long GetUDData(RW_BUSSINESS_RESPONSE&);
	long TWFirmwareDownload(RW_CMD_FIRMWARE);            

	// 其他
	static bool IsExceptionCode(long errCode);                                           // 是否为需要抛异常的ID
	int GetAndSaveUDData();														         // 获取并保存UD数据
	CString gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode);                  // 根据错误代码从文件中查找错误信息
	
	WORD GetTPUParamID(WORD paraID);

	static CVersionInfo::PROG_TYPE ConvertRWTypeToProgType(RW_TYPE rwType,bool isMain);

	RWID &m_RWID;
	RW_TYPE m_RWType;

	static RWID CARD_RWID;
	static RWID TOKEN_RWID;
	static RWID CHARGE_RWID;
	friend class CTVMForeService;
};