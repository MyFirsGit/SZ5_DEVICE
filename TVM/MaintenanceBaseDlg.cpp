#include "stdafx.h"
#include "MaintenanceSvc.h"
#include "MaintenanceBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMaintenanceBaseDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CMaintenanceBaseDlg, COperationDlg)
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
CMaintenanceBaseDlg::CMaintenanceBaseDlg(CService* pService)
	: COperationDlg(CMaintenanceBaseDlg::IDD,pService)
{
	m_baseInfo->detailInfo.labelGroup = m_labelGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceBaseDlg::~CMaintenanceBaseDlg()
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
int CMaintenanceBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA|BASE_AREA)) {
        return -1;
    }

	// �����ı�����
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 630);
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 10);

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
void CMaintenanceBaseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	theAPP_SESSION.SetMaxInputLength(2);

	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F6_ESC2));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceBaseDlg::UpdateUI()
{
	Refresh();
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ҳ����

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceBaseDlg::Refresh()
{
	CMaintenanceSvc* pSvc = (CMaintenanceSvc*)GetService();
	CMaintenanceSvc::Model model = pSvc->GetDataModel();

	// ��ȡ�˵��б�
	vector<Menu_Item*>& list = (model.secondlist == NULL) ? model.firstlist : model.secondlist->children;

	// ���ò˵�����
	int index = 0;
	CString title;
	for (vector<Menu_Item*>::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		if(!(*iter)->isVisible || !(*iter)->isEnable)
			continue;
		title.Format(_T("[%d] %s"), MENU_CODE_1 + index, (*iter)->text);
		m_labelGroup[index].nameLeft = title;
		++index;
	}
	if(model.secondlist != NULL)
	{
		// ֽ�������
		if(model.secondlist->btnID == BANKNOTE_BOX_MANAGE_SVC_ID){
			theAPP_SESSION.SetMenuLevel(BANKNOTE_BOX_MANAGE_SVC);
		}
		// Ӳ�������
		else if(model.secondlist->btnID == COIN_BOX_MANAGE_SVC_ID){
			theAPP_SESSION.SetMenuLevel(COIN_BOX_MANAGE_SVC);
		}
		// ϵͳ�趨
		else if(model.secondlist->btnID == SYSTEM_SET_SVC_ID){
			theAPP_SESSION.SetMenuLevel(SYSTEM_SET_SVC);
		}
		// ���ݹ���
		else if(model.secondlist->btnID == DATA_MANAGE_SVC_ID){
			theAPP_SESSION.SetMenuLevel(DATA_MANAGE_SVC);
		}
		// Ӳ���Լ�
		else if(model.secondlist->btnID == DEVICE_TEST_SVC_ID){
			theAPP_SESSION.SetMenuLevel(DEVICE_TEST_SVC);
		}
		// �ػ�����
		else if(model.secondlist->btnID == SHUTDOWN_ID){
			theAPP_SESSION.SetMenuLevel(SHUTDOWN_RESTART_SVC);
		}
		else if(model.secondlist->children.size() == 4)
		{
			theAPP_SESSION.SetMenuLevel(STATION_SET_SVC);
		}
		else if(model.secondlist->children.size() == 6)
		{
			theAPP_SESSION.SetMenuLevel(ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC);
		}
		else if(model.secondlist->children.size() == 10)
		{
			theAPP_SESSION.SetMenuLevel(RW_TEST_SVC);
		}
		else if(model.secondlist->children.size() == 3)
		{
			theAPP_SESSION.SetMenuLevel(DATA_INPUT_SVC);
		}
		else if(model.secondlist->children.size() == 2)
		{
			theAPP_SESSION.SetMenuLevel(SHUTDOWN_SVC);
		}
	}
	else
	{
		theAPP_SESSION.SetMenuLevel(MAINTENANCE_SVC);
	}
	// �����������
	for (; index < 15; ++index)
	{
		m_labelGroup[index].nameLeft = _T("");
	}
}