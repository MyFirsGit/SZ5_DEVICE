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
	@brief      ���캯��

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
	@brief      ��������

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
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:�����ɹ�    -1:����ʧ��

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CCoinBoxManageDetachAddingBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
			return -1;
		}
		//����GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
		//theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 210);		// ���õ�һ�п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 100);		// ���õڶ��п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 100);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 200);		// ���õ����п��

		m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
		m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);

		//InitDetachCoinAddingBoxInfo();
		//ShowAttachCoinCollectionBoxInfo();

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
	void CCoinBoxManageDetachAddingBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
		theAPP_SESSION.SetMaxInputLength(1);						// �����������λ��

		InitDetachCoinAddingBoxInfo();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��ʼ��BASE����LABEL���������

	@param      void

	@retval     none

	@exception  ��
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

		m_lableGroup[0].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXA);								// Ӳ�Ҳ�����A
		m_lableGroup[1].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXB);								// Ӳ�Ҳ�����B

		m_lableGroup[3].name1 = _T("[1] ")+_opl(TXT_COINMGR_DETACH_COIN_ADDING_BOXA);			// ж��Ӳ�Ҳ�����A
		m_lableGroup[4].name1 = _T("[2] ")+_opl(TXT_COINMGR_DETACH_COIN_ADDING_BOXB);			// ж��Ӳ�Ҳ�����B
		m_lableGroup[5].name1 = _T("[3] ")+_opl(TXT_COINMGR_DETACH_ALL_COIN_ADDING_BOX);			// ж������Ӳ�Ҳ�����

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
			theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
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
	@brief      ����״̬��ʾ����ɫ

	@param      void

	@retval     none

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::SetColor(int row,CString status)
	{
		if(_opl(TXT_COINMGR_BOX_ATTACHED) == status )						// ��װ-��ɫ
		{
			m_baseInfoArea->SetLabelColor(row,LABEL_COL_4,GREEN);
		}
		else																// ж��-��ɫ
		{
			m_baseInfoArea->SetLabelColor(row,LABEL_COL_4,RED);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�Ҳ�����״̬   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::ShowDetachCoinAddingBoxInfo()
	{
		CCoinBoxManageDetachCoinAddingBoxSvc* pSvc = (CCoinBoxManageDetachCoinAddingBoxSvc*)GetService();
		CCoinBoxManageDetachCoinAddingBoxSvc::Model& model = pSvc->GetDataModel();

		// ������ɫ
		SetColor(0,model.CoinAddingBoxAStatus);
		SetColor(1,model.CoinAddingBoxBStatus);

		m_lableGroup[0].name3 = model.CoinAddingBoxAID;							// Ӳ�Ҳ�����AIDData
		m_lableGroup[1].name3 = model.CoinAddingBoxBID;							// Ӳ�Ҳ�����BIDData

		m_lableGroup[0].name4 = model.CoinAddingBoxAStatus;						// Ӳ�Ҳ�����A״̬Data
		m_lableGroup[1].name4 = model.CoinAddingBoxBStatus;						// Ӳ�Ҳ�����B״̬Data
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�Ҳ�����ж��״̬   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachAddingBoxDlg::ShowDoDetachCoinAddingBoxInfo()
	{
		CCoinBoxManageDetachCoinAddingBoxSvc* pSvc = (CCoinBoxManageDetachCoinAddingBoxSvc*)GetService();
		CCoinBoxManageDetachCoinAddingBoxSvc::Model& model = pSvc->GetDataModel();

		// ������ɫ
		SetColor(1,model.CoinAddingBoxAStatus);
		SetColor(2,model.CoinAddingBoxBStatus);
		SetColor(5,model.CoinAddingBoxAfterDetachStatus);
		SetColor(6,model.CoinAddingBoxBfterDetachStatus);

		m_lableGroup[0].name2 = _T("");
		m_lableGroup[0].name3 = _T("");
		m_lableGroup[0].name4 = _T("");
		m_lableGroup[3].name1 = _T("");															// ���

		model.IsPrintFailed ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_FINISH));						// ж�����
		m_lableGroup[0].name1 = _opl(TXT_COINMGR_BOX_BEFORE_DETACH);							// ж��ǰ
		m_lableGroup[1].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXA);								// Ӳ�Ҳ�����A
		m_lableGroup[2].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXB);								// Ӳ�Ҳ�����B

		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID


		m_lableGroup[4].name1 = _opl(TXT_COINMGR_BOX_AFTER_DETACH);								// ж�غ�
		m_lableGroup[5].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXA);								// Ӳ�Ҳ�����A
		m_lableGroup[6].name1 = _opl(TXT_COINMGR_COIN_ADDING_BOXB);								// Ӳ�Ҳ�����B

		m_lableGroup[5].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID
		m_lableGroup[6].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);						// ID

		m_lableGroup[1].name3 = model.CoinAddingBoxAID;											// Ӳ�Ҳ�����AIDData
		m_lableGroup[2].name3 = model.CoinAddingBoxBID;											// Ӳ�Ҳ�����BIDData

		m_lableGroup[1].name4 = model.CoinBoxABeforeAddingStatus;										// Ӳ�Ҳ�����Aж��Data
		m_lableGroup[2].name4 = model.CoinBoxBBeforeAddingStatus;										// Ӳ�Ҳ�����Bж��Data

		m_lableGroup[5].name3 = model.CoinAddingBoxAfterDetachAID;								// Ӳ�Ҳ�����Aж�غ�IDData
		m_lableGroup[6].name3 = model.CoinAddingBoxAfterDetachBID;								// Ӳ�Ҳ�����Bж�غ�IDData

		m_lableGroup[5].name4 = /*model.CoinAddingBoxAfterDetachStatus*/_T("");					// Ӳ�Ҳ�����Aж�غ�״̬
		m_lableGroup[6].name4 = /*model.CoinAddingBoxBfterDetachStatus*/_T("");					// Ӳ�Ҳ�����Bж�غ�״̬
	}