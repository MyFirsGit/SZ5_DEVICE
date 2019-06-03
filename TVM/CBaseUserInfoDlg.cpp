//// CBaseUserInfoDlg.cpp : implementation file
////
//
#include "stdafx.h"
//#include "TVM.h"
//#include "CBaseUserInfoDlg.h"
//#include "afxdialogex.h"
//#include "AnalyzeSvc.h"
//
//
//// CBaseUserInfoDlg dialog
//
//IMPLEMENT_DYNAMIC(CBaseUserInfoDlg, CReceptionDlg)
//BEGIN_MESSAGE_MAP(CBaseUserInfoDlg, CReceptionDlg)
//	ON_WM_CREATE()
//	ON_WM_ACTIVATE()
//END_MESSAGE_MAP()
//
//CBaseUserInfoDlg::CBaseUserInfoDlg(CService* pService)
//	: CReceptionDlg(CBaseUserInfoDlg::IDD, pService)
//{
//	m_pSvc = (CAnalyzeSvc*)pService;
//	m_baseInfo->titleInfo.titleName = _tl(CAnalyzeSvc::UE_ANA_USERINFOTITLE);                    // 标题
//	m_baseInfo->stytle = PASSAGE_WIDE_STYLE;
//	m_baseInfo->detailInfo.row = UD_MAX_ANAROWS;
//	m_baseInfo->detailInfo.labelGroup = m_UserInfoLable;
//
//	createFunBtns();
//}
//
//CBaseUserInfoDlg::~CBaseUserInfoDlg()
//{
//}
//
//void CBaseUserInfoDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}
//
//////////////////////////////////////////////////////////////////////////////
/////**
////@brief      画面创建函数
////
////@param      (i)LPCREATESTRUCT lpCreateStruct  
////
////@retval     int    \n
////             0:创建成功    -1:创建失败
////
////@exception  无
////*/
//////////////////////////////////////////////////////////////////////////////
//int CBaseUserInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//    CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
//    try{    		
//		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA ) == -1) {
//			return -1;
//		}
//    }
//    // 异常处理
//    catch (CSysException& e) {
//        theEXCEPTION_MGR.ProcessException(e);
//    }
//    catch (...) {
//         theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//    }
//    return 0;
//}
//
//////////////////////////////////////////////////////////////////////////////
/////*
////@brief      更新画面函数
////
////@param      无
////
////@retval     无
////
////@exception  无
////*/
//////////////////////////////////////////////////////////////////////////////
//void CBaseUserInfoDlg::UpdateUI()
//{
//	m_baseInfoArea->Invalidate();
//}
//
//////////////////////////////////////////////////////////////////////////////
/////**
////@brief      画面激活后的处理
////
////@param      (i)UINT nState      不使用
////@param      (i)CWnd* pWndOther  不使用
////@param      (i)BOOL bMinimized  不使用
////
////@retval     无
////
////@exception  无
////*/
//////////////////////////////////////////////////////////////////////////////
//void CBaseUserInfoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{	
//	__super::OnActivate(nState,pWndOther,bMinimized);
//}
//
//void CBaseUserInfoDlg::updataBaseUserInfo()
//{
//	CAnalyzeSvc::DataModel model = m_pSvc->GetDataModel();
//	long lIndex=0;
//	//Name
//	m_UserInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_NAME);
//	m_UserInfoLable[lIndex++].nameRight=model.userInfo.tcName;
//	//id type
//	m_UserInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_IDENTITYCARDTYPE);
//	m_UserInfoLable[lIndex++].nameRight=model.userInfo.tcIdentityCardType;
//	//id num
//	m_UserInfoLable[lIndex].nameLeft=_tl(CAnalyzeSvc::UE_ANA_IDENTITYCARDNUM);
//	m_UserInfoLable[lIndex++].nameRight=model.userInfo.tcIdentityCardNum;
//}
//
//void CBaseUserInfoDlg::createFunBtns()
//{
//	m_BTN_GROUP_INFO[0].rows = 1;
//	m_BTN_GROUP_INFO[0].cols = 3;
//
//	m_vec_Right_1_BTN_INFO.clear();
//
//	BTN_INFO btnInfo;
//	btnInfo.text=_tl(CAnalyzeSvc::UE_ANA_BASEM1TITLE);
//	m_vec_Right_1_BTN_INFO.push_back(btnInfo);
//
//	btnInfo.text=_tl(CAnalyzeSvc::UE_ANA_HISTORYTITLE);
//	m_vec_Right_1_BTN_INFO.push_back(btnInfo);
//
//	btnInfo.text=_tl(CAnalyzeSvc::UE_ANA_USERINFOTITLE);
//	m_vec_Right_1_BTN_INFO.push_back(btnInfo);
//}
//// CBaseUserInfoDlg message handlers
