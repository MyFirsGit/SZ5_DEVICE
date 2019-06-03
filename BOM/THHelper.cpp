#pragma once
#include "StdAfx.h"
#include "THHelper.h"
//#include "THException.h"
#include "TicketHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

//@brief TH Helper

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHHelper::CTHHelper(CService& service):CServiceHelper(service)
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
CTHHelper::~CTHHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TH������������

@param      ��

@retval     ��

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::Connect()
{
	try {
		// δ����
		if(!theAPP_SESSION.IsTHConnected()) {
			CommOpen();
			// ����������
			theAPP_SESSION.SetIsTHConnected(true);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...){
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TH�Ͽ�����

@param      ��

@retval     ��

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::Disconnect()
{
	try {
		// ������
		if(theAPP_SESSION.IsTHConnected()) {
			// �ܵ�ͨ�Ų��Ͽ�
			// 			if(theTCM_INFO.GetTHCommuMethod() != 2){
			CommClose();
			// ����δ����
			theAPP_SESSION.SetIsTHConnected(false);
			// 			}
		}		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ���Ʊ��

@param       �� 

@retval      CTCMException

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::Initialize()
{

	try {
		TH_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
		UINT uiRetractNum = 0;

		// �򿪴������ӳ�Ʊ��
		Connect();

		// ��ȡ״̬
		GetDevStatus(&th_rsp_get_dev_status);
		// Ӳ������
		if(TIM_FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
			// ��λ����
			Reset(CMD_INIT_FAREBOX, &uiRetractNum);
		}

		UINT ulReturn = 0;
		Init(CMD_INIT_FAREBOX,&ulReturn);

		// ����ģ�鹤��ģʽ
		SetWorkMode(TH_CMD_SET_WORK_MODE::CMD_WORK_MODE_NORMAL);

		// ���ð汾
		if(theSETTING.GetTHMode() == TH_MODE_NORMAL && !theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))
		{
			// ��¼THӲ���汾
			TH_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);

			DWORD dwVersion = ((th_rsp_get_version.cFirmWareVersion[8] - 48) & 0x0F)*1000 + 
				((th_rsp_get_version.cFirmWareVersion[10] - 48) & 0x0F) *100 +
				((th_rsp_get_version.cFirmWareVersion[12] - 48) & 0x0F) *10 +
				((th_rsp_get_version.cFirmWareVersion[13] - 48) & 0x0F);
			// TODO �������ݸ�ʽת��
			theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP,dwVersion);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �޸���Ʊ��

@param       �� 

@retval      bool   true:�ɹ�; false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTHHelper::Repare()
{
	bool result = false;
	TH_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
	UINT uiRetractNum = 0;
	try {

		// ֹͣ����
		//SetWorkMode(TH_CMD_SET_WORK_MODE::CMD_WORK_MODE_MAINTENANCE);

		// �Ͽ���Ʊ��
		Disconnect();

		// ���ӳ�Ʊ��
		Connect();

		GetDevStatus(&th_rsp_get_dev_status);
		// Ӳ������
		if(TIM_FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
			// ��λ����
			Reset(CMD_INIT_FAREBOX, &uiRetractNum);
		}

		UINT ulReturn = 0;
		Init(CMD_INIT_FAREBOX,&ulReturn);

		// ��ʼ����
		SetWorkMode(CMD_WORK_MODE_NORMAL);

		// ��ȡ״̬
		GetDevStatus(&th_rsp_get_dev_status);
		if(TIM_FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
			// �׳�Ӳ���쳣
			throw CTHHardwareException(CTHHardwareException::TH_HARDWERE_ERROR, _T(__FILE__), __LINE__);
		}

		// ���ð汾
		if(theSETTING.GetTHMode() == TH_MODE_NORMAL && !theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTHException::MODULE_ID))
		{
			// ��¼THӲ���汾
			TH_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);
			//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP,thInfo.THVersion);
		}

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	return result;
}

// ���Գ�Ʊ�� 
void CTHHelper::Test()
{
	bool result = false;
	UINT uiTransportNum = 0;
	UINT uiRetractNum = 0;
	try {

		// ��ʼ����
		SetWorkMode(CMD_WORK_MODE_NORMAL);
		// ��ȡƱ��
		CardOut(CMD_CARD_OUT_BOX_A, &uiTransportNum);
		// �ͳ�Ʊ��
		SendCard(&uiTransportNum);
		// ��ȡƱ��
		CardOut(CMD_CARD_OUT_BOX_A, &uiTransportNum);
		// ����Ʊ��
		RetractCard(&uiRetractNum);

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTHException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	//return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���Ʊ�䵽λ״̬

@param       (o)TH_RSP_GET_DEV_STATUS& response    ��ȡ�豸״̬�����ṹ
@param       (o)bool& isMagazineAExist             Ʊ��A��λ״̬
@param       (o)bool& isMagazineBExist             Ʊ��B��λ״̬
@param       (o)bool& isMagazineCExist             Ʊ��C��λ״̬

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CTHHelper::CheckTicketBoxExist(TH_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist)
{
	long lErrCode = TIM_FAULT_CMD;
	do {
		// ��ȡ�豸״̬
		lErrCode = GetDevStatus(response);
		if (TIM_FAULT_NORMAL != lErrCode) break;
		isMagazineAExist = false;
		if (response->bBoxAArrive) {
			isMagazineAExist = true;
		}
		isMagazineBExist = false;
		if (response->bBoxBArrive) {
			isMagazineBExist = true;
		}
		isMagazineCExist = false;
		if (response->bBoxCArrive) {
			isMagazineCExist = true;
		}
	} while (0);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�Ϊ��Ҫ���쳣��ID

@param      (i)long lErrCode      ���ϴ���

@retval     bool    true:�쳣�Ĵ������; false:�����쳣��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTHHelper::IsExceptionCode(long lErrCode)
{
	bool bRet = false;

	switch(lErrCode) {
	case TIM_FAULT_CMD					 :
		//FAULT_CMD
	case TIM_ERR_CODE_INVALID_CMD        : // invalid command
	case TIM_ERR_CODE_INVALID_PARAM          : // parameter error
	case TIM_ERR_CODE_DEVICE_BUSY    : // the active upload data timeout
	case TIM_ERR_CODE_RECIEVE_TIMEOUT    : // recieve timeout
	case TIM_ERR_CODE_COMCONFIGURE       : // com open error
	case TIM_ERR_CODE_RECIEVE_ERR            : // recieve error                                                         
		bRet = true;
		break;
	default:
		break;
	}
	return bRet;
}

// �׸���API��װ
//////////////////////////////////////////////////////////////////////////
/**
@brief      ͨѶ����

@param      ��

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::CommOpen()
{
	TH_CMD_COMM_OPEN cmd_comm_open;
	cmd_comm_open.uiCommPort = theBOM_INFO.GetTHCommPort();
	cmd_comm_open.uiBaudRate = theBOM_INFO.GetTHBaudRate();
	CTHCommOpen cTHCommOpen(cmd_comm_open);
	long lErrCode = cTHCommOpen.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ر�ͨѶ

@param      ��

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::CommClose()
{
	CTHCommClose cTHCommClose;
	long lErrCode = cTHCommClose.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ģ�鹤��ģʽ

@param      (i)TH_CMD_SET_WORK_MODE command      �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::SetWorkMode(TH_CMD_SET_WORK_MODE command)
{
	CTHSetWorkMode cTHSetWorkMode(command);
	long lErrCode = cTHSetWorkMode.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��

@param      (i)TH_CMD_CARD_OUT command      �������
@param      (o)TH_RSP_CARD_OUT& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::CardOut(TH_CMD_CARD_OUT command, UINT* uiTransportNum)
{
	CTHCardOut cTHCardOut(command);
	long lErrCode = cTHCardOut.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHCardOut.GetTHResponse(uiTransportNum);
	}
	//// �������
	//if(command == CMD_CARD_OUT_BOX_A)
	//	theDeviceRegAR.AddThScratchWheelACount();
	//else
	//	theDeviceRegAR.AddThScratchWheelBCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ͳ�Ʊ��

@param      (o)TH_RSP_SEND_CARD& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::SendCard(UINT* uiTransportNum)
{
	CTHSendCard cTHSendCard;
	long lErrCode = cTHSendCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHSendCard.GetTHResponse(uiTransportNum);
	}
	// �������
	//theDeviceRegAR.AddThSendOutMotorCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��

@param      (o)TH_RSP_RETRACT_CARD& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::RetractCard(UINT* uiRetractNum)
{
	CTHRetractCard cTHRetractCard;
	long lErrCode = cTHRetractCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHRetractCard.GetTHResponse(uiRetractNum);
	}
	// �������
	//theDeviceRegAR.AddThLinkElecFlipCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�豸״̬

@param      (o)TH_RSP_GET_DEV_STATUS& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::GetDevStatus(TH_RSP_GET_DEV_STATUS* response)
{
	CTHGetDevStatus cTHGetDevStatus;
	long lErrCode = cTHGetDevStatus.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHGetDevStatus.GetTHResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ģ�鸴λ

@param      (i)TH_CMD_INIT_RESET command      �������
@param      (o)TH_RSP_RESET& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::Reset(TH_CMD_INIT_RESET command, UINT* uiRetractNum)
{
	CTHReset cTHReset(command);
	long lErrCode = cTHReset.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHReset.GetTHResponse(uiRetractNum);
	}
	//theDeviceRegAR.AddThResetCount();
	// �ȴ���λӲ������30����ɣ������޷�ͨ��
	Sleep(33000);
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��������Ϣ

@param      (i)TH_CMD_SET_COUNT_INFO inBoxID      �������
@param      (i)TH_RSP_SET_COUNT_INFO& inCardNum   �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::SetTicketBoxCountInfo(TH_BOX_TYPE inBoxID, tTIMCardNum* inCardNum)
{
	CTHSetTicketBoxCountInfo cTHSetTicketBoxCountInfo(inBoxID,inCardNum);
	long lErrCode = cTHSetTicketBoxCountInfo.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	tTIMDevReturn DevReturn;
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHSetTicketBoxCountInfo.GetTHResponse(&DevReturn);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��������Ϣ

@param      (i)TH_CMD_GET_COUNT_INFO command      �������
@param      (o)TH_RSP_GET_COUNT_INFO& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::GetTicketBoxCountInfo(TH_BOX_TYPE command, tTIMCardNum* response)
{
	CTHGetTicketBoxCountInfo cTHGetTicketBoxCountInfo(command);
	long lErrCode = cTHGetTicketBoxCountInfo.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHGetTicketBoxCountInfo.GetTHResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ģ���ʼ��

@param      (i)TH_CMD_INIT command      �������
@param      (o)TH_RSP_INIT& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::Init(TH_CMD_INIT_RESET command, UINT* uiRetractNum)
{
	CTHInit cTHInit(command);
	long lErrCode = cTHInit.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHInit.GetTHResponse(uiRetractNum);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�豸�����汾��Ϣ

@param      (o)TH_RSP_GET_VERSION& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::GetVersion(TH_RSP_GET_VERSION* response)
{
	CTHGetVersion cTHGetVersion;
	long lErrCode = cTHGetVersion.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTHGetVersion.GetTHResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ģ�����

@param      (i)TH_CMD_TEST_SETTING command      �������
@param      (o)TH_RSP_TEST_SETTING& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTHHelper::TestSetting(TH_CMD_TEST_SETTING command)
{
	CTHTestSetting CTHTestSetting(command);
	long lErrCode = CTHTestSetting.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTHHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡRFID

@param      IN TH_CMD_SET_COUNT_INFO boxType(��������)
			OUT tTIMRFIDInfo* pTimRfidInfo(���RFID��Ϣ)

@retval     bool true�ɹ� falseʧ��;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CTHHelper::ReadTicketBoxRFID(IN TH_BOX_TYPE boxType,OUT tTIMRFIDInfo* pTimRfidInfo){
//	//CTHReadRFIDInfo thReadRfid(boxType);
//	//long lRet = thReadRfid.ExecuteCommand();
//
//	//if(IsExceptionCode(lRet)){
//	//	throw CTHHardwareException(lRet,_T(__FILE__),__LINE__);
//	//}
//	//
//	//// ȷ�����������ȷ
//	//if(lRet != 0){
//	//	return false;
//	//}
//
//	//thReadRfid.GetTHResponse(pTimRfidInfo);
//
//	// lichao: ��ʱ�޸�RFID
//	if (boxType==TH_BOX_A)
//	{
//		memcpy(pTimRfidInfo->ucBoxID, "1111", 4);
//	}
//	else if(boxType==TH_BOX_B){
//		memcpy(pTimRfidInfo->ucBoxID, "2222", 4);
//	}
//	
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дƱ��RFID

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CTHHelper::WriteTicketBoxRFID(IN TH_BOX_TYPE boxType,OUT tTIMRFIDInfo* pTimRfidInfo)
//{
//	CTHWriteRFIDInfo writeRfid(boxType,pTimRfidInfo);
//	long lRet = writeRfid.ExecuteCommand();
//	if(IsExceptionCode(lRet)){
//		throw CTHHardwareException(lRet,_T(__FILE__),__LINE__);
//	}	
//	if(lRet != 0){
//		return false;
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      IN SC_REPLACE_BOX_CMD_INFO& rpsBoxInfo

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTHHelper::ReplaceTicketBox(TH_REPLACE_BOX_CMD_INFO boxAction){
	//CTHReplaceTicketBox replaceBox(rpsBoxInfo.boxId,rpsBoxInfo.actionType);

	// lichao
	//long lRet = replaceBox.ExecuteCommand();
	//if(IsExceptionCode(lRet)){
	//	throw CTHHardwareException(lRet,_T(__FILE__),__LINE__);
	//}

	//// Ӳ������(ж��)
	//if(rpsBoxInfo.actionType == TH_UNINSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TH_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_DOWN);
	//		}
	//		else if(rpsBoxInfo.boxId == TH_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_DOWN);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

	//// ��װ
	//if(rpsBoxInfo.actionType == TH_INSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TH_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_UP);
	//		}
	//		else if(rpsBoxInfo.boxId == TH_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_UP);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

	//// �������
	//if(rpsBoxInfo.boxId == TH_BOX_A)
	//	theDeviceRegAR.AddThBoxAUpDownCount();
	//else
	//	theDeviceRegAR.AddThBoxBUpDownCount();

	///*if(lRet != 0){
	//return false;
	//}*/

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ģ�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
 int CTHHelper::UpdateProgram(){
	int iRet = 0;
	try
	{
		//CString fileName = (CString)STR_TVM_TH_APP;
		//vector<CString> updateFiles;

		//CString sAppPath;
		//GetAppPath(sAppPath);
		//CString updateFileFolder = sAppPath + TVM_TH_UPDATE_DIR;//BOM_TC_UPDATE_DIR;


		//SearchFile(updateFileFolder + fileName + _T("*.*"),updateFiles);
		//if(updateFiles.size()<=0)
		//{
		//	return -2;
		//}

		//for(int i=0;i<updateFiles.size();i++)
		//{
		//	CString updateFile = updateFiles[i];
		//	string strFilePath = (LPCSTR)CStringA(updateFile);
		//	SetFileAttributes(updateFile,FILE_ATTRIBUTE_NORMAL);
		//	CString upgradeVersionStr = thePARAM_HELPER.GetProgVerFromProgName(updateFile);	// ��ȡ������TH�汾
		//	DWORD upgradeVersion = _ttoi(upgradeVersionStr);

		//	// �汾��Ч
		//	if(UNAVALIBLE_VERSION == upgradeVersion){
		//		DeleteFile(updateFile);
		//		return -3;
		//	}

		//	// ����bin�ļ�����Ϣ
		//	/*VARIABLE_DATA data;
		//	{
		//	CXFile file(updateFile);
		//	file.ReadBinFile(data);
		//	}
		//	DeleteFile(updateFile);*/

		//	theAPP_SESSION.SetHasProgUpdate(true);
		//	THUpdateCommand cmdUpdate(theSETTING.GetTHComPort(),theSETTING.GetTHBaudRate(),theSETTING.GetTHUpadateBaudRate(),(char*)strFilePath.c_str());
		//	if (RW_RSP_OK == cmdUpdate.Execute()) {
		//		DeleteFile(updateFile);
		//		//theSETTING.SetRWUpdateFlag(0,true);
		//		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP,upgradeVersion);//���±��ذ汾
		//	}
		//	else{
		//		//CVersionInfo::PROG_TYPE progType;
		//		//if(isMain){
		//		//	progType = CVersionInfo::RW_MAIN;
		//		//}
		//		//else{
		//		//	progType = CVersionInfo::RW_APP;
		//		//}
		//		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,progType,currentVersion);//����ʧ�ܣ�����TPU��ǰʵ�ʰ汾��
		//		iRet = -1;
		//	}
		//	// �������غ�ȵ�120�뷽�ɽ���Ӧ�ò��������
		//	/*if(isMain){
		//		Sleep(120*1000);
		//	}*/
		//}
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
 /*
 @brief      �����޸���Ʊ��

 @param      ��

 @retval     bool true�ɹ� falseʧ��

 @exception  ��
 */
 //////////////////////////////////////////////////////////////////////////
 bool CTHHelper::TryRepareTH(){

	 try{
		 Repare();
	 }
	 catch(CSysException& e){
		 theEXCEPTION_MGR.ProcessException(e);
		 return false;
	 }
	 catch(...){
		 return false;
	 }
	 return true;
 }


 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      ȡ��·����Ψһ���

 @param      ��

 @retval     ��

 @exception  ��
 */
 //////////////////////////////////////////////////////////////////////////
 bool CTHHelper::GetStaticBoxId(IN TH_BOX_TYPE boxId,OUT tTIMRFIDInfo& rfidInfo){

	 //rfidInfo.ucBoxID[0] = theMAINTENANCE_INFO.GetDeviceType();
	 rfidInfo.ucBoxID[1] = 0x01;
	 rfidInfo.ucBoxID[2] = Dec2Hex(theMAINTENANCE_INFO.GetStationCode());
	 BYTE bNum = theMAINTENANCE_INFO.GetMachineCode();
	 CString strNum(_T(""));

	 switch(boxId){
	 case TH_BOX_A:
		 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
		 rfidInfo.ucBoxID[3] = 2 * bNum - 2/*_tcstoul(strNum,NULL,16)*/;
		 break;
	 case TH_BOX_B:
		 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-1);
		 rfidInfo.ucBoxID[3] = 2 * bNum - 1/*_tcstoul(strNum,NULL,16)*/;
		 break;
	 case TH_BOX_C:
		 rfidInfo.ucBoxID[1] = 0x02;
		 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
		 rfidInfo.ucBoxID[3] = 2 * bNum/*_tcstoul(strNum,NULL,16)*/;
		 break;
	 default:
		 return false;
	 }

	 return true;
 }