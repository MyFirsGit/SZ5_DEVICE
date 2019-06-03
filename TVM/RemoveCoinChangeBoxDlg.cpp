#include "StdAfx.h"
#include "RemoveCoinChangeBoxDlg.h"
#include "RemoveCoinChangeBoxSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRemoveCoinChangeBoxDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CRemoveCoinChangeBoxDlg, COperationDlg)
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
CRemoveCoinChangeBoxDlg::CRemoveCoinChangeBoxDlg(CService* pService)
	: COperationDlg(CRemoveCoinChangeBoxDlg::IDD,pService)
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
CRemoveCoinChangeBoxDlg::~CRemoveCoinChangeBoxDlg()
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
int CRemoveCoinChangeBoxDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CRemoveCoinChangeBoxDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);						// �����������λ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,RESOLUTION * 120);		// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,RESOLUTION * 120);		// ���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3,RESOLUTION * 80);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_4,RESOLUTION * 60);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_5,RESOLUTION * 40);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_6,RESOLUTION * 60);		// ���õ����п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_7,RESOLUTION * 70);		// ���õ����п��

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
	m_baseInfoArea->SetLableAlign(LABEL_COL_7, DT_LEFT | DT_VCENTER);

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
void CRemoveCoinChangeBoxDlg::InitDetachCoinCollectionBoxInfo()
{
	
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

	CRemoveCoinChangeBoxSvc* pSvc = (CRemoveCoinChangeBoxSvc*)GetService();
	CRemoveCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.IsHasException)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_COIN_MODULE_EXCEPTION));
		theAPP_SESSION.DiableKeyboard(KEY_NUMBER);
		//return;
	}
	CRemoveCoinChangeBoxSvc::CHANGE_BOX_INFO boxA = model.boxAinfo;
	CRemoveCoinChangeBoxSvc::CHANGE_BOX_INFO boxB = model.boxBinfo;

	m_lableGroup[0].name1 = _T("Ӳ��������A");							// Ӳ�һ�����A
	m_lableGroup[3].name1 = _T("Ӳ��������B");							// Ӳ�һ�����B

	m_lableGroup[7].name1 = _T("[1] ж��Ӳ��������A");					// ж��Ӳ�һ�����A
	m_lableGroup[8].name1 = _T("[2] ж��Ӳ��������B");					// ж��Ӳ�һ�����B
	m_lableGroup[9].name1 = _T("[3] ж������Ӳ��������");				// ж������Ӳ�һ�����

	m_lableGroup[0].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + boxA.strID;							// ID
	m_lableGroup[3].name2 = _opl(DEVICESTATUS_ID_SET)+_opl(TXT_COLON) + boxB.strID;							// ID


	m_lableGroup[0].name3 = _T("��ֵ")+_opl(TXT_COLON);											// ���
	m_lableGroup[3].name3 = _T("��ֵ")+_opl(TXT_COLON);
	// name5 ��ֵ
	m_lableGroup[0].name4 = _T("1 Ԫ")/*.Format(_T("%d"),boxA.value)*/;
	m_lableGroup[3].name4 = _T("1 Ԫ")/*Format(_T("%d"),boxB.value)*/;
	
	m_lableGroup[0].name5 = _T("����")+_opl(TXT_COLON);											// ����
	m_lableGroup[3].name5 = _T("����")+_opl(TXT_COLON);											// ����
	// name6 ����
	m_lableGroup[0].name6.Format(_T("%d"),boxA.nCount);
	m_lableGroup[3].name6.Format(_T("%d"),boxB.nCount);

	// name7
	m_lableGroup[0].name7 = boxA.StateToString();											  // ״̬
	m_baseInfoArea->SetLabelColor(0,LABEL_COL_7,boxA.GetColor());
	m_lableGroup[3].name7 = boxB.StateToString();	
	m_baseInfoArea->SetLabelColor(3,LABEL_COL_7,boxB.GetColor());

	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxDlg::UpdateUI()
{
	/*CRemoveCoinChangeBoxSvc* pSvc = (CRemoveCoinChangeBoxSvc*)GetService();
	CRemoveCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();*/
	InitDetachCoinCollectionBoxInfo();
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
void CRemoveCoinChangeBoxDlg::ShowDetachCoinCollectionBoxInfo()
{
	CRemoveCoinChangeBoxSvc* pSvc = (CRemoveCoinChangeBoxSvc*)GetService();
	CRemoveCoinChangeBoxSvc::Model& model = pSvc->GetDataModel();

}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��ʾӲ�һ�����ж��״̬   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxDlg::ShowDoDetachCoinCollectionBoxInfo()
{
	
}