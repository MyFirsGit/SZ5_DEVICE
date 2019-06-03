#include "StdAfx.h"
#include "BanknoteCollectingBoxInstallationDlg.h"


static const int FULL_WITDH = 600;
static const int NAME_WITDH = 400;
static const int ID_WITDH = 150;
static const int INDENT_WIDTH = 50;
static const int GIRD_WITDH = 160;
static const int STATUE_WIDTH = 100;
static const int FACE_WIDTH = 80;
static const int FACE_WIDTH_1 = 120;
static const int COUNT_WIDTH = 50;
static const int MONEY_WIDTH = 130;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBanknoteCollectingBoxInstallationDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CBanknoteCollectingBoxInstallationDlg, COperationDlg)
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
CBanknoteCollectingBoxInstallationDlg::CBanknoteCollectingBoxInstallationDlg(CService* pService)
	: COperationDlg(CBanknoteCollectingBoxInstallationDlg::IDD,pService)
{
	pSvc = (CBanknoteCollectingBoxInstallationSvc*)pService;
}

////////////////////////////// ////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxInstallationDlg::~CBanknoteCollectingBoxInstallationDlg()
{
	if (m_baseInfoArea != NULL)	{
		delete m_baseInfoArea;
	}
	m_baseInfoArea = NULL;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CBanknoteCollectingBoxInstallationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}
	// ����GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	CBanknoteCollectingBoxInstallationSvc::Model& model = pSvc->GetDataModel();

	// �����������
	Clear();

	theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_ENTER|KEY_INPUT|KEY_F2);
	theAPP_SESSION.SetMaxInputLength(2);
	// ���漤���
	InitAttachBoxUI();
	if (!pSvc->GetShowErrorMsg().IsEmpty() && pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_FINISH){
		theAPP_SESSION.ShowOfficeGuide(pSvc->GetShowErrorMsg());
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::UpdateUI()
{
	// �������
	Clear();
	// ��װ���� 
	ShowAttachBoxData();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��װֽ���仭�沼�ֳ�ʼ��   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::InitAttachBoxUI()
{
	// ������Ϣ��ʽ
	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 400);					// ֽ���� ID 
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 100);					// ״̬

	// �˵�����ʽ
	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_OPERATION)
	{
		m_baseInfoArea->SetLableWidth(LABEL_ROW_2, LABEL_COL_1, FULL_WITDH);	// �˵�1
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		�������   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::Clear()
{
	for (int i = 0; i < 15; ++i)
	{
		m_lableGroup[i].name1 = _T(""); 
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief		��ʾ��װֽ��������   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::ShowAttachBoxData()
{
	CBanknoteCollectingBoxInstallationSvc::Model& model = pSvc->GetDataModel();

	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_OPERATION)
	{
		m_lableGroup[LABEL_ROW_1].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+ _T(" %s"), model.strBoxIDSt);
		m_lableGroup[LABEL_ROW_1].name2 = model.strState;

		m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2, model.clrState);
		m_lableGroup[LABEL_ROW_8].name1.Format(_T("[%d] ")+_opl(TXT_BANKNOTEMGR_MENU_ATTACH), MENU_CODE_1);	
	}
	else if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_FINISH)
	{
		m_lableGroup[LABEL_ROW_1].name1 = _opl(TXT_BANKNOTEMGR_BOX_BEFORE_ATTACH);

		m_lableGroup[LABEL_ROW_2].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+ _T(" %s"),model.strBoxIDEx);

		m_lableGroup[LABEL_ROW_6].name1 = _opl(TXT_BANKNOTEMGR_BOX_AFTER_ATTACH);

		m_lableGroup[LABEL_ROW_7].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+_T(" %s"), model.strBoxIDSt);
	}
	else
	{
		Clear();
	}
}
