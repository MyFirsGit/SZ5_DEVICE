#include "stdafx.h"
#include "TVMRWHelper.h"
#include "tpuerrorinfo.h"
#include "tvmsetting.h"
#include "Sync.h"
#define PARAM_HEAD_LEN	36		// ����ͷ����
/**
@brief ��д��Helper
*/
RWID CTVMRWHelper::CARD_RWID(INVALID_HANDLE_VALUE,FILTER_CARD_RW_LOG,TVM_CARD_RW);
RWID CTVMRWHelper::TOKEN_RWID(INVALID_HANDLE_VALUE,FILTER_TOKEN_RW_LOG,TVM_TOKEN_RW);
RWID CTVMRWHelper::CHARGE_RWID(INVALID_HANDLE_VALUE,FILTER_RECHARGE_RW_LOG,TVM_RECHARGE_RW);

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTVMRWHelper::CTVMRWHelper(RW_TYPE rwType):m_RWID(rwType == TVM_CARD_RW ? CARD_RWID : rwType == TVM_TOKEN_RW ? TOKEN_RWID : CHARGE_RWID )
{
	m_RWType = rwType;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTVMRWHelper::~CTVMRWHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���д��

@param       (i)bool reset      �Ƿ�reset   true:ִ�� TWReset, false:��ִ�� RWReset

@retval      ��

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::InitCheck(bool reset)
{
	try {
		// ���Ӷ�д��
		long errCode = Open();

		// ��λ��д������ȡ�汾����ȡRW��SAM ID��
		RW_RSP_STATUS status;
		if (reset) {			
			errCode = Reset(status);
		}

		// ��ȡTPU�汾��SAM����Ϣ
		RW_RSP_TPU_INFO tpuInfo;
		errCode = GetTPUInfo(tpuInfo);
		if(errCode == RW_RSP_OK){
			if (m_RWType == TVM_CARD_RW){
				theAPP_SESSION.SetSAMInfo(TH_ECT_ISAM, tpuInfo.bmacISAMid);
				theAPP_SESSION.SetSAMInfo(TH_ECT_PSAM, tpuInfo.bmacPSAMid);
				theAPP_SESSION.SetSAMInfo(TH_ACC_ISAM, tpuInfo.accISAMid);
				theAPP_SESSION.SetSAMInfo(TH_ACC_PSAM, tpuInfo.accPSAMid);
			}
			/*if (m_RWType == TVM_TOKEN_RW){
			theAPP_SESSION.SetSAMInfo(RW_ECT_ISAM, tpuInfo.bmacISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ECT_PSAM, tpuInfo.bmacPSAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_ISAM, tpuInfo.accISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_PSAM, tpuInfo.accPSAMid);
			}
			if (m_RWType == TVM_RECHARGE_RW){
			theAPP_SESSION.SetSAMInfo(RW_ECT_ISAM, tpuInfo.bmacISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ECT_PSAM, tpuInfo.bmacPSAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_ISAM, tpuInfo.accISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_PSAM, tpuInfo.accPSAMid);
			}		*/	
		
			DWORD appVer = 0;
			CString strVer;
			strVer.Format(_T("%01X%02X%02X%02X%02X"),tpuInfo.softwareVersionApp[1] & 0x0F,tpuInfo.softwareVersionApp[2],tpuInfo.softwareVersionApp[3],tpuInfo.softwareVersionApp[4],tpuInfo.softwareVersionApp[5]);
			//memcpy(&appVer,tpuInfo.softwareVersionApp,4);
			appVer = _ttoi(strVer);
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,false),appVer);
		}
	}
	catch (CSysException) {
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,true),0);
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,false),0);
		throw;
	}
	catch (...) {
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,true),0);
		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,ConvertRWTypeToProgType(m_RWType,false),0);
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��鲿���Ƿ���������Ͳ�����������

@param       �� 

