#pragma once
#include "scapi.h"
#include "SCSvcDef.h"
#include <list>
#include "ParamHelper.h"
#include "SCAuditData.h"
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

	OperComHdr_t AquireOperHeader(WORD msgType,WORD wDataLen);
	AfcSysHdr_t	AquireAfcSysHeader(WORD wDataLen);
	//SysComHdr_t AquireComHeader();
	TransComHdr_t AquireTransComHeader(WORD msgType);
	StatusComHdr_t AquireStatusComHeader(WORD msgType);
	SysComHdr_t AquireSysComHeader(WORD msgType);
	OperatorComHdr_t AquireOperatorComHeader();
	// AFC系统公共头
	void MakeAfcSysHdr(AfcSysHdr_t& afcSysHder, LPBYTE lpMsg);
	//系统公共域
	void MakeSysComHdr(SysComHdr_t& sysComHdr,LPBYTE lpMsg);
	//票卡公共域
	void MakeTicketComHdr(TicketComHdr_t& ticketComHdr,LPBYTE lpMsg);
	//操作员公共域
	void MakeOperatorComHdr(OperatorComHdr_t& operatorComHdr,LPBYTE lpMsg);
	//
	//void Make
	
	//安全公共域
	void MakeSysSecurityHdr(SysSecurityHdr_t& sysSecurityHdr,LPBYTE lpMsg);
	// 业务数据头
	void MakeOperComHdr(OperComHdr_t& OperComHdr, LPBYTE lpMsg);
	//交易公共部分头
	void MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg);
	
	void MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg);

	void MakeBOMAR(DEVICE_AR_REASON arReason,VARIABLE_DATA& msg);
	/**********************************************************************************
	// AFC内部交易
	***********************************************************************************/
	// BOM对TVM故障退款
	void MakeBOMRefundTVM(AfcSysHdr_t & afcHeader,BOM_REFUND_TVM& bom_refund_tvm, LPBYTE lpByte);
    // 非即时退卡申请
	//void MakeDelayRefound(DELAY_REFUND, LPBYTE lpByte);
	// 废票回收交易
	void MakeRecycleInvalidTicket(AfcSysHdr_t & afcHeader,INVALIDE_TICKET_RECYCLETRADE& recycleTrade, LPBYTE lpByte);
	// 回收已售车票交易
    //void MakeRecycleIssuedTicket(RECYCLE_ISSUED_TICKETTRADE recycleIssuedTrade, LPBYTE lpByte);
	void MakeBomNonBusinessIncome(AfcSysHdr_t & afcHeader,TransComHdr_t& transHeader,CString& reason,int amount,CString& memo, LPBYTE lpByte);

	//交易审核
	void MakeTransactionAudit(AfcSysHdr_t & afcHeader,TxnAuditData&,VARIABLE_DATA& msg);

	//	//记名卡申请
	void MakeSignCardApply(AfcSysHdr_t & afcHeader,SignCardApply& signCardApply,LPBYTE lpMsg);
	//记名卡修改
	void MakeSignCardModify(AfcSysHdr_t & afcHeader,SignCardApply& signCardModify,LPBYTE lpMsg);
	//挂失
	void MakeReportLoss(AfcSysHdr_t& afcHeader,ReportLoss& reportLoss,LPBYTE lpMsg);
	//解挂
	void MakeCancelReportLoss(AfcSysHdr_t& afcHeader,CancelReportLoss& reportLoss,LPBYTE lpMsg);
	//行政处理
	void MakeAdministrationProcess(AfcSysHdr_t& afcHeader,FinanceComHdr_t &, AdministrationProcess& process,LPBYTE lpMsg);
	//非及时退卡申请
	void MakeNonImmidatelySurrenderApply(AfcSysHdr_t&,NonImmidatelySurrenderApply&, LPBYTE lpByte);
	//非及时退卡申请查询
	void MakeQueryNonImmidatelySurrenderApply(AfcSysHdr_t&,QueryNonImmidatelySurrenderApply&, LPBYTE lpByte);
	//非及时退卡
	void MakeNonImmediatelySurrender(AfcSysHdr_t&,NonImmidatelySurrender&,LPBYTE lpByte);
	//记名卡申请
	void MakeQuerySignCardApplyStatus(AfcSysHdr_t&,QuerySignCardApply&,LPBYTE);
	/**********************************************************************************
	// 业务数据
	***********************************************************************************/
	//--------------------------------业务数据-----------------------------------------
	// 参数和程序更新
	long MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte);
	// 参数和程序下载业务数据
	void MakeDownloadParameterAndProgram(OperComHdr_t& operComHdr,vector<ParameterAndSoftwareUpdate>& updates, LPBYTE lpByte);
	// 设备重要状态改变
	void MakeImportantStatusChange(OperComHdr_t& operComHdr, LPBYTE lpByte, SERVICE_STATUS status);
	// 操作日志
	void MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte);
	// 设备故障数据
	void MakeDeviceErrorData(OperComHdr_t& operComHdr,DEVICE_ERRORDATA&, LPBYTE lpByte);
	// 设备寄存器数据
	void MakeDeviceRegisterData(OperComHdr_t& operComHdr, ACCAR& arData,AR_REASON, LPBYTE lpByte,DWORD& dataLen);
	// 票箱更换
	void MakeMagazineExchange(OperComHdr_t& operComHdr, MAGAZINE_CHANGE& magazine_change,LPBYTE lpByte);
	// 设备部件构成
	void MakeDeviceComponentInfo(OperComHdr_t& operComHdr, LPBYTE lpByte);
	//--------------------------------事件数据---------------------------------------
	// 账户锁定
	void MakeAccountLock(OperComHdr_t& operComHdr, LPBYTE lpByte);
	// 设备流水重置
	void MakeDeviceUdsnNoReset(OperComHdr_t& operComHdr, UDSN_RESET& udsnRest, UDSN_RESET_REASON& resetReason, LPBYTE lpByte);
	// 部件更换
	void MakeDeviceComponentExchange(OperComHdr_t& operComHdr, LPBYTE lpByte, BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);
	// 票箱库存报告
	void MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen);
	// BOM审计数据
	void MakeBomAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData,AUDIT_FLAG auditflag, LPBYTE lpdata, DWORD& datalen);
	// 包传输审计数据
	void MakePackageMsgTransAudit(OperComHdr_t& operComHdr, LPBYTE lpByte);

	/**********************************************************************************
	// 控制数据
	***********************************************************************************/
	//---------------------------------命令数据---------------------------------------
	// 上传指定包编号数据命令
	// 上传指定时间数据命令
	// 上传设备调试数据
	// 运营模式控制命令
	// 设备运行控制命令
	// 24小时运营控制命令
	// 延长运营时间命令
	// 强制时钟同步命令
	void MakeForceTimerSynchronize(LPBYTE, LPBYTE);
	// 售票限制命令
	// 上传参数和程序版本
	// 参数和程序更新命令
	// 强制退出登录
	// 参数和程序版本切换命令
	// 设备状态查询
	// 自动运行参数启用控制
	//---------------------------------请求数据-------------------------------------
	// 登录登出请求
	//void makeLoginOrLogoutRequest(LOGIN_TYPE LoginType,LPBYTE lpByte);
	void MakeLoginOrLogoutRequest(LOGIN_TYPE LoginType, int nOperatorID, LPBYTE nPwd, LPBYTE lpByte);
	// 密码修改请求
	void MakeReformPasswordRequest(CHANGE_STAFFPASSWORD&, LPBYTE lpByte);
	// 即时生效参数下发请求
	void MakeImmediatelyParameterDownloadRequst(LPBYTE lpData, WORD);
	// 连接认证请求
	void MakeConnectAuthRequest(LPBYTE lpByte);
	//---------------------------------通知数据--------------------------------------
	// 账户解锁通知
	// FTP配置信息通知
	// 操作结束通知
	void MakeOpEndNotice(LPBYTE lpByte,NOTICE_TYPE);
	//---------------------------------反馈数据-----------------------------------
	// 登录登出请求反馈
	// 密码修改请求反馈
	// 即时生效参数下发请求反馈
	// 连接认证请求反馈
	// 设备参数和程序版本数据
	void MakeParamAndSoftwareVersionInfo(LPBYTE lpByte,int &length);
	/**********************************************************************************
	// 状态数据
	***********************************************************************************/
	// BOM完整状态数据
	void MakeBomFullStatus(STATUS_TYPE statusType, LPBYTE lpByte);
	void MakeBomFullStatus(BOM_STATUS_ID lpStatusID, BYTE bValue,LPBYTE lpByte);

	// 设备变更状态数据
	void MakeDeviceStatusChange(LPBYTE lpByte, BOM_STATUS_ID lpStatusID, BYTE bValue);
	// 设备故障状态数据
	void MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode);


private:
	static CSCMessageMakeHelper instance;
};