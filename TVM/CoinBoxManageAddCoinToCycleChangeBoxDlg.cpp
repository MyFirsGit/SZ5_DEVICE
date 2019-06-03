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
	@brief      ���캯��

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
	@brief      ��������

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
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:�����ɹ�    -1:����ʧ��

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CCoinBoxManageAddCoinToCycleChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA)) {
			return -1;
		}
		//����GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);	// ����ָ������
		//theAPP_SESSION.SetMaxInputLength(1);				// �����������λ��	

		//m_baseInfoArea->SetLableWidth(LABEL_COL_1,180);		// ���õ�һ�п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);		// ���õڶ��п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_3,70);		// ���õ����п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_4,50);		// ���õ����п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_5,80);		// ���õ����п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_6,50);		// ���õ����п��
		//m_baseInfoArea->SetLableWidth(LABEL_COL_7,100);		// ���õ����п��

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
	@brief      ���漤���Ĵ���

	@param      (i)UINT nState      ��ʹ��

	@param      (i)CWnd* pWndOther  ��ʹ��

	@param      (i)BOOL bMinimized  ��ʹ��

	@retval     void

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState, pWndOther, bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);	// ����ָ������
		theAPP_SESSION.SetMaxInputLength(1);				// �����������λ��	

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,170);		// ���õ�һ�п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,70);		// ���õڶ��п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,80);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,50);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_5,70);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_6,60);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_7,100);		// ���õ����п��

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
	@brief      ��ʼ��BASE����LABEL���������

	@param      void

	@retval     none

	@exception  ��
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

		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// Ӳ��ѭ��������
		m_lableGroup[2].name1 = _T("[1]")+_opl(COIN_TEST_TEXT_START_COIN_RECEIVING);			// ��ʼ����Ӳ��

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// ��ֵ

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// ����
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
	@brief      ��ʾӲ��ѭ��������ID������

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageAddCoinToCycleChangeBoxDlg::ShowCycleChangeBoxData()
	{
		CCoinBoxManageAddCoinToCycleChangeBoxSvc* pSvc = (CCoinBoxManageAddCoinToCycleChangeBoxSvc*)GetService();
		CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& model = pSvc->GetDataModel();

		m_lableGroup[0].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// Ӳ��ѭ��������ID
		m_lableGroup[0].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// Ӳ��ѭ����������ֵ
		m_lableGroup[0].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxQuantity;			// Ӳ��ѭ������������
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��ʾӲ��ѭ��������ӱ��н���

	@param      ��

	@retval     ��

	@exception  ��
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

		m_lableGroup[0].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// Ӳ��ѭ��������
		m_lableGroup[3].name1 = _T("[1]")+_opl(TXT_COINMGR_CYCLE_CAHNGE_BOX_STOP_ADDING);		// ֹͣ�ӱ�
		//m_lableGroup[4].name1 = _T("[2]")+_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_CANCLE_ADDINF);		// ȡ���ӱ�

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

		m_lableGroup[0].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// ��ֵ

		m_lableGroup[0].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// ����
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);			// ���

		m_lableGroup[0].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// Ӳ��ѭ��������ID
		m_lableGroup[0].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// Ӳ��ѭ����������ֵ
		m_lableGroup[0].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxQuantity;			// Ӳ��ѭ������������

		// �ӱ���
		m_lableGroup[1].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity;		// Ӳ��ѭ��������ӱ�����

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��ʾӲ��ѭ��������ӱ���ɽ���

	@param      ��

	@retval     ��

	@exception  ��
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
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// ƾ֤��ӡʧ��
		}
		else
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDCOIN_FINISH));
		}

		m_lableGroup[0].name1 = _opl(TXT_COINMGR_ADDING_BEFORE_ADDING);							// �ӱ�ǰ
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// Ӳ��ѭ��������
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[1].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// ��ֵ
		m_lableGroup[1].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// ����
		m_lableGroup[2].name6 = _opl(TXT_COINMGR_BOX_ADDED_QUANTITY)+_opl(TXT_COLON);			// ���

		m_lableGroup[1].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// Ӳ��ѭ��������ID
		m_lableGroup[1].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// Ӳ��ѭ����������ֵ
		CString strTemp;
		strTemp.Format(_T("%d"),model.cyclechangeboxdata.nBeforeAddingStockCount);
		m_lableGroup[1].name7 = strTemp;			// Ӳ��ѭ������������
		m_lableGroup[2].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity;		// Ӳ��ѭ��������ӱ�����

		m_lableGroup[4].name1 = _opl(TXT_COINMGR_ADDING_AFTER_ADDING);							// �ӱҺ�
		m_lableGroup[5].name1 = _opl(DEVICESTATUS_CYCLE_A_COIN_SET);								// Ӳ��ѭ��������
		m_lableGroup[5].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[5].name4 = _opl(TXT_COINMGR_TYPE)+_opl(TXT_COLON);							// ��ֵ
		m_lableGroup[5].name6 = _opl(TXT_COINMGR_BOX_TOTAL_QUANTITY)+_opl(TXT_COLON);			// ����

		m_lableGroup[5].name3 = model.cyclechangeboxdata.CycleChangeCoinBoxID;					// Ӳ��ѭ��������ID
		m_lableGroup[5].name5 = model.cyclechangeboxdata.CycleChangeCoinBoxTye;					// Ӳ��ѭ����������ֵ
		m_lableGroup[5].name7 = model.cyclechangeboxdata.CycleChangeCoinBoxAfterAddingQuantity;	// Ӳ��ѭ������������
	}


