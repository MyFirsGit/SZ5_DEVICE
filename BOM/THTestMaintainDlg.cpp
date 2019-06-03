#include "stdafx.h"
#include "THTestSvc.h"
#include "THTestMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "TicketHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 信息显示编辑框坐标组合
static CRect editRect[] = {
	BASE_INFO_RECT(1,9),
	BASE_INFO_RECT(2,9),
	BASE_INFO_RECT(3,9),
};

IMPLEMENT_DYNAMIC(CTHTestMaintainDlg, COperationDlg)
	BEGIN_MESSAGE_MAP(CTHTestMaintainDlg, COperationDlg)
		ON_WM_CREATE()
		ON_BTN_OK_CLICK(OnOK)
		ON_BTN_RESET_CLICK(OnReset)
		ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSingleTestTypeBtnClick)
		ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnDetailTestTypeBtnClick)
	END_MESSAGE_MAP()

	static const CString STR_SINGLE_TEST_TYPE[] = {_T("连续出票测试"),_T("重置测试"),_T("升降票箱测试"),_T("废票处理测试"),_T("传感器测试")};
	static const CString STR_TRANSFER_TEST_TYPE[] = {_T("连续出票测试")};
	static const CString STR_RESET_TEST_TYPE[] = {_T("重置")};
	static const CString STR_HANDLE_TEST_TYPE[] = {_T("废票处理测试")};
	static const CString STR_MOVE_BOX[] = {_T("票箱A"),_T("票箱B")};
	static const CString STR_SENSOR_TEST_TYPE[] = {_T("传感器SW测试")};
	static const CString STR_RW_TEST[] = {_T("读RFID"),_T("写RFID")};

	static const CString STR_SINGLE_TEST_TYPE_MSG[] = {/*_T("传送测试")*/_T("连续出票测试"),_T("重置测试"),_T("升降票箱测试"),_T("废票处理测试"),_T("传感器测试")/*,_T("RFID读写")*/};
	static const CString STR_TRANSFER_TEST_TYPE_MSG[] = {_T("连续出票测试")/*_T("发票测试"),_T("回收测试")*/};
	static const CString STR_RESET_TEST_TYPE_MSG[] = {_T("重置")};
	static const CString STR_HANDLE_TEST_TYPE_MSG[] = {_T("废票处理测试")};
	static const CString STR_MOVE_BOX_MSG[] = {_T("票箱1"),_T("票箱2")};
	static const CString STR_SENSOR_TEST_TYPE_MSG[] = {_T("传感器SW测试")};
	static const CString STR_RW_TEST_MSG[] = {_T("读RFID"),_T("写RFID")};


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      构造函数

	@param      (i)CService*    pService    对话框使用的Service

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	CTHTestMaintainDlg::CTHTestMaintainDlg(CService* pService)
		: COperationDlg(CTHTestMaintainDlg::IDD, pService)
	{
		m_baseInfo->titleInfo.titleName = TXT_TH_TEST_SVC_CN;				// 标题
		m_baseInfo->detailInfo.row = 10;								    // 行数
		m_baseInfo->detailInfo.labelGroup = m_labelTHTestInfo;			// 标签文字
		//m_baseInfo->detailInfo.editGroup.editRect = editRect;			// 辑框坐标
		m_baseInfo->detailInfo.editGroup.cnt = 0;						// 编辑框数目

		m_wUnitTestFirstNo = TEST_NUM_UNKNOWN;
		m_wUnitTestSecondNo = TEST_NUM_UNKNOWN;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      void

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	CTHTestMaintainDlg::~CTHTestMaintainDlg()
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
	int CTHTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA | BTN_OK | BTN_RESET /*| EDIT_BOX*/) == -1)
		{
			return -1;
		}
		//GetEdit(0)->SetLimitText(8);
		//GetEdit(1)->SetLimitText(8);
		//GetEdit(2)->SetLimitText(8);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief		对话框初始化

	@param      无

	@retval     BOOL  \n   TRUE:初始化成功  FALSE:初始化失败

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	BOOL CTHTestMaintainDlg::OnInitDialog()
	{
		COperationDlg::OnInitDialog();

		// 初始化基本信息
		InitialBaseInfo();
		// 初始化按钮
		InitialButtons();

		return TRUE;  
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief		初始化按键

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::InitialButtons()
	{
		m_vec_Right_1_BTN_INFO.clear();

		// 设置单选
		m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);

		for (int i = 0; i < 5/*4*/; i++){
			BTN_INFO pSingleTestTypeInfo;
			pSingleTestTypeInfo.btnID = i + 1;
			pSingleTestTypeInfo.btnMsg = STR_SINGLE_TEST_TYPE_MSG[i];
			pSingleTestTypeInfo.text = STR_SINGLE_TEST_TYPE[i];
			m_vec_Right_1_BTN_INFO.push_back(pSingleTestTypeInfo);
		}

		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_1_Group->ClickFirstAvailButton();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief		初始化基本信息

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::InitialBaseInfo()
	{
		CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
		CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();
		m_baseInfo->titleInfo.titleName = TXT_TH_TEST_SVC_CN;				// 标题
		int i = 1;   // 第一行是单体测试详情，清空将不能正常显示，所以从1开始
		for (;i < 10;i++)
		{
			m_labelTHTestInfo[i].nameLeft = _T("");
			m_labelTHTestInfo[i].nameRight = _T("");
		}
		i=0;

		if (m_wUnitTestFirstNo==RW_RFID_TEST){	
			if(m_wUnitTestSecondNo == R_RFIDNUM_TEST){
				m_labelTHTestInfo[i].nameLeft = "票箱A：";
				m_labelTHTestInfo[i++].nameRight = _T("--");
				m_labelTHTestInfo[i].nameLeft = "票箱B：";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				m_labelTHTestInfo[i].nameLeft = "废票箱：";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				// 隐藏重置按钮、编辑框
				HideCompoment(BTN_RESET);
				HideCompoment(EDIT_BOX);
			}
			else if(m_wUnitTestSecondNo == W_RFIDNUM_TEST){	
				//m_baseInfo->titleInfo.titleName = TXT_TH_INFO_SVC;				// 标题
				m_labelTHTestInfo[i].nameLeft = "票箱A：";
				m_labelTHTestInfo[i++].nameRight = _T("--");
				m_labelTHTestInfo[i].nameLeft = "票箱B：";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				m_labelTHTestInfo[i].nameLeft = "废票箱：";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				//// 设置向导文言
				//theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_SET_INFO_INPUT));
				//ShowCompoment(EDIT_BOX);
				//ShowCompoment(BTN_RESET);
				//if (!model.isBoxAExit ){	
				//	GetEdit(0)->ShowWindow(FALSE);
				//}
				//if (!model.isBoxBExit ){	
				//	GetEdit(1)->ShowWindow(FALSE);
				//}
				//if (!model.isBoxCExit ){	
				//	GetEdit(2)->ShowWindow(FALSE);
				//}
			}
		}
		else{
			// 显示BOM文言：请选择TH测试类型
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_CHOOSE_ITEM));

			//初始化lable信息
			m_labelTHTestInfo[i++].nameLeft = "单体测试类型：";
			m_labelTHTestInfo[i++].nameLeft = "详细测试类型：";

			// 隐藏重置按钮、编辑框
			HideCompoment(BTN_RESET);
			HideCompoment(EDIT_BOX);
		}	
		//for (;i<6;i++){
		//	m_labelTHTestInfo[i].nameLeft = TXT_NOTHING;
		//	m_labelTHTestInfo[i].nameRight = TXT_NOTHING;	
		//}
		//m_baseInfo->detailInfo.row = 4;
		m_baseInfoArea->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      处理确认按钮消息

	@param      (i)WPARAM wParam  消息信息
	@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

	@retval     LRESULT  

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT  CTHTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
	{
		if(m_wUnitTestSecondNo == W_RFIDNUM_TEST &&!CheckInputText()){
			return FALSE;
		}
		theSERVICE_MGR.SetForeServiceBusy(true);
		CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
		//SC_CMD_CATCH_PROCESS scIssueProcess;
		//SC_RSP_SEND_PROCESS scRspIssueProcess;
		//SC_RSP_ISSUE_PROCESS thRspInfo;
		//SC_MOVE_BOX_CMD_INFO move_box_info;
		TH_CMD_TEST_SETTING testType;

		bool bRecycle = true;
		int  testCardCount = 0;

		try	{
			switch(m_wUnitTestSecondNo){	
				// 放出
			case VALIDATE_SEND:
				{
					//scIssueProcess.boxId = 0xff;
					//scIssueProcess.TicketCnt = 1;
					bRecycle = false;
					testCardCount = 1;
					TH_CMD_CARD_OUT OutCardBox = CMD_CARD_OUT_BOX_A;//默认从A票箱开始出票

					for (int i=0;i<testCardCount;i++){
						try{

							UINT uiTransportNum = 0;
							pTHTestSvc->GetTHHelper()->CardOut(OutCardBox,&uiTransportNum);// 尝试抓票
							pTHTestSvc->GetTHHelper()->SendCard(&uiTransportNum);// 尝试抓票
						}
						catch(CSysException& e){
							//LOG("出票机异常！！！");
							theEXCEPTION_MGR.WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
							// 票箱1已空(A)
							if(e.GetDetailID() == CTHHardwareException::TH_ERROR_TICKET_BOX1_EMPTY){
								OutCardBox = CMD_CARD_OUT_BOX_B;
								continue;
							}
							else{
								theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_FAIL));
								theSERVICE_MGR.SetForeServiceBusy(false);
								theEXCEPTION_MGR.ProcessException(e);		
							}
						}
						catch(...){
							theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_FAIL));
							theSERVICE_MGR.SetForeServiceBusy(false);
							CInnerException e = CInnerException(pTHTestSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
							theEXCEPTION_MGR.ProcessException(e);
						}

					}			

					break;
				}
				// 回收
			case INVALID_TEST:
				{
					//scIssueProcess.boxId = 0xff;
					//scIssueProcess.TicketCnt = 1;
					bRecycle = true;
					TH_CMD_CARD_OUT OutCardBox = CMD_CARD_OUT_BOX_A;//默认从A票箱开始出票
					testCardCount = 1;

					for (int i=0;i<testCardCount;i++){
						try{
							UINT uiTransportNum = 0;
							pTHTestSvc->GetTHHelper()->CardOut(OutCardBox,&uiTransportNum);
							pTHTestSvc->GetTHHelper()->RetractCard(&uiTransportNum);
							if (OutCardBox = CMD_CARD_OUT_BOX_A)
							{
								theTICKET_HELPER.UpdateTHCount(uiTransportNum,0,uiTransportNum); //更新回收车票计数
							} 
							else
							{
								theTICKET_HELPER.UpdateTHCount(0,uiTransportNum,uiTransportNum); //更新回收车票计数
							}							
							//theTICKET_HELPER.UpdateTHCount(thRspInfo.boxIdACount,thRspInfo.boxIdBCount,thRspInfo.issueRes==3?1:0); //更新回收车票计数	  
						}
						catch(CSysException& e){
							//LOG("出票机异常！！！");
							theEXCEPTION_MGR.WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
							// 票箱1已空(A)
							if(e.GetDetailID() == CTHHardwareException::TH_ERROR_TICKET_BOX1_EMPTY){
								OutCardBox = CMD_CARD_OUT_BOX_B;
								continue;
							}
							else{
								theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_FAIL));
								theSERVICE_MGR.SetForeServiceBusy(false);
								theEXCEPTION_MGR.ProcessException(e);		
							}
						}
						catch(...){
							theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_FAIL));
							theSERVICE_MGR.SetForeServiceBusy(false);
							CInnerException e = CInnerException(pTHTestSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
							theEXCEPTION_MGR.ProcessException(e);
						}

					}			

					break;		
				}
			case TH_RESET:
				// 修复出票机
				pTHTestSvc->GetTHHelper()->Repare();
				break;
				// 升降A票箱
			case MOVE_BOXA_TEST:
				//move_box_info.actionType = 2;
				testType = CMD_TEST_A_TRAY_DOWN;
				//move_box_info.boxId = 1;
				pTHTestSvc->GetTHHelper()->TestSetting(testType);
				//move_box_info.actionType = 1;
				testType = CMD_TEST_A_TRAY_UP;
				//move_box_info.boxId = 1;
				pTHTestSvc->GetTHHelper()->TestSetting(testType);
				break;
				// 升降B票箱
			case MOVE_BOXB_TEST:
				//move_box_info.actionType = 2;
				testType = CMD_TEST_B_TRAY_DOWN;
				//move_box_info.boxId = 2;
				pTHTestSvc->GetTHHelper()->TestSetting(testType);
				//move_box_info.actionType = 1;
				testType = CMD_TEST_B_TRAY_UP;
				//move_box_info.boxId = 2;
				pTHTestSvc->GetTHHelper()->TestSetting(testType);
				break;
				// 传感器测试
			case SENSOR_SW_TEST:
				ShowSensorInfo();
				break;
			case R_RFIDNUM_TEST:
				//ShowRFIDInfo();
				break;
			case W_RFIDNUM_TEST:			
				//DoRW();
				break;
			default:
				break;
			}

			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_SUCCESS_NEW));
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		catch (CSysException& e) {
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_FAIL));
			theSERVICE_MGR.SetForeServiceBusy(false);
			theEXCEPTION_MGR.ProcessException(e);		
		}
		catch (...)	{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_FAIL));
			theSERVICE_MGR.SetForeServiceBusy(false);
			CInnerException e = CInnerException(pTHTestSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
			theEXCEPTION_MGR.ProcessException(e);
		}
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      单体测试按钮

	@param      (i)WPARAM wParam  消息信息
	@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

	@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CTHTestMaintainDlg::OnSingleTestTypeBtnClick(WPARAM wParam,LPARAM lParam)
	{
		try	{
			BTN_INFO* btnInfo = (BTN_INFO*)lParam;
			m_labelTHTestInfo[0].nameRight = btnInfo->btnMsg;
			SINGLE_TEST_TYPE singleTestType = (SINGLE_TEST_TYPE)btnInfo->btnID;

			m_vec_Right_2_BTN_INFO.clear();
			m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
			switch(singleTestType)
			{	
				// 传送测试
			case TRANSFER_TEST:
				for (int i = 0; i < 1/*2*/; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 11;
					pDetailTestTypeBtnInfo.btnMsg = STR_TRANSFER_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_TRANSFER_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				m_wUnitTestFirstNo = TRANSFER_TEST;
				break;
				// 重置测试
			case RESET_TEST:
				for (int i = 0; i < 1; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 21;
					pDetailTestTypeBtnInfo.btnMsg = STR_RESET_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_RESET_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				theAPP_SESSION.ShowOfficeGuide(_T("请点击确认键进行测试。"));
				m_wUnitTestFirstNo = RESET_TEST;
				break;
				// 升降票箱测试
			case MOVE_BOX_TEST:
				for (int i = 0; i < 2; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 31;
					pDetailTestTypeBtnInfo.btnMsg = STR_MOVE_BOX[i];
					pDetailTestTypeBtnInfo.text = STR_MOVE_BOX[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				m_wUnitTestFirstNo = MOVE_BOX_TEST;
				break;

				// 废票处理测试
			case INVALID_HANDLE_TEST:
				for (int i = 0; i < 1; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 41;
					pDetailTestTypeBtnInfo.btnMsg = STR_HANDLE_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_HANDLE_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				//theAPP_SESSION.ShowOfficeGuide(_T("请点击确认键进行测试。"));
				m_wUnitTestFirstNo = INVALID_HANDLE_TEST;
				break;
				// 读写RFID
			case RW_RFID_TEST:		
				{
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = 61;
					pDetailTestTypeBtnInfo.btnMsg = STR_RW_TEST_MSG[0];
					pDetailTestTypeBtnInfo.text = STR_RW_TEST[0];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
					pDetailTestTypeBtnInfo.btnID = 62;
					pDetailTestTypeBtnInfo.btnMsg = STR_RW_TEST_MSG[1];
					pDetailTestTypeBtnInfo.text = STR_RW_TEST[1];
					pDetailTestTypeBtnInfo.isEnable = false;
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				m_wUnitTestFirstNo = RW_RFID_TEST;
				break;	
				// 传感器测试
			case SENSOR_TEST:
				for (int i = 0; i < 1; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 51;
					pDetailTestTypeBtnInfo.btnMsg = STR_SENSOR_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_SENSOR_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				theAPP_SESSION.ShowOfficeGuide(_T("请点击确认键进行测试。"));
				m_wUnitTestFirstNo = SENSOR_TEST;
				break;
			default:
				break;
			}
			m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
			m_Right_2_Group->ClickFirstAvailButton();
			//m_baseInfo->detailInfo.row = 4;
			m_baseInfoArea->Invalidate();
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...)	{
			theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      详细测试按钮

	@param      (i)WPARAM wParam  消息信息
	@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

	@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CTHTestMaintainDlg::OnDetailTestTypeBtnClick(WPARAM wParam,LPARAM lParam)
	{
		try{
			BTN_INFO* btnInfo = (BTN_INFO*)lParam;

			SINGLE_TEST_TYPE detailTestType = (SINGLE_TEST_TYPE)btnInfo->btnID;
			m_wUnitTestSecondNo = detailTestType;
			// 初始化基本信息
			InitialBaseInfo();

			if(m_wUnitTestFirstNo != RW_RFID_TEST){
				m_labelTHTestInfo[1].nameRight = btnInfo->btnMsg;
			}
			//delete m_baseInfoArea;
			//m_baseInfoArea = NULL;	
			//m_baseInfo->detailInfo.row = 4;
			//m_baseInfoArea = new CGStatic(this,m_baseInfo);
			m_baseInfoArea->Invalidate();
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...)	{
			theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示票箱信息

	@param      无

	@retval     无  

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::ShowRFIDInfo()
	{
		try{
			//CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
			//CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();
			//// 票箱A编号
			//model.readBoxAInfo.boxType = RFID_MAGAZINE_A;
			//tTIMRFIDInfo TimRfidInfo;
			////model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID(model.readBoxAInfo);	
			//model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID((TH_BOX_TYPE)RFID_MAGAZINE_A, &TimRfidInfo);	
			//if (model.isBoxAExit ){	
			//	model.strBoxA.Format(_T("%02X%02X%04d"),model.readBoxAInfo.RFIDData.byDeviceID[0],model.readBoxAInfo.RFIDData.byDeviceID[1],ComMakeWORD(model.readBoxAInfo.RFIDData.byDeviceID[2],model.readBoxAInfo.RFIDData.byDeviceID[3]));
			//	SetEditText(0,model.strBoxA);
			//	m_labelTHTestInfo[0].nameRight = model.strBoxA;
			//}

			//// 票箱B编号
			//model.readBoxBInfo.boxType = RFID_MAGAZINE_B;
			////model.isBoxBExit  = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID(model.readBoxBInfo);	
			//model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID((TH_BOX_TYPE)RFID_MAGAZINE_B, &TimRfidInfo);
			//if (model.isBoxBExit ){	
			//	model.strBoxB.Format(_T("%02X%02X%04d"),model.readBoxBInfo.RFIDData.byDeviceID[0],model.readBoxBInfo.RFIDData.byDeviceID[1],ComMakeWORD(model.readBoxBInfo.RFIDData.byDeviceID[2],model.readBoxBInfo.RFIDData.byDeviceID[3]));
			//	SetEditText(1,model.strBoxB);
			//	m_labelTHTestInfo[1].nameRight = model.strBoxB;
			//}

			//// 废票箱编号
			//model.readBoxCInfo.boxType = RFID_MAGAZINE_C;
			////model.isBoxCExit  = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID(model.readBoxCInfo);	
			//model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID((TH_BOX_TYPE)RFID_MAGAZINE_C, &TimRfidInfo);
			//if (model.isBoxCExit ){	
			//	model.strBoxC.Format(_T("%02X%02X%04d"),model.readBoxCInfo.RFIDData.byDeviceID[0],model.readBoxCInfo.RFIDData.byDeviceID[1],ComMakeWORD(model.readBoxCInfo.RFIDData.byDeviceID[2],model.readBoxCInfo.RFIDData.byDeviceID[3]));
			//	SetEditText(2,model.strBoxC);
			//	m_labelTHTestInfo[2].nameRight = model.strBoxC;
			//}

			//if (model.isBoxAExit ||model.isBoxBExit ||model.isBoxCExit ){	
			//	m_Right_2_Group->EnableButton(1,true);
			//}

			//m_baseInfoArea->Invalidate();
		}
		catch (...) {
			throw;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      处理确认、返回、重置 按钮消息

	@param      (i)WPARAM wParam  消息信息
	@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

	@retval     LRESULT  

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CTHTestMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
	{
		CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
		CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();

		SetEditText(0,model.strBoxA);
		SetEditText(1,model.strBoxB);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      检查EDIT框里的数据是否合法

	@param      void

	@retval     bool   true 合法  false 非法

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	bool CTHTestMaintainDlg::CheckInputText()
	{
		bool ret = true;
		CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
		CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();
		CString strIDA = GetEditText(0);
		CString strIDB = GetEditText(1);
		CString strIDC = GetEditText(2);
		if(model.isBoxAExit &&_T("") != strIDA&&strIDA.GetLength() !=8){
			ret = false;
		}
		if(model.isBoxAExit){
			int lineNum = _ttol(strIDA.Mid(0,2));
			int typeCode = _ttol(strIDA.Mid(2,2));
			WORD boxNum = stoms(_ttol(strIDA.Mid(4)));
			if (lineNum==0||(typeCode!=MAGAZINE_NORMAL)){
				ret = false;
			}
		}
		if(model.isBoxBExit &&_T("") != strIDB&&strIDB.GetLength() !=8){
			ret = false;
		}
		if(model.isBoxBExit ){
			int lineNum = _ttol(strIDB.Mid(0,2));
			int typeCode = _ttol(strIDB.Mid(2,2));
			WORD boxNum = stoms(_ttol(strIDB.Mid(4)));
			if (lineNum==0||(typeCode!=MAGAZINE_NORMAL)){
				ret = false;
			}
		}
		if(model.isBoxCExit &&_T("") != strIDC&&strIDC.GetLength() !=8){
			ret = false;
		}
		if(model.isBoxCExit ){
			int lineNum = _ttol(strIDC.Mid(0,2));
			int typeCode = _ttol(strIDC.Mid(2,2));
			WORD boxNum = stoms(_ttol(strIDC.Mid(4)));
			if (lineNum==0||(typeCode!=MAGAZINE_CYCLE)){
				ret = false;
			}
		}

		if (ret == false){	
			theAPP_SESSION.ShowOfficeGuide(_T("（红）输入的内容有错误，请核对后重新输入")/*theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_SET_INPUT_ERROR)*/);
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief     写入RFID

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::DoRW()
	{
		try{
			//CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
			//CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();
			//SC_RES_SET_RFID writeResult;

			//if (model.isBoxAExit ){
			//	CString strIDA = GetEditText(0);
			//	char TicketBoxAID[4];
			//	int2BCD(_ttol(strIDA.Mid(0,2)),TicketBoxAID,1);
			//	int2BCD(_ttol(strIDA.Mid(2,2)),TicketBoxAID+1,1);
			//	WORD boxNum = stoms(_ttol(strIDA.Mid(4)));
			//	memcpy(TicketBoxAID+2,&boxNum,2);
			//	memcpy(model.readBoxAInfo.RFIDData.byDeviceID,&TicketBoxAID,4);
			//	SC_CMD_SET_RFID setRFIDInfo;
			//	setRFIDInfo.magazine = RFID_MAGAZINE_A;
			//	//memcpy(&setRFIDInfo.RFIDData,&model.readBoxAInfo.RFIDData,sizeof(SC_RFID_DATA));
			//	//pTHTestSvc->GetTHHelper()->WriteTicketBoxRFID(setRFIDInfo,writeResult);
			//	model.strBoxA = strIDA;
			//	m_labelTHTestInfo[0].nameRight = model.strBoxA;
			//}

			//if (model.isBoxBExit){
			//	//CString strIDB = GetEditText(1);
			//	//char TicketBoxBID[4];
			//	//int2BCD(_ttol(strIDB.Mid(0,2)),TicketBoxBID,1);
			//	//int2BCD(_ttol(strIDB.Mid(2,2)),TicketBoxBID+1,1);
			//	//WORD boxNum = stoms(_ttol(strIDB.Mid(4)));
			//	//memcpy(TicketBoxBID+2,&boxNum,2);
			//	//memcpy(&model.readBoxBInfo.RFIDData.byDeviceID,&TicketBoxBID,4);
			//	//SC_CMD_SET_RFID setRFIDInfo;
			//	//setRFIDInfo.magazine = RFID_MAGAZINE_B;
			//	//memcpy(&setRFIDInfo.RFIDData,&model.readBoxBInfo.RFIDData,sizeof(SC_RFID_DATA));
			//	//pTHTestSvc->GetTHHelper()->WriteTicketBoxRFID(setRFIDInfo,writeResult);
			//	//model.strBoxB = strIDB;
			//	//m_labelTHTestInfo[1].nameRight = model.strBoxB;
			//}

			//if (model.isBoxCExit){
			//	//CString strIDC = GetEditText(2);
			//	//char TicketBoxCID[4];
			//	//int2BCD(_ttol(strIDC.Mid(0,2)),TicketBoxCID,1);
			//	//int2BCD(_ttol(strIDC.Mid(2,2)),TicketBoxCID+1,1);
			//	//WORD boxNum = stoms(_ttol(strIDC.Mid(4)));
			//	//memcpy(TicketBoxCID+2,&boxNum,2);
			//	//memcpy(&model.readBoxCInfo.RFIDData.byDeviceID,&TicketBoxCID,4);
			//	//SC_CMD_SET_RFID setRFIDInfo;
			//	//setRFIDInfo.magazine = RFID_MAGAZINE_C;
			//	//memcpy(&setRFIDInfo.RFIDData,&model.readBoxCInfo.RFIDData,sizeof(SC_RFID_DATA));
			//	//pTHTestSvc->GetTHHelper()->WriteTicketBoxRFID(setRFIDInfo,writeResult);
			//	//model.strBoxC = strIDC;
			//	//m_labelTHTestInfo[2].nameRight = model.strBoxC;
			//}
			////m_baseInfo->detailInfo.row = 4;
			//m_baseInfoArea->Invalidate();

			//HideCompoment(EDIT_BOX);
		}
		catch (...)	{
			throw;
		}	
	}


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示传感器信息

	@param      无

	@retval     无  

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::ShowSensorInfo()
	{
		try{
			CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
			//SC_RSP_SENSOR_SWITCH_TEST testRes;
			TH_RSP_GET_DEV_STATUS cardSenserStatus = _th_rsp_get_dev_status();
			// 票箱A编号
			pTHTestSvc->GetTHHelper()->GetDevStatus(&cardSenserStatus);
			m_labelTHTestInfo[0].nameLeft = _T("读写器区检测传感器遮挡：");
			m_labelTHTestInfo[0].nameRight = cardSenserStatus.cRwAreaDetect != 0x03?_T("是"):_T("否");
			m_labelTHTestInfo[1].nameLeft = _T("票箱A到位传感器遮挡：");
			m_labelTHTestInfo[1].nameRight = cardSenserStatus.bBoxAArrive?_T("是"):_T("否");
			// 		m_labelTHTestInfo[2].nameLeft = _T("票箱A盖顶传感器遮挡")+_tl(TXT_COLON);
			// 		m_labelTHTestInfo[2].nameRight = cardSenserStatus.bBoxACoverArrive?_T("是"):_T("否");
			m_labelTHTestInfo[2].nameLeft = _T("票箱A将空传感器遮挡：");
			m_labelTHTestInfo[2].nameRight = cardSenserStatus.bBoxANearlyEmpty?_T("是"):_T("否");
			m_labelTHTestInfo[3].nameLeft = _T("票箱A空传感器遮挡：");
			m_labelTHTestInfo[3].nameRight = cardSenserStatus.bBoxAEmpty?_T("是"):_T("否");
			m_labelTHTestInfo[4].nameLeft = _T("票箱B到位传感器遮挡：");
			m_labelTHTestInfo[4].nameRight = cardSenserStatus.bBoxBArrive?_T("是"):_T("否");
			// 		m_labelTHTestInfo[6].nameLeft = _T("票箱B盖顶传感器遮挡")+_tl(TXT_COLON);
			// 		m_labelTHTestInfo[6].nameRight = cardSenserStatus.bBoxBCoverArrive?_T("是"):_T("否");
			m_labelTHTestInfo[5].nameLeft = _T("票箱B将空传感器遮挡：");
			m_labelTHTestInfo[5].nameRight = cardSenserStatus.bBoxBNearlyEmpty?_T("是"):_T("否");
			m_labelTHTestInfo[6].nameLeft = _T("票箱B空传感器遮挡：");
			m_labelTHTestInfo[6].nameRight = cardSenserStatus.bBoxBEmpty?_T("是"):_T("否");
			m_labelTHTestInfo[7].nameLeft = _T("废票箱到位传感器遮挡：");
			m_labelTHTestInfo[7].nameRight = cardSenserStatus.bBoxCArrive?_T("是"):_T("否");
			//m_labelTHTestInfo[10].nameLeft = _T("废票箱盖顶传感器遮挡")+_tl(TXT_COLON);
			//m_labelTHTestInfo[10].nameRight = cardSenserStatus.bBoxCCoverArrive?_T("是"):_T("否");
			m_labelTHTestInfo[8].nameLeft = _T("废票箱满传感器遮挡：");
			m_labelTHTestInfo[8].nameRight = cardSenserStatus.bBoxCFull?_T("是"):_T("否");
			//m_labelTHTestInfo[9].nameLeft = _T("票卡出口检测传感器遮挡：");
			//m_labelTHTestInfo[9].nameRight = cardSenserStatus.bOutShutterDetect?_T("是"):_T("否");
			//m_baseInfo->detailInfo.row = 12;
			m_baseInfoArea->Invalidate();
		}
		catch (...) {
			throw;
		}
	}
