#include "stdafx.h"
#include "BanknoteBoxCollectionSvc.h"
#include "BanknoteBoxCollectionDlg.h"
#include "CTBanknoteCountInfo.h"
#include "TemplateHelper.h"
//#include "CHCountInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static const TCHAR* const WM_CBMGR_BASE_TO_OPERATION = _T("WM_CBMGR_BASE_TO_OPERATION");
//static const TCHAR* const WM_CBMGR_OPERATION_TO_FINISH = _T("WM_CBMGR_OPERATION_TO_FINISH");
//static const TCHAR* const WM_CBMGR_OPERATION_TO_ERROR = _T("WM_CBMGR_OPERATION_TO_ERROR");
//static const TCHAR* const WM_CBMGR_OPERATION_TO_BASE = _T("WM_CBMGR_OPERATION_TO_BASE");
//static const TCHAR* const WM_CBMGR_FINISH_TO_BASE = _T("WM_CBMGR_FINISH_TO_BASE");
//static const TCHAR* const WM_CBMGR_ERROR_TO_BASE = _T("WM_CBMGR_ERROR_TO_BASE");

BEGIN_MESSAGE_MAP(CBanknoteBoxCollectionSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxCollectionSvc::CBanknoteBoxCollectionSvc()
	:CTVMForeService(BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC)
{
    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CBanknoteBoxCollectionDlg(this));

    // ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F3_ESC));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CBanknoteBoxCollectionDlg::IDD);

	/*m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION, CBanknoteBoxCollectionDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, CBanknoteBoxCollectionDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, CBanknoteBoxCollectionDlg::IDD);*/

	// ���û�����ת
	/*m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_CBMGR_BASE_TO_OPERATION, DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_FINISH, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_FINISH, WM_CBMGR_FINISH_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ERROR, WM_CBMGR_ERROR_TO_BASE, DIALOG_GROUP_BASE);*/

    // ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxCollectionSvc::~CBanknoteBoxCollectionSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxCollectionSvc::Model& CBanknoteBoxCollectionSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxCollectionSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteBoxCollectionSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//theAPP_SESSION.ReturnMaintainMenu();
	int nID = GetCurDialogGroupID();
	switch(nID)
	{
	case DIALOG_GROUP_BASE:
		if(!theSERVICE_MGR.IsForeServiceBusy())
			theAPP_SESSION.ReturnMaintainMenu();
		break;
		/*case DIALOG_GROUP_OPERATION:
		m_DataModel = _tagModel();
		DoDialogFlow(WM_CBMGR_OPERATION_TO_BASE);
		break;
		case DIALOG_GROUP_FINISH:
		DoDialogFlow(WM_CBMGR_FINISH_TO_BASE);
		break;
		case DIALOG_GROUP_ERROR:
		DoDialogFlow(WM_CBMGR_ERROR_TO_BASE);
		break;
		default:
		theAPP_SESSION.ReturnMaintainMenu();
		break;*/
	}
	NotifyDataModelChanged();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteBoxCollectionSvc::OnKeyboardF2(WPARAM , LPARAM ){
	// ���ո���ģ���е�ֽ�ҵ���������ȥ
	// ��λ��û�м�鵽����״̬����ʾ���Ӳ���λ���޷���װ

	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RECOVERY));
	// ��ֹֽ��ģ��״̬�����⣬���»���ʧ�ܡ�
	try{
		BANKNOTE_HELPER->Repare(true);
	}
	catch(CSysException&e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_FAULT));
		theSERVICE_MGR.SetForeServiceBusy(false);
		return FALSE;
	}
	theSERVICE_MGR.SetForeServiceBusy(false);

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO collBox = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	if(!BANKNOTE_HELPER->IsCashBoxAttached() || collBox.banknoteBoxID.ToString() == _T("FFFFFFFF")){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_STANDBY_FOR_UNINSTALL));
		return FALSE;
	}

	//int nBillFullCnt = theAFC_TVM_OPERATION.GetRecycleBoxFullCount();
	//DWORD dwLaftCount = recyclerA.BanknotesTotalCount() + recyclerB.BanknotesTotalCount() + recyclerC.BanknotesTotalCount() + recyclerD.BanknotesTotalCount() + changeBox.BanknotesTotalCount();
	//if (nBillFullCnt <= (dwLaftCount + collBox.BankntoeTotalCount()))
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_T("�ɻ���ֽ���������ޣ�����������䡣"));
	//	return FALSE;
	//}
	//nBillFullCnt -= collBox.BankntoeTotalCount();

	theSERVICE_MGR.SetForeServiceBusy(true);
	try{

/*		if(dwLaftCount > 0 && nBillFullCnt > 0)*/
		{

			LOG("-------------------------ֽ������ǰ--------------------------");
			LOG("ѭ����A������%d",	recyclerA.ulCurCount);
			LOG("ѭ����B������%d",	recyclerB.ulCurCount);
			LOG("ѭ����C������%d",	recyclerC.ulCurCount);
			LOG("ѭ����D������%d",	recyclerD.ulCurCount);
			LOG("������������%d",	changeBox.ulCurCount);
			LOG("������1Ԫ������%d",	collBox.ulCount_CNY1);
			LOG("������5Ԫ������%d",	collBox.ulCount_CNY5);
			LOG("������10Ԫ������%d",	collBox.ulCount_CNY10);
			LOG("������20Ԫ������%d",	collBox.ulCount_CNY20);
			LOG("������50Ԫ������%d",	collBox.ulCount_CNY50);
			LOG("������100Ԫ������%d",	collBox.ulCount_CNY100);


			tBncChangeNum result;

			long lErrCode = 0;
			map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> cashUnit = BANKNOTE_HELPER->GetEmptyFull();
			lErrCode = BANKNOTE_HELPER->Empty((char*)"R",&result);
			if(/*recyclerA.ulCurCount > 0 || */cashUnit[BNR_RECYCLER_BOX_A].count > 0){
				//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_A_NAME,&result);
				if(lErrCode == 0){
					LOG("Empty_A: %d", result.uiOutNumber[0]);
					if (theTVM_SETTING.GetBNRRecycleBoxAAmount()==10){
						collBox.ulCount_CNY10 += result.uiOutNumber[0];
					}
					else if (theTVM_SETTING.GetBNRRecycleBoxAAmount()==5){
						collBox.ulCount_CNY5 += result.uiOutNumber[0];
					}							
				}
				//nBillFullCnt -= result.wRecyclerAOutCount;

				recyclerA.ulCurCount -= result.uiOutNumber[0];
				if(recyclerA.ulCurCount <0 )recyclerA.ulCurCount = 0;
				else if(recyclerA.ulCurCount > 0 && lErrCode == 0)recyclerA.ulCurCount = 0;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(recyclerA);

			}

			if(/*nBillFullCnt > 0 && (recyclerB.ulCurCount > 0 || */cashUnit[BNR_RECYCLER_BOX_B].count > 0/*)*/){
				//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_B_NAME,&result);

				if(lErrCode == 0){
					LOG("Empty_B: %d", result.uiOutNumber[1]);
					//collBox.ulCount_CNY5 += result.wRecyclerBOutCount;
					if (theTVM_SETTING.GetBNRRecycleBoxBAmount()==10){
						collBox.ulCount_CNY10 += result.uiOutNumber[1];
					}
					else if (theTVM_SETTING.GetBNRRecycleBoxBAmount()==5){
						collBox.ulCount_CNY5 += result.uiOutNumber[1];
					}	
				}
				//nBillFullCnt -= result.wRecyclerBOutCount;
				recyclerB.ulCurCount -= result.uiOutNumber[1];
				if(recyclerB.ulCurCount <0 )recyclerB.ulCurCount = 0;
				else if(recyclerB.ulCurCount > 0 && lErrCode == 0)recyclerB.ulCurCount = 0;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(recyclerB);
			}
			if(/*nBillFullCnt > 0 && (recyclerC.ulCurCount > 0 || */cashUnit[BNR_RECYCLER_BOX_C].count > 0/*)*/){
				//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_C_NAME,&result);

				if(lErrCode == 0){
					LOG("Empty_C: %d", result.uiOutNumber[2]);
					if (theTVM_SETTING.GetBNRRecycleBoxCAmount()==10){
						collBox.ulCount_CNY10 += result.uiOutNumber[2];
					}
					else if (theTVM_SETTING.GetBNRRecycleBoxCAmount()==5){
						collBox.ulCount_CNY5 += result.uiOutNumber[2];
					}	
				}
				//nBillFullCnt -= result.wRecyclerCOutCount;
				recyclerC.ulCurCount -= result.uiOutNumber[2];
				if(recyclerC.ulCurCount <0 )recyclerC.ulCurCount = 0;
				else if(recyclerC.ulCurCount > 0 && lErrCode == 0)recyclerC.ulCurCount = 0;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(recyclerC);
			}
			if(/*nBillFullCnt > 0 && (recyclerD.ulCurCount > 0 || */cashUnit[BNR_RECYCLER_BOX_D].count > 0/*)*/){
				//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_D_NAME,&result);

				if(lErrCode == 0){
					LOG("Empty_D: %d", result.uiOutNumber[3]);
					//collBox.ulCount_CNY10 += result.wRecyclerDOutCount;
					if (theTVM_SETTING.GetBNRRecycleBoxDAmount()==10){
						collBox.ulCount_CNY10 += result.uiOutNumber[3];
					}
					else if (theTVM_SETTING.GetBNRRecycleBoxDAmount()==5){
						collBox.ulCount_CNY5 += result.uiOutNumber[3];
					}	
				}
				//nBillFullCnt -= result.wRecyclerDOutCount;
				recyclerD.ulCurCount -= result.uiOutNumber[3];
				if(recyclerD.ulCurCount <0 )recyclerD.ulCurCount = 0;
				else if(recyclerD.ulCurCount > 0 && lErrCode == 0)recyclerD.ulCurCount = 0;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(recyclerD);
			}
			if (BANKNOTE_HELPER->IsCashChangeBoxAttached()/* && nBillFullCnt > 0 && (changeBox.ulCurCount > 0 || cashUnit[BNR_MODULE_CHANGE_BOX].count > 0)*/)
			{
				//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_LOADER_BOX_NAME,&result);

				if(lErrCode == 0){
					LOG("Empty_L: total-%d", result.uiOutNumber[4]);
					//collBox.ulCount_CNY5 += result.dwOutAmount/500;
					if (theTVM_SETTING.GetBNRChangeBoxAmount()==10){
						collBox.ulCount_CNY10 += result.uiOutNumber[4];
						changeBox.ulCurCount -= result.uiOutNumber[4];
					}
					else if (theTVM_SETTING.GetBNRChangeBoxAmount()==5){
						collBox.ulCount_CNY5 += result.uiOutNumber[4];
						changeBox.ulCurCount -= result.uiOutNumber[4];
					}	
				}

				if(changeBox.ulCurCount <0 )changeBox.ulCurCount = 0;
				else if(changeBox.ulCurCount > 0 && lErrCode == 0)changeBox.ulCurCount = 0;
				theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBox);

			}
		}
		//else{
		//	if (nBillFullCnt <= 0)
		//	{
		//		theAPP_SESSION.ShowOfficeGuide(_T("������������ֽ��δ��ȫ���գ����������������ԡ�"));
		//	}
		//	else
		//	{
		//		theAPP_SESSION.ShowOfficeGuide(_T("ֽ��ѭ��ģ����ֽ�ҿ��Ի��ա�[F3/Ctrl]���أ�[ESC]�˳���"));
		//	}
		//}
	}
	catch(...){
		
	}
	theSERVICE_MGR.SetForeServiceBusy(false);

	LOG("-------------------------ֽ�������--------------------------");
	LOG("ѭ����A������%d",	recyclerA.ulCurCount);
	LOG("ѭ����B������%d",	recyclerB.ulCurCount);
	LOG("ѭ����C������%d",	recyclerC.ulCurCount);
	LOG("ѭ����D������%d",	recyclerD.ulCurCount);
	LOG("������������%d",	changeBox.ulCurCount);
	LOG("������1Ԫ������%d",	collBox.ulCount_CNY1);
	LOG("������5Ԫ������%d",	collBox.ulCount_CNY5);
	LOG("������10Ԫ������%d",	collBox.ulCount_CNY10);
	LOG("������20Ԫ������%d",	collBox.ulCount_CNY20);
	LOG("������50Ԫ������%d",	collBox.ulCount_CNY50);
	LOG("������100Ԫ������%d",	collBox.ulCount_CNY100);

	// ͬ������ֽ�һ���ģ������
	theBANKNOTE_COUNT.SetBanknoteboxInfo(collBox);
	//if (nBillFullCnt <= 0)
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_T("������������ֽ��δ��ȫ���գ����������������ԡ�"));
	//}
	//else
	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_FROM_CLEAR_OK_F3_ESC));

	m_DataModel.banknoteCashInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.banknoteChangeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	m_DataModel.cycleBoxAInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	m_DataModel.cycleBoxBInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	m_DataModel.cycleBoxCInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	m_DataModel.cycleBoxDInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
	PrintBanknoteCollectionReception();
	NotifyDataModelChanged();


	// ֻ���ڲ����������ӦF2
	//if(GetCurDialogGroupID() != DIALOG_GROUP_OPERATION){
	//	return FALSE;
	//}
	//try{
	//	// ֽ���䰲װ����
	//	if (m_DataModel.operation_type == OPERATION_ATTACH_BOX && m_DataModel.bIsSelectInstall){
	//		if ( IsBoxConnected()){
	//			DoHandleAttachBox();
	//		}
	//		else{
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FORBBIDEN)/*T("��ֽ����������쳣����ǰ��֧�ְ�װ������")*/);
	//		}
	//	}
	//	// ֽ����ж�ز���
	//	else if (m_DataModel.operation_type == OPERATION_DETACH_BOX && m_DataModel.bIsSelectUninstall){
	//		DoHandleDetachBox();		
	//	}
	//}//�쳣����
	//catch (CSysException& e) {
	//	// д������־
	//	CString strGuide = _T("");
	//	if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
	//		if (m_DataModel.banknote_box.now_attached)
	//		{
	//			strGuide = _T("��װ�ɹ�");
	//		}
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
	//	}
	//	else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
	//		if (!m_DataModel.banknote_box.now_attached)
	//		{
	//			strGuide = _T("ж�سɹ�");
	//		}
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
	//	}
	//	theEXCEPTION_MGR.ProcessException(e);
	//	CString str;
	//	//if(e.GetModuleID() == CTVMPrinterException::MODULE_ID )
	//	//{
	//	//	str.Format(_T("%s��ƾ����ӡʧ�ܡ�."),strGuide);
	//	//	theAPP_SESSION.ShowOfficeGuide(str);
	//	//}else//
	//	//{
	//	//	str.Format(_T("%s�����ݿ�д��ʧ�ܡ�."),strGuide);
	//	//	theAPP_SESSION.ShowOfficeGuide(_T("���ݿ�д��ʧ��"));
	//	//}
	//}
	//catch (...) {
	//	// д������־
	//	if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
	//	}
	//	else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
	//	}
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	//}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteBoxCollectionSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//try{
	//	// �������棨�˵�ѡ��
	//	if (GetCurDialogGroupID() == DIALOG_GROUP_BASE){
	//		m_DataModel.bIsSelectInstall = false;
	//		m_DataModel.bIsSelectUninstall = false;
	//		DoHandleBase();
	//	}
	//	// �������棨��װ��ж�أ�
	//	else if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION){
	//		int index = theAPP_SESSION.GetInputToInt();
	//		if(MENU_CODE_1 != index){
	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//			return FALSE;
	//		}
	//		// ֽ���䰲װ����
	//		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
	//			// ��ʾ��װֽ����Ϊѡ��״̬
	//			if(!m_DataModel.banknote_box.raw_attached){
	//				m_DataModel.bIsSelectInstall = true;
	//				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//			}
	//			else{// ��ʾ�Ѿ���װ������Ҫ�ٰ�װ
	//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_INSTALL));
	//			}				
	//		}
	//		// ֽ����ж�ز���
	//		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
	//			if(m_DataModel.banknote_box.raw_attached){
	//				m_DataModel.bIsSelectUninstall = true;
	//				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//			}
	//			else{// ��ʾ�Ѿ�ж�أ������ظ�ж��
	//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_UNINSTALL));
	//			}
	//		}
	//		NotifyDataModelChanged();
	//	}
	//}
	////�쳣����
	//catch (CSysException& e) {
	//	// д������־
	//	CString strGuide = _T("");
	//	if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
	//		if (m_DataModel.banknote_box.now_attached)
	//		{
	//			strGuide = _T("��װ�ɹ�");
	//		}
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
	//	}
	//	else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
	//		if (!m_DataModel.banknote_box.now_attached)
	//		{
	//			strGuide = _T("ж�سɹ�");
	//		}
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
	//	}
	//	theEXCEPTION_MGR.ProcessException(e);
	//	CString str;
	//	//if(e.GetModuleID() == CTVMPrinterException::MODULE_ID )
	//	//{
	//	//	str.Format(_T("%s��ƾ����ӡʧ�ܡ�."),strGuide);
	//	//	 theAPP_SESSION.ShowOfficeGuide(str);
	//	//}else//
	//	//{
	//	//	str.Format(_T("%s�����ݿ�д��ʧ�ܡ�."),strGuide);
	//	//	theAPP_SESSION.ShowOfficeGuide(_T("���ݿ�д��ʧ��"));
	//	//}
	//}
	//catch (...) {
	//	// д������־
	//	if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
	//	}
	//	else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
	//	}
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	//}
	return TRUE;
}

