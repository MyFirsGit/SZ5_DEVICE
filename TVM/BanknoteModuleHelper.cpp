#include "StdAfx.h"
#include "BanknoteModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �쳣�׳��꣨����쳣/Ӳ���쳣��
#define INTERNAL_ERROR(errCode)											\
{																		\
	if (IsHWException(errCode))											\
		throw CBHHardwareException(errCode, _T(__FILE__), __LINE__);	\
	else																\
		throw CBHException(errCode,_T(__FILE__),__LINE__);				\
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		(i)CService& service

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteModuleHelper::CBanknoteModuleHelper()
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
CBanknoteModuleHelper::~CBanknoteModuleHelper()
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
long CBanknoteModuleHelper::Abort()
{
	// ����ֹͣ����
	CBHAbortAction pBhAbortCmd;
	long errCode = pBhAbortCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ��

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Init()
{
	long errCode = 0;
	
	// ��鲢�򿪴���
	errCode = CheckAndOpenBHComport();

	
	/*errCode = getState.ExecuteCommand();
	if(errCode != E_NO_ERROR){
		INTERNAL_ERROR(errCode);
	}
	getState.GetResponse(&pState);*/

	//// ���ù������ģʽ���û�����
	//CBHSetErrorClean cleanMode(false);
	//errCode = cleanMode.ExecuteCommand();
	//
	//// ���ù���ģʽ�� ����ģʽ����ƴ������ļ���ȡ���ݶ�15��
	//CBHSetWorkMode workMode(DEADLINE_NORMAL);
	//errCode = workMode.ExecuteCommand();
	
	// ��λ���������ģʽ��Ч
	errCode = Reset();
	

	// ȡ״̬
	CBHGetSts getState;
	BRH_STS	pState;
	// ���״̬�����Ƿ���Ч
	errCode = getState.ExecuteCommand();
	if(errCode != E_NO_ERROR){
		INTERNAL_ERROR(errCode);
	}
	getState.GetResponse(&pState);
	if (pState.cleanErorMode != CUSTOM_CLEAN_ERROR){
		// ���ù������ģʽ���û�����
		CBHSetErrorClean cleanMode(false);
		errCode = cleanMode.ExecuteCommand();

		// ���ù���ģʽ�� ����ģʽ����ƴ������ļ���ȡ���ݶ�15��
		CBHSetWorkMode workMode(DEADLINE_NORMAL);
		errCode = workMode.ExecuteCommand();

		errCode = Reset();
		// ����ʧ��
		return errCode;
	}
		
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��λ

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Reset()
{
	long errCode = 0;

	// ���͸�λ����
	CBHReset rstCmdl;
	errCode = rstCmdl.ExecuteCommand();

	theDeviceRegAR.AddBhResetCount();

	// ��λʱ������Ҫ���Զ��ָ�ģʽ�޸�
	if(errCode == CBHException::ERROR_RESET_NEED_REC_AUT){
		CBHClearTrans cleanCmd;
		errCode = cleanCmd.ExecuteCommand();
	}
	else{
		// ��ȡ״̬
		CBHGetSts stsCmd;
		BRH_STS	  state;
		stsCmd.ExecuteCommand();
		stsCmd.GetResponse(&state);

		// ȡ���ϴ���
		if (state.errorCode == 0x30){
			// ����޹���,����Ҫ��λ
			return 0;
		}else {
			// ���ڹ���
			//rstCmdl.ExecuteCommand();
			CBHClearTrans cleanCmd;
			errCode = cleanCmd.ExecuteCommand();
		}
		// �ٴ�ȡ״̬
		stsCmd.ExecuteCommand();
		state = BRH_STS();
		stsCmd.GetResponse(&state);
		// �����ϴ���
		if (state.errorCode != 0x30){
			// �޹��ϣ���λ�ɹ�
		}else{
			// ��λʧ��
		}
	}
	
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
long CBanknoteModuleHelper::StopValidation()
{
	// ֹͣ��ǰ����:��Ҫ�ֶ�ֹͣ���жϻص������������ֹͣʶ��
	//this->Abort();
	//BH_DisableInsert();
	SYNC(BHMGR,_T("BHMGR"));
	// ����ֹͣ����
	CBHStopValidation pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

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
 @brief		��ʼ����ʶ��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::OpenAndValidate(BH_STATUS& status)
{
	// Ĭ������ȫ������
	BRH_FACE_ACEPTED_TABLE bhTable;
	bhTable.bIsBanknote1Yuan	= true;
	bhTable.bIsBanknote5Yuan	= true;
	bhTable.bIsBanknote10Yuan	= true;
	bhTable.bIsBanknote20Yuan	= true;
	bhTable.bIsBanknote50Yuan	= true;
	bhTable.bIsBanknote100Yuan	= true;
	// ��������
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
 @brief		MEI ��ʼ����ʶ��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::OpenAndValidate(BH_CMD_FEEDBACK& feedback)
{
	// Ĭ������ȫ������
	BRH_FACE_ACEPTED_TABLE bhTable;
	bhTable.bIsBanknote1Yuan	= true;
	bhTable.bIsBanknote5Yuan	= true;
	bhTable.bIsBanknote10Yuan	= true;
	bhTable.bIsBanknote20Yuan	= true;
	bhTable.bIsBanknote50Yuan	= true;
	bhTable.bIsBanknote100Yuan	= true;
	// ��������
	CBHOpenAndValidate pBhCmd(bhTable);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
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
 @brief		ѹ�ݴ�

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Hold()
{
	// ��������
	CBHHold pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	theDeviceRegAR.AddBhMotorPressCount();
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ�����첽��

 @param		��

 @retval	��

 @exception	��
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
 @brief		ѹ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Collect()
{
	// ��������
		
	CBHCollect pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ��(MEI)

 @param		��

 @retval	��

 @exception	��
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
 @brief		�˻ػ�������ֽ�ң��첽��

 @param		��

 @retval	��

 @exception	��
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
 @brief		�˻ػ�������ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::ReturnCache(bool isCheckShutter)
{
	// ��������
	CBHReturnOne pBhCmd(isCheckShutter);
	/*CBHRef pBhCmd;*/
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

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
void CBanknoteModuleHelper::ReturnAllAsync(CService* pSvc)
{
	// �ж���ѯ����
	//this->Abort();

	// ��������
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
 @brief		�˻�ȫ��ֽ��

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::ReturnAll(bool isCheckShutter)
{
	// ��������
	CBHReturnAll pBhCmd(isCheckShutter);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

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
long CBanknoteModuleHelper::ReturnAll(BH_CMD_FEEDBACK& feedback)
{
	// ��������
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
 @brief		�����Ʊ

 @param		��

 @retval	long	�������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Clean(BH_CLEAN_DEST dest)
{
	// ��ʼ���
	CBHClearTrans pBhCmd(dest);
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ״̬

 @param		��

 @retval	long  �������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::GetStatus(BH_STATUS& status)
{
	
	long errCode = 0;

	// ��ȡ
	CBHGetSts stsCmd;
	errCode = stsCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	stsCmd.GetResponse(&status.bh_sts);

	status.bh_faceValue = VALUE_UNKNOWN; // MEI��ֻ�й�������ȡ��ֽ�ҵ����
	// ��ȡ��ֵ
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
 @brief		��ȡǮ��ID(MEI)

 @param		(o)BH_BOX_ID& box_id  Ǯ��ID

 @retval	long  �������

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::GetBoxID(BRH_BOX_ID& box_id)
{
	// ���ͻ�ȡ�汾����
	CBHGetBoxID pBhCmd;
	long errCode = pBhCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);
	pBhCmd.GetResponse(&box_id);

	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �쳣�޸�

@param      none

@retval     long ���ص��쳣����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::Repare(bool bMenualRepare)
{
	long errCode = 0;
	errCode = Abort();
	//errCode = Init();
	//errCode = Reset();
	
	// ���Թرմ��ں��ٴ�
	errCode = CheckAndOpenBHComport();

	// ��ȡ״̬�����Ƿ�����
	BRH_STS	pState;

	CBHGetSts bhStatus;
	errCode = bhStatus.ExecuteCommand();

	// ����ִ��ʧ��
	if(errCode != E_NO_ERROR){
		if(errCode == CBHException::ERROR_BH_RECEIVE_TIMEOUT)
			return Reset();
		INTERNAL_ERROR(errCode);
	}
	// ȡ״̬�ɹ�
	else{
		bhStatus.GetResponse(&pState);
		// ȡ״̬���ؽ���д���
		if(pState.errorCode != 0x30){
			// ����Ƿ����˱ҿ�̽����������������ʱֻ�ܽ���
			if(memcmp(pState.warning,"D4",2) == 0 && !bMenualRepare){
				throw CBHHardwareException(pState.TranslateWarningCode(),_T(__FILE__),__LINE__);
			}
			
			// ���Ŵ򿪣������˳���ʽ�����˿�
			if(memcmp(pState.warning,"D3",2) == 0 || memcmp(pState.warning,"D1",2) == 0 || memcmp(pState.warning,"D2",2) == 0 || memcmp(pState.warning,"D4",2) == 0){
				// �������˳���ʽ�ָ�ģ��(��������)
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
				// ������ѹ�뷽ʽ�ָ�ģ��(��������)
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
			
			// ����ָ�ʱ�����쳣����ʱ������������쳣
			if(errCode == BH_WORK_INVALID){
				return Reset();
			}
			// ��Ҫ���쳣
			errCode = pState.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

			CString strErrorCode;
			strErrorCode.Format(_T("ֽ��ģ�������룺%d"),errCode);
			throw CBHHardwareException(errCode,_T(__FILE__),__LINE__,strErrorCode);
		}
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
bool CBanknoteModuleHelper::IsHWException(long errCode)
{
	bool ret = false;

	switch(errCode)
	{
	case CBHException::SET_PARAM_ERR:			// �趨ֵ�������쳣
	case CBHException::FUNC_PARAM_ERR:			// ���������쳣
	case CBHException::SEQUENCE_ERR:			// ��������˳���쳣
	case CBHException::FUNC_ERR_RETURN:			// ���������쳣
	case CBHException::LOGIC_ERR:				// �߼��쳣
	case CBHException::OTHER_ERR:				// �쳣����
	case CBHException::ERROR_BH_OTHER:			// �����쳣
		ret = false;
		break;

	case CBHException::ERROR_BH_OPEN_FAILED:	// ��ʱ	���ڴ�ʧ��
	case CBHException::ERROR_BH_ALREADY_OPENED:	// ��ʱ	�����ظ���
	case CBHException::ERROR_BH_CLOSE_FAILED:	// �ر�ʱ	���ڹر�ʧ��
	case CBHException::ERROR_BH_ALREADY_CLOSED:	// �ر�ʱ	�����ظ��ر�
	case CBHException::ERROR_BH_NOT_OPEN:		// ͨ��ʱ	����δ��
	case CBHException::ERROR_BH_CMD:			// ͨ��ʱ	�����쳣
	case CBHException::ERROR_BH_SEND:			// ͨ��ʱ	�����쳣
	case CBHException::ERROR_BH_RECEIVE:		// ͨ��ʱ	�����쳣
	case CBHException::ERROR_BH_DESERIALIZE:	// ͨ��ʱ	�����ظ���Ϣ�쳣

	case ERR_BH_IN_TRANS:			// ����ϵͳ����ֽ��
	case ERR_BH_RETURN_ALL:			// �����˳�ʧ��
	case ERR_BH_RETURN_ONE:			// �����˳�ʧ��
	case ERR_BH_INSERT_JAM:			// Ͷ��ֽ��ʱ����
	case ERR_BH_JAM:				// �ݴ泮Ʊ����Ͳʱʧ�ܣ����������⿨��
	case ERR_BH_COLLECT:			// ѹǮ��ʧ��
	case ERR_BH_RETURN_SHUTTER:		// �˳�������ֽ��				�����棩
	case ERR_BH_INSERT_SHUTTER:		// ����������ֽ�ң��޷��˳�
	case ERR_BH_ALIGN:				// �޷������Ͳ
	case ERR_BH_NO_RECOGNIZER:		// ��������ֽ��ʶ����
	case ERR_BH_RECOGNIZING:		// ֽ��ʶ������������			�����棩
	case ERR_BH_FRAME:				// ��Ǯ���ͨ�ų�ʱ
	case ERR_BH_BOX:				// Ǯ���쳣����ֽ�Ҵ�Ǯ�����˳���
	case ERR_BH_NO_BOX:				// δװ��Ǯ���Ǯ��δ����
	case ERR_BH_FULL:				// Ǯ������
	case ERR_BH_NEARLY_FULL:		// Ǯ�佫��						�����棩
	case ERR_BH_HARDWARE:			// ��ָӲ������
		ret = true;
		break;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ֽ�����Ƿ���װ��������

 @param		

 @retval	bool  true:��װ��������;	false:δװ���δ����

 @exception	��
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
	// �������ΪC1ΪǮ�䲻��λ
	if(memcmp(status.bh_sts.warning,"C1",2) == 0)
	{
		return FALSE;
	}
	else
		return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ֽ�ҽ���ģ��������ϣ����˳���ʽ�޸��˹���(��������)

@param      ��

@retval     long 0 �ɹ�����0��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::TryRecoveryBanknoteModule(){
	
	long lRet = 0;

	try{
		// ȡֽ�ҽ���ģ��״̬
		CBHGetSts bhGetStatus;

		lRet = bhGetStatus.ExecuteCommand();

		if(0 != lRet){
			INTERNAL_ERROR(lRet);
		}
		else{
			// ȡ���
			BRH_STS bhStatus;
			bhGetStatus.GetResponse(&bhStatus);

			if(bhStatus.errorCode != 0x30){
				// �������˳���ʽ�ָ�ģ��(��������)
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

				// ��Ҫ���쳣
				lRet = bhStatus.TranslateWarningCode();//((pState.warning[0] & 0xFF) << 8) | (pState.warning[1] & 0xFF);

				CString strErrorCode;
				strErrorCode.Format(_T("ֽ��ģ�������룺%d"),lRet);
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
@brief      ��ֽ�ҽ���ģ�飬�Ѵ���رպ��ٴ򿪡�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBanknoteModuleHelper::CheckAndOpenBHComport(){
	long errCode = 0;

	int port = theSETTING.GetBHComPort() - 1;//theTVM_INFO.GetBHCommPort() - 1;
	int baud = theSETTING.GetBHBaudRate();   //theTVM_INFO.GetBHBaudRate();

	// �򿪴���
	CBHConnect pBhConnectCmd(port,baud,0);
	errCode = pBhConnectCmd.ExecuteCommand();
	if (errCode != E_NO_ERROR)
		INTERNAL_ERROR(errCode);

	return errCode;
}