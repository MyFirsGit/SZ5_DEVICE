#include "stdafx.h"
#include "CoinBoxManageCollectCoinDlg.h"
#include "CoinBoxManageCollectCoinSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinBoxManageCollectCoinDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CCoinBoxManageCollectCoinDlg, COperationDlg)
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
	CCoinBoxManageCollectCoinDlg::CCoinBoxManageCollectCoinDlg(CService* pService)
		: COperationDlg(CCoinBoxManageCollectCoinDlg::IDD,pService)
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
	CCoinBoxManageCollectCoinDlg::~CCoinBoxManageCollectCoinDlg()
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
	int CCoinBoxManageCollectCoinDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
			return -1;
		}
		//设置GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
		//theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数

		//m_baseInfoArea->SetLableWidth(LABEL_COL_1,200);		// 设置第一列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);		// 设置第二列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_3,150);		// 设置第三列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_4,80);		// 设置第四列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_5,100);		// 设置第五列宽度

		//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);

		//InitCollectCoinInfo();

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
	void CCoinBoxManageCollectCoinDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
		theAPP_SESSION.SetMaxInputLength(1);						// 设置最大输入位数

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 180);		// 设置第一列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 65);		// 设置第二列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 90);		// 设置第三列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 80);		// 设置第四列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 80);		// 设置第五列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 60);		// 设置第六列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_7,RESOLUTION * 90);		// 设置第七列宽度

		m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

		InitCollectCoinInfo();

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      初始化BASE区域LABEL里面的数据

	@param      void

	@retval     none

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::InitCollectCoinInfo()
	{
		CCoinBoxManageCollectCoinSvc* pSvc = (CCoinBoxManageCollectCoinSvc*)GetService();
		CCoinBoxManageCollectCoinSvc::Model& model = pSvc->GetDataModel();
		if(TRUE == model.IsHasException)
		{
			//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
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

		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// 硬币专用找零箱A
		//m_lableGroup[1].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// 硬币专用找零箱B
		////m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		//m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B
		//m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// 硬币回收

		//m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		////m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		//m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		////m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量



		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B
		m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// 硬币回收

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		服务数据模型发生变化时的更新界面   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::UpdateUI()
	{

		CCoinBoxManageCollectCoinSvc* pSvc = (CCoinBoxManageCollectCoinSvc*)GetService();
		CCoinBoxManageCollectCoinSvc::Model& model = pSvc->GetDataModel();
		if(1 == model.CollectCoinFlowTime)
		{
			ShowCollectCoinInfo();			// 显示硬币回收基本界面
		}
		else if(2 == model.CollectCoinFlowTime)
		{
			ShowCollectingCoin();			// 显示硬币回收中界面
		}
		else if(3 == model.CollectCoinFlowTime)
		{
			ShowFinishCollectCoin();		// 显示硬币回收完成界面
		}
		m_baseInfoArea->Invalidate();
		m_baseInfoArea->UpdateWindow();
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		显示硬币回收面值、ID、数量   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::ShowCollectCoinInfo()
	{
		CCoinBoxManageCollectCoinSvc* pSvc = (CCoinBoxManageCollectCoinSvc*)GetService();
		CCoinBoxManageCollectCoinSvc::Model& model = pSvc->GetDataModel();

		//m_lableGroup[0].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// 硬币专用找零箱AID
		//m_lableGroup[1].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// 硬币专用找零箱BID
		////m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		//m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		//m_lableGroup[0].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// 硬币专用找零箱A面值
		//m_lableGroup[1].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// 硬币专用找零箱B面值
		////m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱A面值
		//m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱B面值

		//m_lableGroup[0].name7 = model.boxquantity.CoinDedicatedChangeBoxAQuantity;		// 硬币专用找零箱A数量
		//m_lableGroup[1].name7 = model.boxquantity.CoinDedicatedChangeBoxBQuantity;		// 硬币专用找零箱B数量
		////m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// 硬币循环找零箱A数量
		//m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// 硬币循环找零箱B数量



		m_lableGroup[0].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		m_lableGroup[1].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		m_lableGroup[0].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱A面值
		m_lableGroup[1].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱B面值

		m_lableGroup[0].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// 硬币循环找零箱A数量
		m_lableGroup[1].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// 硬币循环找零箱B数量
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		显示硬币回收箱回收硬币时面值、ID、数量   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::ShowCollectingCoin()
	{
		CCoinBoxManageCollectCoinSvc* pSvc = (CCoinBoxManageCollectCoinSvc*)GetService();
		CCoinBoxManageCollectCoinSvc::Model& model = pSvc->GetDataModel();

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

		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_COLLECTING));				// 硬币回收中，请稍等

		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// 硬币专用找零箱A
		//m_lableGroup[1].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// 硬币专用找零箱B
		////m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		//m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B
		//m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// 硬币回收

		//m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		////m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		//m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		////m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量

		//m_lableGroup[0].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// 硬币专用找零箱AID
		//m_lableGroup[1].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// 硬币专用找零箱BID
		////m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		//m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		//m_lableGroup[0].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// 硬币专用找零箱A面值
		//m_lableGroup[1].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// 硬币专用找零箱B面值
		////m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱A面值
		//m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱B面值

		//m_lableGroup[0].name7 = model.boxquantity.CoinDedicatedChangeBoxAQuantity;		// 硬币专用找零箱A数量
		//m_lableGroup[1].name7 = model.boxquantity.CoinDedicatedChangeBoxBQuantity;		// 硬币专用找零箱B数量
		////m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// 硬币循环找零箱A数量
		//m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// 硬币循环找零箱B数量



		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B
		m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// 硬币回收

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量

		m_lableGroup[0].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		m_lableGroup[1].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		m_lableGroup[0].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱A面值
		m_lableGroup[1].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱B面值

		m_lableGroup[0].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// 硬币循环找零箱A数量
		m_lableGroup[1].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// 硬币循环找零箱B数量
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		显示硬币回收面值、ID、数量   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::ShowFinishCollectCoin()
	{
		CCoinBoxManageCollectCoinSvc* pSvc = (CCoinBoxManageCollectCoinSvc*)GetService();
		CCoinBoxManageCollectCoinSvc::Model& model = pSvc->GetDataModel();

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

		model.IsPrintFailed ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COMPLETED));					// 硬币回收完成
		//// 回收前
		//m_lableGroup[0].name1 = _opl(TXT_COINMGR_BEFORE_COLLECTION);					// 硬币回收前
		//m_lableGroup[1].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// 硬币专用找零箱A
		//m_lableGroup[2].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// 硬币专用找零箱B
		////m_lableGroup[3].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		//m_lableGroup[3].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B

		//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		////m_lableGroup[3].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[3].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		//m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		////m_lableGroup[3].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[3].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量


		//m_lableGroup[1].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// 硬币专用找零箱AID
		//m_lableGroup[2].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// 硬币专用找零箱BID
		////m_lableGroup[3].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		//m_lableGroup[3].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		//m_lableGroup[1].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// 硬币专用找零箱A面值
		//m_lableGroup[2].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// 硬币专用找零箱B面值
		////m_lableGroup[3].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱面值
		//m_lableGroup[3].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱面值

		//m_lableGroup[1].name7 = model.boxquantity.CoinDedicatedChangeBoxAQuantity;		// 硬币专用找零箱A数量
		//m_lableGroup[2].name7 = model.boxquantity.CoinDedicatedChangeBoxBQuantity;		// 硬币专用找零箱B数量
		////m_lableGroup[3].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// 硬币循环找零箱A数量
		//m_lableGroup[3].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// 硬币循环找零箱B数量

		//// 回收后
		//m_lableGroup[6].name1 = _opl(TXT_COINMGR_AFTER_COLLECTION);					// 硬币回收前
		//m_lableGroup[7].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// 硬币专用找零箱A
		//m_lableGroup[8].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// 硬币专用找零箱B
		////m_lableGroup[9].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		//m_lableGroup[9].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B

		//m_lableGroup[7].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[8].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[7].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[8].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		////m_lableGroup[9].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		//m_lableGroup[9].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);				// 面值

		//m_lableGroup[7].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[8].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		////m_lableGroup[9].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		//m_lableGroup[9].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量

		//m_lableGroup[7].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// 硬币专用找零箱AID
		//m_lableGroup[8].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// 硬币专用找零箱BID
		////m_lableGroup[9].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		//m_lableGroup[9].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID
		//

		//m_lableGroup[7].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// 硬币专用找零箱A面值
		//m_lableGroup[8].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// 硬币专用找零箱B面值
		////m_lableGroup[9].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱面值
		//m_lableGroup[9].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱面值

		//m_lableGroup[7].name7 = model.boxquantity.DeBoxAAfterColQuantity;				// 硬币专用找零箱A数量
		//m_lableGroup[8].name7 = model.boxquantity.DeBoxBAfterColQuantity;				// 硬币专用找零箱B数量
		////m_lableGroup[9].name7 = model.boxquantity.CyBoxAfterColAQuantity;				// 硬币循环找零箱数量
		//m_lableGroup[9].name7 = model.boxquantity.CyBoxAfterColBQuantity;				// 硬币循环找零箱数量


		// 回收前
		m_lableGroup[0].name1 = _opl(TXT_COINMGR_BEFORE_COLLECTION);					// 硬币回收前
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// 硬币循环找零箱B

		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量


		m_lableGroup[1].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		m_lableGroup[1].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱面值
		m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱面值

		m_lableGroup[1].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// 硬币循环找零箱A数量
		m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// 硬币循环找零箱B数量

		// 回收后
		m_lableGroup[6].name1 = _opl(TXT_COINMGR_AFTER_COLLECTION);					// 硬币回收前
		m_lableGroup[7].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// 硬币循环找零箱A
		m_lableGroup[8].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);	// 硬币循环找零箱B

		m_lableGroup[7].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[8].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[7].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值
		m_lableGroup[8].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// 面值

		m_lableGroup[7].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量
		m_lableGroup[8].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// 数量

		m_lableGroup[7].name3 = model.boxid.CoinCycleChangeBoxAID;						// 硬币循环找零箱AID
		m_lableGroup[8].name3 = model.boxid.CoinCycleChangeBoxBID;						// 硬币循环找零箱BID

		m_lableGroup[7].name5 = model.boxtype.CoinCycleChangeBoxAType;					// 硬币循环找零箱面值
		m_lableGroup[8].name5 = model.boxtype.CoinCycleChangeBoxBType;					// 硬币循环找零箱面值

		m_lableGroup[7].name7 = model.boxquantity.CyBoxAfterColAQuantity;				// 硬币循环找零箱数量
		m_lableGroup[8].name7 = model.boxquantity.CyBoxAfterColBQuantity;				// 硬币循环找零箱数量
	}