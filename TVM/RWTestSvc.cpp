#include "stdafx.h"
#include "RWTestSvc.h"
#include "CardReadWriteMachineTestMaintainDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INTERNAL_ERROR(errCode) \
{if (COIN_HELPER->IsExceptionCode(errCode)) {\
	throw CCHException(errCode, _T(__FILE__), __LINE__);\
}else\
{throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);}}

static const TCHAR* const WM_RWMGR_MAIN_TO_BASE      = _T("WM_RWMGR_MAIN_TO_BASE");
static const TCHAR* const WM_RWMGR_BASE_TO_OPERATION = _T("WM_RWMGR_BASE_TO_OPERATION");
static const TCHAR* const WM_RWMGR_OPERATION_TO_BASE = _T("WM_RWMGR_OPERATION_TO_BASE");
static const TCHAR* const WM_RWMGR_BASE_TO_MAIN     = _T("WM_RW_MGR_BASE_TO_MAIN");

BEGIN_MESSAGE_MAP(CRWTestSvc, CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::CRWTestSvc()
	:CTVMForeService(RW_TEST_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG,new CCardReadWriteMachineTestMaintainDlg(this));

	//���ò��Ի�����
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MAIN,_opl(GUIDE_ENTER_F3_ESC));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MAIN,IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE,IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION,IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);

	//���û���Ťת
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_MAIN,WM_RWMGR_MAIN_TO_BASE,DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_RWMGR_BASE_TO_OPERATION,DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION,WM_RWMGR_OPERATION_TO_BASE,DIALOG_GROUP_BASE);
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE,WM_RWMGR_BASE_TO_MAIN,DIALOG_GROUP_MAIN);

	//���ó�ʼ������
	//m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MAIN);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

	DeviceType = theMAINTENANCE_INFO.GetDeviceType();		// ��ȡ�豸����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::~CRWTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����һ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::OnStart(){
	__super::OnStart();
	
	m_bIsSupperUser = false;
	//���ó�ʼ������
	//m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MAIN);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	m_Modol = Modol();
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();		// ��ȡ�豸����
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	m_Modol.timesstatus = 1;
	SetPageInfo(2,m_Modol.timesstatus);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����ģ�ͣ�����ֵ����

@param      none

