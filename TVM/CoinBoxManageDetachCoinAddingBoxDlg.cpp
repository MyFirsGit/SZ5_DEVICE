#include "stdafx.h"
#include "CoinBoxManageDetachCoinAddingBoxDlg.h"
#include "CoinBoxManageDetachCoinAddingBoxSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinBoxManageDetachAddingBoxDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CCoinBoxManageDetachAddingBoxDlg, COperationDlg)
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
	CCoinBoxManageDetachAddingBoxDlg::CCoinBoxManageDetachAddingBoxDlg(CService* pService)
		: COperationDlg(CCoinBoxManageDetachAddingBoxDlg::IDD,pService)
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
	CCoinBoxManageDetachAddingBoxDlg::~CCoinBoxManageDetachAddingBoxDlg()
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
	int CCoinBoxManageDetachAddingBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
			return -1;
		}
		//设置GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
		//theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 210);		// 设置第一列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 100);		// 设置第二列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 100);		// 设置第三列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 200);		// 设置第四列宽度

		m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);

		//InitDetachCoinAddingBoxInfo();
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
	void CCoinBoxManageDetachAddingBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
		theAPP_SESSION.SetMaxInputLength(1);						// 设置最大输入位数

		InitDetachCoinAddingBoxInfo();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      初始化BASE区域LABEL里面的数据

	@param      void

	@retval     none

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::InitDetachCoinAddingBoxInfo()
	{
		CCoinBoxManageDetachCoinAddingBoxSvc* pSvc = (CCoinBoxManageDetachCoinAddingBoxSvc*)GetService();
		CCoinBoxManageDetachCoinAddingBoxSvc::Model& model = pSvc->GetDataModel();
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
		}

		m_lableGroup[0].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXA);								// 硬币补充箱A
		m_lableGroup[1].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXB);								// 硬币补充箱B

		m_lableGroup[3].name1 = _T("[1] ")+_opl(TXT_COINMGR_DETACH_COIN_ADDING_BOXA);			// 卸载硬币补充箱A
		m_lableGroup[4].name1 = _T("[2] ")+_opl(TXT_COINMGR_DETACH_COIN_ADDING_BOXB);			// 卸载硬币补充箱B
		m_lableGroup[5].name1 = _T("[3] ")+_opl(TXT_COINMGR_DETACH_ALL_COIN_ADDING_BOX);			// 卸载所有硬币补充箱

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		服务数据模型发生变化时的更新界面   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::UpdateUI()
	{
		CCoinBoxManageDetachCoinAddingBoxSvc* pSvc = (CCoinBoxManageDetachCoinAddingBoxSvc*)GetService();
		CCoinBoxManageDetachCoinAddingBoxSvc::Model& model = pSvc->GetDataModel();
		if(1 == model.DetachCoinAddingBoxFlowTime)
		{
			ShowDetachCoinAddingBoxInfo();
			theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
		}
		else if(2 == model.DetachCoinAddingBoxFlowTime)
		{
			ShowDoDetachCoinAddingBoxInfo();
			theAPP_SESSION.DiableKeyboard(KEY_NUMBER | KEY_ENTER);
		}
		m_baseInfoArea->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置状态显示的颜色

	@param      void

	@retval     none

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::SetColor(int row,CString status)
	{
		if(_opl(TXT_COINMGR_BOX_ATTACHED) == status )						// 安装-绿色
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
	@brief		显示硬币补充箱状态   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::ShowDetachCoinAddingBoxInfo()
	{
		CCoinBoxManageDetachCoinAddingBoxSvc* pSvc = (CCoinBoxManageDetachCoinAddingBoxSvc*)GetService();
		CCoinBoxManageDetachCoinAddingBoxSvc::Model& model = pSvc->GetDataModel();

		// 设置颜色
		SetColor(0,model.CoinAddingBoxAStatus);
		SetColor(1,model.CoinAddingBoxBStatus);

		m_lableGroup[0].name3 = model.CoinAddingBoxAID;							// 硬币补充箱AIDData
		m_lableGroup[1].name3 = model.CoinAddingBoxBID;							// 硬币补充箱BIDData

		m_lableGroup[0].name4 = model.CoinAddingBoxAStatus;						// 硬币补充箱A状态Data
		m_lableGroup[1].name4 = model.CoinAddingBoxBStatus;						// 硬币补充箱B状态Data
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		显示硬币补充箱卸载状态   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::ShowDoDetachCoinAddingBoxInfo()
	{
		CCoinBoxManageDetachCoinAddingBoxSvc* pSvc = (CCoinBoxManageDetachCoinAddingBoxSvc*)GetService();
		CCoinBoxManageDetachCoinAddingBoxSvc::Model& model = pSvc->GetDataModel();

		// 设置颜色
		SetColor(1,model.CoinAddingBoxAStatus);
		SetColor(2,model.CoinAddingBoxBStatus);
		SetColor(5,model.CoinAddingBoxAfterDetachStatus);
		SetColor(6,model.CoinAddingBoxBfterDetachStatus);

		m_lableGroup[0].name2 = _T("");
		m_lableGroup[0].name3 = _T("");
		m_lableGroup[0].name4 = _T("");
		m_lableGroup[3].name1 = _T("");															// 清空

		model.IsPrintFailed ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_FINISH));						// 卸载完成
		m_lableGroup[0].name1 = _opl(TXT_COINMGR_BOX_BEFORE_DETACH);							// 卸载前
		m_lableGroup[1].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXA);								// 硬币补充箱A
		m_lableGroup[2].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXB);								// 硬币补充箱B

		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID


		m_lableGroup[4].name1 = _opl(TXT_COINMGR_BOX_AFTER_DETACH);								// 卸载后
		m_lableGroup[5].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXA);								// 硬币补充箱A
		m_lableGroup[6].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXB);								// 硬币补充箱B

		m_lableGroup[5].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[6].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

		m_lableGroup[1].name3 = model.CoinAddingBoxAID;											// 硬币补充箱AIDData
		m_lableGroup[2].name3 = model.CoinAddingBoxBID;											// 硬币补充箱BIDData

		m_lableGroup[1].name4 = model.CoinBoxABeforeAddingStatus;										// 硬币补充箱A卸载Data
		m_lableGroup[2].name4 = model.CoinBoxBBeforeAddingStatus;										// 硬币补充箱B卸载Data

		m_lableGroup[5].name3 = model.CoinAddingBoxAfterDetachAID;								// 硬币补充箱A卸载后IDData
		m_lableGroup[6].name3 = model.CoinAddingBoxAfterDetachBID;								// 硬币补充箱B卸载后IDData

		m_lableGroup[5].name4 = /*model.CoinAddingBoxAfterDetachStatus*/_T("");					// 硬币补充箱A卸载后状态
		m_lableGroup[6].name4 = /*model.CoinAddingBoxBfterDetachStatus*/_T("");					// 硬币补充箱B卸载后状态
	}