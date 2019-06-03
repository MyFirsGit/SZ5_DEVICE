#pragma once
#include "stdafx.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include <list>
#include "ParamHelper.h"
//#include "SCAuditData.h"
#include "ACCAuditManager.h"
using namespace std;

#define  theSCMessageMakeHelper CSCMessageMakeHelper::GetInstance()
class CSCMessageMakeHelper
{
public:
	static CSCMessageMakeHelper& GetInstance()
	{
		return instance;
	}
	CSCMessageMakeHelper();
	~CSCMessageMakeHelper();

	/**********************************************************************************
		获取公共域
	***********************************************************************************/
	// AFC系统专用头
	AfcSysHdr_t	AquireAfcSysHeader(WORD wDataLen);
	// 系统公共域SysComHdr_t
	SysComHdr_t AquireSysComHeader(WORD msgType);
	// 票卡公共域SysCardCom_t
	SysCardCom_t AquireSysCardComHeader();
	// 持卡人公共域SysCardholderCom_t
	SysCardholderCom_t AquireSysCardholderComHeader();
	// 内部交易数据公共域
	TransComHdr_t AquireTransComHeader(WORD msgType);
	// 业务数据专用头
	OperComHdr_t AquireOperHeader(WORD msgType,WORD wDataLen);
	// 状态数据头
	StatusComHdr_t AquireStatusComHeader(WORD msgType);

