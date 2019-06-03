#include "StdAfx.h"
#include "RemoveCoinChangeBoxDlg.h"
#include "RemoveCoinChangeBoxSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRemoveCoinChangeBoxDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CRemoveCoinChangeBoxDlg, COperationDlg)
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
CRemoveCoinChangeBoxDlg::CRemoveCoinChangeBoxDlg(CService* pService)
	: COperationDlg(CRemoveCoinChangeBoxDlg::IDD,pService)
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
CRemoveCoinChangeBoxDlg::~CRemoveCoinChangeBoxDlg()
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
int CRemoveCoinChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	//设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	//theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数



	//InitDetachCoinCollectionBoxInfo();
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
void CRemoveCoinChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);						// 设置最大输入位数

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 120);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 120);		// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 80);		// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 60);		// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 40);		// 设置第五列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 60);		// 设置第六列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,RESOLUTION * 70);		// 设置第六列宽度

	m_baseInfoArea->SetLableWidth(7,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(8,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(9,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(10,0,RESOLUTION * 230);

	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

	InitDetachCoinCollectionBoxInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxDlg::InitDetachCoinCollectionBoxInfo()
{
	
	for(int i = 0; i < 15; i++)
	{
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}

	CRemoveCoinChangeBoxSvc* pSvc = (CRemoveCoinChangeBoxSvc*)GetService();
	CRemoveCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.IsHasException)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_COIN_MODULE_EXCEPTION));
		theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
		//return;
	}
	CRemoveCoinChangeBoxSvc::CHANGE_BOX_INFO boxA = model.boxAinfo;
	CRemoveCoinChangeBoxSvc::CHANGE_BOX_INFO boxB = model.boxBinfo;

	m_lableGroup[0].name1 = _T("硬币找零箱A");							// 硬币回收箱A
	m_lableGroup[3].name1 = _T("硬币找零箱B");							// 硬币回收箱B

	m_lableGroup[7].name1 = _T("[1] 卸载硬币找零箱A");					// 卸载硬币回收箱A
	m_lableGroup[8].name1 = _T("[2] 卸载硬币找零箱B");					// 卸载硬币回收箱B
	m_lableGroup[9].name1 = _T("[3] 卸载所有硬币找零箱");				// 卸载所有硬币回收箱

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + boxA.strID;							// ID
	m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + boxB.strID;							// ID


	m_lableGroup[0].name3 = _T("面值")+_opl(TXT_COLON);											// 面额
	m_lableGroup[3].name3 = _T("面值")+_opl(TXT_COLON);
	// name5 面值
	m_lableGroup[0].name4 = _T("1 元")/*.Format(_T("%d"),boxA.value)*/;
	m_lableGroup[3].name4 = _T("1 元")/*Format(_T("%d"),boxB.value)*/;
	
	m_lableGroup[0].name5 = _T("数量")+_opl(TXT_COLON);											// 数量
	m_lableGroup[3].name5 = _T("数量")+_opl(TXT_COLON);											// 数量
	// name6 数量
	m_lableGroup[0].name6.Format(_T("%d"),boxA.nCount);
	m_lableGroup[3].name6.Format(_T("%d"),boxB.nCount);

	// name7
	m_lableGroup[0].name7 = boxA.StateToString();											  // 状态
	m_baseInfoArea->SetLabelColor(0,LABEL_COL_7,boxA.GetColor());
	m_lableGroup[3].name7 = boxB.StateToString();	
	m_baseInfoArea->SetLabelColor(3,LABEL_COL_7,boxB.GetColor());

	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxDlg::UpdateUI()
{
	/*CRemoveCoinChangeBoxSvc* pSvc = (CRemoveCoinChangeBoxSvc*)GetService();
	CRemoveCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();*/
	InitDetachCoinCollectionBoxInfo();
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
void CRemoveCoinChangeBoxDlg::ShowDetachCoinCollectionBoxInfo()
{
	CRemoveCoinChangeBoxSvc* pSvc = (CRemoveCoinChangeBoxSvc*)GetService();
	CRemoveCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();

}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示硬币回收箱卸载状态   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxDlg::ShowDoDetachCoinCollectionBoxInfo()
{
	
}