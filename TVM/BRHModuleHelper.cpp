#include "StdAfx.h"
#include "BRHModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		(i)CService& service

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBRHModuleHelper::CBRHModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBRHModuleHelper::~CBRHModuleHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ֹͣ����

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Abort()
{
	//// ����ֹͣ����
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
 @brief		��ʼ��

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Init(bool bRepareCount)
{
	long errCode = 0;
	
	// 1����鲢�򿪴���
	errCode = CheckAndOpenBHComport();
	Reset();
	
	// 2������BRH��ʱ��
	time_t t = time(NULL);
	CBRHSetDateTime setTimeCmd(t);
	setTimeCmd.ExecuteCommand();

	// 3����ȡBRH״̬
	errCode = 0;
	tBncDevReturn Status;
	memset(&Status, 0, sizeof(Status));
	GetStatus(Status);
	// ��ȡ״̬�ɹ�������Ƿ���Ҫ��λ����;���ɹ��Ļ���GetStatus�ڲ������쳣�������ߵ����
	if ((Status.iErrorCode != XFS_S_CDR_DS_ON_LINE)
		&& (Status.iErrorCode != XFS_S_CDR_DS_USER_ERROR)
		&& (Status.iErrorCode != XFS_S_CDR_DS_HARDWARE_ERROR)) {
			Reset();
			
	}
	else if((Status.iErrorCode == XFS_S_CDR_DS_USER_ERROR) || (Status.iErrorCode == XFS_S_CDR_DS_HARDWARE_ERROR)){
		throw CBRHHardwareException(XFS_S_CDR_DS_HARDWARE_ERROR, _T(__FILE__), __LINE__);
	}
	else{
		// ״̬ok
	}
	CheckModuleStatus();


	// 4����������
	// 4.1ȡ��ǰ��������
	//BRH_RSP_GET_CAPABLILITIES capablilities;
	//CBRHGetBillBoxInfo getCapablilities;
	//errCode = getCapablilities.ExecuteCommand();

	//if(errCode!=BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);
	//}
	//BRH_RSP_GET_STATUS CapStatus;
	//getCapablilities.GetBRHResponse(&capablilities,&CapStatus);
	//// ���������Զ��ͳ�
	////capablilities.autoPresent  = true;
	//// �����������Զ�����
	////capablilities.selfTestMode = T_SelfTestMode::STM_AUTO;

	//// 4.2��������
	//CBRHSetBillBoxInfo setCapabilities(0,&capablilities);
	//errCode = setCapabilities.ExecuteCommand();
	//if(errCode!=BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);
	//}

	// 5.1ȡ���ӵ�������Ϣ���˲�������ʼ���ã�����ֽ�ҽ���ʱ������ȷ���ؽ���ֽ�ҵ�������Ϣ��
	// ʹ��do-while����ֹӲ����λʱ����æµ״̬���ӿڱ���
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

			// Ϊ�˷�ֹѭ���ļ�����ʵ�ʲ�һ�£���ʼ������һ�¡�
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_A = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_B = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();	
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_C = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();	
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycle_D = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO& cashBoxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO& changeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

			if(bRepareCount){
				// ����������Ӳ��������ʵ������
				if (cashUnit[BNR_RECYCLER_BOX_A].count != recycle_A.ulCurCount){
					CString strLog;
					strLog.Format(_T("����ֽ��ѭ����A�������ʵ�ʲ�һ�£��������%d�ţ�ʵ��%d��;�����䵱ǰ%d��"), recycle_A.ulCurCount, cashUnit[BNR_RECYCLER_BOX_A].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_A].count - recycle_A.ulCurCount;
					if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_A.ulCurCount = cashUnit[BNR_RECYCLER_BOX_A].count;
				}
				if (cashUnit[BNR_RECYCLER_BOX_B].count != recycle_B.ulCurCount){
					CString strLog;
					strLog.Format(_T("����ֽ��ѭ����B�������ʵ�ʲ�һ�£��������%d�ţ�ʵ��%d��;�����䵱ǰ%d��"), recycle_B.ulCurCount, cashUnit[BNR_RECYCLER_BOX_B].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_B].count - recycle_B.ulCurCount;
					if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_B.ulCurCount = cashUnit[BNR_RECYCLER_BOX_B].count;
				}
				if (cashUnit[BNR_RECYCLER_BOX_C].count != recycle_C.ulCurCount){
					CString strLog;
					strLog.Format(_T("����ֽ��ѭ����C�������ʵ�ʲ�һ�£��������%d�ţ�ʵ��%d��;�����䵱ǰ%d��"), recycle_C.ulCurCount, cashUnit[BNR_RECYCLER_BOX_C].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_C].count - recycle_C.ulCurCount;
					if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_C.ulCurCount = cashUnit[BNR_RECYCLER_BOX_C].count;
				}
				if (cashUnit[BNR_RECYCLER_BOX_D].count != recycle_D.ulCurCount){
					CString strLog;
					strLog.Format(_T("����ֽ��ѭ����D�������ʵ�ʲ�һ�£��������%d�ţ�ʵ��%d��;�����䵱ǰ%d��"), recycle_D.ulCurCount, cashUnit[BNR_RECYCLER_BOX_D].count, changeBoxInfo.ulCurCount);
					theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
					int adjustCount = cashUnit[BNR_RECYCLER_BOX_D].count - recycle_D.ulCurCount;
					if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						changeBoxInfo.ulCurCount -= adjustCount;
					}
					recycle_D.ulCurCount = cashUnit[BNR_RECYCLER_BOX_D].count;
				}
				//// ����������֤���֣�Ӳ�����صĲ������������ȷ���������ע�Ͳ�ʹ�á�
				//if (cashUnit[BNR_MODULE_CHANGE_BOX].count != changeBoxInfo.ulCurCount){
				//	CString strLog;
				//	strLog.Format(_T("����ֽ�Ҳ������������ʵ�ʲ�һ�£��������%d�ţ�ʵ��%d��"), changeBoxInfo.ulCurCount, cashUnit[BNR_MODULE_CHANGE_BOX].count);
				//	theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
				//	changeBoxInfo.ulCurCount = cashUnit[BNR_MODULE_CHANGE_BOX].count;
				//	theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBoxInfo);
				//}
				// ����������֤���֣�Ӳ�����صĻ������������ȷ���������ע�Ͳ�ʹ�á�
				//if (cashUnit[BNR_MODULE_CASH_BOX].count != cashBoxInfo.BankntoeTotalCount()){
				//	CString strLog;
				//	strLog.Format(_T("����ֽ�һ������������ʵ�ʲ�һ�£��������%d�ţ�ʵ��%d��"), cashBoxInfo.BankntoeTotalCount(), cashUnit[BNR_MODULE_CASH_BOX].count);
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
				Sleep(5000);//��ֽ��ģ���˳�æµ״̬
			}
		}
	}while(1);


	// 5���������ӵ�������Ϣ��ֻ�����������䣩
	//CBRHSetCashType setCashType(BNR_MODULE_ID::BNR_MODULE_CHANGE_BOX,theSETTING.GetBNRChangeBoxAmount() * 100);
	//errCode = setCashType.ExecuteCommand();
	/*if(errCode!=BRH_NO_ERROR){
	ProcessErrorCode(errCode);
	}*/


	// 5.1ȡ���ӵ�������Ϣ
	/*BRH_RSP_QUERY_CASHUNIT cashUnit;
	CBRHQueryCashUnit queryCashUnit;
	errCode = queryCashUnit.ExecuteCommand();
	if(errCode != BRH_NO_ERROR){
	ProcessErrorCode(errCode);
	}

	queryCashUnit.GetBRHResponse(&cashUnit);*/


	//// ����������Ϣ
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

	//// 5.2�������ӵ�������Ϣ
	//CBRHUpdateCashUnit updateCashUnit(&cashUnit);
	//errCode = updateCashUnit.ExecuteCommand();

	//if(errCode != BRH_NO_ERROR){
	//	ProcessErrorCode(errCode);
	//}


	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��λ

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Reset()
{
	long errCode = 0;

	//// ���͸�λ����
	//CBHReset rstCmdl;
	//errCode = rstCmdl.ExecuteCommand();

	//theDeviceRegAR.AddBhResetCount();

	//// ��λʱ������Ҫ���Զ��ָ�ģʽ�޸�
	//if(errCode == CBHException::ERROR_RESET_NEED_REC_AUT){
	//	CBHClearTrans cleanCmd;
	//	errCode = cleanCmd.ExecuteCommand();
	//}
	//else{
	//	// ��ȡ״̬
	//	CBHGetSts stsCmd;
	//	BH_STS	  state;
	//	stsCmd.ExecuteCommand();
	//	stsCmd.GetResponse(&state);

	//	// ȡ���ϴ���
	//	if (state.errorCode == 0x30){
	//		// ����޹���,����Ҫ��λ
	//		return 0;
	//	}else {
	//		// ���ڹ���
	//		//rstCmdl.ExecuteCommand();
	//		CBHClearTrans cleanCmd;
	//		errCode = cleanCmd.ExecuteCommand();
	//	}
	//	// �ٴ�ȡ״̬
	//	stsCmd.ExecuteCommand();
	//	state = BH_STS();
	//	stsCmd.GetResponse(&state);
	//	// �����ϴ���
	//	if (state.errorCode != 0x30){
	//		// �޹��ϣ���λ�ɹ�
	//	}else{
	//		// ��λʧ��
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

	//// ���ģ��״̬
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
 @brief		���ģ��״̬

 @param		��

 @retval	long	�������

 @exception	��
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
		// ���SystemErrorCode.
		if (errCode==SEC_OPENED)
		{
//			throw ֽ��ģ���һ���ǰ屻��;
		}
		else if (errCode==SEC_UNLOCKED)
		{
//			throw �ֽ�ģ���������;
		}
		else{ // it means ok(SEC_NO_ERROR).
			// do nothing.
		}
		// ���SystemCashTypeStatus
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
						// throw ���������ֽ����ʹ���
					}
				}else if(
					queryCashUnit.pCashBoxValueInfo[sz].uiBoxType ==0 &&
					(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo  == (RECYCLER_CLASS+3)) ||
					(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo  == (RECYCLER_CLASS+4))){
						if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus != CTS_OK){
							// throw ѭ��������1���ֽ����ʹ���
						}

				}else if((queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+5)) ||
					(queryCashUnit.pCashBoxValueInfo[sz].uiBoxNo == (RECYCLER_CLASS+6))){
						if(queryCashUnit.pCashBoxValueInfo[sz].uiBoxStatus != CTS_OK){
							// throw ѭ��������2���ֽ����ʹ���
						}      
				}  
			}				 
		}
		// ���SystemBillStorageStatus
		if (errCode!=SBSS_OK)
		{
			if (errCode==SBSS_NO_CHANGE){
				// throw ֽ��ģ���޿�������
			}
			else if (errCode==SBSS_ERROR){
				// throw ��������
			}
			else if (errCode==SBSS_WARNING){
				// throw ������ջ򺬴�������ֽ��
			}
		}
		// ���BillTransportStatus
		if (errCode!=BTS_OK)
		{
			// throw ֽ��ģ�鿨��
		}
		// ������ӵİ�װ״̬
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
 @brief		ֹͣ����ʶ��ֽ��

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::StopValidation()
{
	//// ֹͣ��ǰ����:��Ҫ�ֶ�ֹͣ���жϻص������������ֹͣʶ��
	////this->Abort();
	////BH_DisableInsert();
	//SYNC(BHMGR,_T("BHMGR"));
	//// ����ֹͣ����
	//CBHStopValidation pBhCmd;
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	//return errCode;

	BRH_SetStopAcceptFlag(true);//���������ʶ����ֹֹͣ���յ������ڿ�ʼ����֮ǰ������
	CBRHCancel brhCmd;
	long errCode = brhCmd.ExecuteCommand();
	if(errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);	
	}
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ����ʶ��ֽ�ң��첽��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
void CBRHModuleHelper::OpenAndValidateAsync(CService* pSvc,tBncSetInhibitList& bhTable)
{
	BRH_SetStopAcceptFlag(false);//���������ʶ����ֹֹͣ���յ������ڿ�ʼ����֮ǰ������
	CBRHOpenAndValidate* pBhCmd = new CBRHOpenAndValidate(bhTable);
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_INSERT);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ����ʶ��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::OpenAndValidate(BRH_STATUS& status)
{
	BRH_SetStopAcceptFlag(false);
	// Ĭ������ȫ������
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
	// ��������
	CBRHOpenAndValidate pBhCmd(bhAcceptTable);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != 0)
		ProcessErrorCode(errCode);
	pBhCmd.GetResponse(&status);
	return errCode;	
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		MEI ��ʼ����ʶ��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::OpenAndValidate(tBncCashInfo& feedback)
{
	BRH_SetStopAcceptFlag(false);
	// Ĭ������ȫ������
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
	// ��������
	CBRHOpenAndValidate pBhCmd(bhAcceptTable);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != 0)
		ProcessErrorCode(errCode);
	pBhCmd.GetResponse(&feedback);
	return errCode;	
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ�ݴ棨�첽��

 @param		��

 @retval	��

 @exception	��
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
//	// ����BRH��ֽ�Һ�ò�ƻ��Զ�ѹ�ݴ档
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ�ݴ�

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
//long CBRHModuleHelper::Hold()
//{
//	//// ��������
//	//CBHHold pBhCmd;
//	//long errCode = pBhCmd.ExecuteCommand();
//	//if (errCode != E_NO_ERROR)
//	//	ProcessErrorCode(errCode);
//
//	//theDeviceRegAR.AddBhMotorPressCount();
//	//return errCode;
//
//	// ����BRH��ֽ�Һ�ò�ƻ��Զ�ѹ�ݴ档
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ�����첽��

 @param		��

 @retval	��

 @exception	��
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

	// ����BRH����cashinend������ֽ�ң����Զ�ѹ��
	CBRHCashInEnd* pBhCmd = new CBRHCashInEnd();
	pBhCmd->SetAutoDeleted(TRUE);
	pBhCmd->SetNeedCallbackService();
	pBhCmd->SetService(pSvc);
	pBhCmd->SetCommandID(BANKNOTE_MODULE_COMMAND_ID_COLLECT);
	g_pBHControl->DoCommand(pBhCmd, SP_EXECMODE_QUEUE);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::Collect()
{
	//// ��������
	//	
	//CBHCollect pBhCmd;
	//long errCode = pBhCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//return errCode;

	// ����BRH����cashinend������ֽ�ң����Զ�ѹ������������ѹ����
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
 @brief		ѹ��(MEI)

 @param		��

 @retval	��

 @exception	��
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
//	// ����BRH����cashinend������ֽ�ң����Զ�ѹ����
//
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�˻ػ�������ֽ�ң��첽��

 @param		��

 @retval	��

 @exception	��
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
 @brief		�˻ػ�������ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::ReturnCache(bool isCheckShutter)
{
	//// ��������
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
 @brief		�˻�ȫ��ֽ�ң��첽��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
void CBRHModuleHelper::ReturnAllAsync(CService* pSvc)
{
	//// �ж���ѯ����
	////this->Abort();

	//// ��������
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
 @brief		�˻�ȫ��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::ReturnAll(bool isCheckShutter)
{
	//// ��������
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
 @brief		MEI��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::ReturnAll(BRH_CMD_FEEDBACK& feedback)
{
//	// ��������
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
 @brief		�����Ʊ

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
//long CBRHModuleHelper::Clean(BRH_CLEAN_DEST dest)
//{
//	//// ��ʼ���
//	//CBHClearTrans pBhCmd(dest);
//	//long errCode = pBhCmd.ExecuteCommand();
//	//if (errCode != E_NO_ERROR)
//	//	ProcessErrorCode(errCode);
//	//
//	//return errCode;
//
//	//����BRH��ò��û����ͨ���Ľӿڡ�
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ״̬

 @param		��

 @retval	long  �������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::GetStatus(tBncDevReturn& status)
{
	
	//long errCode = 0;

	//// ��ȡ
	//CBHGetSts stsCmd;
	//errCode = stsCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//stsCmd.GetResponse(&status.bh_sts);

	//status.bh_faceValue = VALUE_UNKNOWN; // MEI��ֻ�й�������ȡ��ֽ�ҵ����
	//// ��ȡ��ֵ
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
 @brief		��ȡ��ϸ״̬

 @param		��

 @retval	long  �������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::GetSystemStatus(tBncBillBoxInfo& queryCashUnit)
{
	
	//long errCode = 0;

	//// ��ȡ
	//CBHGetSts stsCmd;
	//errCode = stsCmd.ExecuteCommand();
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);
	//stsCmd.GetResponse(&status.bh_sts);

	//status.bh_faceValue = VALUE_UNKNOWN; // MEI��ֻ�й�������ȡ��ֽ�ҵ����
	//// ��ȡ��ֵ
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
 @brief		��ȡǮ��ID(MEI)

 @param		(o)BH_BOX_ID& box_id  Ǯ��ID

 @retval	long  �������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::GetBoxID(BRH_BOX_ID& box_id)
{
	// ���ͻ�ȡ�汾����
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
@brief      �쳣�޸�

@param      none

@retval     long ���ص��쳣����

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
	//// ���Թرմ��ں��ٴ�
	//errCode = CheckAndOpenBHComport();

	//// ��ȡ״̬�����Ƿ�����
	//BH_STS	pState;

	//CBHGetSts bhStatus;
	//errCode = bhStatus.ExecuteCommand();

	//// ����ִ��ʧ��
	//if(errCode != E_NO_ERROR){
	//	if(errCode == CBHException::ERROR_BH_RECEIVE_TIMEOUT)
	//		return Reset();
	//	ProcessErrorCode(errCode);
	//}
	//// ȡ״̬�ɹ�
	//else{
	//	bhStatus.GetResponse(&pState);
	//	// ȡ״̬���ؽ���д���
	//	if(pState.errorCode != 0x30){
	//		// ����Ƿ����˱ҿ�̽����������������ʱֻ�ܽ���
	//		if(memcmp(pState.warning,"D4",2) == 0 && !bMenualRepare){
	//			throw CBHHardwareException(pState.TranslateWarningCode(),_T(__FILE__),__LINE__);
	//		}
	//		
	//		// ���Ŵ򿪣������˳���ʽ�����˿�
	//		if(memcmp(pState.warning,"D3",2) == 0 || memcmp(pState.warning,"D1",2) == 0 || memcmp(pState.warning,"D2",2) == 0 || memcmp(pState.warning,"D4",2) == 0){
	//			// �������˳���ʽ�ָ�ģ��(��������)
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
	//			// ������ѹ�뷽ʽ�ָ�ģ��(��������)
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
	//		// ����ָ�ʱ�����쳣����ʱ������������쳣
	//		if(errCode == BH_WORK_INVALID){
	//			return Reset();
	//		}
	//		// ��Ҫ���쳣
	//		errCode = pState.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

	//		CString strErrorCode;
	//		strErrorCode.Format(_T("ֽ��ģ�������룺%d"),errCode);
	//		throw CBHHardwareException(errCode,_T(__FILE__),__LINE__,strErrorCode);
	//	}
	//}
	//return errCode;


	long errCode = 0;
	// ���Թرմ��ں��ٴ�
	CBRHClose closeCmd;
	errCode = closeCmd.ExecuteCommand();
	// 20171128, lichao:�쳣�޸�ʱ���ر�ʧ�ܲ����쳣��
	//if (errCode != E_NO_ERROR)
	//	ProcessErrorCode(errCode);

	errCode = CheckAndOpenBHComport(); 
	if (errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}
	// ��ʼ��
	errCode = Init(!bMenualRepare);
	if (errCode!=BRH_NO_ERROR){
		ProcessErrorCode(errCode);
	}

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�Ƿ�ΪӲ���쳣

 @param		(i)long errCode		�������

 @retval	bool  true:Ӳ���쳣;	false:����쳣

 @exception	��
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
// @brief		ֽ�����Ƿ���װ��������
//
// @param		
//
// @retval	bool  true:��װ��������;	false:δװ���δ����
//
// @exception	��
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
	//// �������ΪC1ΪǮ�䲻��λ
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
 @brief		ֽ�һ������Ƿ���װ��������

 @param		

 @retval	bool  true:��װ��������;	false:δװ���δ����

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
bool CBRHModuleHelper::IsCashBoxAttached()
{
	long errCode = 0;
	try{
		// ���ģ��״̬
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
 @brief		ֽ���������Ƿ���װ��������

 @param		

 @retval	bool  true:��װ��������;	false:δװ���δ����

 @exception	��
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
@brief      ��ֽ�ҽ���ģ��������ϣ����˳���ʽ�޸��˹���(��������)

@param      ��

@retval     long 0 �ɹ�����0��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::TryRecoveryBanknoteModule(){
	
	long lRet = 0;

	//try{
	//	// ȡֽ�ҽ���ģ��״̬
	//	CBHGetSts bhGetStatus;

	//	lRet = bhGetStatus.ExecuteCommand();

	//	if(0 != lRet){
	//		ProcessErrorCode(lRet);
	//	}
	//	else{
	//		// ȡ���
	//		BH_STS bhStatus;
	//		bhGetStatus.GetResponse(&bhStatus);

	//		if(bhStatus.errorCode != 0x30){
	//			// �������˳���ʽ�ָ�ģ��(��������)
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

	//			// ��Ҫ���쳣
	//			lRet = bhStatus.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

	//			CString strErrorCode;
	//			strErrorCode.Format(_T("ֽ��ģ�������룺%d"),lRet);
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
@brief      ��ֽ�ҽ���ģ�飬�Ѵ���رպ��ٴ򿪡�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHModuleHelper::CheckAndOpenBHComport(){
	long errCode = 0;

	int port = theTVM_SETTING.GetBHComPort() - 1;//theTVM_INFO.GetBHCommPort() - 1;
	int baud = theTVM_SETTING.GetBHBaudRate();   //theTVM_INFO.GetBHBaudRate();

	//// �򿪴���
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
@brief      ֽ������������

@param      ��

@retval     ��

@exception  ��
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
@brief      ȡ���ȴ��û�ȡ�߳�Ʊ

@param      ��

@retval     ��

@exception  ��
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
@brief      ��������ĳ�Ʊ

@param      ��

@retval     ��

@exception  ��
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
@brief      ֽ������ӿ�

@param      ��

@retval     ��

@exception  ��
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
@brief      ���ֽ��

@param      ��

@retval     ��

@exception  ��
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
@brief      ���û�����ж��ʱ����

@param      ��

@retval     ��

@exception  ��
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


	// ���¹��ڻ����������
	for(int i = 0 ;i < MAX_BOXNUM; i++){
		if(queryCashUnit.pCashBoxValueInfo[i].uiBoxType == 0){
			queryCashUnit.pCashBoxValueInfo[i].uiBillNum =0;
		}
	}

	// ��������
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



	//// ���¹��ڻ����������
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
			|| errCode==10000 // 10000(��ֽ��ģ�鹤�ߴ򿪣�����TVM������Openֽ��ģ�鱨�˴����롣
			|| errCode==1 ){// 1(��ֽ��ģ�鹤�ߴ򿪣�����TVM���Ͽ�����Closeֽ��ģ�鱨�˴����롣
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