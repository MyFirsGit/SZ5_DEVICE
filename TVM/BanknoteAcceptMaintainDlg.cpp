#include "stdafx.h"
#include "BanknoteAcceptModuleTestSvc.h"
#include "BanknoteAcceptMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBanknoteAcceptMaintainDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CBanknoteAcceptMaintainDlg, COperationDlg)
		ON_WM_CREATE()
		ON_WM_ACTIVATE()
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���캯��

	@param      (i)CService* pService

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CBanknoteAcceptMaintainDlg::CBanknoteAcceptMaintainDlg(CService* pService)
		: COperationDlg(CBanknoteAcceptMaintainDlg::IDD,pService)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��������

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CBanknoteAcceptMaintainDlg::~CBanknoteAcceptMaintainDlg()
	{
	}

	////////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:�����ɹ�    -1:����ʧ��

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CBanknoteAcceptMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
			return -1;
		}

		// ����GStatic
		m_baseInfoArea = new CGStatic(this, m_baseInfo, m_labelGroupFour);
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
	void CBanknoteAcceptMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState, pWndOther, bMinimized);

		Clear();
		//Update Menu status when Service input [Enter] 
		CBanknoteAcceptModuleTestSvc* pSvc = (CBanknoteAcceptModuleTestSvc*)GetService();
		if (pSvc->GetCurDialogGroupID() == CBanknoteAcceptModuleTestSvc::DIALOG_GROUP_BANKNOTE)
		{
			theAPP_SESSION.SetKeyboard(KEY_ENTER|KEY_INPUT|KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(2);
			InitMenuUI();
			UpdateMenuData();
		}

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ˢ�½���

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CBanknoteAcceptMaintainDlg::UpdateUI()
	{
		Clear();
		CBanknoteAcceptModuleTestSvc* pSvc = (CBanknoteAcceptModuleTestSvc*)GetService();
		if (pSvc->GetCurDialogGroupID() == CBanknoteAcceptModuleTestSvc::DIALOG_GROUP_BANKNOTE)
		{
			UpdateMenuData();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �������˵�����

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CBanknoteAcceptMaintainDlg::InitMenuUI()
	{
		//�����ı�����
		m_baseInfoArea->SetLableWidth(0,180);
		m_baseInfoArea->SetLableWidth(1,120);
		m_baseInfoArea->SetLableWidth(2,200);
		m_baseInfoArea->SetLableWidth(3,100);

		//Set aliagn
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���ò˵�����

	@param      none

	@retval     none

	@exception  none1
	*/
	//////////////////////////////////////////////////////////////////////////
	void CBanknoteAcceptMaintainDlg::UpdateMenuData()
	{
		CBanknoteAcceptModuleTestSvc* pSvc = (CBanknoteAcceptModuleTestSvc*)GetService();
		CBanknoteAcceptModuleTestSvc::Modol& modol = pSvc->GetDataModol();
		//����������ʾ����
		//�˵���ʾ
		m_labelGroupFour[0].name1 = _T("[1]") + _opl(BH_TEST_TEXT_RESET);
		if(theAPP_SESSION.IsSuperUser() || theFunction_INFO.IsFounderOperation()){
			m_labelGroupFour[1].name1 = _T("[2]") + _opl(BH_TEST_TEXT_IDENTIFY_BANKNOTES);
			if(modol.bh_face != VALUE_UNKNOWN)
			{
				CString bh_face_str = _T("");
				switch(modol.bh_face)
				{
				case Banknote1Yuan:
					bh_face_str = _T("��1��");
					break;
				case Banknote5Yuan:
					bh_face_str = _T("��5��");
					break;
				case Banknote10Yuan:
					bh_face_str = _T("��10��");
					break;
				case Banknote20Yuan:
					bh_face_str = _T("��20��");
					break;
				case Banknote50Yuan:
					bh_face_str = _T("��50��");
					break;
				case Banknote100Yuan:
					bh_face_str = _T("��100��");
					break;
				default:
					bh_face_str = _T("");
				}
				m_labelGroupFour[1].name2 = bh_face_str;
				m_baseInfoArea->SetLabelColor(1,1,SOFTGREEN);
			}
			m_labelGroupFour[2].name1 = _T("[3]") + _opl(GUIDE_BANKNOTE_STOP_RECOVERY) /*+ _opl(BH_TEST_TEXT_RETURN_BANKNOTES)*/;
			m_labelGroupFour[3].name1 = _T("[4]") + _opl(BH_TEST_TEXT_RETURN_BANKNOTES);
			//m_labelGroupFour[3].name1 = _T("[4]") + _opl(BH_TEST_TEXT_ACCEPT_BANKNOTES);
			//m_labelGroupFour[3].name1 = _T("[4]") + _opl(BH_TEST_TEXT_COLLECT_BANKNOTES);

			//m_labelGroupFour[2].name2 = _T("[1]") + _opl(BH_TEST_TEXT_FROM_CACHE);
			//m_labelGroupFour[2].name3 = _T("[2]") + _opl(BH_TEST_TEXT_FROM_CACHE_AND_REGISTER);
		}

		//�˵�����
		for (int nMenu = 0; nMenu < 5; ++nMenu)
		{
			m_labelGroupFour[nMenu].name4 = modol.menuInfo[nMenu].strMenuStatus;
			m_baseInfoArea->SetLabelColor(nMenu,3,modol.menuInfo[nMenu].strMenuStatus == FAILED ? RED : SOFTGREEN);
		}

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���������ʾ���

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CBanknoteAcceptMaintainDlg::Clear()
	{
		for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
		{
			m_labelGroupFour[nRow].name1   = _T("");
			m_labelGroupFour[nRow].name2   = _T("");
			m_labelGroupFour[nRow].name3   = _T("");
			m_labelGroupFour[nRow].name4   = _T("");

			for(int nCol = 0; nCol < _MAX_COL; ++nCol)
			{
				m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
			}		
		} 
	}