#include "stdafx.h"
#include "SwitchOffSetMaintainDlg.h"
#include "SwitchOffSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSwitchOffSetMaintainDlg,COperationDlg)
	
BEGIN_MESSAGE_MAP(CSwitchOffSetMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetMaintainDlg::CSwitchOffSetMaintainDlg(CService* pService)
:COperationDlg(CSwitchOffSetMaintainDlg::IDD,pService)
{
	DeviceType = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetMaintainDlg::~CSwitchOffSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int 0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CSwitchOffSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(3);							// �����������λ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,250);					// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,250);					// ���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,140);					// ���õ����п��

	//InitlableSwitchOffInfo();										// ��ʼ�����������label��Ϣ
	//SetFromReadIniText();											// ��ȡTVM.INI�����ļ�����ʾ�ڶ�Ӧ����
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlableSwitchOffInfo();										// ��ʼ�����������label��Ϣ
	SetFromReadIniText();											// ��ȡTVM.INI�����ļ�����ʾ�ڶ�Ӧ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::InitlableSwitchOffInfo()
{
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();											// ��ȡ�豸����
	if(TVM_TYPE == DeviceType)
	{
		// ��ʼ����Ϣ��������
		int index = 0;
		m_lableGroup[index].nameLeft = _T("[0]")+_opl(GUIDE_RECHARGE_SET)+_opl(TXT_COLON);			// ��ֵ��д��
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[1] ")+_opl(GUIDE_SMARTCARD_RW_SET)+_opl(TXT_COLON);		// Smart-Card��д��
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[2].nameLeft = _T("[3]")+_opl(GUIDE_TOKEN_RW_SET)+_opl(TXT_COLON);			// Token��д��
		//m_lableGroup[2].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		m_lableGroup[index].nameLeft = _T("[2] ")+_opl(GUIDE_BANKNOTE_SET)+_opl(TXT_COLON);			// ֽ�Ҵ���ģ��
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		//m_lableGroup[index].nameLeft = _T("[3] ")+_opl(GUIDE_BANKNOTE_CHANGE_SET)+_opl(TXT_COLON);	// ֽ������ģ��
		//m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[3] ")+_opl(GUIDE_SMARTCARD_MODULE_SET)+_opl(TXT_COLON);	// Smart-Cardģ��
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[5].nameLeft = _T("[6]")+_opl(GUIDE_TOKEN_MODULE_SET)+_opl(TXT_COLON);		// Tokenģ��
		//m_lableGroup[5].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[4] ")+_opl(GUIDE_COIN_MODULE_SET)+_opl(TXT_COLON);		// Ӳ�Ҵ���ģ��
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[5] ")+_opl(GUIDE_RECEIPT_SET)+_opl(TXT_COLON);			// ƾ֤��ӡ��
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[8].nameLeft = _T("[9]")+_opl(GUIDE_PINPAD_SET)+_opl(TXT_COLON);			// �������ģ��
		//m_lableGroup[8].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[9].nameLeft = _T("[10]")+_opl(GUIDE_BANKCARD_SET)+_opl(TXT_COLON);			// ���п�ģ��
		//m_lableGroup[9].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	}
	//else if(AVM_TYPE == DeviceType)
	//{
	//	// ��ʼ����Ϣ��������
	//	m_lableGroup[0].nameLeft = _T("[1]")+_opl(GUIDE_RECHARGE_SET)+_opl(TXT_COLON);			// ��ֵ��д��
	//	m_lableGroup[0].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[1].nameLeft = _T("[2]")+_opl(GUIDE_BANKNOTE_SET)+_opl(TXT_COLON);			// ֽ�Ҵ���ģ��
	//	m_lableGroup[1].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[2].nameLeft = _T("[3]")+_opl(GUIDE_RECEIPT_SET)+_opl(TXT_COLON);			// ƾ֤��ӡ��
	//	m_lableGroup[2].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[3].nameLeft = _T("[4]")+_opl(GUIDE_PINPAD_SET)+_opl(TXT_COLON);			// �������ģ��
	//	m_lableGroup[3].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[4].nameLeft = _T("[5]")+_opl(GUIDE_BANKCARD_SET)+_opl(TXT_COLON);			// ���п�ģ��
	//	m_lableGroup[4].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	/*for(int i = 5; i < 10; i++)
	//	{
	//		m_lableGroup[i] = _T("");
	//	}*/
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��TVM.INI��ȡ�������ļ��ڶ�Ӧ����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::SetFromReadIniText()
{
	CSwitchOffSetSvc* pSvc = (CSwitchOffSetSvc*)GetService();
	CSwitchOffSetSvc::Model& model = pSvc->GetDataModel();
	if(TVM_TYPE == DeviceType)
	{
		int index = 0;
		m_lableGroup[index++].nameRight = model.MenuRechargeRW;										// ��ֵ��д��
		m_lableGroup[index++].nameRight = model.MenuSmartcardRW;								// Smart-card��д��
		//m_lableGroup[2].nameRight = model.MenuTokenRW;										// Token��д��
		m_lableGroup[index++].nameRight = model.MenuBanknoteModule;								// ֽ��ģ��
		//m_lableGroup[index++].nameRight = model.MenuBanknoteChangeModule;						// ֽ������ģ��
		m_lableGroup[index++].nameRight = model.MenuSmartcardModule;							// Smart-cardģ��
		//m_lableGroup[5].nameRight = model.MenuTokenModule;									// Tokenģ��
		m_lableGroup[index++].nameRight = model.MenuCoinModule;									// Ӳ��ģ��
		m_lableGroup[index++].nameRight = model.MenuReceiptPrint;								// ƾ֤��ӡ��
		//m_lableGroup[8].nameRight = model.MenuPinPadModule;									// �������
		//m_lableGroup[9].nameRight = model.MenuBankCardModule;									// ���п�ģ��
	}
	//else if(AVM_TYPE == DeviceType)
	//{
	//	m_lableGroup[0].nameRight = model.MenuRechargeRW;										// ��ֵ��д��
	//	m_lableGroup[1].nameRight = model.MenuBanknoteModule;									// ֽ��ģ��
	//	m_lableGroup[2].nameRight = model.MenuReceiptPrint;										// ƾ֤��ӡ��
	//	m_lableGroup[3].nameRight = model.MenuPinPadModule;										// �������
	//	m_lableGroup[4].nameRight = model.MenuBankCardModule;									// ���п�ģ��
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::UpdateUI()
{
	CSwitchOffSetSvc* pSvc = (CSwitchOffSetSvc*)GetService();
	CSwitchOffSetSvc::Model& model = pSvc->GetDataModel();

	if(TRUE == model.CodeChange)		// �������
	{
		ShowCodeContent();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����ʾ��Ӧ�����õ�����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CSwitchOffSetMaintainDlg::ShowCodeContent()
{
	CSwitchOffSetSvc* pSvc = (CSwitchOffSetSvc*)GetService();
	CSwitchOffSetSvc::Model& model = pSvc->GetDataModel();	
	if(FALSE == model.CodeLegal )		// �������ݷǷ�
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	if(TVM_TYPE == DeviceType)
	{
		switch (model.MenuNumber)
		{
		case MENU_CODE_0:					// ��ֵ��д��
			m_lableGroup[0].nameRight = model.MenuRechargeRW;
			break;
		case MENU_CODE_1:					// Smart-Card��д��
			m_lableGroup[1].nameRight = model.MenuSmartcardRW;
			break;
		//case MENU_CODE_3:					// Token��д��
		//	m_lableGroup[2].nameRight = model.MenuTokenRW;
		//	break;
		case MENU_CODE_2:					// ֽ�Ҵ���ģ��
			m_lableGroup[2].nameRight = model.MenuBanknoteModule;
			break;
		//case MENU_CODE_3:					// ֽ������ģ��
		//	m_lableGroup[3].nameRight = model.MenuBanknoteChangeModule;
		//	break;
		case MENU_CODE_3:					// Smart-Cardģ��
			m_lableGroup[3].nameRight = model.MenuSmartcardModule;
			break;
		//case MENU_CODE_6:					// Tokenģ��
		//	m_lableGroup[5].nameRight = model.MenuTokenModule;
		//	break;
		case MENU_CODE_4:					// Ӳ�Ҵ���ģ��
			m_lableGroup[4].nameRight = model.MenuCoinModule;
			break;
		case MENU_CODE_5:					// ƾ֤��ӡ��
			m_lableGroup[5].nameRight = model.MenuReceiptPrint;
			break;
		//case MENU_CODE_9:					// �������ģ��
		//	m_lableGroup[8].nameRight = model.MenuPinPadModule;
		//	break;
		//case MENU_CODE_10:					// ���п�ģ��
		//	m_lableGroup[9].nameRight = model.MenuBankCardModule;
		//	break;
		default:
			break;
		}
	}
	//else if(AVM_TYPE == DeviceType)
	//{
	//	switch (model.MenuNumber)
	//	{
	//	case MENU_CODE_1:					// ��ֵ��д��
	//		m_lableGroup[0].nameRight = model.MenuRechargeRW;
	//		break;
	//	case MENU_CODE_2:					// ֽ�Ҵ���ģ��
	//		m_lableGroup[1].nameRight = model.MenuBanknoteModule;
	//		break;
	//	case MENU_CODE_3:					// ƾ֤��ӡ��
	//		m_lableGroup[2].nameRight = model.MenuReceiptPrint;
	//		break;
	//	case MENU_CODE_4:					// �������ģ��
	//		m_lableGroup[3].nameRight = model.MenuPinPadModule;
	//		break;
	//	case MENU_CODE_5:					// ���п�ģ��
	//		m_lableGroup[4].nameRight = model.MenuBankCardModule;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}