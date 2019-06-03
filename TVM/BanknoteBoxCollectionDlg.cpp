#include "stdafx.h"
#include "BanknoteBoxCollectionSvc.h"
#include "BanknoteBoxCollectionDlg.h"

static const int FULL_WITDH = 600;
static const int NAME_WITDH = 400;
static const int ID_WITDH = 150;
static const int INDENT_WIDTH = 50;
static const int GIRD_WITDH = 160;
static const int STATUE_WIDTH = 100;
static const int FACE_WIDTH = 80;
static const int FACE_WIDTH_1 = 120;
static const int COUNT_WIDTH = 50;
static const int MONEY_WIDTH = 130;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBanknoteBoxCollectionDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CBanknoteBoxCollectionDlg, COperationDlg)
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
CBanknoteBoxCollectionDlg::CBanknoteBoxCollectionDlg(CService* pService)
	: COperationDlg(CBanknoteBoxCollectionDlg::IDD,pService)
{
	pSvc = (CBanknoteBoxCollectionSvc*)pService;
}

////////////////////////////// ////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxCollectionDlg::~CBanknoteBoxCollectionDlg()
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
int CBanknoteBoxCollectionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}

	// 设置GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);

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
void CBanknoteBoxCollectionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();

	// 清除界面内容
	Clear();

	theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_ENTER|KEY_INPUT|KEY_F2);
	theAPP_SESSION.SetMaxInputLength(2);

	// 基本画面（菜单选择）
	if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_BASE) {
		InitBaseUI();
		//if (!pSvc->IsBoxConnected())
		//{
		//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_FAILED_CONNECT)/*T("通讯异常，请检查与纸币箱的连接。")*/);
		//}
	}
	// 操作画面（安装、卸载）
	//else if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION || pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_FINISH) {
	//	if (model.operation_type == CBanknoteBoxCollectionSvc::OPERATION_ATTACH_BOX) {
	//		InitAttachBoxUI();
	//	}
	//	else if (model.operation_type == CBanknoteBoxCollectionSvc::OPERATION_DETACH_BOX) {
	//		InitDetachBoxUI();
	//	}
	//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//	// 显示错误文言
	//	if (!pSvc->GetShowErrorMsg().IsEmpty() && pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_FINISH){
	//		theAPP_SESSION.ShowOfficeGuide(pSvc->GetShowErrorMsg());
	//	}
	//}
	//else {
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱管理dlg流错误"));
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
void CBanknoteBoxCollectionDlg::UpdateUI()
{
	CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();

	// 清除界面内容
	Clear();
	// 基本画面（菜单选择）
	if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_BASE)
	{
		ShowBaseData();
		//if (!pSvc->IsBoxConnected())
		//{
		//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_FAILED_CONNECT)/*T("通讯失败，请检查与纸币箱的连接。")*/);
		//}
	}
	// 操作画面（安装、卸载）
	/*else if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION || pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_FINISH)
	{
		if (model.operation_type == CBanknoteBoxCollectionSvc::OPERATION_ATTACH_BOX)
		{
			ShowAttachBoxData();
		}
		else if (model.operation_type == CBanknoteBoxCollectionSvc::OPERATION_DETACH_BOX)
			ShowDetachBoxData();
	}
	else {
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱管理dlg流错误"));
	}*/
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		基本界面布局初始化

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxCollectionDlg::InitBaseUI()
{
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, STATUE_WIDTH);
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, STATUE_WIDTH);
	m_baseInfoArea->SetLableWidth(LABEL_COL_3, STATUE_WIDTH);
	m_baseInfoArea->SetLableWidth(LABEL_COL_4, STATUE_WIDTH);
	m_baseInfoArea->SetLableWidth(LABEL_COL_5, STATUE_WIDTH);
	m_baseInfoArea->SetLableWidth(LABEL_COL_6, STATUE_WIDTH);
	m_baseInfoArea->SetLableWidth(LABEL_COL_7, 0);

	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);
	//CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();
	//model.bIsBanknoteModuleException ? NULL:theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		安装纸币箱画面布局初始化   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CBanknoteBoxCollectionDlg::InitAttachBoxUI()
