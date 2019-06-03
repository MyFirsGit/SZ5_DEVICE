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

// ��Ϣ��ʾ�༭���������
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

	static const CString STR_SINGLE_TEST_TYPE[] = {_T("������Ʊ����"),_T("���ò���"),_T("����Ʊ�����"),_T("��Ʊ�������"),_T("����������")};
	static const CString STR_TRANSFER_TEST_TYPE[] = {_T("������Ʊ����")};
	static const CString STR_RESET_TEST_TYPE[] = {_T("����")};
	static const CString STR_HANDLE_TEST_TYPE[] = {_T("��Ʊ�������")};
	static const CString STR_MOVE_BOX[] = {_T("Ʊ��A"),_T("Ʊ��B")};
	static const CString STR_SENSOR_TEST_TYPE[] = {_T("������SW����")};
	static const CString STR_RW_TEST[] = {_T("��RFID"),_T("дRFID")};

	static const CString STR_SINGLE_TEST_TYPE_MSG[] = {/*_T("���Ͳ���")*/_T("������Ʊ����"),_T("���ò���"),_T("����Ʊ�����"),_T("��Ʊ�������"),_T("����������")/*,_T("RFID��д")*/};
	static const CString STR_TRANSFER_TEST_TYPE_MSG[] = {_T("������Ʊ����")/*_T("��Ʊ����"),_T("���ղ���")*/};
	static const CString STR_RESET_TEST_TYPE_MSG[] = {_T("����")};
	static const CString STR_HANDLE_TEST_TYPE_MSG[] = {_T("��Ʊ�������")};
	static const CString STR_MOVE_BOX_MSG[] = {_T("Ʊ��1"),_T("Ʊ��2")};
	static const CString STR_SENSOR_TEST_TYPE_MSG[] = {_T("������SW����")};
	static const CString STR_RW_TEST_MSG[] = {_T("��RFID"),_T("дRFID")};


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���캯��

	@param      (i)CService*    pService    �Ի���ʹ�õ�Service

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	CTHTestMaintainDlg::CTHTestMaintainDlg(CService* pService)
		: COperationDlg(CTHTestMaintainDlg::IDD, pService)
	{
		m_baseInfo->titleInfo.titleName = TXT_TH_TEST_SVC_CN;				// ����
		m_baseInfo->detailInfo.row = 10;								    // ����
		m_baseInfo->detailInfo.labelGroup = m_labelTHTestInfo;			// ��ǩ����
		//m_baseInfo->detailInfo.editGroup.editRect = editRect;			// ��������
		m_baseInfo->detailInfo.editGroup.cnt = 0;						// �༭����Ŀ

		m_wUnitTestFirstNo = TEST_NUM_UNKNOWN;
		m_wUnitTestSecondNo = TEST_NUM_UNKNOWN;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��������

	@param      void

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	CTHTestMaintainDlg::~CTHTestMaintainDlg()
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
	@brief		�Ի����ʼ��

	@param      ��

	@retval     BOOL  \n   TRUE:��ʼ���ɹ�  FALSE:��ʼ��ʧ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	BOOL CTHTestMaintainDlg::OnInitDialog()
	{
		COperationDlg::OnInitDialog();

		// ��ʼ��������Ϣ
		InitialBaseInfo();
		// ��ʼ����ť
		InitialButtons();

		return TRUE;  
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief		��ʼ������

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::InitialButtons()
	{
		m_vec_Right_1_BTN_INFO.clear();

		// ���õ�ѡ
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
	@brief		��ʼ��������Ϣ

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::InitialBaseInfo()
	{
		CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
		CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();
		m_baseInfo->titleInfo.titleName = TXT_TH_TEST_SVC_CN;				// ����
		int i = 1;   // ��һ���ǵ���������飬��ս�����������ʾ�����Դ�1��ʼ
		for (;i < 10;i++)
		{
			m_labelTHTestInfo[i].nameLeft = _T("");
			m_labelTHTestInfo[i].nameRight = _T("");
		}
		i=0;

		if (m_wUnitTestFirstNo==RW_RFID_TEST){	
			if(m_wUnitTestSecondNo == R_RFIDNUM_TEST){
				m_labelTHTestInfo[i].nameLeft = "Ʊ��A��";
				m_labelTHTestInfo[i++].nameRight = _T("--");
				m_labelTHTestInfo[i].nameLeft = "Ʊ��B��";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				m_labelTHTestInfo[i].nameLeft = "��Ʊ�䣺";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				// �������ð�ť���༭��
				HideCompoment(BTN_RESET);
				HideCompoment(EDIT_BOX);
			}
			else if(m_wUnitTestSecondNo == W_RFIDNUM_TEST){	
				//m_baseInfo->titleInfo.titleName = TXT_TH_INFO_SVC;				// ����
				m_labelTHTestInfo[i].nameLeft = "Ʊ��A��";
				m_labelTHTestInfo[i++].nameRight = _T("--");
				m_labelTHTestInfo[i].nameLeft = "Ʊ��B��";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				m_labelTHTestInfo[i].nameLeft = "��Ʊ�䣺";
				m_labelTHTestInfo[i++].nameRight = _T("--");	
				//// ����������
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
			// ��ʾBOM���ԣ���ѡ��TH��������
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_CHOOSE_ITEM));

			//��ʼ��lable��Ϣ
			m_labelTHTestInfo[i++].nameLeft = "����������ͣ�";
			m_labelTHTestInfo[i++].nameLeft = "��ϸ�������ͣ�";

			// �������ð�ť���༭��
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
	@brief      ����ȷ�ϰ�ť��Ϣ

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  

	@exception  ��
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
				// �ų�
			case VALIDATE_SEND:
				{
					//scIssueProcess.boxId = 0xff;
					//scIssueProcess.TicketCnt = 1;
					bRecycle = false;
					testCardCount = 1;
					TH_CMD_CARD_OUT OutCardBox = CMD_CARD_OUT_BOX_A;//Ĭ�ϴ�AƱ�俪ʼ��Ʊ

					for (int i=0;i<testCardCount;i++){
						try{

							UINT uiTransportNum = 0;
							pTHTestSvc->GetTHHelper()->CardOut(OutCardBox,&uiTransportNum);// ����ץƱ
							pTHTestSvc->GetTHHelper()->SendCard(&uiTransportNum);// ����ץƱ
						}
						catch(CSysException& e){
							//LOG("��Ʊ���쳣������");
							theEXCEPTION_MGR.WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
							// Ʊ��1�ѿ�(A)
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
				// ����
			case INVALID_TEST:
				{
					//scIssueProcess.boxId = 0xff;
					//scIssueProcess.TicketCnt = 1;
					bRecycle = true;
					TH_CMD_CARD_OUT OutCardBox = CMD_CARD_OUT_BOX_A;//Ĭ�ϴ�AƱ�俪ʼ��Ʊ
					testCardCount = 1;

					for (int i=0;i<testCardCount;i++){
						try{
							UINT uiTransportNum = 0;
							pTHTestSvc->GetTHHelper()->CardOut(OutCardBox,&uiTransportNum);
							pTHTestSvc->GetTHHelper()->RetractCard(&uiTransportNum);
							if (OutCardBox = CMD_CARD_OUT_BOX_A)
							{
								theTICKET_HELPER.UpdateTHCount(uiTransportNum,0,uiTransportNum); //���»��ճ�Ʊ����
							} 
							else
							{
								theTICKET_HELPER.UpdateTHCount(0,uiTransportNum,uiTransportNum); //���»��ճ�Ʊ����
							}							
							//theTICKET_HELPER.UpdateTHCount(thRspInfo.boxIdACount,thRspInfo.boxIdBCount,thRspInfo.issueRes==3?1:0); //���»��ճ�Ʊ����	  
						}
						catch(CSysException& e){
							//LOG("��Ʊ���쳣������");
							theEXCEPTION_MGR.WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
							// Ʊ��1�ѿ�(A)
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
				// �޸���Ʊ��
				pTHTestSvc->GetTHHelper()->Repare();
				break;
				// ����AƱ��
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
				// ����BƱ��
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
				// ����������
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
	@brief      ������԰�ť

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

	@exception  ��
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
				// ���Ͳ���
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
				// ���ò���
			case RESET_TEST:
				for (int i = 0; i < 1; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 21;
					pDetailTestTypeBtnInfo.btnMsg = STR_RESET_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_RESET_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				theAPP_SESSION.ShowOfficeGuide(_T("����ȷ�ϼ����в��ԡ�"));
				m_wUnitTestFirstNo = RESET_TEST;
				break;
				// ����Ʊ�����
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

				// ��Ʊ�������
			case INVALID_HANDLE_TEST:
				for (int i = 0; i < 1; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 41;
					pDetailTestTypeBtnInfo.btnMsg = STR_HANDLE_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_HANDLE_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				//theAPP_SESSION.ShowOfficeGuide(_T("����ȷ�ϼ����в��ԡ�"));
				m_wUnitTestFirstNo = INVALID_HANDLE_TEST;
				break;
				// ��дRFID
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
				// ����������
			case SENSOR_TEST:
				for (int i = 0; i < 1; i++){
					BTN_INFO pDetailTestTypeBtnInfo;
					pDetailTestTypeBtnInfo.btnID = i + 51;
					pDetailTestTypeBtnInfo.btnMsg = STR_SENSOR_TEST_TYPE_MSG[i];
					pDetailTestTypeBtnInfo.text = STR_SENSOR_TEST_TYPE[i];
					m_vec_Right_2_BTN_INFO.push_back(pDetailTestTypeBtnInfo);
				}
				theAPP_SESSION.ShowOfficeGuide(_T("����ȷ�ϼ����в��ԡ�"));
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
	@brief      ��ϸ���԰�ť

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CTHTestMaintainDlg::OnDetailTestTypeBtnClick(WPARAM wParam,LPARAM lParam)
	{
		try{
			BTN_INFO* btnInfo = (BTN_INFO*)lParam;

			SINGLE_TEST_TYPE detailTestType = (SINGLE_TEST_TYPE)btnInfo->btnID;
			m_wUnitTestSecondNo = detailTestType;
			// ��ʼ��������Ϣ
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
	@brief      ��ʾƱ����Ϣ

	@param      ��

	@retval     ��  

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::ShowRFIDInfo()
	{
		try{
			//CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
			//CTHTestSvc::Model& model = pTHTestSvc->GetDataModel();
			//// Ʊ��A���
			//model.readBoxAInfo.boxType = RFID_MAGAZINE_A;
			//tTIMRFIDInfo TimRfidInfo;
			////model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID(model.readBoxAInfo);	
			//model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID((TH_BOX_TYPE)RFID_MAGAZINE_A, &TimRfidInfo);	
			//if (model.isBoxAExit ){	
			//	model.strBoxA.Format(_T("%02X%02X%04d"),model.readBoxAInfo.RFIDData.byDeviceID[0],model.readBoxAInfo.RFIDData.byDeviceID[1],ComMakeWORD(model.readBoxAInfo.RFIDData.byDeviceID[2],model.readBoxAInfo.RFIDData.byDeviceID[3]));
			//	SetEditText(0,model.strBoxA);
			//	m_labelTHTestInfo[0].nameRight = model.strBoxA;
			//}

			//// Ʊ��B���
			//model.readBoxBInfo.boxType = RFID_MAGAZINE_B;
			////model.isBoxBExit  = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID(model.readBoxBInfo);	
			//model.isBoxAExit = pTHTestSvc->GetTHHelper()->ReadTicketBoxRFID((TH_BOX_TYPE)RFID_MAGAZINE_B, &TimRfidInfo);
			//if (model.isBoxBExit ){	
			//	model.strBoxB.Format(_T("%02X%02X%04d"),model.readBoxBInfo.RFIDData.byDeviceID[0],model.readBoxBInfo.RFIDData.byDeviceID[1],ComMakeWORD(model.readBoxBInfo.RFIDData.byDeviceID[2],model.readBoxBInfo.RFIDData.byDeviceID[3]));
			//	SetEditText(1,model.strBoxB);
			//	m_labelTHTestInfo[1].nameRight = model.strBoxB;
			//}

			//// ��Ʊ����
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
	@brief      ����ȷ�ϡ����ء����� ��ť��Ϣ

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  

	@exception  ��
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
	@brief      ���EDIT����������Ƿ�Ϸ�

	@param      void

	@retval     bool   true �Ϸ�  false �Ƿ�

	@exception  ��
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
			theAPP_SESSION.ShowOfficeGuide(_T("���죩����������д�����˶Ժ���������")/*theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_SET_INPUT_ERROR)*/);
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief     д��RFID

	@param      ��

	@retval     ��

	@exception  ��
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
	@brief      ��ʾ��������Ϣ

	@param      ��

	@retval     ��  

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CTHTestMaintainDlg::ShowSensorInfo()
	{
		try{
			CTHTestSvc* pTHTestSvc = (CTHTestSvc*)GetService();
			//SC_RSP_SENSOR_SWITCH_TEST testRes;
			TH_RSP_GET_DEV_STATUS cardSenserStatus = _th_rsp_get_dev_status();
			// Ʊ��A���
			pTHTestSvc->GetTHHelper()->GetDevStatus(&cardSenserStatus);
			m_labelTHTestInfo[0].nameLeft = _T("��д������⴫�����ڵ���");
			m_labelTHTestInfo[0].nameRight = cardSenserStatus.cRwAreaDetect != 0x03?_T("��"):_T("��");
			m_labelTHTestInfo[1].nameLeft = _T("Ʊ��A��λ�������ڵ���");
			m_labelTHTestInfo[1].nameRight = cardSenserStatus.bBoxAArrive?_T("��"):_T("��");
			// 		m_labelTHTestInfo[2].nameLeft = _T("Ʊ��A�Ƕ��������ڵ�")+_tl(TXT_COLON);
			// 		m_labelTHTestInfo[2].nameRight = cardSenserStatus.bBoxACoverArrive?_T("��"):_T("��");
			m_labelTHTestInfo[2].nameLeft = _T("Ʊ��A���մ������ڵ���");
			m_labelTHTestInfo[2].nameRight = cardSenserStatus.bBoxANearlyEmpty?_T("��"):_T("��");
			m_labelTHTestInfo[3].nameLeft = _T("Ʊ��A�մ������ڵ���");
			m_labelTHTestInfo[3].nameRight = cardSenserStatus.bBoxAEmpty?_T("��"):_T("��");
			m_labelTHTestInfo[4].nameLeft = _T("Ʊ��B��λ�������ڵ���");
			m_labelTHTestInfo[4].nameRight = cardSenserStatus.bBoxBArrive?_T("��"):_T("��");
			// 		m_labelTHTestInfo[6].nameLeft = _T("Ʊ��B�Ƕ��������ڵ�")+_tl(TXT_COLON);
			// 		m_labelTHTestInfo[6].nameRight = cardSenserStatus.bBoxBCoverArrive?_T("��"):_T("��");
			m_labelTHTestInfo[5].nameLeft = _T("Ʊ��B���մ������ڵ���");
			m_labelTHTestInfo[5].nameRight = cardSenserStatus.bBoxBNearlyEmpty?_T("��"):_T("��");
			m_labelTHTestInfo[6].nameLeft = _T("Ʊ��B�մ������ڵ���");
			m_labelTHTestInfo[6].nameRight = cardSenserStatus.bBoxBEmpty?_T("��"):_T("��");
			m_labelTHTestInfo[7].nameLeft = _T("��Ʊ�䵽λ�������ڵ���");
			m_labelTHTestInfo[7].nameRight = cardSenserStatus.bBoxCArrive?_T("��"):_T("��");
			//m_labelTHTestInfo[10].nameLeft = _T("��Ʊ��Ƕ��������ڵ�")+_tl(TXT_COLON);
			//m_labelTHTestInfo[10].nameRight = cardSenserStatus.bBoxCCoverArrive?_T("��"):_T("��");
			m_labelTHTestInfo[8].nameLeft = _T("��Ʊ�����������ڵ���");
			m_labelTHTestInfo[8].nameRight = cardSenserStatus.bBoxCFull?_T("��"):_T("��");
			//m_labelTHTestInfo[9].nameLeft = _T("Ʊ�����ڼ�⴫�����ڵ���");
			//m_labelTHTestInfo[9].nameRight = cardSenserStatus.bOutShutterDetect?_T("��"):_T("��");
			//m_baseInfo->detailInfo.row = 12;
			m_baseInfoArea->Invalidate();
		}
		catch (...) {
			throw;
		}
	}
