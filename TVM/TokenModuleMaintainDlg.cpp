#include "stdafx.h"
#include "TokenModuleMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTokenModuleMaintainDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CTokenModuleMaintainDlg, COperationDlg)
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
CTokenModuleMaintainDlg::CTokenModuleMaintainDlg(CService* pService)
	: COperationDlg(CTokenModuleMaintainDlg::IDD,pService)
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
CTokenModuleMaintainDlg::~CTokenModuleMaintainDlg()
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
int CTokenModuleMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}

	// 设置GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_labelGroupFour);

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
void CTokenModuleMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);	

	CTokenModuleTestSvc* pSvc = (CTokenModuleTestSvc*)GetService();
	CTokenModuleTestSvc::Modol& modol = pSvc->GetDataModol();
	
	Clear();

	if (pSvc->GetCurDialogGroupID() == CTokenModuleTestSvc::DIALOG_GROUP_MENU)
	{
		theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER);
		theAPP_SESSION.SetMaxInputLength(2);
		InitMenuUI();
	}
	else if (pSvc->GetCurDialogGroupID() == CTokenModuleTestSvc::DIALOG_GROUP_OPERATION)
	{
		if (modol.operationType == CTokenModuleTestSvc::OPERATION_TYPE_READ_ID)
		{
			theAPP_SESSION.SetKeyboard(KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(0);
			InitReadRfidUI();
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CONFORM_RFID);
		}
		else if (modol.operationType == CTokenModuleTestSvc::OPERATION_TYPE_WRITE_ID)
		{
			theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER|KEY_F2);
			theAPP_SESSION.SetMaxInputLength(5);
			InitWriteRfidUI();
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      更新界面

@param      none

@retval     none

@exception  none
*/
////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::UpdateUI()
{
	CTokenModuleTestSvc* pSvc = (CTokenModuleTestSvc*)GetService();
	CTokenModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	if (pSvc->GetCurDialogGroupID() == CTokenModuleTestSvc::DIALOG_GROUP_MENU)
	{
		UpdateMenuData();
	}
	else if (pSvc->GetCurDialogGroupID() == CTokenModuleTestSvc::DIALOG_GROUP_OPERATION)
	{
		if (modol.operationType == CTokenModuleTestSvc::OPERATION_TYPE_READ_ID)
		{
			UpdateReadRfidData();
		}
		else if (modol.operationType == CTokenModuleTestSvc::OPERATION_TYPE_WRITE_ID)
		{
			UpdateWriteRfidData();
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
void CTokenModuleMaintainDlg::InitMenuUI()
{
	m_baseInfoArea->SetLableWidth(0,200);
	m_baseInfoArea->SetLableWidth(1,150);
	m_baseInfoArea->SetLableWidth(2,150);
	m_baseInfoArea->SetLableWidth(3,100);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
	m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化读RFID界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::InitReadRfidUI()
{
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);
	m_baseInfoArea->SetLableWidth(2,0);
	m_baseInfoArea->SetLableWidth(3,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
	m_baseInfoArea->SetLableAlign(3,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化写RFID界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::InitWriteRfidUI()
{
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);
	m_baseInfoArea->SetLableWidth(2,0);
	m_baseInfoArea->SetLableWidth(3,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
	m_baseInfoArea->SetLableAlign(3,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新菜单显示结果

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::UpdateMenuData()
{
	//取数据
	CTokenModuleTestSvc* pSvc = (CTokenModuleTestSvc*)GetService();
	CTokenModuleTestSvc::Modol& modol = pSvc->GetDataModol();
	//1.主菜单
	m_labelGroupFour[0].name1 = _T("[1]") + _opl(TH_TEST_TEXT_RESET);
	if(theAPP_SESSION.IsSuperUser()){
		m_labelGroupFour[1].name1 = _T("[2]") + _opl(TH_TEST_TEXT_CATCH_TOKEN);
		m_labelGroupFour[2].name1 = _T("[3]") + _opl(TH_TEST_TEXT_THROW_TOKEN);
		m_labelGroupFour[3].name1 = _T("[4]") + _opl(TH_TEST_TEXT_COLLECT_TOKEN);
		m_labelGroupFour[4].name1 = _T("[5]") + _opl(TH_TEST_TEXT_CLEAR_TOKEN_BOX);
		//m_labelGroupFour[5].name1 = _T("[6]") + _opl(TH_TEST_TEXT_DETACH_TOKEN_BOX);
		//m_labelGroupFour[6].name1 = _T("[7]") + _opl(TH_TEST_TEXT_READ_TOKEN_BOX_ID);
		//m_labelGroupFour[7].name1 = _T("[8]") + _opl(TH_TEST_TEXT_WRITE_TOKEN_BOX_ID);
		//m_labelGroupFour[8].name1 = _T("[9]") + _opl(TH_TEST_TEXT_READ_VERSION);

		//2.子左菜单
		m_labelGroupFour[1].name2 = _T("[1]") + _opl(TH_TEST_TEXT_LEFT);
		//m_labelGroupFour[2].name2 = _T("[1]") + _opl(TH_TEST_TEXT_LEFT);
		//m_labelGroupFour[3].name2 = _T("[1]") + _opl(TH_TEST_TEXT_LEFT);
		m_labelGroupFour[4].name2 = _T("[1]") + _opl(TH_TEST_TEXT_LEFT);
		//m_labelGroupFour[5].name2 = _T("[1]") + _opl(TH_TEST_TEXT_LEFT);


		//3.子右菜单
		m_labelGroupFour[1].name3 = _T("[2]") + _opl(TH_TEST_TEXT_RIGHT);
		//m_labelGroupFour[2].name3 = _T("[2]") + _opl(TH_TEST_TEXT_RIGHT);
		//m_labelGroupFour[3].name3 = _T("[2]") + _opl(TH_TEST_TEXT_RIGHT);
		m_labelGroupFour[4].name3 = _T("[2]") + _opl(TH_TEST_TEXT_RIGHT);
		//m_labelGroupFour[5].name3 = _T("[2]") + _opl(TH_TEST_TEXT_RIGHT);
	}
		

	//4.执行结果显示
	for (int nRow = 0 ; nRow < 5; ++nRow)
	{
		m_labelGroupFour[nRow].name4 = modol.MenuInfo[nRow].strMenuStatus;     //结果：成功（绿），失败（红），完成
		m_baseInfoArea->SetLabelColor(nRow,3,modol.MenuInfo[nRow].strMenuStatus == FAILED ? RED:SOFTGREEN);//颜色
	}

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      更新读RFID数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::UpdateReadRfidData()
{
	CTokenModuleTestSvc* pSvc = (CTokenModuleTestSvc*)GetService();
	CTokenModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	m_labelGroupFour[0].name1   = _opl(TH_TEST_TEXT_TOKEN_ADDING_BOX_1);
	m_labelGroupFour[1].name1   = _opl(TH_TEST_TEXT_TOKEN_ADDING_BOX_2);
	
	//m_labelGroupThree[0].nameCenter = _T("");
	CString strTemp("");
	strTemp = _T("ID:");

	m_labelGroupFour[0].name2 = strTemp + modol.box_id.strRFIDBox1;
	m_labelGroupFour[1].name2 = strTemp + modol.box_id.strRFIDBox2;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新写RFID数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::UpdateWriteRfidData()
{
	CTokenModuleTestSvc* pSvc = (CTokenModuleTestSvc*)GetService();
	CTokenModuleTestSvc::Modol& modol = pSvc->GetDataModol();

	m_labelGroupFour[0].name1   = _T("[1]") + _opl(TH_TEST_TEXT_TOKEN_ADDING_BOX_1);
	m_labelGroupFour[1].name1   = _T("[2]") + _opl(TH_TEST_TEXT_TOKEN_ADDING_BOX_2);

	//m_labelGroupThree[0].nameCenter = _T("");
	CString strTemp("");
	strTemp = _T("ID:");

	m_labelGroupFour[0].name2 = strTemp + modol.box_id.strRFIDBox1;
	m_labelGroupFour[1].name2 = strTemp + modol.box_id.strRFIDBox2;	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      清除界面显示结果

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleMaintainDlg::Clear()
{
	for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
	{
		m_labelGroupFour[nRow].name1   = _T("");
		m_labelGroupFour[nRow].name2   = _T("");
		m_labelGroupFour[nRow].name3   = _T("");
		m_labelGroupFour[nRow].name4   = _T("");

		for(int nCol = 0; nCol < _MAX_COL; ++nCol)
		{
			m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
		}		
	} 
}