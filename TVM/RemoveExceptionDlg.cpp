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
	@brief      ���캯��

	@param      (i)CService*    pService    �Ի���ʹ�õ�Service

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	CRemoveExceptionDlg::CRemoveExceptionDlg(CService* pService)
		: COperationDlg(CRemoveExceptionDlg::IDD,pService)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��������

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	CRemoveExceptionDlg::~CRemoveExceptionDlg()
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
	int CRemoveExceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA))
		{
			return -1;
		}

		//����Gstatic
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LableGroupFour);

		return 0;
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
	void CRemoveExceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);

		CRemoveExceptionSvc* pSvc = (CRemoveExceptionSvc*)GetService();

		if (pSvc->GetCurDialogGroupID() == CRemoveExceptionSvc::DIALOG_GROUP_REMOVE_EXCEPTION)
		{
			//���ü��̿���
			theAPP_SESSION.SetKeyboard(KEY_ENTER|KEY_PAGE|KEY_RETURN);
			InitDialogUI();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ˢ�½�������

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
	@brief      ��ʼ������

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
	@brief      ������ʾ����

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
			int index = pSvc->GetPagePrefix();//��ҳ��ʼλ��
			for (int i = 1; i < _MAX_ROW; ++i)
			{
				if (index < modol.m_ExceptionNum)
				{
					CSysException& e  = modol.m_ListException[index];
					COleDateTime time = e.GetOccurTime();//�쳣����ʱ��

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
	@brief      �����Ϣ��ʾ����

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