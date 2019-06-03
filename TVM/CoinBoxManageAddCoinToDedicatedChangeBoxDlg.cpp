#include "stdafx.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxDlg.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxSvc.h"
#include "AuditHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg, COperationDlg)
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
CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::CCoinBoxManageAddCoinToDedicatedChangeBoxDlg(CService* pService)
	: COperationDlg(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::IDD,pService)
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
CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::~CCoinBoxManageAddCoinToDedicatedChangeBoxDlg()
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
int CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	// 设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);					// 启用指定按键
	//theAPP_SESSION.SetMaxInputLength(1);											// 设置最大输入位数



	//InitDedicatedChangeBoxInfo();

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
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);						// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);											// 设置最大输入位数

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 190);									// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 50);									// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 85);									// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 50);									// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 75);									// 设置第五列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 90);									// 设置第六列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,RESOLUTION * 80);									// 设置第七列宽度

	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

	InitDedicatedChangeBoxInfo();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::InitDedicatedChangeBoxInfo()
{
	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
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
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}

	m_baseInfoArea->SetLableWidth(6,LABEL_COL_1,RESOLUTION * 300);
	m_baseInfoArea->SetLableWidth(7,LABEL_COL_1,RESOLUTION * 300);

	m_lableGroup[0].name1 = _T("硬币循环找零箱A")/*_opl(DEVICESTATUS_DEDICATED_COINA_SET)*/;					// 硬币专用找零箱A
	//m_lableGroup[1].name1 = _opl(TXT_COINMGR_ADDCOIN_BOX);							// 硬币补充箱
	m_lableGroup[3].name1 = _T("硬币循环找零箱B")/*_opl(DEVICESTATUS_DEDICATED_COINB_SET)*/;					// 硬币专用找零箱B
	//m_lableGroup[4].name1 = _opl(TXT_COINMGR_ADDCOIN_BOX);							// 硬币补充箱
	m_lableGroup[6].name1 = _T("[1] ")+_opl(TXT_COINMGR_ADDCOIN_MENU_BOXA);			// 输入硬币箱A的数量
	m_lableGroup[7].name1 = _T("[2] ")+_opl(TXT_COINMGR_ADDCOIN_MENU_BOXB);			// 输入硬币箱B的数量

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	//m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

	m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
	m_lableGroup[3].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

	m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
	m_lableGroup[3].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量

	m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// 添加
	m_lableGroup[4].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// 添加
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::UpdateUI()
{
	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	if(1 == model.addingboxdata.AddingBoxFrameFlowTime)
	{
		ShowDedicatedChangeBoxData();
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);
		if (pSvc->IsShowTips())	// 显示加币提示
		{
			ShowTips();
			theAPP_SESSION.SetKeyboard( KEY_F2 | KEY_RETURN);
		}
	}
	/*else if(2 == model.addingboxdata.AddingBoxFrameFlowTime)
	{
		ShowAddingCoinToDedicatedChangeBox();
		theAPP_SESSION.DiableKeyboard(KEY_NUMBER | KEY_CLEAR | KEY_ESC | KEY_F2);
	}*/
	else if(3 == model.addingboxdata.AddingBoxFrameFlowTime)
	{
		ShowFinishAddingCoinToDedicatedChangeBox();
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	}

	
	m_baseInfoArea->Invalidate();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      显示专用找零箱初始化时的ID、数量

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowDedicatedChangeBoxData()
{
	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxAinfo = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_A];
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxBinfo = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_B];

	m_lableGroup[0].name3 = boxAinfo.strID; /*model.dedicatedchangeboxdata.DedicatedChangeCoinAID;*/	// 硬币专用找零箱AIDData
	//m_lableGroup[1].name3 = model.addingboxdata.AddingBoxAID;						// 硬币补充箱AIDData
	m_lableGroup[3].name3 = boxBinfo.strID;/*model.dedicatedchangeboxdata.DedicatedChangeCoinBID;*/	// 硬币专用找零箱BIDData
	//m_lableGroup[4].name3 = model.addingboxdata.AddingBoxBID;						// 硬币补充箱BIDData

	
	m_lableGroup[0].name5 = CashValueToString(boxAinfo.value);						// 硬币补充箱A 面值 
	m_lableGroup[3].name5 = CashValueToString(boxBinfo.value);						// 硬币补充箱B 面值 

	m_lableGroup[0].name7.Format(_T("%d"),boxAinfo.CurrentCount);					// 硬币专用找零箱A 数量 
	m_lableGroup[3].name7.Format(_T("%d"),boxBinfo.CurrentCount);					// 硬币专用找零箱B 数量 

	m_lableGroup[1].name7.Format(_T("%d"),boxAinfo.AddingCount);					// 硬币补充箱A 加币数量
	m_lableGroup[4].name7.Format(_T("%d"),boxBinfo.AddingCount);					// 硬币补充箱B 加币数量
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示专用找零箱加币中界面

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowAddingCoinToDedicatedChangeBox()
{
	m_baseInfoArea->SetLableWidth(LABEL_COL_1,640);									// 设置第一列宽度
	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDCOIN_DOING));
	m_lableGroup[0].name1 = _opl(TXT_COINMGR_ADDCOIN_1STEP);
	m_lableGroup[1].name1 = _opl(TXT_COINMGR_ADDCOIN_2STEP);
	m_lableGroup[2].name1 = _opl(TXT_COINMGR_ADDCOIN_3STEP);
	m_lableGroup[3].name1 = _opl(TXT_COINMGR_ADDCOIN_4STEP);

	m_lableGroup[4].name1 = _T("");
	m_lableGroup[6].name1 = _T("");						
	m_lableGroup[7].name1 = _T("");		
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示专用找零箱加币完成界面

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowFinishAddingCoinToDedicatedChangeBox()
{
	m_baseInfoArea->SetLableWidth(LABEL_COL_1,180);									// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);									// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,90);									// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,50);									// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,100);									// 设置第五列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,90);									// 设置第六列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,80);									// 设置第七列宽度

	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

	CCoinChangeBoxAddingSvc* pSvc = (CCoinChangeBoxAddingSvc*)GetService();
	CCoinChangeBoxAddingSvc::Model& model = pSvc->GetDataModel();
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxA = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_A];
	CCoinChangeBoxAddingSvc::CHANGE_BOX_INFO boxB = model.mapBoxInfo[CCoinChangeBoxAddingSvc::CHANGE_BOX_B];

	for ( int i = 0; i < 15; i++ ){
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}
	

	//model.IsPrintFailed ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDCOIN_FINISH));

	// name1
	m_lableGroup[0].name1 = _opl(TXT_COINMGR_ADDING_BEFORE_ADDING);					// 加币前
	m_lableGroup[1].name1 = _T("硬币循环找零箱A")/*_opl(DEVICESTATUS_DEDICATED_COINA_SET)*/;					// 硬币专用找零箱A
	m_lableGroup[4].name1 = _T("硬币循环找零箱B")/*_opl(DEVICESTATUS_DEDICATED_COINB_SET)*/;					// 硬币专用找零箱B

	 
	m_lableGroup[7].name1 = _opl(TXT_COINMGR_ADDING_AFTER_ADDING);					// 加币后
	m_lableGroup[8].name1 = _T("硬币循环找零箱A")/*_opl(DEVICESTATUS_DEDICATED_COINA_SET)*/;					// 硬币专用找零箱A
	m_lableGroup[9].name1 = _T("硬币循环找零箱B")/*_opl(DEVICESTATUS_DEDICATED_COINB_SET)*/;					// 硬币专用找零箱B

	// name2 - 编号
	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[8].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
	m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

	// name4 - 面值
	m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
	m_lableGroup[4].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
	m_lableGroup[8].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					  
	m_lableGroup[9].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					

	// name6 - 硬币专用找零箱A、B数量
	m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
	m_lableGroup[4].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
	m_lableGroup[8].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
	m_lableGroup[9].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量

	// name6 - 硬币补充箱加币数量
	m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// 添加
	m_lableGroup[5].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);	// 添加


	// name3-ID Data
	m_lableGroup[1].name3 = boxA.strID;	// 硬币专用找零箱AID
	m_lableGroup[4].name3 = boxB.strID;	// 硬币专用找零箱BID

	m_lableGroup[8].name3 = boxA.strID;	// 硬币专用找零箱AID
	m_lableGroup[9].name3 = boxB.strID;	// 硬币专用找零箱BID

	m_lableGroup[1].name5 = CashValueToString(boxA.value);					// 硬币补充箱A面值
	m_lableGroup[4].name5 = CashValueToString(boxB.value);					// 硬币补充箱B面值

	m_lableGroup[1].name7.Format(_T("%d"),boxA.ExCount);					// 硬币专用找零箱A数量
	m_lableGroup[4].name7.Format(_T("%d"),boxB.ExCount);					// 硬币专用找零箱B数量

	m_lableGroup[2].name7.Format(_T("%d"),boxA.AddingCount);
	m_lableGroup[5].name7.Format(_T("%d"),boxB.AddingCount);

	// 加币后硬币专用找零箱A、B的面值和数量
	m_lableGroup[8].name5 = CashValueToString(boxA.value);					// 硬币专用找零箱A面值
	m_lableGroup[9].name5 = CashValueToString(boxB.value);					// 硬币专用找零箱B面值

	int DQuantity = _ttoi(model.dedicatedchangeboxdata.QuantityA);
	int AQuantity = _ttoi(model.addingboxdata.AddingBoxQuantityA);
	CString AfterAddingQuantity = _T("");
	AfterAddingQuantity.Format(_T("%d"),DQuantity+AQuantity);

	m_lableGroup[8].name7.Format(_T("%d"),boxA.CurrentCount);				// 硬币专用找零箱A数量
	m_lableGroup[9].name7.Format(_T("%d"),boxB.CurrentCount);				// 硬币专用找零箱B数量
}

