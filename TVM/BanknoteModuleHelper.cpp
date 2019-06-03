#include "StdAfx.h"
#include "BanknoteModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 异常抛出宏（软件异常/硬件异常）
#define INTERNAL_ERROR(errCode)											\
{																		\
	if (IsHWException(errCode))											\
		throw CBHHardwareException(errCode, _T(__FILE__), __LINE__);	\
	else																\
		throw CBHException(errCode,_T(__FILE__),__LINE__);				\
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		(i)CService& service

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteModuleHelper::CBanknoteModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteModuleHelper::~CBanknoteModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		停止动作

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Abort()
{
	// 发送停止命令
	CBHAbortAction pBhAbortCmd;
	long errCode = pBhAbortCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		初始化

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Init()
{
	long errCode = 0;
	
	// 检查并打开串口
	errCode = CheckAndOpenBHComport();

	
	/*errCode = getState.ExecuteCommand();
	if(errCode != E_NO_ERROR){
		INTERNAL_ERROR(errCode);
	}
	getState.GetResponse(&pState);*/

	//// 设置故障清除模式：用户决定
	//CBHSetErrorClean cleanMode(false);
	//errCode = cleanMode.ExecuteCommand();
	//
	//// 设置工作模式： 具体模式可设计从配置文件读取，暂定15秒
	//CBHSetWorkMode workMode(DEADLINE_NORMAL);
	//errCode = workMode.ExecuteCommand();
	
	// 复位，故障清除模式生效
	errCode = Reset();
	

	// 取状态
	CBHGetSts getState;
	BRH_STS	pState;
	// 检查状态设置是否生效
	errCode = getState.ExecuteCommand();
	if(errCode != E_NO_ERROR){
		INTERNAL_ERROR(errCode);
	}
	getState.GetResponse(&pState);
	if (pState.cleanErorMode != CUSTOM_CLEAN_ERROR){
		// 设置故障清除模式：用户决定
		CBHSetErrorClean cleanMode(false);
		errCode = cleanMode.ExecuteCommand();

		// 设置工作模式： 具体模式可设计从配置文件读取，暂定15秒
		CBHSetWorkMode workMode(DEADLINE_NORMAL);
		errCode = workMode.ExecuteCommand();

		errCode = Reset();
		// 设置失败
		return errCode;
	}
		
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		复位

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Reset()
{
	long errCode = 0;

	// 发送复位命令
	CBHReset rstCmdl;
	errCode = rstCmdl.ExecuteCommand();

	theDeviceRegAR.AddBhResetCount();

	// 复位时出错，需要以自动恢复模式修复
	if(errCode == CBHException::ERROR_RESET_NEED_REC_AUT){
		CBHClearTrans cleanCmd;
		errCode = cleanCmd.ExecuteCommand();
	}
	else{
		// 读取状态
		CBHGetSts stsCmd;
		BRH_STS	  state;
		stsCmd.ExecuteCommand();
		stsCmd.GetResponse(&state);

		// 取故障代码
		if (state.errorCode == 0x30){
			// 如果无故障,不需要复位
			return 0;
		}else {
			// 存在故障
			//rstCmdl.ExecuteCommand();
			CBHClearTrans cleanCmd;
			errCode = cleanCmd.ExecuteCommand();
		}
		// 再次取状态
		stsCmd.ExecuteCommand();
		state = BRH_STS();
		stsCmd.GetResponse(&state);
		// 检查故障代码
		if (state.errorCode != 0x30){
			// 无故障，复位成功
		}else{
			// 复位失败
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		停止接收识别纸币

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::StopValidation()
{
	// 停止当前动作:需要手动停止，判断回调函数，会调用停止识别。
	//this->Abort();
	//BH_DisableInsert();
	SYNC(BHMGR,_T("BHMGR"));
	// 发送停止命令
	CBHStopValidation pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		开始接收识别纸币（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteModuleHelper::OpenAndValidateAsync(CService* pSvc,BRH_FACE_ACEPTED_TABLE& bhTable)
{
	CBHOpenAndValidate* pBhCmd = new CBHOpenAndValidate(bhTable);
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_INSERT);
	BH_EnableInsert();
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		开始接收识别纸币

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::OpenAndValidate(BH_STATUS& status)
{
	// 默认设置全部接收
	BRH_FACE_ACEPTED_TABLE bhTable;
	bhTable.bIsBanknote1Yuan	= true;
	bhTable.bIsBanknote5Yuan	= true;
	bhTable.bIsBanknote10Yuan	= true;
	bhTable.bIsBanknote20Yuan	= true;
	bhTable.bIsBanknote50Yuan	= true;
	bhTable.bIsBanknote100Yuan	= true;
	// 发送命令
	CBHOpenAndValidate pBhCmd(bhTable);
	BH_EnableInsert();
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	pBhCmd.GetResponse(&status);
	return errCode;	
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		MEI 开始接受识别纸币

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::OpenAndValidate(BH_CMD_FEEDBACK& feedback)
{
	// 默认设置全部接收
	BRH_FACE_ACEPTED_TABLE bhTable;
	bhTable.bIsBanknote1Yuan	= true;
	bhTable.bIsBanknote5Yuan	= true;
	bhTable.bIsBanknote10Yuan	= true;
	bhTable.bIsBanknote20Yuan	= true;
	bhTable.bIsBanknote50Yuan	= true;
	bhTable.bIsBanknote100Yuan	= true;
	// 发送命令
	CBHOpenAndValidate pBhCmd(bhTable);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	pBhCmd.GetResponse(&feedback);
	return errCode;	
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压暂存（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteModuleHelper::HoldAsync(CService* pSvc)
{
	CBHHold* pBhCmd = new CBHHold();
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_ACCEPT);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);

	theDeviceRegAR.AddBhMotorPressCount();
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压暂存

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Hold()
{
	// 发送命令
	CBHHold pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	theDeviceRegAR.AddBhMotorPressCount();
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压钞（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteModuleHelper::CollectAsync(CService* pSvc)
{
	CBHCollect* pBhCmd = new CBHCollect();
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_COLLECT);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压钞

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Collect()
{
	// 发送命令
		
	CBHCollect pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压钞(MEI)

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Collect(BH_CMD_FEEDBACK& feedback)
{	
	CBHCollect pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	errCode = pBhCmd.GetResponse(&feedback);
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回缓冲区的纸币（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteModuleHelper::ReturnCacheAsync(CService* pSvc)
{
	CBHReturnOne* pBhCmd = new CBHReturnOne(false);
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_RETURN_BUFF);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回缓冲区的纸币

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::ReturnCache(bool isCheckShutter)
{
	// 发送命令
	CBHReturnOne pBhCmd(isCheckShutter);
	/*CBHRef pBhCmd;*/
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回全部纸币（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteModuleHelper::ReturnAllAsync(CService* pSvc)
{
	// 中断轮询操作
	//this->Abort();

	// 发送命令
	//CBHReturnAll* pBhCmd = new CBHReturnAll(false);
	CBHRef* pBhCmd =  new CBHRef;
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_RETURN_ALL);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回全部纸币

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::ReturnAll(bool isCheckShutter)
{
	// 发送命令
	CBHReturnAll pBhCmd(isCheckShutter);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		MEI退纸币

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::ReturnAll(BH_CMD_FEEDBACK& feedback)
{
	// 发送命令
	CBHRef refCmd;
	long errCode = refCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
//	errCode = refCmd.GetResponse(&feedback);
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		清除钞票

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Clean(BH_CLEAN_DEST dest)
{
	// 开始清币
	CBHClearTrans pBhCmd(dest);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取状态

 @param		无

 @retval	long  错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::GetStatus(BH_STATUS& status)
{
	
	long errCode = 0;

	// 读取
	CBHGetSts stsCmd;
	errCode = stsCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	stsCmd.GetResponse(&status.bh_sts);

	status.bh_faceValue = VALUE_UNKNOWN; // MEI中只有工作才能取得纸币的面额
	// 读取面值
	/*if (status.bh_sts.IsValidFaceValue())
	{
	CBHGetFaceValue faceCmd(status.bh_sts.valueNo);
	errCode = faceCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
	INTERNAL_ERROR(errCode);
	faceCmd.GetResponse(&status.bh_faceValue);
	}*/
	return errCode;
}



//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取钱箱ID(MEI)

 @param		(o)BH_BOX_ID& box_id  钱箱ID

 @retval	long  错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::GetBoxID(BRH_BOX_ID& box_id)
{
	// 发送获取版本命令
	CBHGetBoxID pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	pBhCmd.GetResponse(&box_id);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      异常修复

@param      none

@retval     long 返回的异常代码

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Repare(bool bMenualRepare)
{
	long errCode = 0;
	errCode = Abort();
	//errCode = Init();
	//errCode = Reset();
	
	// 尝试关闭串口后再打开
	errCode = CheckAndOpenBHComport();

	// 读取状态，看是否正常
	BRH_STS	pState;

	CBHGetSts bhStatus;
	errCode = bhStatus.ExecuteCommand();

	// 命令执行失败
	if(errCode != E_NO_ERROR){
		if(errCode == CBHException::ERROR_BH_RECEIVE_TIMEOUT)
			return Reset();
		INTERNAL_ERROR(errCode);
	}
	// 取状态成功
	else{
		bhStatus.GetResponse(&pState);
		// 取状态返回结果有错误
		if(pState.errorCode != 0x30){
			// 检查是否是退币口探测器不能提升，这时只能降级
			if(memcmp(pState.warning,"D4",2) == 0 && !bMenualRepare){
				throw CBHHardwareException(pState.TranslateWarningCode(),_T(__FILE__),__LINE__);
			}
			
			// 快门打开，则以退出方式还给乘客
			if(memcmp(pState.warning,"D3",2) == 0 || memcmp(pState.warning,"D1",2) == 0 || memcmp(pState.warning,"D2",2) == 0 || memcmp(pState.warning,"D4",2) == 0){
				// 尝试以退出方式恢复模块(尝试三次)
				CBHRecRef bhRecRef;
				for(int i = 0; i < 3; i++){
					errCode = bhRecRef.ExecuteCommand();
					if(errCode == 0){
						return errCode;//break;
					}
					else if(errCode == BH_WORK_INVALID){
						break;
					}
				}
			}
			else{
				// 尝试以压入方式恢复模块(尝试三次)
				CBHRecEnc bhRecEnc;
				for(int i = 0; i < 3; i++){
					errCode = bhRecEnc.ExecuteCommand();
					if(errCode == 0){
						return errCode;//break;
					}
					else if(errCode == BH_WORK_INVALID){
						break;
					}
				}
			}
			
			// 如果恢复时命令异常，此时用重置来解除异常
			if(errCode == BH_WORK_INVALID){
				return Reset();
			}
			// 需要抛异常
			errCode = pState.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

			CString strErrorCode;
			strErrorCode.Format(_T("纸币模块错误代码：%d"),errCode);
			throw CBHHardwareException(errCode,_T(__FILE__),__LINE__,strErrorCode);
		}
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		是否为硬件异常

 @param		(i)long errCode		错误代码

 @retval	bool  true:硬件异常;	false:软件异常

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
bool CBanknoteModuleHelper::IsHWException(long errCode)
{
	bool ret = false;

	switch(errCode)
	{
	case CBHException::SET_PARAM_ERR:			// 设定值：参数异常
	case CBHException::FUNC_PARAM_ERR:			// 函数参数异常
	case CBHException::SEQUENCE_ERR:			// 函数调用顺序异常
	case CBHException::FUNC_ERR_RETURN:			// 函数返回异常
	case CBHException::LOGIC_ERR:				// 逻辑异常
	case CBHException::OTHER_ERR:				// 异常发生
	case CBHException::ERROR_BH_OTHER:			// 其它异常
		ret = false;
		break;

	case CBHException::ERROR_BH_OPEN_FAILED:	// 打开时	串口打开失败
	case CBHException::ERROR_BH_ALREADY_OPENED:	// 打开时	串口重复打开
	case CBHException::ERROR_BH_CLOSE_FAILED:	// 关闭时	串口关闭失败
	case CBHException::ERROR_BH_ALREADY_CLOSED:	// 关闭时	串口重复关闭
	case CBHException::ERROR_BH_NOT_OPEN:		// 通信时	串口未打开
	case CBHException::ERROR_BH_CMD:			// 通信时	命令异常
	case CBHException::ERROR_BH_SEND:			// 通信时	送信异常
	case CBHException::ERROR_BH_RECEIVE:		// 通信时	接收异常
	case CBHException::ERROR_BH_DESERIALIZE:	// 通信时	解析回复信息异常

	case ERR_BH_IN_TRANS:			// 传送系统中有纸币
	case ERR_BH_RETURN_ALL:			// 整叠退钞失败
	case ERR_BH_RETURN_ONE:			// 单张退钞失败
	case ERR_BH_INSERT_JAM:			// 投入纸币时卡钞
	case ERR_BH_JAM:				// 暂存钞票到卷筒时失败，或其它任意卡钞
	case ERR_BH_COLLECT:			// 压钱箱失败
	case ERR_BH_RETURN_SHUTTER:		// 退钞口上有纸币				（警告）
	case ERR_BH_INSERT_SHUTTER:		// 进钞口上有纸币，无法退出
	case ERR_BH_ALIGN:				// 无法对齐卷筒
	case ERR_BH_NO_RECOGNIZER:		// 不能连接纸币识别器
	case ERR_BH_RECOGNIZING:		// 纸币识别器正在启动			（警告）
	case ERR_BH_FRAME:				// 与钱箱架通信超时
	case ERR_BH_BOX:				// 钱箱异常（有纸币从钱箱内退出）
	case ERR_BH_NO_BOX:				// 未装入钱箱或钱箱未锁好
	case ERR_BH_FULL:				// 钱箱已满
	case ERR_BH_NEARLY_FULL:		// 钱箱将满						（警告）
	case ERR_BH_HARDWARE:			// 泛指硬件错误
		ret = true;
		break;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		纸币箱是否已装入且锁好

 @param		

 @retval	bool  true:已装入且锁好;	false:未装入或未锁好

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
bool CBanknoteModuleHelper::IsBankNoteBoxAttached()
{
	BH_STATUS status;

	try {
		long er_code = GetStatus(status);
	}
	catch (...){
		return false;
	}

	/*vector<BH_ERROR>::iterator iter = find(status.bh_sts.error.begin(),status.bh_sts.error.end(),ERR_BH_NO_BOX);
	if(iter == status.bh_sts.error.end())*/
	// 警告代码为C1为钱箱不在位
	if(memcmp(status.bh_sts.warning,"C1",2) == 0)
	{
		return FALSE;
	}
	else
		return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      当纸币接收模块产生故障，以退出方式修复此故障(尝试三次)

@param      无

@retval     long 0 成功；非0：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::TryRecoveryBanknoteModule(){
	
	long lRet = 0;

	try{
		// 取纸币接收模块状态
		CBHGetSts bhGetStatus;

		lRet = bhGetStatus.ExecuteCommand();

		if(0 != lRet){
			INTERNAL_ERROR(lRet);
		}
		else{
			// 取结果
			BRH_STS bhStatus;
			bhGetStatus.GetResponse(&bhStatus);

			if(bhStatus.errorCode != 0x30){
				// 尝试以退出方式恢复模块(尝试三次)
				CBHRecRef bhRecRef;
				for(int i = 0; i < 3; i++){
					lRet = bhRecRef.ExecuteCommand();
					if(lRet == 0){
						return lRet;//break;
					}
					else if(lRet == BH_WORK_INVALID){
						break;
					}
				}

				// 需要抛异常
				lRet = bhStatus.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

				CString strErrorCode;
				strErrorCode.Format(_T("纸币模块错误代码：%d"),lRet);
				throw CBHHardwareException(lRet,_T(__FILE__),__LINE__,strErrorCode);
			}
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      打开纸币接收模块，已打开则关闭后再打开。

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::CheckAndOpenBHComport(){
	long errCode = 0;

	int port = theSETTING.GetBHComPort() - 1;//theTVM_INFO.GetBHCommPort() - 1;
	int baud = theSETTING.GetBHBaudRate();   //theTVM_INFO.GetBHBaudRate();

	// 打开串口
	CBHConnect pBhConnectCmd(port,baud,0);
	errCode = pBhConnectCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}