#include "StdAfx.h"
#include "BRHModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		(i)CService& service

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBRHModuleHelper::CBRHModuleHelper()
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
CBRHModuleHelper::~CBRHModuleHelper()
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
long CBRHModuleHelper::Abort()
{
	//// 发送停止命令
	//CBHAbortAction pBhAbortCmd;
	//long errCode = pBhAbortCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	//return errCode;
	BRH_SetAcceptFlag(false);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		初始化

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Init(bool bRepareCount)
{
	long errCode = 0;
	
	// 1、检查并打开串口
	errCode = CheckAndOpenBHComport();
	Reset();
	
	// 2、设置BRH的时间
	time_t t = time(NULL);
	CBRHSetDateTime setTimeCmd(t);
	setTimeCmd.ExecuteCommand();

	// 3、获取BRH状态
	errCode = 0;
	tBncDevReturn Status;
	memset(&Status, 0, sizeof(Status));
	GetStatus(Status);
	// 获取状态成功，检查是否需要复位重置;不成功的话，GetStatus内部会抛异常，不会走到这里。
	if ((Status.iErrorCode != XFS_S_CDR_DS_ON_LINE)
		&& (Status.iErrorCode != XFS_S_CDR_DS_USER_ERROR)
		&& (Status.iErrorCode != XFS_S_CDR_DS_HARDWARE_ERROR)) {
			Reset();
			
	}
	else if((Status.iErrorCode == XFS_S_CDR_DS_USER_ERROR) || (Status.iErrorCode == XFS_S_CDR_DS_HARDWARE_ERROR)){
		throw CBRHHardwareException(XFS_S_CDR_DS_HARDWARE_ERROR, _T(__FILE__), __LINE__);
	}
	else{
		// 状态ok
	}
	CheckModuleStatus();


	// 4、设置性能
	// 4.1取当前性能配置
	//BRH_RSP_GET_CAPABLILITIES capablilities;
	//CBRHGetBillBoxInfo getCapablilities;
	//errCode = getCapablilities.ExecuteCommand();

	//if(errCode!=BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);
	//}
	//BRH_RSP_GET_STATUS CapStatus;
	//getCapablilities.GetBRHResponse(&capablilities,&CapStatus);
	//// 设置找零自动送钞
	////capablilities.autoPresent  = true;
	//// 设置找零箱自动补钞
	////capablilities.selfTestMode = T_SelfTestMode::STM_AUTO;

	//// 4.2设置性能
	//CBRHSetBillBoxInfo setCapabilities(0,&capablilities);
	//errCode = setCapabilities.ExecuteCommand();
	//if(errCode!=BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);
	//}

	// 5.1取箱子的配置信息（此步骤必须初始调用，后面纸币接收时才能正确返回接收纸币的箱子信息）
	// 使用do-while，防止硬件复位时处于忙碌状态，接口报错。
	do{
		map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> cashUnit;
		try{
			cashUnit = BANKNOTE_HELPER->GetEmptyFull();
			map<BNR_MODULE_ID,T_XfsPhysicalCashUnit>::iterator it = cashUnit.find(BNR_RECYCLER_BOX_A);
			if(it != cashUnit.end()){
				theAPP_SESSION.SetBNRRecyclerNearEmpty(1, it->second.threshold.low);
			}
			it = cashUnit.find(BNR_RECYCLER_BOX_B);
			if(it != cashUnit.end()){
				theAPP_SESSION.SetBNRRecyclerNearEmpty(2, it->second.threshold.low);
			}
			it = cashUnit.find(BNR_RECYCLER_BOX_C);
			if(it != cashUnit.end()){
				theAPP_SESSION.SetBNRRecyclerNearEmpty(3, it->second.threshold.low);
			}
			it = cashUnit.find(BNR_RECYCLER_BOX_D);
			if(it != cashUnit.end()){
				theAPP_SESSION.SetBNRRecyclerNearEmpty(4, it->second.threshold.low);
			}

			// 为了防止循环鼓计数和实际不一致，初始化调整一下。
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_A = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_B = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();	
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_C = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();	
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_D = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO& cashBoxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& changeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

			if(bRepareCount){
				// 检查软计数和硬件箱子中实际数量
				if (cashUnit[BNR_RECYCLER_BOX_A].count != recycle_A.ulCurCount){
					CString strLog;
					strLog.Format(_T("发现纸币循环鼓A软计数与实际不一致！软计数中%d张，实际%d张;补充箱当前%d张"), recycle_A.ulCurCount, cashUnit[BNR_RECYCLER_BOX_A].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_A].count - recycle_A.ulCurCount;
					if(adjustCount>0){//实际比软计数大，调整计数
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_A.ulCurCount = cashUnit[BNR_RECYCLER_BOX_A].count;
				}
				if (cashUnit[BNR_RECYCLER_BOX_B].count != recycle_B.ulCurCount){
					CString strLog;
					strLog.Format(_T("发现纸币循环鼓B软计数与实际不一致！软计数中%d张，实际%d张;补充箱当前%d张"), recycle_B.ulCurCount, cashUnit[BNR_RECYCLER_BOX_B].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_B].count - recycle_B.ulCurCount;
					if(adjustCount>0){//实际比软计数大，调整计数
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_B.ulCurCount = cashUnit[BNR_RECYCLER_BOX_B].count;
				}
				if (cashUnit[BNR_RECYCLER_BOX_C].count != recycle_C.ulCurCount){
					CString strLog;
					strLog.Format(_T("发现纸币循环鼓C软计数与实际不一致！软计数中%d张，实际%d张;补充箱当前%d张"), recycle_C.ulCurCount, cashUnit[BNR_RECYCLER_BOX_C].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_C].count - recycle_C.ulCurCount;
					if(adjustCount>0){//实际比软计数大，调整计数
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_C.ulCurCount = cashUnit[BNR_RECYCLER_BOX_C].count;
				}
				if (cashUnit[BNR_RECYCLER_BOX_D].count != recycle_D.ulCurCount){
					CString strLog;
					strLog.Format(_T("发现纸币循环鼓D软计数与实际不一致！软计数中%d张，实际%d张;补充箱当前%d张"), recycle_D.ulCurCount, cashUnit[BNR_RECYCLER_BOX_D].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_D].count - recycle_D.ulCurCount;
					if(adjustCount>0){//实际比软计数大，调整计数
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_D.ulCurCount = cashUnit[BNR_RECYCLER_BOX_D].count;
				}
				//// 经过测试验证发现，硬件返回的补充箱计数不正确，因此下面注释不使用。
				//if (cashUnit[BNR_MODULE_CHANGE_BOX].count != changeBoxInfo.ulCurCount){
				//	CString strLog;
				//	strLog.Format(_T("发现纸币补充箱软计数与实际不一致！软计数中%d张，实际%d张"), changeBoxInfo.ulCurCount, cashUnit[BNR_MODULE_CHANGE_BOX].count);
				//	theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
				//	changeBoxInfo.ulCurCount = cashUnit[BNR_MODULE_CHANGE_BOX].count;
				//	theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBoxInfo);
				//}
				// 经过测试验证发现，硬件返回的回收箱计数不正确，因此下面注释不使用。
				//if (cashUnit[BNR_MODULE_CASH_BOX].count != cashBoxInfo.BankntoeTotalCount()){
				//	CString strLog;
				//	strLog.Format(_T("发现纸币回收箱软计数与实际不一致！软计数中%d张，实际%d张"), cashBoxInfo.BankntoeTotalCount(), cashUnit[BNR_MODULE_CASH_BOX].count);
				//	theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
				//}
				if (changeBoxInfo.ulCurCount<0){
					changeBoxInfo.ulCurCount = 0;
				}
			}
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(recycle_A);
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(recycle_B);
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(recycle_C);
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(recycle_D);
			theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBoxInfo);
			break;
		}
		catch(CSysException&e)
		{
			if (e.GetDetailID()!=CBRHHardwareException::ERR_BUSY){
				theEXCEPTION_MGR.ProcessException(e);
				return FALSE;
			}
			else{
				Sleep(5000);//等纸币模块退出忙碌状态
			}
		}
	}while(1);


	// 5、设置箱子的配置信息（只能配置找零箱）
	//CBRHSetCashType setCashType(BNR_MODULE_ID::BNR_MODULE_CHANGE_BOX,theSETTING.GetBNRChangeBoxAmount() * 100);
	//errCode = setCashType.ExecuteCommand();
	/*if(errCode!=BRH_NO_ERROR){
	ProcessErrorCode(errCode);
	}*/


	// 5.1取箱子的配置信息
	/*BRH_RSP_QUERY_CASHUNIT cashUnit;
	CBRHQueryCashUnit queryCashUnit;
	errCode = queryCashUnit.ExecuteCommand();
	if(errCode != BRH_NO_ERROR){
	ProcessErrorCode(errCode);
	}

	queryCashUnit.GetBRHResponse(&cashUnit);*/


	//// 更新配置信息
	////cashUnit.physicalCashUnitList
	//for(int i = 0 ;i < cashUnit.logicalCashUnitList.size;i++){
	//	if(strcmp(cashUnit.logicalCashUnitList.items[i].physicalCashUnit->name,BNR_MODULE_LOADER_BOX_NAME) == 0){
	//		cashUnit.logicalCashUnitList.items[i].cashType.value = theSETTING.GetBNRChangeBoxAmount() * 100;
	//	}
	//	else if(strcmp(cashUnit.logicalCashUnitList.items[i].physicalCashUnit->name,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0){
	//		cashUnit.logicalCashUnitList.items[i].cashType.value = theSETTING.GetBNRRecycleBoxAAmount() * 100;
	//	}
	//	else if(strcmp(cashUnit.logicalCashUnitList.items[i].physicalCashUnit->name,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0){
	//		cashUnit.logicalCashUnitList.items[i].cashType.value = theSETTING.GetBNRRecycleBoxBAmount() * 100;
	//	}
	//	else if(strcmp(cashUnit.logicalCashUnitList.items[i].physicalCashUnit->name,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0){
	//		cashUnit.logicalCashUnitList.items[i].cashType.value = theSETTING.GetBNRRecycleBoxCAmount() * 100;
	//	}
	//	else if(strcmp(cashUnit.logicalCashUnitList.items[i].physicalCashUnit->name,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0){
	//		cashUnit.logicalCashUnitList.items[i].cashType.value = theSETTING.GetBNRRecycleBoxDAmount() * 100;
	//	}
	//}

	//// 5.2设置箱子的配置信息
	//CBRHUpdateCashUnit updateCashUnit(&cashUnit);
	//errCode = updateCashUnit.ExecuteCommand();

	//if(errCode != BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);
	//}


	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		复位

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Reset()
{
	long errCode = 0;

	//// 发送复位命令
	//CBHReset rstCmdl;
	//errCode = rstCmdl.ExecuteCommand();

	//theDeviceRegAR.AddBhResetCount();

	//// 复位时出错，需要以自动恢复模式修复
	//if(errCode == CBHException::ERROR_RESET_NEED_REC_AUT){
	//	CBHClearTrans cleanCmd;
	//	errCode = cleanCmd.ExecuteCommand();
	//}
	//else{
	//	// 读取状态
	//	CBHGetSts stsCmd;
	//	BH_STS	  state;
	//	stsCmd.ExecuteCommand();
	//	stsCmd.GetResponse(&state);

	//	// 取故障代码
	//	if (state.errorCode == 0x30){
	//		// 如果无故障,不需要复位
	//		return 0;
	//	}else {
	//		// 存在故障
	//		//rstCmdl.ExecuteCommand();
	//		CBHClearTrans cleanCmd;
	//		errCode = cleanCmd.ExecuteCommand();
	//	}
	//	// 再次取状态
	//	stsCmd.ExecuteCommand();
	//	state = BH_STS();
	//	stsCmd.GetResponse(&state);
	//	// 检查故障代码
	//	if (state.errorCode != 0x30){
	//		// 无故障，复位成功
	//	}else{
	//		// 复位失败
	//	}
	//}
	tBncInitNumInfo pNumInfo;
	tBncDevReturn p_psStatus;
	int nTryTimes = 3;
	CBRHReset resetCmd(0,nTryTimes,&pNumInfo,&p_psStatus);
	errCode = resetCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}

	//// 检查模块状态
	//BRH_RSP_GET_SYSTEM_STATUS systemStatus;
	//CBRHGetSystemStatus brhCmd;
	//errCode = brhCmd.ExecuteCommand();
	//if(errCode!=BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);	
	//}
	//brhCmd.GetBRHResponse(&systemStatus);

	//for(int sz=0; sz<systemStatus.size; sz++){
	//	// print log
	//	if(systemStatus.moduleStatusOverviews[sz].cashbox.id == CASHBOX_CLASS){
	//		if(systemStatus.moduleStatusOverviews[sz].cashbox.errorCode == CEC_MODULE_MISSING){
	//			throw CBRHHardwareException(CBRHHardwareException::ERR_HARDWAREERROR, _T(__FILE__), __LINE__);
	//			break;
	//		}
	//	}else if(systemStatus.moduleStatusOverviews[sz].loader.id == (LOADER_CLASS+1)){
	//		if(systemStatus.moduleStatusOverviews[sz].loader.errorCode == MODEC_MISSING_MODULE){
	//			throw CBRHHardwareException(CBRHHardwareException::ERR_HARDWAREERROR, _T(__FILE__), __LINE__);
	//			break;
	//		}
	//	}else if((systemStatus.moduleStatusOverviews[sz].recycler.id == (RECYCLER_CLASS+3)) ||
	//		(systemStatus.moduleStatusOverviews[sz].recycler.id == (RECYCLER_CLASS+4)) ||
	//		(systemStatus.moduleStatusOverviews[sz].recycler.id == (RECYCLER_CLASS+5)) ||
	//		(systemStatus.moduleStatusOverviews[sz].recycler.id == (RECYCLER_CLASS+6))){
	//			if(systemStatus.moduleStatusOverviews[sz].recycler.errorCode == MODEC_MISSING_MODULE){
	//				throw CBRHHardwareException(CBRHHardwareException::ERR_HARDWAREERROR, _T(__FILE__), __LINE__);
	//				break;
	//			}      
	//	}  
	//}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		检查模块状态

 @param		无

 @retval	long	错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::CheckModuleStatus()
{
	long errCode = 0;

	BRH_RSP_GET_STATUS lsystemStatus;
	BRH_RSP_GET_SYSTEM_STATUS lbillboxStatus;
	tBncBillBoxInfo queryCashUnit;
	errCode = GetSystemStatus(queryCashUnit);

	//if(lsystemStatus.iStdErrorCode ==SOS_NOT_OPERATIONAL)
	//{
		// 检查SystemErrorCode.
		if (errCode==SEC_OPENED)
		{
//			throw 纸币模块的一个盖板被打开;
		}
		else if (errCode==SEC_UNLOCKED)
		{
//			throw 现金模块的锁被打开;
		}
		else{ // it means ok(SEC_NO_ERROR).
			// do nothing.
		}
		// 检查SystemCashTypeStatus
		if (errCode!=SCTS_OK)
		{
			for(int sz=0; sz<MAX_BOXNUM; sz++){
				if( queryCashUnit.pCashBoxValueInfo[sz].uiBoxType >0  &&
					queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (LOADER_CLASS+1)){
					if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus != CTS_OK){						
						// CTS_UNKNOWN,         /**< Can not determine the CashType status of the module. */
						//	 CTS_OK,              /**< The CashType status is OK - No errors. */
						//	 CTS_WRONG_SETTING,   /**< The CashType in the module does not correspond to its LogicalCashUnit's CashType. */
						//	 CTS_WRONG_BILLS,     /**< The bills in the module are different than the CashType defines in the module. */
						//	 CTS_BILLS_NOT_VALID  /**< The bills in the module correspond to its CashType, but are not recognized by the system. */
						// throw 补币箱中现金类型错误
					}
				}else if(
					queryCashUnit.pCashBoxValueInfo[sz].uiBoxType ==0 &&
					(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo  == (RECYCLER_CLASS+3)) ||
					(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo  == (RECYCLER_CLASS+4))){
						if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus != CTS_OK){
							// throw 循环找零箱1中现金类型错误
						}

				}else if((queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+5)) ||
					(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+6))){
						if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus != CTS_OK){
							// throw 循环找零箱2中现金类型错误
						}      
				}  
			}				 
		}
		// 检查SystemBillStorageStatus
		if (errCode!=SBSS_OK)
		{
			if (errCode==SBSS_NO_CHANGE){
				// throw 纸币模块无可用找零
			}
			else if (errCode==SBSS_ERROR){
				// throw 回收箱满
			}
			else if (errCode==SBSS_WARNING){
				// throw 补币箱空或含错误面额的纸币
			}
		}
		// 检查BillTransportStatus
		if (errCode!=BTS_OK)
		{
			// throw 纸币模块卡币
		}
		// 检查箱子的安装状态
		for(int sz=0; sz<MAX_BOXNUM; sz++){
			// print log
			if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == CASHBOX_CLASS){
				if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus == CEC_MODULE_MISSING){
					throw CBRHHardwareException(CBRHHardwareException::ERR_HARDWAREERROR, _T(__FILE__), __LINE__);
					break;
				}
			}else if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (LOADER_CLASS+1)){
				if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus == MODEC_MISSING_MODULE){
					throw CBRHHardwareException(CBRHHardwareException::ERR_HARDWAREERROR, _T(__FILE__), __LINE__);
					break;
				}
			}else if((queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+3)) ||
				(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+4)) ||
				(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+5)) ||
				(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+6))){
					if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus == MODEC_MISSING_MODULE){
						throw CBRHHardwareException(CBRHHardwareException::ERR_HARDWAREERROR, _T(__FILE__), __LINE__);
						break;
					}      
			}  
		}
	//}


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
long CBRHModuleHelper::StopValidation()
{
	//// 停止当前动作:需要手动停止，判断回调函数，会调用停止识别。
	////this->Abort();
	////BH_DisableInsert();
	//SYNC(BHMGR,_T("BHMGR"));
	//// 发送停止命令
	//CBHStopValidation pBhCmd;
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	//return errCode;

	BRH_SetStopAcceptFlag(true);//加入这个标识，防止停止接收的命令在开始接收之前被处理。
	CBRHCancel brhCmd;
	long errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);	
	}
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
void CBRHModuleHelper::OpenAndValidateAsync(CService* pSvc,tBncSetInhibitList& bhTable)
{
	BRH_SetStopAcceptFlag(false);//加入这个标识，防止停止接收的命令在开始接收之前被处理。
	CBRHOpenAndValidate* pBhCmd = new CBRHOpenAndValidate(bhTable);
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_INSERT);
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
long CBRHModuleHelper::OpenAndValidate(BRH_STATUS& status)
{
	BRH_SetStopAcceptFlag(false);
	// 默认设置全部接收
	tBncSetInhibitList bhAcceptTable;
	bhAcceptTable.iNumOfItems = 6;
	bhAcceptTable.psBncCashInfo = new tBncSetInhibitItem[bhAcceptTable.iNumOfItems];
	bhAcceptTable.psBncCashInfo[0].iValue = 100;
	bhAcceptTable.psBncCashInfo[0].bAccepted= true;
	bhAcceptTable.psBncCashInfo[1].iValue = 500;
	bhAcceptTable.psBncCashInfo[1].bAccepted= true;
	bhAcceptTable.psBncCashInfo[2].iValue = 1000;
	bhAcceptTable.psBncCashInfo[2].bAccepted= true;
	bhAcceptTable.psBncCashInfo[3].iValue = 2000;
	bhAcceptTable.psBncCashInfo[3].bAccepted= true;
	bhAcceptTable.psBncCashInfo[4].iValue = 5000;
	bhAcceptTable.psBncCashInfo[4].bAccepted= true;
	bhAcceptTable.psBncCashInfo[5].iValue = 10000;
	bhAcceptTable.psBncCashInfo[5].bAccepted= true;
	// 发送命令
	CBRHOpenAndValidate pBhCmd(bhAcceptTable);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != 0)
		ProcessErrorCode(errCode);
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
long CBRHModuleHelper::OpenAndValidate(tBncCashInfo& feedback)
{
	BRH_SetStopAcceptFlag(false);
	// 默认设置全部接收
	tBncSetInhibitList bhAcceptTable;
	bhAcceptTable.iNumOfItems = 6;
	bhAcceptTable.psBncCashInfo = new tBncSetInhibitItem[bhAcceptTable.iNumOfItems];
	bhAcceptTable.psBncCashInfo[0].iValue = 100;
	bhAcceptTable.psBncCashInfo[0].bAccepted= true;
	bhAcceptTable.psBncCashInfo[1].iValue = 500;
	bhAcceptTable.psBncCashInfo[1].bAccepted= true;
	bhAcceptTable.psBncCashInfo[2].iValue = 1000;
	bhAcceptTable.psBncCashInfo[2].bAccepted= true;
	bhAcceptTable.psBncCashInfo[3].iValue = 2000;
	bhAcceptTable.psBncCashInfo[3].bAccepted= true;
	bhAcceptTable.psBncCashInfo[4].iValue = 5000;
	bhAcceptTable.psBncCashInfo[4].bAccepted= true;
	bhAcceptTable.psBncCashInfo[5].iValue = 10000;
	bhAcceptTable.psBncCashInfo[5].bAccepted= true;
	// 发送命令
	CBRHOpenAndValidate pBhCmd(bhAcceptTable);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != 0)
		ProcessErrorCode(errCode);
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
//void CBRHModuleHelper::HoldAsync(CService* pSvc)
//{
//	//CBHHold* pBhCmd = new CBHHold();
//	//pBhCmd->SetAutoDeleted(TRUE);
//	//pBhCmd->SetNeedCallbackService();
//	//pBhCmd->SetService(pSvc);
//	//pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_ACCEPT);
//	//g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
//
//	//theDeviceRegAR.AddBhMotorPressCount();
//
//	// 重庆BRH收纸币后，貌似会自动压暂存。
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压暂存

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
//long CBRHModuleHelper::Hold()
//{
//	//// 发送命令
//	//CBHHold pBhCmd;
//	//long errCode = pBhCmd.ExecuteCommand();
//	//if (errCode != E_NO_ERROR)
//	//	ProcessErrorCode(errCode);
//
//	//theDeviceRegAR.AddBhMotorPressCount();
//	//return errCode;
//
//	// 重庆BRH收纸币后，貌似会自动压暂存。
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压钞（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBRHModuleHelper::CollectAsync(CService* pSvc)
{
	//CBHCollect* pBhCmd = new CBHCollect();
	//pBhCmd->SetAutoDeleted(TRUE);
	//pBhCmd->SetNeedCallbackService();
	//pBhCmd->SetService(pSvc);
	//pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_COLLECT);
	//g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);

	// 重庆BRH调用cashinend结束收纸币，会自动压钞
	CBRHCashInEnd* pBhCmd = new CBRHCashInEnd();
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
long CBRHModuleHelper::Collect()
{
	//// 发送命令
	//	
	//CBHCollect pBhCmd;
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//return errCode;

	// 重庆BRH调用cashinend结束收纸币，会自动压钞。这里主动压钞。
	long errCode = 0;
	CBRHEncash pBhCmd;
	errCode = pBhCmd.ExecuteCommand();

	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	//UINT32 rejectAmount = 0;
	//CASH_INTO_INFO cashIntoInfo;
	//BRH_RSP_GET_STATUS Status;

	//pBhCmd.GetBRHResponse(&Status);


	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压钞(MEI)

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
//long CBRHModuleHelper::Collect(BRH_CMD_FEEDBACK& feedback)
//{	
//	//CBHCollect pBhCmd;
//	//long errCode = pBhCmd.ExecuteCommand();
//	//if (errCode != E_NO_ERROR)
//	//	ProcessErrorCode(errCode);
//	//errCode = pBhCmd.GetResponse(&feedback);
//	//if (errCode != E_NO_ERROR)
//	//	ProcessErrorCode(errCode);
//	//return errCode;
//
//	// 重庆BRH调用cashinend结束收纸币，会自动压钞。
//
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回缓冲区的纸币（异步）

 @param		无

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
void CBRHModuleHelper::ReturnCacheAsync(CService* pSvc)
{
	CBRHCashInRollback* pBhCmd = new CBRHCashInRollback;
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
long CBRHModuleHelper::ReturnCache(bool isCheckShutter)
{
	//// 发送命令
	//CBHReturnOne pBhCmd(isCheckShutter);
	///*CBHRef pBhCmd;*/
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	//return errCode;

	CBRHCashInRollback brhCmd;
	long errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	UINT32 ejectAmount = 0;
	//BRH_RSP_GET_STATUS Status;

	brhCmd.GetBRHResponse(&ejectAmount);
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
void CBRHModuleHelper::ReturnAllAsync(CService* pSvc)
{
	//// 中断轮询操作
	////this->Abort();

	//// 发送命令
	////CBHReturnAll* pBhCmd = new CBHReturnAll(false);
	//CBHRef* pBhCmd =  new CBHRef;
	//pBhCmd->SetAutoDeleted(TRUE);
	//pBhCmd->SetNeedCallbackService();
	//pBhCmd->SetService(pSvc);
	//pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_RETURN_ALL);
	//g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);


	CBRHCashInRollback* pBhCmd =  new CBRHCashInRollback;
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
long CBRHModuleHelper::ReturnAll(bool isCheckShutter)
{
	//// 发送命令
	//CBHReturnAll pBhCmd(isCheckShutter);
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	//return errCode;

	CBRHCashInRollback pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != 0)
		ProcessErrorCode(errCode);

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
long CBRHModuleHelper::ReturnAll(BRH_CMD_FEEDBACK& feedback)
{
//	// 发送命令
//	CBHRef refCmd;
//	long errCode = refCmd.ExecuteCommand();
//	if (errCode != E_NO_ERROR)
//		ProcessErrorCode(errCode);
////	errCode = refCmd.GetResponse(&feedback);
//	if (errCode != E_NO_ERROR)
//		ProcessErrorCode(errCode);
//	return errCode;

	CBRHCashInRollback pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != 0)
		ProcessErrorCode(errCode);

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
//long CBRHModuleHelper::Clean(BRH_CLEAN_DEST dest)
//{
//	//// 开始清币
//	//CBHClearTrans pBhCmd(dest);
//	//long errCode = pBhCmd.ExecuteCommand();
//	//if (errCode != E_NO_ERROR)
//	//	ProcessErrorCode(errCode);
//	//
//	//return errCode;
//
//	//重庆BRH，貌似没有清通道的接口。
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取状态

 @param		无

 @retval	long  错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::GetStatus(tBncDevReturn& status)
{
	
	//long errCode = 0;

	//// 读取
	//CBHGetSts stsCmd;
	//errCode = stsCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//stsCmd.GetResponse(&status.bh_sts);

	//status.bh_faceValue = VALUE_UNKNOWN; // MEI中只有工作才能取得纸币的面额
	//// 读取面值
	///*if (status.bh_sts.IsValidFaceValue())
	//{
	//CBHGetFaceValue faceCmd(status.bh_sts.valueNo);
	//errCode = faceCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//ProcessErrorCode(errCode);
	//faceCmd.GetResponse(&status.bh_faceValue);
	//}*/
	//return errCode;

	CBRHGetStatus brhCmd;
	long errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	brhCmd.GetBRHResponse(&status);
	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取详细状态

 @param		无

 @retval	long  错误代码

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::GetSystemStatus(tBncBillBoxInfo& queryCashUnit)
{
	
	//long errCode = 0;

	//// 读取
	//CBHGetSts stsCmd;
	//errCode = stsCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//stsCmd.GetResponse(&status.bh_sts);

	//status.bh_faceValue = VALUE_UNKNOWN; // MEI中只有工作才能取得纸币的面额
	//// 读取面值
	///*if (status.bh_sts.IsValidFaceValue())
	//{
	//CBHGetFaceValue faceCmd(status.bh_sts.valueNo);
	//errCode = faceCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//ProcessErrorCode(errCode);
	//faceCmd.GetResponse(&status.bh_faceValue);
	//}*/
	//return errCode;

	CBRHQueryCashUnit brhCmd;
	long errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	brhCmd.GetBRHResponse(&queryCashUnit);
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
long CBRHModuleHelper::GetBoxID(BRH_BOX_ID& box_id)
{
	// 发送获取版本命令
	//CBHGetBoxID pBhCmd;
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//pBhCmd.GetResponse(&box_id);

	//return errCode;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      异常修复

@param      none

@retval     long 返回的异常代码

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Repare(bool bMenualRepare)
{
	//long errCode = 0;
	//errCode = Abort();
	////errCode = Init();
	////errCode = Reset();
	//
	//// 尝试关闭串口后再打开
	//errCode = CheckAndOpenBHComport();

	//// 读取状态，看是否正常
	//BH_STS	pState;

	//CBHGetSts bhStatus;
	//errCode = bhStatus.ExecuteCommand();

	//// 命令执行失败
	//if(errCode != E_NO_ERROR){
	//	if(errCode == CBHException::ERROR_BH_RECEIVE_TIMEOUT)
	//		return Reset();
	//	ProcessErrorCode(errCode);
	//}
	//// 取状态成功
	//else{
	//	bhStatus.GetResponse(&pState);
	//	// 取状态返回结果有错误
	//	if(pState.errorCode != 0x30){
	//		// 检查是否是退币口探测器不能提升，这时只能降级
	//		if(memcmp(pState.warning,"D4",2) == 0 && !bMenualRepare){
	//			throw CBHHardwareException(pState.TranslateWarningCode(),_T(__FILE__),__LINE__);
	//		}
	//		
	//		// 快门打开，则以退出方式还给乘客
	//		if(memcmp(pState.warning,"D3",2) == 0 || memcmp(pState.warning,"D1",2) == 0 || memcmp(pState.warning,"D2",2) == 0 || memcmp(pState.warning,"D4",2) == 0){
	//			// 尝试以退出方式恢复模块(尝试三次)
	//			CBHRecRef bhRecRef;
	//			for(int i = 0; i < 3; i++){
	//				errCode = bhRecRef.ExecuteCommand();
	//				if(errCode == 0){
	//					return errCode;//break;
	//				}
	//				else if(errCode == BH_WORK_INVALID){
	//					break;
	//				}
	//			}
	//		}
	//		else{
	//			// 尝试以压入方式恢复模块(尝试三次)
	//			CBHRecEnc bhRecEnc;
	//			for(int i = 0; i < 3; i++){
	//				errCode = bhRecEnc.ExecuteCommand();
	//				if(errCode == 0){
	//					return errCode;//break;
	//				}
	//				else if(errCode == BH_WORK_INVALID){
	//					break;
	//				}
	//			}
	//		}
	//		
	//		// 如果恢复时命令异常，此时用重置来解除异常
	//		if(errCode == BH_WORK_INVALID){
	//			return Reset();
	//		}
	//		// 需要抛异常
	//		errCode = pState.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

	//		CString strErrorCode;
	//		strErrorCode.Format(_T("纸币模块错误代码：%d"),errCode);
	//		throw CBHHardwareException(errCode,_T(__FILE__),__LINE__,strErrorCode);
	//	}
	//}
	//return errCode;


	long errCode = 0;
	// 尝试关闭串口后再打开
	CBRHClose closeCmd;
	errCode = closeCmd.ExecuteCommand();
	// 20171128, lichao:异常修复时，关闭失败不抛异常。
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	errCode = CheckAndOpenBHComport(); 
	if (errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	// 初始化
	errCode = Init(!bMenualRepare);
	if (errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
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
bool CBRHModuleHelper::IsHWException(long errCode)
{
	if (errCode<=1000){
		return false;
	}

	if (errCode==BRH_ERR_TIMEOUT){
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
///**
// @brief		纸币箱是否已装入且锁好
//
// @param		
//
// @retval	bool  true:已装入且锁好;	false:未装入或未锁好
//
// @exception	无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CBRHModuleHelper::IsBankNoteBoxAttached()
//{
	//BH_STATUS status;

	//try {
	//	long er_code = GetStatus(status);
	//}
	//catch (...){
	//	return false;
	//}

	///*vector<BH_ERROR>::iterator iter = find(status.bh_sts.error.begin(),status.bh_sts.error.end(),ERR_BH_NO_BOX);
	//if(iter == status.bh_sts.error.end())*/
	//// 警告代码为C1为钱箱不在位
	//if(memcmp(status.bh_sts.warning,"C1",2) == 0)
	//{
	//	return FALSE;
	//}
	//else
	//	return TRUE;
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		纸币回收箱是否已装入且锁好

 @param		

 @retval	bool  true:已装入且锁好;	false:未装入或未锁好

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
bool CBRHModuleHelper::IsCashBoxAttached()
{
	long errCode = 0;
	try{
		// 检查模块状态
		tBncSysDevStatus sysStatus;
		CBRHGetSystemStatus brhCmd;
		errCode = brhCmd.ExecuteCommand();

		if(errCode!=BRH_NO_ERROR){
			ProcessErrorCode(errCode);	
		}
		brhCmd.GetBRHResponse(&sysStatus);

		return sysStatus.bIsCollectBoxInstalled;

	}
	catch(...)
	{
		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		纸币找零箱是否已装入且锁好

 @param		

 @retval	bool  true:已装入且锁好;	false:未装入或未锁好

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
bool CBRHModuleHelper::IsCashChangeBoxAttached()
{
	long errCode = 0;

	try{
		tBncSysDevStatus sysStatus;
		CBRHGetSystemStatus brhCmd;
		errCode = brhCmd.ExecuteCommand();

		if(errCode!=BRH_NO_ERROR){
			ProcessErrorCode(errCode);	
		}
		brhCmd.GetBRHResponse(&sysStatus);
		return sysStatus.bIsChangeBoxInstalled;
	}
	catch(...){
		return false;
	}
	
	return false;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      当纸币接收模块产生故障，以退出方式修复此故障(尝试三次)

@param      无

@retval     long 0 成功；非0：失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::TryRecoveryBanknoteModule(){
	
	long lRet = 0;

	//try{
	//	// 取纸币接收模块状态
	//	CBHGetSts bhGetStatus;

	//	lRet = bhGetStatus.ExecuteCommand();

	//	if(0 != lRet){
	//		ProcessErrorCode(lRet);
	//	}
	//	else{
	//		// 取结果
	//		BH_STS bhStatus;
	//		bhGetStatus.GetResponse(&bhStatus);

	//		if(bhStatus.errorCode != 0x30){
	//			// 尝试以退出方式恢复模块(尝试三次)
	//			CBHRecRef bhRecRef;
	//			for(int i = 0; i < 3; i++){
	//				lRet = bhRecRef.ExecuteCommand();
	//				if(lRet == 0){
	//					return lRet;//break;
	//				}
	//				else if(lRet == BH_WORK_INVALID){
	//					break;
	//				}
	//			}

	//			// 需要抛异常
	//			lRet = bhStatus.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

	//			CString strErrorCode;
	//			strErrorCode.Format(_T("纸币模块错误代码：%d"),lRet);
	//			throw CBHHardwareException(lRet,_T(__FILE__),__LINE__,strErrorCode);
	//		}
	//	}
	//}
	//catch(CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch(...){

	//}
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
long CBRHModuleHelper::CheckAndOpenBHComport(){
	long errCode = 0;

	int port = theTVM_SETTING.GetBHComPort() - 1;//theTVM_INFO.GetBHCommPort() - 1;
	int baud = theTVM_SETTING.GetBHBaudRate();   //theTVM_INFO.GetBHBaudRate();

	//// 打开串口
	//CBHConnect pBhConnectCmd(port,baud,0);
	//errCode = pBhConnectCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);


	CBRHOpen brhCmd;
	errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      纸币找零请求函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBRHModuleHelper::DispenseAsync(CService* pSvc, UINT32 dispenseAmount)
{
	BRH_CMD_DISPENSE_REQUEST requestData;
	memset(&requestData, 0, sizeof(requestData));
	char currency [4] = {"CNY"};
	tBncChangeNum pChangeNum;
	tBncDevReturn pDevStatus;
	//strcpy(requestData.currency.currencyCode, currency);
	//requestData.mixNumber = XFS_C_CDR_MIX_ALGORITHM;
	pChangeNum.uiAmount = dispenseAmount;
	
	CBRHDispenseRequest* pBhCmd =  new CBRHDispenseRequest(&pChangeNum, &pDevStatus);
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_CHANGE_COMMAND_ID_SEND_OUT);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取消等待用户取走钞票

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::CancelWaitingCashTaken()
{
	CBRHCancel brhCmd;
	long errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      回收找零的钞票

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Retract(UINT32* retractAmount)
{
	long errCode = 0;
	CBRHCancel brhCmd;
	errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	//BRH_RSP_GET_STATUS Status;

	//brhCmd.GetBRHResponse(&Status);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零接口

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CBRHModuleHelper::SendOutBill(CService* pService,int nRecycBoxA,int nRecycBoxB,int nRecycBoxC,int nCountBoxD,int nChangeBox){
//	BRH_CMD_DISPENSE_REQUEST requestData;
//	memset(&requestData, 0, sizeof(requestData));
//	char currency [4] = {"CNY"};
//	strcpy(requestData.currency.currencyCode, currency);
//	requestData.mixNumber = XFS_C_CDR_MXA_MIN_BILLS;
//	requestData.denomination.amount = 100;
//	CBRHDispenseRequest* pBhCmd =  new CBRHDispenseRequest(&requestData);
//	pBhCmd->SetAutoDeleted(TRUE);
//	pBhCmd->SetNeedCallbackService();
//	pBhCmd->SetService(pService);
//	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_RETURN_ALL);
//	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      清空纸币

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Empty(char* chBoxName,tBncChangeNum* result,bool b_toFloat){
	CBRHEmpty letEmpty(chBoxName,b_toFloat);

	long lErrCode = letEmpty.ExecuteCommand();

	if(lErrCode != BRH_NO_ERROR){
		ProcessErrorCode(lErrCode);
	}

	letEmpty.GetResponse(result);

	return lErrCode;
	    return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置回收箱卸载时计数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::ClearCollectionUnitCount(){
	
	BRH_RSP_QUERY_CASHUNIT stCashUnit;
	tBncDevReturn   status;

	tBncBillBoxInfo queryCashUnit;
	CBRHQueryCashUnit brhCmd;
	long errCode = brhCmd.ExecuteCommand();

	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);	
	}
	brhCmd.GetBRHResponse(&queryCashUnit);


	// 更新关于回收箱的数量
	for(int i = 0 ;i < MAX_BOXNUM; i++){
		if(queryCashUnit.pCashBoxValueInfo[i].uiBoxType == 0){
			queryCashUnit.pCashBoxValueInfo[i].uiBillNum =0;
		}
	}

	// 更新数量
	CBRHUpdateCashUnit updateCashUnit(0,&queryCashUnit);
	errCode = updateCashUnit.ExecuteCommand();

	if(errCode != BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	return errCode;
}


map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> CBRHModuleHelper::GetEmptyFull(){
	BRH_RSP_QUERY_CASHUNIT stCashUnit;
	tBncDevReturn    status;
	map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> mapItems;

	tBncBillBoxInfo queryCashUnit;
	CBRHQueryCashUnit brhCmd;
	long errCode = brhCmd.ExecuteCommand();

	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);	
	}
	brhCmd.GetBRHResponse(&queryCashUnit);



	//// 更新关于回收箱的数量
	//for(int i = 0 ;i < stCashUnit.physicalCashUnitList.size; i++){
	//	T_XfsPhysicalCashUnit item = stCashUnit.physicalCashUnitList.items[i];

	//	if(strcmp((char*)item.name,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0){
	//		mapItems[BNR_RECYCLER_BOX_A] = item;
	//	}
	//	else if(strcmp((char*)item.name,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0){
	//		mapItems[BNR_RECYCLER_BOX_B] = item;
	//	}
	//	else if(strcmp((char*)item.name,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0){
	//		mapItems[BNR_RECYCLER_BOX_C] = item;
	//	}
	//	else if(strcmp((char*)item.name,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0){
	//		mapItems[BNR_RECYCLER_BOX_D] = item;
	//	}
	//	else if(strcmp((char*)item.name,BNR_MODULE_CASH_BOX_NAME) == 0){
	//		mapItems[BNR_MODULE_CASH_BOX] = item;
	//	}
	//	else if(strcmp((char*)item.name,BNR_MODULE_LOADER_BOX_NAME) == 0){
	//		mapItems[BNR_MODULE_CHANGE_BOX] = item;
	//	}
	//}

	return mapItems;
}


void CBRHModuleHelper::ProcessErrorCode(long errCode)
{
	if (IsHWException(errCode)){										
		if( errCode==CBRHHardwareException::USB_NO_SUCH_DEVICE	
			|| errCode==CBRHHardwareException::USB_DEVICE_NOT_FOUND
			|| errCode==CBRHHardwareException::USB_DEVICE_REMOVED
			|| errCode==CBRHHardwareException::USB_DEVICE_LOCKED
			|| errCode==10000 // 10000(当纸币模块工具打开，启动TVM，连接Open纸币模块报此错误码。
			|| errCode==1 ){// 1(当纸币模块工具打开，启动TVM，断开连接Close纸币模块报此错误码。
				errCode = CBRHHardwareException::BNR_USB_ERR_CONNECT;	
		}
		else if( errCode> 10000 ){
			errCode = CBRHHardwareException::BNR_USB_ERR_OTHER;
		}	
		throw CBRHHardwareException(errCode, _T(__FILE__), __LINE__);
	}
	else{
		throw CBRHException(errCode,_T(__FILE__),__LINE__);	
	}		
}