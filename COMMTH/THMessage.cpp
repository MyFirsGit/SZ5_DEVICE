#include "StdAfx.h"
#include "THMessage.h"
//#include "BomInfo.h"
#include "THException.h"
#include "THHardwareException.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
typedef enum TH_CMD_SENSOR
{
	SENSOR01_ID = 61000,	// 61000 (0:����;1:����) Ʊ�����ڼ�⴫����
	SENSOR02_ID = 61001,    // 61001 (0:����;1:����) ��д����⴫����
	SENSOR07_ID = 61006,    // 61006 (0:��λ;1:�Ƴ�) Ʊ�� A ��λ��⴫����
	SENSOR08_ID = 61007,    // 61007 (0:��;1:�ر�) Ʊ�� A ���Ǽ�⴫����������Ʊ��δʹ�ã�
	SENSOR09_ID = 61008,    // 61008 (0:����;1:����) Ʊ�� A ���մ�����   
	SENSOR10_ID = 61009,    // 61009 (0:����;1:�ѿ�) Ʊ�� A �մ�����
	SENSOR13_ID = 61012,    // 61012 (0:��λ;1:�Ƴ�) Ʊ�� B ��λ��⴫����
	SENSOR14_ID = 61013,    // 61013 (0:��;1:�ر�) Ʊ�� B ���Ǽ�⴫����������Ʊ��δʹ�ã� 
	SENSOR15_ID = 61014,    // 61014 (0:����;1:����) Ʊ�� B ���մ�����
	SENSOR16_ID = 61015,    // 61015 (0:����;1:�ѿ�) Ʊ�� B �մ�����
	SENSOR19_ID = 61018,    // 61018 (0:��λ;1:�Ƴ�) ��Ʊ�䵽λ��⴫����
	SENSOR20_ID = 61019,    // 61019 (0:��;1:�ر�) ��Ʊ�䶥�Ǽ�⴫����������Ʊ��δʹ�ã�
	SENSOR21_ID = 61020,    // 61020 (0:����;1:����) ��Ʊ������⴫����
};

