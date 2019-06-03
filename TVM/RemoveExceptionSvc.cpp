#include "stdafx.h"
#include "RemoveExceptionSvc.h"
#include "RemoveExceptionDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRemoveExceptionSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::CRemoveExceptionSvc():CTVMForeService(REMOVE_EXCEPTION_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20701_REMOVE_EXCEPTION_DLG, new CRemoveExceptionDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION,_opl(GUIDE_REMOVE_EXCEPTION_INIT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REMOVE_EXCEPTION, IDD_20701_REMOVE_EXCEPTION_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);

	//��ʼ��
	//InitExceptionData();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::~CRemoveExceptionSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionSvc::OnStart()
{
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);
	//m_modol = Modol();
	//��ʼ��
	InitExceptionData();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_REMOVE_EXCEPTION_INIT);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����ģ��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::Modol& CRemoveExceptionSvc::GetDataModol()
{
	return m_modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���´�����ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ�ϰ������쳣�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRemoveExceptionSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	try {
		theSERVICE_MGR.SetForeServiceBusy(true);
		//1.�쳣���
		theEXCEPTION_MGR.RepareException();
		// �����쳣����
		InitExceptionData();
		//2.ˢ���쳣�б�
		NotifyDataModelChanged();
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_REMOVE_EXCEPTION_FINISHED));
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		//theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_OUT_DEVICE_SELECT));
	}
	catch (...) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__)); 
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRemoveExceptionSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ���쳣����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionSvc::InitExceptionData()
{
	m_modol.m_ListException.clear();
	//��ʼ������
	m_modol.m_ListException = theEXCEPTION_MGR.GetAllException();          //��ȡ���е��쳣��
	m_modol.m_ExceptionNum  = m_modol.m_ListException.size();              //�쳣��Ĵ�С

	if (m_modol.m_ExceptionNum == 0)
	{
		SetPageInfo(0,1,_MAX_ROW-1);
	}
	else
	{
		int nTotalPage = m_modol.m_ExceptionNum / (_MAX_ROW - 1)  ? m_modol.m_ExceptionNum / (_MAX_ROW - 1) : 1;//�쳣���ҳ��
		SetPageInfo(nTotalPage,1,_MAX_ROW-1);
	}
}