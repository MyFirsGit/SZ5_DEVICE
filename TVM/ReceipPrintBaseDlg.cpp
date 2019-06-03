#include "stdafx.h"
#include "ReceipPrintSvc.h"
#include "ReceipPrintBaseDlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CReceipPrintBaseDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CReceipPrintBaseDlg, COperationDlg)
		ON_WM_CREATE()
		ON_WM_ACTIVATE()
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      构造函数

	@param      (i)CService* pService

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CReceipPrintBaseDlg::CReceipPrintBaseDlg(CService* pService)
		: COperationDlg(CReceipPrintBaseDlg::IDD,pService)
	{
		m_baseInfo->detailInfo.row = MAX_ROW;
		m_baseInfo->detailInfo.labelGroup = m_lableGroup;
		pSvc = (CReceipPrintSvc*)pService;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CReceipPrintBaseDlg::~CReceipPrintBaseDlg()
	{
	}

	////////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面创建函数

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:创建成功    -1:创建失败

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CReceipPrintBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)) {
			return -1;
		}

		m_baseInfoArea->SetLableWidth(0, 800);
		m_baseInfoArea->SetLableAlign(0, DT_LEFT);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面激活后的处理

	@param      (i)UINT nState      不使用
	@param      (i)CWnd* pWndOther  不使用
	@param      (i)BOOL bMinimized  不使用

	@retval     void

	@exception  CSysException
	*/
	//////////////////////////////////////////////////////////////////////////
	void CReceipPrintBaseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState, pWndOther, bMinimized);
		UpdateUI();
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_PAGE);
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief		服务数据模型发生变化时的更新界面   

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CReceipPrintBaseDlg::UpdateUI()
	{
		ClearDialogData();

		if(pSvc->GetCurDialogGroupID() == CReceipPrintSvc::DIALOG_GROUP_MAIN_MENU){
			int nIndex = 0;
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_SLIP) + _T("(%d)"),pSvc->GetDataModel().vecBalanceFileList.size());
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_BH) + _T("(%d)"),pSvc->GetDataModel().vecBHReciveFileList.size());
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_NOTES_CHANGE) + _T("(%d)"),pSvc->GetDataModel().vecBHChangeFileList.size());
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_COIN) + _T("(%d)"),pSvc->GetDataModel().vecCHOperFileList.size());
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_CARD) + _T("(%d)"),pSvc->GetDataModel().vecTHOperFileList.size());
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_PASSENGER) + _T("(%d)"),pSvc->GetDataModel().vecPassengerFileList.size());
			m_lableGroup[nIndex++].nameLeft.Format(_opl(DEVICESTATUS_BUSINESS_NOT_HIT_PRINT));
		}
		else{
			vector<CString>* pCurFileList;

			pCurFileList = NULL;

			pSvc->GetSelectedFileList(pCurFileList);


			if(NULL != pCurFileList){
				int nCurMenu = (pSvc->GetCurrentPage() - 1)* MAX_ROW + 1;
				for(int i = 0; i < MAX_ROW && nCurMenu <= pCurFileList->size();i++,nCurMenu++){
					CString strFilePath = pCurFileList->at(nCurMenu - 1);
					m_lableGroup[i].nameLeft.Format(_T("[%d]%s"),nCurMenu,TransFileNameToDisplay(strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind('\\') -1)));
				}

				if(pCurFileList->size() == 0){
					theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_NO_PRINT_RECORD));
				}
				else{
					if(pSvc->GetTotalPage() == 1 ){
						theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
					}
					else{
						if (pSvc->GetCurrentPage() == 1){
							theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F6_F3_ESC);
						}
						else if (pSvc->GetCurrentPage() == pSvc->GetTotalPage()){
							theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F5_F3_ESC);
						}
						else{
							theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F5_F3_ESC);
						}
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      清理界面显示信息

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CReceipPrintBaseDlg::ClearDialogData(){
		//清理画面
		for(int i = 0; i < MAX_ROW; i++)
		{
			m_lableGroup[i].nameLeft = _T("");
		}
		m_baseInfoArea->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      将打印日志明格式化可视化更好的文件名称

	@param      const CString& strFileName;被格式化的打印文件名

	@retval     CString strFileNameForDisplay;转化后的名称

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	const CString CReceipPrintBaseDlg::TransFileNameToDisplay(const CString& strFileName) const {
		CString strFileNameForDisplay;

		strFileNameForDisplay.Format(_T("%s年%s月%s日%s:%s:%s 操作员：%s  操作内容：%s"),
			strFileName.Left(4),
			strFileName.Mid(4,2),
			strFileName.Mid(6,2),
			strFileName.Mid(8,2),
			strFileName.Mid(10,2),
			strFileName.Mid(12,2),
			strFileName.Mid(15,6),
			strFileName.Left(strFileName.Find('.')).Mid(22));
		return strFileNameForDisplay;
	}
