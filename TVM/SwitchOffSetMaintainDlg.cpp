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
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetMaintainDlg::CSwitchOffSetMaintainDlg(CService* pService)
:COperationDlg(CSwitchOffSetMaintainDlg::IDD,pService)
{
	DeviceType = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSwitchOffSetMaintainDlg::~CSwitchOffSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int 0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CSwitchOffSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(3);							// 设置最大输入位数

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,250);					// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,250);					// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,140);					// 设置第三列宽度

	//InitlableSwitchOffInfo();										// 初始化基本区域的label信息
	//SetFromReadIniText();											// 读取TVM.INI配置文件并显示在对应项上
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      无

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlableSwitchOffInfo();										// 初始化基本区域的label信息
	SetFromReadIniText();											// 读取TVM.INI配置文件并显示在对应项上
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::InitlableSwitchOffInfo()
{
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();											// 获取设备类型
	if(TVM_TYPE == DeviceType)
	{
		// 初始化信息区域文字
		int index = 0;
		m_lableGroup[index].nameLeft = _T("[0]")+_opl(GUIDE_RECHARGE_SET)+_opl(TXT_COLON);			// 充值读写器
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[1] ")+_opl(GUIDE_SMARTCARD_RW_SET)+_opl(TXT_COLON);		// Smart-Card读写器
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[2].nameLeft = _T("[3]")+_opl(GUIDE_TOKEN_RW_SET)+_opl(TXT_COLON);			// Token读写器
		//m_lableGroup[2].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		m_lableGroup[index].nameLeft = _T("[2] ")+_opl(GUIDE_BANKNOTE_SET)+_opl(TXT_COLON);			// 纸币处理模块
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		//m_lableGroup[index].nameLeft = _T("[3] ")+_opl(GUIDE_BANKNOTE_CHANGE_SET)+_opl(TXT_COLON);	// 纸币找零模块
		//m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[3] ")+_opl(GUIDE_SMARTCARD_MODULE_SET)+_opl(TXT_COLON);	// Smart-Card模块
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[5].nameLeft = _T("[6]")+_opl(GUIDE_TOKEN_MODULE_SET)+_opl(TXT_COLON);		// Token模块
		//m_lableGroup[5].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[4] ")+_opl(GUIDE_COIN_MODULE_SET)+_opl(TXT_COLON);		// 硬币处理模块
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		
		m_lableGroup[index].nameLeft = _T("[5] ")+_opl(GUIDE_RECEIPT_SET)+_opl(TXT_COLON);			// 凭证打印机
		m_lableGroup[index++].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[8].nameLeft = _T("[9]")+_opl(GUIDE_PINPAD_SET)+_opl(TXT_COLON);			// 密码键盘模块
		//m_lableGroup[8].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
		//m_lableGroup[9].nameLeft = _T("[10]")+_opl(GUIDE_BANKCARD_SET)+_opl(TXT_COLON);			// 银行卡模块
		//m_lableGroup[9].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	}
	//else if(AVM_TYPE == DeviceType)
	//{
	//	// 初始化信息区域文字
	//	m_lableGroup[0].nameLeft = _T("[1]")+_opl(GUIDE_RECHARGE_SET)+_opl(TXT_COLON);			// 充值读写器
	//	m_lableGroup[0].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[1].nameLeft = _T("[2]")+_opl(GUIDE_BANKNOTE_SET)+_opl(TXT_COLON);			// 纸币处理模块
	//	m_lableGroup[1].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[2].nameLeft = _T("[3]")+_opl(GUIDE_RECEIPT_SET)+_opl(TXT_COLON);			// 凭证打印机
	//	m_lableGroup[2].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[3].nameLeft = _T("[4]")+_opl(GUIDE_PINPAD_SET)+_opl(TXT_COLON);			// 密码键盘模块
	//	m_lableGroup[3].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	m_lableGroup[4].nameLeft = _T("[5]")+_opl(GUIDE_BANKCARD_SET)+_opl(TXT_COLON);			// 银行卡模块
	//	m_lableGroup[4].nameCenter = _opl(GUIDE_MENUCONTENT_SET);
	//	/*for(int i = 5; i < 10; i++)
	//	{
	//		m_lableGroup[i] = _T("");
	//	}*/
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示从TVM.INI读取的配置文件在对应项上

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::SetFromReadIniText()
{
	CSwitchOffSetSvc* pSvc = (CSwitchOffSetSvc*)GetService();
	CSwitchOffSetSvc::Model& model = pSvc->GetDataModel();
	if(TVM_TYPE == DeviceType)
	{
		int index = 0;
		m_lableGroup[index++].nameRight = model.MenuRechargeRW;										// 充值读写器
		m_lableGroup[index++].nameRight = model.MenuSmartcardRW;								// Smart-card读写器
		//m_lableGroup[2].nameRight = model.MenuTokenRW;										// Token读写器
		m_lableGroup[index++].nameRight = model.MenuBanknoteModule;								// 纸币模块
		//m_lableGroup[index++].nameRight = model.MenuBanknoteChangeModule;						// 纸币找零模块
		m_lableGroup[index++].nameRight = model.MenuSmartcardModule;							// Smart-card模块
		//m_lableGroup[5].nameRight = model.MenuTokenModule;									// Token模块
		m_lableGroup[index++].nameRight = model.MenuCoinModule;									// 硬币模块
		m_lableGroup[index++].nameRight = model.MenuReceiptPrint;								// 凭证打印机
		//m_lableGroup[8].nameRight = model.MenuPinPadModule;									// 密码键盘
		//m_lableGroup[9].nameRight = model.MenuBankCardModule;									// 银行卡模块
	}
	//else if(AVM_TYPE == DeviceType)
	//{
	//	m_lableGroup[0].nameRight = model.MenuRechargeRW;										// 充值读写器
	//	m_lableGroup[1].nameRight = model.MenuBanknoteModule;									// 纸币模块
	//	m_lableGroup[2].nameRight = model.MenuReceiptPrint;										// 凭证打印机
	//	m_lableGroup[3].nameRight = model.MenuPinPadModule;										// 密码键盘
	//	m_lableGroup[4].nameRight = model.MenuBankCardModule;									// 银行卡模块
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSwitchOffSetMaintainDlg::UpdateUI()
{
	CSwitchOffSetSvc* pSvc = (CSwitchOffSetSvc*)GetService();
	CSwitchOffSetSvc::Model& model = pSvc->GetDataModel();

	if(TRUE == model.CodeChange)		// 代码更新
	{
		ShowCodeContent();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      基本信息区显示对应项设置的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CSwitchOffSetMaintainDlg::ShowCodeContent()
{
	CSwitchOffSetSvc* pSvc = (CSwitchOffSetSvc*)GetService();
	CSwitchOffSetSvc::Model& model = pSvc->GetDataModel();	
	if(FALSE == model.CodeLegal )		// 代码内容非法
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	if(TVM_TYPE == DeviceType)
	{
		switch (model.MenuNumber)
		{
		case MENU_CODE_0:					// 充值读写器
			m_lableGroup[0].nameRight = model.MenuRechargeRW;
			break;
		case MENU_CODE_1:					// Smart-Card读写器
			m_lableGroup[1].nameRight = model.MenuSmartcardRW;
			break;
		//case MENU_CODE_3:					// Token读写器
		//	m_lableGroup[2].nameRight = model.MenuTokenRW;
		//	break;
		case MENU_CODE_2:					// 纸币处理模块
			m_lableGroup[2].nameRight = model.MenuBanknoteModule;
			break;
		//case MENU_CODE_3:					// 纸币找零模块
		//	m_lableGroup[3].nameRight = model.MenuBanknoteChangeModule;
		//	break;
		case MENU_CODE_3:					// Smart-Card模块
			m_lableGroup[3].nameRight = model.MenuSmartcardModule;
			break;
		//case MENU_CODE_6:					// Token模块
		//	m_lableGroup[5].nameRight = model.MenuTokenModule;
		//	break;
		case MENU_CODE_4:					// 硬币处理模块
			m_lableGroup[4].nameRight = model.MenuCoinModule;
			break;
		case MENU_CODE_5:					// 凭证打印机
			m_lableGroup[5].nameRight = model.MenuReceiptPrint;
			break;
		//case MENU_CODE_9:					// 密码键盘模块
		//	m_lableGroup[8].nameRight = model.MenuPinPadModule;
		//	break;
		//case MENU_CODE_10:					// 银行卡模块
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
	//	case MENU_CODE_1:					// 充值读写器
	//		m_lableGroup[0].nameRight = model.MenuRechargeRW;
	//		break;
	//	case MENU_CODE_2:					// 纸币处理模块
	//		m_lableGroup[1].nameRight = model.MenuBanknoteModule;
	//		break;
	//	case MENU_CODE_3:					// 凭证打印机
	//		m_lableGroup[2].nameRight = model.MenuReceiptPrint;
	//		break;
	//	case MENU_CODE_4:					// 密码键盘模块
	//		m_lableGroup[3].nameRight = model.MenuPinPadModule;
	//		break;
	//	case MENU_CODE_5:					// 银行卡模块
	//		m_lableGroup[4].nameRight = model.MenuBankCardModule;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}