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
	// AFCϵͳ����ͷ
	void MakeAfcSysHdr(AfcSysHdr_t& afcSysHder, LPBYTE lpMsg);
	//ϵͳ������
	void MakeSysComHdr(SysComHdr_t& sysComHdr,LPBYTE lpMsg);
	//Ʊ��������
	void MakeTicketComHdr(TicketComHdr_t& ticketComHdr,LPBYTE lpMsg);
	//����Ա������
	void MakeOperatorComHdr(OperatorComHdr_t& operatorComHdr,LPBYTE lpMsg);
	//
	//void Make
	
	//��ȫ������
	void MakeSysSecurityHdr(SysSecurityHdr_t& sysSecurityHdr,LPBYTE lpMsg);
	// ҵ������ͷ
	void MakeOperComHdr(OperComHdr_t& OperComHdr, LPBYTE lpMsg);
	//���׹�������ͷ
	void MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg);
	
	void MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg);

	void MakeBOMAR(DEVICE_AR_REASON arReason,VARIABLE_DATA& msg);
	/**********************************************************************************
	// AFC�ڲ�����
	***********************************************************************************/
	// BOM��TVM�����˿�
	void MakeBOMRefundTVM(AfcSysHdr_t & afcHeader,BOM_REFUND_TVM& bom_refund_tvm, LPBYTE lpByte);
    // �Ǽ�ʱ�˿�����
	//void MakeDelayRefound(DELAY_REFUND, LPBYTE lpByte);
	// ��Ʊ���ս���
	void MakeRecycleInvalidTicket(AfcSysHdr_t & afcHeader,INVALIDE_TICKET_RECYCLETRADE& recycleTrade, LPBYTE lpByte);
	// �������۳�Ʊ����
    //void MakeRecycleIssuedTicket(RECYCLE_ISSUED_TICKETTRADE recycleIssuedTrade, LPBYTE lpByte);
	void MakeBomNonBusinessIncome(AfcSysHdr_t & afcHeader,TransComHdr_t& transHeader,CString& reason,int amount,CString& memo, LPBYTE lpByte);

	//�������
	void MakeTransactionAudit(AfcSysHdr_t & afcHeader,TxnAuditData&,VARIABLE_DATA& msg);

	//	//����������
	void MakeSignCardApply(AfcSysHdr_t & afcHeader,SignCardApply& signCardApply,LPBYTE lpMsg);
	//�������޸�
	void MakeSignCardModify(AfcSysHdr_t & afcHeader,SignCardApply& signCardModify,LPBYTE lpMsg);
	//��ʧ
	void MakeReportLoss(AfcSysHdr_t& afcHeader,ReportLoss& reportLoss,LPBYTE lpMsg);
	//���
	void MakeCancelReportLoss(AfcSysHdr_t& afcHeader,CancelReportLoss& reportLoss,LPBYTE lpMsg);
	//��������
	void MakeAdministrationProcess(AfcSysHdr_t& afcHeader,FinanceComHdr_t &, AdministrationProcess& process,LPBYTE lpMsg);
	//�Ǽ�ʱ�˿�����
	void MakeNonImmidatelySurrenderApply(AfcSysHdr_t&,NonImmidatelySurrenderApply&, LPBYTE lpByte);
	//�Ǽ�ʱ�˿������ѯ
	void MakeQueryNonImmidatelySurrenderApply(AfcSysHdr_t&,QueryNonImmidatelySurrenderApply&, LPBYTE lpByte);
	//�Ǽ�ʱ�˿�
	void MakeNonImmediatelySurrender(AfcSysHdr_t&,NonImmidatelySurrender&,LPBYTE lpByte);
	//����������
	void MakeQuerySignCardApplyStatus(AfcSysHdr_t&,QuerySignCardApply&,LPBYTE);
	/**********************************************************************************
	// ҵ������
	***********************************************************************************/
	//--------------------------------ҵ������-----------------------------------------
	// �����ͳ������
	long MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte);
	// �����ͳ�������ҵ������
	void MakeDownloadParameterAndProgram(OperComHdr_t& operComHdr,vector<ParameterAndSoftwareUpdate>& updates, LPBYTE lpByte);
	// �豸��Ҫ״̬�ı�
	void MakeImportantStatusChange(OperComHdr_t& operComHdr, LPBYTE lpByte, SERVICE_STATUS status);
	// ������־
	void MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte);
	// �豸��������
	void MakeDeviceErrorData(OperComHdr_t& operComHdr,DEVICE_ERRORDATA&, LPBYTE lpByte);
	// �豸�Ĵ�������
	void MakeDeviceRegisterData(OperComHdr_t& operComHdr, ACCAR& arData,AR_REASON, LPBYTE lpByte,DWORD& dataLen);
	// Ʊ�����
	void MakeMagazineExchange(OperComHdr_t& operComHdr, MAGAZINE_CHANGE& magazine_change,LPBYTE lpByte);
	// �豸��������
	void MakeDeviceComponentInfo(OperComHdr_t& operComHdr, LPBYTE lpByte);
	//--------------------------------�¼�����---------------------------------------
	// �˻�����
	void MakeAccountLock(OperComHdr_t& operComHdr, LPBYTE lpByte);
	// �豸��ˮ����
	void MakeDeviceUdsnNoReset(OperComHdr_t& operComHdr, UDSN_RESET& udsnRest, UDSN_RESET_REASON& resetReason, LPBYTE lpByte);
	// ��������
	void MakeDeviceComponentExchange(OperComHdr_t& operComHdr, LPBYTE lpByte, BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);
	// Ʊ���汨��
	void MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen);
	// BOM�������
	void MakeBomAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData,AUDIT_FLAG auditflag, LPBYTE lpdata, DWORD& datalen);
	// �������������
	void MakePackageMsgTransAudit(OperComHdr_t& operComHdr, LPBYTE lpByte);

	/**********************************************************************************
	// ��������
	***********************************************************************************/
	//---------------------------------��������---------------------------------------
	// �ϴ�ָ���������������
	// �ϴ�ָ��ʱ����������
	// �ϴ��豸��������
	// ��Ӫģʽ��������
	// �豸���п�������
	// 24Сʱ��Ӫ��������
	// �ӳ���Ӫʱ������
	// ǿ��ʱ��ͬ������
	void MakeForceTimerSynchronize(LPBYTE, LPBYTE);
	// ��Ʊ��������
	// �ϴ������ͳ���汾
	// �����ͳ����������
	// ǿ���˳���¼
	// �����ͳ���汾�л�����
	// �豸״̬��ѯ
	// �Զ����в������ÿ���
	//---------------------------------��������-------------------------------------
	// ��¼�ǳ�����
	//void makeLoginOrLogoutRequest(LOGIN_TYPE LoginType,LPBYTE lpByte);
	void MakeLoginOrLogoutRequest(LOGIN_TYPE LoginType, int nOperatorID, LPBYTE nPwd, LPBYTE lpByte);
	// �����޸�����
	void MakeReformPasswordRequest(CHANGE_STAFFPASSWORD&, LPBYTE lpByte);
	// ��ʱ��Ч�����·�����
	void MakeImmediatelyParameterDownloadRequst(LPBYTE lpData, WORD);
	// ������֤����
	void MakeConnectAuthRequest(LPBYTE lpByte);
	//---------------------------------֪ͨ����--------------------------------------
	// �˻�����֪ͨ
	// FTP������Ϣ֪ͨ
	// ��������֪ͨ
	void MakeOpEndNotice(LPBYTE lpByte,NOTICE_TYPE);
	//---------------------------------��������-----------------------------------
	// ��¼�ǳ�������
	// �����޸�������
	// ��ʱ��Ч�����·�������
	// ������֤������
	// �豸�����ͳ���汾����
	void MakeParamAndSoftwareVersionInfo(LPBYTE lpByte,int &length);
	/**********************************************************************************
	// ״̬����
	***********************************************************************************/
	// BOM����״̬����
	void MakeBomFullStatus(STATUS_TYPE statusType, LPBYTE lpByte);
	void MakeBomFullStatus(BOM_STATUS_ID lpStatusID, BYTE bValue,LPBYTE lpByte);

	// �豸���״̬����
	void MakeDeviceStatusChange(LPBYTE lpByte, BOM_STATUS_ID lpStatusID, BYTE bValue);
	// �豸����״̬����
	void MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode);


private:
	static CSCMessageMakeHelper instance;
};