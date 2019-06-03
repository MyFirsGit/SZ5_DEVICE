#pragma once
#include "RWCommands.h"
#include "RWExp.h"
#include "RWDef.h"

//////////////////////////读写器设备命令////////////////////////////////////////////////////////////
/** 
@brief   RW打开读写器命令
*/
class RW_API CRWOpen :	public CRWCommands
{
public:
	CRWOpen(RW_CMD_OPEN,RWID&);
	~CRWOpen(void);
	long ExecuteCommand();
	long IsValidCommand();
private:
	RW_CMD_OPEN m_cmdOpen;
};

/** 
@brief   RW关闭读写器命令
*/
class RW_API CRWClose :	public CRWCommands
{
public:
	CRWClose(RWID&);
	~CRWClose(void);
	long IsValidCommand();
	long ExecuteCommand();
};

/** 
@brief   RW复位读写器命令
*/
class RW_API CRWReset :	public CRWCommands
{
public:
	CRWReset(RWID&);
	~CRWReset(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_RSP_STATUS m_response;
};

/** 
@brief   RW读取读写器状态命令
*/
class RW_API CRWGetStatus :	public CRWCommands
{
public:
	CRWGetStatus(RWID&);
	~CRWGetStatus(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_RSP_STATUS m_response;
};

/** 
@brief   RW中止读写器命令
*/
class RW_API CRWAbort : public CRWCommands
{
public:
	CRWAbort(RWID&);
	~CRWAbort(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_RSP_STATUS m_response;
};

RW_API void EnableRead();
RW_API void DisableRead();

/** 
@brief   RW停止读卡命令
*/
class RW_API CRWStopReadCard : public CRWCommands
{
public:
	CRWStopReadCard(RWID&);
	~CRWStopReadCard(void);
	long ExecuteCommand();
	long IsValidCommand();
};

/** 
@brief   RW停止读写器命令
*/
class RW_API CRWStop :	public CRWCommands
{
public:
	CRWStop(RWID&);
	~CRWStop(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_RSP_STATUS m_response;
};

/** 
@brief   RW设置时钟命令
*/
class RW_API CRWSetTime : public CRWCommands
{
public:
	CRWSetTime(_DATE_TIME,RWID&);
	~CRWSetTime(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(_DATE_TIME*);

private:
	_DATE_TIME m_Input;
	BYTE m_response[7];
};

/** 
@brief   RW获取时钟命令
*/
class RW_API CRWGetTime : public CRWCommands
{
public:
	CRWGetTime(RWID&);
	~CRWGetTime(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(_DATE_TIME*);

private:
	BYTE m_response[7];
};

/** 
@brief   RW获取MAC
*/
class RW_API CRWGetMac : public CRWCommands
{
public:
	CRWGetMac(WORD,BYTE*,RWID&);
	~CRWGetMac(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(BYTE*);

private:
	WORD m_keyVersion;
	BYTE m_hashData[20];
	BYTE m_resMac[4];
};

/** 
@brief   RW获取错误代码
*/
class RW_API CRWGetErrCode : public CRWCommands
{
public:
	CRWGetErrCode(int,RWID&);
	~CRWGetErrCode(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(int*);

private:
	int m_ErrorCode;
};

/** 
@brief   RW获取TPU信息
*/
class RW_API CRWGetTpuInfo : public CRWCommands
{
public:
	CRWGetTpuInfo(RWID&);
	~CRWGetTpuInfo(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_TPU_INFO*);

private:
	RW_RSP_TPU_INFO m_TpuInfo;
};

/**
@brief   RW	Firmware 下载
*/
class RW_API CRWFirmwareDownload : public CRWCommands
{
public:
	CRWFirmwareDownload(LPRW_CMD_FIRMWARE,RWID&);
	~CRWFirmwareDownload(void);
	long ExecuteCommand();
	long IsValidCommand();
private:
	LPRW_CMD_FIRMWARE  m_firmwareDownload;
};

/** 
@brief   RW初始化命令
*/
class RW_API CRWInit :	public CRWCommands
{
public:
	CRWInit(RW_CMD_INIT,RWID&);
	~CRWInit(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_CMD_INIT  m_initRequest;
	RW_RSP_STATUS m_response; 
};

/** 
@brief   RW获取票价
*/
class RW_API CRWGetTicketPrice : public CRWCommands
{
public:
	CRWGetTicketPrice(RW_CMD_TICKET_PRICE,RWID&);
	~CRWGetTicketPrice(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_CMD_TICKET_PRICE_RES*);

private:
	RW_CMD_TICKET_PRICE  m_ticketInput;
	RW_CMD_TICKET_PRICE_RES m_response; 
};

/** 
@brief   RW获取税费
*/
class RW_API CRWGetTicketTax : public CRWCommands
{
public:
	CRWGetTicketTax(RW_CMD_TICKET_TAX,RWID&);
	~CRWGetTicketTax(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_CMD_TICKET_TAX_RES*);

private:
	RW_CMD_TICKET_TAX  m_ticketInput;
	RW_CMD_TICKET_TAX_RES m_response; 
};

/** 
@brief   RW获取参数信息
*/
class RW_API CRWGetParameterInfo : public CRWCommands
{
public:
	CRWGetParameterInfo(RWID&);
	~CRWGetParameterInfo(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_PARAM_INFO*);

private:
	RW_RSP_PARAM_INFO m_response; 
};

/** 
@brief   RW参数更新命令
*/
class RW_API CRWParamFirmware : public CRWCommands
{
public:
	CRWParamFirmware(RW_PARAM_DOWNLOAD,RWID&);
	~CRWParamFirmware(void);
	long ExecuteCommand();
	long IsValidCommand();

private:
	RW_PARAM_DOWNLOAD m_downloadInput;
};

/** 
@brief   RW设置运营模式
*/
class RW_API CRWSetOperationMode : public CRWCommands
{
public:
	CRWSetOperationMode(RW_CMD_SET_MODE,RWID&);
	~CRWSetOperationMode(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_CMD_SET_MODE m_setModeInput;
	RW_RSP_STATUS m_response;
};

/** 
@brief   RW设置延长运营时间
*/
class RW_API CRWSetBusinessTime : public CRWCommands
{
public:
	CRWSetBusinessTime(RW_CMD_SET_TIME,RWID&);
	~CRWSetBusinessTime(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	RW_CMD_SET_TIME m_setModeInput;
	RW_RSP_STATUS m_response;
};

/** 
@brief   RW获取UD数据
*/
class RW_API CRWGetUDData : public CRWCommands
{
public:
	CRWGetUDData(RWID&);
	~CRWGetUDData(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(LPRW_BUSSINESS_RESPONSE);

private:
	LPRW_BUSSINESS_RESPONSE m_response;
};

/** 
@brief   RW读卡信息命令
*/
class RW_API CRWReadCard :	public CRWCommands
{
public:
	CRWReadCard(RW_READ_CARD_REQUEST,RWID&);
	~CRWReadCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_READ_CARD_RESPONSE*);

private:
	RW_READ_CARD_REQUEST  m_readCardRequest;
	RW_READ_CARD_RESPONSE m_cardInfo; 
};

/** 
@brief   RW一次读卡信息命令
*/
class RW_API CRWTryReadCard:public CRWCommands
{
public:
	CRWTryReadCard(RW_READ_CARD_REQUEST,RWID&);
	~CRWTryReadCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_READ_CARD_RESPONSE*);
private:
	RW_READ_CARD_REQUEST  m_readCardRequest;
	RW_READ_CARD_RESPONSE m_cardInfo; 
};

//////////////////////////业务命令/////////////////////////////////////////////
/**
@brief   RW	售卡交易命令
*/
class RW_API CRWIssue :	public CRWCommands
{
public:
	CRWIssue(RW_ISSUE_REQUEST,RWID&);
	~CRWIssue(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_ISSUE_REQUEST m_issueInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW售票初检命令
*/
class RW_API CRWSetCheck : public CRWCommands
{
public:
	CRWSetCheck(RW_CMD_SETCHECK,RWID&);
	~CRWSetCheck(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_CMD_RESP_SETCHECK*);

private:
	RW_CMD_SETCHECK m_srtCheck;
	RW_CMD_RESP_SETCHECK m_response;
};

/**
@brief   RW补票交易命令
*/
class RW_API CRWAdjust : public CRWCommands
{
public:
	CRWAdjust(RW_CMD_ADJUST,RWID&);
	~CRWAdjust(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_ADJUST m_adjustInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW充值交易命令
*/
class RW_API CRWCharge : public CRWCommands
{
public:
	CRWCharge(RW_CMD_CHARGE,RWID&);
	~CRWCharge(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_CHARGE m_chargeInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW充值超时命令
*/
class RW_API CRWChargeOverTime : public CRWCommands
{
public:
	CRWChargeOverTime(RW_CMD_CHARGEOVERTIME,RWID&);
	~CRWChargeOverTime(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_CHARGEOVERTIME m_chargeOverTimeInput;
	RW_BUSSINESS_RESPONSE m_response;
};



/**
@brief   RW退卡交易命令
*/
class RW_API CRWRefund : public CRWCommands
{
public:
	CRWRefund(RW_CMD_REFUND,RWID&);
	~CRWRefund(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_REFUND m_refundInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW锁卡交易命令
*/
class RW_API CRWBlock :	public CRWCommands
{
public:
	CRWBlock(RW_CMD_BLOCK,RWID&);
	~CRWBlock(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_BLOCK m_blockInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	抵消交易命令
*/
class RW_API CRWCounteract : public CRWCommands
{
public:
	CRWCounteract(RW_CMD_COUNTERACT,RWID&);
	~CRWCounteract(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_COUNTERACT m_counteractInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	激活交易命令
*/
class RW_API CRWRefresh : public CRWCommands
{
public:
	CRWRefresh(RW_CMD_ACTIVE,RWID&);
	~CRWRefresh(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_ACTIVE m_refreshInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	无卡替换交易命令
*/
class RW_API CRWReplaceNoCard : public CRWCommands
{
public:
	CRWReplaceNoCard(RW_CMD_NO_CARD_REPLACE,RWID&);
	~CRWReplaceNoCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_NO_CARD_REPLACE m_replaceInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	替换旧卡交易命令
*/
class RW_API CRWReplaceOldCard : public CRWCommands
{
public:
	CRWReplaceOldCard(RW_CMD_REPLACE_CARD,RWID&);
	~CRWReplaceOldCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_REPLACE_CARD m_replaceOldInput;
	RW_BUSSINESS_RESPONSE m_response;
};


/**
@brief   RW	替换新卡交易命令
*/
class RW_API CRWReplaceNewCard : public CRWCommands
{
public:
	CRWReplaceNewCard(RW_CMD_REPLACE_CARD,RWID&);
	~CRWReplaceNewCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_REPLACE_CARD m_replaceNewInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	延期交易命令
*/
class RW_API CRWDefer : public CRWCommands
{
public:
	CRWDefer(RW_CMD_DEFER,RWID&);
	~CRWDefer(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_DEFER m_deferInput;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	记名卡信息修改命令
*/
class RW_API CRWUpdateSignCardInfo : public CRWCommands
{
public:
	CRWUpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO,RWID&);
	~CRWUpdateSignCardInfo(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_UPDATE_SIGNCARD_INFO m_updateSignCardInfo;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	ES发行交易
*/
class RW_API CRWReleaseEs : public CRWCommands
{
public:
	CRWReleaseEs(RW_CMD_ES_PM_WRITE_CARD_INFO,RWID&);
	~CRWReleaseEs(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_ES_PM_WRITE_CARD_INFO m_structReleaseEs;
	RW_BUSSINESS_RESPONSE m_pResponse;
};

/**
@brief   RW	个性化交易命令
*/
class RW_API CRWESPersonalize : public CRWCommands
{
public:
	CRWESPersonalize(RW_CMD_ES_PM_WRITE_CARD_INFO,RWID&);
	~CRWESPersonalize(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_ES_PM_WRITE_CARD_INFO m_individuationInfo;
	RW_BUSSINESS_RESPONSE m_pResponse;
};

/**
@brief   RW	个性化命令（个性化和个性化更新（不换卡））
*/
class RW_API CRWPersonalize : public CRWCommands
{
public:
	CRWPersonalize(RW_CMD_PERSONALIZE_INFO,RWID&);
	~CRWPersonalize(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_PERSONALIZE_INFO m_personalizeInfo;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	个性化更新锁旧卡命令
*/
class RW_API CRWPersonalizeUpdateOldCard : public CRWCommands
{
public:
	CRWPersonalizeUpdateOldCard(RW_CMD_PERSONALIZE_INFO,RWID&);
	~CRWPersonalizeUpdateOldCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_PERSONALIZE_INFO m_personalizeInfo;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	个性化更新写新卡命令
*/
class RW_API CRWPersonalizeUpdateNewCard : public CRWCommands
{
public:
	CRWPersonalizeUpdateNewCard(RW_CMD_PERSONALIZE_INFO,RWID&);
	~CRWPersonalizeUpdateNewCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_PERSONALIZE_INFO m_personalizeInfo;
	RW_BUSSINESS_RESPONSE m_response;
};

/**
@brief   RW	发卡母卡外部认证命令
*/
class RW_API CRWAuthIssueCardMasterCard : public CRWCommands
{
public:
	CRWAuthIssueCardMasterCard(AuthIssueCardMasterCardParam,RWID&);
	~CRWAuthIssueCardMasterCard(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_RSP_STATUS*);

private:
	AuthIssueCardMasterCardParam m_authIssueCardMasterCard;
	RW_RSP_STATUS m_response; 
};

/**
@brief   RW EP支付  by：zhengxianle
*/
class RW_API CRWConsumeEP : public CRWCommands
{
public:
	CRWConsumeEP(RW_CMD_CONSUME,RWID&);
	~CRWConsumeEP(void);
	long ExecuteCommand();
	long IsValidCommand();
	void GetRWResponse(RW_BUSSINESS_RESPONSE*);

private:
	RW_CMD_CONSUME			m_Cmd_Consume;	// 命令数据
	RW_BUSSINESS_RESPONSE	m_response;    // 卡操作完成后反馈的数据
};