typedef enum _tem_th_rsp_code {
	TH_RSP_OK = 0x0000,    // �����ɹ�
	TH_RSP_NG = 0x0001,    // ����ʧ��
};
//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHCommands���캯��

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCommands::CTHCommands()
{
	hInst = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHCommands��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCommands::~CTHCommands()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������

@param      long lReturnCode    ��������         

@retval     long    �ɹ�:0; ʧ��:����Ӧ����         

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommands::DealReturnCode(long lReturnCode, tTIMDevReturn* pDevReturn)
{
	long lRet = TIM_ERR_CODE_NONE;
	if (TIM_ERR_CODE_NONE != lReturnCode) {
		lRet = (long)pDevReturn->uiErrorCode;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommands::ExecuteCommand()
{
	return TH_RSP_OK;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     long     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommands::IsValidCommand()
{
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCommOpen::CTHCommOpen(TH_CMD_COMM_OPEN &command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCommOpen::~CTHCommOpen()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommOpen::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommOpen::ExecuteCommand()
{
	long lCompleteCode = TIM_CommOpen(m_command.uiCommPort, m_command.uiBaudRate,_T("D:\\LOG\\"),_T("TIM_"));
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_INIT cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHInit::CTHInit(TH_CMD_INIT_RESET command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHInit::~CTHInit(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHInit::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHInit::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_Init(m_command, &m_uiRetractNum,&tModuleStatus, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_INIT* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHInit::GetTHResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_uiRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_RESET cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHReset::CTHReset( TH_CMD_INIT_RESET command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHReset::~CTHReset(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHReset::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHReset::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_Reset(m_command, &m_uiRetractNum, &tModuleStatus, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_RESET* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHReset::GetTHResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_uiRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHGetVersion::CTHGetVersion(void) : CTHCommands()
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
CTHGetVersion::~CTHGetVersion(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetVersion::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetVersion::ExecuteCommand()
{
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_GetVersion(m_response.cFirmWareVersion, m_response.cDriverVersion,&tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_GET_VERSION* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetVersion::GetTHResponse(TH_RSP_GET_VERSION* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response->cDriverVersion, m_response.cDriverVersion, sizeof(m_response.cDriverVersion));
			memcpy(response->cFirmWareVersion, m_response.cFirmWareVersion, sizeof(m_response.cFirmWareVersion));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHGetDevStatus::CTHGetDevStatus(void) : CTHCommands()
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
CTHGetDevStatus::~CTHGetDevStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetDevStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetDevStatus::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	//TH_RSP_GET_DEV_STATUS tDevStatus;

	long lCompleteCode = TIM_GetDevStatus(&tModuleStatus, &tDevReturn);
	if (TH_RSP_OK == lCompleteCode) {

		// ����ģ���״̬��Ϣ
		m_response.bCardInReadArea       = tModuleStatus.bCardInReadArea;
		// Ʊ��״̬
		m_response.cCassetteAStatus      = tModuleStatus.cCassetteAStatus;
		m_response.cCassetteBStatus      = tModuleStatus.cCassetteBStatus;
		m_response.cRejectCassetteStatus = tModuleStatus.cRejectCassetteStatus;
		// Ʊ��Ʊ������
		m_response.uiCassetteACardNum    = tModuleStatus.tCardNum.uiCassetteACardNum;
		m_response.uiCassetteBCardNum    = tModuleStatus.tCardNum.uiCassetteBCardNum;
		m_response.uiRejectCassetteNum   = tModuleStatus.tCardNum.uiRejectCassetteNum;
		// ����״̬��Ϣ
		m_response.uiType                = tDevReturn.uiType;
		m_response.uiErrorType           = tDevReturn.uiErrorType;
		m_response.uiErrorCode           = tDevReturn.uiErrorCode;

		// ������״̬(0:����  1:����  3:δ�ڵ�  4:�ڵ�  0xff:�����ڸô�����)
		int nCount = (int)tModuleStatus.uiSensorCount;
		for (int i = 0; i < nCount; i++) {
			UINT uiSensorID = tModuleStatus.tSensorType[i].uiSensotID;
			BYTE btSensorStatus = tModuleStatus.tSensorType[i].btSensorStatus;
			switch (uiSensorID)
			{
				//case SENSOR02_ID:m_response.cRwAreaDetect = btSensorStatus;       break;						// 61001 (0:����;1:����) ��д����⴫����
				//case SENSOR07_ID:m_response.bBoxAArrive = (btSensorStatus == 0x04 ? true: false);break;			// 61012 (0:��λ;1:�Ƴ�) Ʊ�� B ��λ��⴫����
				//case SENSOR08_ID:m_response.bBoxACoverArrive = (btSensorStatus == 0x03 ? true : false); break;	// 61013 (0:��;1:�ر�) Ʊ�� B ���Ǽ�⴫����������Ʊ��δʹ�ã�
				//case SENSOR09_ID:m_response.bBoxANearlyEmpty = btSensorStatus;    break;						// 61014 (0:����;1:����) Ʊ�� B ���մ�����
				//case SENSOR10_ID:m_response.bBoxAEmpty =  (btSensorStatus == 0x04 ? true : false);break;		// 61015 (0:����;1:�ѿ�) Ʊ�� B �մ�����
				//case SENSOR13_ID:m_response.bBoxBArrive = (btSensorStatus == 0x04 ? true : false);break;		// 61006 (0:��λ;1:�Ƴ�) Ʊ�� A ��λ��⴫����
				//case SENSOR14_ID:m_response.bBoxBCoverArrive = (btSensorStatus == 0x03 ? true : false); break;	// 61007 (0:��;1:�ر�) Ʊ�� A ���Ǽ�⴫����������Ʊ��δʹ�ã�
				//case SENSOR15_ID:m_response.bBoxBNearlyEmpty = btSensorStatus;    break;						// 61008 (0:����;1:����) Ʊ�� A ���մ�����
				//case SENSOR16_ID:m_response.bBoxBEmpty =  (btSensorStatus == 0x04 ? true : false);	break;		// 61009 (0:����;1:�ѿ�) Ʊ�� A �մ�����

				//case SENSOR19_ID:m_response.bBoxCArrive = (btSensorStatus == 0x04 ? true : false);break;		// 61018 (0:��λ;1:�Ƴ�) ��Ʊ�䵽λ��⴫����
				//case SENSOR20_ID:m_response.bBoxCCoverArrive = true/*(btSensorStatus == 0x03 ? true : false)*/;break;	// 61019 (0:��;1:�ر�) ��Ʊ�䶥�Ǽ�⴫����������Ʊ��δʹ�ã�û�ж���
				//case SENSOR21_ID:m_response.bBoxCFull =  (btSensorStatus == 0x04 ? true : false);break;			// 61020 (0:����;1:����) ��Ʊ������⴫����

			case CODE_SENSOR53_ID:m_response.cRwAreaDetect = (btSensorStatus==0?true:false);break;			// 41006 (0:����;1:����) ��д����⴫����
			case CODE_SENSOR56_ID:m_response.bBoxAArrive = (btSensorStatus == 0?true:false);break;			// 41009 (0:��λ;1:�Ƴ�) Ʊ�� A ��λ��⴫����
			case CODE_SENSOR74_ID:m_response.bBoxACoverArrive = (btSensorStatus==1?true:false);break;		// 41027 (0:��;1:�ر�) Ʊ�� A ���Ǽ�⴫����������Ʊ��δʹ�ã�
			case CODE_SENSOR55_ID:m_response.bBoxANearlyEmpty = (btSensorStatus==1?true:false);break;		// 41008 (0:����;1:����) Ʊ�� A ���մ�����
			case CODE_SENSOR57_ID:m_response.bBoxAEmpty =(btSensorStatus==1?true:false);break;				// 41010 (0:����;1:�ѿ�) Ʊ�� A �մ�����
			case CODE_SENSOR62_ID:m_response.bBoxBArrive = (btSensorStatus==0?true:false);break;			// 41015 (0:��λ;1:�Ƴ�) Ʊ�� B ��λ��⴫����
			case CODE_SENSOR75_ID:m_response.bBoxBCoverArrive = (btSensorStatus==1?true:false);break;		// 41028 (0:��;1:�ر�) Ʊ�� B ���Ǽ�⴫����������Ʊ��δʹ�ã�
			case CODE_SENSOR61_ID:m_response.bBoxBNearlyEmpty = (btSensorStatus==1?true:false);break;		// 41014 (0:����;1:����) Ʊ�� B ���մ�����
			case CODE_SENSOR63_ID:m_response.bBoxBEmpty =  (btSensorStatus==1?true:false);break;			// 41016 (0:����;1:�ѿ�) Ʊ�� B �մ�����

			case CODE_SENSOR71_ID:m_response.bBoxCArrive = (btSensorStatus == 0?true:false);break;		// 41024 (0:��λ;1:�Ƴ�) ��Ʊ�䵽λ��⴫����
			case CODE_SENSOR76_ID:m_response.bBoxCCoverArrive = true/*(btSensorStatus==0?true:false)*/;break;	// 41029 (0:��;1:�ر�) ��Ʊ�䶥�Ǽ�⴫����������Ʊ��δʹ�ã�
			case CODE_SENSOR72_ID:m_response.bBoxCFull = (btSensorStatus==1?true:false);break;			// 41025 (0:����;1:����) ��Ʊ������⴫����
				default:break;
			}
		}
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_GET_DEV_STATUS* response    

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetDevStatus::GetTHResponse(TH_RSP_GET_DEV_STATUS* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			// ������״̬(0:����  1:����  3:δ�ڵ�  4:�ڵ�  0xff:�����ڸô�����)
			response->bCardInReadArea       = m_response.bCardInReadArea      ;
			response->cCassetteAStatus      = m_response.cCassetteAStatus     ;
			response->cCassetteBStatus      = m_response.cCassetteBStatus     ;
			response->cRejectCassetteStatus = m_response.cRejectCassetteStatus;
			response->uiCassetteACardNum    = m_response.uiCassetteACardNum   ;
			response->uiCassetteBCardNum    = m_response.uiCassetteBCardNum   ;
			response->uiRejectCassetteNum   = m_response.uiRejectCassetteNum  ;
			response->cRwAreaDetect         = m_response.cRwAreaDetect        ;
			response->bBoxAArrive           = m_response.bBoxAArrive          ;
			response->bBoxACoverArrive      = m_response.bBoxACoverArrive     ;
			response->bBoxANearlyEmpty      = m_response.bBoxANearlyEmpty     ;
			response->bBoxAEmpty            = m_response.bBoxAEmpty           ;
			response->bBoxBArrive           = m_response.bBoxBArrive          ;
			response->bBoxBCoverArrive      = m_response.bBoxBCoverArrive     ;
			response->bBoxBNearlyEmpty      = m_response.bBoxBNearlyEmpty     ;
			response->bBoxBEmpty            = m_response.bBoxBEmpty           ;
			response->bBoxCArrive           = m_response.bBoxCArrive          ;
			response->bBoxCCoverArrive      = m_response.bBoxCCoverArrive     ;
			response->bBoxCFull             = m_response.bBoxCFull            ;
			response->uiType                = m_response.uiType               ;
			response->uiErrorType           = m_response.uiErrorType          ;
			response->uiErrorCode           = m_response.uiErrorCode          ;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_CARD_OUT cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCardOut::CTHCardOut(TH_CMD_CARD_OUT command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCardOut::~CTHCardOut(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCardOut::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCardOut::ExecuteCommand()
{
	long lCompleteCode = TIM_CardOut((char)m_command, &m_tModuleStatus, &m_uiTransportNum,&m_tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_CARD_OUT* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHCardOut::GetTHResponse(UINT* uiTransportNum)
{
	try {
		if(uiTransportNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiTransportNum = m_uiTransportNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHSendCard::CTHSendCard(void) : CTHCommands()
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
CTHSendCard::~CTHSendCard(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHSendCard::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHSendCard::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_SendCard(&tModuleStatus, &m_uiTransportNum, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_SEND_CARD* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHSendCard::GetTHResponse(UINT* uiTransportNum)
{
	try {
		if(uiTransportNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiTransportNum = m_uiTransportNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHRetractCard::CTHRetractCard(void) : CTHCommands()
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
CTHRetractCard::~CTHRetractCard(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHRetractCard::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHRetractCard::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_RetractCard(&tModuleStatus, &m_uiRetractNum, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_RETRACT_CARD* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHRetractCard::GetTHResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_uiRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHGetAuditData::CTHGetAuditData(void) : CTHCommands()
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
CTHGetAuditData::~CTHGetAuditData(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetAuditData::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetAuditData::ExecuteCommand()
{
	long lCompleteCode = TIM_GetAuditData(&m_tAuditCardNum, &m_tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_GET_AUDIT_DATA* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetAuditData::GetTHResponse(tTIMAuditCardNum* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response,&m_tAuditCardNum,sizeof(tTIMAuditCardNum));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_GET_COUNT_INFO cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHGetTicketBoxCountInfo::CTHGetTicketBoxCountInfo(TH_BOX_TYPE command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHGetTicketBoxCountInfo::~CTHGetTicketBoxCountInfo(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetTicketBoxCountInfo::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetTicketBoxCountInfo::ExecuteCommand()
{
	//long lCompleteCode = GetTicketBoxCountInfo(m_command, &m_response);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_GET_COUNT_INFO* response    

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetTicketBoxCountInfo::GetTHResponse(tTIMCardNum* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			response->uiCassetteACardNum = m_response.uiCassetteACardNum;
			response->uiCassetteBCardNum = m_response.uiCassetteBCardNum;
			response->uiRejectCassetteNum = m_response.uiRejectCassetteNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_SET_COUNT_INFO cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHSetTicketBoxCountInfo::CTHSetTicketBoxCountInfo(TH_BOX_TYPE iBoxID,tTIMCardNum* iCardNum) : CTHCommands()
{
	m_boxID = iBoxID;
	m_cardNum.uiCassetteACardNum = iCardNum->uiCassetteACardNum;
	m_cardNum.uiCassetteBCardNum = iCardNum->uiCassetteBCardNum;
	m_cardNum.uiRejectCassetteNum= iCardNum->uiRejectCassetteNum;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHSetTicketBoxCountInfo::~CTHSetTicketBoxCountInfo(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHSetTicketBoxCountInfo::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

long CTHSetTicketBoxCountInfo::ExecuteCommand()
{
	long lCompleteCode = TIM_SetTicketBoxCountInfo(m_boxID, &m_cardNum,&m_DevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_SET_COUNT_INFO* response  

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHSetTicketBoxCountInfo::GetTHResponse(tTIMDevReturn* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			response = &m_DevReturn;
			/*response->uiCassetteACardNum = m_response.uiCassetteACardNum;
			response->uiCassetteBCardNum = m_response.uiCassetteBCardNum;
			response->uiRejectCassetteNum = m_response.uiRejectCassetteNum;*/
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHGetSN::CTHGetSN(void) : CTHCommands()
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
CTHGetSN::~CTHGetSN(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetSN::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHGetSN::ExecuteCommand()
{
	long lCompleteCode = TIM_GetSN(m_response.cModuleSN, &m_DevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

@param      (o)TH_RSP_GET_SN* response 

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTHGetSN::GetTHResponse(TH_RSP_GET_SN* response)
{
	try {
		if(response == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response->cModuleSN, m_response.cModuleSN, sizeof(response->cModuleSN));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHCommClose::CTHCommClose() : CTHCommands()
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
CTHCommClose::~CTHCommClose()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommClose::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHCommClose::ExecuteCommand()
{
	long lCompleteCode = TIM_CommClose();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_SET_WORK_MODE cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHSetWorkMode::CTHSetWorkMode(TH_CMD_SET_WORK_MODE command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHSetWorkMode::~CTHSetWorkMode(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHSetWorkMode::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHSetWorkMode::ExecuteCommand()
{
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_SetWorkMode((char)m_command, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TH_CMD_TEST_SETTING cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSetting::CTHTestSetting(TH_CMD_TEST_SETTING command) : CTHCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSetting::~CTHTestSetting(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHTestSetting::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHTestSetting::ExecuteCommand()
{
	tTIMModuleStatus tModuleStatus;
	tTIMDevReturn tDevReturn;
	long lCompleteCode = TIM_TestSetting((char)m_command, &tModuleStatus, &tDevReturn);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHReadRFIDInfo::CTHReadRFIDInfo(BYTE bTicketBoxNo){
	m_bTicketBoxNo = bTicketBoxNo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHReadRFIDInfo::~CTHReadRFIDInfo(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��RFIDִ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHReadRFIDInfo::ExecuteCommand(){

	long lRet = TIM_GetTicketBoxRFIDInfo((char)m_bTicketBoxNo,&m_RfidInfo,&m_TimDevReturn);
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ȡRFID�ķ���

@param      tTIMRFIDInfo* rspRfid

@retval     long lRet;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHReadRFIDInfo::GetTHResponse(tTIMRFIDInfo* rspRfid){
	try{
		if(rspRfid == NULL){
			throw CTHException(CTHException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(rspRfid,&m_RfidInfo,sizeof(tTIMRFIDInfo));
		}
			
		return 0;
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTHException(CTHException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дRFID���캯��

@param      BYTE bTicketBoxNo ��������
			tTIMRFIDInfo& pTimRfidInfo RFID��Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHWriteRFIDInfo::CTHWriteRFIDInfo(BYTE bTicketBoxNo,tTIMRFIDInfo* pTimRfidInfo){
	m_bTicketBoxNo = bTicketBoxNo;
	m_pRfidInfo = pTimRfidInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHWriteRFIDInfo::~CTHWriteRFIDInfo(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дRFIDִ�������

@param      ��

@retval     long �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHWriteRFIDInfo::ExecuteCommand(){
	long lRet = ThWriteRfidInfo();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дRFIDִ�к���

@param      ��

@retval     long �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHWriteRFIDInfo::ThWriteRfidInfo(){
	return TIM_SetTicketBoxRFIDInfo((char)m_bTicketBoxNo,m_pRfidInfo,&m_TimDevReturn);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		Ʊ��������캯��

@param      BYTE bTicketBoxNo ����Ʊ����
			BYTE bBoxMode	  ����Ʊ�����ͣ�ж��/��װ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHReplaceTicketBox::CTHReplaceTicketBox(BYTE bTicketBoxNo,BYTE bBoxMode){
	m_bTicketBoxNo = bTicketBoxNo;
	m_bBoxMode = bBoxMode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ʊ�������������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHReplaceTicketBox::~CTHReplaceTicketBox(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ʊ�����ִ������

@param      ��

@retval     long ִ�н��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHReplaceTicketBox::ExecuteCommand(){
	long lRet = ReplaceTicketBox();

	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ʊ�����ִ�к���

@param      ��

@retval     long ִ�н��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTHReplaceTicketBox::ReplaceTicketBox(){
	return TIM_ReplaceTBox((char)m_bTicketBoxNo,m_bBoxMode,&m_TimDevReturn);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH���¹��캯��

@param      WORD port,WORD speed, char* pFilePath

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
THUpdateCommand::THUpdateCommand(DWORD port,DWORD speed,DWORD updateSpeed, char* pFilePath){
	m_dPort = port;
	m_dSpeed = speed;
	m_sFilePath = pFilePath;
	m_dUpdateSpeed = updateSpeed;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH������������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
THUpdateCommand::~THUpdateCommand(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH����ִ�к���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long THUpdateCommand::ExecuteCommand(){
	long lRet = THDownload(m_dPort,m_dSpeed,m_dUpdateSpeed,m_sFilePath);
	this->OnComplete(lRet);
	return lRet;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      TH���¹��캯��

@param      WORD port,WORD speed, char* pFilePath

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
THTrayContral::THTrayContral(int nTrayType, int nTrayCmd){
	m_nTrayType = nTrayType;
	m_nTrayCmd = nTrayCmd;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH������������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
THTrayContral::~THTrayContral(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TH����ִ�к���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long THTrayContral::ExecuteCommand(){
	//tTIMDevReturn ret;
	//long lRet = TIM_TrayContral(m_nTrayType,m_nTrayCmd,&ret);
	//this->OnComplete(lRet);
	//return lRet;
	return 0;
}