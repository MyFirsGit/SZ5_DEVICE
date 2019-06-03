#include "stdafx.h"
#include "MaintenanceSvc.h"
#include "MaintenanceBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CMaintenanceSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceSvc::CMaintenanceSvc()
	:CTVMForeService(MAINTENANCE_SVC)
{
    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_20000_MAINTENANCE_BASE_DLG, new CMaintenanceBaseDlg(this));

    // ���ó˿ͻ���

    // ���û������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F6_ESC2));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CMaintenanceBaseDlg::IDD);

    // ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	theAPP_SESSION.SetMaxInputLength(2);
	m_isReturn = false;
	InitData();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceSvc::~CMaintenanceSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ������ó�ʼ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceSvc::OnStart(){
	m_isSecondary = false;//add
	if (m_isSecondary && 
		(theAPP_SESSION.GetMaintainMenu() != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC &&
		theAPP_SESSION.GetReturnSeviceID() != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC))
	{
		theAPP_SESSION.SetUserLoginStatus(true);
		// ���ó�ʼ������
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(2);

// 		theAPP_SESSION.ReturnMaintainMenu();
		InitData();

		__super::OnStart();
		m_isSecondary = false;

		return;
	}
	m_isSecondary = false;
	// �ǳ����ٴε�¼����Ҫ���õ�һ��������
	if(!theAPP_SESSION.HasUserLogin()){
		theAPP_SESSION.SetUserLoginStatus(true);
		// ���ó�ʼ������
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(2);
		// ÿ�ε�½����Ҫ������ʾ���˵�
		m_DataModel.secondlist = NULL;
	}	
	// �������F3�˳���ֱ����ʾ������
	if(!theAPP_SESSION.IsReturnMaintainMenu()){
		m_DataModel.secondlist = NULL;
	}
	InitData();
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceSvc::Model& CMaintenanceSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMaintenanceSvc::OnKeyboardEnter(WPARAM wParam, LPARAM lParam)
{
	__super::OnKeyboardEnter(wParam, lParam);

	theAPP_SESSION.SetKeyboard(KEY_NOBODY);
	ExecuteService();
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMaintenanceSvc::OnKeyboardF3(WPARAM wParam, LPARAM lParam)
{
	__super::OnKeyboardF3(wParam, lParam);

	// �����˵�����һ���˵�
	if (m_DataModel.secondlist != NULL)
	{
		m_DataModel.secondlist = NULL;
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F6_ESC2));
		NotifyDataModelChanged();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [ESC] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMaintenanceSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam){
	// �����˵�ESC�������˵�
	if(m_DataModel.secondlist != NULL){
		m_DataModel.secondlist = NULL;
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F6_ESC2));
		NotifyDataModelChanged();
	}
	else{// һ���˵��ǳ�
		__super::OnKeyboardEsc(wParam,lParam);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����ҳ�������

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceSvc::InitData()
{
	// ��ȡ�˵��б�
	m_DataModel.firstlist = theMENU_MGR.GetMaintainMenuItems();
	// ��ȡ���ز˵����
	SERVICE_ID svcid = theAPP_SESSION.ResetReturnSeviceID();
	if (m_isReturn)
	{
		// ��ȡ�����˵�
		if (svcid != UNKNOWN_SVC)
		{
			CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
			CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
			Menu_Item* menu = dlg->SearchService(svcid);
			if (menu->pParent != NULL)
				m_DataModel.secondlist = menu->pParent;
			else
				m_DataModel.secondlist = NULL;
		}
		m_isReturn = false;
		return;
	}
	if (m_isSecondary && svcid != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC)
	{
		svcid = theAPP_SESSION.GetMaintainMenu();
		vector<Menu_Item*>& menulist = m_DataModel.firstlist;

		// ������ǰ�˵�
		int index = 0;
		Menu_Item* menu = NULL;
		vector<Menu_Item*>::iterator iter = menulist.begin();
		for (; iter != menulist.end(); ++iter)
		{
			// ���ز˵��������������TVM/AVM�����йأ�
			if (!(*iter)->isVisible || !(*iter)->isEnable)
				continue;
			menu = *iter;
			// �ۼӲ˵���ţ��жϲ˵�����Ƿ����
			if (svcid == menu->serviceID)
			{
				// �жϵ�ǰ���Ƿ�������Ȩ����أ�
				if (!(*iter)->isEnable)
				{
					theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ACCOUNT_ERROR_SHORT));
					return;
				}
				else
				{
					m_DataModel.secondlist = menu;
					break;
				}
			}
		}
		return;
	}
	theAPP_SESSION.SetMaintainMenu(MAINTENANCE_SVC);
	// ��ȡ�����˵�
	if (svcid != UNKNOWN_SVC)
	{
		CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
		Menu_Item* menu = dlg->SearchService(svcid);
		if (menu->pParent != NULL)
			m_DataModel.secondlist = menu->pParent;
		else
			m_DataModel.secondlist = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ�д���

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceSvc::ExecuteService()
{
	// ��ȡ�������������
	int menucode = theAPP_SESSION.GetInputToInt();
	theAPP_SESSION.DeleteAllInput();

	// �жϵ�ǰ��һ���˵����Ƕ����˵�
	vector<Menu_Item*>& menulist = (m_DataModel.secondlist == NULL) ?
		m_DataModel.firstlist : m_DataModel.secondlist->children;

	// ������ǰ�˵�
	int index = 0;
	Menu_Item* menu = NULL;
	vector<Menu_Item*>::iterator iter = menulist.begin();
	for (; iter != menulist.end(); ++iter)
	{
		// ���ز˵��������������TVM/AVM�����йأ�
		if (!(*iter)->isVisible || !(*iter)->isEnable)
			continue;

		// �ۼӲ˵���ţ��жϲ˵�����Ƿ����
		if (menucode == ++index)
		{
			// �жϵ�ǰ���Ƿ�������Ȩ����أ�
			if (!(*iter)->isEnable)
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ACCOUNT_ERROR_SHORT));
				return;
			}
			else
			{
				menu = *iter;
				break;
			}
		}
	}

	// �������˵����
	if (menu == NULL)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	m_isSecondary = true;//add
	if (!m_isSecondary && 
		(menu->serviceID == BANKNOTE_BOX_MANAGE_SVC || 
		menu->serviceID == COIN_BOX_MANAGE_SVC ||
		menu->serviceID == ONE_KEY_SETTLEMENT_SVC))
	{
		m_isSecondary = true;
		theSERVICE_MGR.StartService(LOGIN_SVC);
		theAPP_SESSION.SetMaintainMenu(menu->serviceID);
		return;
	}
	// �ж��Ƿ�����Ӳ˵�
	if (menu->children.empty())
	{
		theSERVICE_MGR.StartService(menu->serviceID);
	}
	else
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
		m_DataModel.secondlist = menu;
		NotifyDataModelChanged();
	}
}