//{
//	// 基本信息格式
//	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
//	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
//	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
//
//	m_baseInfoArea->SetLableWidth(LABEL_COL_1, NAME_WITDH);					// 纸币箱
//	//m_baseInfoArea->SetLableWidth(LABEL_COL_2, ID_WITDH);					// 编号
//	m_baseInfoArea->SetLableWidth(LABEL_COL_2, STATUE_WIDTH);				// 是否安装
//
//	// 菜单栏格式
//	if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION)
//	{
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2, LABEL_COL_1, FULL_WITDH);	// 菜单1
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief		卸载纸币箱画面布局初始化   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CBanknoteBoxCollectionDlg::InitDetachBoxUI()
//{
//	// 基本信息格式
//	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
//	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
//	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
//
//	m_baseInfoArea->SetLableWidth(LABEL_COL_1, NAME_WITDH);					// 纸币箱
//	m_baseInfoArea->SetLableWidth(LABEL_COL_2, ID_WITDH);					// 编号
//	m_baseInfoArea->SetLableWidth(LABEL_COL_3, STATUE_WIDTH);				// 是否安装
//
//	// 纸币信息显示栏格式
//	int offset = (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION) ? 0 : 1; // 完成页面比执行页面多一行
//
//	for (int j = 0; j < 4; ++j)
//	{
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2 + offset + j, LABEL_COL_1, FACE_WIDTH);	// 币种
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2 + offset + j, LABEL_COL_2, COUNT_WIDTH);	// 张数
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2 + offset + j, LABEL_COL_3, MONEY_WIDTH);	// 合计
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2 + offset + j, LABEL_COL_4, FACE_WIDTH_1);	// 币种
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2 + offset + j, LABEL_COL_5, COUNT_WIDTH);	// 张数
//		m_baseInfoArea->SetLableWidth(LABEL_ROW_2 + offset + j, LABEL_COL_6, MONEY_WIDTH);	// 合计
//
//		m_baseInfoArea->SetLableAlign(LABEL_ROW_2 + offset + j, LABEL_COL_1, DT_RIGHT | DT_VCENTER);	// 币种
//		m_baseInfoArea->SetLableAlign(LABEL_ROW_2 + offset + j, LABEL_COL_2, DT_RIGHT | DT_VCENTER);	// 张数
//		m_baseInfoArea->SetLableAlign(LABEL_ROW_2 + offset + j, LABEL_COL_3, DT_RIGHT | DT_VCENTER);	// 合计
//		m_baseInfoArea->SetLableAlign(LABEL_ROW_2 + offset + j, LABEL_COL_4, DT_RIGHT | DT_VCENTER);	// 币种
//		m_baseInfoArea->SetLableAlign(LABEL_ROW_2 + offset + j, LABEL_COL_5, DT_RIGHT | DT_VCENTER);	// 张数
//		m_baseInfoArea->SetLableAlign(LABEL_ROW_2 + offset + j, LABEL_COL_6, DT_RIGHT | DT_VCENTER);	// 合计
//	}
//
//	// 总金额统计页面
//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6/*LABEL_ROW_5*/ + offset, LABEL_COL_1, FACE_WIDTH );
//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6 + offset, LABEL_COL_2, COUNT_WIDTH );
//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6+ offset, LABEL_COL_3, MONEY_WIDTH );
//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6 + offset, LABEL_COL_4, FACE_WIDTH_1 );
//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6 + offset, LABEL_COL_5, COUNT_WIDTH );
//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6 + offset, LABEL_COL_6, MONEY_WIDTH );
//
//	m_baseInfoArea->SetLableAlign(LABEL_ROW_6/*LABEL_ROW_5*/ + offset, LABEL_COL_4, DT_RIGHT | DT_VCENTER);
//	m_baseInfoArea->SetLableAlign(LABEL_ROW_6 + offset, LABEL_COL_6, DT_RIGHT | DT_VCENTER);
//
//	// 菜单栏格式
//	//if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION)
//	//{
//	//	m_baseInfoArea->SetLableWidth(LABEL_ROW_6, LABEL_COL_1, FULL_WITDH);	// 菜单1
//	//}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief		清空数据   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxCollectionDlg::Clear()
{
	for (int i = 0; i < 15; ++i)
	{
		m_lableGroup[i].name1 = _T(""); 
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}
	// 应框架更改，解决更换语言模式登入后纸币箱功能的提示语言为首次语言模式的bug
	//CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();
	//switch( pSvc->GetCurDialogGroupID())
	//{
	//case CBanknoteBoxCollectionSvc::DIALOG_GROUP_BASE:
	//case CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION:
	//	break;//theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);break;
	//case CBanknoteBoxCollectionSvc::DIALOG_GROUP_FINISH:
	//case CBanknoteBoxCollectionSvc::DIALOG_GROUP_ERROR:
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_F3_ESC);break;
	//default:
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);break;
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示菜单选项

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxCollectionDlg::ShowBaseData()
{
	//m_lableGroup[LABEL_ROW_1].name1.Format(_T("[%d]")+_opl(TXT_BANKNOTEMGR_MENU_ATTACH), MENU_CODE_1);
	//m_lableGroup[LABEL_ROW_2].name1.Format(_T("[%d]")+_opl(TXT_BANKNOTEMGR_MENU_DETACH), MENU_CODE_2);
	CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();

	int indexRow = 0;
	m_lableGroup[indexRow].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);
	m_lableGroup[indexRow].name2.Format(_T("1") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteCashInfo.ulCount_CNY1);
	m_lableGroup[indexRow].name3.Format(_T("5") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteCashInfo.ulCount_CNY5);
	m_lableGroup[indexRow].name4.Format(_T("10") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteCashInfo.ulCount_CNY10);
	m_lableGroup[indexRow].name5.Format(_T("20") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteCashInfo.ulCount_CNY20);

	m_lableGroup[indexRow++].name6.Format(_opl(DEVICESTATUS_NORMAL_SET));

	m_lableGroup[indexRow].name2.Format(_T("50") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteCashInfo.ulCount_CNY50);
	m_lableGroup[indexRow++].name3.Format(_T("100") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteCashInfo.ulCount_CNY100);

	// 找零箱
	m_lableGroup[indexRow].name1 = _opl(DEVICESTATUS_SUPPLEMENT_BANKNOTE_SET);
	m_lableGroup[indexRow].name2.Format(_T("5") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.banknoteChangeBoxInfo.ulCurCount);
	m_lableGroup[indexRow++].name6.Format(_opl(DEVICESTATUS_NORMAL_SET));

	// 循环鼓A
	m_lableGroup[indexRow].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_A_SET);
	m_lableGroup[indexRow].name2.Format(_T("10") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.cycleBoxAInfo.ulCurCount);
	m_lableGroup[indexRow++].name6.Format(_opl(DEVICESTATUS_NORMAL_SET));

	// 循环鼓B
	m_lableGroup[indexRow].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_B_SET);
	m_lableGroup[indexRow].name2.Format(_T("5") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.cycleBoxBInfo.ulCurCount);
	m_lableGroup[indexRow++].name6.Format(_opl(DEVICESTATUS_NORMAL_SET));

	// 循环鼓C
	m_lableGroup[indexRow].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_C_SET);
	m_lableGroup[indexRow].name2.Format(_T("5") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.cycleBoxCInfo.ulCurCount);
	m_lableGroup[indexRow++].name6.Format(_opl(DEVICESTATUS_NORMAL_SET));

	// 循环鼓D
	m_lableGroup[indexRow].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_D_SET);
	m_lableGroup[indexRow].name2.Format(_T("10") + _opl(TXT_TAKE_TICKETS_YUAN) + _T("：%d"),model.cycleBoxDInfo.ulCurCount);
	m_lableGroup[indexRow++].name6.Format(_opl(DEVICESTATUS_NORMAL_SET));

	m_baseInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示安装纸币箱数据   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CBanknoteBoxCollectionDlg::ShowAttachBoxData()
