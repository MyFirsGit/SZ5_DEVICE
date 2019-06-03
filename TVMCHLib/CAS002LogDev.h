// CAS002LogDev.h: interface for the OCAS002LogDev class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAS002LOGDEV_H__FA7F1E0A_27DE_4384_9847_61302CEBE8CF__INCLUDED_)
#define AFX_CAS002LOGDEV_H__FA7F1E0A_27DE_4384_9847_61302CEBE8CF__INCLUDED_

#include "OdevBase.h"
#include "CAS002DevStruct.h"

class AFX_EXT_CLASS OCAS002LogDev
{
public:
	OCAS002LogDev();
	virtual ~OCAS002LogDev();

	int iSetCommPara(tDevReturn* p_psStatus); // ����ͨѶ����	
	int iInit(tModuleStatus* p_psModule, tDevReturn* p_psStatus); // ��ʼ��

	// Ӳ�Ҵ���ģ�鹦�ܺ���
	int iGetDevStatus(tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // ȡ�豸״̬
	int iGetDevVersion(tModuleVersion* p_psDevVersion, tDevReturn* p_psStatus); // ȡ���ʳ���汾��
	int iGetVersion(char *p_pcVersion); // ��ȡ����汾
	
	int iStartRevCoin(tModuleStatus *p_psModuleStatus, tDevReturn *p_psStatus); // ��ʼ����Ӳ��	
	int iStopRevCoin(tModuleStatus *p_psModuleStatus, tDevReturn *p_psStatus); // ��������Ӳ��

	int iChange(const UINT p_wChange, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // �Զ�����	
	int iChangeEx(tChange* p_sChange, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // �ֶ�����	

	int iReadCoinNum(_tCoinInfo* p_psCoinInfo, tDevReturn* p_psStatus); // ��ȡ���ս��
	int iWriteCoinNum(tSetCoinNum* p_sCoinNum, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // ���ý��
	
	int iClearCoinFlowNum(tCoinFlow *p_psCoinFlow, tDevReturn *p_psStatus); // ���Ӳ�����������ͳ����Ϣ
	int iReadCoinFlowNum(tCoinFlow *p_psCoinFlow, tDevReturn *p_psStatus);  // ��ȡӲ�����������ͳ����Ϣ
	
	int iEjectCoin(tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // �˳�Ӳ��
	int iClearCoin(tClearCoin* p_sClearCoin, UINT& p_wHopper1Num, UINT& p_wHopper2Num, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // ���
	
	int iAcceptCoinEx(tModuleStatus* p_psModuleStatus, UINT& p_wCoinType1Num, UINT& p_wCoinType2Num, tDevReturn* p_psStatus, const BYTE p_byDir = 1); // �ֶ�����Ӳ�ң��ϲ�ָ������λ��
	int iAcceptCoin(tModuleStatus* p_psModuleStatus, UINT& p_wCoinType1Num, UINT& p_wCoinType2Num, tDevReturn* p_psStatus); // �Զ�����Ӳ�ң�����Ǯ��
	
	int iDenominate(UINT p_wChange, tChange* p_psChange, tDevReturn* p_psStatus); // �߼��䳮
	int iSaveRecord(tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // �����¼
	int iReadAuditData(tAuditData* p_psAuditData, tDevReturn* p_psStatus); // �������Ϣ
	int iReadCamInfo(UINT& p_wLoopNum, UINT& p_wReturnNum, tDevReturn* p_psStatus); // ��ѯ͹��ת���������˱���

	int iBeep(BYTE p_byBeepTimes, BYTE p_byBeepLength,  BYTE p_byBeepInterval, tDevReturn* p_psStatus); // ����������
	int iRouterControl(const UINT p_wDivNo, tModuleStatus *p_psModuleStatus, tDevReturn *p_psStatus); // ����������
	int iEscrowControl(const UINT p_wEscrowNo, tModuleStatus* p_psModuleStatus, tDevReturn *p_psStatus); // �ݴ�������
	
	int iReadSN(BYTE p_abySerialNo[20], tDevReturn* p_psStatus); // ��SN��
	int iWriteSN(BYTE p_abySerialNo[20], tDevReturn* p_psStatus); // дSN��

	int iReadiButtonID(const BYTE p_byiButtonNo, BYTE* p_pbyiBtnID, tDevReturn* p_psStatus); // ��iButton ID��
	int iCleariButton(const BYTE p_byiButtonNo, tDevReturn* p_psStatus); // ���iButton��������
	int iReadiButton(const BYTE p_byiButtonNo, tiButtonData* p_psReadData, tDevReturn* p_psStatus); // ��iButton
	int iWriteiButton(const BYTE p_byiButtonNo, tiButtonData* p_psWriteData,  tDevReturn*  p_psStatus); // дiButton

	int iClearLog(tDevReturn* p_psStatus);
	int iReadLog(unsigned char p_pcLogDate[3], tDevReturn* p_psStatus);
	int iGetLogTableInfo(tLogTableInfo *p_psLogTable, tDevReturn *p_psStatus);

	// ͼ��ʶ��ģ�����
	int iDownLoadData(char* p_pcFilePath, tDevReturn* p_psStatus); //��������
	int iDownLoadUpdate(tDevReturn* p_psStatus);	// ���³���
	int iSetTime(tDevReturn* p_psStatus);	// ����ʱ��

	ODevBase* oGetODevBase(); // ��ȡ odevbase ָ��

	int iSetRejectType(const tRejectInfo* p_psRejectInfo, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // Ӳ�Ҿ��տ���
	
	//added by hguang 2012-04-27
	int iReadSN(tCAS002_Data* p_psData, tDevReturn* p_psStatus); // ��SN
	
	int iRFWriteBlock(const tCAS002_RFIDInfo* p_psRfInfo, const tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iRFReadBlock(const tCAS002_RFIDInfo* p_psRfInfo, tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iRFWriteSector(const tCAS002_RFIDInfo* p_psRfInfo, const tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iRFReadSector(const tCAS002_RFIDInfo* p_psRfInfo, tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	int iGetRFIDCardInfo(unsigned char p_ucPortId, tCAS002_Data* p_psData, tDevReturn *p_psrStatus);
	
	int iGetCoinBoxSN(const unsigned char p_ucCoinBoxId, tCAS002_Data* p_psData, tDevReturn* p_psrStatus); 
	int iSetCoinBoxSN(const unsigned char p_ucCoinBoxId, const tCAS002_Data* p_psData, tDevReturn* p_psrStatus); 
	//end added
	int iRouterControlEx(const UINT p_wDivNo, const UINT p_wDir, tModuleStatus* p_psModuleStatus, tDevReturn* p_psStatus); // ����������

	//added by ZacharyLiu 2015-12-03
	int iGetDeviceCfgPara(BYTE p_byParaType, tCAS002C_CfgPara* p_psCfgPara, tDevReturn* p_psStatus);
	int iSetDeviceCfgPara(BYTE p_byParaType, tCAS002C_CfgPara* p_psCfgPara, tDevReturn* p_psStatus);
	
	//ά���ӿ�
	int iControlGate(BYTE p_byAction, tDevReturn* p_psStatus);
	//end added

	//added by lfan 2016-02-18
	int iSetCommParaEx(tDevReturn* p_psStatus); // ����ͨѶ����,����ȡ�汾�ţ���������ʱʹ��
	//end added

private:
	class OCAS002LogBase* m_pCAS002LogBase;	// �߼���ʵ����ָ��
};

#endif // !defined(AFX_CAS002LOGDEV_H__FA7F1E0A_27DE_4384_9847_61302CEBE8CF__INCLUDED_)
