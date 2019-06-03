#include "stdafx.h"
#include "CoinModuleTestSvc.h"
#include "CoinModuleMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinModuleMaintainDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CCoinModuleMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleMaintainDlg::CCoinModuleMaintainDlg(CService* pService)
	: COperationDlg(CCoinModuleMaintainDlg::IDD,pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinModuleMaintainDlg::~CCoinModuleMaintainDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:创建成功    -1:创建失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CCoinModuleMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}

	// 设置GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_labelGroupThree);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);	

	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	if (pSvc->GetCurDialogGroupID() == CCoinModuleTestSvc::DIALOG_GROUP_MENU)
	{
		//InitMenu
		theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_ENTER|KEY_F2);
		theAPP_SESSION.SetMaxInputLength(2);
		InitMenuUI();
	}
	else if (pSvc->GetCurDialogGroupID() == CCoinModuleTestSvc::DIALOG_GROUP_OPERATION)
	{
		//if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_RETURN_COIN)
		//{
		//	//Init return coin UI
		//	theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_ENTER);
		//	theAPP_SESSION.SetMaxInputLength(2);
		//	InitReturnCoinUI();
		//}else 
		if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_CLEAR_COIN)
		{
			//Init clear coin UI
			theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_ENTER|KEY_F2);
			theAPP_SESSION.SetMaxInputLength(5);
			InitClearCoinUI();
		}
		//else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_SAFELOCK_CTRL)
		//{
		//	//Init safelock UI
		//	theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_ENTER);
		//	theAPP_SESSION.SetMaxInputLength(2);
		//	InitSafeLockCtrlUI();
		//}
		//else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_READ_ID)
		//{
		//	//Init read id UI
		//	theAPP_SESSION.SetKeyboard(KEY_RETURN);
		//	theAPP_SESSION.SetMaxInputLength(0);
		//	InitReadIdUI();
		//}
		//else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_WRITE_ID)
		//{
		//	//Init write id UI
		//	theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_ENTER|KEY_F2);
		//	theAPP_SESSION.SetMaxInputLength(5);
		//	InitWriteIdUI();
		//}
		//else if(modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_ADDING_COIN)
		//{
		//	//Init add coin UI	
		//	theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_ENTER);
		//	theAPP_SESSION.SetMaxInputLength(2);
		//	InitAddCoinUI();
		//}
		else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_UNKNOW)
		{
			//Init menu UI
			InitMenuUI();
		}
	}

	//画面跳转后需要更新界面数据
	UpdateUI();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      更新界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateUI()
{
	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	if (pSvc->GetCurDialogGroupID() == CCoinModuleTestSvc::DIALOG_GROUP_MENU)
	{
		UpdateMenuData();
	}
	else if (pSvc->GetCurDialogGroupID() == CCoinModuleTestSvc::DIALOG_GROUP_OPERATION)
	{
		/*if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_RETURN_COIN)
		{
			UpdateReturnCoinData();
		}else*/ 
		if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_CLEAR_COIN)
		{
			UpdateClearCoinData();
		}
		/*else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_SAFELOCK_CTRL)
		{
		UpdateSafeLockCtrlData();;
		}
		else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_READ_ID)
		{
		UpdateReadIdData();
		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CONFORM_RFID);
		}
		else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_WRITE_ID)
		{
		UpdateWriteIdData();
		}
		else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_ADDING_COIN)
		{
		UpdateAddCoinData();
		}*/
		else if (modol.operationType == CCoinModuleTestSvc::OPERATION_TYPE_UNKNOW)
		{
			UpdateMenuData();
		}
	}

	//强制更新界面
	m_baseInfoArea->Invalidate();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化菜单界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitMenuUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,400);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化退币界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitReturnCoinUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,150);
	m_baseInfoArea->SetLableWidth(1,225);
	m_baseInfoArea->SetLableWidth(2,225);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化清币界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitClearCoinUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,150);
	m_baseInfoArea->SetLableWidth(1,250);
	m_baseInfoArea->SetLableWidth(2,200);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化安全扣控制界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitSafeLockCtrlUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,200);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,200);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化读RFID界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitReadIdUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);
	m_baseInfoArea->SetLableWidth(2,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化写RFID界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitWriteIdUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);
	m_baseInfoArea->SetLableWidth(2,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化加币界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::InitAddCoinUI()
{
	Clear();
	m_baseInfoArea->SetLableWidth(0,200);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,200);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新菜单数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateMenuData()
{
	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	//显示菜单项
	m_labelGroupThree[0].nameLeft  = _T("[1]") +_opl(COIN_TEST_TEXT_RESET);
	// 超级用户
	if(theAPP_SESSION.IsSuperUser() || theFunction_INFO.IsFounderOperation()){
		m_labelGroupThree[1].nameLeft  = _T("[2]") +_opl(COIN_TEST_TEXT_OPEN_COIN_SHUTTER);
		m_labelGroupThree[2].nameLeft  = _T("[3]") +_opl(COIN_TEST_TEXT_CLOSE_COIN_SHUTTER);
		m_labelGroupThree[3].nameLeft  = _T("[4]") +_opl(COIN_TEST_TEXT_START_COIN_RECEIVING);
		m_labelGroupThree[4].nameLeft  = _T("[5]") +_opl(COIN_TEST_TEXT_STOP_COIN_RECEIVING);
		m_labelGroupThree[5].nameLeft  = _T("[6]") +_opl(COIN_TEST_TEXT_ACEEPT_COIN);
		m_labelGroupThree[6].nameLeft  = _T("[7]") +_opl(COIN_TEST_TEXT_RELEASE_COIN);
		m_labelGroupThree[7].nameLeft  = _T("[8]") +_opl(COIN_TEST_TEXT_READ_AMOUNT);
		m_labelGroupThree[8].nameLeft  = _T("[9]") +_opl(COIN_TEST_TEXT_RETURN_COIN);
		//m_labelGroupThree[9].nameLeft  = _T("[10]") +_opl(COIN_TEST_TEXT_CLEAR_COIN);
		//m_labelGroupThree[10].nameLeft  = _T("[11]") +_opl(COIN_TEST_TEXT_START_ADDING);
		//m_labelGroupThree[9].nameLeft  = _T("[10]") +_opl(COIN_TEST_TEXT_STOP_ADDING);
		//m_labelGroupThree[10].nameLeft = _T("[11]") +_opl(COIN_TEST_TEXT_CONTORL_SAFE_LOCK);
		//m_labelGroupThree[11].nameLeft = _T("[12]") +_opl(COIN_TEST_TEXT_READ_BOX_ID);
		//m_labelGroupThree[12].nameLeft = _T("[13]") +_opl(COIN_TEST_TEXT_WRITE_BOX_ID);
		//m_labelGroupThree[13].nameLeft = _T("[14]") +_opl(COIN_TEST_TEXT_READ_VERSION);
	}	
	//显示状态，颜色
	for (int nRow = 0;nRow < 11; ++nRow)
	{
		m_labelGroupThree[nRow].nameCenter = modol.menuInfo[nRow].strMenuStatus;
		m_baseInfoArea->SetLabelColor(nRow,1,modol.menuInfo[nRow].strMenuStatus == FAILED ? RED : SOFTGREEN);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新退币界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateReturnCoinData()
{
	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	////显示菜单选项
	m_labelGroupThree[0].nameLeft   = _T("[1]") +_opl(COIN_TEST_TEXT_REGISTER_A);
	//m_labelGroupThree[0].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_COIN_CYCLE_CHANGE_BOX);
	//m_labelGroupThree[0].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_1);

	m_labelGroupThree[1].nameLeft   = _T("[2]") +_opl(COIN_TEST_TEXT_REGISTER_B);

	//显示状态，颜色
	/*for (int nRow = 0;nRow < 2; ++nRow)
	{*/
	m_labelGroupThree[0].nameRight = modol.returnCoinMenuInfo.str_cancel;
	m_baseInfoArea->SetLabelColor(0,1,modol.returnCoinMenuInfo.str_cancel == FAILED ? RED : SOFTGREEN);

	m_labelGroupThree[1].nameRight = modol.returnCoinMenuInfo.str_accept;
	m_baseInfoArea->SetLabelColor(1,1,modol.returnCoinMenuInfo.str_accept == FAILED ? RED : SOFTGREEN);
	//}

	/*m_labelGroupThree[1].nameCenter = _T("[3]") +_opl(COIN_TEST_TEXT_PASSENGER);
	m_labelGroupThree[1].nameRight  = _T("[4]") +_opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_2);*/

	/*m_labelGroupThree[2].nameLeft   = _T("");
	m_labelGroupThree[2].nameCenter = _T("");
	m_labelGroupThree[2].nameRight  = _T("");

	m_labelGroupThree[3].nameLeft   = _T("[2]") +_opl(COIN_TEST_TEXT_REGISTER_B);
	m_labelGroupThree[3].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_PASSENGER);
	m_labelGroupThree[3].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_2);*/

	////设置选中颜色
	////Menu 1.1
	//if (modol.returnCoinMenuInfo.bMenu_A1)
	//{
	//	m_baseInfoArea->SetLabelColor(0,1,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(0,1,BLACK);
	//}
	////Menu 1.2
	//if (modol.returnCoinMenuInfo.bMenu_A2)
	//{
	//	m_baseInfoArea->SetLabelColor(0,2,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(0,2,BLACK);
	//}
	////Menu 1.3
	//if (modol.returnCoinMenuInfo.bMenu_A3)
	//{
	//	m_baseInfoArea->SetLabelColor(1,1,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(1,1,BLACK);
	//}
	////Menu 1.4
	//if (modol.returnCoinMenuInfo.bMenu_A4)
	//{
	//	m_baseInfoArea->SetLabelColor(1,2,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(1,2,BLACK);
	//}
	////Menu 2.1
	//if (modol.returnCoinMenuInfo.bMenu_B1)
	//{
	//	m_baseInfoArea->SetLabelColor(3,1,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(3,1,BLACK);
	//}
	////Menu 2.2
	//if (modol.returnCoinMenuInfo.bMenu_B2)
	//{
	//	m_baseInfoArea->SetLabelColor(3,2,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(3,2,BLACK);
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新清币界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateClearCoinData()
{
	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();
	CString strNum[4];
	strNum[0].Format(_T("%d"),modol.clearCoinMenuInfo.iClsMenu_A1);
	strNum[1].Format(_T("%d"),modol.clearCoinMenuInfo.iClsMenu_A2);
	strNum[2].Format(_T("%d"),modol.clearCoinMenuInfo.iClsMenu_B1);
	strNum[3].Format(_T("%d"),modol.clearCoinMenuInfo.iClsMenu_B2);
	//显示菜单选项
	m_labelGroupThree[0].nameLeft   = _T("[1]") +_opl(COIN_TEST_TEXT_RECYCLE_HOPPER_A);
	if (modol.clearCoinMenuInfo.bClsMenu_A1)
		m_labelGroupThree[0].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER) 
		+ _T("  ( ") + strNum[0] + _T(" )");
	else
		m_labelGroupThree[0].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER);
	if (modol.clearCoinMenuInfo.bClsMenu_A1_All)
		m_labelGroupThree[0].nameRight = _T("[2]") +_opl(COIN_TEST_TEXT_ALL)
		+ _T("  ( ") + strNum[0] + _T(" )");
	else
		m_labelGroupThree[0].nameRight = _T("[2]") +_opl(COIN_TEST_TEXT_ALL);


	//m_labelGroupThree[1].nameLeft   = _T("[2]") +_opl(COIN_TEST_TEXT_RECYCLE_HOPPER_B);
	//if (modol.clearCoinMenuInfo.bClsMenu_A2)
	//	m_labelGroupThree[1].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER) 
	//	+ _T("  ( ") + strNum[1] + _T(" )");
	//else
	//	m_labelGroupThree[1].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER);
	//if (modol.clearCoinMenuInfo.bClsMenu_A2_All)
	//	m_labelGroupThree[1].nameRight = _T("[2]") +_opl(COIN_TEST_TEXT_ALL)
	//	+ _T("  ( ") + strNum[1] + _T(" )");
	//else
	//	m_labelGroupThree[1].nameRight = _T("[2]") +_opl(COIN_TEST_TEXT_ALL);


	m_labelGroupThree[1].nameLeft   = _T("[2]") +_opl(COIN_TEST_TEXT_RECYCLE_SPARE_BOX_A);
	if (modol.clearCoinMenuInfo.bClsMenu_B1)
		m_labelGroupThree[1].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER)
		+ _T("  ( ") + strNum[2] + _T(" )");
	else
		m_labelGroupThree[1].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER);
	if (modol.clearCoinMenuInfo.bClsMenu_B1_All)
	m_labelGroupThree[1].nameRight = _T("[2]") +_opl(COIN_TEST_TEXT_ALL)
	+ _T("  ( ") + strNum[2] + _T(" )");
	else
		m_labelGroupThree[1].nameRight = _T("[2]") +_opl(COIN_TEST_TEXT_ALL);


	m_labelGroupThree[2].nameLeft   = _T("[3]") +_opl(COIN_TEST_TEXT_RECYCLE_SPARE_BOX_B);
	if (modol.clearCoinMenuInfo.bClsMenu_B2)
		m_labelGroupThree[2].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER)
		+ _T("  ( ") + strNum[3] + _T(" )");
	else
		m_labelGroupThree[2].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_NUMBER);
	if (modol.clearCoinMenuInfo.bClsMenu_B2_All)
		m_labelGroupThree[2].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_ALL)
		+ _T("  ( ") + strNum[3] + _T(" )");
	else
		m_labelGroupThree[2].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_ALL);


	m_labelGroupThree[3].nameLeft   = _T("[4]") +_opl(COIN_TEST_TEXT_RECYCLE_ALL);
	//设置选中颜色
	//Menu 1
	if (modol.clearCoinMenuInfo.bClsMenu_A1)
	{
		m_baseInfoArea->SetLabelColor(0,1,SOFTGREEN);
	}
	else
	{
		m_baseInfoArea->SetLabelColor(0,1,BLACK);
	}
	if (modol.clearCoinMenuInfo.bClsMenu_A1_All)
	{
		m_baseInfoArea->SetLabelColor(0,2,SOFTGREEN);
	}
	else
	{
		m_baseInfoArea->SetLabelColor(0,2,BLACK);
	}

	//Menu 2
	//if (modol.clearCoinMenuInfo.bClsMenu_A2)
	//{
	//	m_baseInfoArea->SetLabelColor(1,1,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(1,1,BLACK);
	//}
	//if (modol.clearCoinMenuInfo.bClsMenu_A2_All)
	//{
	//	m_baseInfoArea->SetLabelColor(1,2,SOFTGREEN);
	//}
	//else
	//{
	//	m_baseInfoArea->SetLabelColor(1,2,BLACK);
	//}


	//Menu 3
	if (modol.clearCoinMenuInfo.bClsMenu_B1)
	{
		m_baseInfoArea->SetLabelColor(1,1,SOFTGREEN);
	}
	else
	{
		m_baseInfoArea->SetLabelColor(1,1,BLACK);
	}
	if (modol.clearCoinMenuInfo.bClsMenu_B1_All)
	{
		m_baseInfoArea->SetLabelColor(1,2,SOFTGREEN);
	}
	else
	{
		m_baseInfoArea->SetLabelColor(1,2,BLACK);
	}

	//Menu 4
	if (modol.clearCoinMenuInfo.bClsMenu_B2)
	{
		m_baseInfoArea->SetLabelColor(2,1,SOFTGREEN);
	}
	else
	{
		m_baseInfoArea->SetLabelColor(2,1,BLACK);
	}
	if (modol.clearCoinMenuInfo.bClsMenu_B2_All)
	{
		m_baseInfoArea->SetLabelColor(2,2,SOFTGREEN);
	}
	else
	{
		m_baseInfoArea->SetLabelColor(2,2,BLACK);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新安全扣控制界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateSafeLockCtrlData()
{
	//菜单选项
	m_labelGroupThree[0].nameLeft   = _T("[1]") +_opl(COIN_TEST_TEXT_COIN_ADDING_BOX_1);
	m_labelGroupThree[0].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_LOCK);
	m_labelGroupThree[0].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_UNLOCK);

	m_labelGroupThree[1].nameLeft   = _T("[2]") +_opl(COIN_TEST_TEXT_COIN_ADDING_BOX_2);
	m_labelGroupThree[1].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_LOCK);
	m_labelGroupThree[1].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_UNLOCK);

	//Don't Need Color
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新读RFID界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateReadIdData()
{
	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	m_labelGroupThree[0].nameLeft   = _opl(COIN_TEST_TEXT_COIN_ADDING_BOX_1);
	m_labelGroupThree[1].nameLeft   = _opl(COIN_TEST_TEXT_COIN_ADDING_BOX_2);
	m_labelGroupThree[2].nameLeft   = _opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_1);
	m_labelGroupThree[3].nameLeft   = _opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_2);
	//m_labelGroupThree[0].nameCenter = _T("");
	CString strTemp("");
	strTemp = _T("ID:");

	m_labelGroupThree[0].nameCenter = strTemp + modol.rwCoinBoxId.strCAddingBoxA;
	m_labelGroupThree[1].nameCenter = strTemp + modol.rwCoinBoxId.strCAddingBoxB;
	m_labelGroupThree[2].nameCenter = strTemp + modol.rwCoinBoxId.strCCollectionBoxA;
	m_labelGroupThree[3].nameCenter = strTemp + modol.rwCoinBoxId.strCCollectionBoxB;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新写RFID界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateWriteIdData()
{
	CCoinModuleTestSvc* pSvc = (CCoinModuleTestSvc*)GetService();
	CCoinModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	m_labelGroupThree[0].nameLeft   = _T("[1]") +_opl(COIN_TEST_TEXT_COIN_ADDING_BOX_1);
	m_labelGroupThree[1].nameLeft   = _T("[2]") +_opl(COIN_TEST_TEXT_COIN_ADDING_BOX_2);
	m_labelGroupThree[2].nameLeft   = _T("[3]") +_opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_1);
	m_labelGroupThree[3].nameLeft   = _T("[4]") +_opl(COIN_TEST_TEXT_COIN_COLLECTION_BOX_2);
	//m_labelGroupThree[0].nameCenter = _T("");
	CString strTemp("");
	strTemp = _T("ID:");

	m_labelGroupThree[0].nameCenter = strTemp + modol.rwCoinBoxId.strCAddingBoxA;
	m_labelGroupThree[1].nameCenter = strTemp + modol.rwCoinBoxId.strCAddingBoxB;
	m_labelGroupThree[2].nameCenter = strTemp + modol.rwCoinBoxId.strCCollectionBoxA;
	m_labelGroupThree[3].nameCenter = strTemp + modol.rwCoinBoxId.strCCollectionBoxB;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新加币界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::UpdateAddCoinData()
{
	//菜单选项
	m_labelGroupThree[0].nameLeft   = _T("[1]") +_opl(COIN_TEST_TEXT_COIN_ADDING_BOX);
	m_labelGroupThree[0].nameCenter = _T("[1]") +_opl(COIN_TEST_TEXT_BOX_1);
	m_labelGroupThree[0].nameRight  = _T("[2]") +_opl(COIN_TEST_TEXT_BOX_2);

	m_labelGroupThree[1].nameLeft   = _T("");
	m_labelGroupThree[1].nameCenter = _T("[3]") +_opl(COIN_TEST_TEXT_BOX_1_AND_2);
	m_labelGroupThree[1].nameRight  = _T("");

	//Don't Need Color
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清除界面显示结果

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinModuleMaintainDlg::Clear()
{
	for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
	{
		m_labelGroupThree[nRow].nameLeft   = _T("");
		m_labelGroupThree[nRow].nameCenter = _T("");
		m_labelGroupThree[nRow].nameRight  = _T("");

		for(int nCol = 0; nCol < _MAX_COL; ++nCol)
		{
			m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
		}		
	} 
}