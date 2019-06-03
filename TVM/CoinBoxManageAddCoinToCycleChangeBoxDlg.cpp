#include "stdafx.h"
#include "CoinBoxManageAddCoinToCycleChangeBoxDlg.h"
#include "CoinBoxManageAddCoinToCycleChangeBoxSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinBoxManageAddCoinToCycleChangeBoxDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CCoinBoxManageAddCoinToCycleChangeBoxDlg, COperationDlg)
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
	CCoinBoxManageAddCoinToCycleChangeBoxDlg::CCoinBoxManageAddCoinToCycleChangeBoxDlg(CService* pService)
		: COperationDlg(CCoinBoxManageAddCoinToCycleChangeBoxDlg::IDD,pService)
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
	CCoinBoxManageAddCoinToCycleChangeBoxDlg::~CCoinBoxManageAddCoinToCycleChangeBoxDlg()
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
	int CCoinBoxManageAddCoinToCycleChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA)) {
			return -1;
		}
		//设置GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);	// 启用指定按键
		//theAPP_SESSION.SetMaxInputLength(1);				// 设置最大输入位数	

		//m_baseInfoArea->SetLableWidth(LABEL_COL_1,180);		// 设置第一列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);		// 设置第二列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_3,70);		// 设置第三列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_4,50);		// 设置第四列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_5,80);		// 设置第五列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_6,50);		// 设置第六列宽度
		//m_baseInfoArea->SetLableWidth(LABEL_COL_7,100);		// 设置第七列宽度

		//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

		//InitCycleChangeBoxInfo();
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
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState, pWndOther, bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);	// 启用指定按键
		theAPP_SESSION.SetMaxInputLength(1);				// 设置最大输入位数	

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,170);		// 设置第一列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);		// 设置第二列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,80);		// 设置第三列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,50);		// 设置第四列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_5,70);		// 设置第五列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_6,60);		// 设置第六列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_7,100);		// 设置第七列宽度

		m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_6, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

		InitCycleChangeBoxInfo();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      初始化BASE区域LABEL里面的数据

	@param      void

	@retval     none

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::InitCycleChangeBoxInfo()
	{
		CCoinBoxManageAddCoinToCycleChangeBoxSvc* pSvc = (CCoinBoxManageAddCoinToCycleChangeBoxSvc*)GetService();
		CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& model = pSvc->GetDataModel();
		if(TRUE == model.IsHasException)
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_COIN_MODULE_EXCEPTION));
			theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
			return;
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

		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// 硬币循环找零箱
		m_lableGroup[2].name1 = _T("[1]")+_opl(COIN_TEST_TEXT_START_COIN_RECEIVING);			// 开始接收硬币

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// 面值

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// 数量
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		服务数据模型发生变化时的更新界面   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::UpdateUI()
	{
		CCoinBoxManageAddCoinToCycleChangeBoxSvc* pSvc = (CCoinBoxManageAddCoinToCycleChangeBoxSvc*)GetService();
		CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& model = pSvc->GetDataModel();

		if(1 == model.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime)
		{
			ShowCycleChangeBoxData();
			theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
		}
		else if(2 == model.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime)
		{
			ShowCycleChangeBoxAddingCoin();
			theAPP_SESSION.DiableKeyboard(KEY_CLEAR | KEY_ESC | KEY_F2);
		}
		else if(3 == model.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime)
		{
			ShowFinishCycleChangeBoxAddingCoin();
			theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
		}
		m_baseInfoArea->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示硬币循环找零箱ID、数量

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::ShowCycleChangeBoxData()
	{
		CCoinBoxManageAddCoinToCycleChangeBoxSvc* pSvc = (CCoinBoxManageAddCoinToCycleChangeBoxSvc*)GetService();
		CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& model = pSvc->GetDataModel();

		m_lableGroup[0].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// 硬币循环找零箱ID
		m_lableGroup[0].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// 硬币循环找零箱面值
		m_lableGroup[0].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxQuantity;			// 硬币循环找零箱数量
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示硬币循环找零箱加币中界面

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::ShowCycleChangeBoxAddingCoin()
	{
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_ADDING_COIN));

		CCoinBoxManageAddCoinToCycleChangeBoxSvc* pSvc = (CCoinBoxManageAddCoinToCycleChangeBoxSvc*)GetService();
		CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& model = pSvc->GetDataModel();

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

		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// 硬币循环找零箱
		m_lableGroup[3].name1 = _T("[1]")+_opl(TXT_COINMGR_CYCLE_CAHNGE_BOX_STOP_ADDING);		// 停止加币
		//m_lableGroup[4].name1 = _T("[2]")+_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_CANCLE_ADDINF);		// 取消加币

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// 面值

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// 数量
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);			// 添加

		m_lableGroup[0].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// 硬币循环找零箱ID
		m_lableGroup[0].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// 硬币循环找零箱面值
		m_lableGroup[0].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxQuantity;			// 硬币循环找零箱数量

		// 加币中
		m_lableGroup[1].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity;		// 硬币循环找零箱加币数量

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示硬币循环找零箱加币完成界面

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::ShowFinishCycleChangeBoxAddingCoin()
	{
		CCoinBoxManageAddCoinToCycleChangeBoxSvc* pSvc = (CCoinBoxManageAddCoinToCycleChangeBoxSvc*)GetService();
		CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& model = pSvc->GetDataModel();

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

		if(FALSE == model.IsPrinterSuccessfull)
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		}
		else
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDCOIN_FINISH));
		}

		m_lableGroup[0].name1 = _opl(TXT_COINMGR_ADDING_BEFORE_ADDING);							// 加币前
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// 硬币循环找零箱
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// 面值
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// 数量
		m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);			// 添加

		m_lableGroup[1].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// 硬币循环找零箱ID
		m_lableGroup[1].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// 硬币循环找零箱面值
		CString strTemp;
		strTemp.Format(_T("%d"),model.cyclechangeboxdata.nBeforeAddingStockCount);
		m_lableGroup[1].name7 = strTemp;			// 硬币循环找零箱数量
		m_lableGroup[2].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity;		// 硬币循环找零箱加币数量

		m_lableGroup[4].name1 = _opl(TXT_COINMGR_ADDING_AFTER_ADDING);							// 加币后
		m_lableGroup[5].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// 硬币循环找零箱
		m_lableGroup[5].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[5].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// 面值
		m_lableGroup[5].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// 数量

		m_lableGroup[5].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// 硬币循环找零箱ID
		m_lableGroup[5].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// 硬币循环找零箱面值
		m_lableGroup[5].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxAfterAddingQuantity;	// 硬币循环找零箱数量
	}


