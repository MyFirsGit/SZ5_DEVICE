#include "stdafx.h"
#include "IOModuleTestSvc.h"
#include "IOModuleTestMaintainDlg.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_IO_TEST_MENU_TO_OPERATION = _T("WM_IO_TEST_MENU_TO_OPERATION");
static const TCHAR* const WM_IO_TEST_OPERATION_TO_MENU = _T("WM_IO_TEST_OPERATION_TO_MENU");

BEGIN_MESSAGE_MAP(CIOModuleTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestSvc::CIOModuleTestSvc()
	:CTVMForeService(IO_MODULE_TEST_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CIOModuleTestMaintainDlg(this));

	//����IO���Ի�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_IO_TEST, _opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_IO_TEST, IDD_20113_STATION_SET_END_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_IO_STATUS);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_IO_STATUS,IDD_20113_STATION_SET_END_DLG);

	//���û���Ťת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_IO_TEST,WM_IO_TEST_MENU_TO_OPERATION,DIALOG_GROUP_IO_STATUS);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_IO_STATUS,WM_IO_TEST_OPERATION_TO_MENU,DIALOG_GROUP_IO_TEST);

	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_IO_TEST);

	m_OperationType  = OPERATION_TYPE_UNKNOW;
	m_bIsStopTesting = false;
	m_hHandle        = NULL;

	DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// ��ȡ�豸����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestSvc::~CIOModuleTestSvc()
{
	//����ESC����Ҫ�˳�����߳�
	m_bIsStopTesting = true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ҵ������ʱ���õ�һ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestSvc::OnStart(){
	__super::OnStart();
	// ��ʼ��ֵ
	m_OperationType  = OPERATION_TYPE_UNKNOW;
	m_bIsStopTesting = false;
	m_hHandle        = NULL;
	m_Modol = Modol();
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// ��ȡ�豸����
	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_IO_TEST);
	if(theEXCEPTION_MGR.HasException(CIOException::MODULE_ID))
		theAPP_SESSION.ShowOfficeGuide(IO_TEST_TEXT_IO_ERROR_WARNING);
	else
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
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
LRESULT CIOModuleTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	int index = theAPP_SESSION.GetInputToInt();

	switch(index)
	{
	case MENU_CODE_1:
		{
			//Do it and set data
			m_OperationType = OPERATION_TYPE_STATUS;
		}
		break;
	//case MENU_CODE_2:
	//	{
	//		/*if(DEVICE_TYPE_AVM == DeviceType)
	//		{
	//		m_OperationType = OPERATION_TYPE_OPEN_ALARM;
	//		break;
	//		}
	//		m_OperationType = OPERATION_TYPE_CHANGE_INDICATOR;*/
	//		m_OperationType = OPERATION_TYPE_CHANGE_INDICATOR;
	//	}
	//	break;
	//case MENU_CODE_3:
	//	{
	//		/*if(DEVICE_TYPE_AVM == DeviceType)
	//		{
	//			m_OperationType = OPERATION_TYPE_CLOSE_ALARM;
	//			break;
	//		}
	//		m_OperationType = OPERATION_TYPE_CHANGE_LIGHT;*/
	//		m_OperationType = OPERATION_TYPE_CHANGE_LIGHT;
	//	}
	//	break;
	//case MENU_CODE_4:
	//	{
	//		m_OperationType = OPERATION_TYPE_CHARGE_INDICATOR;
	//	}
	//	break;
	////case MENU_CODE_5:
	////	{
	////		m_OperationType = OPERATION_TYPE_ALARM;
	////	}
	////	break;
	//case MENU_CODE_5:
	//	{
	//		m_OperationType = OPERATION_TYPE_CH_INDICATOR;
	//	}
	//	break;
	//case MENU_CODE_6:
	//	{
	//		m_OperationType = OPERATION_TYPE_RECEIPT_INDICATOR;
	//	}
	//	break;
	//case MENU_CODE_7:
	//	{
	//		m_OperationType = OPERATION_TYPE_BAFFLE;
	//	}
	//	break;
	//case MENU_CODE_8:
	//	{
	//		m_OperationType = OPERATION_TYPE_CARDFFLE;
	//	}
	//	break;
	case MENU_CODE_2:
		{
			m_OperationType = OPERATION_TYPE_OPEN_ALL;
		}
		break;
	case MENU_CODE_3:
		{
			m_OperationType = OPERATION_TYPE_CLOSE_ALL;
		}
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return FALSE;
	}

	//theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//����ѡ�����Ӧ�Ĳ���
	DoOperation();

	//���½�����ʾ
	NotifyDataModelChanged();
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
//LRESULT CIOModuleTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֹͣIO���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIOModuleTestSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam){
	m_bIsStopTesting = true;
	__super::OnKeyboardEsc(wParam,lParam);
	if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
		try{
			IO_HELPER->CloseAll();
		}
		catch(CSysException& e){

		}
		catch(...){

		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      F3��Ӧ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIOModuleTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_IO_STATUS)
	{
		m_bIsStopTesting = true;
		DoDialogFlow(WM_IO_TEST_OPERATION_TO_MENU);
	}
	else
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	NotifyDataModelChanged();

	if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
		try{
			IO_HELPER->CloseAll();
		}
		catch(CSysException& e){

		}
		catch(...){

		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����ģ������

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestSvc::Modol& CIOModuleTestSvc::GetDataModol()
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
void CIOModuleTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestSvc::DoOperation()
{
	long errCode = 0;
	int nMenuNum = 0;
	int nTryNTimes = 3;
	//bool bIsComOpen = false;
	theSERVICE_MGR.SetForeServiceBusy(true);
	try
	{
		try{
			IO_HELPER->CloseIO();
			IO_HELPER->OpenIO();
			IO_HELPER->InitializeIO();
			IO_RSP_STATUS_INFO rsp;
			IO_HELPER->GetIOStatus(rsp);
		}
		catch(CSysException& e)
		{
			//Open failed
			theAPP_SESSION.ShowOfficeGuide(GUIDE_IO_OPEN_PORT_ERROR);
			theSERVICE_MGR.SetForeServiceBusy(false);

			// IOģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_IO_UNIT_TEST,false);
			theEXCEPTION_MGR.ProcessException(e);
			return;
		}
		//1.IO״̬����
		if (m_OperationType == OPERATION_TYPE_STATUS)
		{
			DoDialogFlow(WM_IO_TEST_MENU_TO_OPERATION);

			nMenuNum = 0;
			//ÿ�μ�ⶼ��Ҫ��ʼ��
			m_bIsStopTesting = false;
			//theSERVICE_MGR.SetForeServiceBusy(false);
			m_hHandle = CreateThread(NULL,0,BeginTestingIOStatus,this,0,&dwThreadID);
			if (m_hHandle != NULL)
			{
				m_Modol.menuInfo[nMenuNum].strMenuStatus = COMPLETED;

				theSERVICE_MGR.SetForeServiceBusy(false);
				CloseHandle(m_hHandle);
				return;
			}
			else
			{
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		////2.��ȡƱ����ָʾ��
		//else if (m_OperationType == OPERATION_TYPE_CHANGE_INDICATOR)
		//{
		//	nMenuNum = 1;
		//	IO_HELPER->OpenReturnCupIndicator();
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////3.��ȡƱ����������
		//else if (m_OperationType == OPERATION_TYPE_CHANGE_LIGHT)
		//{
		//	nMenuNum = 2;
		//	IO_HELPER->OpenReturnLight(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////4.�򿪳�ֵ��ָʾ��
		//else if (m_OperationType == OPERATION_TYPE_CHARGE_INDICATOR)
		//{
		//	nMenuNum = 3;
		//	IO_HELPER->OpenChargeIndicator();
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////5.�򿪷�������
		////else if (m_OperationType == OPERATION_TYPE_ALARM)
		////{
		////	nMenuNum = 4;
		////	IO_HELPER->OpenAlarm(); 
		////	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		////}
		////6.��Ӳ��Ͷ�ҿ�ָʾ��
		//else if (m_OperationType == OPERATION_TYPE_CH_INDICATOR)
		//{
		//	nMenuNum = 4;
		//	IO_HELPER->OpenCHIndicator(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////7.��ƾ����ָʾ��
		//else if (m_OperationType == OPERATION_TYPE_RECEIPT_INDICATOR)
		//{
		//	nMenuNum = 5;
		//	IO_HELPER->OpenReceiptIndicator(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////8.Ӳ�ҵ������������Ͷ�ң�
		//else if (m_OperationType == OPERATION_TYPE_BAFFLE)
		//{
		//	nMenuNum = 6;
		//	IO_HELPER->OpenCoinShutter(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////9.�򿪴�ֵ�������������忨��
		//else if (m_OperationType == OPERATION_TYPE_CARDFFLE)
		//{
		//	nMenuNum = 7;
		//	IO_HELPER->OpenCardShutter(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		//10.������IO����
		else if (m_OperationType == OPERATION_TYPE_OPEN_ALL)
		{
			nMenuNum = 1;
			IO_HELPER->OpenAll();
			m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		}
		//11.�ر�����IO����
		else if (m_OperationType == OPERATION_TYPE_CLOSE_ALL)
		{
			nMenuNum = 2;
			IO_HELPER->CloseAll();
			m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		}

		// IOģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_IO_UNIT_TEST,true);
	}
	catch(CSysException& e)
	{
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;

		// IOģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_IO_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}

	//�մ���
	//if (bIsComOpen)
	//{
	//	try
	//	{
	//		IO_HELPER->CloseIO();
	//	}
	//	catch(...)
	//	{
	//		//�رմ���
	//	}
	//}

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);

	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��̬���IO״̬�߳�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CIOModuleTestSvc::BeginTestingIOStatus(LPVOID lpParameter)
{
	CIOModuleTestSvc* pSvc = (CIOModuleTestSvc*)lpParameter;
	CIOModuleTestSvc::Modol& modol = pSvc->GetDataModol();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);//??
	while(!pSvc->m_bIsStopTesting)
	{
		try
		{
			IO_RSP_STATUS_INFO rsp;
			IO_HELPER->GetIOStatus(rsp);
			//����״̬-����ɹ�
			modol.IO_Status[0].bIsThisActive = !rsp.isFrontDoorOpen;
			modol.IO_Status[1].bIsThisActive = !rsp.isBackDoorOpen;
			modol.IO_Status[2].bIsThisActive = !rsp.isLeftPersonActive;
			modol.IO_Status[3].bIsThisActive = !rsp.isBanknoteModeReady;
			modol.IO_Status[4].bIsThisActive = !rsp.isTokenInvBoxReady;
			modol.IO_Status[5].bIsThisActive = !rsp.isTokenColBoxReady;

			//��������
			pSvc->NotifyDataModelChanged();
			Sleep(500);//500������һ��
		}
		catch(CSysException& e)
		{
			theEXCEPTION_MGR.ProcessException(e);
			//���쳣ֱ������ѭ�����߳̽���
			break;
		}
	}

	//�رմ�������
	//pSvc->IO_HELPER->CloseIO();
	return 0;
}
