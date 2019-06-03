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
		��ȡ������
	***********************************************************************************/
	// AFCϵͳר��ͷ
	AfcSysHdr_t	AquireAfcSysHeader(WORD wDataLen);
	// ϵͳ������SysComHdr_t
	SysComHdr_t AquireSysComHeader(WORD msgType);
	// Ʊ��������SysCardCom_t
	SysCardCom_t AquireSysCardComHeader();
	// �ֿ��˹�����SysCardholderCom_t
	SysCardholderCom_t AquireSysCardholderComHeader();
	// �ڲ��������ݹ�����
	TransComHdr_t AquireTransComHeader(WORD msgType);
	// ҵ������ר��ͷ
	OperComHdr_t AquireOperHeader(WORD msgType,WORD wDataLen);
	// ״̬����ͷ
	StatusComHdr_t AquireStatusComHeader(WORD msgType);

	/**********************************************************************************
		���ù�����
	***********************************************************************************/
	// AFCϵͳ����ͷ
	void MakeAfcSysHdr(AfcSysHdr_t& afcSysHder,LPBYTE lpMsg);
	// ϵͳ������
	void MakeSysComHdr(SysComHdr_t& sysComHdr,LPBYTE lpMsg);
	// Ʊ��������
	void MakeSysCardCom(SysCardCom_t& sysCardCom,LPBYTE lpMsg);
	// �ֿ��˹�����
	void MakeSysCardholderCom(SysCardholderCom_t& sysCardholderCom,LPBYTE lpMsg);
	// �ڲ��������ݹ�����
	void MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg);
	// ҵ������ͷ
	void MakeOperComHdr(OperComHdr_t& OperComHdr, LPBYTE lpMsg);
	// ״̬����ͷ
	void MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg);
	// ��ȫ������
	void MakeSysSecurityHdr(SysSecurityHdr_t& sysSecurityHdr,LPBYTE lpMsg);

	/**********************************************************************************
		ACC����
	***********************************************************************************/
	// ACC�豸�Ĵ�������
	void MakeTxnAuditRegister(TXN_AUDIT_REGISTER& TxnAuditRegister,ACCAR& arData,LPBYTE lpMsg,LPBYTE lpAfcHead);
	// CD�汾�л�
	void MakeTxnEventCDVersionStatus(TXN_EVENT_CD_VERSION_STATUS& TxnEventCDVersionStatus,LPBYTE lpMsg,LPBYTE lpAfcHead);
	// �豸״̬
	void MakeTxnEventDeviceStatus(TXN_EVENT_DEVICE_STATUS& TxnEventDeviceStatus,LPBYTE lpMsg,LPBYTE lpAfcHead);
	// �豸��ʶ
	void MakeTxnEventDeviceIdentification(TXN_EVENT_DEVICE_IDENTIFICATION& TxnEventDeviceIdentification,LPBYTE lpMsg,LPBYTE lpAfcHead);

	/**********************************************************************************
		ҵ������
	***********************************************************************************/
	// TVM��AVM���ϵ�ҵ������
	void MakeBusinessFaliedData(OperComHdr_t& operComHdr,BUSINESS_FAILED_DATA& inBusinessFailedData,LPBYTE lpByte);
	// �����ͳ�������
	void MakeDownloadParameterAndProgram(OperComHdr_t& operComHdr,vector<ParameterAndSoftwareUpdate>& updates, LPBYTE lpByte);
	// �����ͳ������
	long MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte);
	// �豸��Ҫ״̬�ı�
	void MakeImportantStatusChange(OperComHdr_t& operComHdr, LPBYTE lpByte, SERVICE_STATUS status);
	// SAM��ǩ��ǩ��
	void MakeIsamCheckIn(OperComHdr_t operComHdr, LPBYTE lpByte, ISAM_AUTH isam_auth, BYTE result);
	// �豸��������
	void MakeDeviceErrorData(OperComHdr_t& operComHdr,DEVICE_ERRORDATA&, LPBYTE lpByte);
	// ������־
	void MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte);
	// �豸�Ĵ�������
	void MakeDeviceRegisterData(OperComHdr_t& operComHdr, DEVICEREG_AR& arData,AR_REASON, LPBYTE lpByte,DWORD& dataLen);
	// Ʊ�����
	void MakeMagazineExchange(OperComHdr_t& operComHdr, MAGAZINE_CHANGE& magazine_change,LPBYTE lpByte);
	// Ǯ�����
	void MakeCashBoxExchange(OperComHdr_t& operComHdr, CASHBOX_CHANGE& cash_change,LPBYTE lpByte);

	/**********************************************************************************
		�¼�����
	***********************************************************************************/
	// �˻�����
	void MakeAccountLock(OperComHdr_t& operComHdr, LPBYTE lpByte);
	// �豸��ˮ����
	void MakeDeviceUdsnNoReset(OperComHdr_t& operComHdr, UDSN_RESET& udsnRest, UDSN_RESET_REASON& resetReason, LPBYTE lpByte);
	// ��������
	void MakeDeviceComponentExchange(OperComHdr_t& operComHdr, LPBYTE lpByte, BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);
	// SAM������ ��MakeDeviceComponentExchange��ͬ
	// AVM��TVM�������
	void MakeTVMAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData, LPBYTE lpdata/*, DWORD& datalen*/);

	/**********************************************************************************
		�����������
	***********************************************************************************/
	// �������������
	void MakePackageMsgTransAudit(OperComHdr_t& operComHdr, LPBYTE lpByte);

	/**********************************************************************************
		״̬����
	***********************************************************************************/
	// TVM����״̬����
	void MakeTVMFullStatus(STATUS_TYPE statusType, LPBYTE lpByte);
	void MakeTVMFullStatus(TVM_STATUS_ID lpStatusID, BYTE bValue,LPBYTE lpByte);
	// �豸���״̬����
	void MakeDeviceStatusChange(LPBYTE lpByte, TVM_STATUS_ID lpStatusID, BYTE bValue);
	// �豸����״̬����
	void MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode);

	/**********************************************************************************
		��������
	***********************************************************************************/
	// ǿ��ʱ��ͬ������
	void MakeForceTimerSynchronize(LPBYTE, LPBYTE);

	/**********************************************************************************
		��������
	***********************************************************************************/
	// ��¼�ǳ�����
	void MakeLoginOrLogoutRequest(LOGIN_TYPE LoginType, int nOperatorID, LPBYTE nPwd, LPBYTE lpByte);
	// �����޸�����
	void MakeReformPasswordRequest(CHANGE_STAFFPASSWORD&, LPBYTE lpByte);
	// ���Ի���Ϣ��ѯ
	void MakePersonalizationRequest(PersonalizationQuery&,LPBYTE lpByte);
	// ��ʷ��Ϣ��ѯ
	void MakeHistoryInfoRequest(HistoryInfoQuery&,LPBYTE lpByte,DWORD& lenth);
	// ��ʱ��Ч�����·�����
	void MakeImmediatelyParameterDownloadRequst(LPBYTE lpData, WORD);
	// ������֤����
	void MakeConnectAuthRequest(LPBYTE lpByte);
	// ��ԮՈ��
	void MakeCallHelpRequest(LPBYTE lpByte,bool bIsCallOrCancel);

	/**********************************************************************************
		��������
	***********************************************************************************/
	// �豸�����ͳ���汾����
	void MakeParamAndSoftwareVersionInfo(LPBYTE lpByte,int &length);

	/**********************************************************************************
		֪ͨ����
	***********************************************************************************/
	// �������֪ͨ
	void MakeOpEndNotice(LPBYTE lpByte,NOTICE_TYPE);


	/**********************************************************************************
		����
	***********************************************************************************/
	// AR
	void MakeDeviceAR(DEVICE_AR_REASON arReason,VARIABLE_DATA& msg);
	// ��Ʊ���ս���
	void MakeRecycleInvalidTicket(AfcSysHdr_t & afcHeader,INVALIDE_TICKET_RECYCLETRADE& recycleTrade, LPBYTE lpByte);
	//--------------------------------ҵ������-----------------------------------------
	
	// �豸��������
	void MakeDeviceComponentInfo(OperComHdr_t& operComHdr, LPBYTE lpByte);
	//--------------------------------�¼�����---------------------------------------
	// Ʊ���汨��
	void MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen);

	// Ǯ���汨��
	void MakeReportMoneyBoxCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dwMsgLen);

	// ֽ��������¼�
	void MakeClearBanknotesOrCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount,WORD& wMsgLen);
	// Ӳ��������¼�
	//void MakeClearCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount);
private:
	static					CSCMessageMakeHelper instance;
	DWORD                   TranCoinTypeToValue(int nType);
	BankNoteAndCoinType_t	TranCoinType(int nType);
	DWORD					TranTypeToValue(BankNoteAndCoinType_t type);
	DWORD					TranBanknoteToValue(int nType);
	BankNoteAndCoinType_t   TranBanknoteToType(int nType);
};