CString CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::CashValueToString(BankNoteAndCoinType_t type)
{
	CString strValue;
	switch(type)
	{
	case Coin1Yuan: strValue = _T("  1 元");break;
	case CoinHalfYuan :strValue = _T("  5 角");break;
	case Banknote1Yuan:strValue = _T("  1 元");break;
	case Banknote5Yuan:strValue = _T("  5 元");break;
	case Banknote10Yuan:strValue = _T("  50MOP");break;
	case Banknote20Yuan:strValue = _T(" 100MOP");break;
	case Banknote50Yuan:strValue = _T(" 500MOP");break;
	case Banknote100Yuan:strValue = _T("1000MOP");break;
	default: break;
	}
	return strValue;
}

void CCoinBoxManageAddCoinToDedicatedChangeBoxDlg::ShowTips()
{
	for ( int i = 0; i < 15; i++ ){
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,640);									// 设置第一列宽度
	theAPP_SESSION.ShowOfficeGuide(_T("加币进行中，请按照以下指示操作。"));
	//m_lableGroup[0].name1 = _T("第一步：打开硬币找零箱锁扣，拉出硬币找零箱。");
	//m_lableGroup[1].name1 = _T("第二步：打开硬币找零箱盖锁扣，取下硬币找零箱盖。");
	//m_lableGroup[2].name1 = _T("第三步：添加硬币到硬币找零箱。");
	//m_lableGroup[3].name1 = _T("第四步：装上硬币找零箱盖，并关闭找零箱盖锁扣。");
	//m_lableGroup[4].name1 = _T("第五步：推入硬币找零箱，关闭硬币找零箱锁扣。");
	//m_lableGroup[5].name1 = _T("第六步：按[F2/Alt]完成加币操作。");
	m_lableGroup[0].name1 = _T("第一步：装上硬币找零箱，打开硬币找零箱锁扣。");
	m_lableGroup[1].name1 = _T("第二步：打开硬币储币箱盖锁扣，拉出硬币找零箱挡板。");
	m_lableGroup[2].name1 = _T("第三步：推入硬币找零箱挡板，关闭硬币储币箱锁扣。");
	m_lableGroup[3].name1 = _T("第四步：关闭硬币找零箱锁扣，取下硬币找零箱。");
	m_lableGroup[4].name1 = _T("第五步：按[F2/Alt]完成加币操作。");
}