#include "stdafx.h"
#include "BanknoteBoxManageSvc.h"
#include "BanknoteBoxManageBaseDlg.h"
#include "CTBanknoteCountInfo.h"
#include "TemplateHelper.h"
//#include "CHCountInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_CBMGR_BASE_TO_OPERATION = _T("WM_CBMGR_BASE_TO_OPERATION");
static const TCHAR* const WM_CBMGR_OPERATION_TO_FINISH = _T("WM_CBMGR_OPERATION_TO_FINISH");
static const TCHAR* const WM_CBMGR_OPERATION_TO_ERROR = _T("WM_CBMGR_OPERATION_TO_ERROR");
static const TCHAR* const WM_CBMGR_OPERATION_TO_BASE = _T("WM_CBMGR_OPERATION_TO_BASE");
static const TCHAR* const WM_CBMGR_FINISH_TO_BASE = _T("WM_CBMGR_FINISH_TO_BASE");
static const TCHAR* const WM_CBMGR_ERROR_TO_BASE = _T("WM_CBMGR_ERROR_TO_BASE");

BEGIN_MESSAGE_MAP(CBanknoteBoxManageSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxManageSvc::CBanknoteBoxManageSvc()
	:CTVMForeService(BANKNOTE_BOX_MANAGE_SVC)
{
    // ���ò���Ա����
    m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CBanknoteBoxManageBaseDlg(this));

    // ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F3_ESC));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CBanknoteBoxManageBaseDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION, CBanknoteBoxManageBaseDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, CBanknoteBoxManageBaseDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, CBanknoteBoxManageBaseDlg::IDD);

	// ���û�����ת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_CBMGR_BASE_TO_OPERATION, DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_FINISH, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_FINISH, WM_CBMGR_FINISH_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ERROR, WM_CBMGR_ERROR_TO_BASE, DIALOG_GROUP_BASE);

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
CBanknoteBoxManageSvc::~CBanknoteBoxManageSvc()
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
CBanknoteBoxManageSvc::Model& CBanknoteBoxManageSvc::GetDataModel()
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
void CBanknoteBoxManageSvc::NotifyDataModelChanged()
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
LRESULT CBanknoteBoxManageSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//theAPP_SESSION.ReturnMaintainMenu();
	int nID = GetCurDialogGroupID();
	switch(nID)
	{
	case DIALOG_GROUP_BASE:
		theAPP_SESSION.ReturnMaintainMenu();
		break;
	case DIALOG_GROUP_OPERATION:
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
		break;
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
LRESULT CBanknoteBoxManageSvc::OnKeyboardF2(WPARAM , LPARAM ){
	// ֻ���ڲ����������ӦF2
	if(GetCurDialogGroupID() != DIALOG_GROUP_OPERATION){
		return FALSE;
	}
	try{
		// ֽ���䰲װ����
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX && m_DataModel.bIsSelectInstall){
			if ( IsBoxConnected()){
				DoHandleAttachBox();
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FORBBIDEN)/*T("��ֽ����������쳣����ǰ��֧�ְ�װ������")*/);
			}
		}
		// ֽ����ж�ز���
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX && m_DataModel.bIsSelectUninstall){
			DoHandleDetachBox();		
		}
	}//�쳣����
	catch (CSysException& e) {
		// д������־
		CString strGuide = _T("");
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
			if (m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("��װ�ɹ�");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		}
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
			if (!m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("ж�سɹ�");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
		}
		theEXCEPTION_MGR.ProcessException(e);
		CString str;
		//if(e.GetModuleID() == CTVMPrinterException::MODULE_ID )
		//{
		//	str.Format(_T("%s��ƾ����ӡʧ�ܡ�."),strGuide);
		//	theAPP_SESSION.ShowOfficeGuide(str);
		//}else//
		//{
		//	str.Format(_T("%s�����ݿ�д��ʧ�ܡ�."),strGuide);
		//	theAPP_SESSION.ShowOfficeGuide(_T("���ݿ�д��ʧ��"));
		//}
	}
	catch (...) {
		// д������־
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		}
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
		}
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
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
LRESULT CBanknoteBoxManageSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	try{
		// �������棨�˵�ѡ��
		if (GetCurDialogGroupID() == DIALOG_GROUP_BASE){
			m_DataModel.bIsSelectInstall = false;
			m_DataModel.bIsSelectUninstall = false;
			DoHandleBase();
		}
		// �������棨��װ��ж�أ�
		else if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION){
			int index = theAPP_SESSION.GetInputToInt();
			if(MENU_CODE_1 != index){
				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				return FALSE;
			}
			// ֽ���䰲װ����
			if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
				// ��ʾ��װֽ����Ϊѡ��״̬
				if(!m_DataModel.banknote_box.raw_attached){
					m_DataModel.bIsSelectInstall = true;
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				}
				else{// ��ʾ�Ѿ���װ������Ҫ�ٰ�װ
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_INSTALL));
				}				
			}
			// ֽ����ж�ز���
			else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
				if(m_DataModel.banknote_box.raw_attached){
					m_DataModel.bIsSelectUninstall = true;
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				}
				else{// ��ʾ�Ѿ�ж�أ������ظ�ж��
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_UNINSTALL));
				}
			}
			NotifyDataModelChanged();
		}
	}
	//�쳣����
	catch (CSysException& e) {
		// д������־
		CString strGuide = _T("");
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
			if (m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("��װ�ɹ�");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		}
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
			if (!m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("ж�سɹ�");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
		}
		theEXCEPTION_MGR.ProcessException(e);
		CString str;
		//if(e.GetModuleID() == CTVMPrinterException::MODULE_ID )
		//{
		//	str.Format(_T("%s��ƾ����ӡʧ�ܡ�."),strGuide);
		//	 theAPP_SESSION.ShowOfficeGuide(str);
		//}else//
		//{
		//	str.Format(_T("%s�����ݿ�д��ʧ�ܡ�."),strGuide);
		//	theAPP_SESSION.ShowOfficeGuide(_T("���ݿ�д��ʧ��"));
		//}
	}
	catch (...) {
		// д������־
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		}
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
		}
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˵�ѡ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::DoHandleBase()
{
	// ��ȡ��������
	int index = theAPP_SESSION.GetInputToInt();
	m_strShowMsg = _T("");
	// �˵�ѡ��
	switch (index)
	{
	case MENU_CODE_1:
		InitAttachBoxData();
		m_DataModel.operation_type = OPERATION_ATTACH_BOX;
		break;
	case MENU_CODE_2:
		InitDetachBoxData();
		m_DataModel.operation_type = OPERATION_DETACH_BOX;
		break;
	default:
		// �˵�ѡ��Ƿ���ʾ
		m_DataModel.operation_type = OPERATION_UNKNOWN;
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return;
	}

	// ������ת
	DoDialogFlow(WM_CBMGR_BASE_TO_OPERATION);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��װ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::DoHandleAttachBox()
{
	//// ��ȡ��������
	///*int index = theAPP_SESSION.GetInputToInt();*/
	//// �����ʾ����
	//m_strShowMsg = _T("");
	//// �˵�ѡ��
	///*switch (index)
	//{
	//case MENU_CODE_1:*/
	//	// �����ǰ��ֽ����
	//	if (m_DataModel.banknote_box.now_attached == false)
	//	{
	//		BH_BOX_ID box_id;
	//		long er_code = BANKNOTE_HELPER->GetBoxID(box_id);
	//		// ���ֽ����δ����
	//		if ( !BANKNOTE_HELPER->IsBankNoteBoxAttached())
	//		{
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_FAILED_CONNECT));
	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);//д��־
	//			return;
	//		}
	//		// ���ֽ����ID��ȡ�ɹ�
	//		if( E_NO_ERROR !=  BANKNOTE_HELPER->GetBoxID(box_id)) 
	//		{
	//			//?? ��GetBoxIDֽ���䰲װ���ɹ���Ӳ����ȡIDʧ�ܣ�����ϸ���������д���
	//			theAPP_SESSION.ShowOfficeGuide(TXT_BANKNOTEMGR_FAILE_ATTACH);
	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);//д��־
	//			return;
	//		}
	//		// ��װֽ���䣺���ֽ������Ϣ BANKNOTE_BOX_INFO �����뱾���ļ�BankNotCountInfo
	//		// �����հ�BanknoteCountInfo����д���°�װ��ֽ����ID
	//		CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = CTBanknoteCountInfo::_baknote_box_info();
	//		boxInfo.banknoteBoxID.Deserialize(box_id.user_id);
	//		
	//		// ��ֽ����IDд��m_DataModel����ʾ��}
	//		m_DataModel.banknote_box.now_id = boxInfo.banknoteBoxID.ToString();

	//		// д���ļ�
	//		if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
	//			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
	//		}
	//		m_DataModel.banknote_box.now_attached = true;

	//		// ֽ���䰲װ�ɹ�
	//		m_strShowMsg = _opl(TXT_BANKNOTEMGR_INSTALL_COMPLETED)/*T("ֽ���䰲װ�ɹ���")*/;
	//		LoadBankNoteBoxInstallReception();//��ӡֽ���䰲װ��ƾ֤

	//		// ��ֽ���䰲װ�����������ݿ�
	//		try{
	//			BanknoteBoxInstallOperationRecord banknoteBoxInstallOperationRecord;
	//			banknoteBoxInstallOperationRecord.dtTransTime = ComGetCurTime();
	//			banknoteBoxInstallOperationRecord.txtBanknoteBoxID =  boxInfo.banknoteBoxID.ToString();
	//			banknoteBoxInstallOperationRecord.iTotalAmount = 0;
	//			banknoteBoxInstallOperationRecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
	//			DATA_HELPER->DbInsertBanknoteBoxInstallOperationRecord(banknoteBoxInstallOperationRecord);
	//		}
	//		catch(CSysException& e)
	//		{
	//			theEXCEPTION_MGR.ProcessException(e);
	//			m_strShowMsg += _T("���������ݿ�ʧ�ܡ�");
	//		}

	//		// ����ֽ���䰲װ�ĵ���
	//		CASHBOX_CHANGE change;
	//		
	//		change.SetBoxID(boxInfo.banknoteBoxID.bLineCode, boxInfo.banknoteBoxID.bType,boxInfo.banknoteBoxID.wNum); //����ID			
	//		change.handleType       = INSTALL;
	//		change.position         = ONE;
	//		change.State            = VALID_ATTACH;
	//		change.BanNoteBox_Initial();//����
	//		change.btCount = change.GetSumCurrency();//ȡ�ñ�������
	//
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);//������
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,true);
	//		
	//		// ��ʾ����
	//		theAPP_SESSION.ShowOfficeGuide(m_strShowMsg);
	//		DoDialogFlow(WM_CBMGR_OPERATION_TO_FINISH); // ������ת
	//	}
	//	// �����ǰ��δж�ص�ֽ������
	//	else {
	//		theAPP_SESSION.ShowOfficeGuide(TXT_BANKNOTEMGR_FAILE_PREDETACH);
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
	//	}
	//	break;
	//default:
	//	// �˵�ѡ��Ƿ���ʾ
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//	return;
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ж�ش���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::DoHandleDetachBox()
{
	//// ��ȡ��������
	//int index = theAPP_SESSION.GetInputToInt();
	m_strShowMsg = _T("");
	// �˵�ѡ��
	/*switch (index)
	{
	case MENU_CODE_1:*/
		// �����ǰ��ֽ����
		if (m_DataModel.banknote_box.now_attached == true) {
			try{

			// �������ļ�BankNoteCountInfo �ļ���ֽ���������BANKNOTE_BOX_INFO ���
			// �����հ׵�BanknoteCountInfo struct
			
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = CTBanknoteCountInfo::_banknote_box_info_();
			boxInfo.banknoteBoxID = CTBanknoteCountInfo::_banknote_box_id();
			m_DataModel.banknote_box.now_id = boxInfo.banknoteBoxID.ToString();
			
			// д���ļ�
			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
			}

			// �����飨Ӳ����ֽ�����Ƿ��Ѿ����ɹ�ж��
			m_DataModel.banknote_box.now_attached = false;
			m_strShowMsg = _opl(TXT_BANKNOTEMGT_REMOVAL_COMPLETED)/*T("ж�سɹ���")*/;
			// ֽ����ж��ƾ֤
			LoadBankNoteBoxUninstallReception();
			

			BanknoteBoxCashDetail detail;
			BanknoteBoxRemoveOperationRecord banknoteBoxRemoveOperationRecord;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.clear();
			//����ֽ����ж�ص���
			CASHBOX_CHANGE change;
			change.SetBoxID(boxInfo.banknoteBoxID.bLineCode, boxInfo.banknoteBoxID.bType, boxInfo.banknoteBoxID.bNumInQu,boxInfo.banknoteBoxID.bNum); //ֽ����ID
			change.handleType = UNINSTALL; //��������
			change.position = BANKNOTE_COLL_POS; //λ��
			change.State = VALID_DETACH;//״̬
			CASH_TYPE_COUNT cash;
			vector<CASH_TYPE_COUNT> vecCash;
			cash.cash_type = Banknote1Yuan;
			cash.cash_count = m_DataModel.banknote_box.face10;
			vecCash.push_back(cash);
			detail.iBanknoteTypeCode = Banknote1Yuan;
			detail.iQuantity = m_DataModel.banknote_box.face10;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);

			cash.cash_type  = Banknote5Yuan;
			cash.cash_count = m_DataModel.banknote_box.face20;
			vecCash.push_back(cash);
			detail.iBanknoteTypeCode = Banknote5Yuan;
			detail.iQuantity = m_DataModel.banknote_box.face20;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);

			cash.cash_type  = Banknote10Yuan;
			cash.cash_count = m_DataModel.banknote_box.face50;
			vecCash.push_back(cash);
			detail.iBanknoteTypeCode = Banknote10Yuan;
			detail.iQuantity = m_DataModel.banknote_box.face50;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);

			cash.cash_type  = Banknote20Yuan;
			cash.cash_count = m_DataModel.banknote_box.face100;
			vecCash.push_back(cash);
			detail.iBanknoteTypeCode = Banknote20Yuan;
			detail.iQuantity = m_DataModel.banknote_box.face100;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);

			/*cash.cash_type  = Banknote50Yuan;
			cash.cash_count = m_DataModel.banknote_box.face500;
			vecCash.push_back(cash);
			detail.iBanknoteTypeCode = Banknote50Yuan;
			detail.iQuantity = m_DataModel.banknote_box.face500;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);

			cash.cash_type  = Banknote100Yuan;
			cash.cash_count = m_DataModel.banknote_box.face1000;
			vecCash.push_back(cash);
			detail.iBanknoteTypeCode = Banknote100Yuan;
			detail.iQuantity = m_DataModel.banknote_box.face1000;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);*/

			change.vecCash = vecCash; //���ִ���+Ǯ������
			change.btCount = change.GetSumCurrency(); //��������

			// �ϴ�ж���¼�����
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,true);//д��־
			// �ϴ�Ǯ���������
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendClearBankNotesBoxReport(vecCash);
			//д���ݿ�
			banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();//Operation Time
			banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtBanknoteBoxID = m_DataModel.banknote_box.raw_id;//BOX ID
			banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.iTotalAmount = (m_DataModel.banknote_box.face10*10
				+ m_DataModel.banknote_box.face20*20
				+ m_DataModel.banknote_box.face50*50
				+ m_DataModel.banknote_box.face100*100
				/*+ m_DataModel.banknote_box.face500*500
				+ m_DataModel.banknote_box.face1000*1000*/);
			banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;

			DATA_HELPER->DbInsertBanknoteBoxRemoveOperationRecord(banknoteBoxRemoveOperationRecord);
			}
			catch(CSysException& e)
			{
				theEXCEPTION_MGR.ProcessException(e);
				//if(theEXCEPTION_MGR.HasException(CTVMPrinterException::MODULE_ID))
					m_strShowMsg += _T(",");
				m_strShowMsg += _T("�������ݿ�ʧ�ܡ�");
				//throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__, _T("���ݿ��������ʧ��"));
			}
			
			theAPP_SESSION.ShowOfficeGuide(m_strShowMsg);
			DoDialogFlow(WM_CBMGR_OPERATION_TO_FINISH); // ������ת
		}
		// ���ֽ������ǰ�ѱ�ж��
		else {
			theAPP_SESSION.ShowOfficeGuide(TXT_BANKNOTEMGR_NEED_ATTACH);
		}

	//	break;
	//default:
	//	// �˵�ѡ��Ƿ���ʾ
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//	return;
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����װҵ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::InitAttachBoxData()
{
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.banknote_box.raw_id = boxInfo.banknoteBoxID.ToString();

	// ���BanknoteCountInfo��¼IDΪ00000000���������ֽ���䡣����Ӳ����顣
	if (boxInfo.banknoteBoxID == CTBanknoteCountInfo::_banknote_box_id()) {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = false;
	}
	else {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = true;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��ж��ҵ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::InitDetachBoxData()
{
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.banknote_box.raw_id = boxInfo.banknoteBoxID.ToString();
	m_DataModel.banknote_box.face10 = boxInfo.ulCount_CNY10;
	m_DataModel.banknote_box.face20 = boxInfo.ulCount_CNY20;
	m_DataModel.banknote_box.face50 = boxInfo.ulCount_CNY50;
	m_DataModel.banknote_box.face100 = boxInfo.ulCount_CNY100;
	m_DataModel.banknote_box.face5 = boxInfo.ulCount_CNY5;
	m_DataModel.banknote_box.face1 = boxInfo.ulCount_CNY1;

	// ���BanknoteCountInfo��¼ID��00000000���������ֽ���䡣����Ӳ����顣
	if (!(boxInfo.banknoteBoxID == CTBanknoteCountInfo::_banknote_box_id())) {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = true;
	}
	else {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = false;
	}
}

//���ֽ����ID�Ƿ�Ϸ�
bool CBanknoteBoxManageSvc::CheckBoxID(LPBYTE lpBoxID)
{
	//LineCode ��Type�� Number����BankNoteBoxID
	//���ȡ�õ�BankNoteBoxID ǰ��λ != 0192
	//return false;
	//���ȡ�õ�BankNoteBoxID ���� != 8
	//return false;

	return true;
}

//ֽ����ж��ƾ֤
void CBanknoteBoxManageSvc::LoadBankNoteBoxUninstallReception()
{
	try
	{
		CStringArray saBillTemplate;
		CStringArray saTemp;
		LoadBanknoteBoxData();//��ȡ��ǰֽ�����е�Ǯ����Ϣ
		vector<BANKNOTE_BOX_DATA>::iterator iter =  vecBanknote_uninstall.begin();
		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BANK_NOTE_BOX_UNINSTALL.template"), saBillTemplate);
		for ( int i = 0; i < saBillTemplate.GetCount();i++)
		{
			CString strLine = saBillTemplate.ElementAt(i);
			strLine.Replace(_T("{ֽ����ID}"), m_DataModel.banknote_box.raw_id);
			strLine.Replace(_T("{ж��ǰ���}"), ComFormatAmount(100*(m_DataModel.banknote_box.face10*10
				+ m_DataModel.banknote_box.face20*20
				+ m_DataModel.banknote_box.face50*50
				+ m_DataModel.banknote_box.face100*100
				/*+ m_DataModel.banknote_box.face500*500
				+ m_DataModel.banknote_box.face1000*1000*/)));
			if(strLine.Find(_T("{��ֵ}")) >= 0 && iter != vecBanknote_uninstall.end() )
			{
				strLine.Replace(_T("{��ֵ}"), CashValueToString(iter->value));
				strLine.Replace(_T("{����}"),ComFormatCount(iter->nCount));
				strLine.Replace(_T("{���}"),ComFormatAmount(iter->lAmount));
				iter ++;
			}
			strLine.Replace(_T("{����ʱ��}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			strLine.Replace(_T("{����Ա���}"), theAPP_SESSION.GetUserInfo().sUserID);
			strLine.Replace(_T("{��վ����_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			strLine.Replace(_T("{��վ����_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			strLine.Replace(_T("{��վ����_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			strLine.Replace(_T("{�豸ID}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			CStringArray saText;
			ComPrintLineFormatForTemplate(strLine, saText);
			saTemp.Append(saText);
		}
		PRINTER_HELPER->PrintReceiptByBmp(&saTemp,true,true);//��ӡƾ֤
	}
	catch (CSysException& e)
	{
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
		m_strShowMsg += _T("��ӡ���쳣����ӡʧ��");
	}
	catch (...)
	{
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__,_T("ƾ֤��ӡʧ��"));
		theAPP_SESSION.ShowOfficeGuide(_T("ж�سɹ���ƾ֤��ӡʧ�ܡ�"));
	}
	
	
}


//ֽ���䰲װƾ֤
void CBanknoteBoxManageSvc::LoadBankNoteBoxInstallReception()
{
	try{
		CStringArray saBillTemplate;
		CStringArray saTemp;
		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BANK_NOTE_BOX_INSTALL.template"), saBillTemplate);
		for ( int i = 0; i < saBillTemplate.GetCount();i++)
		{
			CString strLine = saBillTemplate.ElementAt(i);
			strLine.Replace(_T("{ֽ����ID}"), m_DataModel.banknote_box.now_id);
			strLine.Replace(_T("{ж��ǰ���}"), ComFormatAmount(100*(m_DataModel.banknote_box.face10*10
				+ m_DataModel.banknote_box.face20*20
				+ m_DataModel.banknote_box.face50*50
				+ m_DataModel.banknote_box.face100*100
				/*+ m_DataModel.banknote_box.face500*500
				+ m_DataModel.banknote_box.face1000*1000*/)));
			CString strCurTime = ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
			theAPP_SESSION.GetUserInfo().sUserID;
			strLine.Replace(_T("{����ʱ��}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			strLine.Replace(_T("{����Ա���}"), theAPP_SESSION.GetUserInfo().sUserID);
			strLine.Replace(_T("{��վ����_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			strLine.Replace(_T("{��վ����_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			strLine.Replace(_T("{��վ����_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			strLine.Replace(_T("{�豸ID}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			CStringArray saText;
			ComPrintLineFormatForTemplate(strLine, saText);
			saTemp.Append(saText);

		}
		PRINTER_HELPER->PrintReceiptByBmp(&saTemp,true,true);//��ӡƾ֤
	}
	catch (CSysException&e)
	{
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
		m_strShowMsg += _T("��ӡ���쳣����ӡʧ��");
	}
	catch (...)
	{
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		theAPP_SESSION.ShowOfficeGuide(_T("��װ�ɹ���ƾ֤��ӡʧ�ܡ�"));
	}
}

//��ʽ��ֽ����ֵ
CString CBanknoteBoxManageSvc::CashValueToString(BankNoteAndCoinType_t& value)
{
	CString strValue;
	switch(value)
	{
	case Banknote1Yuan:strValue = _T("  10MOP");break;
	case Banknote5Yuan:strValue = _T("  20MOP");break;
	case Banknote10Yuan:strValue = _T("  50MOP");break;
	case Banknote20Yuan:strValue = _T(" 100MOP");break;
	case Banknote50Yuan:strValue = _T(" 500MOP");break;
	case Banknote100Yuan:strValue = _T("1000MOP");break;
	default: break;
	}
	return strValue;
}

//ֽ����ж��ʱ����ȡֽ��������Ϣ
void CBanknoteBoxManageSvc::LoadBanknoteBoxData()
{
	vecBanknote_uninstall.clear();

	/*BANKNOTE_BOX_DATA notedata;
	notedata.value = Banknote1Yuan;
	notedata.nCount = m_DataModel.banknote_box.face10;
	notedata.lAmount = 10 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote5Yuan;
	notedata.nCount = m_DataModel.banknote_box.face20;
	notedata.lAmount = 20 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote10Yuan;
	notedata.nCount = m_DataModel.banknote_box.face50;
	notedata.lAmount = 50 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote20Yuan;
	notedata.nCount = m_DataModel.banknote_box.face100;
	notedata.lAmount = 100 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);
	notedata.value = Banknote50Yuan;
	notedata.nCount = m_DataModel.banknote_box.face500;
	notedata.lAmount = 500 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote100Yuan;
	notedata.nCount = m_DataModel.banknote_box.face1000;
	notedata.lAmount = 1000 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);*/

}

CBanknoteBoxManageSvc::OPERATION CBanknoteBoxManageSvc::GetOperationType()
{
	if(BANKNOTE_HELPER->IsCashBoxAttached() && m_DataModel.banknote_box.now_attached == false)
	{//ֽ�����ǰ�װ�ŵģ��������״̬��ж��״̬
		//�Ƿ���װ
		return VIOLATE_INSTALL;
	}
	if( !BANKNOTE_HELPER->IsCashBoxAttached() && m_DataModel.banknote_box.now_attached == true)
	{//ֽ����δ��װ������������Ѱ�װ
		//�Ƿ�ж��
		return VIOLATE_UNINSTALL;
	}
	return NORMAL_OPERATION;
}

//�ж�ֽ�����Ƿ�����
bool CBanknoteBoxManageSvc::IsBoxConnected()
{
	if ( BANKNOTE_HELPER != NULL && BANKNOTE_HELPER->IsCashBoxAttached())
	{
		m_DataModel.bIsBanknoteModuleException = false;
		return true;
	}
	m_DataModel.bIsBanknoteModuleException = true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ʾ��������

@param      ��

@retval     CString strMsg

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CBanknoteBoxManageSvc::GetShowErrorMsg(){
	return m_strShowMsg;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Service����ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::OnStart()
{
	__super::OnStart();
	m_DataModel = _tagModel();
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	m_pDialogFlow->ShowFirstDialogGroup();
	IsBoxConnected();
	//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
}