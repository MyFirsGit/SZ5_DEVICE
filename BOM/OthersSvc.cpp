#include "stdafx.h"
#include "BOM.h"
#include "OthersSvc.h"
#include "WelcomeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COthersSvc::COthersSvc()
:CBOMForeService(OTHERS_SVC)
{
    // ���ò���Ա����
 //   m_pDialogFlow->AddDialog(CWelcomeDlg::IDD, new CWelcomeDlg(this));

 //   // ���ó˿ͻ���

 //   // ���û������
 //   m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, "");
 //   m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CWelcomeDlg::IDD);

 //   // ���ó�ʼ������
	//m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COthersSvc::~COthersSvc()
{
}