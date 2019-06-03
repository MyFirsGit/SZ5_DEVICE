#include "stdafx.h"
#include "DeviceStatusSvc.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  INVALID_ID		_T("FFFFFFFF")
BEGIN_MESSAGE_MAP(CDeviceStatusSvc,CTVMForeService)
	

END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusSvc::CDeviceStatusSvc()
:CTVMForeService(DEVICE_STATUS_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CDeviceStatusDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET,_opl(GUIDE_F3_F6_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DEVICESTATUS_SET,IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET);		// ���ó�ʼ������
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusSvc::~CDeviceStatusSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::OnStart()
{
	m_DataModel.cashboxstatus.timesstatus = 1;
    m_bIsShowNearlyFullStatus = theFunction_INFO.IsShowNearlyFullEmptyParaValue();
	int DeviceType = theMAINTENANCE_INFO.GetDeviceType();						// ��ȡ�豸����
    m_bIsAutoInputRFID = theFunction_INFO.IsAutoReadRfidWithoutManual();
	if(DeviceType == DEVICE_TYPE_TVM)
	{
        int index = 3;
		//// ͬ��Ӳ��ģ�����
		//COIN_HELPER->SyncCoinCountInfoToFile();
		// ������״̬
		ReadCashBoxID();														// ��ȡǮ�䡢Ʊ��ID
		ReadCashBoxData();														// ��ȡǮ�䡢Ʊ��ʣ��ö��/����
		ReadEmptyFull();														// ��ȡǮ�䡢Ʊ�佫�ա��������ѿա�����ö��/����
		ReadStatus();															// �ж�Ǯ�䡢Ʊ��״̬
		if(!m_bIsShowNearlyFullStatus)index--;
		SetPageInfo(index,m_DataModel.cashboxstatus.timesstatus,12);				// ʵ�ַ�ҳ
	}
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET);			// ���ó�ʼ������
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET);
	theAPP_SESSION.SetMenuLevel(DEVICE_STATUS_SVC);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡǮ�䡢Ʊ��ID

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadCashBoxID()
{
 	long errCode = 0;
 	CString Coin_Box_ID;
 	CString Token_Box_AID = _T("");
 	CString Token_Box_BID = _T("");
 	
	// ****** 1. Ӳ��
	CTCoinCountInfo::COIN_BOX_INFO coinHopperBoxAInfo =  theCOIN_COUNT.GetCoinHopperboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO coinHopperBoxBInfo =  theCOIN_COUNT.GetCoinHopperboxBInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coinCollectBoxInfo =  theCOIN_COUNT.GetCollectionCoinboxAInfo();

	m_DataModel.cashboxdata.CycleChangeCoinAID = coinHopperBoxAInfo.coinBoxID.ToString();
	m_DataModel.cashboxdata.CycleChangeCoinBID = coinHopperBoxBInfo.coinBoxID.ToString();
	m_DataModel.cashboxdata.CoinCollectionAID = coinCollectBoxInfo.coinBoxID.ToString();
	// �ж�Ӳ�һ������״̬
 	try
 	{
	//	CH_SPEC1_RSP status;
	//	COIN_HELPER->CH_GetStatus(&status);	
 		 //���Ӳ�һ������Ƿ�װ��λ
		if (coinCollectBoxInfo.coinBoxID.ToString().Compare(INVALID_ID) != 0){ //����� �Ѱ�װ	
		//	if (status.bIsMoneyBoxExist)
			{
				// ��ȡӲ��ID
				CString boxID = COIN_HELPER->CH_ReadRfidData(3);
				if (boxID == coinCollectBoxInfo.coinBoxID.ToString()){ 
					// �������ID��ͬ ������װ
					theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					// Ӳ��ID�����������ͬ �Ƿ���װ
					m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
					theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				}
			}
			//else
			{
				m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{
			//if (status.bIsMoneyBoxExist)
			{
				m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			//else
			{
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}
 	}
 	 //�쳣����
 	catch (CSysException& e) {
 		m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		m_DataModel.cashboxdata.CoinCollectionBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		theEXCEPTION_MGR.ProcessException(e);
 	}
 	catch (...) {
 		m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		m_DataModel.cashboxdata.CoinCollectionBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
 	}
 	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 	 //ֽ�һ�����ID -- �����

	//m_DataModel.cashboxdata.BanknoteCollectionID = theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString();	// ֽ�һ�����
 	
 	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ֽ�������� AB
	DEVICE_STATUS_INFO box_status;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO box_A = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_B = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_C = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();*/
	try
	{
		bool bIsCahsChangeBoxAttched = BANKNOTE_HELPER->IsCashChangeBoxAttached();
		// ֽ��������
		if (bIsCahsChangeBoxAttched){
			// �������� Ϊ�Ѱ�װ
			if(box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
				// ���Ӳ����λ
				m_DataModel.cashboxdata.BanknoteChangeBoxID = box_A.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
			else{
				// �Ƿ���װ
				m_DataModel.cashboxdata.BanknoteChangeBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
		}
		else{
		// ���Ӳ������λ
			// ��������  �Ѱ�װ ж��
			if (box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
				m_DataModel.cashboxdata.BanknoteChangeBoxID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
			else{
			// ����ж�� ��ʾffffffff
				m_DataModel.cashboxdata.BanknoteChangeBoxID = box_A.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}

		//BH_CHANGE_HELPER->GetStatus(box_status);
		// ֽ��������A
		// ���Ӳ����λ
		//if (box_status.CassetteStatus.isFirstBoxExist){
		//	// �������� Ϊ�Ѱ�װ
		//	if(box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		// ���Ӳ����λ
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = box_A.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
		//	}
		//	else{
		//		// �Ƿ���װ
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
		//	}
		//}
		//else{
		//// ���Ӳ������λ
		//	// ��������  �Ѱ�װ ж��
		//	if (box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
		//	}
		//	else{
		//	// ����ж�� ��ʾffffffff
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = box_A.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
		//	}
		//}


		//// ֽ��������B
		//if (box_status.CassetteStatus.isSecondBoxExist){
		//	// �������� Ϊ�Ѱ�װ
		//	if(box_B.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		// ���Ӳ����λ
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = box_B.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
		//	}
		//	else{
		//		// �Ƿ���װ
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
		//	}
		//}
		//else{
		//	// ���Ӳ������λ
		//	// ��������  �Ѱ�װ ж��
		//	if (box_B.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
		//	}
		//	else{
		//		// ����ж�� ��ʾffffffff
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = box_B.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
		//	}
		//}

		// �ϱһ�����
		//if (box_status.SensorStatus.isRejectBoxAvailable){
		//	// �������� Ϊ�Ѱ�װ
		//	if(box_C.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		// ���Ӳ����λ
		//		m_DataModel.cashboxdata.BanknoteInvalidID = box_C.banknoteBoxID.ToString();
		//	}
		//	else{
		//		// �Ƿ���װ
		//		m_DataModel.cashboxdata.BanknoteInvalidID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
		//	}
		//}
		//else{
		//	// ���Ӳ������λ
		//	// ��������  �Ѱ�װ ж��
		//	if (box_B.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		m_DataModel.cashboxdata.BanknoteInvalidID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
		//	}else{
		//		// ����ж�� ��ʾffffffff
		//		m_DataModel.cashboxdata.BanknoteInvalidID = box_C.banknoteBoxID.ToString();
		//	}
		//}
	}
	catch(...)
	{
		m_DataModel.cashboxdata.BanknoteChangeBoxID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		//m_DataModel.cashboxdata.BanknoteChangeBoxBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));

	}
	

 	try
 	{
		// CARDģ��
		TH_RSP_GET_DEV_STATUS status;
		tTIMRFIDInfo rfidInfo;
		bool isBoxAExist = false;
		bool isBoxBExist = false;
		bool isBoxCExist = false;

		CTCardCountInfo::CARD_BOX_ID boxID;
		CTCardCountInfo::CARD_BOX_INFO card_boxA_info = theCARD_COUNT.GetCardboxAInfo();

		// ��ȡӲ��״̬
		CARD_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist ,isBoxCExist);
		if (card_boxA_info.ticketBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
		{
			
			if (isBoxAExist){
				// ��ȡӲ��ID
				bool rst = CARD_HELPER->GetStaticBoxId(TH_BOX_A,rfidInfo);
				boxID.Deserialize(rfidInfo.ucBoxID);
			
				if (boxID == card_boxA_info.ticketBoxID){ 
					// �������ID��ͬ ������װ
					m_DataModel.cashboxdata.CardBoxAID = card_boxA_info.ticketBoxID.ToString();
					theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					// Ӳ��ID�����������ͬ �Ƿ���װ
					m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
					theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				}
			}
			else{
				// �Ƿ�ж��
				m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{ // ����� ж��
			//CARD_HELPER->CheckTicketBoxExist(status, isBoxAExist, isBoxBExist ,isBoxCExist);
			if (isBoxAExist){
				// ��ȡӲ��ID
				m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{ // ���Ӳ���λ
				// ����ж��
				m_DataModel.cashboxdata.CardBoxAID = INVALID_ID;
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}

		//SC_RES_READ_RFID RFID;
		//RFID.boxType = RFID_MAGAZINE_B;
		boxID = CTCardCountInfo::CARD_BOX_ID();

		// Ʊ��B
		CTCardCountInfo::CARD_BOX_INFO card_boxB_info = theCARD_COUNT.GetCardboxBInfo();
		//boxID.Deserialize(RF_ID.RFIDData.byDeviceID);
		if (card_boxB_info.ticketBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
		{
			// ��ȡӲ��״̬
			if (isBoxBExist){
				// ��ȡӲ��ID
				bool result = CARD_HELPER->GetStaticBoxId(TH_BOX_B,rfidInfo);
				if (result)
				{
					boxID.Deserialize(rfidInfo.ucBoxID);
				}
				
				if (boxID == card_boxB_info.ticketBoxID){ 
					// �������ID��ͬ ������װ
					m_DataModel.cashboxdata.CardBoxBID = card_boxB_info.ticketBoxID.ToString();
					theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					// Ӳ��ID�����������ͬ �Ƿ���װ
					m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
					theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				}
			}
			else{
				// �Ƿ�ж��
				m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{ // ����� ж��
			if (isBoxAExist){
				// ��ȡӲ��ID
				m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{ // ���Ӳ���λ
				// ����ж��
				m_DataModel.cashboxdata.CardBoxBID = INVALID_ID;
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}



		//��Ʊ��
		//SC_RES_READ_RFID RF_ID_C;
		//RF_ID.boxType = RFID_MAGAZINE_C;
		CTCardCountInfo::CARD_BOX_INFO card_boxC_info = theCARD_COUNT.GetCardboxWastedInfo();
		boxID = CTCardCountInfo::CARD_BOX_ID();
		if (card_boxC_info.ticketBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
		{
			// ��ȡӲ��״̬
			if (isBoxCExist){
				// ��ȡӲ��ID
				bool rst = CARD_HELPER->GetStaticBoxId(TH_BOX_C,rfidInfo);
				boxID.Deserialize(rfidInfo.ucBoxID);
				//if (boxID == card_boxC_info.ticketBoxID){ 
					// �������ID��ͬ ������װ
					m_DataModel.cashboxdata.INVALIDCardID = card_boxC_info.ticketBoxID.ToString();
					theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				//}else{
				//	// Ӳ��ID�����������ͬ �Ƿ���װ
				//	m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				//}
			}
			else{
				// �Ƿ�ж��
				m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{ // ����� ж��
			if (isBoxAExist){
				// ��ȡӲ��ID
				m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{ // ���Ӳ���λ
				// ����ж��
				m_DataModel.cashboxdata.INVALIDCardID = INVALID_ID;
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}

	}
	catch(...)
	{
		m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}


	//try
	//{
	//	// TOKENģ��
	//	bool isBoxAExist = false;
	//	bool isBoxBExist = false;
	//	bool isBoxCExist = false;
	//	bool isBoxDExist = false;
	//	CTTokenCountInfo::TOKEN_BOX_ID boxID;
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_boxA_info = theTOKEN_COUNT.GetTokenboxAInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_boxB_info = theTOKEN_COUNT.GetTokenboxBInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_hopperA_info = theTOKEN_COUNT.GetHopperAInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_hopperB_info = theTOKEN_COUNT.GetHopperBInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO invalidTokenBoxInfo = theTOKEN_COUNT.GetTokenboxWastedInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO tokenCollected = theTOKEN_COUNT.GetTokenboxCollectedInfo();

	//	m_DataModel.cashboxdata.TokenHopperAID	= token_hopperA_info.tokenBoxID.ToString();
	//	m_DataModel.cashboxdata.TokenHopperBID	= token_hopperB_info.tokenBoxID.ToString();

	//	TOKEN_RSP_GET_DEV_STATUS status;
	//	TOKEN_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist, isBoxCExist, isBoxDExist);
	//	// TOKEN��A
	//	if (token_boxA_info.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
	//	{
	//		if (isBoxAExist){
	//			// ��ȡӲ��ID
	//			CString boxID = TOKEN_HELPER->ReadTokenBoxARFID();
	//			if (boxID == token_boxA_info.tokenBoxID.ToString()){ 
	//				// �������ID��ͬ ������װ
	//				m_DataModel.cashboxdata.TokenBoxAID = token_boxA_info.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}
	//			else{
	//				// Ӳ��ID�����������ͬ �Ƿ���װ
	//				m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// �Ƿ�ж��
	//			m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // ����� ж��
	//		if (isBoxAExist){
	//			// ��ȡӲ��ID
	//			m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // ���Ӳ���λ
	//			// ����ж��
	//			m_DataModel.cashboxdata.TokenBoxAID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}


	//	// TOKEN��B
	//	if (token_boxB_info.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
	//	{
	//		// ��ȡӲ��״̬
	//		if (isBoxBExist){
	//			// ��ȡӲ��ID
	//			CString boxID = TOKEN_HELPER->ReadTokenBoxBRFID();
	//			if (boxID == token_boxB_info.tokenBoxID.ToString()){ 
	//				// �������ID��ͬ ������װ
	//				m_DataModel.cashboxdata.TokenBoxBID = token_boxB_info.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}
	//			else{
	//				// Ӳ��ID�����������ͬ �Ƿ���װ
	//				m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// �Ƿ�ж��
	//			m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // ����� ж��
	//		if (isBoxBExist){
	//			// ��ȡӲ��ID
	//			m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // ���Ӳ���λ
	//			// ����ж��
	//			m_DataModel.cashboxdata.TokenBoxBID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}



	//	//��Ʊ��
	//	if (invalidTokenBoxInfo.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
	//	{
	//		// ��ȡӲ��״̬
	//		if (isBoxCExist){
	//			// ��ȡӲ��ID
	//			CString boxID = TOKEN_HELPER->ReadInvalidBoxRFID();
	//			if (boxID == invalidTokenBoxInfo.tokenBoxID.ToString()){ 
	//				// �������ID��ͬ ������װ
	//				m_DataModel.cashboxdata.InvalidTokenBoxID = invalidTokenBoxInfo.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}else{
	//				// Ӳ��ID�����������ͬ �Ƿ���װ
	//				m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// �Ƿ�ж��
	//			m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // ����� ж��
	//		if (isBoxCExist){
	//			// ��ȡӲ��ID
	//			m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // ���Ӳ���λ
	//			// ����ж��
	//			m_DataModel.cashboxdata.InvalidTokenBoxID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}

	//	//������
	//	if (tokenCollected.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //����� �Ѱ�װ
	//	{
	//		// ��ȡӲ��״̬
	//		if (isBoxDExist){
	//			// ��ȡӲ��ID
	//			CString boxID = TOKEN_HELPER->ReadTokenBoxCollectRFID();
	//			if (boxID == tokenCollected.tokenBoxID.ToString()){ 
	//				// �������ID��ͬ ������װ
	//				m_DataModel.cashboxdata.CollectedTokenBoxID = tokenCollected.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}else{
	//				// Ӳ��ID�����������ͬ �Ƿ���װ
	//				m_DataModel.cashboxdata.CollectedTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// �Ƿ�ж��
	//			m_DataModel.cashboxdata.CollectedTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // ����� ж��
	//		if (isBoxDExist){
	//			// ��ȡӲ��ID
	//			m_DataModel.cashboxdata.CollectedTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // ���Ӳ���λ
	//			// ����ж��
	//			m_DataModel.cashboxdata.CollectedTokenBoxID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}

	//}
	//catch(...)
	//{
	//	m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
	//	m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
	//	m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}


	//ֽ�һ�����
	try{
		bool bIsBanknoteInstall = BANKNOTE_HELPER->IsCashBoxAttached();

		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		if(bIsBanknoteInstall){
			// �Ƿ���װ
			if(banknoteInfo.banknoteBoxID.ToString().Compare(INVALID_ID) == 0){
				m_DataModel.cashboxdata.BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{
				m_DataModel.cashboxdata.BanknoteCollectionID = banknoteInfo.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
		}
		else{
			// �Ƿ�ж��
			if(banknoteInfo.banknoteBoxID.ToString().Compare(INVALID_ID) == 0){
				m_DataModel.cashboxdata.BanknoteCollectionID = banknoteInfo.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
			else{
				m_DataModel.cashboxdata.BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}


		// ֽ��ѭ����
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_A_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().banknoteBoxID.ToString();
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_B_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().banknoteBoxID.ToString();
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_C_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().banknoteBoxID.ToString();
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_D_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().banknoteBoxID.ToString();

	}
	catch(CSysException& e){

	}
	catch(...){

	}
	
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡǮ�䡢Ʊ��ʣ��ö��/����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadCashBoxData()
{
	/*
	  
	Ӳ��ѭ��������	         				���: 99FF9999          ʣ������: 999      
	Ӳ��������A        			     		���: 99FF9999          ʣ������: 999            
	Ӳ��������B                        		���: 99FF9999          ʣ������: 999            
	Ӳ�һ�����A	 		     				���: 99FF9999          ʣ������: 999
	Ӳ�һ�����B	  							���: 99FF9999          ʣ������: 999
	
	ֽ�һ�����	������    					���: 99FF9999          ʣ������: 999    
	ֽ��������        						���: 99FF9999          ʣ������: 999
	ֽ��ѭ����A								��ţ�99FF9999          ʣ��������999
	ֽ��ѭ����B								��ţ�99FF9999          ʣ��������999
	ֽ��ѭ����C								��ţ�99FF9999          ʣ��������999
	ֽ��ѭ����D								��ţ�99FF9999          ʣ��������999

	//ֽ��������B       						���: 99FF9999          ʣ������: 999
	//�ϱһ�����								���: 99FF9999          ʣ������: 999
	Ʊ��A        							���: 99FF9999          ʣ������: 999  
	Ʊ��B									���: 99FF9999          ʣ������: 999
	��Ʊ��	 								���: 99FF9999          ʣ������: 999
	*/

	LONG count = -1;
	count = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;						// Ӳ��ѭ������A��
	m_DataModel.cashboxdata.CycleChangeCoinAData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;						// Ӳ��ѭ������B��
	m_DataModel.cashboxdata.CycleChangeCoinBData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;							// Ӳ��������A
	m_DataModel.cashboxdata.DedicatedChangeCoinAData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;							// Ӳ��������B
	m_DataModel.cashboxdata.DedicatedChangeCoinBData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetCollectionCoinboxAInfo().GetCurCount();					// Ӳ�һ�����A
	m_DataModel.cashboxdata.CoinCollectionAData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetCollectionCoinboxBInfo().GetCurCount();					// Ӳ�һ�����B
	m_DataModel.cashboxdata.CoinCollectionBData.Format(_T("%ld"),count);

	count = theBANKNOTE_COUNT.GetBanknoteboxInfo().BankntoeTotalCount();			// ֽ�һ�����
	m_DataModel.cashboxdata.BanknoteCollectionData.Format(_T("%ld"),count);

	count = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().ulCurCount;				// ֽ��������A
	m_DataModel.cashboxdata.BanknoteChangeData.Format(_T("%ld"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxA.Format(_T("%d"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxB.Format(_T("%d"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxC.Format(_T("%d"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxD.Format(_T("%d"),count);


	//count = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().ulCurCount;				// ֽ��������B
	//m_DataModel.cashboxdata.BanknoteChangeBData.Format(_T("%ld"),count);

	//count = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo().ulCurCount;				// ֽ�ҷϱ���
	//m_DataModel.cashboxdata.BanknoteInvalidData.Format(_T("%ld"),count);

	count = theCARD_COUNT.GetCardboxAInfo().ulCurCount;								// Ʊ��A����
	m_DataModel.cashboxdata.CardBoxAData.Format(_T("%ld"),count);

	count = theCARD_COUNT.GetCardboxBInfo().ulCurCount;								// Ʊ��B����
	m_DataModel.cashboxdata.CardBoxBData.Format(_T("%ld"),count);

	count = theCARD_COUNT.GetCardboxWastedInfo().ulCurCount;						// ��Ʊ��
	m_DataModel.cashboxdata.InvalidCardData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxAInfo().ulCurCount;							// TOKEN��A����
	//m_DataModel.cashboxdata.TokenBoxAData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxAInfo().ulCurCount;							// TOKEN��B����
	//m_DataModel.cashboxdata.TokenBoxBData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetHopperAInfo().ulCurCount;								// TOKEN��hopperA����
	//m_DataModel.cashboxdata.TokenHopperAData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetHopperBInfo().ulCurCount;								// TOKEN��hopperA����
	//m_DataModel.cashboxdata.TokenHopperBData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxWastedInfo().ulCurCount;						// ��Ʊ��
	//m_DataModel.cashboxdata.InvalidTokenData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxCollectedInfo().ulCurCount;						// ������
	//m_DataModel.cashboxdata.CollectedTokenData.Format(_T("%ld"),count);
	
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡǮ�䡢Ʊ�佫�ա��������ѿա�����ö��/����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadEmptyFull()
{
	CAfcTVMOperationParam::MAGAZINE_PARAM magazine_param;
	theAFC_TVM_OPERATION.GetMagazineParam(magazine_param);

	CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
	theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);

 	m_DataModel.emptyfull.CardMNearlyEmpty.Format(_T("%d"),magazine_param.TicketNearEmptyValue);							// Ʊ�佫������
 	m_DataModel.emptyfull.CardMEmpty.Format(_T("%d"),magazine_param.TicketEmptyValue);										// Ʊ���ѿ�����
 	m_DataModel.emptyfull.INVALIDCardMNearlyFull.Format(_T("%d"),magazine_param.TicketInvalidNearFullValue);				// ��Ʊ�佫��
 	m_DataModel.emptyfull.INVALIDCardMFull.Format(_T("%d"),magazine_param.TicketInvalidFullValue);							// ��Ʊ������

	//m_DataModel.emptyfull.TokenANearlyEmpty.Format(_T("%d"),magazine_param.TicketNearEmptyValue);							// Ʊ�佫������
	//m_DataModel.emptyfull.TokenAEmpty.Format(_T("%d"),magazine_param.TicketEmptyValue);										// Ʊ���ѿ�����
	//m_DataModel.emptyfull.TokenBNearlyEmpty.Format(_T("%d"),magazine_param.TicketNearEmptyValue);							// Ʊ�佫������
	//m_DataModel.emptyfull.TokenBEmpty.Format(_T("%d"),magazine_param.TicketEmptyValue);										// Ʊ���ѿ�����
	//m_DataModel.emptyfull.InvalidTokenNearlyFull.Format(_T("%d"),magazine_param.TicketInvalidNearFullValue);				// ��Ʊ�佫��
	//m_DataModel.emptyfull.InvalidTokenFull.Format(_T("%d"),magazine_param.TicketInvalidFullValue);							// ��Ʊ������
	//m_DataModel.emptyfull.CollectedTokenNearlyFull.Format(_T("%d"),800);													// �����佫��
	//m_DataModel.emptyfull.CollectedTokenFull.Format(_T("%d"),1000);															// ����������

 	m_DataModel.emptyfull.DedicatedChangeCoinANearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);		// Ӳ�������佫��
 	m_DataModel.emptyfull.DedicatedChangeCoinAEmpty.Format(_T("%d"),theTVM_INFO.GetCHChangeBoxAEmpty());														//  
 	m_DataModel.emptyfull.DedicatedChangeCoinBNearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);		// Ӳ���������ѿ�
 	m_DataModel.emptyfull.DedicatedChangeCoinBEmpty.Format(_T("%d"),theTVM_INFO.GetCHChangeBoxBEmpty());														//  
	try
	{
		m_DataModel.emptyfull.CycleChangeCoinANearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);				// Ӳ��ѭ�������佫��ö�� lichao
		m_DataModel.emptyfull.CycleChangeCoinBNearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);					// Ӳ��ѭ�������佫��ö��

	}
	catch(...){
		m_DataModel.emptyfull.CycleChangeCoinANearlyEmpty.Format(_T("%d"),0);
		m_DataModel.emptyfull.CycleChangeCoinBNearlyEmpty.Format(_T("%d"),0);
	}

	
 	m_DataModel.emptyfull.CycleChangeCoinAEmpty.Format(_T("%d"),0);	// Ӳ��ѭ������A���ѿ�ö��
	m_DataModel.emptyfull.CycleChangeCoinBEmpty.Format(_T("%d"),0);	// Ӳ��ѭ������B���ѿ�ö��

	

 	m_DataModel.emptyfull.CoinCollectionANearlyFull.Format(_T("%d"),billbox_param.coinCollectionWillFullCoinCnt);			// Ӳ�һ�����A����ö��
 	m_DataModel.emptyfull.CoinCollectionAFull.Format(_T("%d"),billbox_param.coinCollectionAlreadyFullCoinCnt);				// Ӳ�һ�����A����ö��
 	m_DataModel.emptyfull.CoinCollectionBNearlyFull.Format(_T("%d"),billbox_param.coinCollectionWillFullCoinCnt);			// Ӳ�һ�����B����ö��
 	m_DataModel.emptyfull.CoinCollectionBFull.Format(_T("%d"),billbox_param.coinCollectionAlreadyFullCoinCnt);				// Ӳ�һ�����B����ö��

 	m_DataModel.emptyfull.BanknoteCollectionNearlyFulll.Format(_T("%d"),billbox_param.willFullBillCnt);						// ֽ�һ����佫������
 	m_DataModel.emptyfull.BanknoteCollectionFull.Format(_T("%d"),billbox_param.alreadyFullBillCnt);							// ֽ�һ�������������
 
 	m_DataModel.emptyfull.BanknoteChangeNearlyEmpty.Format(_T("%d"),billbox_param.willEmptyBillBoxCnt);						// ֽ�������佫������
 	m_DataModel.emptyfull.BanknoteChangeEmpty.Format(_T("%d"),0);															// ֽ���������ѿ�����

	try{

		map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> mapItems = BANKNOTE_HELPER->GetEmptyFull();

		map<BNR_MODULE_ID,T_XfsPhysicalCashUnit>::iterator it = mapItems.find(BNR_RECYCLER_BOX_A);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}

		it = mapItems.find(BNR_RECYCLER_BOX_B);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}

		it = mapItems.find(BNR_RECYCLER_BOX_C);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}

		it = mapItems.find(BNR_RECYCLER_BOX_D);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}
	}
	catch(...){

	}
	

	//m_DataModel.emptyfull.BanknoteInvalidNearlyFull.Format(_T("%d"),theTVM_INFO.GetBHChangeBoxNearFull());					// �ϱһ����佫��
	//m_DataModel.emptyfull.BanknoteInvalidFull.Format(_T("%d"),theTVM_INFO.GetBHChangeBoxFull());							// �ϱһ���������


	// �ϱһ����� ���� ��������

}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     cashboxdata

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusSvc::Model& CDeviceStatusSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�Ǯ�䡢Ʊ��״̬

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadStatus()
{
	// Ӳ��ѭ��������
	m_DataModel.cashboxstatus.CCyleChangeAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CycleChangeCoinAData,m_DataModel.emptyfull.CycleChangeCoinANearlyEmpty,m_DataModel.emptyfull.CycleChangeCoinAEmpty);					// Ӳ��ѭ��������
	m_DataModel.cashboxstatus.CCyleChangeBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CycleChangeCoinBData,m_DataModel.emptyfull.CycleChangeCoinBNearlyEmpty,m_DataModel.emptyfull.CycleChangeCoinBEmpty);					// Ӳ��ѭ��������

	// Ӳ��������
	m_DataModel.cashboxstatus.CDChangeAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.DedicatedChangeCoinAData,m_DataModel.emptyfull.DedicatedChangeCoinANearlyEmpty,m_DataModel.emptyfull.DedicatedChangeCoinAEmpty);		// Ӳ��ר��������A
	m_DataModel.cashboxstatus.CDChangeBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.DedicatedChangeCoinBData,m_DataModel.emptyfull.DedicatedChangeCoinBNearlyEmpty,m_DataModel.emptyfull.DedicatedChangeCoinBEmpty);		// Ӳ��ר��������B
	
	// Ӳ�һ�����
	m_DataModel.cashboxstatus.CCollectionAStatus = StatusChangeFull(m_DataModel.cashboxdata.CoinCollectionAData,m_DataModel.emptyfull.CoinCollectionANearlyFull,m_DataModel.emptyfull.CoinCollectionAFull);						// Ӳ�һ�����A
	m_DataModel.cashboxstatus.CCollectionBStatus = StatusChangeFull(m_DataModel.cashboxdata.CoinCollectionBData,m_DataModel.emptyfull.CoinCollectionBNearlyFull,m_DataModel.emptyfull.CoinCollectionBFull);						// Ӳ�һ�����B
	// ֽ�һ�����
	m_DataModel.cashboxstatus.BanknoteStatus = StatusChangeFull(m_DataModel.cashboxdata.BanknoteCollectionData,m_DataModel.emptyfull.BanknoteCollectionNearlyFulll,m_DataModel.emptyfull.BanknoteCollectionFull);				// ֽ�һ�����
	// ֽ��������
	m_DataModel.cashboxstatus.BanknoteChangeStatus = StatusChangeEmpty(m_DataModel.cashboxdata.BanknoteChangeData,m_DataModel.emptyfull.BanknoteChangeNearlyEmpty,m_DataModel.emptyfull.BanknoteChangeEmpty);					// ֽ��������	
	//m_DataModel.cashboxstatus.BanknoteChangeBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.BanknoteChangeBData,m_DataModel.emptyfull.BanknoteChangeNearlyEmpty,m_DataModel.emptyfull.BanknoteChangeEmpty);					// ֽ��������	
	// �ϱһ�����Ľ�����������
	//m_DataModel.cashboxstatus.BanknoteInvalidStatus = StatusChangeFull(m_DataModel.cashboxdata.BanknoteInvalidData,m_DataModel.emptyfull.BanknoteInvalidNearlyFull,m_DataModel.emptyfull.BanknoteInvalidFull);						// �ϱһ�����
	// ѭ����
	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxA,m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}

	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxB,m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}

	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxC,m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}

	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxD,m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}


	// Ʊ��
	m_DataModel.cashboxstatus.CardBoxAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CardBoxAData,m_DataModel.emptyfull.CardMNearlyEmpty,m_DataModel.emptyfull.CardMEmpty);													// Smart-card��
	m_DataModel.cashboxstatus.CardBoxBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CardBoxBData,m_DataModel.emptyfull.CardMNearlyEmpty,m_DataModel.emptyfull.CardMEmpty);													// Smart-card��
	// ��Ʊ��
	m_DataModel.cashboxstatus.WastedSmartcardStatus = StatusChangeFull(m_DataModel.cashboxdata.InvalidCardData,m_DataModel.emptyfull.INVALIDCardMNearlyFull,m_DataModel.emptyfull.INVALIDCardMFull);							// Smart-card��Ʊ��
	
	//m_DataModel.cashboxstatus.TokenAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenBoxAData,m_DataModel.emptyfull.TokenANearlyEmpty,m_DataModel.emptyfull.TokenAEmpty);												// Token��A
	//m_DataModel.cashboxstatus.TokenBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenBoxBData,m_DataModel.emptyfull.TokenBNearlyEmpty,m_DataModel.emptyfull.TokenBEmpty);												// Token��B
	//m_DataModel.cashboxstatus.TokenHopperAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenHopperAData,m_DataModel.emptyfull.TokenANearlyEmpty,m_DataModel.emptyfull.TokenAEmpty);												// Token��A
	//m_DataModel.cashboxstatus.TokenHopperBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenHopperBData,m_DataModel.emptyfull.TokenBNearlyEmpty,m_DataModel.emptyfull.TokenBEmpty);												// Token��B
	//m_DataModel.cashboxstatus.WastedTokenStatus = StatusChangeFull(m_DataModel.cashboxdata.InvalidTokenData,m_DataModel.emptyfull.InvalidTokenNearlyFull,m_DataModel.emptyfull.InvalidTokenFull);								// Token��Ʊ��
	//m_DataModel.cashboxstatus.CollectedTokenStatus = StatusChangeFull(m_DataModel.cashboxdata.CollectedTokenData,m_DataModel.emptyfull.CollectedTokenNearlyFull,m_DataModel.emptyfull.CollectedTokenFull);						// Token��Ʊ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��Ǯ�䡢Ʊ�佫�ա��ѿ�����

@param      remainboxdata��			ʣ��ö/����

@param      nearlyemptyboxdata��	����ö/����

@param      emptyboxdata��			�ѿ�ö/����

@param      BoxID��					1-Ӳ��ѭ��������	2��Ӳ��ѭ�������������������

@retval     CString

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CDeviceStatusSvc::StatusChangeEmpty(CString remainboxdata,CString nearlyemptyboxdata,CString emptyboxdata,int BoxID)
{
	int count = -1;
	int will_count = -1;
	int count_ed = -1;
	CString DataStr = _T("");

	count = _ttoi(remainboxdata);				
	will_count = _ttoi(nearlyemptyboxdata);		
	count_ed = _ttoi(emptyboxdata);	
	
	// Ӳ��ѭ�������������������
	if(2 == BoxID)
	{
		if(count > will_count)
		{
			DataStr = _opl(DEVICESTATUS_NORMAL_SET);	//����
		}
		else if(count<=will_count && count>count_ed)
		{
			DataStr = _opl(DEVICESTATUS_NEARLY_EMPTY_TXT);		//����
		}
		else if(count <= count_ed)
		{
			DataStr = _opl(DEVICESTATUS_EMPTY_TXT);	//�ѿ�
		}
	}
	// Ӳ��ѭ��������
	else
	{
		if(count > will_count && count < count_ed)
		{
			DataStr = _opl(DEVICESTATUS_NORMAL_SET);			// ����
		}
		else if(count <= will_count && count > 0)
		{
			DataStr = _opl(DEVICESTATUS_NEARLY_EMPTY_TXT);		// ����
		}
		else if(count >= count_ed)
		{
			DataStr = _opl(DEVICESTATUS_FULL_TXT);				// ����
		}
        else if(count == 0){
			DataStr = _opl(DEVICESTATUS_EMPTY_TXT);				// �ѿ�
		}
	}
	return DataStr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��Ǯ�䡢Ʊ�佫������������

@param      remaindata��				ʣ��ö/����		
@param      nearlyfullboxdata��		����ö/����
@param      fullboxdata��			����ö/����

@retval     CString

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CDeviceStatusSvc::StatusChangeFull(CString remaindata,CString nearlyfullboxdata,CString fullboxdata)
{
	int count = -1;
	int will_count = -1;
	int count_ed = -1;
	CString DataStr = _T("");

	count = _ttoi(remaindata);				
	will_count = _ttoi(nearlyfullboxdata);		
	count_ed = _ttoi(fullboxdata);	
	
	if(count >= count_ed)
	{
		DataStr = _opl(DEVICESTATUS_FULL_TXT);				// ����
	}
	else if(count>=will_count && count<=count_ed)	
	{
		DataStr = _opl(DEVICESTATUS_NEARLY_FULL_TXT);		// ����
	}
	else if(count < will_count)
	{
		DataStr = _opl(DEVICESTATUS_NORMAL_SET);			// ����
	}		
	return DataStr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDeviceStatusSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}
