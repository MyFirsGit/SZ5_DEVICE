#include "stdafx.h"
#include "TimeSetBaseOfficeDlg.h"
#include "TimeSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTimeSetBaseOfficeDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CTimeSetBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::CTimeSetBaseOfficeDlg(CService* pService)
	:COperationDlg(CTimeSetBaseOfficeDlg::IDD, pService)
{
	m_baseInfo->detailInfo.labelGroup = m_labelGroup;           
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::~CTimeSetBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int  0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTimeSetBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)){
		return -1;
	}
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, RESOLUTION * 200);				// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, RESOLUTION * 400);				// ���õڶ��п��

	//InitlabelTimeInfo();											// ��ʼ��BASE����LABEL���������
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���漤��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelTimeInfo();											// ��ʼ��BASE����LABEL���������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::InitlabelTimeInfo()
{
	_DATE_TIME currentTime = ComGetCurTime();
	m_labelGroup[0].nameLeft = _T("[1] ")+_opl(TTILE_SET_TIME);
	m_labelGroup[0].nameRight = currentTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::UpdateUI()
{
	InitlabelTimeInfo();
	CTimeSetSvc* pSvc = (CTimeSetSvc*)GetService();
	CTimeSetSvc::Model& model = pSvc->GetDataModel();
	if(1 == model.TimeChange)
	{
		ShowCodeContent();			//������Ϣ����ʾ��Ӧ�����õ�����
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����ʾ��Ӧ�����õ�����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CTimeSetBaseOfficeDlg::ShowCodeContent()
{
	CTimeSetSvc* pSvc = (CTimeSetSvc*)GetService();
	CTimeSetSvc::Model& model = pSvc->GetDataModel();
	if(FALSE == model.TimeLegal)
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_COMMUNICATION_SET_INPUT_ERROR);
		return ;
	}
	else
	{
		CString TimeStr = _T("");
		TimeStr.Format(_T("%s-%s-%s %s:%s:%s"),model.setYear,model.setMonth,model.setDay,model.setHour,model.setMinute,model.setSecond);
		m_labelGroup[0].nameRight = TimeStr;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

