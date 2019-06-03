#include "stdafx.h"
#include "CardTicketBoxInfoDlg.h"
#include "CTCardCountInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTicketBoxInfoDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CTicketBoxInfoDlg, COperationDlg)
	ON_WM_ACTIVATE()   
	ON_WM_CREATE()
END_MESSAGE_MAP()

#define  GREEN_OF_INSTALL	RGB(0,100,0)
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���캯��
//
//@param      (i)CService* pService
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
CTicketBoxInfoDlg::CTicketBoxInfoDlg(CService* pService)
: COperationDlg(CTicketBoxInfoDlg::IDD,pService)
{
	
	//m_baseInfo->detailInfo.labelGroup = lablelGroups;
	//m_pSvc	= (CTicketBoxExchangeSvc *)pService;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������
//
//@param      none
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
CTicketBoxInfoDlg::~CTicketBoxInfoDlg()
{
	if (m_baseInfoArea != NULL){
		delete m_baseInfoArea;
		m_baseInfoArea = NULL;
	}
}



////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���漤���Ĵ���
//
//@param      (i)UINT nState      ��ʹ��
//@param      (i)CWnd* pWndOther  ��ʹ��
//@param      (i)BOOL bMinimized  ��ʹ��
//
//@retval     void
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);



	theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_RETURN|KEY_F2);
	theAPP_SESSION.SetMaxInputLength(2);
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���洴������
//
//@param      (i)LPCREATESTRUCT lpCreateStruct  
//
//@retval     int    \n
//0:�����ɹ�    -1:����ʧ��
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
int CTicketBoxInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
        return -1;
    }
	m_baseInfoArea = new CGStatic(this, m_baseInfo, lablelGroups);

	// ����GStatic
	for (int n(0); n<4; n++)
	{
		m_baseInfoArea->SetLableWidth(n, 0, RESOLUTION * 200);
		m_baseInfoArea->SetLableWidth(n, 1, RESOLUTION * 180);
		m_baseInfoArea->SetLableWidth(n, 2, RESOLUTION * 120);
		m_baseInfoArea->SetLableWidth(n, 3, RESOLUTION * 140);
	}
	for ( int m(5); m <9; m ++)
	{
		m_baseInfoArea->SetLableWidth(m,0,RESOLUTION * 300);
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief      ������ʾ��ɻص�����
//
//@param      (i)��
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::UpdateUI()
{	
	CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
	//CTicketBoxExchangeSvc::DATA_MODEL model = pSvc->getModel();
	UINT dlg_id = pSvc->GetCurDialogGroupID();
	switch(dlg_id)
	{
	case CTicketBoxExchangeSvc::DLG_BASE: 
		UpdateUIforStart();
		break;
	case CTicketBoxExchangeSvc::DLG_INSTALL:
		UpdateUIforInstall();
		break;
	case CTicketBoxExchangeSvc::DLG_REMOVE:
		UpdateUIForRemoval();
		break;
	}
	m_baseInfoArea->Invalidate();
	return ;
}

///get
void CTicketBoxInfoDlg::UpdateUIforStart()
{
	ClearUI();
	lablelGroups[0].name1 = _T("[1] ") + _opl(UE_ATTACH);
	lablelGroups[1].name1 = _T("[2] ") + _opl(UE_DETACH);
	theAPP_SESSION.AppendMenuLevel(_T(""));
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
}

///get
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ʼ��Ʊ�䰲װ����
//
//@param      (i)��
//
//@retval    
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::UpdateUIforInstall()
{

	theAPP_SESSION.AppendMenuLevel(_opl(UE_ATTACH));

	CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DATA_MODEL model = pSvc->getModel();
	ClearUI();

	int nLine = 0;
	lablelGroups[nLine].name1 = _T("Ʊ��A");
	lablelGroups[nLine].name2 = model.BoxAinfo.szID;
	lablelGroups[nLine].name3 = model.BoxAinfo.szQuantity;
	lablelGroups[nLine].name4 = model.BoxAinfo.GetStateToString();
	m_baseInfoArea->SetLabelColor(nLine,3,model.BoxAinfo.GetColorForState());

	lablelGroups[++nLine].name1 = _T("Ʊ��B");
	lablelGroups[nLine].name2 = model.BoxBinfo.szID;
	lablelGroups[nLine].name3 = model.BoxBinfo.szQuantity;
	lablelGroups[nLine].name4 = model.BoxBinfo.GetStateToString();
	m_baseInfoArea->SetLabelColor(nLine,3,model.BoxBinfo.GetColorForState());

	lablelGroups[++nLine].name1 = _T("��Ʊ��");
	lablelGroups[nLine].name2 = model.InvalidBox.szID;
	lablelGroups[nLine].name3 = model.InvalidBox.szQuantity;
	lablelGroups[nLine].name4 = model.InvalidBox.GetStateToString(); // ��Ʊ���������ʾ����ʾ
	m_baseInfoArea->SetLabelColor(nLine,3,model.InvalidBox.GetColorForState());

	nLine += 3;	// ������

	lablelGroups[nLine].name1 = _T("[1] ��װƱ��A");
	lablelGroups[++nLine].name1 = _T("[2] ��װƱ��B");
	lablelGroups[++nLine].name1 = _T("[3] ��װ��Ʊ��");
	lablelGroups[++nLine].name1 = _T("[4] ��װ����Ʊ��");
}

///get
// ��ʼ��Ʊ��ж�ؽ���
void CTicketBoxInfoDlg::UpdateUIForRemoval()
{
	theAPP_SESSION.AppendMenuLevel(_opl(UE_DETACH));
	ClearUI();
	
	int nLine = 0;
	CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DATA_MODEL model = pSvc->getModel();
	lablelGroups[nLine].name1 = _T("Ʊ��A");
	lablelGroups[nLine].name2 = model.BoxAinfo.szID;
	lablelGroups[nLine].name3 = model.BoxAinfo.szQuantity;
	lablelGroups[nLine].name4 = model.BoxAinfo.GetStateToString();
	m_baseInfoArea->SetLabelColor(nLine,3,model.BoxAinfo.GetColorForState());


	lablelGroups[++nLine].name1 = _T("Ʊ��B");
	lablelGroups[nLine].name2 = model.BoxBinfo.szID;
	lablelGroups[nLine].name3 = model.BoxBinfo.szQuantity;
	lablelGroups[nLine].name4 = model.BoxBinfo.GetStateToString();
	m_baseInfoArea->SetLabelColor(nLine,3,model.BoxBinfo.GetColorForState());


	lablelGroups[++nLine].name1 = _T("��Ʊ��");
	lablelGroups[nLine].name2 = model.InvalidBox.szID;
	lablelGroups[nLine].name3 = model.InvalidBox.szQuantity;
	lablelGroups[nLine].name4 = model.InvalidBox.GetStateToString();
	m_baseInfoArea->SetLabelColor(nLine,3,model.InvalidBox.GetColorForState());

	/*lablelGroups[++nLine].name1 = _T("������");
	CString strBuffer = _T("");
	if (pSvc->m_buffer_result ==0 )	{
	strBuffer = _T("");
	}else if (pSvc->m_buffer_result == 1)
	{
	strBuffer = _T("������");
	}else if (pSvc->m_buffer_result == 2){
	strBuffer = _T("������");
	}else{
	strBuffer = _T("����ʧ��");
	}
	lablelGroups[nLine].name4 = strBuffer*/;

	nLine += 2;	// ������

	lablelGroups[++nLine].name1 = _T("[1] ж��Ʊ��A");
	lablelGroups[++nLine].name1 = _T("[2] ж��Ʊ��B");
	lablelGroups[++nLine].name1 = _T("[3] ж�ط�Ʊ��");
	//lablelGroups[++nLine].name1 = _T("[4] ���ջ�����Ʊ��");
	lablelGroups[++nLine].name1 = _T("[4] ж������Ʊ��");
}


void CTicketBoxInfoDlg::ClearUI()
{
	for (int i(0); i<UD_MAX_ROW; i++)
	{
		lablelGroups[i].name1 = _T("");
		lablelGroups[i].name2 = _T("");
		lablelGroups[i].name3 = _T("");
		lablelGroups[i].name4 = _T("");
	}
}

