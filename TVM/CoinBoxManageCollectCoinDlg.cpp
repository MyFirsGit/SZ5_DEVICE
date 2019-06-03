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
	@brief      ���캯��

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
	@brief      ��������

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
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:�����ɹ�    -1:����ʧ��

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CCoinBoxManageCollectCoinDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
			return -1;
		}
		//����GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
		//theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��

		//m_baseInfoArea->SetLableWidth(LABEL_COL_1,200);		// ���õ�һ�п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);		// ���õڶ��п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_3,150);		// ���õ����п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_4,80);		// ���õ����п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_5,100);		// ���õ����п��

		//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_RIGHT | DT_VCENTER);
		//m_baseInfoArea->SetLableAlign(LABEL_COL_5, DT_LEFT | DT_VCENTER);

		//InitCollectCoinInfo();

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���漤���Ĵ���

	@param      (i)UINT nState      ��ʹ��

	@param      (i)CWnd* pWndOther  ��ʹ��

	@param      (i)BOOL bMinimized  ��ʹ��

	@retval     void

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
		theAPP_SESSION.SetMaxInputLength(1);						// �����������λ��

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 180);		// ���õ�һ�п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 65);		// ���õڶ��п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 90);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 80);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 80);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 60);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_7,RESOLUTION * 90);		// ���õ����п��

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
	@brief      ��ʼ��BASE����LABEL���������

	@param      void

	@retval     none

	@exception  ��
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

		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// Ӳ��ר��������A
		//m_lableGroup[1].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// Ӳ��ר��������B
		////m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		//m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B
		//m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// Ӳ�һ���

		//m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		////m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		//m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		////m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����



		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B
		m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// Ӳ�һ���

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
			ShowCollectCoinInfo();			// ��ʾӲ�һ��ջ�������
		}
		else if(2 == model.CollectCoinFlowTime)
		{
			ShowCollectingCoin();			// ��ʾӲ�һ����н���
		}
		else if(3 == model.CollectCoinFlowTime)
		{
			ShowFinishCollectCoin();		// ��ʾӲ�һ�����ɽ���
		}
		m_baseInfoArea->Invalidate();
		m_baseInfoArea->UpdateWindow();
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�һ�����ֵ��ID������   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageCollectCoinDlg::ShowCollectCoinInfo()
	{
		CCoinBoxManageCollectCoinSvc* pSvc = (CCoinBoxManageCollectCoinSvc*)GetService();
		CCoinBoxManageCollectCoinSvc::Model& model = pSvc->GetDataModel();

		//m_lableGroup[0].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// Ӳ��ר��������AID
		//m_lableGroup[1].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// Ӳ��ר��������BID
		////m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		//m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		//m_lableGroup[0].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// Ӳ��ר��������A��ֵ
		//m_lableGroup[1].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// Ӳ��ר��������B��ֵ
		////m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ��������A��ֵ
		//m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ��������B��ֵ

		//m_lableGroup[0].name7 = model.boxquantity.CoinDedicatedChangeBoxAQuantity;		// Ӳ��ר��������A����
		//m_lableGroup[1].name7 = model.boxquantity.CoinDedicatedChangeBoxBQuantity;		// Ӳ��ר��������B����
		////m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// Ӳ��ѭ��������A����
		//m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// Ӳ��ѭ��������B����



		m_lableGroup[0].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		m_lableGroup[1].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		m_lableGroup[0].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ��������A��ֵ
		m_lableGroup[1].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ��������B��ֵ

		m_lableGroup[0].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// Ӳ��ѭ��������A����
		m_lableGroup[1].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// Ӳ��ѭ��������B����
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�һ��������Ӳ��ʱ��ֵ��ID������   

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

		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_COLLECTING));				// Ӳ�һ����У����Ե�

		//m_lableGroup[0].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// Ӳ��ר��������A
		//m_lableGroup[1].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// Ӳ��ר��������B
		////m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		//m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B
		//m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// Ӳ�һ���

		//m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		////m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		//m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		////m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����

		//m_lableGroup[0].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// Ӳ��ר��������AID
		//m_lableGroup[1].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// Ӳ��ר��������BID
		////m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		//m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		//m_lableGroup[0].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// Ӳ��ר��������A��ֵ
		//m_lableGroup[1].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// Ӳ��ר��������B��ֵ
		////m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ��������A��ֵ
		//m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ��������B��ֵ

		//m_lableGroup[0].name7 = model.boxquantity.CoinDedicatedChangeBoxAQuantity;		// Ӳ��ר��������A����
		//m_lableGroup[1].name7 = model.boxquantity.CoinDedicatedChangeBoxBQuantity;		// Ӳ��ר��������B����
		////m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// Ӳ��ѭ��������A����
		//m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// Ӳ��ѭ��������B����



		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B
		m_lableGroup[5].name1 = _T("[1] ")+_opl(TXT_COINMGR_COLLECTION_COIN);			// Ӳ�һ���

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����

		m_lableGroup[0].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		m_lableGroup[1].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		m_lableGroup[0].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ��������A��ֵ
		m_lableGroup[1].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ��������B��ֵ

		m_lableGroup[0].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// Ӳ��ѭ��������A����
		m_lableGroup[1].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// Ӳ��ѭ��������B����
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�һ�����ֵ��ID������   

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

		model.IsPrintFailed ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COMPLETED));					// Ӳ�һ������
		//// ����ǰ
		//m_lableGroup[0].name1 = _opl(TXT_COINMGR_BEFORE_COLLECTION);					// Ӳ�һ���ǰ
		//m_lableGroup[1].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// Ӳ��ר��������A
		//m_lableGroup[2].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// Ӳ��ר��������B
		////m_lableGroup[3].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		//m_lableGroup[3].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B

		//m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		////m_lableGroup[3].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[3].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		//m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		////m_lableGroup[3].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[3].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����


		//m_lableGroup[1].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// Ӳ��ר��������AID
		//m_lableGroup[2].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// Ӳ��ר��������BID
		////m_lableGroup[3].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		//m_lableGroup[3].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		//m_lableGroup[1].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// Ӳ��ר��������A��ֵ
		//m_lableGroup[2].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// Ӳ��ר��������B��ֵ
		////m_lableGroup[3].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ����������ֵ
		//m_lableGroup[3].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ����������ֵ

		//m_lableGroup[1].name7 = model.boxquantity.CoinDedicatedChangeBoxAQuantity;		// Ӳ��ר��������A����
		//m_lableGroup[2].name7 = model.boxquantity.CoinDedicatedChangeBoxBQuantity;		// Ӳ��ר��������B����
		////m_lableGroup[3].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// Ӳ��ѭ��������A����
		//m_lableGroup[3].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// Ӳ��ѭ��������B����

		//// ���պ�
		//m_lableGroup[6].name1 = _opl(TXT_COINMGR_AFTER_COLLECTION);					// Ӳ�һ���ǰ
		//m_lableGroup[7].name1 = _opl(DEVICESTATUS_DEDICATED_COINA_SET);					// Ӳ��ר��������A
		//m_lableGroup[8].name1 = _opl(DEVICESTATUS_DEDICATED_COINB_SET);					// Ӳ��ר��������B
		////m_lableGroup[9].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		//m_lableGroup[9].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B

		//m_lableGroup[7].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[8].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		////m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		//m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		//m_lableGroup[7].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[8].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		////m_lableGroup[9].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		//m_lableGroup[9].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);				// ��ֵ

		//m_lableGroup[7].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[8].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		////m_lableGroup[9].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		//m_lableGroup[9].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����

		//m_lableGroup[7].name3 = model.boxid.CoinDedicatedChangeBoxAID;					// Ӳ��ר��������AID
		//m_lableGroup[8].name3 = model.boxid.CoinDedicatedChangeBoxBID;					// Ӳ��ר��������BID
		////m_lableGroup[9].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		//m_lableGroup[9].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID
		//

		//m_lableGroup[7].name5 = model.boxtype.CoinDedicatedChangeBoxAType;				// Ӳ��ר��������A��ֵ
		//m_lableGroup[8].name5 = model.boxtype.CoinDedicatedChangeBoxBType;				// Ӳ��ר��������B��ֵ
		////m_lableGroup[9].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ����������ֵ
		//m_lableGroup[9].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ����������ֵ

		//m_lableGroup[7].name7 = model.boxquantity.DeBoxAAfterColQuantity;				// Ӳ��ר��������A����
		//m_lableGroup[8].name7 = model.boxquantity.DeBoxBAfterColQuantity;				// Ӳ��ר��������B����
		////m_lableGroup[9].name7 = model.boxquantity.CyBoxAfterColAQuantity;				// Ӳ��ѭ������������
		//m_lableGroup[9].name7 = model.boxquantity.CyBoxAfterColBQuantity;				// Ӳ��ѭ������������


		// ����ǰ
		m_lableGroup[0].name1 = _opl(TXT_COINMGR_BEFORE_COLLECTION);					// Ӳ�һ���ǰ
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		m_lableGroup[2].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);					// Ӳ��ѭ��������B

		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		m_lableGroup[2].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����


		m_lableGroup[1].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		m_lableGroup[2].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		m_lableGroup[1].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ����������ֵ
		m_lableGroup[2].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ����������ֵ

		m_lableGroup[1].name7 = model.boxquantity.CoinCycleChangeBoxAQuantity;			// Ӳ��ѭ��������A����
		m_lableGroup[2].name7 = model.boxquantity.CoinCycleChangeBoxBQuantity;			// Ӳ��ѭ��������B����

		// ���պ�
		m_lableGroup[6].name1 = _opl(TXT_COINMGR_AFTER_COLLECTION);					// Ӳ�һ���ǰ
		m_lableGroup[7].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);					// Ӳ��ѭ��������A
		m_lableGroup[8].name1 = _opl(DEVICESTATUS_CYCLE_B_COIN_SET);	// Ӳ��ѭ��������B

		m_lableGroup[7].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID
		m_lableGroup[8].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);				// ID

		m_lableGroup[7].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ
		m_lableGroup[8].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);					// ��ֵ

		m_lableGroup[7].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����
		m_lableGroup[8].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);	// ����

		m_lableGroup[7].name3 = model.boxid.CoinCycleChangeBoxAID;						// Ӳ��ѭ��������AID
		m_lableGroup[8].name3 = model.boxid.CoinCycleChangeBoxBID;						// Ӳ��ѭ��������BID

		m_lableGroup[7].name5 = model.boxtype.CoinCycleChangeBoxAType;					// Ӳ��ѭ����������ֵ
		m_lableGroup[8].name5 = model.boxtype.CoinCycleChangeBoxBType;					// Ӳ��ѭ����������ֵ

		m_lableGroup[7].name7 = model.boxquantity.CyBoxAfterColAQuantity;				// Ӳ��ѭ������������
		m_lableGroup[8].name7 = model.boxquantity.CyBoxAfterColBQuantity;				// Ӳ��ѭ������������
	}