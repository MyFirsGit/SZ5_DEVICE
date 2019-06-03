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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetSvc::CSwitchOffSetSvc()
	:CTVMForeService(SWITCH_OFF_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CSwitchOffSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SWITCHOFF_SET, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SWITCHOFF_SET, IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SWITCHOFF_SET);		// 设置初始画面组

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetSvc::~CSwitchOffSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键

	EnterTime = 1;
	GuideMsg = _T("");

	DeviceType = theMAINTENANCE_INFO.GetDeviceType();					// 获取设备类型

	ReadTvmIni();

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SWITCHOFF_SET);		// 设置初始画面组
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_SWITCHOFF_SET);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

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
@brief      读取TVM.INI文件

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::ReadTvmIni()
{
	m_DataModel.UsedUnUsed = theTVM_INFO.IsRWUsed(TVM_RECHARGE_RW);			// 充值读写器是否启用
	m_DataModel.MenuRechargeRW = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveRechargeRW = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsRWUsed(TVM_CARD_RW);				// Smart-Card读写器是否启用
	m_DataModel.MenuSmartcardRW = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveSmartcardRW = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsRWUsed(TVM_TOKEN_RW);			// Token读写器是否启用
	//m_DataModel.MenuTokenRW = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.saveTokenRW = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsBHUsed();						// 纸币处理模块
	m_DataModel.MenuBanknoteModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveBanknoteModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsBDHUsed();						// 纸币找零处理模块
	m_DataModel.MenuBanknoteChangeModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveBanknoteChangeModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsTHUsed();						// Smart-Card模块
	m_DataModel.MenuSmartcardModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveSmartcardModule = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsTokenUsed();						// Token模块
	//m_DataModel.MenuTokenModule = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.saveTokenModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsCHUsed();						// 硬币处理模块
	m_DataModel.MenuCoinModule = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveCoinModule = SaveDataChange(m_DataModel.UsedUnUsed);

	m_DataModel.UsedUnUsed = theTVM_INFO.IsPrinterUsed();					// 凭证打印机模块
	m_DataModel.MenuReceiptPrint = ReadUSEING(m_DataModel.UsedUnUsed);
	UsSave.saveReceiptPrint = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsPinPadUsed();					// 密码键盘模块
	//m_DataModel.MenuPinPadModule = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.savePinPadModule = SaveDataChange(m_DataModel.UsedUnUsed);

	//m_DataModel.UsedUnUsed = theTVM_INFO.IsBankCardUsed();					// 银行卡模块
	//m_DataModel.MenuBankCardModule = ReadUSEING(m_DataModel.UsedUnUsed);
	//UsSave.saveBankCardModule = SaveDataChange(m_DataModel.UsedUnUsed);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      识别启用 停用

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
		UsingType = _opl(GUIDE_USED_SET);		// 启用
		break;
	case FALSE:
		UsingType = _opl(GUIDE_UNUSED_SET);		// 停用
		break;
	default:
		break;
	}
	return UsingType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      识别启用 停用

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
		UsingType = _T("1");		// 启用
		break;
	case 2:
		UsingType = _T("0");		// 停用
		break;
	default:
		break;
	}
	return UsingType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardEnter(0,0);
	if(1 == EnterTime)				// 第一次按下Enter键确认菜单的选择
	{
		CheckMenuNumber();
	}
	//else if(2 == EnterTime)			// 第二次按下Enter键确认代码设置的选择
	//{
	//	DoCheckInput();				// 对输入数据是否合理做出相应项的实际判断
	//	theAPP_SESSION.SetMaxInputLength(2);
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     通知数据模型更新

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      菜单号是否非法判断

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::CheckMenuNumber()
{
	// 验证输入的菜单编号
	CString MenuNumber = _T("");
	MenuNumber = theAPP_SESSION.GetInputToString();
	m_DataModel.MenuNumber = _ttoi(MenuNumber.Left(MenuNumber.GetLength()-1));

	if(CSwitchOffSetMaintainDlg::TVM_TYPE == DeviceType)
	{
		if (m_DataModel.MenuNumber < 0 || m_DataModel.MenuNumber > MAX_SWITCHOFF_NUM)		// 主菜单编号除1-10之外的为非法菜单号
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
			return;
		}
		else
		{
			//InputGuideMsg();																// 对GuideMsg分别赋值
			//theAPP_SESSION.DeleteAllInput();
			//theAPP_SESSION.SetPrefixToGuide(GuideMsg);
			//EnterTime = 2;

			DoCheckInput();				// 对输入数据是否合理做出相应项的实际判断
		}
	}
	//else if(CSwitchOffSetMaintainDlg::AVM_TYPE == DeviceType)
	//{
	//	if (m_DataModel.MenuNumber < 1 || m_DataModel.MenuNumber > 5)		// 主菜单编号除1-5之外的为非法菜单号
	//	{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
	//		return;
	//	}
	//	else
	//	{
	//		DoCheckInput();				// 对输入数据是否合理做出相应项的实际判断
	//	}
	//}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对GuideMsg分别赋值

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::InputGuideMsg()
{
	theAPP_SESSION.SetMaxInputLength(1);
	switch(m_DataModel.MenuNumber)							// 依据菜单号选择相应的提示
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
@brief     对输入数据是否合理做出相应项的实际判断

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetSvc::DoCheckInput()
{
	BOOL UsFlag = FALSE;

	CString usedstr = theAPP_SESSION.GetInputToString();	// 获取向导栏输入内容
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
		case MENU_CODE_0:		// 充值读写器
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
		case MENU_CODE_1:		// Smart-Card读写器
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
			//case MENU_CODE_3:		// Token读写器
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
		case MENU_CODE_2:		// 纸币处理模块
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
		/*case MENU_CODE_3:		// 纸币找零处理模块
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
		case MENU_CODE_3:		// Smart-Card模块
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
			//case MENU_CODE_6:		// Token模块
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
		case MENU_CODE_4:		// 硬币处理模块
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
		case MENU_CODE_5:		// 凭证打印机
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
			//case MENU_CODE_9:		// 密码键盘模块
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
			//case MENU_CODE_10:	// 银行卡模块
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
	//	case MENU_CODE_1:		// 充值读写器
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
	//	case MENU_CODE_2:		// 纸币处理模块
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
	//	case MENU_CODE_3:		// 凭证打印机
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
	//	//case MENU_CODE_4:		// 密码键盘模块
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
	//	//case MENU_CODE_5:	// 银行卡模块
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

	m_DataModel.CodeChange = TRUE;		// 代码发生变化标志
	NotifyDataModelChanged();
	EnterTime = 1;						// 置按键次数为1，重新从选择菜单编号开始
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对输入数据做相应的转换

@param      int index

@retval     none

@exception  无
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
@brief      处理 [F2/Alt] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF2(0,0);
	if(CSwitchOffSetMaintainDlg::TVM_TYPE == DeviceType)
	{
		theTVM_INFO.SetRechargeIsRWUsed(UsSave.saveRechargeRW);		// 充值读写器
		theTVM_INFO.SetSmartCardIsRWUsed(UsSave.saveSmartcardRW);		// Smart-card读写器
		//theTVM_INFO.SetTokenIsRWUsed(UsSave.saveTokenRW);				// Token读写器
		theTVM_INFO.SetBankNoteIsUsed(UsSave.saveBanknoteModule);		// 纸币模块
		//theTVM_INFO.SetBankNoteChangeIsUsed(UsSave.saveBanknoteChangeModule); // 纸币找零模块
		theTVM_INFO.SetSmartCardIsUsed(UsSave.saveSmartcardModule);		// Smart-card模块
		//theTVM_INFO.SetTokenIsUsed(UsSave.saveTokenModule);				// Token模块
		theTVM_INFO.SetCoinIsUsed(UsSave.saveCoinModule);				// 硬币模块
		theTVM_INFO.SetReceiptIsUsed(UsSave.saveReceiptPrint);			// 凭证打印机
		//theTVM_INFO.SetPinPadIsUsed(UsSave.savePinPadModule);			// 密码键盘模块
		//theTVM_INFO.SetBankCardIsUsed(UsSave.saveBankCardModule);		// 银行卡模块
		theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));
	}
	//else if(CSwitchOffSetMaintainDlg::AVM_TYPE == DeviceType)
	//{
	//	theTVM_INFO.SetRechargeIsRWUsed(UsSave.saveRechargeRW);			// 充值读写器
	//	theTVM_INFO.SetBankNoteIsUsed(UsSave.saveBanknoteModule);		// 纸币模块
	//	theTVM_INFO.SetReceiptIsUsed(UsSave.saveReceiptPrint);			// 凭证打印机
	//	theTVM_INFO.SetPinPadIsUsed(UsSave.savePinPadModule);			// 密码键盘模块
	//	theTVM_INFO.SetBankCardIsUsed(UsSave.saveBankCardModule);		// 银行卡模块
	//	theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Clear] 键按下消息（删除）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	//if(true == theAPP_SESSION.IsEmptyPrefix())
	//{
	//	EnterTime = 1;		// 置按键次数为1，重新从选择菜单编号开始
	//	theAPP_SESSION.SetMaxInputLength(3);
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSwitchOffSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}