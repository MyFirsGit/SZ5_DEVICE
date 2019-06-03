#include "stdafx.h"
#include "CoinBoxManageDetachCoinCollectionBoxDlg.h"
#include "CoinBoxManageDetachCoinCollectionBoxSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinBoxManageDetachCoinCollectionBoxDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CCoinBoxManageDetachCoinCollectionBoxDlg, COperationDlg)
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
	CCoinBoxManageDetachCoinCollectionBoxDlg::CCoinBoxManageDetachCoinCollectionBoxDlg(CService* pService)
		: COperationDlg(CCoinBoxManageDetachCoinCollectionBoxDlg::IDD,pService)
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
	CCoinBoxManageDetachCoinCollectionBoxDlg::~CCoinBoxManageDetachCoinCollectionBoxDlg()
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
	int CCoinBoxManageDetachCoinCollectionBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	void CCoinBoxManageDetachCoinCollectionBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
		theAPP_SESSION.SetMaxInputLength(1);						// 设置最大输入位数

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 100);		// 设置第一列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 70);		// 设置第二列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 120);		// 设置第三列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 70);		// 设置第四列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 120);		// 设置第五列宽度
		m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 100);		// 设置第六列宽度

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
	void CCoinBoxManageDetachCoinCollectionBoxDlg::InitDetachCoinCollectionBoxInfo()
	{
		if(LANGUAGE_ID_ENGLISH == theAPP_SESSION.GetCurrentOperatorLanguageType())
		{
			m_baseInfoArea->SetLableWidth(0,LABEL_COL_1,RESOLUTION * 160);
			m_baseInfoArea->SetLableWidth(0,LABEL_COL_2,RESOLUTION * 40);		
			m_baseInfoArea->SetLableWidth(0,LABEL_COL_3,RESOLUTION * 100);		
			m_baseInfoArea->SetLableWidth(0,LABEL_COL_4,RESOLUTION * 70);		
			m_baseInfoArea->SetLableWidth(0,LABEL_COL_5,RESOLUTION * 100);		
			m_baseInfoArea->SetLableWidth(0,LABEL_COL_6,RESOLUTION * 120);

			m_baseInfoArea->SetLableWidth(3,LABEL_COL_1,RESOLUTION * 160);
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_2,RESOLUTION * 40);		
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_3,RESOLUTION * 100);		
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_4,RESOLUTION * 70);		
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_5,RESOLUTION * 100);		
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_6,RESOLUTION * 120);
		}

		for(int i = 0; i < 15; i++)
		{
			m_lableGroup[i].name1 = _T("");
			m_lableGroup[i].name2 = _T("");
			m_lableGroup[i].name3 = _T("");
			m_lableGroup[i].name4 = _T("");
			m_lableGroup[i].name5 = _T("");
			m_lableGroup[i].name6 = _T("");
		}

		CCoinBoxManageDetachCoinCollectionBoxSvc* pSvc = (CCoinBoxManageDetachCoinCollectionBoxSvc*)GetService();
		CCoinBoxManageDetachCoinCollectionBoxSvc::Model& model = pSvc->GetDataModel();
		if(TRUE == model.IsHasException)
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_COIN_MODULE_EXCEPTION));
			theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
			//return;
		}

		m_lableGroup[0].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);							// 硬币回收箱A
		//m_lableGroup[3].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);							// 硬币回收箱B

		m_lableGroup[7].name1 = _T("[1] ")+_opl(TXT_COINMGR_DETACH_COIN_COLLECTION_BOXA);			// 卸载硬币回收箱A
		//m_lableGroup[8].name1 = _T("[2] ")+_opl(TXT_COINMGR_DETACH_COIN_COLLECTION_BOXB);			// 卸载硬币回收箱B
		//m_lableGroup[9].name1 = _T("[3] ")+_opl(TXT_COINMGR_DETACH_ALL_COIN_COLLECTION_BOX);			// 卸载所有硬币回收箱

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
		//m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID

		m_lableGroup[1].name2 = _opl(TXT_COINMGR_TYPE);					
		//m_lableGroup[4].name2 = _T("面值:");
		


		m_lableGroup[0].name4 = _opl(TXT_BANKNOTEMGR_BOX_TOTAL_AMOUNT);					// 合计
		//m_lableGroup[3].name4 =  _T("合计");				// 合计

		m_lableGroup[1].name4 = _opl(TXT_BANKNOTEMGT_QTY);					// 数量
		//m_lableGroup[4].name4 =  _T("数量");				// 数量

		
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		服务数据模型发生变化时的更新界面   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachCoinCollectionBoxDlg::UpdateUI()
	{
		CCoinBoxManageDetachCoinCollectionBoxSvc* pSvc = (CCoinBoxManageDetachCoinCollectionBoxSvc*)GetService();
		CCoinBoxManageDetachCoinCollectionBoxSvc::Model& model = pSvc->GetDataModel();
		if(1 == model.DetachCoinCollectionBoxFlowTime)
		{
			ShowDetachCoinCollectionBoxInfo();
			if(FALSE == model.IsHasException)
			{
				theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
			}
		}
		else if(2 == model.DetachCoinCollectionBoxFlowTime)
		{
			ShowDoDetachCoinCollectionBoxInfo();
			theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
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
	void CCoinBoxManageDetachCoinCollectionBoxDlg::SetColor(int row,CString status)
	{
		if(_opl(TXT_COINMGR_BOX_ATTACHED) == status)						// 安装-绿色
		{
			m_baseInfoArea->SetLabelColor(row,LABEL_COL_6,GREEN);
		}
		else																// 卸载-红色
		{
			m_baseInfoArea->SetLabelColor(row,LABEL_COL_6,RED);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		显示硬币回收箱状态   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachCoinCollectionBoxDlg::ShowDetachCoinCollectionBoxInfo()
	{
		CCoinBoxManageDetachCoinCollectionBoxSvc* pSvc = (CCoinBoxManageDetachCoinCollectionBoxSvc*)GetService();
		CCoinBoxManageDetachCoinCollectionBoxSvc::Model& model = pSvc->GetDataModel();

		// 设置颜色
		SetColor(0,model.coincollectionboxAdata.CoinCollectionBoxAStatus);
		SetColor(3,model.coincollectionboxBdata.CoinCollectionBoxBStatus);

		m_lableGroup[0].name3 = model.BoxA.countInfo.coinBoxID.ToString();				// 硬币回收箱AID
		//m_lableGroup[3].name3 = model.BoxB.countInfo.coinBoxID.ToString();				// 硬币回收箱BID

		m_lableGroup[0].name5.Format(_T("%d ") + _opl(TXT_TAKE_TICKETS_YUAN),model.BoxA.countInfo.GetCurCount());	// 硬币回收箱A合计
		//m_lableGroup[3].name5.Format(_T("%d 元"),model.BoxB.countInfo.GetCurCount());	// 硬币回收箱B合计

		m_lableGroup[0].name6 = model.BoxA.GetStatusString();			// 硬币回收箱A安装状态
		m_baseInfoArea->SetLabelColor(0,LABEL_COL_6,model.BoxA.GetStatusColor());
		//m_lableGroup[3].name6 = model.BoxB.GetStatusString();			// 硬币回收箱B安装状态
		//m_baseInfoArea->SetLabelColor(3,LABEL_COL_6,model.BoxB.GetStatusColor());

		m_lableGroup[1].name3 = _T("1 ") + _opl(TXT_TAKE_TICKETS_YUAN);		// 硬币回收箱A50AVOSData
		//m_lableGroup[4].name3 = _T("1 元");		// 硬币回收箱A2MOPData	


		m_lableGroup[1].name5 .Format(_T("%d ") + _opl(TXT_COIN_UNIT), model.BoxA.countInfo.GetCurCount());		// 硬币回收箱A1MOPData
		//m_lableGroup[4].name5 .Format(_T("%d 枚"), model.BoxB.countInfo.GetCurCount());		// 硬币回收箱A5MOPData


	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		显示硬币回收箱卸载状态   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachCoinCollectionBoxDlg::ShowDoDetachCoinCollectionBoxInfo()
	{
		if(LANGUAGE_ID_ENGLISH == theAPP_SESSION.GetCurrentOperatorLanguageType())
		{
			m_baseInfoArea->SetLableWidth(1,LABEL_COL_1,160);
			m_baseInfoArea->SetLableWidth(1,LABEL_COL_2,40);		
			m_baseInfoArea->SetLableWidth(1,LABEL_COL_3,130);		
			m_baseInfoArea->SetLableWidth(1,LABEL_COL_4,70);		
			m_baseInfoArea->SetLableWidth(1,LABEL_COL_5,120);		
			m_baseInfoArea->SetLableWidth(1,LABEL_COL_6,110);

			m_baseInfoArea->SetLableWidth(4,LABEL_COL_1,160);
			m_baseInfoArea->SetLableWidth(4,LABEL_COL_2,40);		
			m_baseInfoArea->SetLableWidth(4,LABEL_COL_3,130);		
			m_baseInfoArea->SetLableWidth(4,LABEL_COL_4,70);		
			m_baseInfoArea->SetLableWidth(4,LABEL_COL_5,120);		
			m_baseInfoArea->SetLableWidth(4,LABEL_COL_6,110);

			m_baseInfoArea->SetLableWidth(9,0,230);
			m_baseInfoArea->SetLableWidth(10,0,230);

			m_baseInfoArea->SetLableWidth(3,LABEL_COL_1,100);		// 设置第一列宽度
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_2,70);		// 设置第二列宽度
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_3,150);		// 设置第三列宽度
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_4,70);		// 设置第四列宽度
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_5,130);		// 设置第五列宽度
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_6,90);
		}
		else
		{
			m_baseInfoArea->SetLableWidth(9,0,RESOLUTION * 100);
			m_baseInfoArea->SetLableWidth(10,0,RESOLUTION * 100);
			m_baseInfoArea->SetLableWidth(1,4,RESOLUTION * 100);
			m_baseInfoArea->SetLableWidth(4,4,RESOLUTION * 100);
			m_baseInfoArea->SetLableWidth(3,4,RESOLUTION * 150);
		}

		for(int i = 0; i < 15; i++)
		{
			m_lableGroup[i].name1 = _T("");
			m_lableGroup[i].name2 = _T("");
			m_lableGroup[i].name3 = _T("");
			m_lableGroup[i].name4 = _T("");
			m_lableGroup[i].name5 = _T("");
			m_lableGroup[i].name6 = _T("");
		}

		CCoinBoxManageDetachCoinCollectionBoxSvc* pSvc = (CCoinBoxManageDetachCoinCollectionBoxSvc*)GetService();
		CCoinBoxManageDetachCoinCollectionBoxSvc::Model& model = pSvc->GetDataModel();

		// 设置颜色
		SetColor(1,model.coincollectionboxAdata.CoinCollectionBoxAStatus);
		SetColor(4,model.coincollectionboxBdata.CoinCollectionBoxBStatus);

		if(_opl(TXT_COINMGR_BOX_ATTACHED) == model.coincollectionboxAdata.CoinCollectionAfterBoxAStatus )
		{
			m_baseInfoArea->SetLabelColor(9,LABEL_COL_5,GREEN);
		}
		else
		{
			m_baseInfoArea->SetLabelColor(9,LABEL_COL_5,RED);
		}

		if(_opl(TXT_COINMGR_BOX_ATTACHED) == model.coincollectionboxBdata.CoinCollectionAfterBoxBStatus )
		{
			m_baseInfoArea->SetLabelColor(10,LABEL_COL_5,GREEN);
		}
		else
		{
			m_baseInfoArea->SetLabelColor(10,LABEL_COL_5,RED);
		}

		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_FINISH));							// 卸载完成

		//m_lableGroup[0].name1 = _opl(TXT_COINMGR_BOX_BEFORE_DETACH);								// 卸载前
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);							// 硬币回收箱A
		//m_lableGroup[4].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);							// 硬币回收箱B

		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
		//m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID

		//m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
		//m_lableGroup[10].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID

		//m_lableGroup[2].name2 = _opl(GUIDE_ADDBOX_SET_FAVO)+_opl(TXT_COLON);						// 50 AVOS
		//m_lableGroup[3].name2 = _opl(GUIDE_ADDBOX_SET_SMOP)+_opl(TXT_COLON);						// 2  MOP 
	//	m_lableGroup[5].name2 = _opl(GUIDE_ADDBOX_SET_FAVO)+_opl(TXT_COLON);						// 50 AVOS
		//m_lableGroup[6].name2 = _opl(GUIDE_ADDBOX_SET_SMOP)+_opl(TXT_COLON);						// 2  MOP 


		m_lableGroup[1].name4 = _opl(TXT_COINMGR_BOX_TOTAL_AMOUNT)+_opl(TXT_COLON);					// 合计
		//m_lableGroup[4].name4 = _opl(TXT_COINMGR_BOX_TOTAL_AMOUNT)+_opl(TXT_COLON);					// 合计

		m_lableGroup[2].name4 = _opl(GUIDE_ADDBOX_SET_OMOP)+_opl(TXT_COLON);						// 1  MOP 
		//m_lableGroup[3].name4 = _opl(GUIDE_ADDBOX_SET_FMOP)+_opl(TXT_COLON);						// 5  MOP
		//m_lableGroup[5].name4 = _opl(GUIDE_ADDBOX_SET_OMOP)+_opl(TXT_COLON);						// 1  MOP 
		//m_lableGroup[6].name4 = _opl(GUIDE_ADDBOX_SET_FMOP)+_opl(TXT_COLON);						// 5  MOP

		//m_lableGroup[8].name1 = _opl(TXT_COINMGR_BOX_AFTER_DETACH);									// 卸载后
		//m_lableGroup[9].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);							// 硬币回收箱A
		//m_lableGroup[10].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);							// 硬币回收箱B


		m_lableGroup[1].name3 = model.coincollectionboxAdata.CoinCollectionBoxAID;				// 硬币回收箱AID
		//m_lableGroup[4].name3 = model.coincollectionboxBdata.CoinCollectionBoxBID;				// 硬币回收箱BID

		m_lableGroup[1].name5 = model.coincollectionboxAdata.CoinCollectionBoxATotalData;		// 硬币回收箱A合计
		//m_lableGroup[4].name5 = model.coincollectionboxBdata.CoinCollectionBoxBTotalData;		// 硬币回收箱B合计

		m_lableGroup[1].name6 = model.coincollectionboxAdata.CoinBoxABeforeRemove;				// 硬币回收箱A安装状态
		//m_lableGroup[4].name6 = model.coincollectionboxBdata.CoinBoxBBeforeRemove;				// 硬币回收箱B安装状态

		//m_lableGroup[2].name3 = model.coincollectionboxAdata.CoinCollectionBoxA50AVOSData;		// 硬币回收箱A50AVOSData
		//m_lableGroup[3].name3 = model.coincollectionboxAdata.CoinCollectionBoxA2MOPData;		// 硬币回收箱A2MOPData	

		//m_lableGroup[5].name3 = model.coincollectionboxBdata.CoinCollectionBoxB50AVOSData;		// 硬币回收箱B50AVOSData
		//m_lableGroup[6].name3 = model.coincollectionboxBdata.CoinCollectionBoxB2MOPData;		// 硬币回收箱B2MOPData

		m_lableGroup[2].name5 = model.coincollectionboxAdata.CoinCollectionBoxA1MOPData;		// 硬币回收箱A1MOPData
		//m_lableGroup[3].name5 = model.coincollectionboxAdata.CoinCollectionBoxA5MOPData;		// 硬币回收箱A5MOPData

		//m_lableGroup[5].name5 = model.coincollectionboxBdata.CoinCollectionBoxB1MOPData;		// 硬币回收箱B1MOPData
		//m_lableGroup[6].name5 = model.coincollectionboxBdata.CoinCollectionBoxB5MOPData;		// 硬币回收箱B5MOPData

		//m_lableGroup[9].name3 = model.coincollectionboxAdata.CoinCollectionBoxAfterDetachAID;	// 硬币回收箱A卸载IDData
		//m_lableGroup[10].name3 = model.coincollectionboxBdata.CoinCollectionBoxAfterDetachBID;	// 硬币回收箱B卸载IDData

		//m_lableGroup[9].name5 = /*model.coincollectionboxAdata.CoinCollectionAfterBoxAStatus*/_T("");		// 硬币回收箱A卸载IDData
		//m_lableGroup[10].name5 = /*model.coincollectionboxBdata.CoinCollectionAfterBoxBStatus*/_T("");		// 硬币回收箱B卸载IDData
	}