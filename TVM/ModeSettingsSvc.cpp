#include "StdAfx.h"
#include "ModeSettingsSvc.h"
#include "ModeSettingsDlg.h"

static const TCHAR* const BASE_TO_SVC = _T("BASE_TO_SVC");
static const TCHAR* const BASE_TO_CHANGE = _T("BASE_TO_CHANGE");
static const TCHAR* const BASE_TO_PAYMENT = _T("BASE_TO_PAYMENT");
static const TCHAR* const BASE_TO_PRINT = _T("BASE_TO_PRINT");
static const TCHAR* const SVC_TO_BASE = _T("SVC_TO_BASE");
static const TCHAR* const CHANGE_TO_BASE = _T("CHANGE_TO_BASE");
static const TCHAR* const PRINT_TO_BASE = _T("PRINT_TO_BASE");
static const TCHAR* const PAYMENT_TO_BASE = _T("PAYMENT_TO_BASE");

BEGIN_MESSAGE_MAP(CModeSettingsSvc,CTVMForeService)
END_MESSAGE_MAP()

CModeSettingsSvc::CModeSettingsSvc(void)
	:CTVMForeService(RUN_MODE_SETTING_SVC)
{
	m_pDialogFlow->AddDialog(CModeSettingsDlg::IDD, new CModeSettingsDlg(this));
	//�������
	m_pDialogFlow->AddDialogGroup(DLG_GROUP_BASE, _opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DLG_GROUP_BASE, CModeSettingsDlg::IDD);	//ģʽ����

	m_pDialogFlow->AddDialogGroup(DLG_GROUP_CHANGE, _opl(GUIDE_ENTER_F2_F3_ESC));//����ģʽ
	m_pDialogFlow->AddDialogGroupMember(DLG_GROUP_CHANGE, CModeSettingsDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DLG_GROUP_PAYMENT, _opl(GUIDE_ENTER_F2_F3_ESC));//֧��ģʽ
	m_pDialogFlow->AddDialogGroupMember(DLG_GROUP_PAYMENT, CModeSettingsDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DLG_GROUP_PRINT, _opl(GUIDE_ENTER_F2_F3_ESC)); //��ӡģʽ
	m_pDialogFlow->AddDialogGroupMember(DLG_GROUP_PRINT, CModeSettingsDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DLG_GROUP_SERVICE, _opl(GUIDE_ENTER_F2_F3_ESC));// ҵ��ģʽ
	m_pDialogFlow->AddDialogGroupMember(DLG_GROUP_SERVICE, CModeSettingsDlg::IDD);
	

	m_pDialogFlow->AddFlow(DLG_GROUP_BASE,BASE_TO_SVC, DLG_GROUP_SERVICE);
	m_pDialogFlow->AddFlow(DLG_GROUP_SERVICE, SVC_TO_BASE, DLG_GROUP_BASE);

	m_pDialogFlow->AddFlow(DLG_GROUP_BASE, BASE_TO_CHANGE, DLG_GROUP_CHANGE);
	m_pDialogFlow->AddFlow(DLG_GROUP_CHANGE, CHANGE_TO_BASE,DLG_GROUP_BASE );

	m_pDialogFlow->AddFlow(DLG_GROUP_BASE, BASE_TO_PAYMENT, DLG_GROUP_PAYMENT);
	m_pDialogFlow->AddFlow(DLG_GROUP_PAYMENT, PAYMENT_TO_BASE,DLG_GROUP_BASE );

	m_pDialogFlow->AddFlow(DLG_GROUP_BASE, BASE_TO_PRINT, DLG_GROUP_PRINT);
	m_pDialogFlow->AddFlow(DLG_GROUP_PRINT, PRINT_TO_BASE,DLG_GROUP_BASE );

	m_pDialogFlow->SetFirstDialogGroup(DLG_GROUP_BASE);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_F2);
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();
	m_mapDvcCode.clear();
}


CModeSettingsSvc::~CModeSettingsSvc(void)
{
	m_mapDvcCode.clear();
}

