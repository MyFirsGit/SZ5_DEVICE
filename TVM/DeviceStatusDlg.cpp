#include "stdafx.h"
#include "DeviceStatusDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDeviceStatusDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CDeviceStatusDlg,COperationDlg)
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
CDeviceStatusDlg::CDeviceStatusDlg(CService* pService)
	:COperationDlg(CDeviceStatusDlg::IDD,pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusDlg::~CDeviceStatusDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CDeviceStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	//设置GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_PAGE | KEY_RETURN);	// 启用翻页键

	//m_baseInfoArea->SetLableWidth(LABEL_COL_1,220);		// 设置第一列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_2,120);		// 设置第二列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_3,80);		// 设置第三列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_4,100);		// 设置第四列宽度
	//m_baseInfoArea->SetLableWidth(LABEL_COL_5,80);		// 设置第五列宽度

	//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
	//DeviceType = theMAINTENANCE_INFO.GetDeviceType();	// 获取设备类型
	//if(DEVICE_TYPE_TVM == DeviceType)
	//{
	//	InitlableDeviceStatusInfo();
	//}
	//else if(DEVICE_TYPE_AVM == DeviceType)
	//{
	//	InitlableDeviceStatusInfoforAVM();
	//}
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
void CDeviceStatusDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	theAPP_SESSION.SetKeyboard(KEY_PAGE | KEY_RETURN);	// 启用翻页键

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,180);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,120);		// 设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,125);		// 设置第三列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,100);		// 设置第四列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,115);		// 设置第五列宽度
	//m_baseInfoArea->SetLableWidth(0,LABEL_COL_1,300);

	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);


	DeviceType = theMAINTENANCE_INFO.GetDeviceType();	// 获取设备类型
	//if(DEVICE_TYPE_TVM == DeviceType)
	//{
		InitlableDeviceStatusInfo();
	//}
	/*else if(DEVICE_TYPE_AVM == DeviceType)
	{
		InitlableDeviceStatusInfoforAVM();
	}*/
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::UpdateUI()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();
	model.cashboxstatus.timesstatus = pSvc->GetCurrentPage();
    bool bIsShowNearFullStatus = theFunction_INFO.IsShowNearlyFullEmptyParaValue();
	if(Main_Page == model.cashboxstatus.timesstatus)				// 显示正常、将空、将满、已空、已满状态界面
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F3_F6_ESC));
		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DISPLAY_SET);
		for(int i=0;i<LABLE_GROUP_MAX_SIZE;i++)
		{
			m_lableGroup[i].name2 = _T("");
			m_lableGroup[i].name4 = _T("");
			m_lableGroup[i].name5 = _T("");
		}
		ShowStatus();
	}
	else if(Remain_page == model.cashboxstatus.timesstatus)			// 显示编号、剩余枚/张数界面
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F3_F5_F6_ESC));
		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DISPLAY_REMAIN_SET);
		for(int i=0;i<LABLE_GROUP_MAX_SIZE;i++)
		{
			m_baseInfoArea->SetLabelColor(i,LABEL_COL_3,BLACK);
		}
		ShowCashBoxData();
	}
	else if(Empty_Full_page == model.cashboxstatus.timesstatus)		// 显示将空、将满、已空、已满枚/张数
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_F5_ESC);
		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DISPLAY_EMPTYFULL_SET);
		for(int i=0;i<LABLE_GROUP_MAX_SIZE;i++)
		{
			m_baseInfoArea->SetLabelColor(i,LABEL_COL_3,BLACK);
		}
		ShowEmptyFullData();
	}
	m_baseInfoArea->Invalidate();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面name1的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::InitlableDeviceStatusInfo()
{
	int index = 0;
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_DISPLAY_SET);
	m_lableGroup[index++].name1 = _T("硬币循环找零箱A")/*_opl(DEVICESTATUS_CYCLE_A_COIN_SET)*/;			// 硬币循环找零箱A
	m_lableGroup[index++].name1 = _T("硬币循环找零箱B")/*_opl(DEVICESTATUS_CYCLE_B_COIN_SET)*/;			// 硬币循环找零箱B
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);		// 硬币专用找零箱A
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);		// 硬币专用找零箱B
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);		// 硬币回收箱A
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);		// 硬币回收箱B
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);	// 纸币回收箱
	m_lableGroup[index++].name1 = _opl(TXT_BANKNOTEMGR_BOX);								// 纸币找零箱
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_A_SET);							// 纸币循环鼓A
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_B_SET);							// 纸币循环鼓B
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_C_SET);							// 纸币循环鼓C
	m_lableGroup[index++].name1 = _opl(DEVICESTATUS_CIRCULAR_DRUM_D_SET);							// 纸币循环鼓D
	m_lableGroup[index++].name1	= _T("票箱A");									// 票箱A
	m_lableGroup[index++].name1	= _T("票箱B");									// 票箱B
	m_lableGroup[index++].name1	= _T("废票箱");									// 废票箱
	////m_lableGroup[index++].name1 = _opl(DEVICESTATUS_TOKENA_SET);					// Token箱A
	////m_lableGroup[index++].name1 = _opl(DEVICESTATUS_TOKENB_SET);					// Token箱B
	//m_lableGroup[index++].name1 = _opl(TH_TEST_TEXT_TOKEN_HOPPER_BOX_1);				// Token Hopper 箱A
	//m_lableGroup[index++].name1 = _opl(TH_TEST_TEXT_TOKEN_HOPPER_BOX_2);				// Token Hopper 箱B
	//m_lableGroup[index++].name1 = _opl(DEVICESTATUS_INVALID_TOKEN_SET);					// Token废票箱
	//m_lableGroup[index++].name1 = _T("Token回收箱");									// Token回收箱
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面name2、name3、name4、name5的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::ShowCashBoxData()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();

	int index = 0;
	SetColor(index++,model.cashboxdata.CycleChangeCoinAID);
	SetColor(index++,model.cashboxdata.CycleChangeCoinBID);
	//SetColor(index++,model.cashboxdata.DedicatedChangeCoinAID);
	//SetColor(index++,model.cashboxdata.DedicatedChangeCoinBID);
	SetColor(index++,model.cashboxdata.CoinCollectionAID);
	//SetColor(index++,model.cashboxdata.CoinCollectionBID);
	SetColor(index++,model.cashboxdata.BanknoteCollectionID);
 	SetColor(index++,model.cashboxdata.BanknoteChangeBoxID);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxA);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxB);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxC);
	SetColor(index++,model.cashboxdata.BNR_RecyclerBoxD);
	SetColor(index++,model.cashboxdata.CardBoxAID);
	SetColor(index++,model.cashboxdata.CardBoxBID);
	SetColor(index++,model.cashboxdata.INVALIDCardID);
	//SetColor(index++,model.cashboxdata.TokenBoxAID);
	//SetColor(index++,model.cashboxdata.TokenBoxBID);
	//SetColor(index++,model.cashboxdata.TokenHopperAID);
	//SetColor(index++,model.cashboxdata.TokenHopperBID);
	//SetColor(index++,model.cashboxdata.InvalidTokenBoxID);
	//SetColor(index++,model.cashboxdata.CollectedTokenBoxID);
	
	// 编号
	index = 0;
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);											
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);
	// 剩余数量
	index = 0;
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_REMAIN_SET)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);

	// 编号Data	
	index = 0;
	m_lableGroup[index++].name3 = model.cashboxdata.CycleChangeCoinAID;
	m_lableGroup[index++].name3 = model.cashboxdata.CycleChangeCoinBID;
	//m_lableGroup[index++].name3 = model.cashboxdata.DedicatedChangeCoinAID;
	//m_lableGroup[index++].name3 = model.cashboxdata.DedicatedChangeCoinBID;
	m_lableGroup[index++].name3 = model.cashboxdata.CoinCollectionAID;
	//m_lableGroup[index++].name3 = model.cashboxdata.CoinCollectionBID;
	m_lableGroup[index++].name3 = model.cashboxdata.BanknoteCollectionID;
	m_lableGroup[index++].name3 = model.cashboxdata.BanknoteChangeBoxID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_A_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_B_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_C_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.BNR_RECYCLER_BOX_D_ID;
	m_lableGroup[index++].name3 = model.cashboxdata.CardBoxAID;	
	m_lableGroup[index++].name3 = model.cashboxdata.CardBoxBID;
	m_lableGroup[index++].name3 = model.cashboxdata.INVALIDCardID;
	////m_lableGroup[index++].name3 = model.cashboxdata.TokenBoxAID;	
	////m_lableGroup[index++].name3 = model.cashboxdata.TokenBoxBID;
	//m_lableGroup[index++].name3 = model.cashboxdata.TokenHopperAID;	
	//m_lableGroup[index++].name3 = model.cashboxdata.TokenHopperBID;
	//m_lableGroup[index++].name3 = model.cashboxdata.InvalidTokenBoxID;
	//m_lableGroup[index++].name3 = model.cashboxdata.CollectedTokenBoxID;

	index = 0;
	// 剩余枚数/张数Data
	m_lableGroup[index++].name5 = model.cashboxdata.CycleChangeCoinAData;	
	m_lableGroup[index++].name5 = model.cashboxdata.CycleChangeCoinBData;
	//m_lableGroup[index++].name5 = model.cashboxdata.DedicatedChangeCoinAData;
	//m_lableGroup[index++].name5 = model.cashboxdata.DedicatedChangeCoinBData;
	m_lableGroup[index++].name5 = model.cashboxdata.CoinCollectionAData;
	//m_lableGroup[index++].name5 = model.cashboxdata.CoinCollectionBData;
	m_lableGroup[index++].name5 = model.cashboxdata.BanknoteCollectionData;
	m_lableGroup[index++].name5 = model.cashboxdata.BanknoteChangeData;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxA;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxB;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxC;
	m_lableGroup[index++].name5 = model.cashboxdata.BNR_RecyclerBoxD;
	m_lableGroup[index++].name5 = model.cashboxdata.CardBoxAData;
	m_lableGroup[index++].name5 = model.cashboxdata.CardBoxBData;
	m_lableGroup[index++].name5 = model.cashboxdata.InvalidCardData;
	////m_lableGroup[index++].name5 = model.cashboxdata.TokenBoxAData;	
	////m_lableGroup[index++].name5 = model.cashboxdata.TokenBoxBData;
	//m_lableGroup[index++].name5 = model.cashboxdata.TokenHopperAData;	
	//m_lableGroup[index++].name5 = model.cashboxdata.TokenHopperBData;
	//m_lableGroup[index++].name5 = model.cashboxdata.InvalidTokenData;
	//m_lableGroup[index++].name5 = model.cashboxdata.CollectedTokenData;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面name2、name3、name4、name5的将空、将满、已满数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::ShowEmptyFullData()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();

	int index = 0;
	// 将空、满
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// 将空枚数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// 将空枚数
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// 将空枚数
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// 将空枚数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);		// 将满枚数
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);		// 将满枚数

	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);		// 将满枚数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);		// 将空张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// 将满张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// 将满张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// 将满张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// 将满张数

	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// 将空张数
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// 将空张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// 将空张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_EMPTY_SET)+_opl(TXT_COLON);	// 将空张数
	m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// 将满张数
	//m_lableGroup[index++].name2 = _opl(DEVICESTATUS_BS_NEARLY_FULL_SET)+_opl(TXT_COLON);	// 将满张数

	// 已空、满
	index = 0;
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// 已空枚数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// 已空枚数
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// 已空枚数
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// 已空枚数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// 已满枚数
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// 已满枚数

	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);			// 已满枚数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_EMPTY_SET)+_opl(TXT_COLON);				// 已空张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// 已满张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// 已满张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// 已满张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_FULL_SET)+_opl(TXT_COLON);				// 已满张数

	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// 已空张数
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// 已空张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// 已空张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_EMPTY_SET)+_opl(TXT_COLON);			// 已空张数
	m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);			// 已满张数
	//m_lableGroup[index++].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);			// 已满张数

	index = 0;
	m_lableGroup[index++].name3 = model.emptyfull.CycleChangeCoinANearlyEmpty;		// 硬币循环找零箱A	将空枚数
	m_lableGroup[index++].name3 = model.emptyfull.CycleChangeCoinBNearlyEmpty;		// 硬币循环找零箱B	将空枚数
	//m_lableGroup[index++].name3 = model.emptyfull.DedicatedChangeCoinANearlyEmpty;	// 硬币专用找零箱A	将空枚数
	//m_lableGroup[index++].name3 = model.emptyfull.DedicatedChangeCoinBNearlyEmpty;	// 硬币专用找零箱B	将空枚数
	m_lableGroup[index++].name3 = model.emptyfull.CoinCollectionANearlyFull;		// 硬币专用回收箱A	将满枚数
	//m_lableGroup[index++].name3 = model.emptyfull.CoinCollectionBNearlyFull;		// 硬币专用回收箱B	将满枚数
	m_lableGroup[index++].name3 = model.emptyfull.BanknoteCollectionNearlyFulll;	// 纸币回收箱		将满张数
	m_lableGroup[index++].name3 = model.emptyfull.BanknoteChangeNearlyEmpty;		// 纸币找零箱		将空枚数
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_A_NearlyFull;	// 循环鼓A			将满张数
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_B_NearlyFull;	// 循环鼓B			将满张数
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_C_NearlyFull;	// 循环鼓C			将满张数
	m_lableGroup[index++].name3 = model.emptyfull.BNR_RECYCLER_BOX_D_NearlyFull;	// 循环鼓D			将满张数
	m_lableGroup[index++].name3 = model.emptyfull.CardMNearlyEmpty;					// Smart-card箱		将空张数
	m_lableGroup[index++].name3 = model.emptyfull.CardMNearlyEmpty;					// Smart-card箱		将空张数
	m_lableGroup[index++].name3 = model.emptyfull.INVALIDCardMNearlyFull;			// Smart-card废票箱	将满张数
	////m_lableGroup[index++].name3 = model.emptyfull.TokenANearlyEmpty;						// Token箱A			将空枚数
	////m_lableGroup[index++].name3 = model.emptyfull.TokenBNearlyEmpty;						// Token箱B			将空枚数
	//m_lableGroup[index++].name3 = model.emptyfull.TokenANearlyEmpty;						// Token Hopper箱A	将空枚数
	//m_lableGroup[index++].name3 = model.emptyfull.TokenBNearlyEmpty;						// Token Hopper箱B	将空枚数
	//m_lableGroup[index++].name3 = model.emptyfull.InvalidTokenNearlyFull;					// Token废票箱		将满枚数
	//m_lableGroup[index++].name3 = model.emptyfull.CollectedTokenNearlyFull;					// Token回收箱		将满枚数

	index = 0;
	m_lableGroup[index++].name5 = model.emptyfull.CycleChangeCoinAEmpty;		// 硬币循环找零箱	已空枚数
	m_lableGroup[index++].name5 = model.emptyfull.CycleChangeCoinBEmpty;		// 硬币循环找零箱	已空枚数
	//m_lableGroup[index++].name5 = model.emptyfull.DedicatedChangeCoinAEmpty;	// 硬币专用找零箱A	已空枚数
	//m_lableGroup[index++].name5 = model.emptyfull.DedicatedChangeCoinBEmpty;	// 硬币专用找零箱B	已空枚数
	m_lableGroup[index++].name5 = model.emptyfull.CoinCollectionAFull;			// 硬币专用回收箱A	已满枚数
	//m_lableGroup[index++].name5 = model.emptyfull.CoinCollectionBFull;			// 硬币专用回收箱B	已满枚数
	m_lableGroup[index++].name5 = model.emptyfull.BanknoteCollectionFull;		// 纸币回收箱		已满张数
	m_lableGroup[index++].name5 = model.emptyfull.BanknoteChangeEmpty;			// 纸币找零箱		已空枚数
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_A_Full;		// 循环鼓A			已满张数
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_B_Full;		// 循环鼓B			已满张数
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_C_Full;		// 循环鼓C			已满张数
	m_lableGroup[index++].name5 = model.emptyfull.BNR_RECYCLER_BOX_D_Full;		// 循环鼓D			已满张数
	m_lableGroup[index++].name5 = model.emptyfull.CardMEmpty;					// Smart-card箱		已空张数
	m_lableGroup[index++].name5 = model.emptyfull.CardMEmpty;					// Smart-card箱		已空张数
	m_lableGroup[index++].name5 = model.emptyfull.INVALIDCardMFull;				// Smart-card废票箱	已满张数
	////m_lableGroup[index++].name5 = model.emptyfull.TokenAEmpty;								// Token箱A			已空枚数
	////m_lableGroup[index++].name5 = model.emptyfull.TokenBEmpty;								// Token箱B			已空枚数
	//m_lableGroup[index++].name5 = model.emptyfull.TokenAEmpty;					// Token Hopper箱A	已空枚数
	//m_lableGroup[index++].name5 = model.emptyfull.TokenBEmpty;					// Token Hopper箱B	已空枚数
	//m_lableGroup[index++].name5 = model.emptyfull.InvalidTokenFull;				// Token废票箱		已满枚数
	//m_lableGroup[index++].name5 = model.emptyfull.CollectedTokenFull;			// Token回收箱		已满枚数
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面Status

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::ShowStatus()
{
	CDeviceStatusSvc* pSvc = (CDeviceStatusSvc*)GetService();
	CDeviceStatusSvc::Model& model = pSvc->GetDataModel();
	// 设置状态颜色
	int index = 0;
	SetColor(index++,model.cashboxstatus.CCyleChangeAStatus);
	SetColor(index++,model.cashboxstatus.CCyleChangeBStatus);
	//SetColor(index++,model.cashboxstatus.CDChangeAStatus);
	//SetColor(index++,model.cashboxstatus.CDChangeBStatus);
	SetColor(index++,model.cashboxstatus.CCollectionAStatus);
	//SetColor(index++,model.cashboxstatus.CCollectionBStatus);
	SetColor(index++,model.cashboxstatus.BanknoteStatus);
	SetColor(index++,model.cashboxstatus.BanknoteChangeStatus);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS);
	SetColor(index++,model.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS);
	SetColor(index++,model.cashboxstatus.CardBoxAStatus);
	SetColor(index++,model.cashboxstatus.CardBoxBStatus);
	SetColor(index++,model.cashboxstatus.WastedSmartcardStatus);
	////SetColor(index++,model.cashboxstatus.TokenAStatus);
	////SetColor(index++,model.cashboxstatus.TokenBStatus);
	//SetColor(index++,model.cashboxstatus.TokenHopperAStatus);
	//SetColor(index++,model.cashboxstatus.TokenHopperBStatus);
	//SetColor(index++,model.cashboxstatus.WastedTokenStatus);
	//SetColor(index++,model.cashboxstatus.CollectedTokenStatus);

	// 设置状index++态
	index = 0;
	m_lableGroup[index++].name3 = model.cashboxstatus.CCyleChangeAStatus;						
	m_lableGroup[index++].name3 = model.cashboxstatus.CCyleChangeBStatus;						
	//m_lableGroup[index++].name3 = model.cashboxstatus.CDChangeAStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.CDChangeBStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.CCollectionAStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.CCollectionBStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.BanknoteStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.BanknoteChangeStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS;
	m_lableGroup[index++].name3 = model.cashboxstatus.CardBoxAStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.CardBoxBStatus;
	m_lableGroup[index++].name3 = model.cashboxstatus.WastedSmartcardStatus;
	////m_lableGroup[index++].name3 = model.cashboxstatus.TokenAStatus;
	////m_lableGroup[index++].name3 = model.cashboxstatus.TokenBStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.TokenHopperAStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.TokenHopperBStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.WastedTokenStatus;
	//m_lableGroup[index++].name3 = model.cashboxstatus.CollectedTokenStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置状态显示的颜色

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusDlg::SetColor(int row,CString status)
{
	if(_opl(DEVICESTATUS_NORMAL_SET) == status)						// 正常-绿色
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_3,GREEN);
	}
	else if(_opl(DEVICESTATUS_NEARLY_EMPTY_TXT) == status || _opl(DEVICESTATUS_NEARLY_FULL_TXT) == status)		// 将空-黄色、将满-黄色
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_3,YELLOW);
	}
	else if(_opl(DEVICESTATUS_EMPTY_TXT) == status || _opl(DEVICESTATUS_FULL_TXT) == status || _opl(DEVICESTATUS_ILLEGAL_DETACH) == status || _opl(DEVICESTATUS_ILLEGAL_INSTALLATION) == status || _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION) == status)					// 已空-红色、已满-红色
	{
		m_baseInfoArea->SetLabelColor(row,LABEL_COL_3,RED);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面name1的数据forAVM

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CDeviceStatusDlg::InitlableDeviceStatusInfoforAVM()
//{
//	CString BanknoteCollectionID = _T("");
//	long errCode = 0;
//
//	m_baseInfoArea->SetLableWidth(LABEL_COL_2,80);													// 设置第二列宽度
//	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
//
//
//	CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
//	theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);
//
//	m_lableGroup[0].name1 = _opl(DEVICESTATUS_BANKNOTE_INFO);										// 纸币箱信息
//	m_lableGroup[1].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);								// 纸币回收箱
//	m_lableGroup[3].name1 = _opl(DEVICESTATUS_REMAIN_QTY);											// 纸币剩余张数
//	m_lableGroup[4].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);								// 纸币回收箱
//	m_lableGroup[6].name1 = _opl(DEVICESTATUS_NEARLY_FULL_OR_FULL_CASH);							// 纸币箱将满、已满张数
//	m_lableGroup[7].name1 = _opl(DEVICESTATUS_COLLECTION_BANKNOTE_SET);								// 纸币回收箱
//
//	m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);								// 纸币回收箱ID
//
//	try
//	{
//		//		CBanknoteModuleHelper* BANKNOTE_HELPER;
//		BH_STATUS bhstatus;
//
//		errCode = BANKNOTE_HELPER->GetStatus(bhstatus);
//
//		BanknoteCollectionID = theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString();		// 纸币回收箱
//		bool IsBanknoteAttached = BANKNOTE_HELPER->IsBankNoteBoxAttached();
//		if(_T("FFFFFFFF") == BanknoteCollectionID)
//		{
//			if(true == IsBanknoteAttached)
//			{
//				BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
//			}
//			else
//			{
//				BanknoteCollectionID = _T("--------");
//			}
//
//		}
//		else
//		{
//			if(false == IsBanknoteAttached)
//			{
//				BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
//			}
//		}
//		m_lableGroup[4].name3 = BanknoteCollectionID;													// 纸币回收箱IDData
//	}
//	// 异常处理
//	catch (CSysException& e) {
//		BanknoteCollectionID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
//		m_lableGroup[4].name3 = BanknoteCollectionID;
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		BanknoteCollectionID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
//		m_lableGroup[4].name3 = BanknoteCollectionID;
//		theEXCEPTION_MGR.ProcessException(CInnerException(DEVICE_STATUS_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//	m_lableGroup[4].name4 = _opl(DEVICESTATUS_BS_REMAIN)+_opl(TXT_COLON);							// 纸币回收箱剩余张数
//	int count = 0;
//	count = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_10										
//		+	theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_20
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_50
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_100
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_5
//		+ theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_1;
//	CString Banknote_Count = _T("");
//	Banknote_Count.Format(_T("%ld"),count);
//	m_lableGroup[4].name5 = Banknote_Count;															// 纸币回收箱剩余张数Data
//	m_lableGroup[7].name2 = _opl(DEVICESTATUS_NEARLY_FULL_SET)+_opl(TXT_COLON);						// 纸币回收箱将满张数
//	Banknote_Count.Format(_T("%d"),billbox_param.willFullBillCnt);
//	m_lableGroup[7].name3 =	Banknote_Count;															// 纸币回收箱将满张数Data
//	m_lableGroup[7].name4 = _opl(DEVICESTATUS_BS_FULL_SET)+_opl(TXT_COLON);							// 纸币回收箱已满张数
//	Banknote_Count.Format(_T("%d"),billbox_param.alreadyFullBillCnt);
//	m_lableGroup[7].name5 = Banknote_Count;															// 纸币回收箱已满张数Data
//
//	CString DeviceStatus = _T("");
//
//	// 纸币回收箱纸币张数正常
//	if(count < billbox_param.willFullBillCnt)
//	{
//		DeviceStatus = _opl(DEVICESTATUS_NORMAL_SET);												// 正常 
//	}
//	else if(count >= billbox_param.alreadyFullBillCnt)
//	{
//		DeviceStatus = _opl(DEVICESTATUS_FULL_TXT);													// 已满
//	}
//	else
//	{
//		DeviceStatus = _opl(DEVICESTATUS_NEARLY_FULL_TXT);											// 将满
//	}
//	m_lableGroup[1].name3 = DeviceStatus;
//	SetColor(1,DeviceStatus);
//	SetColor(4,BanknoteCollectionID);
//}
