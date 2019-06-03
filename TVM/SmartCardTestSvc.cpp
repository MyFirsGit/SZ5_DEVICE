#include "stdafx.h"
#include "SmartCardTestSvc.h"
#include "SmartCardTestMaintainDlg.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_COINTEST_MENU_TO_OPERATION = _T("WM_COINTEST_MENU_TO_OPERATION");
static const TCHAR* const WM_COINTEST_OPERATION_TO_MENU = _T("WM_COINTEST_OPERATION_TO_MENU");

static const TCHAR* const WM_COINTEST_MENU_TO_SENSOR = _T("WM_COINTEST_MENU_TO_SENSOR");
static const TCHAR* const WM_COINTEST_SENSOR_TO_MENU = _T("WM_COINTEST_SENSOR_TO_MENU");

static const TCHAR* const WM_COINTEST_MENU_TO_READ_RFID = _T("WM_COINTEST_MENU_TO_READ_RFID");
static const TCHAR* const WM_COINTEST_READ_RFID_TO_MENU = _T("WM_COINTEST_READ_RFID_TO_MENU");

static const TCHAR* const WM_COINTEST_MENU_TO_WRITE_RFID = _T("WM_COINTEST_MENU_TO_WRITE_RFID");
static const TCHAR* const WM_COINTEST_WRITE_RFID_TO_MENU = _T("WM_COINTEST_WRITE_RFID_TO_MENU");
BEGIN_MESSAGE_MAP(CSmardCardTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSmardCardTestSvc::CSmardCardTestSvc() :CTVMForeService(CARD_MODULE_TEST_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_TH_TEST_DLG, new CSmartCardTestMaintainDlg(this));

	//����TH���Ի�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TH_TEST, _opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TH_TEST, IDD_TH_TEST_DLG);

	//���õ�����Ի�����
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MOTOR_TEST, _opl(GUIDE_ENTER_F3_ESC));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MOTOR_TEST, IDD_TH_TEST_DLG);

	//���ö�RFID���Ի�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_READ_RFID_TEST, _opl(GUIDE_F3_F6_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_READ_RFID_TEST, IDD_TH_TEST_DLG);

	//����дRFID���Ի�����
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WRITE_RFID_TEST, _opl(GUIDE_ENTER_F3_ESC));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WRITE_RFID_TEST, IDD_TH_TEST_DLG);

	//���ô��������Ի�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SENSOR_TEST, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SENSOR_TEST, IDD_TH_TEST_DLG);

	//���û���Ťת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_OPERATION,DIALOG_GROUP_MOTOR_TEST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_MOTOR_TEST,WM_COINTEST_OPERATION_TO_MENU,DIALOG_GROUP_TH_TEST);

	//���û���Ťת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_READ_RFID,DIALOG_GROUP_READ_RFID_TEST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_READ_RFID_TEST,WM_COINTEST_READ_RFID_TO_MENU ,DIALOG_GROUP_TH_TEST);

	//���û���Ťת
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_WRITE_RFID ,DIALOG_GROUP_WRITE_RFID_TEST);
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_WRITE_RFID_TEST,WM_COINTEST_WRITE_RFID_TO_MENU ,DIALOG_GROUP_TH_TEST);

	//���û���Ťת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_SENSOR ,DIALOG_GROUP_SENSOR_TEST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_SENSOR_TEST,WM_COINTEST_SENSOR_TO_MENU ,DIALOG_GROUP_TH_TEST);

	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_TEST);

	//��ʼ��
	//m_wSmartCardFirstCommond  = TEST_NUM_UNKNOWN;
	//m_wSmartCardSecondCommand = TEST_NUM_UNKNOWN;
	////m_eTransferSpeedSet       = TEST_SPEED::NORMAL_SPEED;//�����ٶ�
	//memset(m_bIssueData,0,sizeof(m_bIssueData));         //��ʼ��
	//m_bIssueData[0]           = 1;                       //ֻ��AƱ�䷢Ʊ
	//m_bIssueData[1]           = theTVM_INFO.GetTHTestRelease();//3; //������Ʊ����
	//m_bIssueData[6]           = 1;
	//isConnect = FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSmardCardTestSvc::~CSmardCardTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ҵ��������Ĳ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::OnStart()
{
	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_TEST);

	//��ʼ��
	m_Modol = Modol();
	m_OperationType = OPERATION_TYPE_UNKNOW;
	//m_wSmartCardFirstCommond  = TEST_NUM_UNKNOWN;
	m_wSmartCardSecondCommand = -1;
	testType = -1;
	type_op = NULL_OP;
	////m_eTransferSpeedSet       = TEST_SPEED::NORMAL_SPEED;//�����ٶ�
	//memset(m_bIssueData,0,sizeof(m_bIssueData));         //��ʼ��
	//m_bIssueData[0]           = 1;                       //ֻ��AƱ�䷢Ʊ
	//m_bIssueData[1]           = theTVM_INFO.GetTHTestRelease();//3;//������Ʊ����
	//m_bIssueData[6]           = 1;

	// ����ģ��Ϊά��ģʽ
	try{
		CARD_HELPER->SetWorkMode(CMD_WORK_MODE_MAINTENANCE);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
	isMotorStart = FALSE;

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [ENTER]����������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSmardCardTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	// ����ģ�����������
	if (GetCurDialogGroupID() == DIALOG_GROUP_TH_TEST){
		int index = theAPP_SESSION.GetInputToInt();
		// �ǳ����û�ֻ��Ӧ���ò���
		if(index != MENU_CODE_1 && !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return FALSE;
		}
		switch (index){
		// [1] ����
		case MENU_CODE_1: 
			m_OperationType = OPERATION_TYPE_RETSET;
			break;
		// [2] ץȡƱ��  [1]A 
		case MENU_CODE_21:   
			m_OperationType = OPERATION_TYPE_CATCH_CARD;
			m_wSmartCardSecondCommand = 1;
			break;
		//     ץȡƱ��   [2]B
		case MENU_CODE_22:
			m_OperationType = OPERATION_TYPE_CATCH_CARD;
			m_wSmartCardSecondCommand = 2;
			break;
		//	[3] �ų�Ʊ��
		case MENU_CODE_3:
			m_OperationType = OPERATION_TYPE_ISSUE_CARD;
			break;
		// Ʊ��A����  
		case MENU_CODE_41: 
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 1;
			m_bActionType = 1;
			break;
		// Ʊ��A�½�
		case MENU_CODE_42:
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 1;
			m_bActionType = 2;
			break;
		// Ʊ��B����
		case MENU_CODE_43:
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 2;
			m_bActionType = 1;
			break;
		// Ʊ��B�½�
		case MENU_CODE_44:
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 2;
			m_bActionType = 2;
			break;
		//��Ʊ��RFID
		case MENU_CODE_5:
			m_OperationType = OPERATION_TYPE_READ_RFID;
			DoReadRFIDTestOperation();
			DoDialogFlow(WM_COINTEST_MENU_TO_READ_RFID);
			break;
		//дƱ��RFID
			/*case MENU_CODE_6:
			m_OperationType = OPERATION_TYPE_WRITE_RFID;
			ReadRFID();
			DoDialogFlow(WM_COINTEST_MENU_TO_WRITE_RFID);
			break;*/
		//Ʊ��A -> ��Ʊ��
		case MENU_CODE_61: 
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 1;m_bIsCardSendOut = true;
			break;
		// Ʊ��A -> ��Ʊ��
		case MENU_CODE_62:
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 1;m_bIsCardSendOut = false;
			break;
		//Ʊ��B -> ��Ʊ�� 
		case MENU_CODE_63:
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 2;m_bIsCardSendOut = true;
			break;
		// Ʊ��B -> ��Ʊ��
		case MENU_CODE_64:
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 2;m_bIsCardSendOut = false;
			break;
		//Ʊ��������[1]A  
		/*case MENU_CODE_81:
			m_OperationType = OPERATION_TYPE_MOVE_MOTOR_TEST;
			m_wSmartCardSecondCommand = 1;
			break;
		//Ʊ�������� [2]B
		case MENU_CODE_82:
			m_OperationType = OPERATION_TYPE_MOVE_MOTOR_TEST;
			m_wSmartCardSecondCommand = 2;
			break;*/
		//���ֵ������    ����תҳ�桿
		//case MENU_CODE_7:
		//	m_OperationType = OPERATION_TYPE_MOTOR_TEST;
		//	DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
		//	break;
		 //����������
		case MENU_CODE_7:
			m_OperationType = OPERATION_TYPE_SENSOR_TEST;
			DoSensorTestOperation();
			DoDialogFlow(WM_COINTEST_MENU_TO_SENSOR);
			break;
		/*case MENU_CODE_11:
			m_OperationType = OPERATION_TYPE_ELECTROMAGNET_TEST;
			break;
		case MENU_CODE_12:
			m_OperationType = OPERATION_TYPE_CYCLE_TEMP_TICKS;
			break;*/
		case MENU_CODE_8:
			m_OperationType = OPERATION_TYPE_CYCLE_TEST;
			break;
		default:
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				return FALSE;
			}
			break;
		}
		//��������
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		//����ѡ�����Ӧ�Ĳ���
		DoOperation();
	}
	// 
	else if (GetCurDialogGroupID() == DIALOG_GROUP_MOTOR_TEST){
		int index = theAPP_SESSION.GetInputToInt();
		DoMotorTestOperation();
		//��������
		if(index <10){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
	}
	//else if (GetCurDialogGroupID() == DIALOG_GROUP_READ_RFID_TEST)
	//{
	//	DoReadRFIDTestOperation();
	//	//��������
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//}
	// RFID���Խ���
	/*else if (GetCurDialogGroupID() == DIALOG_GROUP_WRITE_RFID_TEST)	{
		DoWriteRFIDTestOperation();
	}*/
	else{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	}
	//���½�����ʾ
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSmardCardTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MOTOR_TEST)
	{
		//������δֹͣ������ֹͣ
		if(isMotorStart)
		{
			try
			{
				TH_MOTORTEST_DATA motor;
				isMotorStart = false;
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			catch(CSysException& e)
			{
				// Cardģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...)
			{
				// Cardģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
			}
		}
		DoDialogFlow(WM_COINTEST_OPERATION_TO_MENU);
	}
	else if(GetCurDialogGroupID() == DIALOG_GROUP_READ_RFID_TEST)
	{
		DoDialogFlow(WM_COINTEST_READ_RFID_TO_MENU);
	}
	/*else if(GetCurDialogGroupID() == DIALOG_GROUP_WRITE_RFID_TEST)
	{
		DoDialogFlow(WM_COINTEST_WRITE_RFID_TO_MENU);
	}*/
	else if(GetCurDialogGroupID() == DIALOG_GROUP_SENSOR_TEST)
	{
		DoDialogFlow(WM_COINTEST_SENSOR_TO_MENU);
	}
	else
	{
		// �ָ�ģʽΪ����ģʽ
		try{
			CARD_HELPER->SetWorkMode(CMD_WORK_MODE_NORMAL);
		}
		catch(...){

		}
		theAPP_SESSION.ReturnMaintainMenu();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSmardCardTestSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam)
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MOTOR_TEST)
	{
		//������δֹͣ������ֹͣ
		if(isMotorStart)
		{
			try
			{
				TH_MOTORTEST_DATA motor;
				isMotorStart = false;
				motor.Switch = 2;
				motor.testType = testType;
//				CARD_HELPER->MotorTest(motor);
			}
			catch(CSysException& e)
			{
				// Cardģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...)
			{
				// Cardģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
			}
		}
	}
	__super::OnKeyboardEsc(wParam,lParam);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [0~9]����������Ϣ

@param      (i)WPARAM wParam  ���ּ�ֵ�����磺���ּ�7��ֵΪ7
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CSmardCardTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����ģ������

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSmardCardTestSvc::Modol& CSmardCardTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ݸı��֪ͨ�����������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Դ�����

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoOperation()
{
	theSERVICE_MGR.SetForeServiceBusy(true);
	bool bIsStart = false;
	long errCode = 0;
	int nMenuNum = 0;
	theAPP_SESSION.ShowOfficeGuide(_T("���ڲ�����..."));
	bool bSuccess = true;
	try{		
		//1.���ò���
		if (m_OperationType == OPERATION_TYPE_RETSET){
			nMenuNum = 0;
			//�ȸ�λ����λʧ�ܶϿ���������
			try{
				UINT uiRecycleNum = 0;
				CARD_HELPER->Init(CMD_INIT_FAREBOX,&uiRecycleNum);
				//CARD_HELPER->EnableWork(TH_WORK_FLAG::TH_BEGIN_WORK);
			}
			catch(CSysException& e){
				bSuccess = false;
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				bSuccess = false;
			}
			
			m_Modol.menuInfo[nMenuNum].strMenuStatus = bSuccess ? SUCCESS : FAILED;
			
			if(!bSuccess)
				theAPP_SESSION.ShowOfficeGuide(_T("���죩����ģ������ʧ��!"));
		}
		else if (m_OperationType == OPERATION_TYPE_CATCH_CARD) // THץȡƱ������
		{
			nMenuNum = 1;
			bSuccess = false;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			// ���ж϶������Ƿ���Ʊ
			if(statusRsp.bCardInReadArea){
				theAPP_SESSION.ShowOfficeGuide(_T("��������Ʊ����ʱ�޷�ץȡƱ����"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;m_bIsCardCatch = true;
			}
			else if(m_wSmartCardSecondCommand == 1 && statusRsp.bBoxAEmpty){// ץȡƱ��1������û�п�
				theAPP_SESSION.ShowOfficeGuide(_T("Ʊ��A�ѿգ��޷�ץȡƱ����"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;m_bIsCardCatch = false;
			}
			else if(m_wSmartCardSecondCommand == 2 && statusRsp.bBoxBEmpty){
				theAPP_SESSION.ShowOfficeGuide(_T("Ʊ��B�ѿգ��޷�ץȡƱ����"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;m_bIsCardCatch = false;
			}
			else{
				//CARD_HELPER->SetWorkMode(TH_CMD_SET_WORK_MODE::CMD_WORK_MODE_NORMAL);
				TH_CMD_CARD_OUT OutCardBox;// = (TH_CMD_CARD_OUT)m_wSmartCardSecondCommand;
				if(m_wSmartCardSecondCommand == 1){
					OutCardBox = CMD_CARD_OUT_BOX_A;
				}
				else{
					OutCardBox = CMD_CARD_OUT_BOX_B;
				}
				
				UINT uiSendOutCount = 0;
				CARD_HELPER->CardOut(OutCardBox,&uiSendOutCount);m_bIsCardCatch = true;
				// ���¼���
				CTCardCountInfo::CARD_BOX_INFO cardAInfo = theCARD_COUNT.GetCardboxAInfo();
				CTCardCountInfo::CARD_BOX_INFO cardBInfo = theCARD_COUNT.GetCardboxBInfo();
				if(m_wSmartCardSecondCommand == 1 && cardAInfo.ticketBoxID.ToString() != INVALID_ID && cardAInfo.ulCurCount >0){
					cardAInfo.ulCurCount -= uiSendOutCount;
					theCARD_COUNT.SetCardboxAInfo(cardAInfo);
				}
				else if(m_wSmartCardSecondCommand == 2  && cardBInfo.ticketBoxID.ToString() != INVALID_ID && cardBInfo.ulCurCount >0){
					cardBInfo.ulCurCount -= uiSendOutCount;
					theCARD_COUNT.SetCardboxBInfo(cardBInfo);
				}
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				bSuccess = true;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_ISSUE_CARD)// TH�ų�Ʊ������/����Ʊ������
		{
			nMenuNum = 2;
			bSuccess = false;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			// ���ж϶������Ƿ���Ʊ
			if(statusRsp.bCardInReadArea && m_bIsCardCatch){
				UINT uiTransportNum = 0;
				CARD_HELPER->SendCard(&uiTransportNum);
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				bSuccess = true;
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_T("��������Ʊ����δ��ץȡƱ���޷��ų���Ʊ��"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_MOVE_BOX)// TH����Ʊ��
		{
			nMenuNum = 3;
			bSuccess = false;
			//theAPP_SESSION.ShowOfficeGuide(_T("��ǰ�豸��֧��Ʊ������������"));
			/*SC_MOVE_BOX_CMD_INFO move;
			move.boxId = m_wSmartCardSecondCommand;
			move.actionType = m_bActionType;*/
			TH_CMD_TEST_SETTING testType;
			if(m_wSmartCardSecondCommand == 1 && m_bActionType == 1){
				testType = CMD_TEST_A_TRAY_UP;
			}
			else if(m_wSmartCardSecondCommand == 1 && m_bActionType == 2){
				testType = CMD_TEST_A_TRAY_DOWN;
			}
			else if(m_wSmartCardSecondCommand == 2 && m_bActionType == 1){
				testType = CMD_TEST_B_TRAY_UP;
			}
			else if(m_wSmartCardSecondCommand == 2 && m_bActionType == 2){
				testType = CMD_TEST_B_TRAY_DOWN;
			}
			else{
				testType = CMD_TEST_STOP;
			}
			// �ж�����ʱ���ǰ���Ҫ�򿪣�������ʾ����
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			if(!statusRsp.bBoxACoverArrive && m_wSmartCardSecondCommand == 1 && m_bActionType == 1){
				theAPP_SESSION.ShowOfficeGuide(_T("Ʊ��A�ǰ�δ�򿪣����ܽ�������������"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else if(!statusRsp.bBoxBCoverArrive && m_wSmartCardSecondCommand == 2 && m_bActionType == 1){
				theAPP_SESSION.ShowOfficeGuide(_T("Ʊ��B�ǰ�δ�򿪣����ܽ�������������"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else{
				CARD_HELPER->TestSetting(testType);m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				bSuccess = true;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_ISSUE_TEST)// TH��Ʊ��������
		{
			nMenuNum = 7;
			bSuccess = false;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);

			if(statusRsp.bBoxAEmpty && m_wSmartCardSecondCommand == 1){
				theAPP_SESSION.ShowOfficeGuide(_T("Ʊ��A�ѿգ��޷���Ʊ��"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else if(statusRsp.bBoxBEmpty && m_wSmartCardSecondCommand == 2){
				theAPP_SESSION.ShowOfficeGuide(_T("Ʊ��B�ѿգ��޷���Ʊ��"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else{
				//CARD_HELPER->EnableWork(TH_WORK_FLAG::TH_BEGIN_WORK);
				// ��ץȡһ�ſ�
				TH_CMD_CARD_OUT OutCardBox ;//= (TH_CMD_CARD_OUT)m_wSmartCardSecondCommand;
				if(m_wSmartCardSecondCommand == 1){
					OutCardBox = CMD_CARD_OUT_BOX_A;
				}
				else{
					OutCardBox = CMD_CARD_OUT_BOX_B;
				}
				
				UINT uiTransportNum = 0;
				long lRet = CARD_HELPER->CardOut(OutCardBox,&uiTransportNum);;

				// ���¼���
				CTCardCountInfo::CARD_BOX_INFO cardAInfo = theCARD_COUNT.GetCardboxAInfo();
				CTCardCountInfo::CARD_BOX_INFO cardBInfo = theCARD_COUNT.GetCardboxBInfo();
				if(m_wSmartCardSecondCommand == 1 && cardAInfo.ticketBoxID.ToString() != INVALID_ID && cardAInfo.ulCurCount >0){
					cardAInfo.ulCurCount -= uiTransportNum;
					theCARD_COUNT.SetCardboxAInfo(cardAInfo);
				}
				else if(m_wSmartCardSecondCommand == 2  && cardBInfo.ticketBoxID.ToString() != INVALID_ID && cardBInfo.ulCurCount >0){
					cardBInfo.ulCurCount -= uiTransportNum;
					theCARD_COUNT.SetCardboxBInfo(cardBInfo);
				}

				// �ų�Ʊ��
				if(0 == lRet){
					UINT uiTransportNum = 0;
					lRet = m_bIsCardSendOut ? CARD_HELPER->SendCard(&uiTransportNum) : CARD_HELPER->RetractCard(&uiTransportNum);

					// ����Ʊ����Ҫ���Ӽ���
					if(!m_bIsCardSendOut){
						// �����յ�Ʊ��Ҫ�����Ʊ����
						CTCardCountInfo::CARD_BOX_INFO cardInfo = theCARD_COUNT.GetCardboxWastedInfo();
						cardInfo.ulCurCount += uiTransportNum;
						theCARD_COUNT.SetCardboxWastedInfo(cardInfo);

						// ��Ҫ���ɷ�Ʊ����
						INVALIDE_TICKET_RECYCLETRADE cardInvalid;
						// ����Ͽ�����
						cardInvalid.reson = 0x02;// ����Ʊ��
						cardInvalid.cardPhysicalType = ULTRALIGHT;
						// ����
						//memset(cardInvalid.cardSerialNo, 0, sizeof(cardInvalid.cardSerialNo));
						cardInvalid.cardSerialNo = 0;
						// ������
						cardInvalid.issueId		 = 0x0002;//ACC
						// ��Ʊ��ID
						//cardInvalid.magazineId.bDeviceType = cardInfo.ticketBoxID.bDeviceType;
						//cardInvalid.magazineId.bType = cardInfo.ticketBoxID.bType;
						//cardInvalid.magazineId.bNum = cardInfo.ticketBoxID.bNum;
						//cardInvalid.magazineId.bStationID = cardInfo.ticketBoxID.bStationID;

						cardInvalid.productType		= 0x6200;

						theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(cardInvalid);
					}
					m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
					bSuccess = true;
				}
				else m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_MOVE_MOTOR_TEST)// ���������������
		{
			nMenuNum = 7;
			//CARD_HELPER->MoveMotorTest(m_wSmartCardSecondCommand);
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		}
		else if (m_OperationType == OPERATION_TYPE_ELECTROMAGNET_TEST)// ���������
		{
			nMenuNum = 10;
			//CARD_HELPER->ElectromagnetTest();
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		}
		else if(m_OperationType == OPERATION_TYPE_CYCLE_TEMP_TICKS){	// ������Ʊ������
			nMenuNum = 9;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			// �ж϶������Ƿ���Ʊ
			if(statusRsp.bCardInReadArea){
				UINT uiTransportNum = 0;
				CARD_HELPER->RetractCard(&uiTransportNum);;
			

				// �����յ�Ʊ��Ҫ�����Ʊ����
				CTCardCountInfo::CARD_BOX_INFO cardInfo = theCARD_COUNT.GetCardboxWastedInfo();
				cardInfo.ulCurCount += uiTransportNum;
				theCARD_COUNT.SetCardboxWastedInfo(cardInfo);

				// ��Ҫ���ɷ�Ʊ����
				INVALIDE_TICKET_RECYCLETRADE cardInvalid;
				// ����Ͽ�����
				cardInvalid.reson = 0x02;// ����Ʊ��
				cardInvalid.cardPhysicalType = ULTRALIGHT;
				// ����
				//memset(cardInvalid.cardSerialNo, 0, sizeof(cardInvalid.cardSerialNo));
				cardInvalid.cardSerialNo = 0;
				// ������
				cardInvalid.issueId		 = 0x0002;//ACC
				// ��Ʊ��ID
				//cardInvalid.magazineId.bDeviceType = cardInfo.ticketBoxID.bDeviceType;
				//cardInvalid.magazineId.bType = cardInfo.ticketBoxID.bType;
				//cardInvalid.magazineId.bNum = cardInfo.ticketBoxID.bNum;
				//cardInvalid.magazineId.bStationID = cardInfo.ticketBoxID.bStationID;
				//cardInvalid.productType		= 0x6200;

				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(cardInvalid);
			}
			else{
				bSuccess = false;
				theAPP_SESSION.ShowOfficeGuide(_T("��������Ʊ�������ܽ��л��ղ�����"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		// Ʊ�����ղ���
		else if(m_OperationType == OPERATION_TYPE_CYCLE_TEST){
			CString strMsg;
			bool bSuccessBoxA = true,bSuccessBoxB = true;
			// �ȴ�A����һ�ų�Ʊ��Ȼ������Ʊ��
			try{
				// ץȡƱ��
				long lRet = 0;
				UINT uiTransportNum = 0;

				lRet = CARD_HELPER->CardOut(CMD_CARD_OUT_BOX_A,&uiTransportNum);	// ����ץƱ��������˼���ץȡ��һ��Ʊ��
				
				if(lRet == 0){
					UINT uiRetractBoxNum = 0;
					CARD_HELPER->RetractCard(&uiRetractBoxNum);// ����Ʊ��
				}
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
				bSuccessBoxA = false;
				bSuccess = false;
			}
			catch(...){
				bSuccessBoxA = false;
				bSuccess = false;
			}

			// ��B����һ�ų�Ʊ��Ȼ������Ʊ��
			try{
				// ץȡƱ��
				long lRet = 0;
				UINT uiTransportNum = 0;

				lRet = CARD_HELPER->CardOut(CMD_CARD_OUT_BOX_B,&uiTransportNum);	// ����ץƱ��������˼���ץȡ��һ��Ʊ��
				
				if(lRet == 0){
					UINT uiRetractBoxNum = 0;
					CARD_HELPER->RetractCard(&uiRetractBoxNum);// ����Ʊ��
				}
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
				bSuccessBoxB = false;
				bSuccess = false;
			}
			catch(...){
				bSuccessBoxB = false;
				bSuccess = false;
			}


			if(bSuccessBoxA && bSuccessBoxB){
				strMsg = _T("Ʊ�����ղ��Գɹ���");
				m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			}
			else if(bSuccessBoxA && !bSuccessBoxB){
				strMsg = _T("Ʊ��AƱ�����ղ���ʧ�ܣ�");
				m_Modol.menuInfo[9].strMenuStatus = FAILED;
			}
			else if(!bSuccessBoxA && bSuccessBoxB){
				strMsg = _T("Ʊ��BƱ�����ղ���ʧ�ܣ�");
				m_Modol.menuInfo[9].strMenuStatus = FAILED;
			}
			else{
				strMsg = _T("Ʊ�����ղ���ʧ�ܣ�");
				m_Modol.menuInfo[9].strMenuStatus = FAILED;
			}

			theAPP_SESSION.ShowOfficeGuide(strMsg);
			
		}

		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_SUCCESS_NEW);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,true);
	}
	catch(CSysException& e)
	{
		bSuccess = false;
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_T("����ģ�����ʧ�ܣ������豸״̬��"));
	}
	catch(...){
		bSuccess = false;
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		theAPP_SESSION.ShowOfficeGuide(_T("����ģ�����ʧ�ܣ������豸״̬��"));
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
	if (bIsStart)
	{
		try
		{
			//CARD_HELPER->EndMaintenanceModeCMD();
		}
		catch (CSysException& e)
		{
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			// Cardģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
			theEXCEPTION_MGR.ProcessException(e);
		}
	}
	theSERVICE_MGR.SetForeServiceBusy(false);

	if(bSuccess)
		theAPP_SESSION.ShowOfficeGuide(_T("������ɣ�������Խ����"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Դ�����

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoMotorTestOperation()
{
	int index = theAPP_SESSION.GetInputToInt();
	int nMenuNum = 0;
	try
	{
		TH_MOTORTEST_DATA motor;
		switch (index)
		{
		case 1://ֹͣ��ǰ���
			nMenuNum = 0;
			isMotorStart = false;
			/*motor.Switch = 2;
			motor.testType = testType;
			//CARD_HELPER->MotorTest(motor);*/
			CARD_HELPER->TestSetting(CMD_TEST_STOP);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 2://1����Ʊ��1 ���������ת
			if (isMotorStart)
			{
//				motor.Switch = 2;
//				motor.testType = testType;
//				CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 1; /*motor.testType = testType =*/;
			CARD_HELPER->TestSetting(CMD_TEST_A_TRAY_UP);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 3:// 2����Ʊ��1 ���������ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 2;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 2;
			CARD_HELPER->TestSetting(CMD_TEST_A_TRAY_DOWN);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 4:// 3����Ʊ��2 ���������ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 3;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 3;
			CARD_HELPER->TestSetting(CMD_TEST_B_TRAY_UP);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 5:// 4����Ʊ��2 ���������ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 4;
			//CARD_HELPER->MotorTest(motor);

			nMenuNum = 4;
			CARD_HELPER->TestSetting(CMD_TEST_B_TRAY_DOWN);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 6:// 5����Ʊ��1 ��Ʊ�����ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 5;
			//CARD_HELPER->MotorTest(motor);

			nMenuNum = 5;
			CARD_HELPER->TestSetting(CMD_TEST_TICKSCRUB_MOTOR_BOXA);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 7:// 6����Ʊ��1 ��Ʊ�����ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 6;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 6;
			CARD_HELPER->TestSetting(CMD_TEST_TICKSCRUB_MOTOR_BOXB);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 8:// 7����Ʊ��2 ��Ʊ�����ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 7;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 7;
			CARD_HELPER->TestSetting(CMD_TEST_TRANSTICK_MOTOR);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 9:// 8����Ʊ��2 ��Ʊ�����ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 8;
			//CARD_HELPER->MotorTest(motor);

			nMenuNum = 8;
			CARD_HELPER->TestSetting(CMD_TEST_CHANNEL_CLEAN);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
/*		case 10:// 9�����ݴ�����ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 9;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;
		case 11:// 10�����ݴ�����ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 10;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;
		case 12:// 11���������������ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 11;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;
		case 13:// 12���������������ת
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 12;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;*/
		default:
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			break;
		}
	}
	catch(CSysException& e)
	{
		m_Modol.menuInfo[nMenuNum].motorStatus = FAILED;
		m_Modol.menuInfo[7].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		m_Modol.menuInfo[nMenuNum].motorStatus = FAILED;
		m_Modol.menuInfo[7].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��RFDI���Դ�����

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoReadRFIDTestOperation()
{
	int nMenuNum = 0;
	TH_BOX_TYPE boxType;
	tTIMRFIDInfo rfidInfo;
	int indexMenu = 5;
	try{
		nMenuNum = 0;
		boxType = TH_BOX_TYPE::TH_BOX_A;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = boxID.ToString();//.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0],res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}

	try{
		nMenuNum = 1;
		boxType = TH_BOX_TYPE::TH_BOX_B;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = boxID.ToString();//.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0],res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		//m_Modol.menuInfo[nMenuNum].read_rfidStatus.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}

	try{
		nMenuNum = 2;
		boxType = TH_BOX_TYPE::TH_BOX_C;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = boxID.ToString();//.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0],res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		//m_Modol.menuInfo[nMenuNum].read_rfidStatus.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;	
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��RFID����

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::ReadRFID()
{
	int nMenuNum = 0;
	try{
		TH_BOX_TYPE boxType;
		tTIMRFIDInfo rfidInfo;

		boxType = TH_BOX_TYPE::TH_BOX_A;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].writeID = boxID.ToString();
		m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),((rfidInfo.ucCardNum[1] << 8 ) | rfidInfo.ucCardNum[0]));
		//m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);

		nMenuNum = 1;
		boxType = TH_BOX_TYPE::TH_BOX_B;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		//CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].writeID = boxID.ToString();
		m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),((rfidInfo.ucCardNum[1] << 8 ) | rfidInfo.ucCardNum[0]));
		//m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);

		nMenuNum = 2;
		boxType = TH_BOX_TYPE::TH_BOX_C;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		//CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].writeID = boxID.ToString();
		m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),((rfidInfo.ucCardNum[1] << 8 ) | rfidInfo.ucCardNum[0]));
		//m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		/*nMenuNum = 3;
		res.boxType = RFID_MAGAZINE_A_B;
		CARD_HELPER->ReadTicketBoxRFID(res);
		m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%04x"),res.RFIDData.byBoxID[0]
		,res.RFIDData.byBoxID[1],(res.RFIDData.byBoxID[2]<<8) + res.RFIDData.byBoxID[3]);*/
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      дRFID����

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoWriteRFIDTestOperation()
{
	//int index = theAPP_SESSION.GetInputToInt();
	//int nMenuNum = 0;
	////SC_CMD_SET_RFID cmd;
	//int indexMenu = 6;
	//SC_RES_SET_RFID res;
	//WORD wNum = 0;
	//WORD wInput = 0;
	//CString str;
	//try{
	//	switch(type_op)	{
	//	case NULL_OP: {
	//			switch (index){
	//			case 1://Ʊ��A
	//				type_op = MenuA;
	//				theAPP_SESSION.SetPrefixToGuide(_opl(GUIDE_TH_TEST_TEXT_INPUT_ID) + m_Modol.strPreBoxID);
	//				theAPP_SESSION.SetMaxInputLength(4);
	//				break;
	//			case 2://Ʊ��B
	//				type_op = MenuB;
	//				theAPP_SESSION.SetPrefixToGuide(_opl(GUIDE_TH_TEST_TEXT_INPUT_ID) + m_Modol.strPreBoxID);
	//				theAPP_SESSION.SetMaxInputLength(4);
	//				break;
	//			case 3:// Ʊ��C
	//				type_op = MenuC;
	//				theAPP_SESSION.SetPrefixToGuide(_opl(GUIDE_TH_TEST_TEXT_INPUT_ID) + m_Modol.strPreInvalidID);
	//				theAPP_SESSION.SetMaxInputLength(4);
	//				break;
	//			default:
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//				break;
	//			}
	//		}
	//		break;
	//	 // ����Ʊ��A��RFID����λ
	//	case MenuA:
	//		// ����ID���Ϸ�
	//		if(index == 0){
	//			theAPP_SESSION.SetPrefixToGuide(_T("����Ʊ��ID���Ϸ�������������:") + m_Modol.strPreBoxID);
	//			break;
	//		}
	//		// ����Ʊ��ID����λ�Ϸ�
	//		else{
	//			nMenuNum = 0;
	//			wInput = (index % 10000) & 0xFFFF;
	//			wNum = wInput;
	//			//int2BCD(wInput,(char*)&wNum,2);// BCD: 1234 -> 0x1234 | HEX: 1234 -> 0x3412
	//			m_cmd.magazine = RFID_MAGAZINE_A;
	//			int2BCD(theMAINTENANCE_INFO.GetLineCode(),(char*)m_rfidInfo.ucBoxID,1);	// BCD 1
	//			m_rfidInfo.ucBoxID[1] = 0x01;								// BCD 1
	//			m_rfidInfo.ucBoxID[2] = HIBYTE(wNum);						// HEX 2
	//			m_rfidInfo.ucBoxID[3] = LOBYTE(wNum);
	//			str.Format(_T("%02d01%04d"),theMAINTENANCE_INFO.GetLineCode(),wInput);
	//			//��֤3��Ʊ���RFID��ͬ
	//			if(str == m_Modol.menuInfo[1].writeID || str == m_Modol.menuInfo[2].writeID ){
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_ID_REPEAT);
	//				type_op = NULL_OP;
	//				break;
	//			}
	//			m_Modol.menuInfo[nMenuNum].writeID = str;
	//			type_op = NumA;
	//			theAPP_SESSION.SetPrefixToGuide(_T("������Ʊ��A������:"));
	//			theAPP_SESSION.SetMaxInputLength(3);
	//		}			
	//		break;
	//	// ����Ʊ��B��RFID����λ
	//	case MenuB:
	//		// ����ID���Ϸ�
	//		if(index == 0){
	//			theAPP_SESSION.SetPrefixToGuide(_T("����Ʊ��ID���Ϸ�������������:") + m_Modol.strPreBoxID);
	//			break;
	//		}
	//		else{
	//			nMenuNum = 1;
	//			wInput = (index % 10000) & 0xFFFF;
	//			wNum = wInput;
	//			//int2BCD(wInput,(char*)&wNum,2);// BCD: 1234 -> 0x1234 | HEX: 1234 -> 0x3412
	//			m_cmd.magazine = RFID_MAGAZINE_B;
	//			int2BCD(theMAINTENANCE_INFO.GetLineCode(),(char*)m_rfidInfo.ucBoxID,1);	// BCD 1
	//			m_rfidInfo.ucBoxID[1] = 0x01;								// BCD 1
	//			m_rfidInfo.ucBoxID[2] = HIBYTE(wNum);						// HEX 2
	//			m_rfidInfo.ucBoxID[3] = LOBYTE(wNum);
	//			str.Format(_T("%02d01%04d"),theMAINTENANCE_INFO.GetLineCode(),wInput);
	//			if(str == m_Modol.menuInfo[0].writeID || str == m_Modol.menuInfo[2].writeID ){
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_ID_REPEAT);
	//				type_op = NULL_OP;
	//				break;
	//			}
	//			m_Modol.menuInfo[nMenuNum].writeID = str;
	//			type_op = NumB;
	//			theAPP_SESSION.SetPrefixToGuide(_T("������Ʊ��B������:"));
	//			theAPP_SESSION.SetMaxInputLength(3);				
	//		}
	//		break;
	//	// �����Ʊ���RFID����λ
	//	case MenuC:
	//		// ID�Ϸ���
	//		if(index == 0){
	//			theAPP_SESSION.SetPrefixToGuide(_T("����Ʊ��ID���Ϸ�������������:") + m_Modol.strPreInvalidID);
	//			break;
	//		}
	//		nMenuNum = 2;
	//		wInput = (index % 10000) & 0xFFFF;
	//		wNum = wInput;
	//		//int2BCD(wInput,(char*)&wNum,2);// BCD: 1234 -> 0x1234 | HEX: 1234 -> 0x3412
	//		TH_BOX_TYPE boxType;
	//		tTIMRFIDInfo rfidInfo;
	//		boxType = TH_BOX_TYPE::TH_BOX_C;
	//		int2BCD(theMAINTENANCE_INFO.GetLineCode(),(char*)rfidInfo.ucBoxID,1);	// BCD 1
	//		rfidInfo.ucBoxID[1] = 0x02;									// BCD 1
	//		rfidInfo.ucBoxID[2] = HIBYTE(wNum);							// HEX 2
	//		rfidInfo.ucBoxID[3] = LOBYTE(wNum);
	//		str.Format(_T("%02d02%04d"),theMAINTENANCE_INFO.GetLineCode(),wInput);
	//		if(str == m_Modol.menuInfo[0].writeID || str == m_Modol.menuInfo[1].writeID){
	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_ID_REPEAT);
	//			type_op = NULL_OP;
	//			break;
	//		}
	//		// ����Ϊ0
	//		memset(rfidInfo.ucCardNum,0x00,sizeof(rfidInfo.ucCardNum));			
	//		CARD_HELPER->WriteTicketBoxRFID(boxType,&rfidInfo);
	//		m_Modol.menuInfo[nMenuNum].writeID = str;
	//		m_Modol.menuInfo[nMenuNum].write_rfidStatus = SUCCESS;
	//		type_op = NULL_OP;
	//		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_SUCCESS);
	//		break;
	//	// ����Ʊ��A������	
	//	case NumA:	
	//		{
	//			WORD wTickCount = (WORD)index;
	//			// ����Ʊ��A������
	//			if(wTickCount > 1000 || wTickCount <= 0){
	//				theAPP_SESSION.SetPrefixToGuide(_T("������������Ϸ�������������:"));
	//				break;
	//			}
	//			else{
	//				nMenuNum = 0;
	//				//m_cmd.RFIDData.sTicketCount = wTickCount;
	//				TH_BOX_TYPE boxType;
	//				boxType = TH_BOX_TYPE::TH_BOX_A;
	//				//tTIMRFIDInfo rfidInfo;
	//				memcpy(m_rfidInfo.ucCardNum,&wTickCount,2);
	//				CARD_HELPER->WriteTicketBoxRFID(boxType,&m_rfidInfo);
	//				//m_Modol.menuInfo[nMenuNum].writeID = str;
	//				m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),wTickCount);
	//				m_Modol.menuInfo[nMenuNum].write_rfidStatus = SUCCESS;
	//				type_op = NULL_OP;
	//				m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_SUCCESS);
	//			}			
	//			break;
	//		}
	//	case NumB:	
	//		{
	//			WORD wTickCount = (WORD)index;
	//			// ����Ʊ��B������
	//			if(wTickCount > 1000 || wTickCount <= 0){
	//				theAPP_SESSION.SetPrefixToGuide(_T("������������Ϸ�������������:"));
	//			}
	//			else{
	//				nMenuNum = 1;
	//				m_cmd.RFIDData.sTicketCount = wTickCount;
	//				TH_BOX_TYPE boxType;
	//				boxType = TH_BOX_TYPE::TH_BOX_B;
	//				//tTIMRFIDInfo rfidInfo;
	//				memcpy(m_rfidInfo.ucCardNum,&wTickCount,2);
	//				CARD_HELPER->WriteTicketBoxRFID(boxType,&m_rfidInfo);
	//				//m_Modol.menuInfo[nMenuNum].writeID = str;
	//				m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),wTickCount);
	//				m_Modol.menuInfo[nMenuNum].write_rfidStatus = SUCCESS;
	//				type_op = NULL_OP;
	//				m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_SUCCESS);
	//			}	
	//			break;
	//		}						
	//	default:
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//		break;
	//	}
	//}
	//catch(CSysException& e){
	//	m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
	//	m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_FAIL);
	//	// Cardģ�����--������־
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch(...){
	//	m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
	//	m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_FAIL);
	//	// Cardģ�����--������־
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	//}
}

void CSmardCardTestSvc::DoSensorTestOperation()
{
	try
	{
		//SC_RSP_SENSOR_SWITCH_TEST rsp;
		//CARD_HELPER->SensorTest(rsp);
		m_Modol.cardSenserStatus = _th_rsp_get_dev_status();
		CARD_HELPER->GetDevStatus(&m_Modol.cardSenserStatus);
		m_Modol.bCardSenserTestSuccess = true;
		m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
		/*m_Modol.INFS1Covered = rsp.INFS1Covered;
		m_Modol.INFS2Covered = rsp.INFS2Covered;
		m_Modol.INFS3Covered = rsp.INFS3Covered;
		m_Modol.INFS4Covered = rsp.INFS4Covered;
		m_Modol.INFS5Covered = rsp.INFS5Covered;
		m_Modol.INFS6Covered = rsp.INFS6Covered;*/
	}
	catch(CSysException& e)
	{
		m_Modol.menuInfo[8].strMenuStatus = FAILED;
		m_Modol.bCardSenserTestSuccess = false;
		/*m_Modol.INFS1Covered = -1;
		m_Modol.INFS2Covered = -1;
		m_Modol.INFS3Covered = -1;
		m_Modol.INFS4Covered = -1;
		m_Modol.INFS5Covered = -1;
		m_Modol.INFS6Covered = -1;*/
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		m_Modol.bCardSenserTestSuccess = false;
		m_Modol.menuInfo[8].strMenuStatus = FAILED;
		/*m_Modol.INFS1Covered = -1;
		m_Modol.INFS2Covered = -1;
		m_Modol.INFS3Covered = -1;
		m_Modol.INFS4Covered = -1;
		m_Modol.INFS5Covered = -1;
		m_Modol.INFS6Covered = -1;*/
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Cardģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}