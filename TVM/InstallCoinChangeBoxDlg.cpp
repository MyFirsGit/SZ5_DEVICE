#include "stdafx.h"
#include "InstallCoinChangeBoxDlg.h"
#include "InstallCoinChangeBoxSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CInstallCoinChangeBoxDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CInstallCoinChangeBoxDlg, COperationDlg)
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
CInstallCoinChangeBoxDlg::CInstallCoinChangeBoxDlg(CService* pService)
	: COperationDlg(CInstallCoinChangeBoxDlg::IDD,pService)
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
CInstallCoinChangeBoxDlg::~CInstallCoinChangeBoxDlg()
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
int CInstallCoinChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	//设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	//theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数

	//m_baseInfoArea->SetLableWidth(LABEL_COL_1,220);		// 设置第一列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_2,80);		// 设置第二列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_3,150);		// 设置第三列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_4,150);		// 设置第四列宽度

	//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);

	//InitAttachCoinCollectionBoxInfo();
	//ShowAttachCoinCollectionBoxInfo();

	return 0;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用

@param      (i)CWnd* pWndOther  不使用

@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);						// 设置最大输入位数

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 170);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 150);		// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 150);		// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 120);		// 设置第四列宽度

	m_baseInfoArea->SetLableAlign(LABEL_COL_1,  DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2,  DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);

	m_baseInfoArea->SetLableWidth(3,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(4,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(5,0,RESOLUTION * 230);

	InitAttachCoinCollectionBoxInfo();
	ShowAttachCoinCollectionBoxInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::InitAttachCoinCollectionBoxInfo()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.IsHasException)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_COIN_MODULE_EXCEPTION));
		theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
		//return;
	}

	for(int i = 0; i < 15; i++)
	{
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
	}

	m_lableGroup[0].name1 = _T("硬币找零箱A");							
	m_lableGroup[1].name1 = _T("硬币找零箱B");							

	m_lableGroup[3].name1 = _T("[1] 安装硬币找零箱A");			// 安装 A
	m_lableGroup[4].name1 = _T("[2] 安装硬币找零箱B");			// 安装 B
	m_lableGroup[5].name1 = _T("[3] 安装所有硬币找零箱");			// 安装所有 

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::UpdateUI()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();
	if(1 == model.AttachCoinCollectionBoxFlowTime)
	{
		ShowAttachCoinCollectionBoxInfo();
	}
	/*else if(2 == model.AttachCoinCollectionBoxFlowTime)
	{
		ShowDoAttachCoinCollectionBoxInfo();
	}*/
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示硬币回收箱状态 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::ShowAttachCoinCollectionBoxInfo()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();

	// 设置颜色
	SetColor(0,model.CoinCollectionBoxAStatus);
	SetColor(1,model.CoinCollectionBoxBStatus);

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxAID;				// 硬币回收箱AIDData
	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxBID;				// 硬币回收箱BIDData

	m_lableGroup[0].name3 = _T("数量:") + model.strCoinACount;				// 硬币回收箱AIDData
	m_lableGroup[1].name3 = _T("数量:") + model.strCoinBCount;				// 硬币回收箱BIDData

	m_lableGroup[0].name4 = model.strGetState(model.stateA);			// 硬币回收箱A安装Data
	m_lableGroup[1].name4 = model.strGetState(model.stateB);			// 硬币回收箱B安装Data
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置状态显示的颜色

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::SetColor(int row,CString status)
{
	if(_opl(TXT_COINMGR_BOX_ATTACHED) == status || _opl(TXT_TOKENMGR_WAITTOATTACH) == status)						// 安装-绿色
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_4,GREEN);
	}
	else																// 卸载-红色
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_4,RED);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示硬币回收箱安装状态 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::ShowDoAttachCoinCollectionBoxInfo()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();

	// 设置颜色
	SetColor(1,model.CoinCollectionBoxAStatus);
	SetColor(2,model.CoinCollectionBoxBStatus);
	SetColor(5,model.CoinCollectionAfterBoxAStatus);
	SetColor(6,model.CoinCollectionAfterBoxBStatus);

	m_lableGroup[0].name2 = _T("");
	m_lableGroup[0].name3 = _T("");
	m_lableGroup[0].name4 = _T("");
	m_lableGroup[3].name1 = _T("");																			// 清空

	model.IsPrintFalied ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ATTACH_FINISH));
	m_lableGroup[0].name1 = _opl(TXT_COINMGR_BOX_BEFORE_ATTACH);											// 安装前
	m_lableGroup[1].name1 = _T("硬币找零箱A");	
	m_lableGroup[2].name1 = _T("硬币找零箱B");							

	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxAID;			// ID
	m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxBID;			// ID

	m_lableGroup[1].name3 = _T("数量:")+model.strCoinACount;												// 硬币数量
	m_lableGroup[2].name3 = _T("数量:")+model.strCoinBCount;												// 硬币数量

	m_lableGroup[1].name4 = model.strGetState(model.stateA);															// 状态
	m_lableGroup[2].name4 = model.strGetState(model.stateB);															// 状态

	m_lableGroup[4].name1 = _opl(TXT_COINMGR_BOX_AFTER_ATTACH);									// 安装后
	m_lableGroup[5].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);										 
	m_lableGroup[6].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);										 

	m_lableGroup[5].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinCollctionBoxAfterAttachAID; 
	m_lableGroup[6].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinCollctionBoxAfterAttachAID; 

	m_lableGroup[5].name3 = _T("数量:")+model.strCoinACountAfter;									
	m_lableGroup[6].name3 = _T("数量:")+model.strCoinBCountAfter;								

	m_lableGroup[5].name4 = /*model.CoinCollectionAfterBoxAStatus*/_T("");						// 硬币回收箱A安装后Status
	m_lableGroup[6].name4 = /*model.CoinCollectionAfterBoxBStatus*/_T("");						// 硬币回收箱B安装后Status
}