//{
//	CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();
//	CString strID = _T("--------");
//	if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION)
//	{
//		if(model.banknote_box.raw_id == _T("FFFFFFFF"))
//		{
//			strID = _T("--------");
//		}
//		else
//			strID = model.banknote_box.raw_id;
//		m_lableGroup[LABEL_ROW_1].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+ _T(" %s"), strID);
//		if(!model.bIsSelectInstall){
//			m_lableGroup[LABEL_ROW_1].name2.Format(_T("%s"), model.banknote_box.now_attached ? _opl(TXT_BANKNOTEMGR_BOX_ATTACHED) : _opl(TXT_BANKNOTEMGR_BOX_DETACHED));
//			m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2, model.banknote_box.raw_attached ? SOFTGREEN : RED);
//		}
//		else{
//			m_lableGroup[LABEL_ROW_1].name2 = _opl(TXT_TOKENMGR_WAITTOATTACH);// 待安装
//			m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2,SOFTGREEN);
//		}
//		//if( pSvc->GetOperationType() == CBanknoteBoxCollectionSvc::VIOLATE_UNINSTALL)
//		//{
//		//	//箱子实际上安装，但是软计数是卸载状态，非法安装
//		//	m_lableGroup[LABEL_ROW_1].name2 = _opl(TXT_BANKNOTEMGR_ILLEGAL_UNINSTALL)/*_T("非法卸载")*/;
//		//}
//		m_lableGroup[LABEL_ROW_2].name1.Format(_T("[%d]")+_opl(TXT_BANKNOTEMGR_MENU_ATTACH), MENU_CODE_1);	
//	}
//	else if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_FINISH)
//	{
//		m_lableGroup[LABEL_ROW_1].name1 = _opl(TXT_BANKNOTEMGR_BOX_BEFORE_ATTACH);
//
//		m_lableGroup[LABEL_ROW_2].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+ _T(" %s"), _T("--------"));
//
//		m_lableGroup[LABEL_ROW_6].name1 = _opl(TXT_BANKNOTEMGR_BOX_AFTER_ATTACH);
//
//		m_lableGroup[LABEL_ROW_7].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+_T(" %s"), model.banknote_box.now_id);
//	}
//	else
//	{
//		Clear();
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示卸载回收箱数据   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CBanknoteBoxCollectionDlg::ShowDetachBoxData()
//{
//	CBanknoteBoxCollectionSvc::Model& model = pSvc->GetDataModel();
//	CString strID = _T("--------");
//	// 纸币箱卸载操作界面
//	if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_OPERATION)
//	{
//		if(model.banknote_box.raw_id == _T("FFFFFFFF"))
//		{
//			strID = _T("--------");
//		}
//		else
//			strID = model.banknote_box.raw_id;
//		m_lableGroup[LABEL_ROW_1].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+_T(" %s"), strID);
//		if(!model.bIsSelectUninstall){
//			m_lableGroup[LABEL_ROW_1].name2.Format(_T("%s"), model.banknote_box.now_attached ? _opl(TXT_BANKNOTEMGR_BOX_ATTACHED) : _opl(TXT_BANKNOTEMGR_BOX_DETACHED));
//			m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2, model.banknote_box.raw_attached ? SOFTGREEN : RED);
//		}
//		else{
//			m_lableGroup[LABEL_ROW_1].name2 = _opl(TXT_TOKENMGR_WAITTODETACH);
//			m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2, RED);
//		}
//		if( pSvc->GetOperationType() == CBanknoteBoxCollectionSvc::VIOLATE_INSTALL)
//		{
//			//箱子实际上安装，但是软计数是卸载状态，非法安装
//			//m_lableGroup[LABEL_ROW_1].name2 = _opl(TXT_BANKNOTEMGR_ILLEGAL_INSTALL) /*_T("非法安装")*/;
//		}
//		m_lableGroup[LABEL_ROW_2].name1 = _T("");
//		m_lableGroup[LABEL_ROW_2].name2 = _opl(TXT_BANKNOTEMGT_QTY);
//		m_lableGroup[LABEL_ROW_2].name3 = _opl(TXT_BANKNOTEMGY_AMOUNT);
//		m_lableGroup[LABEL_ROW_2].name4 = _T("");
//		m_lableGroup[LABEL_ROW_2].name5 = _opl(TXT_BANKNOTEMGT_QTY);
//		m_lableGroup[LABEL_ROW_2].name6 = _opl(TXT_BANKNOTEMGY_AMOUNT);
//
//		// 币种  数量(无单位)  金额
//		m_lableGroup[LABEL_ROW_3].name1 = _T("10 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_3, model.banknote_box.raw_attached ? SOFTGREEN : RED);
//		m_lableGroup[LABEL_ROW_3].name1 = _T("10 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_3].name2.Format(_T(" %d ") , model.banknote_box.face10);
//		m_lableGroup[LABEL_ROW_3].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face10*1000));
//		m_lableGroup[LABEL_ROW_3].name4 = _T("20 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_3].name5.Format(_T(" %d ") , model.banknote_box.face20);
//		m_lableGroup[LABEL_ROW_3].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face20*2000));
//
//		m_lableGroup[LABEL_ROW_4].name1 = _T("50 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_4].name2.Format(_T(" %d ") , model.banknote_box.face50);
//		m_lableGroup[LABEL_ROW_4].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face50*5000));
//		m_lableGroup[LABEL_ROW_4].name4 = _T("100 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_4].name5.Format(_T(" %d ") , model.banknote_box.face100);
//		m_lableGroup[LABEL_ROW_4].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face100*10000));
//
//		m_lableGroup[LABEL_ROW_5].name1 = _T("500 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_5].name2.Format(_T(" %d ") , model.banknote_box.face500);
//		m_lableGroup[LABEL_ROW_5].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face500*50000));
//		m_lableGroup[LABEL_ROW_5].name4 = _T("1000 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_5].name5.Format(_T(" %d ") , model.banknote_box.face1000);
//		m_lableGroup[LABEL_ROW_5].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face1000*100000));
//
//		m_lableGroup[LABEL_ROW_6].name4 = _opl(TXT_BANKNOTEMGR_BOX_TOTAL_AMOUNT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_6].name6.Format( _T("%s ") + _opl(MONEY_UNIT), 
//			ComFormatAmount(100*(model.banknote_box.face10*10
//			+ model.banknote_box.face20*20
//			+ model.banknote_box.face50*50
//			+ model.banknote_box.face100*100
//			+ model.banknote_box.face500*500
//			+ model.banknote_box.face1000*1000)));
//
//		m_lableGroup[LABEL_ROW_7].name1.Format(_T("[%d]")+_opl(TXT_BANKNOTEMGR_MENU_DETACH), MENU_CODE_1);
//	}
//	else if (pSvc->GetCurDialogGroupID() == CBanknoteBoxCollectionSvc::DIALOG_GROUP_FINISH) // 卸载完成
//	{
//		// 安装之前:
//		m_lableGroup[LABEL_ROW_1].name1 = _opl(TXT_BANKNOTEMGR_BOX_BEFORE_DETACH) + _opl(TXT_COLON);
//		// 纸币箱ID
//		m_lableGroup[LABEL_ROW_2].name1.Format( _opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+_T(" %s"), model.banknote_box.raw_id);
//		// QTY  Amount 表头标题
//		m_lableGroup[LABEL_ROW_3].name1 = _T("");
//		m_lableGroup[LABEL_ROW_3].name2 = _opl(TXT_BANKNOTEMGT_QTY);
//		m_lableGroup[LABEL_ROW_3].name3 = _opl(TXT_BANKNOTEMGY_AMOUNT);
//		m_lableGroup[LABEL_ROW_3].name4 = _T("");
//		m_lableGroup[LABEL_ROW_3].name5 = _opl(TXT_BANKNOTEMGT_QTY);
//		m_lableGroup[LABEL_ROW_3].name6 = _opl(TXT_BANKNOTEMGY_AMOUNT);
//		// 币种 数量 金额
//		m_lableGroup[LABEL_ROW_4].name1 = _T("10 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_4].name2.Format(_T(" %d ") , model.banknote_box.face10);
//		m_lableGroup[LABEL_ROW_4].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face10*1000));//10Mop
//		m_lableGroup[LABEL_ROW_4].name4 = _T("20 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_4].name5.Format(_T(" %d ") , model.banknote_box.face20);
//		m_lableGroup[LABEL_ROW_4].name6.Format(_T(" %s ") + _opl(MONEY_UNIT),ComFormatAmount( model.banknote_box.face20*2000));//20Mop
//
//		m_lableGroup[LABEL_ROW_5].name1 = _T("50 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_5].name2.Format(_T(" %d ") , model.banknote_box.face50);
//		m_lableGroup[LABEL_ROW_5].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face50*5000));//50Mop
//		m_lableGroup[LABEL_ROW_5].name4 = _T("100 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_5].name5.Format(_T(" %d ") , model.banknote_box.face100);
//		m_lableGroup[LABEL_ROW_5].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face100*10000));//100Mop
//
//		m_lableGroup[LABEL_ROW_6].name1 = _T("500 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_6].name2.Format(_T(" %d "), model.banknote_box.face500);
//		m_lableGroup[LABEL_ROW_6].name3.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face500*50000));//500Mop
//		m_lableGroup[LABEL_ROW_6].name4 = _T("1000 ") + _opl(MONEY_UNIT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_6].name5.Format(_T(" %d "), model.banknote_box.face1000);
//		m_lableGroup[LABEL_ROW_6].name6.Format(_T(" %s ") + _opl(MONEY_UNIT), ComFormatAmount(model.banknote_box.face1000*100000));//1000Mop
//
//		m_lableGroup[LABEL_ROW_7].name4 = _opl(TXT_BANKNOTEMGR_BOX_TOTAL_AMOUNT) + _opl(TXT_COLON);
//		m_lableGroup[LABEL_ROW_7].name6.Format(_T("%s ") + _opl(MONEY_UNIT), 
//			ComFormatAmount(100*(model.banknote_box.face10*10
//			+ model.banknote_box.face20*20
//			+ model.banknote_box.face50*50
//			+ model.banknote_box.face100*100
//			+ model.banknote_box.face500*500
//			+ model.banknote_box.face1000*1000)));
//		// 安装之后
//		m_lableGroup[LABEL_ROW_8].name1 = _opl(TXT_BANKNOTEMGR_BOX_AFTER_DETACH);
//
//		m_lableGroup[LABEL_ROW_9].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+_T(" %s"), _T("--------"));
//	}
//	else
//	{
//		Clear();
//	}
//}
