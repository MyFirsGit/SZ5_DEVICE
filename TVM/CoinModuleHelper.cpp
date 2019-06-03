#include "StdAfx.h"
#include "Sync.h"
#include "CoinModuleHelper.h"
#include "CTCoinCountInfo.h"
#include "CoinShutterCommand.h"
#include "IOModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**
@brief  Ӳ��ģ�������ʵ��
*/


// Ӳ���쳣����
#define INTERNAL_ERROR(errCode) \
{if (IsExceptionCode(errCode)) {\
        throw CCHException(errCode, _T(__FILE__), __LINE__);\
	}else\
	{throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);}}

// Ӳ�ҽ��տڴ����쳣
//#define SHUTTER_ERROR(errCode)\
//{if(IsShutterExceptionCode(errCode)){\
//	throw CCHShutterException(errCode,_T(__FILE__),__LINE__);\
//}else{\
//	throw CCHShutterHardwareException(errCode,_T(__FILE__),__LINE__);}}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleHelper::CCoinModuleHelper():
m_nSortTrayRunCount(0)
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
CCoinModuleHelper::~CCoinModuleHelper(void)
{
}


//////////////////////////////////////////////////////////////////////////
BOOL CCoinModuleHelper::IsExceptionCode(long errCode)
{
	bool ret = false;

	switch(errCode) {
	case    CH_SET_PARAM_ERR       : // �趨ֵ�������쳣
	case    CH_FUNC_PARAM_ERR      : // ���������쳣
	case    CH_SEQUENCE_ERR	       : // ��������˳���쳣
	case    CH_FUNC_ERR_RETURN	   : // ���������쳣
	case    CH_LOGIC_ERR	       : // �߼��쳣
	case    CH_OTHER_ERR           : // FTP��ش���   
	case    CHTRS_RES_WINERR       : // ϵͳ����
	case	CCHException::ERROR_CH_SEQUNCE_ERR   :		// ����ʱ�����
	case	CCHException::ERROR_CH_SEND		     :		// �������ݴ���
	case	CCHException::ERROR_CH_RECEIVE	     :		// �������ݴ���
	case    CCHException::ERROR_CH_RECEIVE_TIMEOUT:		// �������ݴ���
	case    CHTRS_ERR_HANDLE       : // The handle is invalid.
	case    CHTRS_RES_ERR_CHREAD   : // Could not create thread. Window error.
	case    CHTRS_RES_ERR_TIMEOUT  : // Timeout. Could not finished task in time.
	case    CHTRS_RES_RESET        : // Operation was terminate because of reseting.
	case    CHTRS_RES_STRANGE      : // Unrecognizable response.
	case    CHTRS_RECV_EMPTY       : // There is no data to receive.
	case    CHTRS_HTDL_ERR_FULL    : // Could not open connection because the conneciton pool is full
	case    CHTRS_HTDL_ERR_OPNTO   : // The operation could not finish in time.
	case    CHTRS_HTDL_ERR_WORKNG  : // Could not allocate workspace, out of memory.
	case    CHTRS_HTDL_ERR_SQUENG  : // Could not allocate the queue, out of memory.
	case    CHTRS_HTDL_ERR_RBUFNG  : // Could not allocate buffer for receiving data, out of memory.
	case    CHTRS_RFID_A_SUM_ERR   : // Ʊ��A��RFID����SUMֵ�쳣
	case    CHTRS_RFID_B_SUM_ERR   : // Ʊ��B��RFID����SUMֵ�쳣
	case    CHTRS_RFID_A_B_SUM_ERR : // Ʊ��A��B��RFID����SUMֵ�쳣
	case    CHTRS_MSG_EMPTY	       : // The message is empty now.
	case    CHTRS_MSG_OVRFLW	   : // The buffer is too small to store message.
	case    CHTRS_CMD_INVALID	   : // Unsupported command.
	case    CHTRS_CMD_PARAM	       : // Invalid parameter.
	case    CHTRS_CMD_REJECT	   : // Too many commands are queuing, could not send command.
		ret = true;
		break;
	//Hardware Error!
	//case	CCHException::ERROR_CH_OPEN_COM      :      //��ʱ���޷��򿪴��ڴ���
	//case	CCHException::ERROR_CH_CLOSE_COM     :      //��ʱ���رմ��ڴ���
	//case	CCHException::ERROR_CH_OPEN_ERROR    :      //��ʱ���򿪴���
	//case	CCHException::ERROR_CH_WAIT_TIMEOUT  :      //��ӡʱ  ��ʱ
	//case	CCHException::ERROR_CH_CLOSE         :      //�ر�ʱ���رմ��ڴ���
	case    CHTRS_HTDL_ERR_SENDFAIL: // Sending error. Unknow reason.
	case    CHTRS_HTDL_ERR_PORT    : // Could not open the specified serial port.
	case    CHTRS_HTDL_ERR_CTSOFF  : // Serial port does not support CTS (clear-to-send) event
	case    CHTRS_HTDL_ERR_SQUE    : // ���Ͷ�����
	case    CHTRS_HTDL_ERR_RQUE    : // Too many items are queueing to be received
	case    CHTRS_MTX_ERR_ENQUIRE  : // ENQ����ʧ�ܣ����ܷ��ͱ���
	case    CHTRS_MTX_ERR_SEND     : // ����������Ȼ����ʧ��
		ret = false;
		break;
	default:
		break;
	}

	return ret;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��Ӳ��ģ�齨������

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Open()
{
	try {
		long errCode = 0;
		// δ����
		if(!theAPP_SESSION.IsCHConnected())
		{
			// Ӳ��ģ�鸴λ
			int port = theTVM_SETTING.GetCHComPort();
			int baud = theTVM_SETTING.GetCHBaudRate();
			CCHConnect cmd(port,baud,0);
			errCode = cmd.ExecuteCommand();
			//if (errCode == E_NO_CH_ERROR) 
			//{			
			//	errCode = CH_Init();
			//}
			if (E_NO_CH_ERROR != errCode){
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}
			CCSConnect shuter(theTVM_SETTING.GetCSHComPort(),theTVM_SETTING.GetCSHBaudRate());
			errCode = shuter.ExecuteCommand();
			if (E_NO_CH_ERROR != errCode)
			{
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}
			theAPP_SESSION.SetIsCHConnected(true);

		}
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��Ӳ��ģ��Ͽ�����

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Close()
{
	try {
		long errCode = 0;
		// ������
		if(theAPP_SESSION.IsCHConnected()){
			CCSClose csCommand;
			errCode = csCommand.ExecuteCommand();
		
			// ���Ͽ����Ӳ���
			CCHClose command;
			errCode = command.ExecuteCommand();
			if (E_NO_CH_ERROR != errCode){
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}
			theAPP_SESSION.SetIsCHConnected(false);
		}
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       Ӳ��ģ���ʼ��

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Init()
{
	try {
		long errCode = 0;

		CCHInit command(0x00);
		errCode = command.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		CCSInit shutInit;
		errCode = shutInit.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      CH��Ͷ�ҿ�

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CoinShutter_Open()
{
	try {                     
		IO_HELPER->OpenCoinShutter();
		return 0;
	}
	catch(CSysException&) {
		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ״̬

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_GetStatus(tCHADevStatus* pStatus)
{
	try {
		long errCode = 0;

		CCHGetStatus command;
		errCode = command.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		command.GetResponse(pStatus);

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�Ӳ��ģ��

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_InitModule()
{
	try {
		long errCode = 0;

		errCode = CH_Open();
		errCode = CH_Init();
		//tCHADevStatus status;
		//errCode = CH_GetStatus(&status);
		errCode = CH_SetCoinNum();

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �쳣�޸�

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Repare()
{
	try {
		long errCode = 0;

		errCode = CH_Close();
		errCode = CH_InitModule();
		tCHADevStatus status;
		errCode = CH_GetStatus(&status);
		
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Ӳ��ģ���������Ӳ����(��ʼ��ʱ������ʱ���ã�����Hopper���������޷����㣩

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_SetCoinNum()
{
	try {
		long errCode = 0;

		CTCoinCountInfo::COIN_CHANGE_BOX_INFO addBoxA = theCOIN_COUNT.GetChangeCoinboxAInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO addBoxB = theCOIN_COUNT.GetChangeCoinboxBInfo();
		CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
		CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectA = theCOIN_COUNT.GetCollectionCoinboxAInfo();	
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectB = theCOIN_COUNT.GetCollectionCoinboxBInfo();	
		
		tCoinBoxInfo coinInfo;
		coinInfo.OneY_inHopper = hopperA.ulCurCount;						// HopperA������
		coinInfo.HalfY_inHopper = hopperB.ulCurCount;						// HopperB������
		coinInfo.OneY_inRecover = collectA.ulCount_1yuan;					// ������1Ԫ����
		coinInfo.OneY_preparation = addBoxA.ulCount;
		coinInfo.HalfY_preparation = addBoxB.ulCount;
		CAfcTVMOperationParam::BILLBOX_PARAM parm;
		theAFC_TVM_OPERATION.GetBillboxParam(parm);
		coinInfo.OneY_maxthreshold_inRecover = parm.coinCollectionAlreadyFullCoinCnt;
		CCHSetCoinNum command(coinInfo);
		errCode = command.ExecuteCommand();
		
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ����Ӳ��

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StartReceive()
{
	try {
		long errCode = 0;
		// 1. ��Ͷ�ҿ�ָʾ��
		IO_HELPER->OpenCHIndicator();
		// 2. ��Ӳ��բ��
		errCode = CH_OpenShutter(); 
		if (E_NO_CH_ERROR == errCode){
			// 3. ��ʼ����
			CCHStartReceive command;
			errCode = command.ExecuteCommand();
		}

		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ����Ӳ��(�첽��

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StartReceive(CService* pCallbackSvc)
{
	try {
		long errCode = 0;
		// 1. ��Ͷ�ҿ�ָʾ��
		IO_HELPER->OpenCHIndicator();
		// 2. ��Ͷ�ҿ�
		errCode = CH_OpenShutter(); 
		if (E_NO_CH_ERROR == errCode){		
			// 3. ��ʼ����
			CCHStartReceive command;
			errCode = command.ExecuteCommand();
		}
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 2. �첽��ȡ���ս��
		CH_GetInsertInfoAsync(pCallbackSvc);

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ����Ӳ��(�첽��

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StartReceive(CService* pCallbackSvc, COIN_ACCEPTABLE& coinFaceTable)
{
	try {
		long errCode = 0;
		// �쳣��ȫ���
		if(pCallbackSvc == NULL){
			throw CCHHardwareException(CCHHardwareException::ERROR_CH_BEGIN_INSERT_COMMOND_ERR,_T(__FILE__),__LINE__);
		}

		// �趨�������
		if(!coinFaceTable.CH_COIN_ALLOW_05 && !coinFaceTable.CH_COIN_ALLOW_1){// ��Ч�Ľ����б�
			throw CCHHardwareException(CCHHardwareException::ERROR_CH_BEGIN_INSERT_COMMOND_ERR,_T(__FILE__),__LINE__);
		}
		// ���þ��ս��
		CCHSetRejectType rejectCommand(coinFaceTable);
		errCode = rejectCommand.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 1. ��Ͷ�ҿ�ָʾ��
		IO_HELPER->OpenCHIndicator();
		// 2. ��Ͷ�ҿ�
		errCode = CH_OpenShutter(); 
		if (E_NO_CH_ERROR == errCode){		
			// 3. ��ʼ����
			CCHStartReceive revCommand;
			errCode = revCommand.ExecuteCommand();
			if (E_NO_CH_ERROR != errCode){
				throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
			}

		}
		else{
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// 3. �첽��ȡ���ս��
		CH_GetInsertInfoAsync(pCallbackSvc);

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH��ȡӲ�ҽ��첽���ã�
//
//@param      CService* pCallbackSvc �ص�ָ��
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_GetInsertInfoAsync(CService* pCallbackSvc)
{
	CCHReadReceivs * pChCmd = new CCHReadReceivs();
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_READ_COIN_AMOUNT);
	CH_EnableInsert();
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH��ȡӲ�ҽ�ͬ�����ã�
//
//@param      CH_SYSTEM_STATUS_RSP& rsp  �����
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_GetInsertInfo(tCHACoinValueInfo& rsp)
{
	/*
	try {
		long errCode = 0;

		CCHGetStatus readstatus;

		errCode = readstatus.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}		
		readstatus.GetResponse(&rsp);

		//// Ӳ����Ӳ������ͬ�����������
		//SyncCoinTemporaryInfoToMasterFile(coin_info);//lichao

		m_nSortTrayRunCount = rsp.nNumOfEscrow1 + rsp.nNumOfEscrow2;

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	*/
	{
		try {
			long errCode = 1;

			{
				CCHReadCoinNum receive;
				errCode = receive.ExecuteCommand();
				if (E_NO_CH_ERROR != errCode) {
					INTERNAL_ERROR(errCode);
				}
				receive.GetResponse(&rsp);
			}
			return errCode;
		}
		catch (CSysException) {
			throw;
		}
		catch (...){
			throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������Ӳ��

@param       void 

@retval      long

@exception   CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_EndReceive()
{
	try {
		long errCode = 0;
		// 1. ��Ͷ�ҿ�ָʾ��
		IO_HELPER->CloseCHIndicator();
		// 2. �ر�Ͷ�ҿ�
		errCode = CH_CloseShutter();
		// ���ﲻ�жϹ�Ͷ�ҿڵĽ������ʹʧ�ܣ�ҲҪ��������Ӳ�ҡ�
		// 3. ��������
		CCHEndReceive command;
		errCode = command.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}
		

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH������ؽ�����Ϣ
//
//@param      ��
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_StopInsertInfo()
{
	try {
		long errCode = 0;
		CHStopReadRV stopMonitor;
		errCode = stopMonitor.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      �˻ؽ��յ�Ӳ�ң�ͬ�����ã�
//
//@param      ��
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Refund()
{
	try {
		long errCode = 0;
		CCHRefund Cancel(m_nSortTrayRunCount);
		errCode = Cancel.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		// �������
		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      �˻ؽ��յ�Ӳ�ң��첽���ã�
//
//@param     CService* pCallbackSvc �ص�ָ��
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_RefundAsync(CService* pCallbackSvc)
{
	CCHRefund * pChCmd = new CCHRefund(m_nSortTrayRunCount);
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_RETURN);
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	// �������
	theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
	m_nSortTrayRunCount = 0;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH����Ӳ�ң�ͬ�����ã�
//
//@param      (o)CH_COMMON_RSP& rsp ͨ�������
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Accept(bool bHopperAccept)
{
	try {
		long errCode = 0;
		//CCHAccept pChCmd(bHopperAccept);
		//errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH���ɣ��첽���ã�
//
//@param      (i)CService* pCallbackSvc 
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_AcceptAsync(CService* pCallbackSvc, bool bHopperAccept)
{
	CCHAccept * pChCmd = new CCHAccept();
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_ACCEPT);
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	// �������
	theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
	m_nSortTrayRunCount = 0;

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CHӲ�����㣨ͬ�����ã�
//
//@param      
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_Change(WORD num_OneY, WORD num_HalfY, tCHAChangeNum* rsp)
{
	try {
		long errCode = 0;
		CCHChange pChCmd(num_OneY, num_HalfY);
		errCode = pChCmd.ExecuteCommand();
		pChCmd.GetResponse(rsp);// ���ܳɹ���ʧ�ܣ���ȡ�������ֹ���㲻��ʧ�ܣ����ҳ���Ӳ�ҡ�
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      CH���ɣ��첽���ã�
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_ChangeAsync(CService* pCallbackSvc,WORD num_OneY, WORD num_HalfY)
{
	CCHChange * pChCmd = new CCHChange(num_OneY, num_HalfY);
	pChCmd->SetAutoDeleted(TRUE);
	pChCmd->SetService(pCallbackSvc);
	pChCmd->SetNeedCallbackService(true);
	pChCmd->SetCommandID(COIN_MODULE_COMMAND_ID_CHANGE);
	g_pCHControl->DoCommand(pChCmd,SP_EXECMODE_QUEUE);

	// �������
	theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
	m_nSortTrayRunCount = 0;
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      ��ҵ�������
//
//@param      void
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_ClearCoin(tCoinBoxInfo *tBoxInfo)
{
	try {
		long errCode = 0;
		CCHClearCoin pChCmd;
		errCode = pChCmd.ExecuteCommand();
		pChCmd.GetResponse(tBoxInfo);
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		theDeviceRegAR.AddChSortTrayRunCount(m_nSortTrayRunCount);
		m_nSortTrayRunCount = 0;
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      ��RFID
//
//@param      void
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
CString CCoinModuleHelper::CH_ReadRfidData(int boxNo)
{
	try {
		long errCode = 0;
		USES_CONVERSION;
		//CCHReadRfid pChCmd(boxNo);
		char RfidData[8] = {0};
		//errCode = pChCmd.ExecuteCommand();
		//pChCmd.GetResponse((BYTE*)RfidData);
		if (E_NO_CH_ERROR != errCode){
			return CString(_T(""));
		}
		
		return A2T(RfidData);
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}


//////////////////////////////////////////////////////////////////////////////
//**
//@brief      ��RFID
//
//@param      void
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_WriteRfidData(int boxNo, BYTE* rfidData)
{
	try {
		long errCode = 0;
		// ���ӱ�ţ�0x01 = �ӱ���2  0x02 = �ӱ���1  0x03 = ������1  0x04 = ������2
		if (boxNo!=0x03){
			return 1;
		}

		//CCHWriteRfid pChCmd(boxNo, rfidData);
		//errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			
		}
		
		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}



//////////////////////////////////////////////////////////////////////////////
//**
//@brief      ��Ͷ�ҿ�
//
//@param      void
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_OpenShutter()
{
	try {
		long errCode = 0;
		CCSOpenShutter pChCmd;
		errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      �ر�Ͷ�ҿ�
//
//@param      void
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_CloseShutter()
{
	try {
		long errCode = 0;
		CCSCloseShutter pChCmd;
		errCode = pChCmd.ExecuteCommand();
		if (E_NO_CH_ERROR != errCode){
			throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);
		}

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////////
//**
//@brief      �޸�Ͷ�ҿ�
//
//@param      void
//
//@retval     long    �������
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////////
long CCoinModuleHelper::CH_RepareShutter()
{
	try {
		long errCode = 0;
		errCode = CH_CloseShutter();
		errCode = CH_OpenShutter();

		return errCode;
	}
	catch (CSysException&) {
		throw;
	}
	catch(...){
		throw CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}