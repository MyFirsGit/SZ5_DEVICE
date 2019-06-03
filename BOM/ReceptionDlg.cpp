#include "StdAfx.h"
#include "ReceptionDlg.h"
#include "appsession.h"
#include "InnerException.h"
#include "exceptionmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static const UINT const REC_EN_FONT_SIZE3 = 14;

IMPLEMENT_DYNAMIC(CReceptionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CReceptionDlg, CBaseDlg)
    ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���캯��

 @param      (i)UINT ID             �Ի���ID
 @param      (i)CService* pService  �Ի���ʹ�õ�Service

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg::CReceptionDlg(UINT ID, CService* pService)
	: CBaseDlg(ID, NULL, REC_BODY_RECT, RECEPTION_BK_COLOR, pService, false)
{
	m_BusinessArea = NULL;
    m_BusinessInfo = new MSG_INFO;

    m_BusinessInfo->titleName.nameChinese = theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pService->GetServiceID());
	m_BusinessInfo->titleName.nameEnglish = theAPP_SESSION.GetServiceNameEN((SERVICE_ID)pService->GetServiceID());

}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������

 @param      ��

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg::~CReceptionDlg(void)
{
    delete m_BusinessInfo;
    m_BusinessInfo = NULL;

    delete m_BusinessArea;
    m_BusinessArea = NULL;
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
int CReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulAreaFlag)
{
    try {
	    if (__super::OnCreate(lpCreateStruct) == -1) {
		    return -1;
	    }

         //������������
        if (ulAreaFlag & BUSINESS_AREA) {
            m_BusinessArea = new CReceptionStaticInfo(this, m_BusinessInfo);
        }
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
         theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ������ƺ���

 @param      (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnDraw(CDC* pDC)
{

	__super::OnDraw(pDC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(theSERVICE_MGR.GetState()!=READY)
	{
		// ��ʾ�˿ͻ����������
		theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_SHOW);
		theAPP_SESSION.GetChildViewRec()->m_pReceptionGuideDlg->ShowWindow(SW_SHOW);

		// ���ء���ӭ���١�����
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->ShowWindow(SW_HIDE);
		__super::OnActivate(nState,pWndOther,bMinimized);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ���

@param      (i)ulong ulCompomentFlag           �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ���

@param      (i)CWnd *pCompoment                �������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i)ulong ulCompomentFlag           �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i)CWnd *pCompoment                �������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ/�������

@param      (i)ulong ulCompomentFlag           �����־
@param      (i)int nCmdShow                    ����(SW_SHOW/SW_HIDE)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
    if (ulCompomentFlag & BUSINESS_AREA) {
        ShowHideCompoment(m_BusinessArea, nCmdShow);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ/�������

@param      (i)CWnd *pCompoment                �������
@param      (i)int nCmdShow                    ����(SW_SHOW/SW_HIDE)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(CWnd *pCompoment, int nCmdShow)
{
    CRect rect;

    if (pCompoment != NULL) {
        pCompoment->ShowWindow(nCmdShow);

        GetCompomentRect(pCompoment, &rect);

        InvalidateRect(&rect, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��������ڵ�����

@param      (i)CWnd *pCompoment                �������
@param      (o)CRect* lpRect                   ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);

    // ת��������ڵ�ǰOperationDlg��λ��
    ScreenToClient(lpRect);

    // ���ϱ߿�Ĵ�С
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);
}
