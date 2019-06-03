#include "stdafx.h"
#include "ScreenBrightnessSetSvc.h"
#include "ScreenBrightnessSetMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CScreenBrightnessSetDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CScreenBrightnessSetDlg,COperationDlg)
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
CScreenBrightnessSetDlg::CScreenBrightnessSetDlg(CService* pService)
	:COperationDlg(CScreenBrightnessSetDlg::IDD, pService)
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
CScreenBrightnessSetDlg::~CScreenBrightnessSetDlg()
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
int CScreenBrightnessSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)){
		return -1;
	}
	theAPP_SESSION.SetKeyboard(KEY_F2 | KEY_PAGE | KEY_RETURN);		// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 200);				// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 400);				// ���õڶ��п��

	//InitlabelScreenBrightnessInfo();											// ��ʼ��BASE����LABEL���������
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
void CScreenBrightnessSetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelScreenBrightnessInfo();											// ��ʼ��BASE����LABEL���������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CScreenBrightnessSetDlg::InitlabelScreenBrightnessInfo()
{
	CScreenBrightnessSetSvc* pSvc = (CScreenBrightnessSetSvc*)GetService();
	CScreenBrightnessSetSvc::Model& model = pSvc->GetDataModel();
	
	CString Brightness = _T("");
	Brightness.Format(_T("%d"),model.BrightnessNumber);
	m_labelGroup[0].nameLeft = _opl(GUIDE_SCREEN_BRIGHTNESS_SET);
	m_labelGroup[0].nameRight = Brightness;		// ��Ļ����Ĭ��Ϊ5
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CScreenBrightnessSetDlg::UpdateUI()
{
	ShowCodeContent();			//������Ϣ����ʾ��Ӧ�����õ�����
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

void CScreenBrightnessSetDlg::ShowCodeContent()
{
	CScreenBrightnessSetSvc* pSvc = (CScreenBrightnessSetSvc*)GetService();
	CScreenBrightnessSetSvc::Model& model = pSvc->GetDataModel();
	
	CString TimeStr = _T("");
	TimeStr.Format(_T("%d"),model.BrightnessNumber);
	m_labelGroup[0].nameRight = TimeStr;
}