	/**********************************************************************************
		设置公共域
	***********************************************************************************/
	// AFC系统公共头
	void MakeAfcSysHdr(AfcSysHdr_t& afcSysHder,LPBYTE lpMsg);
	// 系统公共域
	void MakeSysComHdr(SysComHdr_t& sysComHdr,LPBYTE lpMsg);
	// 票卡公共域
	void MakeSysCardCom(SysCardCom_t& sysCardCom,LPBYTE lpMsg);
	// 持卡人公共域
	void MakeSysCardholderCom(SysCardholderCom_t& sysCardholderCom,LPBYTE lpMsg);
	// 内部交易数据公共域
	void MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg);
	// 业务数据头
	void MakeOperComHdr(OperComHdr_t& OperComHdr, LPBYTE lpMsg);
	// 状态数据头
	void MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg);
	// 安全公共域
	void MakeSysSecurityHdr(SysSecurityHdr_t& sysSecurityHdr,LPBYTE lpMsg);

	/**********************************************************************************
		ACC交易
	***********************************************************************************/
	// ACC设备寄存器数据
	void MakeTxnAuditRegister(TXN_AUDIT_REGISTER& TxnAuditRegister,ACCAR& arData,LPBYTE lpMsg,LPBYTE lpAfcHead);
	// CD版本切换
	void MakeTxnEventCDVersionStatus(TXN_EVENT_CD_VERSION_STATUS& TxnEventCDVersionStatus,LPBYTE lpMsg,LPBYTE lpAfcHead);
	// 设备状态
	void MakeTxnEventDeviceStatus(TXN_EVENT_DEVICE_STATUS& TxnEventDeviceStatus,LPBYTE lpMsg,LPBYTE lpAfcHead);
	// 设备标识
	void MakeTxnEventDeviceIdentification(TXN_EVENT_DEVICE_IDENTIFICATION& TxnEventDeviceIdentification,LPBYTE lpMsg,LPBYTE lpAfcHead);

	/**********************************************************************************
		业务数据
	***********************************************************************************/
	// TVM、AVM故障单业务数据
	void MakeBusinessFaliedData(OperComHdr_t& operComHdr,BUSINESS_FAILED_DATA& inBusinessFailedData,LPBYTE lpByte);
	// 参数和程序下载
	void MakeDownloadParameterAndProgram(OperComHdr_t& operComHdr,vector<ParameterAndSoftwareUpdate>& updates, LPBYTE lpByte);
	// 参数和程序更新
	long MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte);
	// 设备重要状态改变
	void MakeImportantStatusChange(OperComHdr_t& operComHdr, LPBYTE lpByte, SERVICE_STATUS status);
	// SAM卡签到签退
	void MakeIsamCheckIn(OperComHdr_t operComHdr, LPBYTE lpByte, ISAM_AUTH isam_auth, BYTE result);
	// 设备故障数据
	void MakeDeviceErrorData(OperComHdr_t& operComHdr,DEVICE_ERRORDATA&, LPBYTE lpByte);
	// 操作日志
	void MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte);
	// 设备寄存器数据
	void MakeDeviceRegisterData(OperComHdr_t& operComHdr, DEVICEREG_AR& arData,AR_REASON, LPBYTE lpByte,DWORD& dataLen);
	// 票箱更换
	void MakeMagazineExchange(OperComHdr_t& operComHdr, MAGAZINE_CHANGE& magazine_change,LPBYTE lpByte);
	// 钱箱更换
	void MakeCashBoxExchange(OperComHdr_t& operComHdr, CASHBOX_CHANGE& cash_change,LPBYTE lpByte);

	/**********************************************************************************
		事件数据
	***********************************************************************************/
	// 账户锁定
	void MakeAccountLock(OperComHdr_t& operComHdr, LPBYTE lpByte);
	// 设备流水重置
	void MakeDeviceUdsnNoReset(OperComHdr_t& operComHdr, UDSN_RESET& udsnRest, UDSN_RESET_REASON& resetReason, LPBYTE lpByte);
	// 部件更换
	void MakeDeviceComponentExchange(OperComHdr_t& operComHdr, LPBYTE lpByte, BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);
	// SAM卡更换 与MakeDeviceComponentExchange相同
	// AVM、TVM审计数据
	void MakeTVMAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData, LPBYTE lpdata/*, DWORD& datalen*/);

	/**********************************************************************************
		传输审计数据
	***********************************************************************************/
	// 包传输审计数据
	void MakePackageMsgTransAudit(OperComHdr_t& operComHdr, LPBYTE lpByte);

	/**********************************************************************************
		状态数据
	***********************************************************************************/
	// TVM完整状态数据
	void MakeTVMFullStatus(STATUS_TYPE statusType, LPBYTE lpByte);
	void MakeTVMFullStatus(TVM_STATUS_ID lpStatusID, BYTE bValue,LPBYTE lpByte);
	// 设备变更状态数据
	void MakeDeviceStatusChange(LPBYTE lpByte, TVM_STATUS_ID lpStatusID, BYTE bValue);
	// 设备故障状态数据
	void MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode);

	/**********************************************************************************
		命令数据
	***********************************************************************************/
	// 强制时钟同步命令
	void MakeForceTimerSynchronize(LPBYTE, LPBYTE);

	/**********************************************************************************
		请求数据
	***********************************************************************************/
	// 登录登出请求
	void MakeLoginOrLogoutRequest(LOGIN_TYPE LoginType, int nOperatorID, LPBYTE nPwd, LPBYTE lpByte);
	// 密码修改请求
	void MakeReformPasswordRequest(CHANGE_STAFFPASSWORD&, LPBYTE lpByte);
	// 个性化信息查询
	void MakePersonalizationRequest(PersonalizationQuery&,LPBYTE lpByte);
	// 历史信息查询
	void MakeHistoryInfoRequest(HistoryInfoQuery&,LPBYTE lpByte,DWORD& lenth);
	// 即时生效参数下发请求
	void MakeImmediatelyParameterDownloadRequst(LPBYTE lpData, WORD);
	// 连接认证请求
	void MakeConnectAuthRequest(LPBYTE lpByte);
	// 招援請求
	void MakeCallHelpRequest(LPBYTE lpByte,bool bIsCallOrCancel);

	/**********************************************************************************
		反馈数据
	***********************************************************************************/
	// 设备参数和程序版本数据
	void MakeParamAndSoftwareVersionInfo(LPBYTE lpByte,int &length);

	/**********************************************************************************
		通知数据
	***********************************************************************************/
	// 操作完成通知
	void MakeOpEndNotice(LPBYTE lpByte,NOTICE_TYPE);


	/**********************************************************************************
		保留
	***********************************************************************************/
	// AR
	void MakeDeviceAR(DEVICE_AR_REASON arReason,VARIABLE_DATA& msg);
	// 废票回收交易
	void MakeRecycleInvalidTicket(AfcSysHdr_t & afcHeader,INVALIDE_TICKET_RECYCLETRADE& recycleTrade, LPBYTE lpByte);
	//--------------------------------业务数据-----------------------------------------
	
	// 设备部件构成
	void MakeDeviceComponentInfo(OperComHdr_t& operComHdr, LPBYTE lpByte);
	//--------------------------------事件数据---------------------------------------
	// 票箱库存报告
	void MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen);

	// 钱箱库存报告
	void MakeReportMoneyBoxCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dwMsgLen);

	// 纸币箱清空事件
	void MakeClearBanknotesOrCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount,WORD& wMsgLen);
	// 硬币箱清空事件
	//void MakeClearCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount);
private:
	static					CSCMessageMakeHelper instance;
	DWORD                   TranCoinTypeToValue(int nType);
	BankNoteAndCoinType_t	TranCoinType(int nType);
	DWORD					TranTypeToValue(BankNoteAndCoinType_t type);
	DWORD					TranBanknoteToValue(int nType);
	BankNoteAndCoinType_t   TranBanknoteToType(int nType);
};