#include "stdafx.h"
#include "SmartCardTestSvc.h"
#include "SmartCardTestMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CSmartCardTestMaintainDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CSmartCardTestMaintainDlg, COperationDlg)
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
	CSmartCardTestMaintainDlg::CSmartCardTestMaintainDlg(CService* pService)
		: COperationDlg(CSmartCardTestMaintainDlg::IDD,pService)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CSmartCardTestMaintainDlg::~CSmartCardTestMaintainDlg()
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
	int CSmartCardTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
			return -1;
		}

		// 设置GStatic
		m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroupFive);
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
	void CSmartCardTestMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState, pWndOther, bMinimized);

		Clear();
		//Update Menu status when Service input [Enter] 
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_TH_TEST)
		{
			theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_ENTER|KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(2);
			InitMenuUI();
			UpdateMenuData();
		}
		else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_MOTOR_TEST)
		{
			theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_ENTER|KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(2);
			InitMotorTestMenuUI();
			UpdateMotorTestMenuData();
		}
		else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_READ_RFID_TEST)
		{
			theAPP_SESSION.SetKeyboard(KEY_ENTER|KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(0);
			InitReadRFIDTestMenuUI();
			UpdateReadRFIDTestMenuData();
			theAPP_SESSION.ShowOfficeGuide(GUIDE_F3_ESC);
		}
		/*else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_WRITE_RFID_TEST)
		{
		theAPP_SESSION.SetKeyboard(KEY_INPUT|KEY_ENTER|KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(3);
		InitWriteRFIDTestMenuUI();
		UpdateWriteRFIDMenuData();
		}*/
		else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_SENSOR_TEST)
		{
			theAPP_SESSION.SetKeyboard(KEY_RETURN);
			theAPP_SESSION.SetMaxInputLength(0);
			InitSensorTestMenuUI();
			UpdateSensorTestMenuData();
			theAPP_SESSION.ShowOfficeGuide(GUIDE_F3_ESC);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      刷新界面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::UpdateUI()
	{
		Clear();
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_TH_TEST)
		{
			UpdateMenuData();
		}
		else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_MOTOR_TEST)
		{
			UpdateMotorTestMenuData();
		}
		else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_READ_RFID_TEST)
		{
			UpdateReadRFIDTestMenuData();
		}
		/*else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_WRITE_RFID_TEST)
		{
		UpdateWriteRFIDMenuData();
		}*/
		else if (pSvc->GetCurDialogGroupID() == CSmardCardTestSvc::DIALOG_GROUP_SENSOR_TEST)
		{
			UpdateSensorTestMenuData();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置主菜单界面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::InitMenuUI()
	{
		//设置菜单选项宽度
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 200);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 130);
		m_baseInfoArea->SetLableWidth(2,RESOLUTION * 130);
		m_baseInfoArea->SetLableWidth(3,RESOLUTION * 0);
		m_baseInfoArea->SetLableWidth(4,RESOLUTION * 120);

		/*m_baseInfoArea->SetLableWidth(3,1,RESOLUTION * 250);
		m_baseInfoArea->SetLableWidth(3,2,RESOLUTION * 250);
		m_baseInfoArea->SetLableWidth(3,3,0);
		m_baseInfoArea->SetLableWidth(3,4,RESOLUTION * 160);

		m_baseInfoArea->SetLableWidth(6,1,RESOLUTION * 250);
		m_baseInfoArea->SetLableWidth(6,2,RESOLUTION * 250);
		m_baseInfoArea->SetLableWidth(6,3,0);
		m_baseInfoArea->SetLableWidth(6,4,RESOLUTION * 160);*/

		//Set aliagn
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_LEFT);
		m_baseInfoArea->SetLableAlign(4,DT_RIGHT);

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置电机测试画面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::InitMotorTestMenuUI()
	{
		//设置菜单选项宽度
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 200);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 120);
		m_baseInfoArea->SetLableWidth(2,RESOLUTION * 160);
		m_baseInfoArea->SetLableWidth(3,RESOLUTION * 160);
		m_baseInfoArea->SetLableWidth(4,RESOLUTION * 160);

		//Set aliagn
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_LEFT);
		m_baseInfoArea->SetLableAlign(4,DT_LEFT);
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置读RFID测试画面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::InitReadRFIDTestMenuUI()
	{
		//设置菜单选项宽度
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(2,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(3,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(4,RESOLUTION * 128);

		//Set aliagn
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_LEFT);
		m_baseInfoArea->SetLableAlign(4,DT_LEFT);
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置写RFID测试画面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::InitWriteRFIDTestMenuUI()
	{
		//设置菜单选项宽度
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(2,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(3,RESOLUTION * 128);
		m_baseInfoArea->SetLableWidth(4,RESOLUTION * 128);

		//Set aliagn
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_LEFT);
		m_baseInfoArea->SetLableAlign(4,DT_LEFT);
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置传感器测试画面

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::InitSensorTestMenuUI()
	{
		//设置菜单选项宽度
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 200);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 100);
		m_baseInfoArea->SetLableWidth(2,RESOLUTION * 100);
		m_baseInfoArea->SetLableWidth(3,RESOLUTION * 100);
		m_baseInfoArea->SetLableWidth(4,RESOLUTION * 100);

		//Set aliagn
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_LEFT);
		m_baseInfoArea->SetLableAlign(2,DT_LEFT);
		m_baseInfoArea->SetLableAlign(3,DT_LEFT);
		m_baseInfoArea->SetLableAlign(4,DT_LEFT);
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置菜单数据

	@param      none

	@retval     none

	@exception  none1
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::UpdateMenuData()
	{
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		CSmardCardTestSvc::Modol& modol = pSvc->GetDataModol();
		int index = 0;
		//菜单
		m_lableGroupFive[index++].name1 = _T("[1] ") + _opl(CARD_TEST_TEXT_RESET);
		if(theAPP_SESSION.IsSuperUser() || theFunction_INFO.IsFounderOperation()){
			// line 2
			m_lableGroupFive[index].name1 = _T("[2] ") + _opl(CARD_TEST_TEXT_CATCH_CARD);
			m_lableGroupFive[index].name2 = _T("[1]") + _opl(CARD_TEST_TEXT_BOX_A);
			m_lableGroupFive[index].name3 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_B);
			index++;

			// line 3
			m_lableGroupFive[index].name1 = _T("[3] ") + _opl(CARD_TEST_TEXT_ISSUE_CARD);
			index++;

			// line 4
			m_lableGroupFive[index].name1 = _T("[4] ") + _opl(CARD_TEST_TEXT_MOVE_BOX);
			m_lableGroupFive[index].name2 = _T("[1]") + _opl(CARD_TEST_TEXT_BOX_A) + _T("/上升");
			m_lableGroupFive[index].name3 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_A) + _T("/下降");
			index++;
			// line 5
			m_lableGroupFive[index].name2 = _T("[3]") + _opl(CARD_TEST_TEXT_BOX_B) + _T("/上升");
			m_lableGroupFive[index].name3 = _T("[4]") + _opl(CARD_TEST_TEXT_BOX_B) + _T("/下降");
			index++;

			// line 6~7
			m_lableGroupFive[index++].name1 = _T("[5] ") + _opl(CARD_TEST_TEXT_READ_RFID);
			//m_lableGroupFive[index++].name1 = _T("[6] ") + _opl(CARD_TEST_TEXT_WRITE_RFID);

			// line 8
			m_lableGroupFive[index].name1 = _T("[6] ") + _opl(CARD_TEST_TEXT_ISSUE_TEST);
			m_lableGroupFive[index].name2 = _T("[1]") + _opl(CARD_TEST_TEXT_BOX_A) + _T("/出口");
			m_lableGroupFive[index].name3 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_A) + _T("/废票");
			index++;
			// line 9
			m_lableGroupFive[index].name2 = _T("[3]") + _opl(CARD_TEST_TEXT_BOX_B) + _T("/出口");
			m_lableGroupFive[index].name3 = _T("[4]") + _opl(CARD_TEST_TEXT_BOX_B) + _T("/废票");
			index++;
			// line 10
			/*m_lableGroupFive[index].name1 = _T("[8] ") + _opl(CARD_TEST_TEXT_MOVE_MOTOR_TEST);
			m_lableGroupFive[index].name2 = _T("[1]") + _opl(CARD_TEST_TEXT_BOX_A);
			m_lableGroupFive[index].name3 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_B);
			index++;*/

			// line 11~15
			//m_lableGroupFive[index++].name1 = _T("[7] ") + _opl(CARD_TEST_TEXT_MOTOR_TEST);
			m_lableGroupFive[index++].name1 = _T("[7] ") + _opl(CARD_TEST_TEXT_SENSOR_TEST);
			//m_lableGroupFive[index++].name1 = _T("[11]") + _opl(CARD_TEST_TEXT_ELECTROMAGNET_TEST);
			//m_lableGroupFive[index++].name1 = _T("[12]回收缓冲区票卡");
			m_lableGroupFive[index++].name1 = _T("[8] 票卡回收测试");
		}	

		//菜单数据
		for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu)
		{
			m_lableGroupFive[nMenu].name5 = modol.menuInfo[nMenu].strMenuStatus;
			m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 1,modol.menuInfo[nMenu].strMenuStatus == FAILED ? RED : SOFTGREEN);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置电机测试菜单数据

	@param      none

	@retval     none

	@exception  none1
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::UpdateMotorTestMenuData()
	{
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		CSmardCardTestSvc::Modol& modol = pSvc->GetDataModol();
		if( !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation())
			return;

		//菜单
		m_lableGroupFive[0].name1.Format(_T("%s%s"),_T("[1]"),_T("停止当前电机"));//_opl(CARD_TEST_TEXT_STOP_MOTOR);
		m_lableGroupFive[1].name1 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_A_UD_CLOCKWISE);
		m_lableGroupFive[2].name1 = _T("[3]") + _opl(CARD_TEST_TEXT_BOX_A_UD_ANTICLOCKWISE);
		m_lableGroupFive[3].name1 = _T("[4]") + _opl(CARD_TEST_TEXT_BOX_B_UD_CLOCKWISE);
		m_lableGroupFive[4].name1 = _T("[5]") + _opl(CARD_TEST_TEXT_BOX_B_UD_ANTICLOCKWISE);
		m_lableGroupFive[5].name1.Format(_T("%s%s"),_T("[6]"),_T("A票箱刮票马达测试"));//_opl(CARD_TEST_TEXT_BOX_A_SHAVE_CLOCKWISE);
		m_lableGroupFive[6].name1.Format(_T("%s%s"),_T("[7]"),_T("B票箱刮票马达测试"));//_opl(CARD_TEST_TEXT_BOX_A_SHAVE_ANTICLOCKWISE);
		m_lableGroupFive[7].name1.Format(_T("%s%s"),_T("[8]"),_T("出票马达测试"));//_opl(CARD_TEST_TEXT_BOX_B_SHAVE_CLOCKWISE);
		m_lableGroupFive[8].name1.Format(_T("%s%s"),_T("[9]"),_T("通道清理测试"));//_opl(CARD_TEST_TEXT_BOX_B_SHAVE_ANTICLOCKWISE);
		/*m_lableGroupFive[9].name1 = _T("[10]") + _opl(CARD_TEST_TEXT_TS_CLOCKWISE);
		m_lableGroupFive[10].name1 = _T("[11]") + _opl(CARD_TEST_TEXT_TS_ANTICLOCKWISE);
		m_lableGroupFive[11].name1 = _T("[12]") + _opl(CARD_TEST_TEXT_CARD_READ_CLOCKWISE);
		m_lableGroupFive[12].name1 = _T("[13]") + _opl(CARD_TEST_TEXT_CARD_READ_ANTICLOCKWISE);*/

		//菜单数据
		for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu)
		{
			m_lableGroupFive[nMenu].name4 = modol.menuInfo[nMenu].motorStatus;
			m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 2,modol.menuInfo[nMenu].motorStatus == FAILED ? RED : SOFTGREEN);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置读RFID测试菜单数据

	@param      none

	@retval     none

	@exception  none1
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::UpdateReadRFIDTestMenuData()
	{
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		CSmardCardTestSvc::Modol& modol = pSvc->GetDataModol();
		if( !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation())
			return;

		//菜单
		m_lableGroupFive[0].name1 = _T("[1]") + _opl(CARD_TEST_TEXT_BOX_A);
		m_lableGroupFive[1].name1 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_B);
		m_lableGroupFive[2].name1 = _T("[3]") + _opl(CARD_TEST_TEXT_BOX_C);
		//m_lableGroupFive[3].name1 = _T("[4]") + _opl(CARD_TEST_TEXT_BOX_A_B);


		//菜单数据
		for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu)
		{
			m_lableGroupFive[nMenu].name4 = (nMenu < 3 ? _T("ID："):_T("")) +  modol.menuInfo[nMenu].read_rfidStatus;
			m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 2,modol.menuInfo[nMenu].read_rfidStatus == FAILED ? RED : SOFTGREEN);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置写RFID测试菜单数据

	@param      none

	@retval     none

	@exception  none1
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::UpdateWriteRFIDMenuData()
	{
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		CSmardCardTestSvc::Modol& modol = pSvc->GetDataModol();
		if( !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation())
			return;

		//菜单
		m_lableGroupFive[0].name1 = _T("[1]") + _opl(CARD_TEST_TEXT_BOX_A);
		m_lableGroupFive[1].name1 = _T("[2]") + _opl(CARD_TEST_TEXT_BOX_B);
		m_lableGroupFive[2].name1 = _T("[3]") + _opl(CARD_TEST_TEXT_BOX_C);
		//m_lableGroupFive[3].name1 = _T("[4]") + _opl(CARD_TEST_TEXT_BOX_A_B);

		//菜单数据
		for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu)
		{
			m_lableGroupFive[nMenu].name2 = (nMenu < 3 ? _T("ID:"):_T("") )+  modol.menuInfo[nMenu].writeID;
			m_lableGroupFive[nMenu].name4 = (nMenu < 2 ? (_T("数量:")+ modol.menuInfo[nMenu].writeCount ):_T(""));
			m_lableGroupFive[nMenu].name5 = modol.menuInfo[nMenu].write_rfidStatus;
			m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 1,modol.menuInfo[nMenu].write_rfidStatus == FAILED ? RED : SOFTGREEN);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      设置传感器测试菜单数据

	@param      none

	@retval     none

	@exception  none1
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::UpdateSensorTestMenuData()
	{
		CSmardCardTestSvc* pSvc = (CSmardCardTestSvc*)GetService();
		CSmardCardTestSvc::Modol& modol = pSvc->GetDataModol();
		if( !theAPP_SESSION.IsSuperUser() &&!theFunction_INFO.IsFounderOperation())
			return;

		int index= 0,iMenuIndex = 1;
		//菜单
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("读写器区检测传感器"));// = _T("[1]") + _opl(CARD_TEST_TEXT_SENSOR_EP1);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱A到位传感器"));// = _T("[2]") + _opl(CARD_TEST_TEXT_SENSOR_EP2);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱A盖顶传感器"));// = _T("[3]") + _opl(CARD_TEST_TEXT_SENSOR_EP3);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱A将空传感器"));// = _T("[4]") + _opl(CARD_TEST_TEXT_SENSOR_EP4);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱A空传感器"));// = _T("[5]") + _opl(CARD_TEST_TEXT_SENSOR_EP5);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱B到位传感器"));// = _T("[6]") + _opl(CARD_TEST_TEXT_SENSOR_EP6);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱B盖顶传感器"));// = _T("[7]") + _opl(CARD_TEST_TEXT_SENSOR_EP1);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱B将空传感器"));// = _T("[8]") + _opl(CARD_TEST_TEXT_SENSOR_EP2);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票箱B空传感器"));// = _T("[9]") + _opl(CARD_TEST_TEXT_SENSOR_EP3);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("废票箱到位传感器"));// = _T("[10]") + _opl(CARD_TEST_TEXT_SENSOR_EP4);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("废票箱盖顶传感器"));// = _T("[11]") + _opl(CARD_TEST_TEXT_SENSOR_EP5);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("废票箱满传感器"));// = _T("[12]") + _opl(CARD_TEST_TEXT_SENSOR_EP6);
		m_lableGroupFive[index++].name1.Format(_T("[%d]%s"),iMenuIndex++,_T("票卡出口检测传感器"));// = _T("[12]") + _opl(CARD_TEST_TEXT_SENSOR_EP6);


		int iResultMenu = 0;
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(modol.cardSenserStatus.cRwAreaDetect == 0x00)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;

		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxAArrive)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxACoverArrive)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxANearlyEmpty)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxAEmpty)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxBArrive)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxBCoverArrive)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxBNearlyEmpty)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxBEmpty)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxCArrive)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxCCoverArrive)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;
		
		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else if(!modol.cardSenserStatus.bBoxCFull)
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;

		if(!modol.bCardSenserTestSuccess)
			m_lableGroupFive[iResultMenu].name4 = FAILED;
		else
			m_lableGroupFive[iResultMenu].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(iResultMenu,3,m_lableGroupFive[iResultMenu].name4 == FAILED ? RED : SOFTGREEN);
		iResultMenu++;

		/*if(modol.INFS2Covered == 0)
		m_lableGroupFive[1].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else if(modol.INFS2Covered == -1)
		m_lableGroupFive[1].name4 = FAILED;
		else
		m_lableGroupFive[1].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(1,3,m_lableGroupFive[1].name4 == FAILED ? RED : SOFTGREEN);

		if(modol.INFS3Covered == 0)
		m_lableGroupFive[2].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else if(modol.INFS3Covered == -1)
		m_lableGroupFive[2].name4 = FAILED;	
		else
		m_lableGroupFive[2].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(2,3,m_lableGroupFive[2].name4 == FAILED ? RED : SOFTGREEN);

		if(modol.INFS4Covered == 0)
		m_lableGroupFive[3].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else if(modol.INFS4Covered == -1)
		m_lableGroupFive[3].name4 = FAILED;
		else
		m_lableGroupFive[3].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(3,3,m_lableGroupFive[3].name4 == FAILED ? RED : SOFTGREEN);

		if(modol.INFS5Covered == 0)
		m_lableGroupFive[4].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else if(modol.INFS5Covered == -1)
		m_lableGroupFive[4].name4 = FAILED;
		else
		m_lableGroupFive[4].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(4,3,m_lableGroupFive[4].name4 == FAILED ? RED : SOFTGREEN);

		if(modol.INFS6Covered == 0)
		m_lableGroupFive[5].name4 = _opl(CARD_TEST_TEXT_SHELTER_NO);
		else if(modol.INFS6Covered == -1)
		m_lableGroupFive[5].name4 = FAILED;
		else
		m_lableGroupFive[5].name4 = _opl(CARD_TEST_TEXT_SHELTER);
		m_baseInfoArea->SetLabelColor(5,3,m_lableGroupFive[5].name4 == FAILED ? RED : SOFTGREEN);*/

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      清除界面显示结果

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CSmartCardTestMaintainDlg::Clear()
	{
		for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
		{
			m_lableGroupFive[nRow].name1   = UNKNOW;
			m_lableGroupFive[nRow].name2   = UNKNOW;
			m_lableGroupFive[nRow].name3   = UNKNOW;
			m_lableGroupFive[nRow].name4   = UNKNOW;
			m_lableGroupFive[nRow].name5   = UNKNOW;

			for(int nCol = 0; nCol < _MAX_COL; ++nCol)
			{
				m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
			}		
		} 
	}
