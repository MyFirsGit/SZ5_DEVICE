#include "stdafx.h"
#include "SwitchOffSetSvc.h"
#include "SwitchOffSetMaintainDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSwitchOffSetSvc,CTVMForeService)

END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetSvc::CSwitchOffSetSvc()
	:CTVMForeService(SWITCH_OFF_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CSwitchOffSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SWITCHOFF_SET, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SWITCHOFF_SET, IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SWITCHOFF_SET);		// ���ó�ʼ������

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetSvc::~CSwitchOffSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������

	EnterTime = 1;
	GuideMsg = _T("");

	DeviceType = theMAINTENANCE_INFO.GetDeviceType();					// ��ȡ�豸����

	ReadTvmIni();

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SWITCHOFF_SET);		// ���ó�ʼ������
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_SWITCHOFF_SET);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     currency

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetSvc::Model& CSwitchOffSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡTVM.INI�ļ�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::ReadTvmIni()
{
	m_DataModel.UsedUnUsed = theTVM_INFO.IsRWUsed(TVM_RECHARGE_RW);			// ��ֵ��д���Ƿ�����
	m_DataModel.MenuRechargeRW = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveRechargeRW = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsRWUsed(TVM_CARD_RW);				// Smart-Card��д���Ƿ�����
	m_DataModel.MenuSmartcardRW = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveSmartcardRW = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsRWUsed(TVM_TOKEN_RW);			// Token��д���Ƿ�����
	//m_DataModel.MenuTokenRW = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.saveTokenRW = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsBHUsed();						// ֽ�Ҵ���ģ��
	m_DataModel.MenuBanknoteModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveBanknoteModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsBDHUsed();						// ֽ�����㴦��ģ��
	m_DataModel.MenuBanknoteChangeModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveBanknoteChangeModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsTHUsed();						// Smart-Cardģ��
	m_DataModel.MenuSmartcardModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveSmartcardModule = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsTokenUsed();						// Tokenģ��
	//m_DataModel.MenuTokenModule = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.saveTokenModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsCHUsed();						// Ӳ�Ҵ���ģ��
	m_DataModel.MenuCoinModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveCoinModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsPrinterUsed();					// ƾ֤��ӡ��ģ��
	m_DataModel.MenuReceiptPrint = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveReceiptPrint = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsPinPadUsed();					// �������ģ��
	//m_DataModel.MenuPinPadModule = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.savePinPadModule = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsBankCardUsed();					// ���п�ģ��
	//m_DataModel.MenuBankCardModule = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.saveBankCardModule = SaveDataChange(m_DataModel.UsedUnUsed);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʶ������ ͣ��

@param      UsedUnUsed

@retval     CString

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CSwitchOffSetSvc::ReadUSEING(BOOL UsedUnUsed)
{
	CString UsingType = _T("");
	switch(UsedUnUsed)
	{
	case TRUE:
		UsingType = _opl(GUIDE_USED_SET);		// ����
		break;
	case FALSE:
		UsingType = _opl(GUIDE_UNUSED_SET);		// ͣ��
		break;
	default:
		break;
	}
	return UsingType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʶ������ ͣ��

@param      UsedUnUsed

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CSwitchOffSetSvc::ChangeToStr(int UsedUnUsed)
{
	CString UsingType = _T("");
	switch(UsedUnUsed)
	{
	case 1:
		UsingType = _T("1");		// ����
		break;
	case 2:
		UsingType = _T("0");		// ͣ��
		break;
	default:
		break;
	}
	return UsingType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardEnter(0,0);
	if(1 == EnterTime)				// ��һ�ΰ���Enter��ȷ�ϲ˵���ѡ��
	{
		CheckMenuNumber();
	}
	//else if(2 == EnterTime)			// �ڶ��ΰ���Enter��ȷ�ϴ������õ�ѡ��
	//{
	//	DoCheckInput();				// �����������Ƿ����������Ӧ���ʵ���ж�
	//	theAPP_SESSION.SetMaxInputLength(2);
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˵����Ƿ�Ƿ��ж�

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::CheckMenuNumber()
{
	// ��֤����Ĳ˵����
	CString MenuNumber = _T("");
	MenuNumber = theAPP_SESSION.GetInputToString();
	m_DataModel.MenuNumber = _ttoi(MenuNumber.Left(MenuNumber.GetLength()-1));

	if(CSwitchOffSetMaintainDlg::TVM_TYPE == DeviceType)
	{
		if (m_DataModel.MenuNumber < 0 || m_DataModel.MenuNumber > MAX_SWITCHOFF_NUM)		// ���˵���ų�1-10֮���Ϊ�Ƿ��˵���
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
			return;
		}
		else
		{
			//InputGuideMsg();																// ��GuideMsg�ֱ�ֵ
			//theAPP_SESSION.DeleteAllInput();
			//theAPP_SESSION.SetPrefixToGuide(GuideMsg);
			//EnterTime = 2;

			DoCheckInput();				// �����������Ƿ����������Ӧ���ʵ���ж�
		}
	}
	//else if(CSwitchOffSetMaintainDlg::AVM_TYPE == DeviceType)
	//{
	//	if (m_DataModel.MenuNumber < 1 || m_DataModel.MenuNumber > 5)		// ���˵���ų�1-5֮���Ϊ�Ƿ��˵���
	//	{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
	//		return;
	//	}
	//	else
	//	{
	//		DoCheckInput();				// �����������Ƿ����������Ӧ���ʵ���ж�
	//	}
	//}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��GuideMsg�ֱ�ֵ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::InputGuideMsg()
{
	theAPP_SESSION.SetMaxInputLength(1);
	switch(m_DataModel.MenuNumber)							// ���ݲ˵���ѡ����Ӧ����ʾ
	{
	case MENU_CODE_1:
		GuideMsg = _opl(GUIDE_RECHARGE_SET)+_opl(TXT_COLON);
		break;
	case MENU_CODE_2:
		GuideMsg = _opl(GUIDE_SMARTCARD_RW_SET)+_opl(TXT_COLON);
		break;
		/*case MENU_CODE_3:
		GuideMsg = _opl(GUIDE_TOKEN_RW_SET)+_opl(TXT_COLON);
		break;*/
	case MENU_CODE_3:
		GuideMsg = _opl(GUIDE_BANKNOTE_SET)+_opl(TXT_COLON);
		break;
		/*case MENU_CODE_4:
		GuideMsg = _opl(GUIDE_BANKNOTE_CHANGE_SET)+_opl(TXT_COLON);
		break;*/
	case MENU_CODE_4:
		GuideMsg = _opl(GUIDE_SMARTCARD_MODULE_SET)+_opl(TXT_COLON);
		break;
		/*case MENU_CODE_6:
		GuideMsg = _opl(GUIDE_TOKEN_MODULE_SET)+_opl(TXT_COLON);
		break;*/
	case MENU_CODE_5:
		GuideMsg = _opl(GUIDE_COIN_MODULE_SET)+_opl(TXT_COLON);
		break;
	case MENU_CODE_6:
		GuideMsg = _opl(GUIDE_RECEIPT_SET)+_opl(TXT_COLON);
		break;
		/*case MENU_CODE_9:
		GuideMsg = _opl(GUIDE_PINPAD_SET)+_opl(TXT_COLON);
		break;
		case MENU_CODE_10:
		GuideMsg = _opl(GUIDE_BANKCARD_SET)+_opl(TXT_COLON);
		break;*/
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �����������Ƿ����������Ӧ���ʵ���ж�

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::DoCheckInput()
{
	BOOL UsFlag = FALSE;

	CString usedstr = theAPP_SESSION.GetInputToString();	// ��ȡ������������
	int index = _ttoi(usedstr.Right(1));	

	if(_T("") == usedstr)
	{
		return;
	}

	if(1 == _ttoi(usedstr.Right(1)))
	{
		UsFlag = TRUE;
	}
	else if(2 == _ttoi(usedstr.Right(1)))
	{
		UsFlag = FALSE;
	}

	if(CSwitchOffSetMaintainDlg::TVM_TYPE == DeviceType)
	{
		switch(m_DataModel.MenuNumber)
		{
		case MENU_CODE_0:		// ��ֵ��д��
			if(index<1 || index>2)
			{
				m_DataModel.CodeLegal = FALSE;
			}
			else
			{
				m_DataModel.MenuRechargeRW = ReadUSEING(UsFlag);
				UsSave.saveRechargeRW = ChangeToStr(index);
				m_DataModel.CodeLegal = TRUE;
			}
			break;
		case MENU_CODE_1:		// Smart-Card��д��
			if(index<1 || index>2)
			{
				m_DataModel.CodeLegal = FALSE;
			}
			else
			{
				m_DataModel.MenuSmartcardRW = ReadUSEING(UsFlag);
				UsSave.saveSmartcardRW = ChangeToStr(index);
				m_DataModel.CodeLegal = TRUE;
			}
			break;
			//case MENU_CODE_3:		// Token��д��
			//	if(index<1 || index>2)
			//	{
			//		m_DataModel.CodeLegal = FALSE;
			//	}
			//	else
			//	{
			//		m_DataModel.MenuTokenRW = ReadUSEING(UsFlag);
			//		UsSave.saveTokenRW = ChangeToStr(index);
			//		m_DataModel.CodeLegal = TRUE;
			//	}
			//	break;
		case MENU_CODE_2:		// ֽ�Ҵ���ģ��
			if(index<1 || index>2)
			{
				m_DataModel.CodeLegal = FALSE;
			}
			else
			{
				m_DataModel.MenuBanknoteModule = ReadUSEING(UsFlag);
				UsSave.saveBanknoteModule = ChangeToStr(index);
				m_DataModel.CodeLegal = TRUE;
			}
			break;
		/*case MENU_CODE_3:		// ֽ�����㴦��ģ��
			if(index<1 || index>2)
			{
			m_DataModel.CodeLegal = FALSE;
			}
			else
			{
			m_DataModel.MenuBanknoteChangeModule = ReadUSEING(UsFlag);
			UsSave.saveBanknoteChangeModule = ChangeToStr(index);
			m_DataModel.CodeLegal = TRUE;
			}
			break;*/
		case MENU_CODE_3:		// Smart-Cardģ��
			if(index<1 || index>2)
			{
				m_DataModel.CodeLegal = FALSE;
			}
			else
			{
				m_DataModel.MenuSmartcardModule = ReadUSEING(UsFlag);
				UsSave.saveSmartcardModule = ChangeToStr(index);
				m_DataModel.CodeLegal = TRUE;
			}
			break;
			//case MENU_CODE_6:		// Tokenģ��
			//	if(index<1 || index>2)
			//	{
			//		m_DataModel.CodeLegal = FALSE;
			//	}
			//	else
			//	{
			//		m_DataModel.MenuTokenModule = ReadUSEING(UsFlag);
			//		UsSave.saveTokenModule = ChangeToStr(index);
			//		m_DataModel.CodeLegal = TRUE;
			//	}
			//	break;
		case MENU_CODE_4:		// Ӳ�Ҵ���ģ��
			if(index<1 || index>2)
			{
				m_DataModel.CodeLegal = FALSE;
			}
			else
			{
				m_DataModel.MenuCoinModule = ReadUSEING(UsFlag);
				UsSave.saveCoinModule = ChangeToStr(index);
				m_DataModel.CodeLegal = TRUE;
			}
			break;
		case MENU_CODE_5:		// ƾ֤��ӡ��
			if(index<1 || index>2)
			{
				m_DataModel.CodeLegal = FALSE;
			}
			else
			{
				m_DataModel.MenuReceiptPrint = ReadUSEING(UsFlag);
				UsSave.saveReceiptPrint = ChangeToStr(index);
				m_DataModel.CodeLegal = TRUE;
			}
			break;
			//case MENU_CODE_9:		// �������ģ��
			//	if(index<1 || index>2)
			//	{
			//		m_DataModel.CodeLegal = FALSE;
			//	}
			//	else
			//	{
			//		m_DataModel.MenuPinPadModule = ReadUSEING(UsFlag);
			//		UsSave.savePinPadModule = ChangeToStr(index);
			//		m_DataModel.CodeLegal = TRUE;
			//	}
			//	break;
			//case MENU_CODE_10:	// ���п�ģ��
			//	if(index<1 || index>2)
			//	{
			//		m_DataModel.CodeLegal = FALSE;
			//	}
			//	else
			//	{
			//		m_DataModel.MenuBankCardModule = ReadUSEING(UsFlag);
			//		UsSave.saveBankCardModule = ChangeToStr(index);
			//		m_DataModel.CodeLegal = TRUE;
			//	}
			//	break;
		default:
			break;
		}
	}
	//else if(CSwitchOffSetMaintainDlg::AVM_TYPE == DeviceType)
	//{
	//	switch(m_DataModel.MenuNumber)
	//	{
	//	case MENU_CODE_1:		// ��ֵ��д��
	//		if(index<1 || index>2)
	//		{
	//			m_DataModel.CodeLegal = FALSE;
	//		}
	//		else
	//		{
	//			m_DataModel.MenuRechargeRW = ReadUSEING(UsFlag);
	//			UsSave.saveRechargeRW = ChangeToStr(index);
	//			m_DataModel.CodeLegal = TRUE;
	//		}
	//		break;
	//	case MENU_CODE_2:		// ֽ�Ҵ���ģ��
	//		if(index<1 || index>2)
	//		{
	//			m_DataModel.CodeLegal = FALSE;
	//		}
	//		else
	//		{
	//			m_DataModel.MenuBanknoteModule = ReadUSEING(UsFlag);
	//			UsSave.saveBanknoteModule = ChangeToStr(index);
	//			m_DataModel.CodeLegal = TRUE;
	//		}
	//		break;
	//	case MENU_CODE_3:		// ƾ֤��ӡ��
	//		if(index<1 || index>2)
	//		{
	//			m_DataModel.CodeLegal = FALSE;
	//		}
	//		else
	//		{
	//			m_DataModel.MenuReceiptPrint = ReadUSEING(UsFlag);
	//			UsSave.saveReceiptPrint = ChangeToStr(index);
	//			m_DataModel.CodeLegal = TRUE;
	//		}
	//		break;
	//	//case MENU_CODE_4:		// �������ģ��
	//	//	if(index<1 || index>2)
	//	//	{
	//	//		m_DataModel.CodeLegal = FALSE;
	//	//	}
	//	//	else
	//	//	{
	//	//		m_DataModel.MenuPinPadModule = ReadUSEING(UsFlag);
	//	//		UsSave.savePinPadModule = ChangeToStr(index);
	//	//		m_DataModel.CodeLegal = TRUE;
	//	//	}
	//	//	break;
	//	//case MENU_CODE_5:	// ���п�ģ��
	//	//	if(index<1 || index>2)
	//	//	{
	//	//		m_DataModel.CodeLegal = FALSE;
	//	//	}
	//	//	else
	//	//	{
	//	//		m_DataModel.MenuBankCardModule = ReadUSEING(UsFlag);
	//	//		UsSave.saveBankCardModule = ChangeToStr(index);
	//	//		m_DataModel.CodeLegal = TRUE;
	//	//	}
	//	//	break;
	//	default:
	//		break;
	//	}
	//}

	m_DataModel.CodeChange = TRUE;		// ���뷢���仯��־
	NotifyDataModelChanged();
	EnterTime = 1;						// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��������������Ӧ��ת��

@param      int index

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSwitchOffSetSvc::SaveDataChange(BOOL SaveData)
{
	CString SaveIsUsed = _T("");
	if(TRUE == SaveData)
	{
		SaveIsUsed = _T("1");
	}
	else
	{
		SaveIsUsed = _T("0");
	}
	return SaveIsUsed;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF2(0,0);
	if(CSwitchOffSetMaintainDlg::TVM_TYPE == DeviceType)
	{
		theTVM_INFO.SetRechargeIsRWUsed(UsSave.saveRechargeRW);		// ��ֵ��д��
		theTVM_INFO.SetSmartCardIsRWUsed(UsSave.saveSmartcardRW);		// Smart-card��д��
		//theTVM_INFO.SetTokenIsRWUsed(UsSave.saveTokenRW);				// Token��д��
		theTVM_INFO.SetBankNoteIsUsed(UsSave.saveBanknoteModule);		// ֽ��ģ��
		//theTVM_INFO.SetBankNoteChangeIsUsed(UsSave.saveBanknoteChangeModule); // ֽ������ģ��
		theTVM_INFO.SetSmartCardIsUsed(UsSave.saveSmartcardModule);		// Smart-cardģ��
		//theTVM_INFO.SetTokenIsUsed(UsSave.saveTokenModule);				// Tokenģ��
		theTVM_INFO.SetCoinIsUsed(UsSave.saveCoinModule);				// Ӳ��ģ��
		theTVM_INFO.SetReceiptIsUsed(UsSave.saveReceiptPrint);			// ƾ֤��ӡ��
		//theTVM_INFO.SetPinPadIsUsed(UsSave.savePinPadModule);			// �������ģ��
		//theTVM_INFO.SetBankCardIsUsed(UsSave.saveBankCardModule);		// ���п�ģ��
		theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));
	}
	//else if(CSwitchOffSetMaintainDlg::AVM_TYPE == DeviceType)
	//{
	//	theTVM_INFO.SetRechargeIsRWUsed(UsSave.saveRechargeRW);			// ��ֵ��д��
	//	theTVM_INFO.SetBankNoteIsUsed(UsSave.saveBanknoteModule);		// ֽ��ģ��
	//	theTVM_INFO.SetReceiptIsUsed(UsSave.saveReceiptPrint);			// ƾ֤��ӡ��
	//	theTVM_INFO.SetPinPadIsUsed(UsSave.savePinPadModule);			// �������ģ��
	//	theTVM_INFO.SetBankCardIsUsed(UsSave.saveBankCardModule);		// ���п�ģ��
	//	theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Clear] ��������Ϣ��ɾ����

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	//if(true == theAPP_SESSION.IsEmptyPrefix())
	//{
	//	EnterTime = 1;		// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
	//	theAPP_SESSION.SetMaxInputLength(3);
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}