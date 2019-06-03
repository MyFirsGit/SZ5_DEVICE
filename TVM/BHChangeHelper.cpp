#include "stdafx.h"
#include "BHChangeHelper.h"

////////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      None

@retval     None

@exception  无
*/
////////////////////////////////////////////////////////////////////////////

CBHChangeHelper::CBHChangeHelper()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      None

@retval     None

@exception  无
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
@brief      初始化

@param      None

@retval     long    错误代码

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::initialize()
{
	try{
		// 1、先打开串口，自动判断是否打开（如果有异常会抛出异常）
		OpenBHChangeComPort();

		// 2、设置纸币找零模块
		long lRet = BHTRS_RES_OK;
		ParamDevInit var;
		//设置盒1纸币长度、厚度限制
		var.box[0].max_Len = Lenmax; 	//范围高187mm
		var.box[0].min_Len = Lenmin;	//范围低110mm
		var.box[0].thickness = Thick;	//0.10mm

		//设置盒2纸币长度、厚度限制
		var.box[1].max_Len = Lenmax; 	//范围高187mm
		var.box[1].min_Len = Lenmin;	//范围低110mm
		var.box[1].thickness = Thick;	//0.10mm

		BYTE FrontShutterAction = 0x00; //前挡板动作 开/关	
		BYTE RearShutterAction = 0x00; //后挡板动作 开/关

		CBHChangeDevInit BHInit(FrontShutterAction,RearShutterAction,var);
		lRet = BHInit.ExecuteCommand();
		if(lRet !=  BHTRS_RES_OK){
			INITER_ERROR(lRet);//throw CBHChangeHardwareException((CBHChangeHardwareException::ERRCODE)lRet,_T(__FILE__),__LINE__/*,_T("初始化纸币找零模块失败！")*/);
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
@brief      无

@param      无

@retval     无

@exception  无
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
@brief      出款 同步调用

@param      BYTE numFrombox1          从盒1出款张数
			BYTE numFrombox2          从盒2出款张数

@retval     long    错误代码

@exception  无
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
@brief      出款 异步调用

@param      CService* pCallbackSvc    回调指针
            BYTE numFrombox1          从盒1出款张数
			BYTE numFrombox2          从盒2出款张数

@retval     long    错误代码

@exception  无
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
@brief      获取状态

@param      CommonFeedback& ref  存储反馈数据的结构体对象的引用

@retval     long    错误代码

@exception  无
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
@brief      关闭

@param      None

@retval     long    错误代码

@exception  无
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
@brief      打开纸币找零模块串口

@param      无

@retval     long errCode;错误代码

@exception  无
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
			throw CBHChangeException(lRet,_T(__FILE__),__LINE__,_T("纸币模块：打开串口失败！"));
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
@brief      关闭纸币找零串口

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBHChangeHelper::CloseBHChangeComPort(){
	try{
		CBHChangeClose close;
		long lRet = close.ExecuteCommand();
		if(BHTRS_RES_OK != lRet)
			throw CBHChangeHardwareException(lRet,_T(__FILE__),__LINE__,_T("纸币找零模块关闭串口失败！"));
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
@brief      取纸币找零模块状态

@param      DEVICE_STATUS_INFO& device_info 纸币找零状态

@retval     long lRet；错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::GetStatus(DEVICE_STATUS_INFO& device_info){
	long lRet = 0;
	try{
		CBHChangeGetStatusInfo getCHChangeInfo;
		lRet = getCHChangeInfo.ExecuteCommand();
		if(BHTRS_RES_OK != lRet){
			throw CBHChangeHardwareException(lRet,_T(__FILE__),__LINE__,_T("取状态失败。"));
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
@brief      纸币送出并且搬送给乘客(同步)

@param      int nCountBoxA;纸币箱A出币数量
			int nCountBoxB;纸币箱B出币数量

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::SendOutBill(int nCountBoxA,int nCountBoxB){
	long lRet = BHTRS_RES_OK;
	CBHChangeSendOutBill sendBill(nCountBoxA,nCountBoxB);
	lRet = sendBill.ExecuteCommand();

	if(BHTRS_RES_OK != lRet){
		throw CBHChangeHardwareException(lRet,_T(__FILE__),__LINE__,_T("纸币送出失败！"));
	}

	theDeviceRegAR.AddBhcBoxAChangeCount(nCountBoxA);
	theDeviceRegAR.AddBhcBoxBChangeCount(nCountBoxB);

	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币送出并且搬送给乘客（异步）

@param      int nCountBoxA;纸币箱A出币数量
			int nCountBoxB;纸币箱B出币数量

@retval     无

@exception  无
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
@brief      重置纸币找零模块

@param      无

@retval     long lRet;错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::Reset(){
	long lRet = BHTRS_RES_OK;
	try{
		DEVICE_STATUS_INFO device_info;
		// 1.取状态，是否有故障
		lRet = GetStatus(device_info);
		// 2.有故障，重新初始化
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
@brief      修复纸币找零模块

@param      无

@retval     long lRet;错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeHelper::Repare(){
	long lRet = BHTRS_RES_OK;
	try{
		// 1.返初始化
		lRet = UnInitialize();
		// 2.重新初始化
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
@brief      是否为纸币找零模块硬件异常

@param      long lErrorCode(错误代码);

@retval     bool true(是);false(否);

@exception  无
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
