#include "stdafx.h"
#include "BanknoteChangeModuleTestSvc.h"
#include "BanknoteChangeMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBanknoteChangeMaintainDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CBanknoteChangeMaintainDlg, COperationDlg)
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
	CBanknoteChangeMaintainDlg::CBanknoteChangeMaintainDlg(CService* pService)
		: COperationDlg(CBanknoteChangeMaintainDlg::IDD,pService)
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
	CBanknoteChangeMaintainDlg::~CBanknoteChangeMaintainDlg()
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
	int CBanknoteChangeMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	void CBanknoteChangeMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState, pWndOther, bMinimized);

		Clear();
		//Update Menu status when Service input [Enter] 
		CBanknoteChangeModuleTestSvc* pSvc = (CBanknoteChangeModuleTestSvc*)GetService();
		if (pSvc->GetCurDialogGroupID() == CBanknoteChangeModuleTestSvc::DIALOG_GROUP_BANKNOTE_CHANGE)
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
	void CBanknoteChangeMaintainDlg::UpdateUI()
	{
		Clear();
		CBanknoteChangeModuleTestSvc* pSvc = (CBanknoteChangeModuleTestSvc*)GetService();
		if (pSvc->GetCurDialogGroupID() == CBanknoteChangeModuleTestSvc::DIALOG_GROUP_BANKNOTE_CHANGE)
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
	void CBanknoteChangeMaintainDlg::InitMenuUI()
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
	void CBanknoteChangeMaintainDlg::UpdateMenuData()
	{
		CBanknoteChangeModuleTestSvc* pSvc = (CBanknoteChangeModuleTestSvc*)GetService();
		CBanknoteChangeModuleTestSvc::Modol& modol = pSvc->GetDataModol();
		//�˵�
		m_labelGroupFour[0].name1 = _T("[1]") + _opl(BH_TEST_TEXT_RESET);
		if(theAPP_SESSION.IsSuperUser() || theFunction_INFO.IsFounderOperation()){
			m_labelGroupFour[1].name1 = _T("[2]") + _opl(BH_TEST_TEXT_RETURN_BANKNOTES);

			m_labelGroupFour[2].name1 = _T("[3]") + _opl(BH_TEST_TEXT_READ_RFID);

			m_labelGroupFour[2].name2 = _T("[1]") + _opl(BH_TEST_TEXT_CHANGE_BOXA);
			m_labelGroupFour[3].name2 = _T("[2]") + _opl(BH_TEST_TEXT_CHANGE_BOXB);
			m_labelGroupFour[4].name2 = _T("[3]") + _opl(BH_TEST_TEXT_CHANGE_COIN);
			m_labelGroupFour[2].name3 = modol.readRfidBoxA.ToString() == INVALID_ID ? _T("") : modol.readRfidBoxA.ToString();
			m_labelGroupFour[3].name3 = modol.readRfidBoxB.ToString() == INVALID_ID ? _T("") : modol.readRfidBoxB.ToString();
			m_labelGroupFour[4].name3 = modol.readRfidInvalid.ToString() == INVALID_ID ? _T("") : modol.readRfidInvalid.ToString();

			m_labelGroupFour[5].name1 = _T("[4]") + _opl(BH_TEST_TEXT_WRITE_RFID);

			m_labelGroupFour[5].name2 = _T("[1]") + _opl(BH_TEST_TEXT_CHANGE_BOXA);;
			m_labelGroupFour[6].name2 = _T("[2]") + _opl(BH_TEST_TEXT_CHANGE_BOXB);;
			m_labelGroupFour[7].name2 = _T("[3]") + _opl(BH_TEST_TEXT_CHANGE_COIN);
			m_labelGroupFour[5].name3 = modol.WriteRfidBoxA.ToString() == INVALID_ID ? _T("") : modol.WriteRfidBoxA.ToString();
			m_labelGroupFour[6].name3 = modol.WriteRfidBoxB.ToString() == INVALID_ID ? _T("") : modol.WriteRfidBoxB.ToString();
			m_labelGroupFour[7].name3 = modol.WriteRfidInvalid.ToString() == INVALID_ID ? _T("") : modol.WriteRfidInvalid.ToString();


			/*m_labelGroupFour[1].name1 = _T("[2]") + _opl(BH_TEST_TEXT_IDENTIFY_BANKNOTES);
			m_labelGroupFour[2].name1 = _T("[3]") + _opl(BH_TEST_TEXT_RETURN_BANKNOTES);
			m_labelGroupFour[3].name1 = _T("[4]") + _opl(BH_TEST_TEXT_ACCEPT_BANKNOTES);
			m_labelGroupFour[4].name1 = _T("[5]") + _opl(BH_TEST_TEXT_COLLECT_BANKNOTES);
			m_labelGroupFour[1].name2 = _T("[1]") + _opl(BH_TEST_TEXT_FROM_CACHE);
			m_labelGroupFour[1].name3 = _T("[2]") + _opl(BH_TEST_TEXT_FROM_CACHE_AND_REGISTER);*/
		}

		//�˵�����
		for (int nMenu = 0; nMenu < 2; ++nMenu)
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
	void CBanknoteChangeMaintainDlg::Clear()
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