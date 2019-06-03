#include "stdafx.h"
#include "SelectLanguageSvc.h"
#include "SelectLanguageBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSelectLanguageSvc,CTVMForeService)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageSvc::CSelectLanguageSvc()
	:CTVMForeService(WAIT_MAINTENANCE_SVC)
{
	// ����Ĭ������
	theAPP_SESSION.SetCurrentOperatorLanguageType(LANGUAGE_ID_CHINESE);
	// �л��˵�����
	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);

    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG, new CSelectLanguageBaseDlg(this));

    // ���ó˿ͻ���

    // ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CSelectLanguageBaseDlg::IDD);

    // ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageSvc::~CSelectLanguageSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageSvc::Model& CSelectLanguageSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
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
LRESULT CSelectLanguageSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// ��ȡ��������
	int index = theAPP_SESSION.GetInputToInt();

	// ת��Ϊ��������
	LANGUAGETYPE_T language;
	switch (index)
	{
	case 1:
		language = LANGUAGE_ID_CHINESE;
		break;
	case 2:
		language = LANGUAGE_ID_ENGLISH;
		break;
		/*case 3:
		language = LANGUAGE_ID_PORTUGUESE;
		break;*/
	default:
		language = LANGUAGE_ID_UNSPECIFIED;
	}

	// ��֤�������ݣ��������ԣ���������½ҵ��
	if (language != LANGUAGE_ID_UNSPECIFIED)
	{
		theAPP_SESSION.SetCurrentOperatorLanguageType(language);
		theSERVICE_MGR.StartService(LOGIN_SVC);
	}
	else
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	}

	return TRUE;
}
