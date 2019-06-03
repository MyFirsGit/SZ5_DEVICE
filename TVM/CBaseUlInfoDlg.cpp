// CBaseUlInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TVM.h"
#include "CBaseUlInfoDlg.h"
#include "afxdialogex.h"
#include "AnalyzeSvc.h"

// CBaseUlInfoDlg dialog

IMPLEMENT_DYNAMIC(CBaseUlInfoDlg, CReceptionDlg)
BEGIN_MESSAGE_MAP(CBaseUlInfoDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CBaseUlInfoDlg::CBaseUlInfoDlg(CService* pService)
	: CReceptionDlg(CBaseUlInfoDlg::IDD, pService)
{
	m_pSvc = (CAnalyzeSvc*)pService;
	m_baseInfo->titleInfo.titleName = _tl(CAnalyzeSvc::UE_ANA_BASEULTITLE);                    // ����
	m_baseInfo->stytle = PASSAGE_WIDE_STYLE;
	m_baseInfo->detailInfo.row = UD_MAX_ANAROWS;
	m_baseInfo->detailInfo.labelGroup = m_UlInfoLable;	

	createFunBtns();
}

CBaseUlInfoDlg::~CBaseUlInfoDlg()
{
}

void CBaseUlInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���洴������
//
//@param      (i)LPCREATESTRUCT lpCreateStruct  
//
//@retval     int    \n
//             0:�����ɹ�    -1:����ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
int CBaseUlInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
    try{    		
		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA ) == -1) {
			return -1;
		}
    }
    // �쳣����
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
         theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

void CBaseUlInfoDlg::createFunBtns()
{
	m_BTN_GROUP_INFO[0].rows = 1;
	m_BTN_GROUP_INFO[0].cols = 1;

	m_vec_Right_1_BTN_INFO.clear();

	BTN_INFO btnInfo;
	btnInfo.text=_tl(CAnalyzeSvc::UE_ANA_BASEIINFOTITLE);

	m_vec_Right_1_BTN_INFO.push_back(btnInfo);
}

void CBaseUlInfoDlg::updataBaseUlInfo()
{
	/*
	CAnalyzeSvc::DataModel model = m_pSvc->GetDataModel();
	long lIndex=0;

	//ticket ID
	m_UlInfoLable[lIndex].nameLeft = _tl(CAnalyzeSvc::UE_ANA_TICKID);
	m_UlInfoLable[lIndex++].nameRight = model.ulBaseInfo.baseInfo.tcTicketID;

	//ticket type
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_TICKTYPE);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcTicketType;

	//expire data
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_EXPIREDATA);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcExpireData;

	//passage type
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_PASSAGETYPE);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcPassageType;

	//original
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_ORIGINAL);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcOriginal;

	//destination
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_DESTINATION);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcDestination;

	//remain value
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_REMAINVALUE);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcRemainValue;

	//ticket status
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_TICKETSTATUS);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcTicketStatus;

	//active status 
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_ACTIVESTATUS);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.tcActiveStatus;

	//journey status
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_JOUNERYSTATUS);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcJouneryStatus;

	//last trans time
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_LASTTRANSTIME);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcLastTransTime;

	//last trans location
	m_UlInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_LASTTRANSLOCATION);
	m_UlInfoLable[lIndex++].nameRight=model.ulBaseInfo.baseInfo.tcLastTransLocation;
	*/
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      ���»��溯��
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseUlInfoDlg::UpdateUI()
{
	updataBaseUlInfo();
	m_baseInfoArea->Invalidate();
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���漤���Ĵ���
//
//@param      (i)UINT nState      ��ʹ��
//@param      (i)CWnd* pWndOther  ��ʹ��
//@param      (i)BOOL bMinimized  ��ʹ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseUlInfoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{	
	__super::OnActivate(nState,pWndOther,bMinimized);
}