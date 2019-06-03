#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "RWMessage.h"
#include "VersionInfo.h"

/**
@brief  RW控制类
*/

class CRWHelper : public CServiceHelper
{
public:
	
	CRWHelper(CService& service);                                                 // 构造函数
	~CRWHelper();                                                                 // 析构函数

	// RW共有接口 
	void InitCheck(bool reset = true);                                            // 初始化读写器自检
	void CheckDeviceExchange();													  // 检查部件是否更换
	long Close();
    void InitData();															  // 登录时初始化读写器                     
	static long SetTime(_DATE_TIME,_DATE_TIME&);								  // 设置TPU时钟
	bool RepareIni();                                                             // 修复读写器(初始化自检，维护中使用)
	bool Repare();																  // 修复读写器(普通业务中用)
	void ReadCard(bool IsPaidArea = false);										  // 读卡
	long Abort(bool checkStatus = true);                                          // 中止读写器
	long StopReadCard();														  // 停止读卡命令
	bool SynchronismParam();													  // 同步读卡器参数
	bool UpdateParamter(CParamHelper::VEC_PARAM_ID);								  // 更新读写器参数
	static int  UpdateProgram(bool isMain);										  // 更新读写器程序
	static long SetOperationMode(RW_CMD_SET_MODE,RW_RSP_STATUS&);				  // 设置运营模式
	static long SetBusinessTime(RW_CMD_SET_TIME,RW_RSP_STATUS&);				  // 设置TPU 24小时运营或延长运营时间
	long GetMac(BYTE*,BYTE*);													  // 计算MAC值
	long GetTicketPrice(RW_CMD_TICKET_PRICE,RW_CMD_TICKET_PRICE_RES&);							  // 获取票价
	static long Reset(RW_RSP_STATUS&);

	long GetStatus(RW_RSP_STATUS&);
	long GetTime(_DATE_TIME&);
	long GetTPUInfo(RW_RSP_TPU_INFO&);
	long GetParameterInfo(VERSION_ACC&,VERSION_ECT&,VERSION_AFC&);

	// 业务命令
	void ReadCard(RW_READ_CARD_REQUEST);
	long Issue(RW_ISSUE_REQUEST,RW_BUSSINESS_RESPONSE&);
	long Adjust(RW_CMD_ADJUST,RW_BUSSINESS_RESPONSE&);
	long Charge(RW_CMD_CHARGE,RW_BUSSINESS_RESPONSE&);
	long Refund(RW_CMD_REFUND,RW_BUSSINESS_RESPONSE&);
	long Block(RW_CMD_BLOCK,RW_BUSSINESS_RESPONSE&);
	long Counteract(RW_CMD_COUNTERACT,RW_BUSSINESS_RESPONSE&);  
	long Refresh(RW_CMD_ACTIVE,RW_BUSSINESS_RESPONSE&);
	long Defer(RW_CMD_DEFER,RW_BUSSINESS_RESPONSE&);
	long ReplaceOldCard(RW_CMD_REPLACE_CARD&,RW_BUSSINESS_RESPONSE&);
	long ReplaceNewCard(RW_CMD_REPLACE_CARD&,RW_BUSSINESS_RESPONSE&);
	long ChargeOverTimeProcess(RW_CMD_CHARGEOVERTIME chargeOverTime, RW_BUSSINESS_RESPONSE& response);
	long IssueInitCheckProcess(RW_CMD_SETCHECK chargeInput,RW_CMD_RESP_SETCHECK &response);
	long UpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO,RW_BUSSINESS_RESPONSE&);
	bool IsRWHasException();

	int SaveRWUDData(WORD txnLen,LPBYTE txnBuf);								    // 保存读写器返回的UD数据到交易管理中
	void SaveRWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType=0);		// 保存读写器返回的AR数据到AR管理中
	long GetErrCode(int&);
	CString GetTpuErrorInfo(int iError);                                            // 根据错误代码查找错误信息
private:
	// 通用命令
	long Open();
	long Abort(RW_RSP_STATUS&);
	long Stop(RW_RSP_STATUS&);
	
	// 功能命令
	long InitRW(RW_CMD_INIT,RW_RSP_STATUS&);
	long DownloadParamter(RW_PARAM_DOWNLOAD);
	long GetUDData(RW_BUSSINESS_RESPONSE&);
    static long RWFirmwareDownload(RW_CMD_FIRMWARE);            
	
	// 其他
	static bool IsExceptionCode(long errCode);                                           // 是否为需要抛异常的ID

	int GetAndSaveUDData();														  // 获取并保存UD数据

	CString gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode);             // 根据错误代码从文件中查找错误信息

	static RWID s_RWID;
	friend class CBOMForeService;
};
