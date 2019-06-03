#include "stdafx.h"
#include "InstallCoinChangeBoxDlg.h"
#include "InstallCoinChangeBoxSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CInstallCoinChangeBoxDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CInstallCoinChangeBoxDlg, COperationDlg)
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
CInstallCoinChangeBoxDlg::CInstallCoinChangeBoxDlg(CService* pService)
	: COperationDlg(CInstallCoinChangeBoxDlg::IDD,pService)
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
CInstallCoinChangeBoxDlg::~CInstallCoinChangeBoxDlg()
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
int CInstallCoinChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	//����GStatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_lableGroup);

	//theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
	//theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��

	//m_baseInfoArea->SetLableWidth(LABEL_COL_1,220);		// ���õ�һ�п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_2,80);		// ���õڶ��п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_3,150);		// ���õ����п��
	//m_baseInfoArea->SetLableWidth(LABEL_COL_4,150);		// ���õ����п��

	//m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_RIGHT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	//m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);

	//InitAttachCoinCollectionBoxInfo();
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
void CInstallCoinChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);						// �����������λ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 170);		// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 150);		// ���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 150);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 120);		// ���õ����п��

	m_baseInfoArea->SetLableAlign(LABEL_COL_1,  DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2,  DT_LEFT| DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_4, DT_LEFT | DT_VCENTER);

	m_baseInfoArea->SetLableWidth(3,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(4,0,RESOLUTION * 230);
	m_baseInfoArea->SetLableWidth(5,0,RESOLUTION * 230);

	InitAttachCoinCollectionBoxInfo();
	ShowAttachCoinCollectionBoxInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::InitAttachCoinCollectionBoxInfo()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();
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
	}

	m_lableGroup[0].name1 = _T("Ӳ��������A");							
	m_lableGroup[1].name1 = _T("Ӳ��������B");							

	m_lableGroup[3].name1 = _T("[1] ��װӲ��������A");			// ��װ A
	m_lableGroup[4].name1 = _T("[2] ��װӲ��������B");			// ��װ B
	m_lableGroup[5].name1 = _T("[3] ��װ����Ӳ��������");			// ��װ���� 

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON);							// ID
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::UpdateUI()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();
	if(1 == model.AttachCoinCollectionBoxFlowTime)
	{
		ShowAttachCoinCollectionBoxInfo();
	}
	/*else if(2 == model.AttachCoinCollectionBoxFlowTime)
	{
		ShowDoAttachCoinCollectionBoxInfo();
	}*/
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��ʾӲ�һ�����״̬ 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::ShowAttachCoinCollectionBoxInfo()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();

	// ������ɫ
	SetColor(0,model.CoinCollectionBoxAStatus);
	SetColor(1,model.CoinCollectionBoxBStatus);

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxAID;				// Ӳ�һ�����AIDData
	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxBID;				// Ӳ�һ�����BIDData

	m_lableGroup[0].name3 = _T("����:") + model.strCoinACount;				// Ӳ�һ�����AIDData
	m_lableGroup[1].name3 = _T("����:") + model.strCoinBCount;				// Ӳ�һ�����BIDData

	m_lableGroup[0].name4 = model.strGetState(model.stateA);			// Ӳ�һ�����A��װData
	m_lableGroup[1].name4 = model.strGetState(model.stateB);			// Ӳ�һ�����B��װData
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����״̬��ʾ����ɫ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::SetColor(int row,CString status)
{
	if(_opl(TXT_COINMGR_BOX_ATTACHED) == status || _opl(TXT_TOKENMGR_WAITTOATTACH) == status)						// ��װ-��ɫ
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
@brief		��ʾӲ�һ����䰲װ״̬ 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxDlg::ShowDoAttachCoinCollectionBoxInfo()
{
	CInstallCoinChangeBoxSvc* pSvc = (CInstallCoinChangeBoxSvc*)GetService();
	CInstallCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();

	// ������ɫ
	SetColor(1,model.CoinCollectionBoxAStatus);
	SetColor(2,model.CoinCollectionBoxBStatus);
	SetColor(5,model.CoinCollectionAfterBoxAStatus);
	SetColor(6,model.CoinCollectionAfterBoxBStatus);

	m_lableGroup[0].name2 = _T("");
	m_lableGroup[0].name3 = _T("");
	m_lableGroup[0].name4 = _T("");
	m_lableGroup[3].name1 = _T("");																			// ���

	model.IsPrintFalied ? NULL : theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ATTACH_FINISH));
	m_lableGroup[0].name1 = _opl(TXT_COINMGR_BOX_BEFORE_ATTACH);											// ��װǰ
	m_lableGroup[1].name1 = _T("Ӳ��������A");	
	m_lableGroup[2].name1 = _T("Ӳ��������B");							

	m_lableGroup[1].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxAID;			// ID
	m_lableGroup[2].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinChangeBoxBID;			// ID

	m_lableGroup[1].name3 = _T("����:")+model.strCoinACount;												// Ӳ������
	m_lableGroup[2].name3 = _T("����:")+model.strCoinBCount;												// Ӳ������

	m_lableGroup[1].name4 = model.strGetState(model.stateA);															// ״̬
	m_lableGroup[2].name4 = model.strGetState(model.stateB);															// ״̬

	m_lableGroup[4].name1 = _opl(TXT_COINMGR_BOX_AFTER_ATTACH);									// ��װ��
	m_lableGroup[5].name1 = _opl(DEVICESTATUS_COLLECTION_COINA_SET);										 
	m_lableGroup[6].name1 = _opl(DEVICESTATUS_COLLECTION_COINB_SET);										 

	m_lableGroup[5].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinCollctionBoxAfterAttachAID; 
	m_lableGroup[6].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + model.CoinCollctionBoxAfterAttachAID; 

	m_lableGroup[5].name3 = _T("����:")+model.strCoinACountAfter;									
	m_lableGroup[6].name3 = _T("����:")+model.strCoinBCountAfter;								

	m_lableGroup[5].name4 = /*model.CoinCollectionAfterBoxAStatus*/_T("");						// Ӳ�һ�����A��װ��Status
	m_lableGroup[6].name4 = /*model.CoinCollectionAfterBoxBStatus*/_T("");						// Ӳ�һ�����B��װ��Status
}