void CModeSettingsSvc::OnStart()
{
	__super::OnStart();
	m_pDialogFlow->SetFirstDialogGroup(DLG_GROUP_BASE);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_F2);
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();
	m_mapDvcCode.clear();
	SetPageInfo(1);
	m_pDialogFlow->ShowFirstDialogGroup();
	LoadItem(m_SvcData.mapItem, DLG_GROUP_BASE);
	NotifyDataModelChanged();
}

void CModeSettingsSvc::ClearPage()
{
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ENTER������Ӧ
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CModeSettingsSvc::OnKeyboardEnter(WPARAM, LPARAM)
{
	UINT dlg = GetCurDialogGroupID();
	int nIndex = theAPP_SESSION.GetInputToInt();
	switch(dlg)
	{
	case DLG_GROUP_BASE:
		switch(nIndex)
		{
		case 1:
			theAPP_SESSION.AppendMenuLevel(_opl(TXT_ID_TVM_PAYMENT_MODE_SETTING_SVC));
			DoDialogFlow(BASE_TO_PAYMENT);break;//֧��
			break;
		case 2:
			theAPP_SESSION.AppendMenuLevel(_opl(TXT_ID_TVM_SERVICE_MODE_SETTING_SVC));
			DoDialogFlow(BASE_TO_SVC);break;//ҵ��ģʽ����
			break;
		//case 1:
		//	theAPP_SESSION.AppendMenuLevel(_opl(TXT_ID_TVM_SERVICE_MODE_SETTING_SVC));
		//	DoDialogFlow(BASE_TO_SVC);break;//ҵ��ģʽ����
		//case 2:
			//theAPP_SESSION.AppendMenuLevel(_opl(TXT_ID_TVM_PAYMENT_MODE_SETTING_SVC));
			//DoDialogFlow(BASE_TO_PAYMENT);break;//֧��
		//case 3:
		//	theAPP_SESSION.AppendMenuLevel(_opl(TXT_ID_TVM_CHANGE_MODE_SETTING_SVC));
		//	DoDialogFlow(BASE_TO_CHANGE);
		//	break;//����
		//case 4:
		//	theAPP_SESSION.AppendMenuLevel(_opl(TXT_ID_TVM_PRINT_MODE_SETTING_SVC));
		//	DoDialogFlow(BASE_TO_PRINT);break;//��ӡ
		default:
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
			return FALSE;
			//break;
		}
		LoadItem(m_SvcData.mapItem, (DLG_GROUP)GetCurDialogGroupID());
		break;
	case DLG_GROUP_SERVICE:
	case DLG_GROUP_CHANGE:
	case DLG_GROUP_PAYMENT:
	case DLG_GROUP_PRINT:
		if ( nIndex/10 <= 0 )//����λ��С��2
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
			return 1;
		}
		SetSvcWorkMode(nIndex);		
		break;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	NotifyDataModelChanged();
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      F2���ܰ�ť��Ӧ����
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CModeSettingsSvc::OnKeyboardF2(WPARAM, LPARAM)
{
	UINT dlg = GetCurDialogGroupID();
	int nIndex = theAPP_SESSION.GetInputToInt();
	switch(dlg)
	{
	case DLG_GROUP_BASE:
		break;
	case DLG_GROUP_SERVICE:
	case DLG_GROUP_CHANGE:
	case DLG_GROUP_PAYMENT:
	case DLG_GROUP_PRINT:
		//SetSvcWorkMode(nIndex);
		SetItemRunMode();
		break;
	}
	NotifyDataModelChanged();
	LoadItem(m_SvcData.mapItem, (DLG_GROUP)GetCurDialogGroupID());
	//��ʾ����ɹ�
	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_DEVICE_SAVE_SUCCESS_NOT_RESTART));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��������������ϵ���ʾ��Ŀ
@param      map<int, SELECTION>& mapItem ���� ��������	 [1]ON  [2]OFF			ON"��ʽ��ѡ��
			DLG_GROUP dlg	��ǰ������ID
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsSvc::LoadItem(map<int, SELECTION>& mapItem, DLG_GROUP dlg)
{
	mapItem.clear();
	SELECTION slc;
	switch(dlg)
	{
	case DLG_GROUP_BASE:			//�������û�������
		slc.strItem = _opl(TXT_ID_TVM_PAYMENT_MODE_SETTING_SVC);
		slc.Statuse = UNDEFINE;
		mapItem[1] = slc;
		slc.strItem = _opl(TXT_ID_TVM_SERVICE_MODE_SETTING_SVC);
		slc.Statuse = UNDEFINE;
		mapItem[2] = slc;

		//slc.strItem = _opl(TXT_ID_TVM_SERVICE_MODE_SETTING_SVC);
		//slc.Statuse = UNDEFINE;
		//mapItem[1] = slc;
		//slc.strItem = _opl(TXT_ID_TVM_PAYMENT_MODE_SETTING_SVC);
		//slc.Statuse = UNDEFINE;
		//mapItem[2] = slc;
		///*if(DEVICE_TYPE_AVM == DeviceType)
		//{
		//	slc.strItem = _opl(TXT_ID_TVM_PRINT_MODE_SETTING_SVC);
		//	slc.Statuse = UNDEFINE;
		//	mapItem[3] = slc;
		//	break;
		//}*/
		//slc.strItem = _opl(TXT_ID_TVM_CHANGE_MODE_SETTING_SVC);
		//slc.Statuse = UNDEFINE;
		//mapItem[3] = slc;
		//slc.strItem = _opl(TXT_ID_TVM_PRINT_MODE_SETTING_SVC);
		//slc.Statuse = UNDEFINE;
		//mapItem[4] = slc;
		break;
	case DLG_GROUP_SERVICE:			//ҵ��ģʽ
		slc.strItem = _T("��ͣ����") + _opl(TXT_COLON);
		slc.Statuse = theTVM_SETTING.IsSCServiceOutOfService()?ON:OFF;
		slc.code = (DEVICE_CONTRL_CODE)1;
		mapItem[1] = slc;
		break;
//		if(DEVICE_TYPE_AVM == DeviceType)
//		{
//			slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_RECHARGE_CARD)+ _opl(TXT_COLON);
//			slc.Statuse = GetCurSvcStatus(RECHARGE_CARD);
//			slc.code = DEVICE_CONTRL_CODE_BUSINESS_CHARGE;
//			mapItem[1] = slc;
//
//			slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_QUERY_INFO)+ _opl(TXT_COLON);
//			slc.Statuse = GetCurSvcStatus(QUERY_INFO);
////			slc.code = DEVICE_CONTRL_CODE_NO_COIN_QUERY;
//			mapItem[2] = slc;
//			break;
//		}
		/*
		slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_ISSUE_TOKEN)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(ISSUE_TOKEN);
//		slc.code = DEVICE_CONTRL_CODE_BUSINESS_TOKEN;
		mapItem[1] = slc;*/

		slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_ISSUE_CARD)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(ISSUE_CARD);
		slc.code = DEVICE_CONTRL_CODE_BUSINESS_CARD;
		mapItem[1] = slc;

		/*
		slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_ADD_PRODUCT)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(ADD_PRODUCT);
//		slc.code = DEVICE_CONTRL_CODE_BUSINESS_PRODUCT;
		mapItem[3] = slc;*/

		slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_RECHARGE_CARD)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(RECHARGE_CARD);
		slc.code = DEVICE_CONTRL_CODE_BUSINESS_CHARGE;
		mapItem[2] = slc;

		
		slc.strItem = _opl(TXT_ID_TVM_MODE_SVC_QUERY_INFO)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(QUERY_INFO);
		slc.code = DEVICE_CONTRL_CODE_BUSINESS_QUERY;
		mapItem[3] = slc;
		break;
	case DLG_GROUP_CHANGE:		//����ģʽ
		// Ӳ��
		slc.strItem = _opl(TXT_ID_TVM_MODE_COIN_CHANGE) + _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(COIN_CHANGE);
		slc.code	= DEVICE_CONTRL_CODE_NO_BILL_CHANGE;
		mapItem[1] = slc;

		// ֽ��
		slc.strItem = _opl(TXT_ID_TVM_MODE_BANKNOTE_CHANGE) + _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(BANKNOTE_CHANGE);
		slc.code    = DEVICE_CONTRL_CODE_NO_COIN_CHANGE;
		mapItem[2] = slc;
		break;
	case DLG_GROUP_PAYMENT:		//֧��ģʽ
//		if(DEVICE_TYPE_AVM == DeviceType)
//		{
//			slc.strItem = _opl(TXT_ID_TVM_MODE_PAYMENT_BANKNOTE) + _opl(TXT_COLON);
//			slc.Statuse = GetCurSvcStatus(BANKNOTE);
//			slc.code = DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
//			mapItem[1] = slc;
//
//			slc.strItem = _opl(TXT_ID_TVM_MODE_PAYMENT_BANKCARD)+ _opl(TXT_COLON);
//			slc.Statuse = GetCurSvcStatus(BANKCARD);
////			slc.code = DEVICE_CONTRL_CODE_PAYMENT_BANKCARD;
//			mapItem[2] = slc;
//			break;
//		}
		slc.strItem = _opl(TXT_ID_TVM_MODE_PAYMENT_COIN)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(COIN);
		slc.code = DEVICE_CONTRL_CODE_PAYMENT_COIN;
		mapItem[1] = slc;

		slc.strItem = _opl(TXT_ID_TVM_MODE_PAYMENT_BANKNOTE)+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(BANKNOTE);
		slc.code = DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		mapItem[2] = slc;

//		slc.strItem = _opl(TXT_ID_TVM_MODE_PAYMENT_BANKCARD)+ _opl(TXT_COLON);
//		slc.Statuse = GetCurSvcStatus(BANKCARD);
////		slc.code = DEVICE_CONTRL_CODE_PAYMENT_BANKCARD;
//		mapItem[3] = slc;
//
		slc.strItem = _T("��ֵ��")/*_opl(TXT_ID_TVM_MODE_PAYMENT_EP)*/+ _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(EP);
		slc.code = DEVICE_CONTRL_CODE_PAYMENT_SVT;
		mapItem[3] = slc;

		break;
	case DLG_GROUP_PRINT:	//��ӡģʽ
		slc.strItem = _opl(TXT_ID_TVM_MODE_PRINT/*TXT_ID_TVM_PRINT_MODE_SETTING_SVC*/) + _opl(TXT_COLON);
		slc.Statuse = GetCurSvcStatus(PRINT);
		mapItem[1] = slc;
		break;	
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ӷ���ǰ��״̬

@param      SVC_ITEM svc  �ӷ���,����ö�ٶ���
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CModeSettingsSvc::STATUS CModeSettingsSvc::GetCurSvcStatus(SVC_ITEM svc)
{
	CModeSettingsSvc::STATUS status = OFF;
	WORD wMode;
	WORD work   = theTVM_SETTING.GetServiceMode();
	WORD pay    = theTVM_SETTING.GetPayMentMode();
	WORD change = theTVM_SETTING.GetChangeMode();
	WORD print  = theTVM_SETTING.GetPrintMode();

	switch(svc)
	{
	case ISSUE_TOKEN: 
		//if( DEVICE_CONTRL_CODE_BUSINESS_TOKEN == (work & DEVICE_CONTRL_CODE_BUSINESS_TOKEN))
			status = ON; 
		break;

	case ISSUE_CARD: 
		if( DEVICE_CONTRL_CODE_BUSINESS_CARD == (work & DEVICE_CONTRL_CODE_BUSINESS_CARD))
			status = ON; 
		break;

	case RECHARGE_CARD: 
		if( DEVICE_CONTRL_CODE_BUSINESS_CHARGE== (work&DEVICE_CONTRL_CODE_BUSINESS_CHARGE))
			status = ON;
		break;

	case QUERY_INFO:  
		if( DEVICE_CONTRL_CODE_BUSINESS_QUERY == (work & DEVICE_CONTRL_CODE_BUSINESS_QUERY))
			status = ON; 
		break;

	case ADD_PRODUCT:  
		//if(DEVICE_CONTRL_CODE_BUSINESS_PRODUCT == (work & DEVICE_CONTRL_CODE_BUSINESS_PRODUCT))
			status = ON; 
		break;

	case COIN: 
		if(DEVICE_CONTRL_CODE_PAYMENT_COIN == (pay & DEVICE_CONTRL_CODE_PAYMENT_COIN))
			status = ON;
		break;

	case BANKNOTE: 
		if(DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE == (pay & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE))
			status = ON;
		break;

	case BANKCARD: 
		//if( DEVICE_CONTRL_CODE_PAYMENT_BANKCARD == (pay & DEVICE_CONTRL_CODE_PAYMENT_BANKCARD))
			status = ON;
		break;

	case EP: 
		if (DEVICE_CONTRL_CODE_PAYMENT_SVT == (pay & DEVICE_CONTRL_CODE_PAYMENT_SVT))
			status = ON;
		break;
	case PRINT:
		if( DEVICE_CONTRL_CODE_SC_NO_PRINT == /*(*/print /*& DEVICE_CONTRL_CODE_NO_PRINT)*/)
			status = OFF;
		else if(print== 0)	
			status = ON;
		break;//�޴�ӡģʽ
		// Ӳ������
	case COIN_CHANGE: 
		{
			if((change & DEVICE_CONTRL_CODE_NO_BILL_CHANGE) == DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
				status = ON;
			}
			else{
				status = OFF;
			}
		}
		break;//������ģʽ
		// ֽ������
	case BANKNOTE_CHANGE:
		{
			if((change & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) == DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
				status = ON;
			}
			else{
				status = OFF;
			}
		}
		break;
	}
	return status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���÷���ģʽ���ݣ�������

@param     int nIndex:����������ַ�
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsSvc::SetSvcWorkMode(int nIndex)
{
	WORD wSetCode = 0;
	int nItem = 0, nSwitch = 1;
	STATUS status;
	if( nIndex % 10 >= 0 && (nIndex / 10) <=  (m_SvcData.mapItem.size()) )
	{
		nItem = nIndex / 10;
		nSwitch = nIndex % 10;
		if (nSwitch > 2)
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
			Sleep(500);//���������ʾһ������
			return;
		}
		status = nSwitch <= 2 ? (STATUS)nSwitch : UNDEFINE;
		if(m_SvcData.mapItem[nItem].Statuse != status)
		{
			m_SvcData.mapItem[nItem].Statuse = status;
		}
	}
	else
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		Sleep(500);//���������ʾһ������
	}
		//��ʾ��������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ģʽ

@param    
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CModeSettingsSvc::SetItemRunMode()
{
	WORD wCode = 0;
	switch(GetCurDialogGroupID())
	{
	case DLG_GROUP_SERVICE:
		wCode = 0;
		//wCode=DEVICE_CONTRL_CODE_BUSINESS;
		break;
	case DLG_GROUP_PAYMENT:		
		wCode=DEVICE_CONTRL_CODE_PAYMENT;
		break;
	case DLG_GROUP_CHANGE:
		wCode=DEVICE_CONTRL_CODE_NO_CHANGE;
		break;
	case  DLG_GROUP_PRINT:
		wCode=0;
		break;
	default:
		break;
	}
	for(int n = 1; n <= m_SvcData.mapItem.size();n++)	//���û����õ�״̬���������洢���ж�ȡ���û����õ�״̬��������룩
	{
		if(m_SvcData.mapItem[n].Statuse == ON)
		{
			WORD wd = m_SvcData.mapItem[n].code;
			wCode |= m_SvcData.mapItem[n].code;			// �������֮�����߼�������
		}
	}
	switch(GetCurDialogGroupID())
	{
	case DLG_GROUP_SERVICE:

		//theTVM_STATUS_MGR.SetWorkMode(wCode);			// ����ģʽ��WorkMode��������
		//theSETTING.SetServiceMode(wCode);
		theTVM_SETTING.SetSCServiceStatus(wCode);
		break;
	case DLG_GROUP_PAYMENT:								// ֧��ģʽ����
		if (DEVICE_TYPE_AVM == DeviceType){
			// ȥ��Ӳ�ң�EP��Ӳ�����㣬���ÿ�
			if(wCode & DEVICE_CONTRL_CODE_PAYMENT_COIN == DEVICE_CONTRL_CODE_PAYMENT_COIN){
				wCode = (wCode ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
			}
			// 6.AVM ��Ӳ������
			if (wCode & DEVICE_CONTRL_CODE_NO_BILL_CHANGE == DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
				wCode = (wCode ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
			}
			// AVM��EP
			/*if (wCode & DEVICE_CONTRL_CODE_PAYMENT_EP == DEVICE_CONTRL_CODE_PAYMENT_EP){
				wCode = (wCode ^ DEVICE_CONTRL_CODE_PAYMENT_EP) | DEVICE_CONTRL_CODE_PAYMENT;
			}*/
			// AVM�����ÿ�
			/*if (wCode & DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD == DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD){
				wCode = (wCode ^ DEVICE_CONTRL_CODE_PAYMENT_CREDIT_CARD) | DEVICE_CONTRL_CODE_PAYMENT;
			}*/
		}
		theTVM_STATUS_MGR.SetPaymentMode(wCode);
		theTVM_SETTING.SetPayMentMode(wCode);
		break;
	case DLG_GROUP_CHANGE:								// ����ģʽ����
		/*if(m_SvcData.mapItem[1].Statuse == ON){
		theTVM_STATUS_MGR.SetChangeMode(DEVICE_CONTRL_CODE_ALL_CHANGE);
		theSETTING.SetChangeMode(DEVICE_CONTRL_CODE_ALL_CHANGE);
		}
		else{
		theTVM_STATUS_MGR.SetChangeMode(DEVICE_CONTRL_CODE_NO_CHANGE);
		theSETTING.SetChangeMode(DEVICE_CONTRL_CODE_NO_CHANGE);
		}*/
		theTVM_STATUS_MGR.SetChangeMode(wCode);
		theTVM_SETTING.SetChangeMode(wCode);
		break;
	case  DLG_GROUP_PRINT:								// ��ӡģʽ����
		if(m_SvcData.mapItem[1].Statuse == ON){
			theTVM_STATUS_MGR.SetPrintMode(0);
			theTVM_SETTING.SetPrintMode(0);
		}
		else{
			theTVM_STATUS_MGR.SetPrintMode(DEVICE_CONTRL_CODE_SC_NO_PRINT);
			theTVM_SETTING.SetPrintMode(DEVICE_CONTRL_CODE_SC_NO_PRINT);
		}
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ȡSVC����

@param    
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CModeSettingsSvc::SVC_DATA& CModeSettingsSvc::GetDataModel()
{
	return m_SvcData;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      SVC���ݸ���

@param    
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      F3���ܰ�����Ӧ

@param    
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CModeSettingsSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	UINT dlg = GetCurDialogGroupID();
	switch(dlg)
	{
	case DLG_GROUP_BASE:
		theAPP_SESSION.ReturnMaintainMenu();	// ����ģʽ->������
		break;
	case DLG_GROUP_CHANGE:						// ����->����ģʽ
		DoDialogFlow(CHANGE_TO_BASE);
		break;
	case DLG_GROUP_SERVICE:						// ����ģʽ->����ģʽ
		DoDialogFlow(SVC_TO_BASE);
		break;
	case DLG_GROUP_PAYMENT:						// ֧��->����ģʽ
		DoDialogFlow(PAYMENT_TO_BASE);
		break;
	case DLG_GROUP_PRINT:						// ��ӡģʽ->����ģʽ
		DoDialogFlow(PRINT_TO_BASE);
		break;
	default:
		theAPP_SESSION.ReturnMaintainMenu();
		break;
	}
	LoadItem(m_SvcData.mapItem, (DLG_GROUP)GetCurDialogGroupID());

	theAPP_SESSION.AppendMenuLevel(_T(""));
	NotifyDataModelChanged();
	return 0;
}