@retval     Modol& m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::Modol& CRWTestSvc::GetDataModol()
{
	return m_Modol;
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
//LRESULT CRWTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Enter����Ӧ�¼�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRWTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	try
	{
		//RW_RSP_STATUS RWStatus;
		//long errCode = 0;
		//errCode = RECHARGERW_HELPER->GetStatus(RWStatus);
		//if(E_NO_CH_ERROR != errCode)	// ������벻Ϊ0
		//{
		//	INTERNAL_ERROR(errCode);	// �ж���Ӳ���쳣������쳣�ֱ���
		//}

		if (GetCurDialogGroupID() == DIALOG_GROUP_MAIN)
		{
			DoMainMenu();
		}
		else if (GetCurDialogGroupID() == DIALOG_GROUP_BASE)
		{
			DoBaseMenu();
		}
		return TRUE;
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_IO_OPEN_PORT_ERROR));
		// ֽ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_IO_OPEN_PORT_ERROR));
		// ֽ��ģ�����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      F3������Ӧ�¼�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRWTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MAIN)
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	else if (GetCurDialogGroupID() == DIALOG_GROUP_BASE)
	{
		//DoDialogFlow(WM_RWMGR_BASE_TO_MAIN);
		//ResetModolData();   
		theAPP_SESSION.ReturnMaintainMenu();
	}else if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	{
		m_Modol.timesstatus = 1;
		DoDialogFlow(WM_RWMGR_OPERATION_TO_BASE);
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F5] ��������Ϣ����һҳ��

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRWTestSvc::OnKeyboardF5(WPARAM wParam, LPARAM lParam)
{
	m_Modol.timesstatus = 1;
	ReFlashDlg();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F6] ��������Ϣ����һҳ��

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRWTestSvc::OnKeyboardF6(WPARAM wParam, LPARAM lParam)
{
	m_Modol.timesstatus = 2;
	ReFlashDlg();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����˵�ѡ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::DoMainMenu()
{
	CCardReadWriteMachineTestMaintainDlg* pDlg = dynamic_cast<CCardReadWriteMachineTestMaintainDlg*>(m_pDialogFlow->GetDialog(CCardReadWriteMachineTestMaintainDlg::IDD));

	bool bIsOk = true;
	int index = theAPP_SESSION.GetInputToInt();

	switch(index)
	{
	case 1:
		{
			m_Modol.rwType = RECHARGE_RW;
		}
		break;
	case 2:
		{
			if(DEVICE_TYPE_AVM == DeviceType)
			{
				bIsOk = false;
				break;
			}
			m_Modol.rwType = CARD_RW;
		}
		break;
		/*case 3:
		{
		if(DEVICE_TYPE_AVM == DeviceType)
		{
		bIsOk = false;
		break;
		}
		m_Modol.rwType = TOKEN_RW;
		}
		break;*/
	default:
		{
			m_Modol.rwType = UNKNOW_RW;
			bIsOk = false;
		}
		break;
	}

	if (!bIsOk)
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);   //��ʾ����˵��Ƿ�
		return;
	}
	//���û���Ťת
	DoDialogFlow(WM_RWMGR_MAIN_TO_BASE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ʵ�ֲ˵�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::DoBaseMenu()
{
	CCardReadWriteMachineTestMaintainDlg* pDlg = dynamic_cast<CCardReadWriteMachineTestMaintainDlg*>(m_pDialogFlow->GetDialog(CCardReadWriteMachineTestMaintainDlg::IDD));

	bool bIsOk = true;
	int index = theAPP_SESSION.GetInputToInt();

	theSERVICE_MGR.SetForeServiceBusy(true);
	switch(index)
	{
		//Reset
	case 1:
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_TESTING);            
			if(RWResetTest())
			{
				m_Modol.baseMenuInfo[0].strMenuStatus = SUCCESS;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESSED);     //!!���Գɹ�
			}
			else
			{
				m_Modol.baseMenuInfo[0].strMenuStatus = FAILED;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_FAILED);       //!!����ʧ��
			}
		}
		break;
		//Read Card
	case 2:
		{
			m_Modol.operation_type = Operation_ReadCard;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_PUTCARD_ON_READER);           //!!��ʾ���ÿ�Ƭ
			if(RWReadCardTest())
			{
				m_Modol.baseMenuInfo[1].strMenuStatus = SUCCESS;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESSED);   //�������Գɹ�
			}
			else
			{
				m_Modol.baseMenuInfo[1].strMenuStatus = FAILED;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_TESTING_FAILED);      //!!����ʧ��
			}
			//theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
		//Parameter
	case 3:
		{	
			// �ǳ����û�
			if(!theAPP_SESSION.IsSuperUser()){
				bIsOk = false;
				break;
			}
			m_Modol.operation_type = Operetion_Parameter_Ver;

			RWParameterTest();
			DoDialogFlow(WM_RWMGR_BASE_TO_OPERATION);
		}
		break;
		//RW info
	case 4:
		{
			// �ǳ����û�
			if(!theAPP_SESSION.IsSuperUser()){
				bIsOk = false;
				break;
			}
			m_Modol.operation_type = Operation_RW_Info;

			RWInfoTest();

			DoDialogFlow(WM_RWMGR_BASE_TO_OPERATION);
		}
		break;
	default:
		{
			bIsOk = false;
		}
		break;
	}

	theSERVICE_MGR.SetForeServiceBusy(false);

	if (!bIsOk)
	{

		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);       //������ʾ�������˵�
		return;
	}

	ReFlashDlg();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      RW���ò���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CRWTestSvc::RWResetTest()
{
	switch(m_Modol.rwType)
	{
		case RECHARGE_RW:
			{
				try
				{
					RECHARGERW_HELPER->Close();        //��ʼ��ǰ��Ҫ�ȹر�����
					RECHARGERW_HELPER->InitCheck(true);//����
					RECHARGERW_HELPER->InitData();
					CString comment(_T("RECHARGE_RW-ResetTest"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
				}
				catch(CSysException& e)
				{
					CString comment(_T("RECHARGE_RW-ResetTest"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
					theEXCEPTION_MGR.ProcessException(e);
					return false;
				}				
			}
			break;
	case CARD_RW:
		{
			try
			{
				CARDRW_HELPER->Close();           //��ʼ��ǰ��Ҫ�ȹر�����
				CARDRW_HELPER->InitCheck(true);   //����
				// ��ʼ��
				CARDRW_HELPER->InitData();

				CString comment(_T("RECHARGE_RW-ResetTest"));
				// ����������--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);

			}
			catch(CSysException& e)
			{
				CString comment(_T("RECHARGE_RW-ResetTest"));
				// ����������--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
				theEXCEPTION_MGR.ProcessException(e);
				return false;
			}
		}
		break;
		//case TOKEN_RW:
		//	{
		//		try
		//		{
		//			TOKENRW_HELPER->Close();           //��ʼ��ǰ��Ҫ�ȹر�����
		//			TOKENRW_HELPER->InitCheck(true);   //����

		//			CString comment(_T("RECHARGE_RW-ResetTest"));
		//			// ����������--������־
		//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
		//		}
		//		catch(CSysException& e)
		//		{
		//			CString comment(_T("RECHARGE_RW-ResetTest"));
		//			// ����������--������־
		//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
		//			theEXCEPTION_MGR.ProcessException(e);
		//			return false;
		//		}
		//	}
		//	break;
	default:
		break;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      RW��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CRWTestSvc::RWReadCardTest()
{
	switch(m_Modol.rwType)
	{
		case RECHARGE_RW:
			{
				try
				{
					/*RECHARGERW_HELPER->Close();
					RECHARGERW_HELPER->InitCheck();*/
					RW_READ_CARD_RESPONSE cardRspInfo_TK;
					long lErrCode = -1;
					int nTryTimes = 1;//���Զ�10�ο������������Զ�����10�Σ������ھͷ��ض���ʧ�ܡ�
					while(true)
					{
						lErrCode = RECHARGERW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),cardRspInfo_TK,false);
						if(lErrCode != RW_RSP_OK)
						{
							if (lErrCode == RW_RSP_FINDERROR)
							{
								if (nTryTimes>0)
								{
									--nTryTimes;
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
						else
						{
							break;
						}
					}
					CString comment(_T("RECHARGE_RW-RWReadCardTes"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
				}

				catch(CSysException& e)
				{
					CString comment(_T("RECHARGE_RW-RWReadCardTes"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
					theEXCEPTION_MGR.ProcessException(e);
					return false;
				}				
			}
			break;
	case CARD_RW:
		{
			try
			{
				/*CARDRW_HELPER->Close();
				CARDRW_HELPER->InitCheck();*/
				RW_READ_CARD_RESPONSE cardRspInfo_TK;
				long lErrCode = -1;
				int nTryTimes = 1;//���Զ�1�ο������������Զ�����10�Σ������ھͷ��ض���ʧ�ܡ�
				while(true)
				{
					lErrCode = CARDRW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),cardRspInfo_TK,false);
					if(lErrCode != RW_RSP_OK)
					{
						if (lErrCode == RW_RSP_FINDERROR)
						{
							if (nTryTimes>0)
							{
								--nTryTimes;
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						break;
					}
				}
				CString comment(_T("CARD_RW-RWReadCardTes"));
				// ����������--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
			}

			catch(CSysException& e)
			{
				CString comment(_T("CARD_RW-RWReadCardTes"));
				// ����������--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
				theEXCEPTION_MGR.ProcessException(e);
				return false;
			}
		}
		break;
		//case TOKEN_RW:
		//	{
		//		try
		//		{
		//			/*TOKENRW_HELPER->Close();
		//			TOKENRW_HELPER->InitCheck();*/
		//			RW_READ_CARD_RESPONSE cardRspInfo_TK;
		//			long lErrCode = -1;
		//			int nTryTimes = 1;//���Զ�10�ο������������Զ�����10�Σ������ھͷ��ض���ʧ�ܡ�
		//			while(true)
		//			{
		//				lErrCode = TOKENRW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),cardRspInfo_TK,false);
		//				if(lErrCode != RW_RSP_OK)
		//				{
		//					/*if (lErrCode == RW_RSP_FINDERROR)
		//					{
		//					if (nTryTimes>0)
		//					{
		//					--nTryTimes;
		//					}
		//					else
		//					{
		//					return false;
		//					}
		//					}
		//					else
		//					{
		//					return false;
		//					}*/
		//				}
		//				else
		//				{
		//					break;
		//				}
		//			}
		//			CString comment(_T("TOKEN_RW-RWReadCardTes"));
		//			// ����������--������־
		//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
		//		}

		//		catch(CSysException& e)
		//		{
		//			CString comment(_T("TOKEN_RW-RWReadCardTes"));
		//			// ����������--������־
		//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
		//			theEXCEPTION_MGR.ProcessException(e);
		//			return false;
		//		}
		//	}
		//	break;
	default:
		break;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Parameter��Ϣ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::RWParameterTest()
{
	try{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_WAITING_TO_READ);    //!!���ڶ���...

		VERSION_ACC accVersion;
		VERSION_ECT etcVersion;
		VERSION_AFC afcVersion;

		switch(m_Modol.rwType)
		{
			case RECHARGE_RW:
				{
					long errorCode = -1;
					errorCode = RECHARGERW_HELPER->GetParameterInfo(accVersion,etcVersion,afcVersion);
					if (errorCode == 0)
					{
						int index = 0;
						// TPU����
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lBusinessParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lServerFeeParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lOverTimeAdjuestParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lDeviceControlParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lStationParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lPayStationParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lSectionParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lCardAttributeParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lissueCompanyParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lBasePriceParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lPrieLevelParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lPriceAdjuestParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lTimeFloatParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lTimeTypeParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lTimeDetailParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lDiscountParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lNotServStationParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lSingleBlackListParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lSectionBlackListParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lWriteListParam);
						m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lOtherCityCardParam);

						m_Modol.baseMenuInfo[2].strMenuStatus = COMPLETED;
						//theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESS);   //!!������Ϣ�ɹ�

						CString comment(_T("RECHARGE_RW-RWParameterTest"));
						// ����������--������־
						theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
					}
					else
					{
						m_Modol.baseMenuInfo[2].strMenuStatus = FAILED;
						CString comment(_T("RECHARGE_RW-RWParameterTest"));
						// ����������--������־
						theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
					}
				}
				break;
		case CARD_RW:
			{
				long errorCode = -1;
				errorCode = CARDRW_HELPER->GetParameterInfo(accVersion,etcVersion,afcVersion);
				if (errorCode == 0)
				{
					int index = 0;
					// TPU����
					m_Modol.paramterInfo[index].strID   = _T("0101");
					m_Modol.paramterInfo[index].strName = _T("��Ӫ����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lBusinessParam);

					m_Modol.paramterInfo[index].strID   = _T("0102");
					m_Modol.paramterInfo[index].strName = _T("�������������Ѳ���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lServerFeeParam);

					m_Modol.paramterInfo[index].strID   = _T("0103");
					m_Modol.paramterInfo[index].strName = _T("��ʱ�������ò���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lOverTimeAdjuestParam);

					m_Modol.paramterInfo[index].strID   = _T("0201");
					m_Modol.paramterInfo[index].strName = _T("�豸���Ʋ���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lDeviceControlParam);

					m_Modol.paramterInfo[index].strID   = _T("0301");
					m_Modol.paramterInfo[index].strName = _T("��վλ�ò���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lStationParam);

					m_Modol.paramterInfo[index].strID   = _T("0302");
					m_Modol.paramterInfo[index].strName = _T("�Ʒ�վ�����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lPayStationParam);

					m_Modol.paramterInfo[index].strID   = _T("0303");
					m_Modol.paramterInfo[index].strName = _T("���β���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lSectionParam);

					m_Modol.paramterInfo[index].strID   = _T("0401");
					m_Modol.paramterInfo[index].strName = _T("Ʊ�����Բ���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lCardAttributeParam);

					m_Modol.paramterInfo[index].strID   = _T("0501");
					m_Modol.paramterInfo[index].strName = _T("Ʊ�������̲���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lissueCompanyParam);

					m_Modol.paramterInfo[index].strID   = _T("0601");
					m_Modol.paramterInfo[index].strName = _T("����Ʊ�۱����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lBasePriceParam);
					
					m_Modol.paramterInfo[index].strID   = _T("0602");
					m_Modol.paramterInfo[index].strName = _T("Ʊ�ۼ�������");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lPrieLevelParam);

					m_Modol.paramterInfo[index].strID   = _T("0603");
					m_Modol.paramterInfo[index].strName = _T("Ʊ�۵������Ա����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lPriceAdjuestParam);

					m_Modol.paramterInfo[index].strID   = _T("0604");
					m_Modol.paramterInfo[index].strName = _T("ʱ�両�������");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lTimeFloatParam);

					m_Modol.paramterInfo[index].strID   = _T("0605");
					m_Modol.paramterInfo[index].strName = _T("ʱ�����ͱ����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lTimeTypeParam);

					m_Modol.paramterInfo[index].strID   = _T("0606");
					m_Modol.paramterInfo[index].strName = _T("ʱ��������ϸ����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lTimeDetailParam);

					m_Modol.paramterInfo[index].strID   = _T("0607");
					m_Modol.paramterInfo[index].strName = _T("�ۿ��ʱ����");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lDiscountParam);

					m_Modol.paramterInfo[index].strID   = _T("0608");
					m_Modol.paramterInfo[index].strName = _T("ͣ�˳�վ��");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lNotServStationParam);

					m_Modol.paramterInfo[index].strID   = _T("0701");
					m_Modol.paramterInfo[index].strName = _T("��������������");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lSingleBlackListParam);

					m_Modol.paramterInfo[index].strID   = _T("0702");
					m_Modol.paramterInfo[index].strName = _T("���κ���������");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lSectionBlackListParam);

					m_Modol.paramterInfo[index].strID   = _T("0801");
					m_Modol.paramterInfo[index].strName = _T("����������");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lWriteListParam);

					m_Modol.paramterInfo[index].strID   = _T("0802");
					m_Modol.paramterInfo[index].strName = _T("��ؿ�/���д�����ղ���");
					m_Modol.paramterInfo[index++].strVersion.Format(_T("%d"),accVersion.lOtherCityCardParam);

					m_Modol.baseMenuInfo[2].strMenuStatus = SUCCESS;
					//theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESS);
					CString comment(_T("CARD_RW-RWParameterTest"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
				}
				else
				{
					m_Modol.baseMenuInfo[2].strMenuStatus = FAILED;
					CString comment(_T("CARD_RW-RWParameterTest"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
				}
			}
			break;
			//case TOKEN_RW:
			//	{
			//		long errorCode = -1;
			//		errorCode = TOKENRW_HELPER->GetParameterInfo(accVersion,etcVersion,afcVersion);
			//		if (errorCode == 0)
			//		{
			//			m_Modol.paramterInfo[0].strVersion.Format(_T("%d"),accVersion.lSystemParam);
			//			m_Modol.paramterInfo[1].strVersion.Format(_T("%d"),accVersion.lBusinessParam);
			//			m_Modol.paramterInfo[2].strVersion.Format(_T("%d"),accVersion.lVersionParam);
			//			m_Modol.paramterInfo[3].strVersion.Format(_T("%d"),accVersion.lBlackListParam);
			//			m_Modol.paramterInfo[4].strVersion.Format(_T("%d"),accVersion.lProductParam);
			//			m_Modol.paramterInfo[5].strVersion.Format(_T("%d"),accVersion.lLocationParam);
			//			m_Modol.paramterInfo[6].strVersion.Format(_T("%d"),accVersion.lCalanderParam);
			//			m_Modol.paramterInfo[7].strVersion.Format(_T("%d"),accVersion.lShopParam);
			//			m_Modol.paramterInfo[8].strVersion.Format(_T("%d"),accVersion.lSalesParam);
			//			m_Modol.paramterInfo[9].strVersion.Format(_T("%d"),afcVersion.lModeHistoryParam);

			//			m_Modol.baseMenuInfo[2].strMenuStatus = COMPLETED;
			//			//theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESS);
			//			CString comment(_T("TOKEN_RW-RWParameterTest"));
			//			// ����������--������־
			//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
			//		}
			//		else
			//		{
			//			m_Modol.baseMenuInfo[2].strMenuStatus = FAILED;
			//			CString comment(_T("TOKEN_RW-RWParameterTest"));
			//			// ����������--������־
			//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
			//		}
			//	
			//	}
			//	break;
		default:
			break;
		}
	}
	catch(CSysException& e){
		m_Modol.baseMenuInfo[2].strMenuStatus = FAILED;
		theEXCEPTION_MGR.ProcessException(e);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      RW��Ϣ��ȡ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::RWInfoTest()
{
	try{
		long errorCode = -1;
		theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_WAITING_TO_READ);        //!!���ڶ���...

		RW_RSP_TPU_INFO tpuInfo;
		RW_RSP_STATUS rwstatus;
		errorCode = CARDRW_HELPER->Reset(rwstatus);
		switch(m_Modol.rwType)
		{
			case RECHARGE_RW:
				{
					errorCode = RECHARGERW_HELPER->GetTPUInfo(tpuInfo);
					if (errorCode == 0)
					{
						m_Modol.rwInfo[0].strRWValue.Format(_T("0x%04s"),tpuInfo.DeviceCode);
						m_Modol.rwInfo[1].strRWValue.Format(_T("%d"),ComMakeWORD(tpuInfo.softwareVersionApp[1],tpuInfo.softwareVersionApp[0]));
						m_Modol.rwInfo[2].strRWValue.Format(_T("%d"),ComMakeWORD(tpuInfo.hardwareVersion[1],tpuInfo.hardwareVersion[0]));
						CString TPUISA_Num=_T("");
						CString ISANum = _T("");
						for(unsigned int i=0;i<6;i++)
						{
							ISANum.Format(_T("%02x"),*(tpuInfo.accISAMid+i));
							TPUISA_Num+=ISANum;
						}
						m_Modol.rwInfo[3].strRWValue = TPUISA_Num;
						m_Modol.baseMenuInfo[3].strMenuStatus = COMPLETED;
						if(_T("000000000000") == TPUISA_Num)
						{
							m_Modol.rwInfo[3].strRWValue = _opl(GUIDE_RW_ISAM_NO_INSTALL);
							m_Modol.baseMenuInfo[3].strMenuStatus = FAILED;
						}
						//theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESS); //!!...����info�ɹ�
						CString comment(_T("RECHARGE_RW-RWInfoTest"));
						// ����������--������־
						theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
					}
					else
					{
						m_Modol.baseMenuInfo[3].strMenuStatus = FAILED;
						CString comment(_T("RECHARGE_RW-RWInfoTest"));
						// ����������--������־
						theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
					}
				}
				break;
		case CARD_RW:
			{
				errorCode = CARDRW_HELPER->GetTPUInfo(tpuInfo);
				if (errorCode == 0)
				{
					m_Modol.rwInfo[0].strRWValue.Format(_T("%02x%02x%02x%02x"),tpuInfo.DeviceCode[0],tpuInfo.DeviceCode[1],tpuInfo.DeviceCode[2],tpuInfo.DeviceCode[3]);
					CString strVer;
					strVer.Format(_T("%01X%02X%02X%02X%02X"),tpuInfo.softwareVersionApp[1] & 0x0F,tpuInfo.softwareVersionApp[2],tpuInfo.softwareVersionApp[3],tpuInfo.softwareVersionApp[4],tpuInfo.softwareVersionApp[5]);		
					m_Modol.rwInfo[1].strRWValue.Format(_T("%d"), _ttoi(strVer));
					strVer.Format(_T("%02X%02X"),tpuInfo.hardwareVersion[0],tpuInfo.hardwareVersion[1]);	
					m_Modol.rwInfo[2].strRWValue.Format(_T("%d"), _ttoi(strVer));
					CString TPUISA_Num=_T("");/*=_T("0x")*/;
					CString ISANum = _T("");
					for(unsigned int i=0;i<6;i++){
						ISANum.Format(_T("%02x"),*(tpuInfo.accISAMid+i));
						TPUISA_Num+=ISANum;
					}
					if(_T("000000000000") == TPUISA_Num){
						m_Modol.rwInfo[3].strRWValue = _opl(GUIDE_RW_ISAM_NO_INSTALL);
					}
					else{
						m_Modol.rwInfo[3].strRWValue = TPUISA_Num;
					}

					TPUISA_Num=_T("");
					for(unsigned int i=0;i<6;i++){
						ISANum.Format(_T("%02x"),*(tpuInfo.accPSAMid+i));
						TPUISA_Num+=ISANum;
					}
					if(_T("000000000000") == TPUISA_Num){
						m_Modol.rwInfo[4].strRWValue = _opl(GUIDE_RW_ISAM_NO_INSTALL);
					}
					else{
						m_Modol.rwInfo[4].strRWValue = TPUISA_Num;
					}

					TPUISA_Num=_T("");
					for(unsigned int i=0;i<6;i++){
						ISANum.Format(_T("%02x"),*(tpuInfo.bmacPSAMid+i));
						TPUISA_Num+=ISANum;
					}
					if(_T("000000000000") == TPUISA_Num){
						m_Modol.rwInfo[5].strRWValue = _opl(GUIDE_RW_ISAM_NO_INSTALL);
					}
					else{
						m_Modol.rwInfo[5].strRWValue = TPUISA_Num;
					}

					m_Modol.baseMenuInfo[3].strMenuStatus = SUCCESS;
					//theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESS);
					CString comment(_T("CARD_RW-RWInfoTest"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
				}
				else
				{
					m_Modol.baseMenuInfo[3].strMenuStatus = FAILED;
					CString comment(_T("CARD_RW-RWInfoTest"));
					// ����������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
				}
			}
			break;
			//case TOKEN_RW:
			//	{
			//		errorCode = TOKENRW_HELPER->GetTPUInfo(tpuInfo);
			//		if (errorCode == 0)
			//		{
			//			m_Modol.rwInfo[0].strRWValue.Format(_T("0x%04s"),tpuInfo.DeviceCode);
			//			m_Modol.rwInfo[1].strRWValue.Format(_T("%d"),ComMakeWORD(tpuInfo.softwareVersionApp[1],tpuInfo.softwareVersionApp[0]));
			//			m_Modol.rwInfo[2].strRWValue.Format(_T("%d"),ComMakeWORD(tpuInfo.hardwareVersion[1],tpuInfo.hardwareVersion[0]));
			//			CString TPUISA_Num=_T("");
			//			CString ISANum = _T("");
			//			for(unsigned int i=0;i<6;i++)
			//			{
			//				ISANum.Format(_T("%02x"),*(tpuInfo.accISAMid+i));
			//				TPUISA_Num+=ISANum;
			//			}
			//			m_Modol.rwInfo[3].strRWValue = TPUISA_Num;
			//			m_Modol.baseMenuInfo[3].strMenuStatus = COMPLETED;
			//			//theAPP_SESSION.ShowOfficeGuide(GUIDE_RW_TEST_SUCCESS);
			//			CString comment(_T("TOKEN_RW-RWInfoTest"));
			//			// ����������--������־
			//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,true,comment);
			//		}
			//		else
			//		{
			//			m_Modol.baseMenuInfo[3].strMenuStatus = FAILED;
			//			CString comment(_T("TOKEN_RW-RWInfoTest"));
			//			// ����������--������־
			//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_RW_TEST,false,comment);
			//		}
			//	}
			//	break;
		default:
			break;
		}
	}
	catch(CSysException& e){
		m_Modol.baseMenuInfo[3].strMenuStatus = FAILED;
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ģ���еĿɱ�ֵ��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::ResetModolData()
{
	for (int i = 0 ; i < 10 ; i++)
	{
		if (i<4)
		{
			m_Modol.baseMenuInfo[i].strMenuStatus = _T("");
			m_Modol.rwInfo[i].strRWValue          = _T("");
		}
		m_Modol.paramterInfo[i].strVersion    = _T("");
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ɲ���ʱ�����½���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::ReFlashDlg()
{
	CCardReadWriteMachineTestMaintainDlg* pDlg = dynamic_cast<CCardReadWriteMachineTestMaintainDlg*>(m_pDialogFlow->GetDialog(CCardReadWriteMachineTestMaintainDlg::IDD));
	pDlg->UpdataUI();
}