//ֽ�һ���ƾ֤
void CBanknoteBoxCollectionSvc::PrintBanknoteCollectionReception()
{
	try
	{
		// ����Template
		CString CoinBox_templatename = _T("TVM_BANK_NOTE_COLLECT.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{��վ����}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{����Ա}"), theAPP_SESSION.GetUserInfo().sUserID);
			line.Replace(_T("{�豸��}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());	
			if(line.Find(_T("ѭ��������1"))>=0){
				line.Replace(_T("{����ǰ���}"), ComFormatAmount(m_DataModel.Ex_cycleBoxAInfo.BankNotesCountAvos()));
				line.Replace(_T("{��������}"), ComFormatAmount(m_DataModel.cycleBoxAInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("ѭ��������2"))>=0){
				line.Replace(_T("{����ǰ���}"), ComFormatAmount(m_DataModel.Ex_cycleBoxBInfo.BankNotesCountAvos()));
				line.Replace(_T("{��������}"), ComFormatAmount(m_DataModel.cycleBoxBInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("ѭ��������3"))>=0){
				line.Replace(_T("{����ǰ���}"), ComFormatAmount(m_DataModel.Ex_cycleBoxCInfo.BankNotesCountAvos()));
				line.Replace(_T("{��������}"), ComFormatAmount(m_DataModel.cycleBoxCInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("ѭ��������4"))>=0){
				line.Replace(_T("{����ǰ���}"), ComFormatAmount(m_DataModel.Ex_cycleBoxDInfo.BankNotesCountAvos()));
				line.Replace(_T("{��������}"), ComFormatAmount(m_DataModel.cycleBoxDInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("�ϼ�"))>=0){
				int nTotalBefore = m_DataModel.Ex_cycleBoxAInfo.BankNotesCountAvos() + m_DataModel.Ex_cycleBoxBInfo.BankNotesCountAvos()
					+ m_DataModel.Ex_cycleBoxCInfo.BankNotesCountAvos() + m_DataModel.Ex_cycleBoxDInfo.BankNotesCountAvos();
				int nTotalAfter = m_DataModel.cycleBoxAInfo.BankNotesCountAvos() + m_DataModel.cycleBoxBInfo.BankNotesCountAvos()
					+ m_DataModel.cycleBoxCInfo.BankNotesCountAvos() + m_DataModel.cycleBoxDInfo.BankNotesCountAvos();
				line.Replace(_T("{����ǰ���}"), ComFormatAmount(nTotalBefore));
				line.Replace(_T("{��������}"), ComFormatAmount(nTotalAfter));
			}
			line.Replace(_T("{����}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray saText;
			ComPrintLineFormatForTemplate(line, saText);
			printtext.Append(saText);
			Index++;
		}
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);//��ӡƾ֤
	}
	catch (CSysException& e)
	{
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_PRINT_FAILED));
	}
	catch (...)
	{
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__,_T("ƾ֤��ӡʧ��"));
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(LANGUAGE_RED) + _T("���ճɹ���ƾ֤��ӡʧ�ܡ�"));
	}
	
	
}
CString CBanknoteBoxCollectionSvc::FormatAmount(long amount)
{
	switch(amount)
	{
	case 100:return _T(" 1Ԫ");
	case 500:return _T(" 5Ԫ");
	case 1000:return _T("10Ԫ");
	case 2000:return _T("20Ԫ");
	case 5000:return _T("50Ԫ");
	case 10000:return _T("100Ԫ");
	}
	return _T("");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Service����ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxCollectionSvc::OnStart()
{
	__super::OnStart();
	m_DataModel = _tagModel();
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	m_pDialogFlow->ShowFirstDialogGroup();
	//IsBoxConnected();
	//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
	theAPP_SESSION.ShowOfficeGuide(TXT_COINMGR_SHOW_MSG_SAVE_INPUT_COLLECTION);
	m_DataModel.Ex_banknoteCashInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.Ex_banknoteChangeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	m_DataModel.Ex_cycleBoxAInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	m_DataModel.Ex_cycleBoxBInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	m_DataModel.Ex_cycleBoxCInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	m_DataModel.Ex_cycleBoxDInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

	m_DataModel.banknoteCashInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.banknoteChangeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	m_DataModel.cycleBoxAInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	m_DataModel.cycleBoxBInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	m_DataModel.cycleBoxCInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	m_DataModel.cycleBoxDInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
}