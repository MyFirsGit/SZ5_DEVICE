#include "stdafx.h"
#include "CoinModuleTestSvc.h"
#include "CoinModuleMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_COINTEST_MENU_TO_OPERATION = _T("WM_COINTEST_MENU_TO_OPERATION");
static const TCHAR* const WM_COINTEST_OPERATION_TO_MENU = _T("WM_COINTEST_OPERATION_TO_MENU");

BEGIN_MESSAGE_MAP(CCoinModuleTestSvc,CTVMForeService)
	//  ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	// 	ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	// 	ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
	// 	ON_SERVICE_MESSAGE(WM_KEYBOARD_F2,OnKeyboardF2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleTestSvc::CCoinModuleTestSvc()
	:CTVMForeService(COIN_MODULE_TEST_SVC)
{
	//���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG,new CCoinModuleMaintainDlg(this));

	//���û�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MENU,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MENU,IDD_20113_STATION_SET_END_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION,IDD_20113_STATION_SET_END_DLG);

	//���û���Ťת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_MENU,WM_COINTEST_MENU_TO_OPERATION,DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION,WM_COINTEST_OPERATION_TO_MENU,DIALOG_GROUP_MENU);

	//���ó�ʼ����
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MENU);

	bIsCoinShutterOpen = false;
	bIsCoinRecvStart   = false;
	//bIsCoinAddingStart = false;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleTestSvc::~CCoinModuleTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ҵ��ʼʱ��ʼ��״̬����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::OnStart(){
	__super::OnStart();
	//���ó�ʼ����
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MENU);
	// ��ʼ������
	ClearModolData();
	// ��ʼ������
	bIsCoinShutterOpen = false;
	bIsCoinRecvStart   = false;
	type_op = NULL_OP;
	//bIsCoinAddingStart = false;
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
LRESULT CCoinModuleTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MENU)
	{
		DoMenuOperation();
	}
	else if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	{
		DoOperation();
	}
	//ˢ�½���
	NotifyDataModelChanged();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      [F3/Ctrl]��Ӧ�¼�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinModuleTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	{
		DoDialogFlow(WM_COINTEST_OPERATION_TO_MENU);
		ClearModolData();
	}
	else
	{
		if(bIsCoinRecvStart){
			StopCoinRecvTest();
		}
		theAPP_SESSION.ReturnMaintainMenu();
	}
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
//LRESULT CCoinModuleTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [F2/Alt]������������Ӧ����

@param      (i)WPARAM ����
@param      (i)LPARAM ����

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinModuleTestSvc::OnKeyboardF2(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION
		&& m_Modol.operationType == OPERATION_TYPE_CLEAR_COIN)
	{
		//ִ����Ҳ���
		//Do it
		try
		{
			long errCode = 0;
			CH_SPEC5_RSP rsp;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_EMPTYING);
			/*
			errCode = COIN_HELPER->Chp_Save(rsp,
				m_Modol.clearCoinMenuInfo.bClsMenu_A1_All == false?m_Modol.clearCoinMenuInfo.iClsMenu_A1:0xFFFF,
				m_Modol.clearCoinMenuInfo.bClsMenu_A2_All == false?m_Modol.clearCoinMenuInfo.iClsMenu_A2:0xFFFF,
				m_Modol.clearCoinMenuInfo.bClsMenu_B1_All == false?m_Modol.clearCoinMenuInfo.iClsMenu_B1:0xffff,
				m_Modol.clearCoinMenuInfo.bClsMenu_B2_All == false?m_Modol.clearCoinMenuInfo.iClsMenu_B2:0xffff);
				*/
			if (errCode == E_NO_CH_ERROR)
			{
				m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CLEAR_SUCCESS);

				CString comment(_T("ClearCoinTest"));
				// Ӳ��ģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			}
			else
			{
				m_Modol.menuInfo[9].strMenuStatus = FAILED;

				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CLEAR_FALIED);

				CString comment(_T("ClearCoinTest"));
				// Ӳ��ģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			}

		}
		catch(CSysException& e)
		{
			m_Modol.menuInfo[9].strMenuStatus = FAILED;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CLEAR_FALIED);

			CString comment(_T("ClearCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			theEXCEPTION_MGR.ProcessException(e);
		}

		//������������
		//��Ҳ˵���״̬
		m_Modol.clearCoinMenuInfo.bClsMenu_A1 = false;
		m_Modol.clearCoinMenuInfo.bClsMenu_A2 = false;
		m_Modol.clearCoinMenuInfo.iClsMenu_A1 = 0;
		m_Modol.clearCoinMenuInfo.iClsMenu_A2 = 0;

		m_Modol.clearCoinMenuInfo.bClsMenu_B1 = false;
		m_Modol.clearCoinMenuInfo.bClsMenu_B2 = false;
		m_Modol.clearCoinMenuInfo.iClsMenu_B1 = 0;
		m_Modol.clearCoinMenuInfo.iClsMenu_B2 = 0;

		m_Modol.clearCoinMenuInfo.bClsMenu_A1_All = false;
		m_Modol.clearCoinMenuInfo.bClsMenu_A2_All = false;
		m_Modol.clearCoinMenuInfo.bClsMenu_B1_All = false;
		m_Modol.clearCoinMenuInfo.bClsMenu_B2_All = false;
	}

	//ˢ�½���
	NotifyDataModelChanged();
	//�����޸ĵ�RFID����
	//if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	//{
	//	if (m_Modol.operationType == OPERATION_TYPE_WRITE_ID)
	//	{
	//		bool bIsOk = true;
	//		try
	//		{
	//			if (m_Modol.rwCoinBoxId.strCAddingBoxA.IsEmpty()&&
	//				m_Modol.rwCoinBoxId.strCAddingBoxB.IsEmpty()&&
	//				m_Modol.rwCoinBoxId.strCCollectionBoxA.IsEmpty()&&
	//				m_Modol.rwCoinBoxId.strCCollectionBoxB.IsEmpty())
	//			{
	//				//û����Ҫ�޸���
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_NEED_ALTER_RFID);
	//				return TRUE ;
	//			}
	//			long errCode = 0; 
	//			//RFID is not null update it!
	//			//1.�ӱ���1
	//			if (!m_Modol.rwCoinBoxId.strCAddingBoxA.IsEmpty())
	//			{
	//				string strTemp = CT2A(m_Modol.rwCoinBoxId.strCAddingBoxA.GetBuffer());
	//				errCode = COIN_HELPER->WriteCoinBoxRFID(CH_BOX_ID_CHANGE1,strTemp);
	//				if (errCode != E_NO_CH_ERROR)
	//				{
	//					bIsOk = false;
	//				}
	//			}
	//			//2.�ӱ���2
	//			if (!m_Modol.rwCoinBoxId.strCAddingBoxB.IsEmpty())
	//			{
	//				string strTemp = CT2A(m_Modol.rwCoinBoxId.strCAddingBoxB.GetBuffer());
	//				errCode = COIN_HELPER->WriteCoinBoxRFID(CH_BOX_ID_CHANGE2,strTemp);
	//				if (errCode != E_NO_CH_ERROR)
	//				{
	//					bIsOk = false;
	//				}
	//
	//			}
	//			//3.������1
	//			if (!m_Modol.rwCoinBoxId.strCCollectionBoxA.IsEmpty())
	//			{
	//				string strTemp = CT2A(m_Modol.rwCoinBoxId.strCCollectionBoxA.GetBuffer());
	//				errCode = COIN_HELPER->WriteCoinBoxRFID(CH_BOX_ID_COLLECT1,strTemp);
	//				if (errCode != E_NO_CH_ERROR)
	//				{
	//					bIsOk = false;
	//				}
	//
	//			}
	//			//4.������2
	//			if (!m_Modol.rwCoinBoxId.strCCollectionBoxB.IsEmpty())
	//			{
	//				string strTemp = CT2A(m_Modol.rwCoinBoxId.strCCollectionBoxB.GetBuffer());
	//				errCode = COIN_HELPER->WriteCoinBoxRFID(CH_BOX_ID_COLLECT2,strTemp);
	//				if (errCode != E_NO_CH_ERROR)
	//				{
	//					bIsOk = false;
	//				}
	//
	//			}
	//			//Result
	//			if (bIsOk)
	//			{
	//				//ok
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_SAVE_RFID_SUCCESS);
	//				m_Modol.menuInfo[12].strMenuStatus = COMPLETED;
	//			}
	//			else
	//			{
	//				//error
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_SAVE_RFID_FAILED);
	//				m_Modol.menuInfo[12].strMenuStatus = FAILED;
	//			}
	//		}
	//		catch(CSysException& e)
	//		{
	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_SAVE_RFID_ERROR);
	//			m_Modol.menuInfo[12].strMenuStatus = FAILED;
	//			theEXCEPTION_MGR.ProcessException(e);
	//		}
	//	}
	//}
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
CCoinModuleTestSvc::Modol& CCoinModuleTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ˢ�½�����ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˵�ѡ����Ⱥ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::DoMenuOperation()
{
	if (!theAPP_SESSION.IsCHConnected())
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONN_ERROR));
		return;
	}
	int index = theAPP_SESSION.GetInputToInt();

	// �ǳ����û�ֻ��Ӧ��һ���˵�
	if(index != MenuNum1 && !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return;
	}

	switch(index)
	{
	case MenuNum1:
		{
			//1.����
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (ResetTest())
			{
				m_Modol.menuInfo[0].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[0].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum2:
		{
			//2.��Ӳ�ҽ��տ�
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (OpenCoinShutterTest())
			{
				m_Modol.menuInfo[1].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[1].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum3:
		{
			//3.�ر�Ӳ�ҽ��տ�
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (CloseCoinShutterTest())
			{
				m_Modol.menuInfo[2].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[2].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum4:
		{
			//4.��ʼ����Ӳ��
			//theSERVICE_MGR.SetForeServiceBusy(true);
			if (StartCoinRecvTest())
			{
				m_Modol.menuInfo[3].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[3].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			//theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum5:
		{
			//5.��������Ӳ��
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (StopCoinRecvTest())
			{
				m_Modol.menuInfo[4].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[4].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum6:
		{
			//6.CH����Ӳ��
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (AcceptCoinTest())
			{
				m_Modol.menuInfo[5].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[5].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum7:
		{
			//7.CHӲ������
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (ReleaseCoinTest())
			{
				m_Modol.menuInfo[6].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.menuInfo[6].strMenuStatus = FAILED;
			}
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum8:
		{
			//8.�����
			theSERVICE_MGR.SetForeServiceBusy(true);
			ReadAmountTest();
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case MenuNum9:
		{
			//9.�˱�
			//m_Modol.operationType = OPERATION_TYPE_RETURN_COIN;
			//DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
			theSERVICE_MGR.SetForeServiceBusy(true);
			ReturnCoinTest();
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	//case MenuNum10:
	//	{
	//		//10.���
	//		m_Modol.operationType = OPERATION_TYPE_CLEAR_COIN;
	//		DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
	//	}
	//	break;
	/*case MenuNum11:
		{
			//11.����
			//m_Modol.operationType = OPERATION_TYPE_ADDING_COIN;
			theSERVICE_MGR.SetForeServiceBusy(true);
			CoinAddingTest();
			theSERVICE_MGR.SetForeServiceBusy(false);
			//DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
		}
		break;*/
		//case MenuNum10:
		//	{
		//		//10.��������
		//		theSERVICE_MGR.SetForeServiceBusy(true);
		//		if (StopCoinAddingTest())
		//		{
		//			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
		//		}
		//		else
		//		{
		//			m_Modol.menuInfo[9].strMenuStatus = FAILED;
		//		}
		//		theSERVICE_MGR.SetForeServiceBusy(false);
		//	}
		//	break;
		//case MenuNum11:
		//	{
		//		//11.��ȫ�ۿ���
		//		m_Modol.operationType = OPERATION_TYPE_SAFELOCK_CTRL;
		//		DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
		//	}
		//	break;
		//case MenuNum12:
		//	{
		//		//12.��RFID
		//		m_Modol.operationType = OPERATION_TYPE_READ_ID;
		//		ReadCoinBoxIdTest();
		//		DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
		//	}
		//	break;
		//case MenuNum13:
		//	{
		//		//13.дRFID
		//		m_Modol.operationType = OPERATION_TYPE_WRITE_ID;
		//		DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
		//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_INPUT_BOXID));
		//	}
		//	break;
		//case MenuNum14:
		//	{
		//		//14.���汾
		//		theSERVICE_MGR.SetForeServiceBusy(true);
		//		ReadVersionTest();
		//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
		//		theSERVICE_MGR.SetForeServiceBusy(false);
		//	}
		//	break;
	default:
		{
			//Error Menu Number
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		}
		break;
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������Ⱥ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::DoOperation()
{
	//���ݲ�ͬ�Ĳ�����������ͬ���ж�
	switch(m_Modol.operationType)
	{
	case OPERATION_TYPE_RETURN_COIN:
		{
			theSERVICE_MGR.SetForeServiceBusy(true);
			ReturnCoinTest();
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case OPERATION_TYPE_CLEAR_COIN:
		{
			theSERVICE_MGR.SetForeServiceBusy(true);
			ClearCoinTest();
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
		/*case OPERATION_TYPE_SAFELOCK_CTRL:
		{
		theSERVICE_MGR.SetForeServiceBusy(true);
		ContorlSafeLockTest();
		theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
		case OPERATION_TYPE_READ_ID:
		{
		theSERVICE_MGR.SetForeServiceBusy(true);
		ReadCoinBoxIdTest();
		theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
		case OPERATION_TYPE_WRITE_ID:
		{
		theSERVICE_MGR.SetForeServiceBusy(true);
		WriteCoinBoxIdTest();
		theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;*/
		//case OPERATION_TYPE_ADDING_COIN:
		//	{
		//		//��ʼ�ӱ�
		//		theSERVICE_MGR.SetForeServiceBusy(true);
		//		CoinAddingTest();
		//		theSERVICE_MGR.SetForeServiceBusy(false);
		//	}
		//	break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ò�������

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::ResetTest()
{
	try
	{
		long errCode = 0;
		//CH_SYSTEM_STATUS_RSP rsp;
		//errCode = COIN_HELPER->CH_Reset(/*rsp*/);//20180328��ֱ�ӵ���Ӳ��ģ��ĸ�λ�ӿڣ��ϵ������£�Ҳ�������ʲ�ʹ�á�
		COIN_HELPER->CH_Close();
		COIN_HELPER->CH_InitModule();
		if (errCode == E_NO_CH_ERROR)
		{
			CString comment(_T("ResetTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
	}
	catch(CSysException& e)
	{
		CString comment(_T("ResetTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ӳ�ҽ��տ�

@param      none

@retval     (o) true  or   false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::OpenCoinShutterTest()
{
	try
	{
		long errCode = 0;
		errCode = COIN_HELPER->CH_OpenShutter();
		if (errCode == E_NO_CH_ERROR)
		{
			bIsCoinShutterOpen = true;
			CString comment(_T("OpenCoinShutterTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
		CString comment(_T("OpenCoinShutterTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		return  false;
	}
	catch(CSysException& e)
	{
		CString comment(_T("OpenCoinShutterTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ر�Ӳ�ҽ��տ�

@param      none

@retval     (o)true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::CloseCoinShutterTest()
{
	try
	{
		if (!bIsCoinShutterOpen)
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_NEED_OPEN_SHUTTER);
			CString comment(_T("CloseCoinShutterTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
		long errCode = 0;
		errCode = COIN_HELPER->CH_CloseShutter();
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
		if (errCode == E_NO_CH_ERROR)
		{
			CString comment(_T("CloseCoinShutterTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
		else
		{
			CString comment(_T("CloseCoinShutterTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
	}
	catch (CSysException& e)
	{
		CString comment(_T("CloseCoinShutterTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����Ӳ��

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::StartCoinRecvTest()
{
	try
	{
		long errCode =  0;
		errCode = COIN_HELPER->CH_StartReceive();
		if (errCode == E_NO_CH_ERROR)
		{
			bIsCoinRecvStart = true;
			CString comment(_T("StartCoinRecvTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
		else
		{
			CString comment(_T("StartCoinRecvTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
	}
	catch(CSysException& e)
	{
		CString comment(_T("StartCoinRecvTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ����Ӳ��

@param      none

@retval     (o) true or false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::StopCoinRecvTest()
{
	try
	{
		if (!bIsCoinRecvStart)
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_NEED_BEGIN_RECV);
			CString comment(_T("StopCoinRecvTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
		long errCode = 0;
		errCode = COIN_HELPER->CH_EndReceive();
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
		if (errCode == E_NO_CH_ERROR)
		{
			CString comment(_T("StopCoinRecvTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
		else
		{
			CString comment(_T("StopCoinRecvTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
	}
	catch(CSysException& e)
	{
		CString comment(_T("StopCoinRecvTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CH����Ӳ��

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::AcceptCoinTest()
{
	try
	{
		long errCode =  0;
		errCode = COIN_HELPER->CH_Accept();//ͬ������
		if (errCode == E_NO_CH_ERROR)
		{
			CString comment(_T("AcceptCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
		else
		{
			CString comment(_T("AcceptCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
	}
	catch(CSysException& e)
	{
		CString comment(_T("AcceptCoinTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CHӲ������

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinModuleTestSvc::ReleaseCoinTest()
{
	try
	{
		long errCode = 0;
		tCHAChangeNum ch_rsp;
		errCode = COIN_HELPER->CH_Change(1,0,&ch_rsp); //ͬ������
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
		if (errCode == E_NO_CH_ERROR && ch_rsp.wMHopper1Num != 0)
		{
			CString comment(_T("ReleaseCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
			return true;
		}
		else
		{
			 if(ch_rsp.wMHopper1Num == 0 && ch_rsp.wMHopper2Num == 0)
			{
				CString comment(_T("No Coin"));
				// Ӳ��ģ�����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_NO_COIN));
				return false;
		    }
			CString comment(_T("ReleaseCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
			return false;
		}
	}
	catch(CSysException& e)
	{
		CString comment(_T("ReleaseCoinTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::ReadAmountTest()
{
	try
	{
		long errCode = 0;
		tCHACoinValueInfo rsp;
		//�ڶ�ȡ���ʱ��ֹͣ�첽���ĵ���
		//COIN_HELPER->Chp_Stop_Insert_Info();
		//��ʼ��ȡ���:ֻ��һ��
		errCode = COIN_HELPER->CH_GetInsertInfo(rsp);
		if (errCode == E_NO_CH_ERROR)
		{
			double dCountMoney   = 0.0;
			int nCount1  = 0;   //1Ԫ������
			int nCount05  = 0;  //5�ǵ�����		
			nCount1 = rsp.stCoinBoxValueInfo[0].uiCoinNum;
			nCount05 = rsp.stCoinBoxValueInfo[1].uiCoinNum;
			dCountMoney = nCount1 + (double)nCount05*0.5;
			m_Modol.menuInfo[7].strMenuStatus.Format(_T("%0.1fRMB"),dCountMoney);

			CString comment(_T("ReadAmountTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
		}
		else
		{
			m_Modol.menuInfo[7].strMenuStatus = FAILED;
			CString comment(_T("ReadAmountTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		}
		//������ȡ���
		//COIN_HELPER->Chp_Stop_Insert_Info();
	}
	catch (CSysException& e)
	{
		m_Modol.menuInfo[7].strMenuStatus = FAILED;

		CString comment(_T("ReadAmountTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˱Ҳ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::ReturnCoinTest()
{
	try
	{
		////ѡ���ݴ���Ӳ��ȥ��
		//int index = theAPP_SESSION.GetInputToInt();
		//bool bErrMsg = false;
		//switch(index)
		//{
		//case MenuNum1:
		//	{
		//1.����Ӳ��
		long errCode = 0;
		errCode = COIN_HELPER->CH_Refund();
		if(errCode == E_NO_CH_ERROR)
		{
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			m_Modol.returnCoinMenuInfo.str_cancel = SUCCESS;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_RETURN_SUCCESS);
			CString comment(_T("ReturnCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
		}
		else
		{
			m_Modol.menuInfo[8].strMenuStatus = FAILED;
			m_Modol.returnCoinMenuInfo.str_cancel = FAILED;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_RETURN_FAILED);
			CString comment(_T("ReturnCoinTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		}

		//	}
		//	break;
		//case MenuNum2:
		//	{
		//		//2.����
		//		long errCode = 0;
		//		CH_COMMON_RSP rsp;
		//		errCode = COIN_HELPER->CH_Accept(rsp);
		//		if (errCode == E_NO_CH_ERROR)
		//		{
		//			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
		//			m_Modol.returnCoinMenuInfo.str_accept = SUCCESS;
		//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_RETURN_SUCCESS);
		//			CString comment(_T("ReturnCoinTest"));
		//			// Ӳ��ģ�����--������־
		//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
		//		}
		//		else
		//		{
		//			m_Modol.menuInfo[8].strMenuStatus = FAILED;
		//			m_Modol.returnCoinMenuInfo.str_accept= FAILED;
		//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_RETURN_FAILED);
		//			CString comment(_T("ReturnCoinTest"));
		//			// Ӳ��ģ�����--������־
		//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		//		}
		//	}
		//	break;
		//default:
		//	{
		//		//Error Menu Number
		//		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		//	}
		//	break;
		//}
	}
	catch(CSysException& e)
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_RETURN_FAILED);

		CString comment(_T("ReturnCoinTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡӲ��ר��������Ӳ������


@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CCoinModuleTestSvc::GetHopperType(int hopetype)
{
	BYTE CurrencyType = 0;
	switch(hopetype)		// ��TVM.INI�ļ��ж�ȡ�ļ�ֵ
	{
	case MENU_CODE_1:
		CurrencyType = 4;		//50AVOS
		break;
	case MENU_CODE_2:
		CurrencyType = 1;		//1MOP
		break;
	case MENU_CODE_3:
		CurrencyType = 2;		//2MOP
		break;
	case MENU_CODE_4:
		CurrencyType = 3;		//5MOP
		break;
	default:
		break;
	}
	return CurrencyType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ҳ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::ClearCoinTest()
{
	CString strMenuNum = theAPP_SESSION.GetInputToString();

	int index = _ttoi(strMenuNum);
	CString strSonMenu = strMenuNum.Right(strMenuNum.GetLength() -1);
	int hopper1_num = 0,hopper2_num = 0,backup1_num=0,backup2_num=0;
	hopper1_num = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount; //hopper1����Ӳ������
	hopper2_num = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;//hopper1����Ӳ������
	backup1_num = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;//��������Ӳ������
	backup2_num = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;//��������Ӳ������

	switch(type_op)
	{
	case NULL_OP:
		{
			switch(index)
			{
			case MenuNum11:
				m_Modol.clearCoinMenuInfo.bClsMenu_A1 = true;
				type_op = MenuA1;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_RECYCLE_NUMBER);
				break;
			case MenuNum12:
				m_Modol.clearCoinMenuInfo.bClsMenu_A1_All = true;
				m_Modol.clearCoinMenuInfo.bClsMenu_A1 = false;
				m_Modol.clearCoinMenuInfo.iClsMenu_A1 = hopper1_num;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CHOOSE_SUCCESS);
				break;
			//case MenuNum21:
			//	m_Modol.clearCoinMenuInfo.bClsMenu_A2 = true;
			//	type_op = MenuA2;
			//	theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_RECYCLE_NUMBER);
			//	break;
			//case MenuNum22:
			//	m_Modol.clearCoinMenuInfo.bClsMenu_A2_All = true;
			//	m_Modol.clearCoinMenuInfo.bClsMenu_A2 = false;
			//	m_Modol.clearCoinMenuInfo.iClsMenu_A2 = hopper2_num;
			//	theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CHOOSE_SUCCESS);
			//	break;
			case MenuNum21:
				m_Modol.clearCoinMenuInfo.bClsMenu_B1 = true;
				type_op = MenuB1;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_RECYCLE_NUMBER);
				break;
			case MenuNum22:
				m_Modol.clearCoinMenuInfo.bClsMenu_B1_All = true;
				m_Modol.clearCoinMenuInfo.bClsMenu_B1 = false;
				m_Modol.clearCoinMenuInfo.iClsMenu_B1 = backup1_num;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CHOOSE_SUCCESS);
				break;
			case MenuNum31:
				m_Modol.clearCoinMenuInfo.bClsMenu_B2 = true;
				type_op = MenuB2;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_RECYCLE_NUMBER);
				break;
			case MenuNum32:
				m_Modol.clearCoinMenuInfo.bClsMenu_B2_All = true;
				m_Modol.clearCoinMenuInfo.bClsMenu_B2 = false;
				m_Modol.clearCoinMenuInfo.iClsMenu_B2 = backup2_num;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CHOOSE_SUCCESS);
				break;
			case MenuNum4:
				{
					try
					{
						long errCode = 0;
						theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_EMPTYING);
						tCoinBoxInfo box;
						errCode = COIN_HELPER->CH_ClearCoin(&box);
						if (errCode == E_NO_CH_ERROR)
						{
							m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
							theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CLEAR_SUCCESS);

							CString comment(_T("ClearCoinTest"));
							// Ӳ��ģ�����--������־
							theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
						}
						else
						{
							m_Modol.menuInfo[9].strMenuStatus = FAILED;

							theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CLEAR_FALIED);

							CString comment(_T("ClearCoinTest"));
							// Ӳ��ģ�����--������־
							theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
						}

					}
					catch(CSysException& e)
					{
						m_Modol.menuInfo[9].strMenuStatus = FAILED;
						theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CLEAR_FALIED);

						CString comment(_T("ClearCoinTest"));
						// Ӳ��ģ�����--������־
						theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
						theEXCEPTION_MGR.ProcessException(e);
					}
				}
				break;
			default:
				{
					theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				}
				break;
			}
		}
		break;
	case MenuA1:
		{
			if(index <= hopper1_num)
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_A1_All = false;
				m_Modol.clearCoinMenuInfo.bClsMenu_A1 = true;
				m_Modol.clearCoinMenuInfo.iClsMenu_A1 = index;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_SUCCESS);
			}
			else
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_A1 = false;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OUT_OF_RANGE);
			}
			type_op = NULL_OP;
		}
		break;
	case MenuA2:
		{
			if(index <= hopper2_num)
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_A2_All = false;
				m_Modol.clearCoinMenuInfo.bClsMenu_A2 = true;
				m_Modol.clearCoinMenuInfo.iClsMenu_A2 = index;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_SUCCESS);
			}
			else
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_A2 = false;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OUT_OF_RANGE);
			}
			type_op = NULL_OP;
		}
		break;
	case MenuB1:
		{
			if(index <= backup1_num)
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_B1_All = false;
				m_Modol.clearCoinMenuInfo.bClsMenu_B1 = true;
				m_Modol.clearCoinMenuInfo.iClsMenu_B1 = index;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_SUCCESS);
			}
			else
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_B1 = false;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OUT_OF_RANGE);
			}
			type_op = NULL_OP;
		}
		break;
	case MenuB2:
		{
			if(index <= backup2_num)
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_B2_All = false;
				m_Modol.clearCoinMenuInfo.bClsMenu_B2 = true;
				m_Modol.clearCoinMenuInfo.iClsMenu_B2 = index;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_SUCCESS);
			}
			else
			{
				m_Modol.clearCoinMenuInfo.bClsMenu_B2 = false;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OUT_OF_RANGE);
			}
			type_op = NULL_OP;
		}
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ҳ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::CoinAddingTest()
{
	//int index = theAPP_SESSION.GetInputToInt();

	//CH_COMMON_ADDING_BOX_TYPE box_num;

	//switch(index)
	//{
	//case MenuNum11:
	//	{
	//		box_num = CH_COMMON_ADDING_BOX_TYPE::ADDING_BOX1;
	//	}
	//	break;
	//case MenuNum12:
	//	{
	//		box_num = CH_COMMON_ADDING_BOX_TYPE::ADDING_BOX2;
	//	}
	//	break;
	//case MenuNum13:
	//	{
	//		box_num = CH_COMMON_ADDING_BOX_TYPE::ADDING_BOX12;
	//	}
	//	break;
	//default:
	//	{
	//		//Error Input Code
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//		return;
	//	}
	//	break;
	//}

	try
	{
		long errCode = 0;
		//CH_SYSTEM_STATUS_RSP rsp;
		//errCode = COIN_HELPER->CH_Supply(/*rsp*/);
		if (errCode != E_NO_CH_ERROR)
		{
			//error infomation
			m_Modol.menuInfo[10].strMenuStatus = FAILED;
			//theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_ADD_FAILED);//!..

			CString comment(_T("CoinAddingTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		}
		else
		{
			//	bIsCoinAddingStart = true;
			m_Modol.menuInfo[10].strMenuStatus = SUCCESS;
			//theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_BEGIN_ADD_PULL);//!..

			CString comment(_T("CoinAddingTest"));
			// Ӳ��ģ�����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
		}

	}
	catch (CSysException& e)
	{
		m_Modol.menuInfo[10].strMenuStatus = FAILED;
		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_ADD_FAILED);//!..

		CString comment(_T("CoinAddingTest"));
		// Ӳ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��տɱ����ݣ���ֹ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleTestSvc::ClearModolData()
{
	// ��ʼ������
	m_Modol = Modol();
	//��������״̬
	m_Modol.operationType = OPERATION_TYPE_UNKNOW;

	//�˱Ҳ˵���״̬
	/*m_Modol.returnCoinMenuInfo.bMenu_A1 = false;
	m_Modol.returnCoinMenuInfo.bMenu_A2 = false;
	m_Modol.returnCoinMenuInfo.bMenu_A3 = false;
	m_Modol.returnCoinMenuInfo.bMenu_A4 = false;

	m_Modol.returnCoinMenuInfo.bMenu_B1 = false;
	m_Modol.returnCoinMenuInfo.bMenu_B2 = false;*/

	//��Ҳ˵���״̬
	m_Modol.clearCoinMenuInfo.bClsMenu_A1 = false;
	m_Modol.clearCoinMenuInfo.bClsMenu_A2 = false;
	m_Modol.clearCoinMenuInfo.iClsMenu_A1 = 0;
	m_Modol.clearCoinMenuInfo.iClsMenu_A2 = 0;

	m_Modol.clearCoinMenuInfo.bClsMenu_B1 = false;
	m_Modol.clearCoinMenuInfo.bClsMenu_B2 = false;
	m_Modol.clearCoinMenuInfo.iClsMenu_B1 = 0;
	m_Modol.clearCoinMenuInfo.iClsMenu_B2 = 0;

	m_Modol.clearCoinMenuInfo.bClsMenu_A1_All = false;
	m_Modol.clearCoinMenuInfo.bClsMenu_A2_All = false;
	m_Modol.clearCoinMenuInfo.bClsMenu_B1_All = false;
	m_Modol.clearCoinMenuInfo.bClsMenu_B2_All = false;

	//��ȫ�۲˵�״̬
	m_Modol.safeLockCtrlMenuInfo.bSfMenu_A1 = false;
	m_Modol.safeLockCtrlMenuInfo.bSfMenu_A2 = false;
	m_Modol.safeLockCtrlMenuInfo.bSfMenu_B1 = false;
	m_Modol.safeLockCtrlMenuInfo.bSfMenu_B2 = false;

	//RFID״̬
	m_Modol.rwCoinBoxId.strCAddingBoxA = UNKNOW;
	m_Modol.rwCoinBoxId.strCAddingBoxB = UNKNOW;
	m_Modol.rwCoinBoxId.strCCollectionBoxA = UNKNOW;
	m_Modol.rwCoinBoxId.strCCollectionBoxB = UNKNOW;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������Ҳ���

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//bool CCoinModuleTestSvc::StopCoinAddingTest()
//{
//	try
//	{
//		//if(!bIsCoinAddingStart)
//		//{
//		//	theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_NEED_BEGIN_ADD);
//		//	return false;
//		//}
//		//long errCode = 0;
//		//CH_COMMON_RSP rsp;
//		//errCode = COIN_HELPER->Chp_Supply_End(rsp);
//		//if (errCode == E_NO_CH_ERROR)
//		//{
//		//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CONFIRM_RESULT));
//
//		//	CString comment(_T("StopCoinAddingTest"));
//		//	// Ӳ��ģ�����--������־
//		//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//		//	return true;
//		//}
//		//else
//		//{
//		//	//���ؽ���ж�(�����쳣)
//		//	//1.�ӱ���1�ӱ�δ���
//		//	if (errCode == CH_RETURN_ERROR_CODE::CHRET_FAILED_ADD_BOX_A)
//		//	{
//		//		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_ADD_BOX1_UNFINISH);
//		//	}
//		//	//2.�ӱ���2�ӱ�δ���
//		//	if (errCode == CH_RETURN_ERROR_CODE::CHRET_FAILED_ADD_BOX_B)
//		//	{
//		//		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_ADD_BOX2_UNFINISH);
//		//	}
//		//	//3.�ӱ���1,2�ӱ�δ���
//		//	if (errCode == CH_RETURN_ERROR_CODE::CHRET_FAILED_ADD_BOX_AB)
//		//	{
//		//		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_ADD_BOX12_UNFINISH);
//		//	}
//		//	CString comment(_T("StopCoinAddingTest"));
//		//	// Ӳ��ģ�����--������־
//		//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//		//	return false;
//		//}
//	}
//	catch (CSysException& e)
//	{
//		CString comment(_T("StopCoinAddingTest"));
//		// Ӳ��ģ�����--������־
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ӱҰ�ȫ�ۿ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CCoinModuleTestSvc::ContorlSafeLockTest()
//{
//	//int index = theAPP_SESSION.GetInputToInt();
//	//
//	//switch(index)
//	//{
//	//case MenuNum11:
//	//	{
//	//		//Do Coin Safe Lock Ctrl and return Result:Box 1 Lock
//	//		try
//	//		{
//	//			long errCode = 0;
//	//			errCode = COIN_HELPER->CtrlSafeLock(CH_COMMON_SAFE_LOCK_CTRL::LOCK_AB);
//	//			if (errCode != E_NO_CH_ERROR)
//	//			{
//	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//				CString comment(_T("ContorlSafeLockTest"));
//	//				// Ӳ��ģ�����--������־
//	//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//			}
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_SUCCESS);
//	//		}
//	//		catch(CSysException& e)
//	//		{
//	//			//Failed
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//			CString comment(_T("ContorlSafeLockTest"));
//	//			// Ӳ��ģ�����--������־
//	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//			theEXCEPTION_MGR.ProcessException(e);
//	//		}
//	//	}
//	//	break;
//	//case MenuNum12:
//	//	{
//	//		//Do Coin Safe Lock Ctrl and return Result:Box 1 Unlock
//	//		try
//	//		{
//	//			long errCode = 0;
//	//			errCode = COIN_HELPER->CtrlSafeLock(CH_COMMON_SAFE_LOCK_CTRL::UNLOCK_A);
//	//			if (errCode != E_NO_CH_ERROR)
//	//			{
//	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//			}
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_SUCCESS);
//	//		}
//	//		catch(CSysException& e)
//	//		{
//	//			//Failed
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//			theEXCEPTION_MGR.ProcessException(e);
//	//		}
//	//	}
//	//	break;
//	//case MenuNum21:
//	//	{
//	//		//Do Coin Safe Lock Ctrl and return Result:Box 2 Lock
//	//		try
//	//		{
//	//			long errCode = 0;
//	//			errCode = COIN_HELPER->CtrlSafeLock(CH_COMMON_SAFE_LOCK_CTRL::LOCK_AB);
//	//			if (errCode != E_NO_CH_ERROR)
//	//			{
//	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//				CString comment(_T("ContorlSafeLockTest"));
//	//				// Ӳ��ģ�����--������־
//	//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//			}
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_SUCCESS);
//	//		}
//	//		catch(CSysException& e)
//	//		{
//	//			//Failed
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//			CString comment(_T("ContorlSafeLockTest"));
//	//			// Ӳ��ģ�����--������־
//	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//			theEXCEPTION_MGR.ProcessException(e);
//	//		}
//	//	}
//	//	break;
//	//case MenuNum22:
//	//	{
//	//		//Do Coin Safe Lock Ctrl and return Result:Box 2 Unlock
//	//		try
//	//		{
//	//			long errCode = 0;
//	//			errCode = COIN_HELPER->CtrlSafeLock(CH_COMMON_SAFE_LOCK_CTRL::UNLOCK_B);
//	//			if (errCode != E_NO_CH_ERROR)
//	//			{
//	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//				CString comment(_T("ContorlSafeLockTest"));
//	//				// Ӳ��ģ�����--������־
//	//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//			}
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_SUCCESS);
//	//		}
//	//		catch(CSysException& e)
//	//		{
//	//			//Failed
//	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_OPERATION_FAILED);
//	//			CString comment(_T("ContorlSafeLockTest"));
//	//			// Ӳ��ģ�����--������־
//	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//			theEXCEPTION_MGR.ProcessException(e);
//	//		}
//	//	}
//	//	break;
//	//default:
//	//	{
//	//		//Error Input Code
//	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
//	//	}
//	//	break;
//	//}
//	//m_Modol.menuInfo[10].strMenuStatus = COMPLETED;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ǯ��RFID

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CCoinModuleTestSvc::ReadCoinBoxIdTest()
//{
//	//Just Read and set data to m_Modol
//	//Like...
//	//try
//	//{
//	//	long errCode = 0;
//	//	CH_RSP_READ_RFID result,result1,result2,result3;
//	//	bool isExecOk = true;
//	//	//�ӱ���1
//	//	errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_CHANGE1,result);
//	//	if (errCode == E_NO_CH_ERROR)
//	//	{
//	//		m_Modol.rwCoinBoxId.strCAddingBoxA = result.rfid_info.data;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//	}
//	//	else
//	//	{
//	//		isExecOk = false;
//	//		m_Modol.rwCoinBoxId.strCAddingBoxA = UNKNOW;
//
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	}
//	//	//�ӱ���2
//	//	errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_CHANGE2,result1);
//	//	if (errCode == E_NO_CH_ERROR)
//	//	{
//	//		m_Modol.rwCoinBoxId.strCAddingBoxB = result1.rfid_info.data;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//	}
//	//	else
//	//	{
//	//		isExecOk = false;
//	//		m_Modol.rwCoinBoxId.strCAddingBoxB = UNKNOW;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	}
//	//	//������1
//	//	errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_COLLECT1,result2);
//	//	if (errCode == E_NO_CH_ERROR)
//	//	{
//	//		m_Modol.rwCoinBoxId.strCCollectionBoxA = result2.rfid_info.data;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//	}
//	//	else
//	//	{
//	//		isExecOk = false;
//	//		m_Modol.rwCoinBoxId.strCCollectionBoxA = UNKNOW;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	}
//	//	//������2
//	//	errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_COLLECT2,result3);
//	//	if (errCode == E_NO_CH_ERROR)
//	//	{
//	//		m_Modol.rwCoinBoxId.strCCollectionBoxB = result3.rfid_info.data;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//	}
//	//	else
//	//	{
//	//		isExecOk = false;
//	//		m_Modol.rwCoinBoxId.strCCollectionBoxB = UNKNOW;
//	//		CString comment(_T("ReadCoinBoxIdTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	}
//
//	//	//������ʾ���˵�״̬
//	//	if (isExecOk)
//	//	{
//	//		m_Modol.menuInfo[11].strMenuStatus = COMPLETED;
//	//	}
//	//	else
//	//	{
//	//		m_Modol.menuInfo[11].strMenuStatus = FAILED;
//	//	}
//	//}
//	//catch (CSysException& e)
//	//{
//	//	//Set default rfid
//	//	m_Modol.rwCoinBoxId.strCAddingBoxA = UNKNOW;
//	//	m_Modol.rwCoinBoxId.strCAddingBoxB = UNKNOW;
//	//	m_Modol.rwCoinBoxId.strCCollectionBoxA = UNKNOW;
//	//	m_Modol.rwCoinBoxId.strCCollectionBoxB = UNKNOW;
//	//	//��ʾʧ��
//	//	m_Modol.menuInfo[11].strMenuStatus = FAILED;
//
//	//	CString comment(_T("ReadCoinBoxIdTest"));
//	//	// Ӳ��ģ�����--������־
//	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	theEXCEPTION_MGR.ProcessException(e);
//	//}
//
//	//m_Modol.rwCoinBoxId.strCAddingBoxA = SUCCESS;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      дǮ��RFID

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CCoinModuleTestSvc::WriteCoinBoxIdTest()
//{
//	CString strMenuInfo = theAPP_SESSION.GetInputToString();
//	if (strMenuInfo.GetLength() != 5)
//	{
//		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_ERROR_RETRY);
//		return;
//	}
//
//	int nMainMenu = _ttoi(strMenuInfo.Left(1));
//
//	CString strRFIDHead;
//
//	switch(nMainMenu)
//	{
//	case MenuNum1:
//		{
//			strRFIDHead.Format(_T("0191%s"),strMenuInfo.Right(strMenuInfo.GetLength() - 1));
//			m_Modol.rwCoinBoxId.strCAddingBoxA = strRFIDHead;
//		}
//		break;
//	case MenuNum2:
//		{
//			strRFIDHead.Format(_T("0191%s"),strMenuInfo.Right(strMenuInfo.GetLength() - 1));
//			m_Modol.rwCoinBoxId.strCAddingBoxB = strRFIDHead;
//		}
//		break;
//	case MenuNum3:
//		{
//			strRFIDHead.Format(_T("0190%s"),strMenuInfo.Right(strMenuInfo.GetLength() - 1));
//			m_Modol.rwCoinBoxId.strCCollectionBoxA = strRFIDHead;
//
//		}
//		break;
//	case MenuNum4:
//		{
//			strRFIDHead.Format(_T("0190%s"),strMenuInfo.Right(strMenuInfo.GetLength() - 1));
//			m_Modol.rwCoinBoxId.strCCollectionBoxB = strRFIDHead;		
//		}
//		break;
//	default:
//		{
//			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
//		}
//		break;
//	}
//	//??
//	if (m_Modol.rwCoinBoxId.strCAddingBoxA.IsEmpty())
//	{
//		//??
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_INPUT_ADD_BOX1));
//	}
//	else if (m_Modol.rwCoinBoxId.strCAddingBoxB.IsEmpty())
//	{
//		//??
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_INPUT_ADD_BOX2));
//	}
//	else if(m_Modol.rwCoinBoxId.strCCollectionBoxA.IsEmpty())
//	{
//		//??
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_INPUT_COLLECTION_BOX1));
//	}
//	else if (m_Modol.rwCoinBoxId.strCCollectionBoxB.IsEmpty())
//	{
//		//??
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_INPUT_COLLECTION_BOX2));
//	}
//	else
//	{
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_INPUT_SAVE));		
//	}
//	CString comment(_T("WriteCoinBoxIdTest"));
//	// Ӳ��ģ�����--������־
//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���̼��汾

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CCoinModuleTestSvc::ReadVersionTest()
//{
//	//Read and set to modol
//	//try
//	//{
//	//	CH_RSP_VERSION chVersion;
//	//	COIN_HELPER->GetCHVersion(chVersion);
//	//	CString strTemp("");
//	//	strTemp = chVersion.version_info.HardVersison;
//	//	if (!strTemp.IsEmpty())
//	//	{
//	//		m_Modol.menuInfo[13].strMenuStatus = strTemp;
//	//		CString comment(_T("ReadVersionTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,true,comment);
//	//	}
//	//	else
//	//	{
//	//		m_Modol.menuInfo[13].strMenuStatus = FAILED;
//	//		CString comment(_T("ReadVersionTest"));
//	//		// Ӳ��ģ�����--������־
//	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	}
//
//	//	
//	//}
//	//catch(CSysException& e)
//	//{
//	//	m_Modol.menuInfo[13].strMenuStatus = FAILED;
//	//	CString comment(_T("ReadVersionTest"));
//	//	// Ӳ��ģ�����--������־
//	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_UNIT_TEST,false,comment);
//	//	theEXCEPTION_MGR.ProcessException(e);
//	//	return;
//	//}
//}