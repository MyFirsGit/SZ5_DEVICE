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
	@brief      ���캯��

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
	@brief      ��������

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
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:�����ɹ�    -1:����ʧ��

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CCoinBoxManageDetachCoinCollectionBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
			return -1;
		}
		//����GStatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

		//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
		//theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��



		//InitDetachCoinCollectionBoxInfo();
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
	void CCoinBoxManageDetachCoinCollectionBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
		theAPP_SESSION.SetMaxInputLength(1);						// �����������λ��

		m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 100);		// ���õ�һ�п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 70);		// ���õڶ��п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 120);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 70);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 120);		// ���õ����п��
		m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 100);		// ���õ����п��

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
	@brief      ��ʼ��BASE����LABEL���������

	@param      void

	@retval     none

	@exception  ��
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

		m_lableGroup[0].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);							// Ӳ�һ�����A
		//m_lableGroup[3].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);							// Ӳ�һ�����B

		m_lableGroup[7].name1 = _T("[1] ")+_opl(TXT_COINMGR_DETACH_COIN_COLLECTION_BOXA);			// ж��Ӳ�һ�����A
		//m_lableGroup[8].name1 = _T("[2] ")+_opl(TXT_COINMGR_DETACH_COIN_COLLECTION_BOXB);			// ж��Ӳ�һ�����B
		//m_lableGroup[9].name1 = _T("[3] ")+_opl(TXT_COINMGR_DETACH_ALL_COIN_COLLECTION_BOX);			// ж������Ӳ�һ�����

		m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
		//m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID

		m_lableGroup[1].name2 = _opl(TXT_COINMGR_TYPE);					
		//m_lableGroup[4].name2 = _T("��ֵ:");
		


		m_lableGroup[0].name4 = _opl(TXT_BANKNOTEMGR_BOX_TOTAL_AMOUNT);					// �ϼ�
		//m_lableGroup[3].name4 =  _T("�ϼ�");				// �ϼ�

		m_lableGroup[1].name4 = _opl(TXT_BANKNOTEMGT_QTY);					// ����
		//m_lableGroup[4].name4 =  _T("����");				// ����

		
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
				theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
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
	@brief      ����״̬��ʾ����ɫ

	@param      void

	@retval     none

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachCoinCollectionBoxDlg::SetColor(int row,CString status)
	{
		if(_opl(TXT_COINMGR_BOX_ATTACHED) == status)						// ��װ-��ɫ
		{
			m_baseInfoArea->SetLabelColor(row,LABEL_COL_6,GREEN);
		}
		else																// ж��-��ɫ
		{
			m_baseInfoArea->SetLabelColor(row,LABEL_COL_6,RED);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�һ�����״̬   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CCoinBoxManageDetachCoinCollectionBoxDlg::ShowDetachCoinCollectionBoxInfo()
	{
		CCoinBoxManageDetachCoinCollectionBoxSvc* pSvc = (CCoinBoxManageDetachCoinCollectionBoxSvc*)GetService();
		CCoinBoxManageDetachCoinCollectionBoxSvc::Model& model = pSvc->GetDataModel();

		// ������ɫ
		SetColor(0,model.coincollectionboxAdata.CoinCollectionBoxAStatus);
		SetColor(3,model.coincollectionboxBdata.CoinCollectionBoxBStatus);

		m_lableGroup[0].name3 = model.BoxA.countInfo.coinBoxID.ToString();				// Ӳ�һ�����AID
		//m_lableGroup[3].name3 = model.BoxB.countInfo.coinBoxID.ToString();				// Ӳ�һ�����BID

		m_lableGroup[0].name5.Format(_T("%d ") + _opl(TXT_TAKE_TICKETS_YUAN),model.BoxA.countInfo.GetCurCount());	// Ӳ�һ�����A�ϼ�
		//m_lableGroup[3].name5.Format(_T("%d Ԫ"),model.BoxB.countInfo.GetCurCount());	// Ӳ�һ�����B�ϼ�

		m_lableGroup[0].name6 = model.BoxA.GetStatusString();			// Ӳ�һ�����A��װ״̬
		m_baseInfoArea->SetLabelColor(0,LABEL_COL_6,model.BoxA.GetStatusColor());
		//m_lableGroup[3].name6 = model.BoxB.GetStatusString();			// Ӳ�һ�����B��װ״̬
		//m_baseInfoArea->SetLabelColor(3,LABEL_COL_6,model.BoxB.GetStatusColor());

		m_lableGroup[1].name3 = _T("1 ") + _opl(TXT_TAKE_TICKETS_YUAN);		// Ӳ�һ�����A50AVOSData
		//m_lableGroup[4].name3 = _T("1 Ԫ");		// Ӳ�һ�����A2MOPData	


		m_lableGroup[1].name5 .Format(_T("%d ") + _opl(TXT_COIN_UNIT), model.BoxA.countInfo.GetCurCount());		// Ӳ�һ�����A1MOPData
		//m_lableGroup[4].name5 .Format(_T("%d ö"), model.BoxB.countInfo.GetCurCount());		// Ӳ�һ�����A5MOPData


	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		��ʾӲ�һ�����ж��״̬   

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

			m_baseInfoArea->SetLableWidth(3,LABEL_COL_1,100);		// ���õ�һ�п��
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_2,70);		// ���õڶ��п��
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_3,150);		// ���õ����п��
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_4,70);		// ���õ����п��
			m_baseInfoArea->SetLableWidth(3,LABEL_COL_5,130);		// ���õ����п��
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

		// ������ɫ
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

		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_FINISH));							// ж�����

		//m_lableGroup[0].name1 = _opl(TXT_COINMGR_BOX_BEFORE_DETACH);								// ж��ǰ
		m_lableGroup[1].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);							// Ӳ�һ�����A
		//m_lableGroup[4].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);							// Ӳ�һ�����B

		m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
		//m_lableGroup[4].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID

		//m_lableGroup[9].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
		//m_lableGroup[10].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID

		//m_lableGroup[2].name2 = _opl(GUIDE_ADDBOX_SET_FAVO)+_opl(TXT_COLON);						// 50 AVOS
		//m_lableGroup[3].name2 = _opl(GUIDE_ADDBOX_SET_SMOP)+_opl(TXT_COLON);						// 2  MOP 
	//	m_lableGroup[5].name2 = _opl(GUIDE_ADDBOX_SET_FAVO)+_opl(TXT_COLON);						// 50 AVOS
		//m_lableGroup[6].name2 = _opl(GUIDE_ADDBOX_SET_SMOP)+_opl(TXT_COLON);						// 2  MOP 


		m_lableGroup[1].name4 = _opl(TXT_COINMGR_BOX_TOTAL_AMOUNT)+_opl(TXT_COLON);					// �ϼ�
		//m_lableGroup[4].name4 = _opl(TXT_COINMGR_BOX_TOTAL_AMOUNT)+_opl(TXT_COLON);					// �ϼ�

		m_lableGroup[2].name4 = _opl(GUIDE_ADDBOX_SET_OMOP)+_opl(TXT_COLON);						// 1  MOP 
		//m_lableGroup[3].name4 = _opl(GUIDE_ADDBOX_SET_FMOP)+_opl(TXT_COLON);						// 5  MOP
		//m_lableGroup[5].name4 = _opl(GUIDE_ADDBOX_SET_OMOP)+_opl(TXT_COLON);						// 1  MOP 
		//m_lableGroup[6].name4 = _opl(GUIDE_ADDBOX_SET_FMOP)+_opl(TXT_COLON);						// 5  MOP

		//m_lableGroup[8].name1 = _opl(TXT_COINMGR_BOX_AFTER_DETACH);									// ж�غ�
		//m_lableGroup[9].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);							// Ӳ�һ�����A
		//m_lableGroup[10].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);							// Ӳ�һ�����B


		m_lableGroup[1].name3 = model.coincollectionboxAdata.CoinCollectionBoxAID;				// Ӳ�һ�����AID
		//m_lableGroup[4].name3 = model.coincollectionboxBdata.CoinCollectionBoxBID;				// Ӳ�һ�����BID

		m_lableGroup[1].name5 = model.coincollectionboxAdata.CoinCollectionBoxATotalData;		// Ӳ�һ�����A�ϼ�
		//m_lableGroup[4].name5 = model.coincollectionboxBdata.CoinCollectionBoxBTotalData;		// Ӳ�һ�����B�ϼ�

		m_lableGroup[1].name6 = model.coincollectionboxAdata.CoinBoxABeforeRemove;				// Ӳ�һ�����A��װ״̬
		//m_lableGroup[4].name6 = model.coincollectionboxBdata.CoinBoxBBeforeRemove;				// Ӳ�һ�����B��װ״̬

		//m_lableGroup[2].name3 = model.coincollectionboxAdata.CoinCollectionBoxA50AVOSData;		// Ӳ�һ�����A50AVOSData
		//m_lableGroup[3].name3 = model.coincollectionboxAdata.CoinCollectionBoxA2MOPData;		// Ӳ�һ�����A2MOPData	

		//m_lableGroup[5].name3 = model.coincollectionboxBdata.CoinCollectionBoxB50AVOSData;		// Ӳ�һ�����B50AVOSData
		//m_lableGroup[6].name3 = model.coincollectionboxBdata.CoinCollectionBoxB2MOPData;		// Ӳ�һ�����B2MOPData

		m_lableGroup[2].name5 = model.coincollectionboxAdata.CoinCollectionBoxA1MOPData;		// Ӳ�һ�����A1MOPData
		//m_lableGroup[3].name5 = model.coincollectionboxAdata.CoinCollectionBoxA5MOPData;		// Ӳ�һ�����A5MOPData

		//m_lableGroup[5].name5 = model.coincollectionboxBdata.CoinCollectionBoxB1MOPData;		// Ӳ�һ�����B1MOPData
		//m_lableGroup[6].name5 = model.coincollectionboxBdata.CoinCollectionBoxB5MOPData;		// Ӳ�һ�����B5MOPData

		//m_lableGroup[9].name3 = model.coincollectionboxAdata.CoinCollectionBoxAfterDetachAID;	// Ӳ�һ�����Aж��IDData
		//m_lableGroup[10].name3 = model.coincollectionboxBdata.CoinCollectionBoxAfterDetachBID;	// Ӳ�һ�����Bж��IDData

		//m_lableGroup[9].name5 = /*model.coincollectionboxAdata.CoinCollectionAfterBoxAStatus*/_T("");		// Ӳ�һ�����Aж��IDData
		//m_lableGroup[10].name5 = /*model.coincollectionboxBdata.CoinCollectionAfterBoxBStatus*/_T("");		// Ӳ�һ�����Bж��IDData
	}