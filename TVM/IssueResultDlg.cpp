#include "stdafx.h"
#include "IssueResultDlg.h"
#include "IssueSvc.h"
#include "tickethelper.h"
#include "GuideInfo.h"


IMPLEMENT_DYNAMIC(CIssueResultDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueResultDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_BACK_CLICK(OnCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIssueResultDlg::CIssueResultDlg(CService* pService) : CReceptionDlg(CIssueResultDlg::IDD, pService)
{
	pSvc = (CIssueSvc*)pService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIssueResultDlg::~CIssueResultDlg()
{
	if (m_guideInfoArea!=NULL){
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����ʾ������Ϣ

@param      (i)long lComponents  �Զ��������Ϣ

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueResultDlg::ShowGuideArea()
{
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		m_guideInfoArea->Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueResultDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	CIssueSvc::Model& model = pSvc->GetDataModel();
	DWORD textSize = theGUI_INFO.GetRecTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	CString textFontName = theGUI_INFO.GetRecTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());
	// �쳣����
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		CRect m_error_msg_rc = CRect(50,280,450,380);
		DrawText(pDC, m_error_msg_rc,_T("����ʧ��"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_error_dealInfo_rc = CRect(50,400,450,500);
		DrawText(pDC, m_error_dealInfo_rc,_T("����ȡ����ƾ֤����ϵ����Ա"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
	}
	// ȡ������
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		CRect m_cancle_msg_rc = CRect(50,280,450,380);
		DrawText(pDC, m_cancle_msg_rc,_T("����ȡ��"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_msg_rc = CRect(50,400,250,500);
		DrawText(pDC, m_refund_msg_rc,_T("�˿��"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_amount_rc = CRect(280,400,450,500);
		DrawText(pDC, m_refund_amount_rc,ComFormatAmount(0)+_tl(MONEY_UNIT), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_dealInfo_rc = CRect(50,600,450,500);
		DrawText(pDC, m_refund_dealInfo_rc,_T("���պ������ֽ�"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
	}
	// ���㳬�޻���
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
		CRect m_cancle_msg_rc = CRect(50,280,450,380);
		DrawText(pDC, m_cancle_msg_rc,_T("����"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_msg_rc = CRect(50,400,250,500);
		DrawText(pDC, m_refund_msg_rc,_T("�˿��"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_amount_rc = CRect(280,400,450,500);
		DrawText(pDC, m_refund_amount_rc,ComFormatAmount(0)+_tl(MONEY_UNIT), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		CRect m_refund_dealInfo_rc = CRect(50,600,450,500);
		DrawText(pDC, m_refund_dealInfo_rc,_T("���պ������ֽ�"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
	}
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���洴������
//
//@param      (i)LPCREATESTRUCT lpCreateStruct
//
//@retval     int    \n
//			0:�����ɹ�    -1:����ʧ��
//
//@exception  ��
//*/
//////////////////////////////////////////////////////////////////////////
int CIssueResultDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea ->createWnd(this);

	theSERVICE_MGR.SetState(RECEPT);
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CIssueResultDlg::UpdateUI()
{
	CIssueSvc::Model& model = pSvc->GetDataModel();

	// �쳣����
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_TICKET_INFO);
		m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
		ShowGuideArea();
	}
	// ȡ������
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_TICKET_INFO);
		m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
		ShowGuideArea();
	}

	// ���㳬�޻���
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
		m_guideInfo->tipsCount = 1;
		m_guideInfo->detailList[0].bitInfo =  _tl(TXT_TICKET_INFO);
		m_guideInfo->detailList[0].strfileName = _T(".\\res\\SJT_issue.png");
		ShowGuideArea();
	}

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
void CIssueResultDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	m_btnOK->SetSelect(false);

	/*theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);*/
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȡ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueResultDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	pSvc->OnCancel();
	return 0;
}