@retval      ��

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::CheckDeviceExchange()
{
	try {
		// ��ȡTPU�豸ID
		RW_RSP_TPU_INFO tpuInfo;
		long errCode = GetTPUInfo(tpuInfo);
		if(errCode == RW_RSP_OK){
			//// ��鲿��ID�Ƿ���
			//CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
			//theDeviceInfo.GetDeviceInfo(deviceInfo);
			//if(memcmp(tpuInfo.DeviceCode,deviceInfo.TW_DEVICE_ID,sizeof(deviceInfo.TW_DEVICE_ID)) != 0){
			//	// �����豸������������
			//	//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(DEVICE_INNER_TPU,tpuInfo.DeviceCode,deviceInfo.TW_DEVICE_ID);
			//	theDeviceInfo.SetTWId(tpuInfo.DeviceCode);
			//}

			//// �жϷ���SMA����λ
			//RW_RSP_TPU_INFO rsp_compare;
			//if(memcmp(tpuInfo.accISAMid,rsp_compare.accISAMid,sizeof(rsp_compare.accISAMid)) == 0){
			//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
			//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
			//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
			//}
			//if(memcmp(tpuInfo.accPSAMid,rsp_compare.accPSAMid,sizeof(rsp_compare.accPSAMid)) == 0){
			//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
			//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
			//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
			//}
			//if(memcmp(tpuInfo.bmacPSAMid,rsp_compare.bmacPSAMid,sizeof(rsp_compare.bmacPSAMid)) == 0){
			//	//theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_FAILURE);
			//	theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_SAM_ERROR);
			//	throw CRWHardwareException(RW_TYPE::TVM_CARD_RW,RW_RSP_ACCISAMERR,_T(__FILE__),__LINE__);
			//}	

			//// �ϱ�SAM��״̬
			////theTVM_STATUS_MGR.SetSam1Type(SAM_STATUS::SAM_STATUS_LRT_NORMAL);
			//// �ϱ���д��״̬
			//theTVM_STATUS_MGR.SetRW1Status(STATUS_RW::RW_NORMAL);
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ����д��

@param       �� 

@retval      ��

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::InitData()
{
	RW_CMD_INIT initInput;
	// һֱѰ��
	initInput.ReadTimeout = 0;
	// �豸ID
	//theMAINTENANCE_INFO.GetCurrentDevice().Serialize(initInput.DeviceId);
	CString deviceId;
	deviceId.Format(_T("%.2d%.2d%.2x%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	initInput.DeviceId = _ttoi(deviceId);
	// �豸����
	//initInput.DeviceType = MAKEWORD(0x00,theMAINTENANCE_INFO.GetCurrentDevice().bAfcDeviceType);
	DWORD DeviceType = MAKEWORD(theMAINTENANCE_INFO.GetCurrentDevice().bAfcDeviceType,0);

	//  ��վ����
	//DWORD dwLocationNum = theACC_LOCATION.GetStationLocationNumber(theMAINTENANCE_INFO.GetStationCodeWithLine());
	initInput.StationId = ComMakeLong(HIBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),LOBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),0,0);//dwLocationNum;

	// ��Ӫ��˾����뷽
	initInput.AgentCode = 99;	//����һ��ͨ
	// ��Ӫ����
	initInput.OperationDate = ComGetBusiDate();
	 // ��ǰ��������
	initInput.CurrentDate = ComGetCurDate();
	// �豸����ģʽ
	initInput.TestMode = theMAINTENANCE_INFO.GetDeviceTestMode();	//0x00����ʾ����ģʽ��0x01����ʾ����ģʽ
	// ��վ��Ӫģʽ �μ�ModeCode_t				
	initInput.OperationMode = theTVM_STATUS_MGR.GetOperationMode();
	// ����Ա�ı��,�μ�OperateID_t
	initInput.OperaterID = 0;//_ttoi(theAPP_SESSION.GetUserInfo().sUserID);��Զ��Ϊ��
	// �������� 01 02 03���ֱ��ʾ˫���ߣ�A���ߣ�B����	
	initInput.AntennaConfig = 0x01;			
	//0x00���������ϰ����˳�վ��0x01�������ϰ����˳�վ��
	initInput.ChangeStationSign = 0;

	// "ͨѶ������IP��ַ, ͨѶ�˿�,����ͨѶ��ʱʱ��"���ַ�����ʽ���ԡ�\0����β�����磺"10.8.3.45,50031,5"
	CString strYPTonlinePara = _T("");
	strYPTonlinePara.Format(_T("%s,%d,%d\0"),theMAINTENANCE_INFO.GetYPTHostID(),theMAINTENANCE_INFO.GetYPTPort(),theMAINTENANCE_INFO.GetYPTTimeOutTime());
	USES_CONVERSION;
	LPSTR YPTonlinePara = T2A(strYPTonlinePara);	
	memcpy(initInput.YPTonlinePara,YPTonlinePara,strlen(YPTonlinePara));
	CString strECTonlinePara = _T("");
	strECTonlinePara.Format(_T("%s,%d,%d\0"),theMAINTENANCE_INFO.GetECTHostID(),theMAINTENANCE_INFO.GetECTPort(),theMAINTENANCE_INFO.GetECTTimeOutTime());
	LPSTR ECTonlinePara = T2A(strECTonlinePara);	
	memcpy(initInput.YKTonlinePara,ECTonlinePara,strlen(ECTonlinePara));

	/*
	ReaderPlace����д�����豸�ϵ�λ�ã�0x00�����������ͣ�
	0x01����բReader��
	0x02����բReader��
	0x05��BOM������
	0x07��TVM������
	0x09��TCM������
	*/
	initInput.ReaderPosition =  0x07; 
	CAfcTVMOperationParam::TPU_PARAM tpuPar;
	theAFC_TVM_OPERATION.GetTpuParam(tpuPar);
	initInput.ErrFareCtrMax = tpuPar.maxWriteErrorCnt;
	initInput.WaitTimeForCardWR = tpuPar.unwriteWaitTime;
	initInput.RetryTimesForCardWR = tpuPar.unwriteRetryTimes;

	// ��ȡTPU״̬
	RW_RSP_STATUS Status = 0;
	GetStatus(Status);

	int iResult = RW_RSP_OK;
	bool hasInit = false;
	while(!hasInit && iResult == RW_RSP_OK){
		switch(Status){
			// ����״̬
			case STATUE_IDLE:
				{
					iResult = Stop(Status);
					break;
				}
				// ��ʼ״̬
			case STATUE_INIT:
				{
					iResult = Reset(Status);
					break;
				}	
				// δ��ʼ��״̬
			case STATUE_NOTINIT:
				{
					iResult = InitTW(initInput,Status);
					hasInit = true;
					break;
				}			
			case STATUE_FINDED:			// ��Ѱ����״̬
			case STATUE_POLLCARD:		// Ѱ��״̬
			case STATUE_TRANSACTION:	// ����״̬			
				{
					iResult = Abort(Status);
					break;
				}
			case STATUE_GETUDDATA:
				{
					iResult = GetAndSaveUDData();
					GetStatus(Status);
					break;
				}
			default:
				{
					throw CRWException(m_RWType,CRWException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__,_tl(TXT_TPU_RETURN_STATUS_ILLEGAL));
				}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����UD����

@param       �� 

@retval       int          0:�ɹ�  ����:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTVMRWHelper::GetAndSaveUDData()
{
	int iRet = -1;
	RW_BUSSINESS_RESPONSE udData;
	iRet = GetUDData(udData);
	if (iRet == RW_RSP_OK)
	{
		SaveRWUDData(udData.UDlen,udData.UDData);
	}
	else{
		int iLastErrCode = 0;
		GetErrCode(iLastErrCode);			
	}
	return iRet;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �����д�����ص�UD��������

@param      (i)WORD txnLen	   ��д�����ص�UD���ݳ���
@param      (i)LPBYTE txnBuf   ��д�����ص�UD��������

@retval     int 0:�ɹ� 1:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMRWHelper::SaveRWUDData(WORD txnLen,LPBYTE txnBuf)
{
	if(txnLen <= 0 || txnBuf == NULL){
		throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	// ��ȡ����ԱID
	int staffId = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	LPBYTE lpBuffer = txnBuf;
	// UD����	
	BYTE udCount = *lpBuffer;
	lpBuffer ++;
	for(int i = 0;i<udCount;i++){
		// �������ݼ�¼���
		BYTE transType = *lpBuffer;
		lpBuffer ++;
		// �������ݼ�¼����
		WORD transLen = mstos(ComMakeWORD(*lpBuffer,*(lpBuffer + 1)));
		lpBuffer += 2;
		VARIABLE_DATA singleTXNData;
		singleTXNData.nLen =transLen + LEN_AFC_SYS_HEADER; 
		singleTXNData.lpData = new BYTE[singleTXNData.nLen];

		LPBYTE lpData = singleTXNData.lpData;
		memset(lpData,0x00,singleTXNData.nLen);
		// ����ԱID
		int2BCD(staffId, (char*)lpData, 3);
		lpData += 3;
		// ��������
		Date2BCD(ComGetBusiDate(),lpData);
		lpData += 4;
		// ��Ϣ����
		ComRevWORD(mstos(singleTXNData.nLen),lpData);		
		lpData +=2;
		// Ԥ��
		*lpData = 0xFF;
		lpData ++;
		// ��Ϣ����
		memcpy(lpData,lpBuffer,transLen);
		lpBuffer += transLen;
		switch(transType){
			case 2: // ACC����
				theACC_TXN_MGR.SaveTxnData(singleTXNData.nLen,singleTXNData.lpData);
				break;
			case 1: // ECT����
				theECT_TXN_MGR.SaveTxnData(singleTXNData.nLen,singleTXNData.lpData);
				break;
			default:
				break;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����д�����ص�AR����

@param      (i)WORD txnLen	   ��д�����ص�AR���ݳ���
@param      (i)DAT::ARDATA txnBuf   ��д�����ص�AR��������

@retval     int 0:�ɹ� 1:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::SaveRWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType)
{
	if(ARDataLen>0)
	{
		ACCAR ar;
		unsigned char* lpData = ARData.ARdata;
		for (int i=0;i<ARData.ARnum;i++)
		{
			WORD arKey =  MAKEWORD(*lpData,*(lpData + 1));
			lpData += 2;
			DWORD arValue = MAKELONG(MAKEWORD(*lpData, *(lpData + 1)),MAKEWORD(*(lpData + 2), *(lpData + 3)));
			lpData += 4;

			if ((arKey<15 && arKey>6)||arKey ==5){
				theDeviceAR.AddProcessStatByType(ticketType,arKey,arValue);
			}
			else{				
				ar[arKey] = (int)arValue;
			}
		}
		theACCAUDIT_MGR.AddAR(ar);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �޸���д��(��ʼ���Լ죬ά����ʹ��)

@param       �� 

@retval      bool   true:�ɹ�; false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::RepareIni()
{
	try {
		// �رն�д��
		Close();
		// �����Լ�
		InitCheck();

		// ͬ����ǰվ�㵽ȫ�����Ʊ��
		RW_CMD_TICKET_PRICE query;
		RW_CMD_TICKET_PRICE_RES result;

		query.CardType = CARD_TYPE_SY_EPT_UL;
		query.ProductCategory = PURSE;
		query.ProductType = SJT;
		query.ProductSubType = 0;
		query.PriceType = 0x01; 
		query.Time = ComGetCurTime();

		WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
		memcpy(query.BegainStation, &currentStation, 4);
		WORD uncurrentStation = 0xFFFF;
		memcpy(query.EndStation,&uncurrentStation, 4);

		long resultCode = CARDRW_HELPER->GetTicketPrice(query, result);
		if(resultCode == RW_RSP_OK){
			theFunction_INFO.SetTicketMaxAmount(result.TicketPrice);
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CRWException(m_RWType,CRWException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       �޸���д��(��ͨҵ������)

@param       �� 

@retval      bool   true:�ɹ�; false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::Repare()
{
	try {
		// �����޸�
		RepareIni();
		// ��ʼ������
		InitData();
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CRWException(m_RWType,CRWException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������

@param     (i)bool IsPaidArea  ��������־ true:������;false���Ǹ�����

@param	   (i)bool IsEpPayment EP֧����־ true:EP֧��������false����EP֧������

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::ReadCard(SERVICE_ID serviceId,bool IsPaidArea,bool IsEpPayment)
{
	// ����ǰ��ֹ��д��
	Abort(true);

	// ����
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//�Ϸ��Լ���־	true:��飻false:�����
	readCard.IsPaidArea = IsPaidArea;	//��������־		true:������;false���Ǹ�����
	readCard.IsReadRecords = false;		//���׼�¼��ȡ��־	true:��ȡ;false:����ȡ
	readCard.PollCardMethod = ANTENNA_MARK_B;
	switch(serviceId){
			// ��������
		case RW_TEST_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.PollCardMethod = ANTENNA_MARK_ALL;
			break;
			// ��Ʊ
		case ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;	
			readCard.PollCardMethod = ANTENNA_MARK_A;
			break;
		case SVT_ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_SVT_ISSUE;
			break;
			// ��Ʊ
		case ADJUST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ADJUST;	
			break;
			// ��ֵ
		case CHARGE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CHARGE;	
			break;
			// ����
		case ANALYZE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		case MONITOR_CARD_RW__SVC:
			readCard.FunctionCode = FUNCTION_CODE_MAINTENANCE_DOOR;
			readCard.PollCardMethod = ANTENNA_MARK_C;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;

	}

	CService* pCallBackService=theSERVICE_MGR.GetService(serviceId);
	if(NULL!=pCallBackService)
	{
		ReadCard(readCard,pCallBackService);
	}

	// �ȴ���������ִ��
	Sleep(100);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������

@param     (i)bool IsPaidArea  ��������־ true:������;false���Ǹ�����
@param     (o)RW_READ_CARD_RESPONSE  ��������

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::TryReadCard(SERVICE_ID serviceId,RW_READ_CARD_RESPONSE& rspReadCard,bool IsPaidArea)
{
	// ����ǰ��ֹ��д��
	Abort(true);

	// ����
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//�Ϸ��Լ���־	true:��飻false:�����  ע��������Ʊ��Ҫ��Ϊ��true����ʱ�����á�
	readCard.IsPaidArea = IsPaidArea;	//��������־		true:������;false���Ǹ�����
	readCard.IsReadRecords = false;		//���׼�¼��ȡ��־	true:��ȡ;false:����ȡ
	readCard.PollCardMethod = ANTENNA_MARK_B;
	switch(serviceId){
		// ��������
		case RW_TEST_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.PollCardMethod = ANTENNA_MARK_ALL;
			break;
		// ��Ʊ
		case ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;	
			readCard.PollCardMethod = ANTENNA_MARK_A;
			break;
		case SVT_ISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_SVT_ISSUE;
			break;
			// ��Ʊ
		case ADJUST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ADJUST;
			readCard.PollCardMethod = ANTENNA_MARK_A;
			break;
			// ��ֵ
		case CHARGE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CHARGE;	
			break;
			// ����
		case ANALYZE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		case MONITOR_CARD_RW__SVC:
			readCard.FunctionCode = FUNCTION_CODE_MAINTENANCE_DOOR;
			readCard.PollCardMethod = ANTENNA_MARK_C;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;
	}

	return TryReadCard(readCard,&rspReadCard);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֹ��д������

@param      (i)bool checkStatus      true:�ȼ���д��״̬; false:������д��״̬

@retval     long   �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Abort(bool checkStatus)
{
	bool mustStop = true;
	// ��ȡ��д��״̬
	RW_RSP_STATUS status;
	if (checkStatus) {		
		GetStatus(status);
		// ����ģʽʱ��������ֹ��д��
		if (status == STATUE_IDLE) {
			mustStop = false;
		}
	}

	long errCode = RW_RSP_OK;
	if (mustStop) {
		errCode = Abort(status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ��д����������

@param      none

@retval     long   �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::StopReadCard()
{
	CRWStopReadCard pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������ͬ������

@param      ��

@retval     bool  true:�ɹ� false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::SynchronismParam()
{
	CParamHelper::VEC_PARAM_ID vecParamId;

	VERSION_ACC versionAcc;
	VERSION_ECT versionEct;
	VERSION_AFC versionAfc;
	DWORD		versionModHistory;
	vecParamId.clear();

	if(GetParameterInfo(versionAcc,versionEct,versionAfc) != 0){
		return false;
	}

	if(versionAcc.lBusinessParam != theACC_BUSINESS.GetCurMasterVer()){
		vecParamId.push_back(ACC_BUSINESS_ID);
	}
	if(versionAcc.lServerFeeParam != theACC_SERVICEFEE.GetCurMasterVer()){
		vecParamId.push_back(ACC_SERVICEFEE_ID);
	}
	if(versionAcc.lOverTimeAdjuestParam != theACC_OVERTIMEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_OVERTIME_UPDATE_ID);
	}
	if(versionAcc.lDeviceControlParam != theACC_DEVICECONTROL.GetCurMasterVer()){
		vecParamId.push_back(ACC_DEVICE_ID);
	}
	if(versionAcc.lStationParam != theACC_LOCATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_STATION_ID);
	}
	if(versionAcc.lPayStationParam != theACC_TOLLSTATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_CHARGE_STATION_ID);
	}
	if(versionAcc.lCardAttributeParam != theACC_PRODUCT.GetCurMasterVer()){
		vecParamId.push_back(ACC_CARDATTRIBUTE_ID);
	}
	if(versionAcc.lBasePriceParam != theACC_BASEPRICE.GetCurMasterVer()){
		vecParamId.push_back(ACC_BASE_PRICE_TABLE_ID);
	}
	if(versionAcc.lPrieLevelParam != theACC_PRICELEVEL.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_LEVEL_TABLE_ID);
	}
	if(versionAcc.lPriceAdjuestParam != theACC_PRICEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_ADJUEST_TABLE_ID);
	}
	if(versionAcc.lTimeFloatParam != theACC_TIMEFLOAT.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_FLOATING_TABLE_ID);
	}
	if(versionAcc.lTimeTypeParam != theACC_TIMETYPE.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_TYPE_TABLE_ID);
	}
	if(versionAcc.lTimeDetailParam != theACC_TIMEDETAIL.GetCurMasterVer()){
		vecParamId.push_back(ACC_TTIME_TYPE_DETAIL_ID);
	}
	if(versionAcc.lDiscountParam != theACC_DISCOUNT.GetCurMasterVer()){
		vecParamId.push_back(ACC_DISSCOUNT_TABLE_ID);
	}
	//if(versionAcc.lNotServStationParam != theACC_NOTSERVSTATION.GetCurMasterVer()){
	//	vecParamId.push_back(ACC_NOT_SERVICE_STATION_ID);
	//}
	if(versionAcc.lWriteListParam != theACC_WRITELIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_WRITE_LIST_ID);
	}
	if(versionAcc.lOtherCityCardParam != theACC_OTHERCITYCARD.GetCurMasterVer()){
		vecParamId.push_back(ACC_OTHER_CITY_CARD_ID);
	}
	if(versionAcc.lSingleBlackListParam != theACC_SINGLEBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SINGLE_BLACK_LIST_ID);
	}
	if(versionAcc.lSectionBlackListParam != theACC_SECTIONBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_BLACK_LIST_ID);
	}
	if(versionAcc.lSectionParam != theACC_SECTION.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_ID);
	}
	if(versionAcc.lissueCompanyParam != theACC_ISSUECOMPANY.GetCurMasterVer()){
		vecParamId.push_back(ACC_ISSUE_COMPANY_ID);
	}


	
	return UpdateParamter(vecParamId);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ���¶�д��ָ������

@param      (i)vecParamId ����ID����

@retval     bool  true:�ɹ� false:ʧ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::UpdateParamter(CParamHelper::VEC_PARAM_ID vecParamId)
{
	bool ret = true;
	CString paramPathName = _T("");
	CString sPrefix = PARAM_PREFIX;	
	CString ParamPath = _T("");

	// ��������ID
	vector<WORD>::iterator iteId = vecParamId.begin();
	while (iteId != vecParamId.end()) {
		// ȡ�ò����ļ�·��
		if(*iteId ==  AFC_MODEHISTORY_ID || *iteId == AFC_DEVICECOMMONPARAM_ID || *iteId == AFC_STAFFPWDPARAM_ID
			||*iteId == AFC_ACCESSLEVELPARAM_ID /*||*iteId == AFC_TVMOPERATIONPARAM_ID*/ ||*iteId == AFC_DEVICERUNTIMEPARAM_ID){
				ParamPath = PARAM_AFC_CURRENT_DIR;
		}
		else if(
			*iteId == ACC_BUSINESS_ID				
			|| *iteId == ACC_SERVICEFEE_ID			
			|| *iteId == ACC_OVERTIME_UPDATE_ID		
			|| *iteId == ACC_DEVICE_ID				
			|| *iteId == ACC_STATION_ID				
			|| *iteId == ACC_CHARGE_STATION_ID		
			|| *iteId == ACC_SECTION_ID				
			|| *iteId == ACC_CARDATTRIBUTE_ID		
			|| *iteId == ACC_ISSUE_COMPANY_ID		
			|| *iteId == ACC_BASE_PRICE_TABLE_ID		
			|| *iteId == ACC_PRICE_LEVEL_TABLE_ID	
			|| *iteId == ACC_PRICE_ADJUEST_TABLE_ID	
			|| *iteId == ACC_TIME_FLOATING_TABLE_ID	
			|| *iteId == ACC_TIME_TYPE_TABLE_ID		
			|| *iteId == ACC_TTIME_TYPE_DETAIL_ID	
			|| *iteId == ACC_DISSCOUNT_TABLE_ID		
			|| *iteId == ACC_NOT_SERVICE_STATION_ID	
			|| *iteId == ACC_SINGLE_BLACK_LIST_ID	
			|| *iteId == ACC_SECTION_BLACK_LIST_ID	
			|| *iteId == ACC_WRITE_LIST_ID			
			|| *iteId == ACC_OTHER_CITY_CARD_ID		
		){
			ParamPath = PARAM_ACC_CURRENT_DIR;
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CRWException(m_RWType,CRWException::PARAM_NOT_EXIST,_T(__FILE__),__LINE__,_tl(TXT_RW_RETURN_PARA_ID_ERROR )+ sParaID);
		}
		paramPathName = thePARAM_HELPER.GetTheParamPathName(*iteId,ParamPath);
		if(thePARAM_HELPER.GetFileNameFromFullPath(paramPathName).GetLength()!=0){
			// ����bin�ļ�����Ϣ
			VARIABLE_DATA data;
			CXFile file(paramPathName);
			file.ReadBinFile(data);

			// ��Ҫȥ��ͷ���ļ���36�ֽڣ�
			if(data.nLen > PARAM_HEAD_LEN){

				// ����RW�ĸ�������
				RW_PARAM_DOWNLOAD input;
				if(*iteId != AFC_MODEHISTORY_ID){
					input.fileData = data.lpData + PARAM_HEAD_LEN;
					input.size = data.nLen - PARAM_HEAD_LEN;
				}
				else{			
					input.fileData = data.lpData;
					input.size = data.nLen;
				}
				input.paramId = /*GetTPUParamID*/(*iteId);

				int iRet = DownloadParamter(input);
				if(iRet != RW_RSP_OK){
					ret = false;
					int iLastErrCode = 0;
					GetErrCode(iLastErrCode);
				}
			}
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CRWException(m_RWType,CRWException::PARAM_NOT_EXIST,_T(__FILE__),__LINE__,_tl(TXT_RW_SYNC_LOCAL_PARAMETER_DOES_NOT_EXIST) + sParaID);
		}
		iteId ++;
		//Sleep(1000);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��д��ָ������ID

@param      (i)vecParamId ����ID����

@retval     bool  true:�ɹ� false:ʧ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CTVMRWHelper::GetTPUParamID(WORD paraID)
{
	//switch(paraID){
	//case ACC_BLACKLIST_ID:// ACC������������
	//	return BLACK_LIST_PARA;
	//case ACC_PRICE_ID:// ACC�Ƽ۷�������
	//	return TICKET_PRICE_PARA;
	//case ACC_STATION_ID:// ACC��Ӫ�����
	//	return STATION_NUM_PARA;
	//case ACC_OPERATION_CONTROL_ID:// ACC��Ӫ���Ʋ���
	//	return SERV_CONTROL_PARA;
	//case ACC_MODE_RESUME_ID:// ACCģʽ�Ěv
	//	return MODE_HISTORY_PARA;
	//case RE_APP_PARA:// Ӧ�ó���
	//	return RE_APP_PARA;
	//}
	return 0;
}

////////////////////////////////////////////ԭ������//////////////////////
//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪶�д������

@param      none

@retval     long   �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Open()
{
	RW_CMD_OPEN cmd_open;
	cmd_open.isUsb = false;
	if (m_RWType == TVM_CARD_RW)
	{
		cmd_open.port = theTVM_SETTING.GetRWComPort();//theTVM_INFO.GetRWCommPort(m_RWType);// ֻ��CARD���͵Ķ�������֣��ֻ��һ��TPU
		cmd_open.baud = theTVM_SETTING.GetRWBaudRate();//theTVM_INFO.GetRWBaudRate(m_RWType);
	}
	else if (m_RWType == TVM_RECHARGE_RW)
	{
		cmd_open.port = theTVM_SETTING.GetChargeRWComPort();//theTVM_INFO.GetRWCommPort(m_RWType);// ֻ��CARD���͵Ķ�������֣��ֻ��һ��TPU
		cmd_open.baud = theTVM_SETTING.GetChargeRWBaudRate();//theTVM_INFO.GetRWBaudRate(m_RWType);
	}
	CRWOpen pRwCmd(cmd_open,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �رն�д������

@param      none

@retval     long   �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Close()
{
	CRWClose pRwCmd(m_RWID);
	long errCode =  pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ��д������

@param      (o)RW_RSP_STATUS& status ��д��״̬

@retval     long   �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Reset(RW_RSP_STATUS& status)
{
	CRWReset pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��д��״̬����

@param      (o)RW_RSP_STATUS& status         ��д��״̬

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetStatus(RW_RSP_STATUS& status)
{
	CRWGetStatus pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&status);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֹ��д������

@param      (o)RW_RSP_STATUS& status         ��д��״̬

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Abort(RW_RSP_STATUS& status)
{
	CRWAbort pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ��д������

@param      (o)RW_RSP_STATUS& status         ��д��״̬

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Stop(RW_RSP_STATUS& status)
{
	CRWStop pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ö�д��ʱ��

@param      (i)_DATE_TIME    ����ʱ��
@param      (o)_DATE_TIME    ���ú��TPUʱ��

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::SetTime(_DATE_TIME currentDateTime,_DATE_TIME& tpuDateTime)
{
	CRWSetTime pRwCmd(currentDateTime,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&tpuDateTime);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��д��ʱ��

@param      (o)_DATE_TIME    TPUʱ��

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTime(_DATE_TIME& tpuDateTime)
{
	CRWGetTime pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&tpuDateTime);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���㽻����֤��(MAC)

@param      (i)BYTE*   ��Ҫ����MAC��Hash���� 20�ֽ�
@param      (o)BYTE*   ���ص�MAC����		 4�ֽ�	

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetMac(BYTE* hashData,BYTE* mac)
{
	WORD keyVersion = 1;
	CRWGetMac pRwCmd(keyVersion,hashData,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(mac);
	}
	return errCode;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU�������

@param      (o)int &iErrorCode    TPU��ϸ�������

@retval     long    �ɹ�:0; ʧ��:����Ӧ���� 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetErrCode(int &iErrorCode)
{
	CRWGetErrCode pRwCmd(iErrorCode,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&iErrorCode);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTPU��Ϣ

@param      (o)DAT::TPUINFO&    TPU��Ϣ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTPUInfo(RW_RSP_TPU_INFO& tpuInfo)
{
	CRWGetTpuInfo pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&tpuInfo);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������

@param      (i)RW_CMD_FIRMWARE input    ��������ṹ��

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::TWFirmwareDownload(RW_CMD_FIRMWARE input)
{
	CRWFirmwareDownload pRwCmd(&input,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if(RW_RSP_OK != errCode) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����д������

@param      (o)RW_RSP_STATUS& status         ��д��״̬

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::InitTW(RW_CMD_INIT initInfo,RW_RSP_STATUS& status)
{
	CRWInit pRwCmd(initInfo,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��

@param      (i)RW_CMD_TICKET_PRICE ticketInfo   ��Ʊ��Ϣ
@param      (o)DWORD& price						Ʊ��

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTicketPrice(RW_CMD_TICKET_PRICE ticketInfo,RW_CMD_TICKET_PRICE_RES& res)
{
	CRWGetTicketPrice pRwCmd(ticketInfo,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	//if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&res);
	//}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ˰�ѽӿ�

@param      RW_CMD_TICKET_TAX		��Ʊ��Ʊ����Ϣ
@param      RW_CMD_TICKET_TAX_RES	������˰��

@retval     long   �ɹ���0��ʧ�ܣ�����Ӧ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetTicketPriceTax(RW_CMD_TICKET_TAX ticketInfo,RW_CMD_TICKET_TAX_RES& taxRsp){
	CRWGetTicketTax getTax(ticketInfo,m_RWID);
	long errCode = getTax.ExecuteCommand();
	if(IsExceptionCode(errCode)){
		throw CRWHardwareException(m_RWType,errCode,_T(__FILE__),__LINE__);
	}
	getTax.GetRWResponse(&taxRsp);
	
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)VERSION_ACC& accVersion   ACC�����汾��Ϣ
@param      (o)VERSION_ECT& ectVersion	 ECT�����汾��Ϣ
@param      (o)VERSION_AFC& afcVersion	 AFC�����汾��Ϣ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetParameterInfo(VERSION_ACC& accVersion,VERSION_ECT& ectVersion,VERSION_AFC& afcVersion)
{
	CRWGetParameterInfo pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		RW_RSP_PARAM_INFO paramData;
		pRwCmd.GetRWResponse(&paramData);
		int paramCnt = paramData.ParaCount;
		WORD paraID = 0;
		DWORD paraVer = 0;
		if(paramCnt>0){
			for(int i = 0;i<paramCnt;i++){
				paraID = ComMakeWORD(paramData.ParamData[i].ParaID[0],paramData.ParamData[i].ParaID[1]); 
				CString pareVersion(paramData.ParamData[i].ParaVer);
				paraVer= _ttoi(pareVersion.Mid(2,10));//(DWORD)ComMakeLong(paramData.ParamData[i].ParaVer[3],paramData.ParamData[i].ParaVer[2],paramData.ParamData[i].ParaVer[1],paramData.ParamData[i].ParaVer[0]);
				switch(paraID){
				case ACC_BUSINESS_ID:			
					accVersion.lBusinessParam = paraVer;
					break;
				case ACC_SERVICEFEE_ID:				
					accVersion.lServerFeeParam = paraVer;
					break;
				case ACC_OVERTIME_UPDATE_ID:			
					accVersion.lOverTimeAdjuestParam	= paraVer;
					break;
				case ACC_DEVICE_ID:		
					accVersion.lDeviceControlParam = paraVer;
					break;
				case ACC_STATION_ID:	
					accVersion.lStationParam = paraVer;
					break;
				case ACC_CHARGE_STATION_ID:		
					accVersion.lPayStationParam	= paraVer;
					break;
				case ACC_CARDATTRIBUTE_ID:		
					accVersion.lCardAttributeParam	= paraVer;
					break;
				case ACC_BASE_PRICE_TABLE_ID:		
					accVersion.lBasePriceParam	= paraVer;
					break;
				case ACC_PRICE_LEVEL_TABLE_ID:		
					accVersion.lPrieLevelParam	= paraVer;
					break;
				case ACC_PRICE_ADJUEST_TABLE_ID:		
					accVersion.lPriceAdjuestParam	= paraVer;
					break;
				case ACC_TIME_FLOATING_TABLE_ID:		
					accVersion.lTimeFloatParam	= paraVer;
					break;
				case ACC_TIME_TYPE_TABLE_ID:		
					accVersion.lTimeTypeParam	= paraVer;
					break;
				case ACC_TTIME_TYPE_DETAIL_ID:		
					accVersion.lTimeDetailParam	= paraVer;
					break;
				case ACC_DISSCOUNT_TABLE_ID:		
					accVersion.lDiscountParam	= paraVer;
					break;
				case ACC_NOT_SERVICE_STATION_ID:		
					accVersion.lNotServStationParam	= paraVer;
					break;
				case ACC_WRITE_LIST_ID:		
					accVersion.lWriteListParam	= paraVer;
					break;
				case ACC_OTHER_CITY_CARD_ID:		
					accVersion.lOtherCityCardParam	= paraVer;
					break;
				case ACC_SINGLE_BLACK_LIST_ID:		
					accVersion.lSingleBlackListParam	= paraVer;
					break;
				case ACC_SECTION_BLACK_LIST_ID:		
					accVersion.lSectionBlackListParam	= paraVer;
					break;
				case ACC_SECTION_ID:		
					accVersion.lSectionParam	= paraVer;
					break;
				case ACC_ISSUE_COMPANY_ID:		
					accVersion.lissueCompanyParam	= paraVer;
					break;
				default:
					try{
						CString  sParaID = _T("");
						sParaID.Format(_T("%.4x"),paraID);  
						throw CRWException(m_RWType,errCode, _T(__FILE__), __LINE__,_tl(TXT_RW_RETURN_PARA_ID_ERROR) + sParaID);
					}
					catch(CSysException& e){
						theEXCEPTION_MGR.WriteExceptionLog(e);// �������ֻд��־
					}
												
				}
			}
		}
	}

	//// ACC ������������TPUʹ�ã�
	//accVersion.lProductParam	   = theACC_PRODUCT.GetCurMasterVer();
	//accVersion.lLocationParam	   = theACC_LOCATION.GetCurMasterVer();
	//accVersion.lFeeParam		   = theACC_FEE.GetCurMasterVer();

	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ز�������

@param      (i)RW_PARAM_DOWNLOAD paramInfo ������Ϣ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::DownloadParamter(RW_PARAM_DOWNLOAD paramInfo)
{
	CRWParamFirmware pRwCmd(paramInfo,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ӫģʽ

@param      (i)RW_CMD_SET_MODE paramInfo ���ƴ���
@param      (o)RW_RSP_STATUS status      ��д��״̬

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::SetOperationMode(RW_CMD_SET_MODE cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetOperationMode pRwCmd(cmdSetMode,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����TPU 24Сʱ��Ӫ���ӳ���Ӫʱ��

@param      (i)RW_CMD_SET_TIME cmdSetMode ���ƴ���
@param      (o)RW_RSP_STATUS status      ��д��״̬

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::SetBusinessTime(RW_CMD_SET_TIME cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetBusinessTime pRwCmd(cmdSetMode,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡUD����

@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::GetUDData(RW_BUSSINESS_RESPONSE& commonResponse)
{
	CRWGetUDData pRwCmd(m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&commonResponse);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)RW_READ_CARD_REQUEST cmdReadCard	   ���������������
@param      (o)RW_READ_CARD_RESPONSE& rspReadCard  �����������ݽṹ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTVMRWHelper::ReadCard(RW_READ_CARD_REQUEST cmdReadCard,CService* pService)
{
	if(NULL!=pService)
	{
		CRWReadCard* pRwCmd = new CRWReadCard(cmdReadCard,m_RWID);
		pRwCmd->SetAutoDeleted(TRUE);
		pRwCmd->SetService(pService);
		pRwCmd->SetNeedCallbackService(true);
		pRwCmd->SetCommandID(RW_COMMAND_ID_READ_CARD);
		EnableRead();
		g_pTVMRWControl->DoCommand(pRwCmd,SP_EXECMODE_QUEUE);
	}else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)RW_READ_CARD_REQUEST cmdReadCard	   ���������������
@param      (o)RW_READ_CARD_RESPONSE& rspReadCard  �����������ݽṹ

@retval     long    �ɹ�:0; ʧ��:����Ӧ����     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::TryReadCard(RW_READ_CARD_REQUEST cmdReadCard,RW_READ_CARD_RESPONSE* rspReadCard)
{
	CRWTryReadCard rwCmd(cmdReadCard,m_RWID);
	long result = g_pTVMRWControl->DoCommand(&rwCmd);
	rwCmd.GetRWResponse(rspReadCard);
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ۿ�����

@param      (i)RW_ISSUE_REQUEST issueInput        �ۿ���������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Issue(RW_ISSUE_REQUEST issueInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWIssue pRwCmd(issueInput,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&response);
	}
	else{
		int errorCode=0;
		GetErrCode(errorCode);
	}
	return errCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��Ʊ����
//
//@param      (i)RW_CMD_ADJUST adjustInput        ��Ʊ��������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Adjust(RW_CMD_ADJUST adjustInput, RW_BUSSINESS_RESPONSE& response)  
//{
//	CRWAdjust pRwCmd(adjustInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	
//		pRwCmd.GetRWResponse(&response);
//	
//	return errCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵ����

@param      (i)RW_CMD_CHARGE chargeInput        ��ֵ��������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Charge(RW_CMD_CHARGE chargeInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWCharge pRwCmd(chargeInput,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �˿�����
//
//@param      (i)RW_CMD_REFUND refundInput        �˿���������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Refund(RW_CMD_REFUND refundInput, RW_BUSSINESS_RESPONSE& response)  
//{
//	CRWRefund pRwCmd(refundInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)RW_CMD_BLOCK blockInput             ������������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::Block(RW_CMD_BLOCK blockInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWBlock pRwCmd(blockInput,m_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&response);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      EP֧������

@param      (i)RW_CMD_CONSUME rwCmd			��������
@param		(o)RW_BUSSINESS_RESPONSE& rwRsp ���׷���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTVMRWHelper::ConsumeEP(RW_CMD_CONSUME rwCmd,RW_BUSSINESS_RESPONSE& rwRsp){
	CRWConsumeEP consumeCmd(rwCmd,m_RWID);
	long errCode = consumeCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)){
		throw CRWHardwareException(m_RWType,errCode,_T(__FILE__),__LINE__);
	}
	if(RW_RSP_OK == errCode){
		consumeCmd.GetRWResponse(&rwRsp);
	}
	return errCode;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������
//
//@param      (i)RW_CMD_BLOCK blockInput        ������������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Counteract(RW_CMD_COUNTERACT conteractInput, RW_BUSSINESS_RESPONSE& response)  
//{
//	CRWCounteract pRwCmd(conteractInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief     ��������
//
//@param      (i)RW_CMD_BLOCK blockInput        ������������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Refresh(RW_CMD_ACTIVE refreshInput, RW_BUSSINESS_RESPONSE& response)
//{
//	CRWRefresh pRwCmd(refreshInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief     ��������
//
//@param      (i)RW_CMD_DEFER deferInput        ������������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Defer(RW_CMD_DEFER deferInput, RW_BUSSINESS_RESPONSE& response)
//{
//	CRWDefer pRwCmd(deferInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      һ��ͨ��ֵ��ʱ����
//
//@param      (i) 
//
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::ChargeOverTimeProcess(RW_CMD_CHARGEOVERTIME chargeOverTime, RW_BUSSINESS_RESPONSE& response )
//{
//	CRWChargeOverTime pRwCmd(chargeOverTime,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      һ��ͨ�ۿ����촦��
//
//@param      (i) 
//
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::IssueInitCheckProcess(RW_CMD_SETCHECK chargeInput,RW_CMD_RESP_SETCHECK &response)
//{
//	CRWSetCheck pRwCmd(chargeInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	if (RW_RSP_OK == errCode) {
//		pRwCmd.GetRWResponse(&response);
//	}
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief     �������ֿ�����Ϣ�޸�����
//
//@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo   �����������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response					ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::UpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo,RW_BUSSINESS_RESPONSE &response)
//{
//	CRWUpdateSignCardInfo pRwCmd(updateSignCardInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//	return errCode;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж϶�д���Ƿ�����

@param      none

@retval     bool true:�쳣 false:���� 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::IsTWHasException()
{
	return theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�Ϊ��Ҫ���쳣��ID

@param      (i)long errCode      ��������ṹ��

@retval     bool \n  true:�쳣�Ĵ������; false:�����쳣��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTVMRWHelper::IsExceptionCode(long errCode)
{
	bool ret = false;

	switch(errCode) {
	//case RW_RSP_STATUSERROR:       // TPU״̬�Ƿ�
	//case RW_RSP_COMERR:            // ͨѶ�ӿ������
	////case RW_RSP_SAMERR:		   // SAM �����ڶ��������������д���
	//	//case RW_RSP_PARAMERR:	   // ���ò��������
	//case RW_RSP_UDERR:		       // UD���������
	//case RW_RSP_HARDWAREERR:	   // Ӳ�������
	//case RW_RSP_UNKNOWERR:		   // δ�������
	case RECV_TIMEOUT:			   // ͨѶ��ʱ
	case SEND_ERR:				   //���ʹ���
	case OPENCOM_ERR:		   //���ڴ�ʧ��
	case RECV_ERR:				   //����ʧ��
	case TPU_POWERDOWN:            //��TPU�ϵ�����
		ret = true;
		break;
	default:
		break;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��д���������

@param      ()bool isMain   �Ƿ��ǻ������������

@retval     int      0:�ɹ�  -2: �����ļ�������   -3:����汾��Ч  ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMRWHelper::UpdateProgram(bool isMain)
{
	int iRet = 0;
	try
	{
		CString fileName = isMain?MID_STR_BOM_RW_PROG_MAIN:MID_STR_BOM_RW_PROG_APP;
		CString pathName = m_RWType == TVM_RECHARGE_RW? TVM_CHARGE_RW_UPDATE_DIR: m_RWType == TVM_CARD_RW ? TVM_CARD_RW_UPDATE_DIR : m_RWType == TVM_TOKEN_RW ? TVM_TOKEN_RW_UPDATE_DIR:_T("");
		vector<CString> updateFiles;

		CString sAppPath;
		GetAppPath(sAppPath);

		CString updateFileFolder = sAppPath + pathName;

		SearchFile(updateFileFolder + fileName + _T("*.*"),updateFiles);
		if(updateFiles.size()<=0)
		{
			LOG("�޶�д�������ļ�");
			//return -2;
			return 0;
		}

		for(int i=0;i<updateFiles.size();i++)
		{
			CString updateFile = updateFiles[i];
			SetFileAttributes(updateFile,FILE_ATTRIBUTE_NORMAL);
			CString upgradeVersionStr = thePARAM_HELPER.GetProgVerFromProgName(updateFile);	// ��ȡ������tpu�汾
			DWORD upgradeVersion = _ttoi(upgradeVersionStr);

			// �汾��Ч
			if(UNAVALIBLE_VERSION == upgradeVersion){
				DeleteFile(updateFile);
				LOG("��д�����³���汾��Ч");
				return -3;
			}

			// ����bin�ļ�����Ϣ
			VARIABLE_DATA data;
			CXFile file(updateFile);
			file.ReadBinFile(data);
			file.Close();

			DeleteFile(updateFile);

			// ����TW�ĸ�������
			RW_CMD_FIRMWARE input;
			input.fileData = data.lpData;
			input.size = data.nLen;
			input.isMain = isMain;

			theAPP_SESSION.SetHasProgUpdate(true);
			theTVM_SETTING.SetCardRWUpdateFlag(1);
			if (RW_RSP_OK == TWFirmwareDownload(input)) {
				theTVM_SETTING.SetCardRWUpdateFlag(0);
				LOG("��д��������³ɹ���");
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,m_RWType == TVM_RECHARGE_RW? CVersionInfo::CHARGE_RW_APP:  CVersionInfo::CARD_RW_APP/* ? CVersionInfo::CARD_RW_APP : m_RWType == TVM_TOKEN_RW ? CVersionInfo::TOKEN_RW_APP:CVersionInfo::CHARGE_RW_APP*/,upgradeVersion);//���±��ذ汾
			}
			else{
				//CVersionInfo::PROG_TYPE progType;
				//if(isMain){
				//	progType = CVersionInfo::RW_MAIN;
				//}
				//else{
				//	progType = CVersionInfo::RW_APP;
				//}
				//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,progType,currentVersion);//����ʧ�ܣ�����TPU��ǰʵ�ʰ汾��
				iRet = -1;
			}
			// �������غ�ȵ�120�뷽�ɽ���Ӧ�ò��������
			if(isMain){
				Sleep(120*1000);
			}
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��д��������Ϣ

@param      (i)int iError   �������

@retval     CStringt     ��д��������Ϣ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMRWHelper::GetTpuErrorInfo(int iError)
{
	return theTPUERROR_INFO.GetTpuErrorInfo(iError);
	/*TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath, _MAX_PATH); 
	CString fileName = CString(szAppPath) + _T("TpuErrorCode.ini");
	return gGetErrorInfofromFile(fileName, iError);*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��д��������Ϣ

@param      (i)int iError   �������

@retval     CStringt     ��д��������Ϣ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CTVMRWHelper::gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode)
{

	CString pstrErrorCode(_T(""));
	pstrErrorCode.Format(_T("0x%08X"), dwErrorCode);

	if (pstrErrorCode == _T("") || pstrFile == NULL)
		return _T("");


	CString strInfo(_T(""));
	try
	{
		CString strFlag1(_T("/*")), strFlag2(_T("*/"));
		//$$CStdioFile file(pstrFile, CFile::typeText | CFile::modeRead);
		CXFile file(pstrFile);
		CString strTxt(_T(""));
		bool bReadInfo = file.ReadString(strTxt);
		while(bReadInfo)
		{
			strTxt.MakeUpper();
			strTxt.Trim();
			if ((strTxt[0] == '\\' && strTxt[1] == '\\')
				|| (strTxt[0] == '/' && strTxt[1] == '*'))
			{
				bReadInfo = file.ReadString(strTxt);
				continue;
			}

			CString strErrorCode  = pstrErrorCode;
			strErrorCode.MakeUpper();
			int nPos = strTxt.Find(strErrorCode);
			if (nPos != -1)
			{
				strInfo.Empty();
				int nPos1 = strTxt.Find(strFlag1);
				int nPos2 = strTxt.Find(strFlag2);
				if (nPos1 != -1 && nPos2 != -1) 
				{
					strInfo = strTxt.Mid(nPos1+strFlag1.GetLength(), nPos2-(nPos1+strFlag1.GetLength()));
				}
				break;
			}

			bReadInfo = file.ReadString(strTxt);
		}
		if (!bReadInfo)
		{
			strInfo = _tl(TXT_UNKOWN_ERROR);
			theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, _tl(TXT_HAVE_NOT_FOUND_ERROR_MSG));
		}
	}
	catch (CFileException e)
	{
#ifdef _DEBUG
		afxDump << _T("File could not be opened ")
			<< e.m_cause << _T("\n");
#endif
		TCHAR strError[256];
		memset(strError, 0, sizeof(strError));
		e.GetErrorMessage(strError, 256);
		theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, strError);
	}
	strInfo = _tl(LANGUAGE_RED) + strInfo +_T("\r\n")+ _T("(")+pstrErrorCode+_T(")");
	return strInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ͨ����д��������ת������������    

@param RW_TYPE rwType ��д������
@param bool isMain �Ƿ������ز�

@retval   PROG_TYPE ��������

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfo::PROG_TYPE CTVMRWHelper::ConvertRWTypeToProgType(RW_TYPE rwType, bool isMain)
{
	CVersionInfo::PROG_TYPE programType = CVersionInfo::UNKOWN;	
	switch(rwType)
	{
	case RW_TYPE::TVM_CARD_RW:
		programType =  isMain ? CVersionInfo::CARD_RW_MAIN : CVersionInfo::CARD_RW_APP;
		break;
//	case RW_TYPE::TVM_TOKEN_RW:
//		programType =  isMain ? CVersionInfo::TOKEN_RW_MAIN : CVersionInfo::TOKEN_RW_APP;
//		break;
	case RW_TYPE::TVM_RECHARGE_RW:
		programType = isMain ? CVersionInfo::CHARGE_RW_MAIN : CVersionInfo::CHARGE_RW_APP;
		break;
	default:
		programType =  CVersionInfo::UNKOWN;
	}

	return programType;
}



////////////////////////////////////////////////////////////////////////////
///**
//@brief      �滻(�ɿ�)��������
//
//@param      (i)RW_CMD_REPLACE_CARD replaceInput        �滻(�ɿ�)��������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::ReplaceOldCard(RW_CMD_REPLACE_CARD& replaceInput,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWReplaceOldCard pRwCmd(replaceInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �滻(�¿�)��������
//
//@param      (i)RW_CMD_DEFER replaceInput        ������������ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::ReplaceNewCard(RW_CMD_REPLACE_CARD&replaceInput,RW_BUSSINESS_RESPONSE&response)
//{
//	CRWReplaceOldCard pRwCmd(replaceInput,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���Ի�������Ի��͸��Ի����£�����������
//
//@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo	���Ի���Ϣ�ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response	ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::Personalize(RW_CMD_PERSONALIZE_INFO personalizeInfo,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWPersonalize pRwCmd(personalizeInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���Ի��������ɿ�����
//
//@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo	���Ի���Ϣ�ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response	ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::PersonalizeUpdateOldCard(RW_CMD_PERSONALIZE_INFO personalizeInfo,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWPersonalizeUpdateOldCard pRwCmd(personalizeInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���Ի�����д�¿�����
//
//@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo	���Ի���Ϣ�ṹ
//@param      (o)RW_BUSSINESS_RESPONSE& response	ҵ�񷵻�ͨ�ýṹ
//
//@retval     long   �������   
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//long CTVMRWHelper::PersonalizeUpdateNewCard(RW_CMD_PERSONALIZE_INFO personalizeInfo,RW_BUSSINESS_RESPONSE& response)
//{
//	CRWPersonalizeUpdateNewCard pRwCmd(personalizeInfo,m_RWID);
//	long errCode = pRwCmd.ExecuteCommand();
//	if (IsExceptionCode(errCode)) {
//		throw CRWHardwareException(m_RWType,errCode, _T(__FILE__), __LINE__);
//	}
//	pRwCmd.GetRWResponse(&response);
//
//	return errCode;
//}


DWORD WINAPI RechargeServerMoniterFunction(IN LPVOID vThreadParm){
	CTVMRWHelper* pThreadParm = reinterpret_cast<CTVMRWHelper*>(vThreadParm);

	if(NULL != pThreadParm){
		while(1){
			//// �ж��Ƿ��˳��߳�
			//if(pThreadParm->m_bExitThread){
			//	break;
			//}

			// ����ֵ�������Ƿ����
			theAPP_SESSION.SetRechargeServerEnabled(false);
			bool ret1,ret2;
			ret1 = ret2 = false;

			WSADATA wsa;
			SOCKET s;
			struct sockaddr_in server;
			USES_CONVERSION;
			char* pHost = NULL;
			int nPort = 0;
			int ret = 0;
			// ���YKT������
			pHost = T2A(theTVM_SETTING.GetRechargeServeYKT_IP());
			nPort = theTVM_SETTING.GetRechargeServeYKT_Port();    //��ǰ�˿�
			WSAStartup(MAKEWORD(2, 2), &wsa);    //ʹ��winsock����֮ǰ��������WSAStartup������װ�벢��ʼ����̬���ӿ�
			server.sin_family = AF_INET;    //ָ����ַ��ʽ����winsock��ֻ��ʹ��AF_INET
			server.sin_addr.s_addr = inet_addr(pHost); //ָ����ɨ���IP��ַ
			s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			server.sin_port = htons(nPort); //ָ����ɨ��IP��ַ�Ķ˿ں�
			ret = connect(s, (struct sockaddr *)&server, sizeof(server));
			if(ret==0){
				ret1 = true;
				closesocket(s);
			}
			// ���YPT������
			pHost = T2A(theTVM_SETTING.GetRechargeServeYPT_IP());
			nPort = theTVM_SETTING.GetRechargeServeYPT_Port();    //��ǰ�˿�
			server.sin_addr.s_addr = inet_addr(pHost); //ָ����ɨ���IP��ַ
			s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			server.sin_port = htons(nPort); //ָ����ɨ��IP��ַ�Ķ˿ں�
			ret = connect(s, (struct sockaddr *)&server, sizeof(server));
			if(ret==0){
				ret2 = true;
				closesocket(s);
			}

			if (ret1&&ret2){
				theAPP_SESSION.SetRechargeServerEnabled(true);
			}

			Sleep(10000);
		}// end of while
	}

	return RSP_CODE_OK;
}


// ��ֵ���������
long CTVMRWHelper::DoRechargeServerMoniter()
{
	// ������ֵ����������߳�
	DWORD dwIdleThreadID = 0;
	HANDLE hIdleThread = CreateThread(
		NULL,				// Pointer to thread security attributes
		0,					// Initial thread stack size, in bytes 
		RechargeServerMoniterFunction,	// Pointer to thread function
		this,				// The argument for the new thread
		0,					// Creation flags 
		&dwIdleThreadID			// Pointer to returned thread identifier
		);

	if(hIdleThread != NULL && hIdleThread != INVALID_HANDLE_VALUE){
		CloseHandle(hIdleThread);
		hIdleThread = NULL;
		return 1;
	}

	return 0;
}

