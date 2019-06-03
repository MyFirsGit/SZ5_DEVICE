#include "stdafx.h"
#include "CSaleStaticBaseOfficeDlg.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSaleStaticBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CSaleStaticBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticBaseOfficeDlg::CSaleStaticBaseOfficeDlg(CService* pService)
	: COperationDlg(CSaleStaticBaseOfficeDlg::IDD, pService)
{
	CSaleStaticSvc* pCSaleStaticSvc = (CSaleStaticSvc*)GetService();
	// ��Ϣ��ʾ����
	m_baseInfo->titleInfo.titleName = "";                    // ����
 	m_baseInfo->detailInfo.row = 8;           // ����
 	m_baseInfo->detailInfo.labelGroup = 0;     // ��ǩ���֣�3�У�
 	m_baseInfo->detailInfo.editGroup.cnt = 0;                               // �༭����Ŀ
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticBaseOfficeDlg::~CSaleStaticBaseOfficeDlg()
{
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
int CSaleStaticBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( -1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA  )) {
		return -1;
	}
	CSaleStaticSvc* pCSaleStaticSvc = (CSaleStaticSvc*)GetService();
	// ����ҵ�񲿷ֺ�����������ʾ��Ϣ����
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo, pCSaleStaticSvc->GetAuditInfoGroup());

   
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i) CDC* pDC  �豸������ָ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CSaleStaticBaseOfficeDlg::OnDraw(CDC* pDC)
{
    CSaleStaticSvc* pCSaleStaticSvc = (CSaleStaticSvc*)GetService();

    try {
	    __super::OnDraw(pDC);
	}
	catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(pCSaleStaticSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}
