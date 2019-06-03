#include "stdafx.h"
#include "ComportAndBaudrateSetSvc.h"
#include "ComportAndBaudrateSetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CComportAndBaudrateSetSvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetSvc::CComportAndBaudrateSetSvc() : CTVMForeService(DEVICE_COMPORT_SET_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(CComportAndBaudrateSetDlg::IDD,new CComportAndBaudrateSetDlg(this));

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COMPORT_BAUD_SET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COMPORT_BAUD_SET, CComportAndBaudrateSetDlg::IDD);
	
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMPORT_BAUD_SET);	// 设置初始画面组
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetSvc::~CComportAndBaudrateSetSvc()
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
void CComportAndBaudrateSetSvc::OnStart()
{
	__super::OnStart();
	
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMPORT_BAUD_SET);	// 设置初始画面组
	
	m_data_model = _tag_data_model_();
	
	ReadFromSettingIni();
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
LRESULT CComportAndBaudrateSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/){

	// 当前未选中任何菜单
	if(m_data_model.iCurSelectMenu == 0){

		// 判断菜单合法性
		int nIndex = theAPP_SESSION.GetInputToInt();// 21

		int nMainMenu	= nIndex / 10;				// 2
		int nChildMenu	= nIndex % 10;				// 1


		// 子菜单只有两个
		if(nChildMenu != MENU_CODE_1 && nChildMenu != MENU_CODE_2){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return FALSE;
		}

		m_data_model.iCurSelectMenu = nMainMenu;
		m_data_model.iCurSelSonMenu = nChildMenu;

		CString strMainModelName(_T(""));

		
		// 验证主菜单和提示
		switch(nMainMenu){
		case MENU_KEY_RW:// 读写器
		//case MENU_KEY_BHCHANGE://充值读写器
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_RW_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		//case MENU_KEY_BH:// 纸币接收
		//	{
		//		strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_BH_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
		//	}
		//	break;
		case MENU_KEY_TH:// 票卡
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_TH_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		case MENU_KEY_CH:// 硬币模块
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_CH_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		//case MENU_KEY_PRINT:// 打印机
		//	{
		//		strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_PRINTER_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
		//	}
		//	break;
		case MENU_KEY_DISPLAY:// 状态显示屏
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_DISPLAY_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		case MENU_KEY_IO:// IO
			{
				strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_IO_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
			}
			break;
		//case MENU_KEY_BHCHANGE:// 纸币找零
		//	{
		//		strMainModelName.Format(_T("%s%s"),_opl(TXT_DEVICE_PLEASE_INPUT_BHCHANGE_INFO),nChildMenu == MENU_CODE_1 ? _opl(TXT_DEVICE_COMPORT_NAME):_opl(TXT_DEVICE_BAUDRATE_NAME));
		//	}
		//	break;
		default:
			m_data_model.iCurSelectMenu = 0;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			break;
		}

		nChildMenu == MENU_CODE_1 ? theAPP_SESSION.SetMaxInputLength(2) : theAPP_SESSION.SetMaxInputLength(6);//115200

		theAPP_SESSION.SetPrefixToGuide(strMainModelName);
	}
	else{
		CString strInputDetail = theAPP_SESSION.GetInputToString();

		if(strInputDetail.IsEmpty()){
			m_data_model.iCurSelectMenu = m_data_model.iCurSelSonMenu = 0;
			theAPP_SESSION.ShowOfficeGuide(TXT_DEVICE_INPUT_ILLEGAL);
			return FALSE;
		}

		Menu_Detail& menuDetail = m_data_model.stMenuDetail[m_data_model.iCurSelectMenu -1];

		// 串口号设置
		if(m_data_model.iCurSelSonMenu == MENU_CODE_1){
			menuDetail.strComport = strInputDetail;
			menuDetail.emComportStatus = MENU_STATUS_SELECT;
		}
		// 波特率设置
		else{
			menuDetail.strBaudRate = strInputDetail;
			menuDetail.emBaudRateStatus = MENU_STATUS_SELECT;
		}

		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F2_ESC);

		m_data_model.bAlreadySaved = false;

		m_data_model.iCurSelectMenu = m_data_model.iCurSelSonMenu = 0;

		theAPP_SESSION.SetMaxInputLength(2);

		NotifyDataModelChanged();
	}
	
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
void CComportAndBaudrateSetSvc::NotifyDataModelChanged(){
	__super::NotifyDataModelChanged();
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
LRESULT CComportAndBaudrateSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/){

	if(!m_data_model.bAlreadySaved){

		for(int i = 0;i< 7;i++){
			Menu_Detail& thisMenu = m_data_model.stMenuDetail[i];
			if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT || thisMenu.emComportStatus == MENU_STATUS_SELECT){
				int nMenu = i + 1;
				switch(nMenu){
				case MENU_KEY_RW:// 读写器
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetRwComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetRwBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				//case MENU_KEY_BH:// 纸币接收
				//	{
				//		if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theSETTING.SetBHComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
				//		if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theSETTING.SetBHBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
				//	}
				//	break;
				case MENU_KEY_TH:// 票卡
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetTokenComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetTokenBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;

					{
				case MENU_KEY_CH:
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetCHComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetCHBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				//case MENU_KEY_PRINT:// 打印机
				//	{
				//		if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theSETTING.SetPrinterComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
				//		if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theSETTING.SetPrinterBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
				//	}
				//	break;
				case MENU_KEY_DISPLAY:// 状态显示屏
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetDisplayComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetDisplayBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				//case MENU_KEY_BHCHANGE:// 纸币找零
				//	{
				//		if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theSETTING.SetBanknoteChangeComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
				//		if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theSETTING.SetBanknoteChangeBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
				//	}
				//	break;
				case MENU_KEY_IO:// 状态显示屏
					{
						if(thisMenu.emComportStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetIOComPort(thisMenu.strComport);thisMenu.emComportStatus = MENU_STATUS_UPDATED;}
						if(thisMenu.emBaudRateStatus == MENU_STATUS_SELECT){theTVM_SETTING.SetIOBaudRate(thisMenu.strBaudRate);thisMenu.emBaudRateStatus = MENU_STATUS_UPDATED;}
					}
					break;
				default:
					break;
				}
			}
		}
		
		m_data_model.bAlreadySaved = true;

		theAPP_SESSION.ShowOfficeGuide(TXT_DEVICE_SAVE_SUCCESS);
		
		// 刷新界面
		NotifyDataModelChanged();
		return TRUE;
	}

	theAPP_SESSION.ShowOfficeGuide(TXT_DEVICE_PLEASE_EDIT_COMPORT_BAUDRATE);

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
LRESULT CComportAndBaudrateSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/){
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      将串口号配置信息充配置文件中读取出来

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetSvc::ReadFromSettingIni(){

	int index = 0;
	m_data_model.stMenuDetail[index].strMenuName = /*_opl(GUIDE_SMARTCARD_RW_SET);*/_T("读写器");
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetRWComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetRWBaudRate());
	index++;

	//m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_BH);//.Format(_T("纸币接收"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetBHComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetBHBaudRate());
	//index++;

	m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_TH);//.Format(_T("票卡"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetTHComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetTHBaudRate());
	index++;

	m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_CH);//.Format(_T("硬币"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetCHComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetCHBaudRate());
	index++;

	//m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_PRINTER);//.Format(_T("打印机"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetPrinterComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetPrinterBaudRate());
	//index++;

	m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_DISPLAY);//.Format(_T("状态显示屏"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetDisplayComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetDisplayBaudRate());
	index++;
	//m_data_model.stMenuDetail[index].strMenuName = _opl(GUIDE_RECHARGE_SET);//.Format(_T("读写器"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetChargeRWComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetChargeRWBaudRate());
	//index++;

	//m_data_model.stMenuDetail[index].strMenuName = _opl(TXT_DEVICE_MOUDEL_BHCHANGE);//.Format(_T("纸币找零"));
	//m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theSETTING.GetBanknoteChangeComPort());
	//m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theSETTING.GetBanknoteChangeBaudRate());
	//index++;

	m_data_model.stMenuDetail[index].strMenuName.Format(_T("IO"));
	m_data_model.stMenuDetail[index].strComport.Format(_T("%d"),theTVM_SETTING.GetIOComPort());
	m_data_model.stMenuDetail[index].strBaudRate.Format(_T("%d"),theTVM_SETTING.GetIOBaudRate());
	index++;
}