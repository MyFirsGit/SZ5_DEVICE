#include "stdafx.h"
#include "RWHelper.h"
#include "tpuerrorinfo.h"
#include "caccdevicecontrolparam.h"
#include "setting.h"

/**
@brief RW Helper
*/
RWID CRWHelper::s_RWID(INVALID_HANDLE_VALUE,FILTER_CARD_RW_LOG,BOM_CARD_RW);

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWHelper::CRWHelper(CService &service):CServiceHelper(service)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWHelper::~CRWHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���д��

@param       (i)bool reset      �Ƿ�reset   true:ִ�� RWReset, false:��ִ�� RWReset

@retval      ��

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWHelper::InitCheck(bool reset)
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
			theAPP_SESSION.SetSAMInfo(RW_ECT_ISAM, tpuInfo.bmacISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ECT_PSAM, tpuInfo.bmacPSAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_ISAM, tpuInfo.accISAMid);
			theAPP_SESSION.SetSAMInfo(RW_ACC_PSAM, tpuInfo.accPSAMid);
			theSOFT_VERSION.SetRWBootVer((char*)tpuInfo.softwareVersionMain); 
			theSOFT_VERSION.SetICRWVersion((char*)tpuInfo.softwareVersionApp); 
		
			DWORD mainVer;
			memcpy(&mainVer,tpuInfo.softwareVersionMain,4);
			
			DWORD appVer;
			memcpy(&appVer,tpuInfo.softwareVersionApp,4);
			
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_MAIN,mainVer);
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP,appVer);
		}
	}
	catch (CSysException& e) {
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_MAIN,0);
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP,0);
		throw e;
	}
	catch (...) {
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_MAIN,0);
		theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP,0);
		throw CInnerException(CRWException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CRWHelper::CheckDeviceExchange()
{
	try {
		// ��ȡTPU�豸ID
		RW_RSP_TPU_INFO tpuInfo;
		long errCode = GetTPUInfo(tpuInfo);
		if(errCode == RW_RSP_OK){
			// ��鲿��ID�Ƿ���
			CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
			theDeviceInfo.GetDeviceInfo(deviceInfo);
			if(memcmp(tpuInfo.DeviceCode,deviceInfo.RW_DEVICE_ID,sizeof(deviceInfo.RW_DEVICE_ID)) != 0){
				// �����豸������������
				theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendDeviceComponentExchange(BOM_DEVICE_OUT_TPU,tpuInfo.DeviceCode,deviceInfo.RW_DEVICE_ID);
				theDeviceInfo.SetRWId(tpuInfo.DeviceCode);
			}
		}
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CInnerException(CTWException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CRWHelper::InitData()
{
	RW_CMD_INIT initInput;
	// һֱѰ��
	initInput.ReadTimeout = 0;
	// �豸ID
	CString deviceId;
	deviceId.Format(_T("%.2d%.2d%.2x%.3d"),theMAINTENANCE_INFO.GetLineCode(),theMAINTENANCE_INFO.GetStationCode(),theMAINTENANCE_INFO.GetDeviecTypeID(),theMAINTENANCE_INFO.GetMachineCode());
	initInput.DeviceId = _ttoi(deviceId);
	//initInput.DeviceId[5] = 0;
	//initInput.DeviceId[4] = 0;
	//initInput.DeviceId[3] = theMAINTENANCE_INFO.GetLineCode();		// ��·���
	//initInput.DeviceId[2] = theMAINTENANCE_INFO.GetStationCode();	// ��վ���
	//initInput.DeviceId[1] = DEVICE_BOM;								// �豸���� BOM 20
	//initInput.DeviceId[0] = theMAINTENANCE_INFO.GetMachineCode();	//�豸���
	// �豸����
	initInput.DeviceType = theMAINTENANCE_INFO.GetCurrentACCDevice().deviceType;
	//  ��վ����
	initInput.StationId = ComMakeLong(HIBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),LOBYTE(theMAINTENANCE_INFO.GetTPUStationCode()),0,0);//dwLocationNum;
	// ��Ӫ��˾����뷽
	initInput.AgentCode = 2;	//��������2������Ӫ��˾	02
	// ��Ӫ����
	initInput.OperationDate = ComGetBusiDate();
	 // ��ǰ��������
	initInput.CurrentDate = ComGetCurDate();
	// �豸����ģʽ
	initInput.TestMode = theSETTING.GetDeviceTestMode();	//0x00����ʾ����ģʽ��0x01����ʾ����ģʽ
	// ��վ��Ӫģʽ �μ�ModeCode_t				
	initInput.OperationMode = theBOM_STATUS_MGR.GetRunMode();
	// ����Ա�ı��,�μ�OperateID_t
	initInput.OperaterID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);		
	// �������� 0x00�������ߣ�0x01��˫����������A����Ѱ����0x02��˫����������B����Ѱ��	
	initInput.AntennaConfig = 0x02;			
	
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
					iResult = InitRW(initInput,Status);
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
					throw CRWException(CRWException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__,_T("TPU����״ֵ̬�Ƿ�"));
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
int CRWHelper::GetAndSaveUDData()
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
int CRWHelper::SaveRWUDData(WORD txnLen,LPBYTE txnBuf)
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
void CRWHelper::SaveRWARData(WORD &ARDataLen,DAT::ARDATA &ARData,BYTE ticketType)
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
				theBomAR.AddBOMProcessStatByType(ticketType,arKey,arValue);
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
bool CRWHelper::RepareIni()
{
	try {
		// �رն�д��
		Close();
		// �����Լ�
		InitCheck();
	}
	catch (CSysException) {
		throw;
	}
	catch (...) {
		throw CRWException(CRWException::OTHER_ERR,_T(__FILE__),__LINE__);
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
bool CRWHelper::Repare()
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
		throw CRWException(CRWException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������

@param     (i)bool IsPaidArea  ��������־ true:������;false���Ǹ�����

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRWHelper::ReadCard(bool IsPaidArea)
{
	// ����ǰ��ֹ��д��
	Abort(true);

	// ����
	RW_READ_CARD_REQUEST readCard;
	readCard.IsCheck = true;			//�Ϸ��Լ���־	true:��飻false:�����
	readCard.IsPaidArea = IsPaidArea;	//��������־		true:������;false���Ǹ�����
	readCard.IsReadRecords = false;		//���׼�¼��ȡ��־	true:��ȡ;false:����ȡ
	UINT serviceId = m_service.GetServiceID();
	switch(serviceId){
		// ��Ʊ || Ԥ��Ʊ
		case ISSUE_SVC:
		case PREISSUE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ISSUE;				
			break;
		// ��Ʊ
		case ADJUST_SVC:
			readCard.FunctionCode = FUNCTION_CODE_ADJUST;	
			break;
		// ��ֵ
		case CHARGE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CHARGE;	
			break;
		// ���� || ��������
		case ANALYZE_SVC:
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		case OUTRW_TEST_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			readCard.IsReadRecords = true;
			break;
		// ����
		case REFRESH_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_REFRESH_AUTO;
			readCard.IsReadRecords = true;
			break;
		// ����
		case DEFER_SVC:
			readCard.FunctionCode = FUNCTION_CODE_DEFER;
			break;
		// ����
		case EXCHANGE_SVC:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_EXCHANGE_OLD;
			readCard.IsReadRecords = true;
			break;
		// ��ʱ�˿� | �˿����� | �˿��ѯ
		case REFUND_BASE_SVC:
		case REFUND_APPLY_SVC:
		case REFUND_QUERY_SVC:
			readCard.FunctionCode = FUNCTION_CODE_REFUND;
			break;
		// ����
		case COUNTERACT_SVC:
			readCard.FunctionCode = FUNCTION_CODE_CONTERACT;
			break;
		default:
			readCard.IsCheck = false;
			readCard.FunctionCode = FUNCTION_CODE_OTHER;
			break;
	}

	ReadCard(readCard);

	// �ȴ���������ִ��
	Sleep(100);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֹ��д������

@param      (i)bool checkStatus      true:�ȼ���д��״̬; false:������д��״̬

@retval     long   �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Abort(bool checkStatus)
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
long CRWHelper::StopReadCard()
{
	CRWStopReadCard pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
bool CRWHelper::SynchronismParam()
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

	// ACC��Ӫ����
	if(versionAcc.lBusinessParam != theACC_BUSINESS.GetCurMasterVer()){
		vecParamId.push_back(ACC_BUSINESS_ID);
	}
	// ACC�������������Ѳ���
	if(versionAcc.lServerFeeParam != theACC_SERVICEFEE.GetCurMasterVer()){
		vecParamId.push_back(ACC_SERVICEFEE_ID);
	}
	// ACC��ʱ�������ò���
	if(versionAcc.lOverTimeAdjuestParam != theACC_OVERTIMEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_OVERTIME_UPDATE_ID);
	}
	// ACC�豸���Ʋ���
	if(versionAcc.lDeviceControlParam != theACC_DEVICECONTROL.GetCurMasterVer()){
		vecParamId.push_back(ACC_DEVICE_ID);
	}
	// ACC��վλ�ò���
	if(versionAcc.lStationParam != theACC_LINE.GetCurMasterVer()){
		vecParamId.push_back(ACC_STATION_ID);
	}
	// ACC�Ʒ�վ�����
	if(versionAcc.lPayStationParam != theACC_TOLLSTATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_CHARGE_STATION_ID);
	}
	// ACCƱ�����Բ���
	if(versionAcc.lCardAttributeParam != theACC_TICKET.GetCurMasterVer()){
		vecParamId.push_back(ACC_CARDATTRIBUTE_ID);
	}
	// ACC����Ʊ�۱����
	if(versionAcc.lBasePriceParam != theACC_BASEPRICE.GetCurMasterVer()){
		vecParamId.push_back(ACC_BASE_PRICE_TABLE_ID);
	}
	// ACCƱ�ۼ�������
	if(versionAcc.lPrieLevelParam != theACC_PRICELEVEL.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_LEVEL_TABLE_ID);
	}
	// ACCƱ�۵������Ա����
	if(versionAcc.lPriceAdjuestParam != theACC_PRICEADJUEST.GetCurMasterVer()){
		vecParamId.push_back(ACC_PRICE_ADJUEST_TABLE_ID);
	}
	// ACCʱ�両�������
	if(versionAcc.lTimeFloatParam != theACC_TIMEFLOAT.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_FLOATING_TABLE_ID);
	}
	// ACCʱ�����ͱ����
	if(versionAcc.lTimeTypeParam != theACC_TIMETYPE.GetCurMasterVer()){
		vecParamId.push_back(ACC_TIME_TYPE_TABLE_ID);
	}
	// ACCʱ��������ϸ����
	if(versionAcc.lTimeDetailParam != theACC_TIMEDETAIL.GetCurMasterVer()){
		vecParamId.push_back(ACC_TTIME_TYPE_DETAIL_ID);
	}
	// ACC�ۿ��ʱ����
	if(versionAcc.lDiscountParam != theACC_DISCOUNT.GetCurMasterVer()){
		vecParamId.push_back(ACC_DISSCOUNT_TABLE_ID);
	}
	// ACCͣ�˳�վ��
	if(versionAcc.lNotServStationParam != theACC_NOTSERVSTATION.GetCurMasterVer()){
		vecParamId.push_back(ACC_NOT_SERVICE_STATION_ID);
	}
	// ACC���β���
	if(versionAcc.lSectionParam != theACC_SECTION.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_ID);
	}
	// ACC����������
	if(versionAcc.lWriteListParam != theACC_WRITELIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_WRITE_LIST_ID);
	}
	// ACC��ؿ�/���д�����ղ���
	if(versionAcc.lOtherCityCardParam != theACC_OTHERCITYCARD.GetCurMasterVer()){
		vecParamId.push_back(ACC_OTHER_CITY_CARD_ID);
	}
	// ACC��������������
	if(versionAcc.lSingleBlackListParam != theACC_SINGLEBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SINGLE_BLACK_LIST_ID);
	}
	// ACC���κ���������
	if(versionAcc.lSectionBlackListParam != theACC_SECTIONBLACKLIST.GetCurMasterVer()){
		vecParamId.push_back(ACC_SECTION_BLACK_LIST_ID);
	}
	// ACCƱ�������̲���
	if(versionAcc.lissueCompanyParam != theACC_ISSUECOMPANY.GetCurMasterVer()){
		vecParamId.push_back(ACC_ISSUE_COMPANY_ID);
	}
	 //AFCģʽ��������(��TPU������ʱע��)
	CCTLOGSoftVer::VERSION_INFO Version_Info;
	theSOFT_VERSION.GetSoftVerData(Version_Info);
	if(versionAfc.lModeHistoryParam != Version_Info.dwModeHistory){
		vecParamId.push_back(AFC_MODEHISTORY_ID);
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
bool CRWHelper::UpdateParamter(CParamHelper::VEC_PARAM_ID vecParamId)
{
	bool ret = true;
	CString paramPathName = _T("");
	CString sPrefix = PARAM_PREFIX;	
	CString ParamPath = _T("");

	// ��������ID
	vector<WORD>::iterator iteId = vecParamId.begin();
	while (iteId != vecParamId.end()) {
		// ȡ�ò����ļ�·��
		if(*iteId == AFC_MODEHISTORY_ID || *iteId == AFC_DEVICECOMMONPARAM_ID || *iteId == AFC_STAFFPWDPARAM_ID
			||*iteId == AFC_ACCESSLEVELPARAM_ID ||*iteId == AFC_BOMOPERATIONPARAM_ID ||*iteId == AFC_DEVICERUNTIMEPARAM_ID){
			ParamPath = PARAM_AFC_CURRENT_DIR;
		}
		else if(*iteId == ACC_BUSINESS_ID 
			|| *iteId == ACC_SERVICEFEE_ID 
			|| *iteId == ACC_OVERTIME_UPDATE_ID 
			|| *iteId == ACC_DEVICE_ID 
			|| *iteId == ACC_STATION_ID 
			|| *iteId == ACC_CHARGE_STATION_ID 
			|| *iteId == ACC_SECTION_ID 
			|| *iteId == ACC_CARDATTRIBUTE_ID 
			|| *iteId == ACC_BASE_PRICE_TABLE_ID 
			|| *iteId == ACC_PRICE_LEVEL_TABLE_ID 
			|| *iteId == ACC_PRICE_ADJUEST_TABLE_ID 
			|| *iteId == ACC_TIME_FLOATING_TABLE_ID 
			|| *iteId == ACC_TIME_TYPE_TABLE_ID 
			|| *iteId == ACC_TTIME_TYPE_DETAIL_ID 
			|| *iteId == ACC_DISSCOUNT_TABLE_ID 
			|| *iteId == ACC_NOT_SERVICE_STATION_ID 
			|| *iteId == ACC_WRITE_LIST_ID 
			|| *iteId == ACC_OTHER_CITY_CARD_ID 
			|| *iteId == ACC_SINGLE_BLACK_LIST_ID 
			|| *iteId == ACC_SECTION_BLACK_LIST_ID
			|| *iteId == ACC_ISSUE_COMPANY_ID){
			ParamPath = PARAM_ACC_CURRENT_DIR;
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CRWException(CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__,_T("��д�����صĲ���ID�쳣:" + sParaID));
		}
		paramPathName = thePARAM_HELPER.GetTheParamPathName(*iteId,ParamPath);
		if(thePARAM_HELPER.GetFileNameFromFullPath(paramPathName).GetLength()!=0){
			// ����bin�ļ�����Ϣ
			VARIABLE_DATA data;
			CXFile file(paramPathName,_T(""));
			file.ReadBinFileWithoutSUM(data);

			// ����RW�ĸ�������
			RW_PARAM_DOWNLOAD input;
			input.fileData = data.lpData;
			input.size = data.nLen;		
			input.paramId = *iteId;

			int iRet = DownloadParamter(input);
			if(iRet != RW_RSP_OK){
				ret = false;
				int iLastErrCode = 0;
				GetErrCode(iLastErrCode);	
			}
		}
		else{
			CString  sParaID = _T("");
			sParaID.Format(_T("%.4x"),*iteId);  
			throw CRWException(CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__,_T("��д��ͬ���ñ��ز���������:" + sParaID));
		}
		iteId ++;
		//Sleep(1000);
	}
	return ret;
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
long CRWHelper::Open()
{
	RW_CMD_OPEN cmd_open;
	cmd_open.isUsb = false;
	cmd_open.port = theBOM_INFO.GetRWCommPort();
	cmd_open.baud = theBOM_INFO.GetRWBaudRate();
	CRWOpen pRwCmd(cmd_open,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::Close()
{
	CRWClose pRwCmd(s_RWID);
	long errCode =  pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::Reset(RW_RSP_STATUS& status)
{
	CRWReset pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetStatus(RW_RSP_STATUS& status)
{
	CRWGetStatus pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&status);
	}

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
long CRWHelper::Abort(RW_RSP_STATUS& status)
{
	CRWAbort pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::Stop(RW_RSP_STATUS& status)
{
	CRWStop pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::SetTime(_DATE_TIME currentDateTime,_DATE_TIME& tpuDateTime)
{
	CRWSetTime pRwCmd(currentDateTime,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetTime(_DATE_TIME& tpuDateTime)
{
	CRWGetTime pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetMac(BYTE* hashData,BYTE* mac)
{
	WORD keyVersion = 1;
	CRWGetMac pRwCmd(keyVersion,hashData,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetErrCode(int &iErrorCode)
{
	CRWGetErrCode pRwCmd(iErrorCode,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetTPUInfo(RW_RSP_TPU_INFO& tpuInfo)
{
	CRWGetTpuInfo pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::RWFirmwareDownload(RW_CMD_FIRMWARE input)
{
	CRWFirmwareDownload pRwCmd(&input,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if(RW_RSP_OK != errCode) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::InitRW(RW_CMD_INIT initInfo,RW_RSP_STATUS& status)
{
	CRWInit pRwCmd(initInfo,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetTicketPrice(RW_CMD_TICKET_PRICE ticketInfo,RW_CMD_TICKET_PRICE_RES& res)
{
	CRWGetTicketPrice pRwCmd(ticketInfo,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	//if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&res);
	//}
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
long CRWHelper::GetParameterInfo(VERSION_ACC& accVersion,VERSION_ECT& ectVersion,VERSION_AFC& afcVersion)
{
	CRWGetParameterInfo pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
				paraVer=(DWORD)ComMakeLong(paramData.ParamData[i].ParaVer[3],paramData.ParamData[i].ParaVer[2],paramData.ParamData[i].ParaVer[1],paramData.ParamData[i].ParaVer[0]);
				switch(paraID){
					case ACC_BUSINESS_ID:// ACC��Ӫ����
						accVersion.lBusinessParam = paraVer;
						break;
					case ACC_SERVICEFEE_ID:// ACC�������������Ѳ���
						accVersion.lServerFeeParam = paraVer;
						break;
					case ACC_OVERTIME_UPDATE_ID:// ACC��ʱ�������ò���
						accVersion.lOverTimeAdjuestParam = paraVer;
						break;
					case ACC_DEVICE_ID:// ACC�豸���Ʋ���
						accVersion.lDeviceControlParam = paraVer;
						break;
					case ACC_STATION_ID:// ACC��վλ�ò���
						accVersion.lStationParam = paraVer;
						break;
					case ACC_CHARGE_STATION_ID:// ACC�Ʒ�վ�����
						accVersion.lPayStationParam = paraVer;
						break;
					case ACC_SECTION_ID:// ACC���β���
						accVersion.lSectionParam = paraVer;
						break;
					case ACC_CARDATTRIBUTE_ID:// ACCƱ�����Բ���
						accVersion.lCardAttributeParam = paraVer;
						break;
					case ACC_BASE_PRICE_TABLE_ID:// ACC����Ʊ�۱����
						accVersion.lBasePriceParam = paraVer;
						break;
					case ACC_PRICE_LEVEL_TABLE_ID:// ACCƱ�ۼ�������
						accVersion.lPrieLevelParam = paraVer;
						break;
					case ACC_PRICE_ADJUEST_TABLE_ID:// ACCƱ�۵������Ա����
						accVersion.lPriceAdjuestParam = paraVer;
						break;
					case ACC_TIME_FLOATING_TABLE_ID:// ACCʱ�両�������
						accVersion.lTimeFloatParam = paraVer;
						break;
					case ACC_TIME_TYPE_TABLE_ID:// ACCʱ�����ͱ����
						accVersion.lTimeTypeParam = paraVer;
						break;
					case ACC_TTIME_TYPE_DETAIL_ID:// ACCʱ��������ϸ����
						accVersion.lTimeDetailParam = paraVer;
						break;
					case ACC_DISSCOUNT_TABLE_ID:// ACC�ۿ��ʱ����
						accVersion.lDiscountParam = paraVer;
						break;
					case ACC_NOT_SERVICE_STATION_ID:// ACCͣ�˳�վ��
						accVersion.lNotServStationParam = paraVer;
						break;
					case ACC_WRITE_LIST_ID:// ACC����������
						accVersion.lWriteListParam = paraVer;
						break;
					case ACC_OTHER_CITY_CARD_ID:// ACC��ؿ�/���д�����ղ���
						accVersion.lOtherCityCardParam = paraVer;
						break;
					case ACC_SINGLE_BLACK_LIST_ID:// ACC��������������
						accVersion.lSingleBlackListParam = paraVer;
						break;
					case ACC_SECTION_BLACK_LIST_ID:// ACC���κ���������
						accVersion.lSectionBlackListParam = paraVer;
						break;
					case ACC_ISSUE_COMPANY_ID:// ACCƱ�������̲���
						accVersion.lissueCompanyParam = paraVer;
						break;
					case AFC_MODEHISTORY_ID:		//ģʽ��������
						afcVersion.lModeHistoryParam = paraVer;
						break;	
					default:
						CString  sParaID = _T("");
						sParaID.Format(_T("%.4x"),paraID);  
						//throw CRWException(errCode, _T(__FILE__), __LINE__,_T("�ⲿ��д�����صĲ���ID�쳣:" + sParaID));						
				}
			}
		}
	}
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
long CRWHelper::DownloadParamter(RW_PARAM_DOWNLOAD paramInfo)
{
	CRWParamFirmware pRwCmd(paramInfo,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::SetOperationMode(RW_CMD_SET_MODE cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetOperationMode pRwCmd(cmdSetMode,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::SetBusinessTime(RW_CMD_SET_TIME cmdSetMode,RW_RSP_STATUS& status)
{
	CRWSetBusinessTime pRwCmd(cmdSetMode,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
long CRWHelper::GetUDData(RW_BUSSINESS_RESPONSE& commonResponse)
{
	CRWGetUDData pRwCmd(s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
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
void CRWHelper::ReadCard(RW_READ_CARD_REQUEST cmdReadCard)
{
	CRWReadCard* pRwCmd = new CRWReadCard(cmdReadCard,s_RWID);
	pRwCmd->SetAutoDeleted(TRUE);
	pRwCmd->SetService(&m_service);
	pRwCmd->SetNeedCallbackService(true);
	pRwCmd->SetCommandID(RW_COMMAND_ID_READ_CARD);
	g_pRWControl->DoCommand(pRwCmd,SP_EXECMODE_QUEUE);
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
long CRWHelper::Issue(RW_ISSUE_REQUEST issueInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWIssue pRwCmd(issueInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ʊ����

@param      (i)RW_CMD_ADJUST adjustInput        ��Ʊ��������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Adjust(RW_CMD_ADJUST adjustInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWAdjust pRwCmd(adjustInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	
		pRwCmd.GetRWResponse(&response);
	
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵ����

@param      (i)RW_CMD_CHARGE chargeInput        ��ֵ��������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Charge(RW_CMD_CHARGE chargeInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWCharge pRwCmd(chargeInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿�����

@param      (i)RW_CMD_REFUND refundInput        �˿���������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Refund(RW_CMD_REFUND refundInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWRefund pRwCmd(refundInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)RW_CMD_BLOCK blockInput             ������������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Block(RW_CMD_BLOCK blockInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWBlock pRwCmd(blockInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)RW_CMD_BLOCK blockInput        ������������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Counteract(RW_CMD_COUNTERACT conteractInput, RW_BUSSINESS_RESPONSE& response)  
{
	CRWCounteract pRwCmd(conteractInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��������

@param      (i)RW_CMD_BLOCK blockInput        ������������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Refresh(RW_CMD_ACTIVE refreshInput, RW_BUSSINESS_RESPONSE& response)
{
	CRWRefresh pRwCmd(refreshInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��������

@param      (i)RW_CMD_DEFER deferInput        ������������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::Defer(RW_CMD_DEFER deferInput, RW_BUSSINESS_RESPONSE& response)
{
	CRWDefer pRwCmd(deferInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      һ��ͨ��ֵ��ʱ����

@param      (i) 


@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::ChargeOverTimeProcess(RW_CMD_CHARGEOVERTIME chargeOverTime, RW_BUSSINESS_RESPONSE& response )
{
	CRWChargeOverTime pRwCmd(chargeOverTime,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);

	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      һ��ͨ�ۿ����촦��

@param      (i) 


@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::IssueInitCheckProcess(RW_CMD_SETCHECK chargeInput,RW_CMD_RESP_SETCHECK &response)
{
	CRWSetCheck pRwCmd(chargeInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	if (RW_RSP_OK == errCode) {
		pRwCmd.GetRWResponse(&response);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �������ֿ�����Ϣ�޸�����

@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo   �����������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response					ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::UpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo,RW_BUSSINESS_RESPONSE &response)
{
	CRWUpdateSignCardInfo pRwCmd(updateSignCardInfo,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж϶�д���Ƿ�����

@param      none

@retval     bool true:�쳣 false:���� 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CRWHelper::IsRWHasException()
{
	return theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�Ϊ��Ҫ���쳣��ID

@param      (i)long errCode      ��������ṹ��

@retval     bool \n  true:�쳣�Ĵ������; false:�����쳣��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CRWHelper::IsExceptionCode(long errCode)
{
	bool ret = false;

	switch(errCode) {
	case RW_RSP_STATUSERROR:       // TPU״̬�Ƿ�
	case RW_RSP_COMERR:            // ͨѶ�ӿ������
	case RW_RSP_SAMERR:
		//case RW_RSP_PARAMERR:	   // ���ò��������
	case RW_RSP_UDERR:		       // UD���������
	case RW_RSP_HARDWAREERR:	   // Ӳ�������
	case RW_RSP_UNKNOWERR:		   // δ�������
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
int CRWHelper::UpdateProgram(bool isMain)
{
	int iRet = 0;
	try
	{
		CString fileName = isMain?MID_STR_BOM_RW_PROG_MAIN:MID_STR_BOM_RW_PROG_APP;
		vector<CString> updateFiles;

		CString sAppPath;
		GetAppPath(sAppPath);
		CString updateFileFolder = sAppPath + BOM_IC_UPDATE_DIR;

		SearchFile(updateFileFolder + fileName + _T("*.*"),updateFiles);
		if(updateFiles.size()<=0)
		{
			return -2;
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
				return -3;
			}

			// ����bin�ļ�����Ϣ
			VARIABLE_DATA data;
			CXFile file(updateFile,_T(""));
			file.ReadBinFileWithoutSUM(data);

			DeleteFile(updateFile);

			// ����RW�ĸ�������
			RW_CMD_FIRMWARE input;
			input.fileData = data.lpData;
			input.size = data.nLen;
			input.isMain = isMain;

			theAPP_SESSION.SetHasProgUpdate(true);
			theSETTING.SetRWUpdateFlag(1,true);
			if (RW_RSP_OK == RWFirmwareDownload(input)) {
				theSETTING.SetRWUpdateFlag(0,true);
				//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,progType,upgradeVersion);//���±��ذ汾
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
CString CRWHelper::GetTpuErrorInfo(int iError)
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
CString CRWHelper::gGetErrorInfofromFile(LPCTSTR pstrFile, DWORD dwErrorCode)
{
	
	CString pstrErrorCode(_T(""));
	pstrErrorCode.Format(_T("0x%08X"), dwErrorCode);

	if (pstrErrorCode == "" || pstrFile == NULL)
		return _T("");


	CString strInfo(_T(""));
	try
	{
		CString strFlag1(_T("/*")), strFlag2(_T("*/"));
		CStdioFile file(pstrFile, CFile::typeText | CFile::modeRead);
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
			strInfo = "δ֪����";
			theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, _T("û���ҵ���Ӧ�Ĵ�����Ϣ��"));
		}
	}
	catch (CFileException e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened "
			<< e.m_cause << "\n";
#endif
		TCHAR strError[256];
		memset(strError, 0, sizeof(strError));
		e.GetErrorMessage(strError, 256);
		theDEBUG_LOG->WriteData(_T(""), _T("gGetErrorInfromFile"), __LINE__, strError);
	}
	strInfo = _T("���죩") + strInfo +_T("\r\n")+ _T("(")+pstrErrorCode+_T(")");
	return strInfo;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      �滻(�ɿ�)��������

@param      (i)RW_CMD_REPLACE_CARD replaceInput        �滻(�ɿ�)��������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::ReplaceOldCard(RW_CMD_REPLACE_CARD& replaceInput,RW_BUSSINESS_RESPONSE& response)
{
	CRWReplaceOldCard pRwCmd(replaceInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �滻(�¿�)��������

@param      (i)RW_CMD_DEFER replaceInput        ������������ṹ
@param      (o)RW_BUSSINESS_RESPONSE& response     ҵ�񷵻�ͨ�ýṹ

@retval     long   �������   

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CRWHelper::ReplaceNewCard(RW_CMD_REPLACE_CARD&replaceInput,RW_BUSSINESS_RESPONSE&response)
{
	CRWReplaceOldCard pRwCmd(replaceInput,s_RWID);
	long errCode = pRwCmd.ExecuteCommand();
	if (IsExceptionCode(errCode)) {
		throw CRWHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	pRwCmd.GetRWResponse(&response);

	return errCode;
}