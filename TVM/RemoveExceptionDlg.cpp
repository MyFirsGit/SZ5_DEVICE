#include "stdafx.h"
#include "RemoveExceptionDlg.h"
#include "RemoveExceptionSvc.h"
#include "ServiceMgr.h"
#include "ExceptionMgr.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRemoveExceptionDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CRemoveExceptionDlg, COperationDlg)
		ON_WM_CREATE()
		ON_WM_ACTIVATE()
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      构造函数

	@param      (i)CService*    pService    对话框使用的Service

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	CRemoveExceptionDlg::CRemoveExceptionDlg(CService* pService)
		: COperationDlg(CRemoveExceptionDlg::IDD,pService)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	CRemoveExceptionDlg::~CRemoveExceptionDlg()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面创建函数

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    \n
	0:创建成功    -1:创建失败

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	int CRemoveExceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA))
		{
			return -1;
		}

		//设置Gstatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LableGroupFour);

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
	void CRemoveExceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);

		CRemoveExceptionSvc* pSvc = (CRemoveExceptionSvc*)GetService();

		if (pSvc->GetCurDialogGroupID() == CRemoveExceptionSvc::DIALOG_GROUP_REMOVE_EXCEPTION)
		{
			//设置键盘可用
			theAPP_SESSION.SetKeyboard(KEY_ENTER|KEY_PAGE|KEY_RETURN);
			InitDialogUI();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      刷新界面数据

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CRemoveExceptionDlg::UpdateUI()
	{
		UpdateDialogData();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      初始化界面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CRemoveExceptionDlg::InitDialogUI()
	{
		ClearLabelData();
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 200);//Time
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 70);//EC
		m_baseInfoArea->SetLableWidth(2,RESOLUTION * 100);//MTC
		m_baseInfoArea->SetLableWidth(3,RESOLUTION * 260);//Description

		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_LEFT);

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      更新显示数据

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CRemoveExceptionDlg::UpdateDialogData()
	{
		ClearLabelData();
		CRemoveExceptionSvc* pSvc = (CRemoveExceptionSvc*)GetService();
		CRemoveExceptionSvc::Modol& modol = pSvc->GetDataModol();

		m_LableGroupFour[0].name1 = _opl(TEXT_REMOVE_EXCEPTION_TIME);
		m_LableGroupFour[0].name2 = _opl(TEXT_REMOVE_EXCTPTION_EC);
		m_LableGroupFour[0].name3 = _opl(TEXT_REMOVE_EXCTPTION_MTC);
		m_LableGroupFour[0].name4 = _opl(TEXT_REMOVE_EXCTPTION_DESC);

		if (pSvc->GetCurrentPage() <= pSvc->GetTotalPage())
		{
			int index = pSvc->GetPagePrefix();//本页起始位置
			for (int i = 1; i < _MAX_ROW; ++i)
			{
				if (index < modol.m_ExceptionNum)
				{
					CSysException& e  = modol.m_ListException[index];
					COleDateTime time = e.GetOccurTime();//异常产生时间

					CString strTime,strEC,strMTC,strDesc;

					strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
					strEC.Format(_T("%.4X"),theEXCEPTION_MGR.GetEC(e));
					strMTC.Format(_T("%.8X"),theEXCEPTION_MGR.GetActualMTC(e));
					strDesc.Format(_T("%s"),theEXCEPTION_MGR.GetDescription(e));


					m_LableGroupFour[i].name1 = strTime;
					m_LableGroupFour[i].name2 = strEC;
					m_LableGroupFour[i].name3 = strMTC;
					m_LableGroupFour[i].name4 = strDesc;

					++index;
				}
				else
				{
					m_LableGroupFour[i].name1 =  _T("");
					m_LableGroupFour[i].name2 =  _T("");
					m_LableGroupFour[i].name3 =  _T("");
					m_LableGroupFour[i].name4 =  _T("");
				}
			}
		}
		else
		{
			ClearLabelData();
		}

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      清除消息显示内容

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CRemoveExceptionDlg::ClearLabelData()
	{
		for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
		{
			m_LableGroupFour[nRow].name1  = _T("");
			m_LableGroupFour[nRow].name2  = _T("");
			m_LableGroupFour[nRow].name3  = _T("");
			m_LableGroupFour[nRow].name4  = _T("");

			for(int nCol = 0; nCol < _MAX_COL; ++nCol)
			{
				m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
			}		
		} 
	}