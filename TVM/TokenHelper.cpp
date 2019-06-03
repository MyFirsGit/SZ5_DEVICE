#pragma once
#include "StdAfx.h"
#include "TOKENHelper.h"
#include "TOKENException.h"
#include "TOKENHardwareException.h"
#include "TicketHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef TOKENIS_FILE
static TCHAR TOKENIS_FILE[] = _T(__FILE__);
#endif

//@brief TOKEN Helper

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENHelper::CTOKENHelper(CService& service):CServiceHelper(service)
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
CTOKENHelper::~CTOKENHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TOKEN������������

@param      ��

@retval     ��

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::Connect()
{
	try {
		// δ����
		if(!theAPP_SESSION.IsTOKENModuleConnected()) {
			CommOpen();
			// ����������
			theAPP_SESSION.SetIsTOKENModuleConnected(true);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...){
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TOKEN�Ͽ�����

@param      ��

@retval     ��

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::Disconnect()
{
	try {
		// ������
		if(theAPP_SESSION.IsTOKENModuleConnected()) {
			CommClose();
			// ����δ����
			theAPP_SESSION.SetIsTOKENModuleConnected(false);
		}		
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
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
void CTOKENHelper::Initialize()
{

	try {
		TOKEN_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
		UINT uiRetractNum = 0;

		// �򿪴������ӳ�Ʊ��
		Connect();

		//// Ӳ������
		//if(FAULT_HARDWARE == th_rsp_get_dev_status.uiErrorType){
		//	// ��λ����
		//	Reset(TOKEN_CMD_RESET::CMD_RESET_FAREBOX, &uiRetractNum);
		//}

		UINT ulReturn = 0;
		Init(&ulReturn);

		// ��ȡ״̬
		GetDevStatus(&th_rsp_get_dev_status);

		//// ����ģ�鹤��ģʽ
		//SetWorkMode(TOKEN_CMD_SET_WORK_MODE::CMD_WORK_MODE_NORMAL);

		// ���ð汾
		if(!theEXCEPTION_MGR.HasException(CTOKENHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTOKENException::MODULE_ID))
		{
			// ��¼TOKENӲ���汾
			TOKEN_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);

			//DWORD dwVersion = th_rsp_get_version.ucFwVer
			//	
			//	((th_rsp_get_version.cFirmWareVersion[8] - 48) & 0x0F)*1000 + 
			//	((th_rsp_get_version.cFirmWareVersion[10] - 48) & 0x0F) *100 +
			//	((th_rsp_get_version.cFirmWareVersion[12] - 48) & 0x0F) *10 +
			//	((th_rsp_get_version.cFirmWareVersion[13] - 48) & 0x0F);
			//// TODO �������ݸ�ʽת��
			//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_APP,dwVersion);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
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
bool CTOKENHelper::Repare()
{
	bool result = false;
	TOKEN_RSP_GET_DEV_STATUS th_rsp_get_dev_status;
	UINT uiRetractNum = 0;
	try {

		// ֹͣ����
		//SetWorkMode(TOKEN_CMD_SET_WORK_MODE::CMD_WORK_MODE_MAINTENANCE);

		// �Ͽ���Ʊ��
		Disconnect();

		// ���ӳ�Ʊ��
		Connect();

		UINT ulReturn = 0;
		Init(&ulReturn);

		GetDevStatus(&th_rsp_get_dev_status);

		// ���ð汾
		if(!theEXCEPTION_MGR.HasException(CTOKENHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CTOKENException::MODULE_ID))
		{
			// ��¼TOKENӲ���汾
			TOKEN_RSP_GET_VERSION th_rsp_get_version;
			GetVersion(&th_rsp_get_version);
			//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_APP,thInfo.TOKENVersion);
		}

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	return result;
}

// ���Գ�Ʊ�� 
void CTOKENHelper::Test()
{
	bool result = false;
	UINT uiTransportNum = 0;
	UINT uiRetractNum = 0;
	try {
		// ��ȡƱ��
		CardOut(TOKEN_BOX_A, &uiTransportNum);
		// �ͳ�Ʊ��
		SendCard(&uiTransportNum);
		// ��ȡƱ��
		CardOut(TOKEN_BOX_A, &uiTransportNum);
		// ����Ʊ��
		RetractCard(&uiRetractNum);

		result = true;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CTOKENException(TIM_FAULT_CMD, _T(__FILE__), __LINE__);
	}
	//return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���Ʊ�䵽λ״̬

@param       (o)TOKEN_RSP_GET_DEV_STATUS& response    ��ȡ�豸״̬�����ṹ
@param       (o)bool& isMagazineAExist             Ʊ��A��λ״̬
@param       (o)bool& isMagazineBExist             Ʊ��B��λ״̬
@param       (o)bool& isMagazineCExist             Ʊ��C��λ״̬

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENHelper::CheckTicketBoxExist(TOKEN_RSP_GET_DEV_STATUS* response, bool& isMagazineAExist,bool& isMagazineBExist,bool& isMagazineCExist,bool& isMagazineDExist)
{
	long lErrCode = TIM_FAULT_CMD;
	do {
		// ��ȡ�豸״̬
		lErrCode = GetDevStatus(response);
		if (TIM_FAULT_NORMAL != lErrCode){
			throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
		}

		isMagazineAExist = false;
		isMagazineBExist = false;
		isMagazineCExist = false;
		isMagazineDExist = false;
		IO_RSP_STATUS_INFO ioStatus;
		if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			try{
				IO_HELPER->GetIOStatus(ioStatus);
				isMagazineCExist = ioStatus.isTokenInvBoxReady;
				isMagazineDExist = ioStatus.isTokenColBoxReady;
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
		isMagazineAExist = response->bBoxAExist;
		isMagazineBExist = response->bBoxBExist;

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
bool CTOKENHelper::IsExceptionCode(long lErrCode)
{
	if (lErrCode!=TIM_ERR_CODE_NONE){
		return true;
	}
	return false;


	//bool bRet = false;

	//switch(lErrCode) {
	//case FAULT_CMD					 :
	//	//FAULT_CMD
	//case ERR_CODE_INVALID_CMD        : // invalid command
	//case ERR_CODE_PARAM_ERR          : // parameter error
	//case ERR_CODE_UPLOAD_DATA_ERR    : // the active upload data timeout
	//case ERR_CODE_RECIEVE_TIMEOUT    : // recieve timeout
	//case ERR_CODE_COMCONFIGURE       : // com open error
	//case ERR_CODE_TO_NORMALMD        : // maintenance mode recieve normal mode command
	//case ERR_CODE_TO_MAINTMD         : // normal mode recieve maintenance mode command
	//case ERR_CODE_RECIEVE            : // recieve error
	//	//FAULT_HARDWARE
	//case ERR_CODE_EXIT_SEN           : // exit detect sensor error
	//case ERR_CODE_CSC_SEN            : // csc area sensor error
	//case ERR_CODE_A_ENTRY_SEN        : // A entrance Channel sensor status
	//case ERR_CODE_B_ENTRY_SEN        : // B entrance Channel sensor status
	//case ERR_CODE_SENSOR5            : // sensor5 error
	//case ERR_CODE_CHG_DIRE_SEN       : // change direction sensor status
	//case ERR_CODE_BOXB_TICKET_DE_SEN : // case box B ticket to anti detect sensor
	//case ERR_CODE_A_AVL_SEN          : // case box A available detection sensor
	//case ERR_CODE_A_COVER_SEN        : // case box A cover status sensor
	//case ERR_CODE_A_AE_SEN           : // case box A about empty detection sensor
	//case ERR_CODE_A_EM_SEN           : // case box A empty detection sensor
	//case ERR_CODE_A_MOTOR_UP_SEN     : // case box A motor up sensor status
	//case ERR_CODE_A_MOTOR_DW_SEN     : // case box A motor down sensor status
	//case ERR_CODE_B_AVL_SEN          : // case box B available detection sensor
	//case ERR_CODE_B_COVER_SEN        : // case box B cover sensor
	//case ERR_CODE_B_AE_SEN           : // case box B about empty detection sensor
	//case ERR_CODE_B_EM_SEN           : // case box B empty detection sensor
	//case ERR_CODE_B_MOTOR_UP_SEN     : // case box B motor up sensor status
	//case ERR_CODE_B_MOTOR_DW_SEN     : // case box B motor down sensor status
	//case ERR_CODE_FARE_AVL_SEN       : // fare box available detection sensor
	//case ERR_CODE_FARE_COVER_SEN     : // fare box Cover detection sensor
	//case ERR_CODE_FARE_FULL_SEN      : // fare box full detection sensor
	//case ERR_CODE_FARE_RETURN_SEN    : // fare box return ticket sensor
	//case ERR_CODE_DIG1_LO1_SEN       : // dig card wheel 1 load 1 sensor status
	//case ERR_CODE_DIG1_LO2_SEN       : // dig card wheel 1 load 2 sensor status
	//case ERR_CODE_DIG2_LO1_SEN       : // dig card wheel 2 load 1 sensor status
	//case ERR_CODE_DIG2_LO2_SEN       : // dig card wheel 2 load 2 sensor status
	//case ERR_CODE_SETBLOCKA          : // set block A failure
	//case ERR_CODE_READBLOCKA         : // read block A failure
	//case ERR_CODE_READBOXID          : // read box id failure
	//case ERR_CODE_SETBOXID           : // set box id failure
	//case ERR_CODE_READRFIDFAILURE    : // two block of RFID couldn't read
	//case ERR_CODE_SETBLOCKB          : // set block B failure
	//case ERR_CODE_READBLOCKB         : // read block B failure
	//case ERR_CODE_A_UP_DW_SEN        : // case box A motor up or down sensor status
	//case ERR_CODE_B_UP_DW_SEN        : // case box B motor up or down sensor status
	//case ERR_CODE_READ_SN            : // read sn failure
	//case ERR_CODE_WRITE_SN           : // write sn failure
	//	//FAULT_CHANNEL
	//case ERR_CODE_CASEBOXA_JAM       : // case box A dig ticket failed
	//case ERR_CODE_CASEBOXB_JAM       : // case box B dig ticket failed
	//case ERR_CODE_BOXA_ANTN_JAM      : // jam in from case boxA to ANTN
	//case ERR_CODE_BOXB_ANTN_JAM      : // jam in from case boxB to ANTN
	//case ERR_CODE_TICK_SELL_JAM      : // ticket sell jam
	//case ERR_CODE_FAREBOX_JAM        : // jam in form ANTN to farebox
	//case ERR_CODE_NOCARD_IN_ANTNAREA : // No card in antenna area
	//case ERR_CODE_CARD_IN_ANTNAREA   : // one card in antenna area
	//	//FAULT_CARDBOX
	//case ERR_CODE_CASEBOXABF_NA      : // case box A,B,farebox not available
	//case ERR_CODE_FAREBOX_COVER      : // fare box cover not opened
	//case ERR_CODE_CASEBOXA_COVER     : // case box A cover error
	//case ERR_CODE_CASEBOXB_COVER     : // case box B cover error
	//case ERR_CODE_CASEBOXA_AE        : // case box A about empty
	//case ERR_CODE_CASEBOXB_AE        : // case box B about empty
	//case ERR_CODE_CASEBOXAB_AE       : // case box A and case box B about empty
	//case ERR_CODE_CASEBOXA_EM        : // case box A empty
	//case ERR_CODE_CASEBOXB_EM        : // case box B empty
	//case ERR_CODE_FAREBOX_FU         : // fare box full
	//case ERR_CODE_CASEBOXAB_EM       : // case box A and B empty
	//case ERR_CODE_CASEBOXA_NA        : // case box A not available
	//case ERR_CODE_CASEBOXB_NA        : // case box B not available
	//case ERR_CODE_FAREBOX_NA         : // fare box not available
	//case ERR_CODE_CASEBOXAB_NA       : // case box A and B not available                                                                    
	//	bRet = true;
	//	break;
	//default:
	//	break;
	//}
	//return bRet;
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
long CTOKENHelper::CommOpen()
{
	TOKEN_CMD_COMM_OPEN cmd_comm_open;
	cmd_comm_open.uiCommPort = theTVM_SETTING.GetTokenComPort();
	cmd_comm_open.uiBaudRate = theTVM_SETTING.GetTokenBaudRate();
	CTOKENCommOpen cTOKENCommOpen(cmd_comm_open);
	long lErrCode = cTOKENCommOpen.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
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
long CTOKENHelper::CommClose()
{
	CTOKENCommClose cTOKENCommClose;
	long lErrCode = cTOKENCommClose.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	return lErrCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����ģ�鹤��ģʽ
//
//@param      (i)TOKEN_CMD_SET_WORK_MODE command      �������
//
//@retval     long    0:�ɹ�; ����:�������
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::SetWorkMode(TOKEN_CMD_SET_WORK_MODE command)
//{
//	CTOKENSetWorkMode cTOKENSetWorkMode(command);
//	long lErrCode = cTOKENSetWorkMode.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	return lErrCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƱ��

@param      (i)TOKEN_CMD_CARD_OUT command      �������
@param      (o)TOKEN_RSP_CARD_OUT& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::CardOut(TOKEN_BOX_NO boxNo, UINT* uiTransportNum)
{
	CTOKENCardOut cTOKENCardOut(boxNo);
	long lErrCode = cTOKENCardOut.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENCardOut.GetTOKENResponse(uiTransportNum);
	}
	// �������
	if(boxNo == CMD_CARD_OUT_BOX_A)
		theDeviceRegAR.AddThScratchWheelACount();
	else
		theDeviceRegAR.AddThScratchWheelBCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ͳ�Ʊ��

@param      (o)TOKEN_RSP_SEND_CARD& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::SendCard(UINT* uiTransportNum)
{
	CTOKENSendCard cTOKENSendCard;
	long lErrCode = cTOKENSendCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENSendCard.GetTOKENResponse(uiTransportNum);
	}
	// �������
	theDeviceRegAR.AddThSendOutMotorCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��

@param      (o)TOKEN_RSP_RETRACT_CARD& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::RetractCard(UINT* uiRetractNum)
{
	CTOKENRetractCard cTOKENRetractCard;
	long lErrCode = cTOKENRetractCard.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENRetractCard.GetTOKENResponse(uiRetractNum);
	}
	// �������
	theDeviceRegAR.AddThLinkElecFlipCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ��

@param      (i)TOKEN_BOX_NO boxNo    Ʊ����

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::CleanOut(TOKEN_BOX_NO boxNo)
{
	CTOKENCleanOut cTOKENCleanOut(boxNo);
	long lErrCode = cTOKENCleanOut.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	// �������
	theDeviceRegAR.AddThLinkElecFlipCount();
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�豸״̬

@param      (o)TOKEN_RSP_GET_DEV_STATUS& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::GetDevStatus(TOKEN_RSP_GET_DEV_STATUS* response)
{
	CTOKENGetDevStatus cTOKENGetDevStatus;
	long lErrCode = cTOKENGetDevStatus.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
		// �����ȡ״̬������λ����λ���豸���������³�ʼ����
		//Reset();
		//UINT retractNum = 0;
		//Init(&retractNum);
	}
	else{
		cTOKENGetDevStatus.GetTOKENResponse(response);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ģ�鸴λ

@param      (i)TOKEN_CMD_RESET command      �������
@param      (o)TOKEN_RSP_RESET& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::Reset()
{
	CTOKENReset cTOKENReset;
	long lErrCode = cTOKENReset.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	//if (ERR_CODE_NONE == lErrCode) {
	//	cTOKENReset.GetTOKENResponse(uiRetractNum);
	//}
	theDeviceRegAR.AddThResetCount();
	// �ȴ���λӲ������30����ɣ������޷�ͨ��
	Sleep(33000);
	return lErrCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����Ʊ��������Ϣ
//
//@param      (i)TOKEN_CMD_SET_COUNT_INFO inBoxID      �������
//@param      (i)TOKEN_RSP_SET_COUNT_INFO& inCardNum   �������
//
//@retval     long    0:�ɹ�; ����:�������
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::SetTicketBoxCountInfo(TOKEN_CMD_SET_COUNT_INFO inBoxID, tTIMCardNum* inCardNum)
//{
//	CTOKENSetTicketBoxCountInfo cTOKENSetTicketBoxCountInfo(inBoxID,inCardNum);
//	long lErrCode = cTOKENSetTicketBoxCountInfo.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	tTIMDevReturn DevReturn;
//	if (ERR_CODE_NONE == lErrCode) {
//		cTOKENSetTicketBoxCountInfo.GetTOKENResponse(&DevReturn);
//	}
//	return lErrCode;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡƱ��������Ϣ
//
//@param      (i)TOKEN_CMD_GET_COUNT_INFO command      �������
//@param      (o)TOKEN_RSP_GET_COUNT_INFO& response    �������
//
//@retval     long    0:�ɹ�; ����:�������
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::GetTicketBoxCountInfo(TOKEN_CMD_GET_COUNT_INFO command, tTIMCardNum* response)
//{
//	CTOKENGetTicketBoxCountInfo cTOKENGetTicketBoxCountInfo(command);
//	long lErrCode = cTOKENGetTicketBoxCountInfo.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	if (ERR_CODE_NONE == lErrCode) {
//		cTOKENGetTicketBoxCountInfo.GetTOKENResponse(response);
//	}
//	return lErrCode;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ģ���ʼ��

@param      (i)TOKEN_CMD_INIT command      �������
@param      (o)TOKEN_RSP_INIT& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::Init(UINT* uiRetractNum)
{
	// 20171124��lichao��ʵ��Ӳ��ͨ�����������ܿ�Ʊ����˲��������������Ὣ�ݴ����ı�Ʊ�嵽�����䡣
	TOKEN_CMD_INIT command = CMD_INIT_NOT_CLEAN/*CMD_INIT_CLEAN*/;
	CTOKENInit cTOKENInit(command);
	long lErrCode = cTOKENInit.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENInit.GetTOKENResponse(uiRetractNum);
	}
	return lErrCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�豸�����汾��Ϣ

@param      (o)TOKEN_RSP_GET_VERSION& response    �������

@retval     long    0:�ɹ�; ����:�������

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENHelper::GetVersion(TOKEN_RSP_GET_VERSION* response)
{
	CTOKENGetVersion cTOKENGetVersion;
	long lErrCode = cTOKENGetVersion.ExecuteCommand();
	if (IsExceptionCode(lErrCode)) {
		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
	}
	if (TIM_ERR_CODE_NONE == lErrCode) {
		cTOKENGetVersion.GetTOKENResponse(response);
	}
	return lErrCode;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����ģ�����
//
//@param      (i)TOKEN_CMD_TEST_SETTING command      �������
//@param      (o)TOKEN_RSP_TEST_SETTING& response    �������
//
//@retval     long    0:�ɹ�; ����:�������
//
//@exception  CTCMException
//*/
////////////////////////////////////////////////////////////////////////////
//long CTOKENHelper::TestSetting(TOKEN_CMD_TEST_SETTING command)
//{
//	CTOKENTestSetting CTOKENTestSetting(command);
//	long lErrCode = CTOKENTestSetting.ExecuteCommand();
//	if (IsExceptionCode(lErrCode)) {
//		throw CTOKENHardwareException(lErrCode, _T(__FILE__), __LINE__);
//	}
//	return lErrCode;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡRFID

@param      IN TOKEN_CMD_SET_COUNT_INFO boxType(��������)
			OUT tTIMRFIDInfo* pTimRfidInfo(���RFID��Ϣ)

@retval     bool true�ɹ� falseʧ��;

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
bool CTOKENHelper::ReadTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
{
	CTOKENReadRFIDInfo readRfid(pTimRfidInfo->ucRfidNo);
	long lRet = readRfid.ExecuteCommand();

	if(IsExceptionCode(lRet)){
		throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	}
	
	if (TIM_ERR_CODE_NONE == lRet) {
		readRfid.GetTOKENResponse(pTimRfidInfo);
	}	

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дƱ��RFID

@param      ��

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
bool CTOKENHelper::WriteTicketBoxRFID(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
{
	CTOKENWriteRFIDInfo writeRfid(pTimRfidInfo);
	long lRet = writeRfid.ExecuteCommand();
	if(IsExceptionCode(lRet)){
		throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      IN SC_REPLACE_BOX_CMD_INFO& rpsBoxInfo

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CTOKENHelper::ReplaceTicketBox(IN SC_REPLACE_BOX_CMD_INFO& rpsBoxInfo)
//{
	//// Ӳ������(ж��)
	//if(rpsBoxInfo.actionType == TOKEN_UNINSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TOKEN_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_DOWN);
	//		}
	//		else if(rpsBoxInfo.boxId == TOKEN_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_DOWN);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

	//// ��װ
	//if(rpsBoxInfo.actionType == TOKEN_INSTALL){
	//	try{
	//		if(rpsBoxInfo.boxId == TOKEN_BOX_A){
	//			TestSetting(CMD_TEST_A_TRAY_UP);
	//		}
	//		else if(rpsBoxInfo.boxId == TOKEN_BOX_B){
	//			TestSetting(CMD_TEST_B_TRAY_UP);
	//		}
	//	}
	//	catch(...){
	//		return false;
	//	}
	//}

//	// �������
//	if(rpsBoxInfo.boxId == TOKEN_BOX_A)
//		theDeviceRegAR.AddThBoxAUpDownCount();
//	else
//		theDeviceRegAR.AddThBoxBUpDownCount();
//
//	/*if(lRet != 0){
//	return false;
//	}*/
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ģ�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
 int CTOKENHelper::UpdateProgram(){
	int iRet = 0;
	try
	{
		//CString fileName = (CString)STR_TVM_TOKEN_APP;
		//vector<CString> updateFiles;

		//CString sAppPath;
		//GetAppPath(sAppPath);
		//CString updateFileFolder = sAppPath + TVM_TOKEN_UPDATE_DIR;//BOM_TC_UPDATE_DIR;


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
		//	CString upgradeVersionStr = thePARAM_HELPER.GetProgVerFromProgName(updateFile);	// ��ȡ������TOKEN�汾
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
		//	TOKENUpdateCommand cmdUpdate(theSETTING.GetTOKENComPort(),theSETTING.GetTOKENBaudRate(),theSETTING.GetTOKENUpadateBaudRate(),(char*)strFilePath.c_str());
		//	if (RW_RSP_OK == cmdUpdate.Execute()) {
		//		DeleteFile(updateFile);
		//		//theSETTING.SetRWUpdateFlag(0,true);
		//		//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_APP,upgradeVersion);//���±��ذ汾
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
 bool CTOKENHelper::TryRepareTOKEN(){

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
 //bool CTOKENHelper::GetStaticBoxId(IN TOKEN_BOX_NO boxId,OUT tTIMRFIDInfo& rfidInfo){

	// rfidInfo.ucBoxID[0] = theMAINTENANCE_INFO.GetDeviceType();
	// rfidInfo.ucBoxID[1] = 0x01;
	// rfidInfo.ucBoxID[2] = Dec2Hex(theMAINTENANCE_INFO.GetStationCode());
	// BYTE bNum = theMAINTENANCE_INFO.GetEquNumberInGroup();
	// CString strNum(_T(""));

	// switch(boxId){
	// case TOKEN_BOX_A:
	//	 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
	//	 rfidInfo.ucBoxID[3] = 2 * bNum - 2/*_tcstoul(strNum,NULL,16)*/;
	//	 break;
	// case TOKEN_BOX_B:
	//	 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-1);
	//	 rfidInfo.ucBoxID[3] = 2 * bNum - 1/*_tcstoul(strNum,NULL,16)*/;
	//	 break;
	// case TOKEN_BOX_C:
	//	 rfidInfo.ucBoxID[1] = 0x02;
	//	 //strNum.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode()*3-2);
	//	 rfidInfo.ucBoxID[3] = 2 * bNum/*_tcstoul(strNum,NULL,16)*/;
	//	 break;
	// default:
	//	 return false;
	// }

	// return true;
 //}


 long CTOKENHelper::TokenCyclerRfidInit()
 {
	long lRet = 0;

	// �Ͽ�����
	CTCRfidClose rfidCloseCmd;
	lRet = rfidCloseCmd.ExecuteCommand();

	// ���ӻ�����RFID��д��
	CTCRfidConnect rfidCommand(theTVM_SETTING.GetTokenCyclerComPort(), theTVM_SETTING.GetTokenCyclerBaudRate());
	lRet = rfidCommand.ExecuteCommand();
	//if(IsExceptionCode(lRet)){
	// throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	//}
	if (lRet){
		return lRet;
	}

	// ��дǰ׼��
	CTCRfidInit rfidInitCommand;
	lRet = rfidInitCommand.ExecuteCommand();
	//if(IsExceptionCode(lRet)){
	// throw CTOKENHardwareException(lRet,_T(__FILE__),__LINE__);
	//}
	if (lRet){
		return lRet;
	}

	return 0;
 }


 long CTOKENHelper::TokenCyclerRfidRead(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
 {
	long lRet = 0;
	lRet = TokenCyclerRfidInit();
	if (lRet){
		return lRet;
	}

	CTCRfidRead rfidCommand;
	lRet = rfidCommand.ExecuteCommand();
	if (lRet==0){
		rfidCommand.GetResponse(pTimRfidInfo->rfidData.uBoxNo);
	}

	return 0;
 }


 long CTOKENHelper::TokenCyclerRfidWrite(TOKEN_READ_WRITE_RFID* pTimRfidInfo)
 {
	 long lRet = 0;
	 lRet = TokenCyclerRfidInit();
	 if (lRet){
		 return lRet;
	 }

	 CTCRfidWrite rfidCommand(pTimRfidInfo->rfidData.uBoxNo);
	 lRet = rfidCommand.ExecuteCommand();

	 return lRet;
 }


 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      ȡTOKEN��A��RFID

 @param      ��

 @retval     CString strRFID;���ӵ�RFID

 @exception  ��
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadTokenBoxARFID()
 {
	CString strRFID(_T(""));

	try{
		USES_CONVERSION;
		TOKEN_READ_WRITE_RFID readTokenData;
		readTokenData.ucRfidNo = 0x04;
		if(ReadTicketBoxRFID(&readTokenData)){
			CString strTemp;
			strRFID.Format(_T("%s"), A2T((const char*)readTokenData.rfidData.uBoxNo));
			return strRFID;
		}

		return strRFID;
	 }
	 catch(...){
		return strRFID;
	 }
 }


 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      ȡTOKEN��B��RFID

 @param      ��

 @retval     CString strRFID;���ӵ�RFID 

 @exception  ��
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadTokenBoxBRFID()
 {
	 CString strRFID(_T(""));

	 try{
		 USES_CONVERSION;
		 TOKEN_READ_WRITE_RFID readTokenData;
		 readTokenData.ucRfidNo = 0x03;
		 if(ReadTicketBoxRFID(&readTokenData)){
			 CString strTemp;
			 strRFID.Format(_T("%s"), A2T((const char*)readTokenData.rfidData.uBoxNo));
			 return strRFID;
		 }

		 return strRFID;
	 }
	 catch(...){
		 return strRFID;
	 }
 }

 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      ȡ��Ʊ��RFID

 @param      ��

 @retval     CString strRFID�����ӵ�ID

 @exception  ��
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadInvalidBoxRFID()
 {
	 CString strRFID(_T(""));

	 try{
		 strRFID.Format(_T("%02d03%04d"),theMAINTENANCE_INFO.GetLineCode(),1);
		 return strRFID;
	 }
	 catch(...){
		 return strRFID;
	 }
 }


  //////////////////////////////////////////////////////////////////////////
 /*
 @brief      ȡTOKEN�������RFID

 @param      ��

 @retval     CString strRFID;���ӵ�RFID 

 @exception  ��
 */
 //////////////////////////////////////////////////////////////////////////
 CString CTOKENHelper::ReadTokenBoxCollectRFID()
 {
	 CString strRFID(_T(""));

	 try{
		 USES_CONVERSION;
		 TOKEN_READ_WRITE_RFID sTimRfidInfo;
		 if(TokenCyclerRfidRead(&sTimRfidInfo)==0){
			 CString strTemp;
			 strRFID.Format(_T("%s"), A2T((const char*)sTimRfidInfo.rfidData.uBoxNo));
			 return strRFID;
		 }

		 return strRFID;
	 }
	 catch(...){
		 return strRFID;
	 }
 }