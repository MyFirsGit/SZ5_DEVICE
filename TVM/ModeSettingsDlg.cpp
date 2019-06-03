#include "StdAfx.h"
#include "ModeSettingsDlg.h"

IMPLEMENT_DYNAMIC(CModeSettingsDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CModeSettingsDlg, COperationDlg)
		ON_WM_CREATE()
		ON_WM_ACTIVATE()
	END_MESSAGE_MAP()

CModeSettingsDlg::CModeSettingsDlg(CService *pSvc)
	:COperationDlg(CModeSettingsDlg::IDD,pSvc)
{
	m_data = ((CModeSettingsSvc*)pSvc)->GetDataModel();
}


CModeSettingsDlg::~CModeSettingsDlg(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���洴������
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CModeSettingsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);
	// ��ͬ�������µİ�������
	if(((CModeSettingsSvc*)GetService())->GetCurDialogGroupID() == CModeSettingsSvc::DLG_GROUP::DLG_GROUP_BASE )
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	else
		theAPP_SESSION.SetKeyboard(KEY_F2 | KEY_INPUT | KEY_RETURN);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���漤��
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	InitOfficeGuide();
	CModeSettingsSvc *pSvc = (CModeSettingsSvc*)GetService();
	//�������볤��
	if (pSvc->GetCurDialogGroupID() == CModeSettingsSvc::DLG_GROUP::DLG_GROUP_BASE )
	{
		theAPP_SESSION.SetMaxInputLength(1);
		theAPP_SESSION.SetKeyboard( KEY_INPUT | KEY_RETURN);
	}else{
		theAPP_SESSION.SetMaxInputLength(2);
		theAPP_SESSION.SetKeyboard(KEY_F2 | KEY_INPUT | KEY_RETURN);
	}
	
	InitDialog();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �������
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsDlg::UpdateUI()
{
	ShowDialog();
	m_baseInfoArea->Invalidate(TRUE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʾ��������
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsDlg::ShowDialog()
{
	CModeSettingsSvc *pSvc = (CModeSettingsSvc*)GetService();
	InitDialog();
	m_data = pSvc->GetDataModel();
	int nSize = m_data.mapItem.size();
	CString strState(_opl(TXT_MODE_SETTINGS_MODE_ON));
	// �����洢�˽������ݵ�����
	for ( int n = 0; n <  nSize ;n++ )
	{
		m_lableGroup[n].name1.Format(_T("[%d] %s"),n+1,(m_data.mapItem[n+1]).strItem);
		if(pSvc->GetCurDialogGroupID() != CModeSettingsSvc::DLG_GROUP::DLG_GROUP_BASE)
		{
			m_lableGroup[n].name2.Format(_T("[1]%s"),_opl(TXT_MODE_SETTINGS_MODE_ON));
			m_lableGroup[n].name3.Format(_T("[2]%s"),_opl(TXT_MODE_SETTINGS_MODE_OFF));
		}else{
			m_lableGroup[n].name2 = _T("");
			m_lableGroup[n].name3 = _T("");
			m_lableGroup[n].name4 = _T("");}
		switch((m_data.mapItem[n+1]).Statuse)
		{
		case CModeSettingsSvc::STATUS::ON: strState = _opl(TXT_MODE_SETTINGS_MODE_ON);break;
		case CModeSettingsSvc::STATUS::OFF: strState = _opl(TXT_MODE_SETTINGS_MODE_OFF);break;
		case CModeSettingsSvc::STATUS::UNDEFINE: strState = _T("");break;
		default: strState = _T("");break;
		}
		m_lableGroup[n].name4.Format(_T("%s"),strState);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ������
@param       
@param    

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CModeSettingsDlg::InitDialog()
{
	// �����п��
	m_baseInfoArea->SetLableAlign(LABEL_COL_1,DT_LEFT);
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 300);	

	m_baseInfoArea->SetLableAlign(LABEL_COL_2,DT_LEFT);
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 75);	

	m_baseInfoArea->SetLableAlign(LABEL_COL_3,DT_LEFT);
	m_baseInfoArea->SetLableWidth(LABEL_COL_3, 75);	

	m_baseInfoArea->SetLableAlign(LABEL_COL_4,DT_RIGHT);
	m_baseInfoArea->SetLableWidth(LABEL_COL_4, 100);	

	for (int i = 0; i < MAX_ROW; i++)
	{
		m_lableGroup[i].name1 = _T("");
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
	}
}

void CModeSettingsDlg::InitOfficeGuide()
{
	CModeSettingsSvc *pSvc = (CModeSettingsSvc*)GetService();
	switch(pSvc->GetCurDialogGroupID())
	{
	case CModeSettingsSvc::DLG_GROUP_BASE:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);break;
	case CModeSettingsSvc::DLG_GROUP_CHANGE:
	case CModeSettingsSvc::DLG_GROUP_PAYMENT:
	case CModeSettingsSvc::DLG_GROUP_PRINT:
	case CModeSettingsSvc::DLG_GROUP_SERVICE:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F2_F3_ESC);break;
	}
}