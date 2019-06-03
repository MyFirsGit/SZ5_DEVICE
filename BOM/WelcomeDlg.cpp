#include "stdafx.h"

#include "WelcomeDlg.h"
#include "InnerException.h"
#include "ExceptionMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const STR_WELCOME = _T("��ӭʹ��");

IMPLEMENT_DYNAMIC(CWelcomeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CWelcomeDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService  Dialog��Ӧ��Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeDlg::CWelcomeDlg(CService* pService)
: COperationDlg(CWelcomeDlg::IDD, pService)
{
	try {
		UIINFO uiLableInfo;
		uiLableInfo.m_ForeColor = BLACK;
		uiLableInfo.m_Font.lfHeight *= 3;
		uiLableInfo.m_pParentWnd = this;
		uiLableInfo.m_Location = LEFTUP_AREA_POINT + CPoint(160,100);
		lblMessage = new CLable(uiLableInfo,STR_WELCOME);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeDlg::~CWelcomeDlg()
{
	delete lblMessage;
	lblMessage = NULL;	                     
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CWelcomeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try {
		// ����������Ϣ����ȷ����ť�����ּ���
		if (__super::OnCreate(lpCreateStruct,GUIDE_AREA) != 0) {
			return -1;
		}
		lblMessage->Create();
	}
	catch (CSysException& e) {
		//theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		//theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}
