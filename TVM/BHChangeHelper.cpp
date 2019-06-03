#include "stdafx.h"
#include "BHChangeHelper.h"

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      None

@retval     None

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////

CBHChangeHelper::CBHChangeHelper()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      None

@retval     None

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////

CBHChangeHelper::~CBHChangeHelper()
{
}

#define INITER_ERROR(errCode)	\
{	if(IsHardWareException(errCode)){\
		throw CBHChangeHardwareException(errCode,_T(__FILE__),__LINE__);}\
		else {throw CBHChangeException(errCode,_T(__FILE__),__LINE__);}}
////////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��

@param      None

@retval     long    �������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::initialize()
{
	try{
		// 1���ȴ򿪴��ڣ��Զ��ж��Ƿ�򿪣�������쳣���׳��쳣��
		OpenBHChangeComPort();

		// 2������ֽ������ģ��
		long lRet = BHTRS_RES_OK;
		ParamDevInit var;
		//���ú�1ֽ�ҳ��ȡ��������
		var.box[0].max_Len = Lenmax; 	//��Χ��187mm
		var.box[0].min_Len = Lenmin;	//��Χ��110mm
		var.box[0].thickness = Thick;	//0.10mm

		//���ú�2ֽ�ҳ��ȡ��������
		var.box[1].max_Len = Lenmax; 	//��Χ��187mm
		var.box[1].min_Len = Lenmin;	//��Χ��110mm
		var.box[1].thickness = Thick;	//0.10mm

		BYTE FrontShutterAction = 0x00; //ǰ���嶯�� ��/��	
		BYTE RearShutterAction = 0x00; //�󵲰嶯�� ��/��

		CBHChangeDevInit BHInit(FrontShutterAction,RearShutterAction,var);
		lRet = BHInit.ExecuteCommand();
		if(lRet !=  BHTRS_RES_OK){
			INITER_ERROR(lRet);//throw CBHChangeHardwareException((CBHChangeHardwareException::ERRCODE)lRet,_T(__FILE__),__LINE__/*,_T("��ʼ��ֽ������ģ��ʧ�ܣ�")*/);
		}

		theDeviceRegAR.AddBhcResetCount();
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
	return BHTRS_RES_OK;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::UnInitialize(){
	try{
		CloseBHChangeComPort();
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
	return BHTRS_RES_OK;
}
////////////////////////////////////////////////////////////////////////////
/**
@brief      ���� ͬ������

@param      BYTE numFrombox1          �Ӻ�1��������
			BYTE numFrombox2          �Ӻ�2��������

@retval     long    �������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////


//long CBHChangeHelper::CountBill(BYTE numFrombox1,BYTE numFrombox2)
//{
//	ParamCountBill val;
//	memset(&val,0,sizeof(val));
//	val.box[0].num_out = numFrombox1;
//	val.box[0].reject = REJECT;
//	val.box[0].pick_retries = RETRY;
//
//	val.box[1].num_out = numFrombox1;
//	val.box[1].reject = REJECT;
//	val.box[1].pick_retries = RETRY;
//
//	long lret=1;
//	CBHChangeBillCount count(val);
//	lret=count.ExecuteCommand();
//
//	return lret;
//}


////////////////////////////////////////////////////////////////////////////
/**
@brief      ���� �첽����

@param      CService* pCallbackSvc    �ص�ָ��
            BYTE numFrombox1          �Ӻ�1��������
			BYTE numFrombox2          �Ӻ�2��������

@retval     long    �������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
//void CBHChangeHelper::CountBill(CService* pCallbackSvc,BYTE numFrombox1,BYTE numFrombox2)
//{
//	ParamCountBill val;
//	memset(&val,0,sizeof(val));
//	val.box[0].num_out = numFrombox1;
//	val.box[0].reject = REJECT;
//	val.box[0].pick_retries = RETRY;
//
//	val.box[1].num_out = numFrombox1;
//	val.box[1].reject = REJECT;
//	val.box[1].pick_retries = RETRY;
//	
//	CBHChangeBillCount* pBhChangdeCmd = new CBHChangeBillCount(val);
//	pBhChangdeCmd->SetAutoDeleted(TRUE);
//	pBhChangdeCmd->SetService(pCallbackSvc);
//	pBhChangdeCmd->SetNeedCallbackService(true);
//	pBhChangdeCmd->SetCommandID(COIN_MODULE_COMMAND_ID_CHANGE);
//	g_pBHControl->DoCommand(pBhChangdeCmd,SP_EXECMODE_QUEUE);
//}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ״̬

@param      CommonFeedback& ref  �洢�������ݵĽṹ����������

@retval     long    �������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
//long CBHChangeHelper::getStatus( CommonFeedback& ref)
//{
//	long lret=1;
//	CBHChangeGetStatusInfo obj;
//	lret=obj.ExecuteCommand();
//	obj.GetResponse(&ref);
//	return lret;
//}

////////////////////////////////////////////////////////////////////////////
/**
@brief      �ر�

@param      None

@retval     long    �������

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
//long CBHChangeHelper::close()
//{
//	long lret=1;
//	CBHChangeClose close;
//	lret=close.ExecuteCommand();
//	return lret;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ֽ������ģ�鴮��

@param      ��

@retval     long errCode;�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBHChangeHelper::OpenBHChangeComPort(){
	try{
		int port = theTVM_SETTING.GetBanknoteChangeComPort() - 1;//theTVM_INFO.GetBDHCommPort() - 1;
		int baud = theTVM_SETTING.GetBanknoteChangeBaudRate();   //theTVM_INFO.GetBDHBaudRate();

		long lRet = BHTRS_HTDL_ERR_PORT;

		CBHChangeConnect BHConnect(port,baud,8);
		lRet = BHConnect.ExecuteCommand();
		if(lRet != BHTRS_RES_OK){
			throw CBHChangeException(lRet,_T(__FILE__),__LINE__,_T("ֽ��ģ�飺�򿪴���ʧ�ܣ�"));
		}
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ر�ֽ�����㴮��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBHChangeHelper::CloseBHChangeComPort(){
	try{
		CBHChangeClose close;
		long lRet = close.ExecuteCommand();
		if(BHTRS_RES_OK != lRet)
			throw CBHChangeHardwareException(lRet,_T(__FILE__),__LINE__,_T("ֽ������ģ��رմ���ʧ�ܣ�"));
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡֽ������ģ��״̬

@param      DEVICE_STATUS_INFO& device_info ֽ������״̬

@retval     long lRet���������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::GetStatus(DEVICE_STATUS_INFO& device_info){
	long lRet = 0;
	try{
		CBHChangeGetStatusInfo getCHChangeInfo;
		lRet = getCHChangeInfo.ExecuteCommand();
		if(BHTRS_RES_OK != lRet){
			throw CBHChangeHardwareException(lRet,_T(__FILE__),__LINE__,_T("ȡ״̬ʧ�ܡ�"));
		}
		getCHChangeInfo.GetResponse(&device_info);
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ���ͳ����Ұ��͸��˿�(ͬ��)

@param      int nCountBoxA;ֽ����A��������
			int nCountBoxB;ֽ����B��������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::SendOutBill(int nCountBoxA,int nCountBoxB){
	long lRet = BHTRS_RES_OK;
	CBHChangeSendOutBill sendBill(nCountBoxA,nCountBoxB);
	lRet = sendBill.ExecuteCommand();

	if(BHTRS_RES_OK != lRet){
		throw CBHChangeHardwareException(lRet,_T(__FILE__),__LINE__,_T("ֽ���ͳ�ʧ�ܣ�"));
	}

	theDeviceRegAR.AddBhcBoxAChangeCount(nCountBoxA);
	theDeviceRegAR.AddBhcBoxBChangeCount(nCountBoxB);

	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ���ͳ����Ұ��͸��˿ͣ��첽��

@param      int nCountBoxA;ֽ����A��������
			int nCountBoxB;ֽ����B��������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBHChangeHelper::SendOutBill(CService* pService,int nCountBoxA,int nCountBoxB){
	CBHChangeSendOutBill* pNewSendOut = new CBHChangeSendOutBill(nCountBoxA,nCountBoxB);
	pNewSendOut->SetAutoDeleted(TRUE);
	pNewSendOut->SetService(pService);
	pNewSendOut->SetNeedCallbackService(TRUE);
	pNewSendOut->SetCommandID(BANKNOTE_CHANGE_COMMAND_ID_SEND_OUT);
	g_pBHChangeControl->DoCommand(pNewSendOut,SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ������ģ��

@param      ��

@retval     long lRet;�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::Reset(){
	long lRet = BHTRS_RES_OK;
	try{
		DEVICE_STATUS_INFO device_info;
		// 1.ȡ״̬���Ƿ��й���
		lRet = GetStatus(device_info);
		// 2.�й��ϣ����³�ʼ��
		if(BHTRS_RES_OK == lRet){
			//if(device_info.errCommCode != 0){
				Repare();
			//}
		}
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �޸�ֽ������ģ��

@param      ��

@retval     long lRet;�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::Repare(){
	long lRet = BHTRS_RES_OK;
	try{
		// 1.����ʼ��
		lRet = UnInitialize();
		// 2.���³�ʼ��
		if(BHTRS_RES_OK == lRet)
			lRet = initialize();
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ�Ϊֽ������ģ��Ӳ���쳣

@param      long lErrorCode(�������);

@retval     bool true(��);false(��);

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBHChangeHelper::IsHardWareException(long lErrorCode){

	switch(lErrorCode){
	case CBHChangeException::SET_PARAM_ERR:
	case CBHChangeException::FUNC_ERR_RETURN:
	case CBHChangeException::SEQUENCE_ERR:
	case CBHChangeException::FUNC_PARAM_ERR:
	case CBHChangeException::LOGIC_ERR:
	case CBHChangeException::OTHER_ERR:
	case CBHChangeException::ERROR_BH_OTHER:			
	case CBHChangeException::ERROR_BH_OPEN_FAILED:	
	case CBHChangeException::ERROR_BH_ALREADY_OPENED:	
	case CBHChangeException::ERROR_BH_CLOSE_FAILED:	
	case CBHChangeException::ERROR_BH_ALREADY_CLOSED:	
	case CBHChangeException::ERROR_BH_NOT_OPEN:		
	case CBHChangeException::ERROR_BH_CMD:			
	case CBHChangeException::ERROR_BH_SEND:			
	case CBHChangeException::ERROR_BH_RECEIVE:		
	case CBHChangeException::ERROR_BH_DESERIALIZE:	
	case CBHChangeException::ERROR_BH_RECEIVE_TIMEOUT:
	case CBHChangeException::ERROR_BH_SEQUNCE_ERR:
		return false;
	default:
		return true;
	}
}
