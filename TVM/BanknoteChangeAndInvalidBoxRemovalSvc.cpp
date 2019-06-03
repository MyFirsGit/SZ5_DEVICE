#include "StdAfx.h"
#include "BanknoteChangeAndInvalidBoxRemovalSvc.h"
#include "BanknoteChangeAndInvalidBoxRemovalDlg.h"
#include "BusinessTranslateSvc.h"
#include "SvcDef.h"
#include "TemplateHelper.h"

BEGIN_MESSAGE_MAP(CBanknoteChangeAndInvalidBoxRemovalSvc,CTVMForeService)
END_MESSAGE_MAP()

CBanknoteChangeAndInvalidBoxRemovalSvc::CBanknoteChangeAndInvalidBoxRemovalSvc(void)
	:CTVMForeService(REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC)
{
	m_pDialogFlow->AddDialog(CBanknoteChangeAndInvalidBoxRemovalDlg::IDD, new CBanknoteChangeAndInvalidBoxRemovalDlg(this));
	m_pDialogFlow->AddDialogGroup(DAILOG_CHANGE_INVALID_BOX_REMOVAL,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DAILOG_CHANGE_INVALID_BOX_REMOVAL, CBanknoteChangeAndInvalidBoxRemovalDlg::IDD);

	m_pDialogFlow->SetFirstDialogGroup(DAILOG_CHANGE_INVALID_BOX_REMOVAL);

	m_nEnterTimes = 0;
	m_nIndex	  = 0;
}


CBanknoteChangeAndInvalidBoxRemovalSvc::~CBanknoteChangeAndInvalidBoxRemovalSvc(void)
{
}

CBanknoteChangeAndInvalidBoxRemovalSvc::Model& CBanknoteChangeAndInvalidBoxRemovalSvc::GetDataModel()
{
	return m_model;
}

void CBanknoteChangeAndInvalidBoxRemovalSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

LRESULT CBanknoteChangeAndInvalidBoxRemovalSvc::OnKeyboardEnter(WPARAM, LPARAM)
{
	int nInput = theAPP_SESSION.GetInputToInt() ;
	if (nInput <0 || nInput > 1){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		Sleep(1500);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
		return 1;
	}
	
	// 判断箱子是否可以被卸载?
	if(nInput == CHANGE_BOX){

		if(theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().banknoteBoxID.ToString() != INVALID_ID && 
			!BANKNOTE_HELPER->IsCashChangeBoxAttached()){
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR__CHANGE_BOX_NOT_IN_PLACE_UNINSTALL));
				return false;
		}
	}
	/*else if(nInput == CHANGE_BOX_B){
		if(theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().banknoteBoxID.ToString() != INVALID_ID && 
			!m_bhChangeInfo.CassetteStatus.isSecondBoxExist){
				theAPP_SESSION.ShowOfficeGuide(_T("找零箱B不在位，无法卸载！"));
				return false;
		}
	}*/

	// 如果箱子可被卸载
	if (IsBoxCouldBeRemoved((BOX_LOC)nInput)){
		SetStateOfPreRemove((BOX_LOC)nInput);		// 更新状态为待卸载
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
	else 
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_ALREADY));
	
	NotifyDataModelChanged();
	return 0;
}

LRESULT CBanknoteChangeAndInvalidBoxRemovalSvc::OnKeyboardF2(WPARAM , LPARAM)
{

	if(m_model.changeBoxAData.boxState != WAIT_TO_BE_REMOVED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_UNINSTALL_FAILED_SAVE));
		return 0;
	}

	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO box_info;
	BYTE line = 0xFF;
	BYTE Type = 0xFF;
	BYTE NumInGrup = 0xFF;
	WORD wNum = 0xFFFF;

	CString strTips = _T("");

	// 找零箱A
	if (m_model.changeBoxAData.boxState == WAIT_TO_BE_REMOVED)
	{
		line = m_Ex_change.banknoteBoxID.bLineCode;//_tcstoul(m_model.changeBoxAData.strID.Left(2),NULL,10);
		Type = m_Ex_change.banknoteBoxID.bType;    //_tcstoul(m_model.changeBoxAData.strID.Mid(2,2),NULL,10);
		NumInGrup = m_Ex_change.banknoteBoxID.bNumInQu;
		wNum = m_Ex_change.banknoteBoxID.bNum;     //_tcstoul(m_model.changeBoxAData.strID.Right(4),NULL,10);

		CASHBOX_CHANGE change;
		change.SetBoxID(line,Type, NumInGrup,wNum);		//纸币箱ID
		change.handleType = UNINSTALL;			//操作类型
		change.position = MAGAZINEH;//位置
		change.State = VALID_DETACH;//状态
		CASH_TYPE_COUNT cash;
		cash.cash_type  = TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());	// 5元
		cash.cash_count = m_model.changeBoxAData.nCount;
		change.vecCash.push_back(cash);
		change.btCount = 0x01;

		InsertOperaterToDatabase(CHANGE_BOX);

		box_info.operateTime = ComGetCurTime();
		box_info.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
		if (theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(box_info)){
			strTips += _opl(TXT_BANKNOTEMGY_SUPPLE_BOX_UNINSTALL_OK);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_REMOVAL, true);
			m_model.changeBoxAData.boxState = REMOVED;
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
			m_model.changeBoxAData.strID = _T("FFFFFFFF");
			m_model.changeBoxAData.nCount = 0;

			LOG("卸载纸币补充箱成功：%d", cash.cash_count);
			theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
			theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
		}else{// 失败
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_REMOVAL, false);
			strTips += _opl(TXT_BANKNOTEMGY_SUPPLE_BOX_UNINSTALL_FAILED);
		}
	}

	//// 找零箱B
	//if (m_model.changeBoxBData.boxState == WAIT_TO_BE_REMOVED)
	//{
	//	line = m_Ex_changeB.banknoteBoxID.bLineCode;//_tcstoul(m_model.changeBoxBData.strID.Left(2),NULL,10);
	//	Type = m_Ex_changeB.banknoteBoxID.bType;    //_tcstoul(m_model.changeBoxBData.strID.Mid(2,2),NULL,10);
	//	wNum = m_Ex_changeB.banknoteBoxID.wNum;     //_tcstoul(m_model.changeBoxBData.strID.Right(4),NULL,10);

	//	CASHBOX_CHANGE change;
	//	change.SetBoxID(line,Type, wNum); //纸币箱ID
	//	change.handleType = UNINSTALL; //操作类型
	//	change.position = BANKNOTE_CHANGE_BOX_B_POS; //位置
	//	change.State = VALID_DETACH;//状态
	//	CASH_TYPE_COUNT cash;
	//	cash.cash_type  = TransBanknoteType(theTVM_INFO.GetBHChangeBoxBType());						// 5元
	//	cash.cash_count = m_model.changeBoxBData.nCount;
	//	change.vecCash.push_back(cash);
	//	change.btCount = 0x01;

	//	InsertOperaterToDatabase(CHANGE_BOX_B);

	//	if (theBANKNOTE_COUNT.SetBanknoteChangeBoxBInfo(box_data)){
	//		isDone = true;
	//		strTips += _T("找零箱B卸载成功。");
	//		m_model.changeBoxBData.boxState = REMOVED;
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_REMOVAL, true);
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
	//		m_model.changeBoxBData.strID = _T("FFFFFFFF");
	//		m_model.changeBoxBData.nCount = 0;
	//	}else{// 失败
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CHANGE_BOX_REMOVAL, false);
	//		strTips += _T("找零箱B卸载失败。");
	//	}
	//	theTVM_STATUS_MGR.SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
	//	theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//}

	//if (m_model.invalidBoxData.boxState == WAIT_TO_BE_REMOVED)
	//{
	//	line = m_Ex_invalid.banknoteBoxID.bLineCode;//_tcstoul(m_model.changeBoxBData.strID.Left(2),NULL,10);
	//	Type = m_Ex_invalid.banknoteBoxID.bType;    //_tcstoul(m_model.changeBoxBData.strID.Mid(2,2),NULL,10);
	//	wNum = m_Ex_invalid.banknoteBoxID.wNum;     //_tcstoul(m_model.changeBoxBData.strID.Right(4),NULL,10);

	//	CASHBOX_CHANGE change;
	//	change.SetBoxID(line,Type, wNum); //纸币箱ID
	//	change.handleType = UNINSTALL; //操作类型
	//	change.position = BANKNOTE_INVALID_POS; //位置
	//	change.State = VALID_DETACH;//状态
	//	
	//	CASH_TYPE_COUNT cash;
	//	cash.cash_type  = TransBanknoteType(theTVM_INFO.GetBHChangeBoxAType());						// 5元
	//	cash.cash_count = m_model.invalidBoxData.nCount;
	//	change.vecCash.push_back(cash);
	//	change.btCount = 0x01;

	//	InsertOperaterToDatabase(INVALID_BOX);

	//	if (theBANKNOTE_COUNT.SetBanknoteInvalidBoxInfo(box_data)){
	//		isDone = true;
	//		strTips += _T("废币回收箱卸载成功。");
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INVALID_BOX_REMOVAL, true);
	//		m_model.invalidBoxData.boxState = REMOVED;
	//		m_model.invalidBoxData.strID = _T("FFFFFFFF");
	//		m_model.changeBoxAData.nCount = 0;
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change); // 电文
	//	}
	//	else{// 失败
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INVALID_BOX_REMOVAL, false);
	//		strTips += _T("废币回收箱卸载失败。");
	//	}
	//}
	strTips += _opl(GUIDE_F3_ESC);

	theAPP_SESSION.ShowOfficeGuide(strTips);

	if ( m_model.changeBoxAData.boxState == REMOVED)	{ // 如果有操作且成功 打印凭条
		PrintBanknoteChangeReceiption();

		if(m_model.bIsPrintFailed){
			Sleep(1000);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败		
		}
	}
	
	// 初始化数据
	m_Ex_change = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	//m_Ex_changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	//m_Ex_invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();

	NotifyDataModelChanged();
	return 0;
}

LRESULT CBanknoteChangeAndInvalidBoxRemovalSvc::OnKeyboardF3(WPARAM , LPARAM)
{
	NotifyDataModelChanged();
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}


void CBanknoteChangeAndInvalidBoxRemovalSvc::OnStart()
{
	__super:: OnStart();

	InitializeData();
	m_Ex_change = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	/*m_Ex_changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	m_Ex_invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();*/
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      判断箱子是否可被卸载

@param      loc 箱子位置
@retval     None
*/
//////////////////////////////////////////////////////////////////////////
bool CBanknoteChangeAndInvalidBoxRemovalSvc::IsBoxCouldBeRemoved(BOX_LOC loc)
{
	bool yes = true;
	
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_B = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_C = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();
	*/

	if (changeBox.banknoteBoxID.ToString() != _T("FFFFFFFF"))
	{
		yes = true;
	}
	else{
		yes = false;
	}
	
	return yes;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置指定箱子的状态为待卸载

@param      loc 箱子位置

@retval     None
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeAndInvalidBoxRemovalSvc::SetStateOfPreRemove(BOX_LOC loc)
{
	switch(loc)
	{
	case CHANGE_BOX:
		{
			m_model.changeBoxAData.boxState = WAIT_TO_BE_REMOVED;
		}
		break;

		/*case CHANGE_BOX_B:
		{
		m_model.changeBoxBData.boxState = WAIT_TO_BE_REMOVED;
		}
		break;

		case INVALID_BOX:
		{
		m_model.invalidBoxData.boxState = WAIT_TO_BE_REMOVED;
		}
		break;
		case ALL_BOX:
		if (IsBoxCouldBeRemoved(CHANGE_BOX_A)){
		m_model.changeBoxAData.boxState = WAIT_TO_BE_REMOVED;
		}
		if (IsBoxCouldBeRemoved(CHANGE_BOX_B)){
		m_model.changeBoxBData.boxState = WAIT_TO_BE_REMOVED;
		}
		if (IsBoxCouldBeRemoved(INVALID_BOX)){
		m_model.invalidBoxData.boxState = WAIT_TO_BE_REMOVED;
		}
		break;*/

	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置指定箱子的状态为待卸载

@param      loc 箱子位置

@retval     None
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeAndInvalidBoxRemovalSvc::InitializeData()
{
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

	// 找零箱
	if (changeBox.banknoteBoxID.bLineCode == 0xFF){
		m_model.changeBoxAData.strID = _T("FFFFFFFF");
		m_model.changeBoxAData.boxState = REMOVED;
	}else{
		m_model.changeBoxAData.boxState = INSTALLED;
	}
	m_model.changeBoxAData.nCount = changeBox.ulCurCount;
	m_model.changeBoxAData.strID = changeBox.banknoteBoxID.ToString();

	//// 找零箱B
	//if (box_B.banknoteBoxID.bLineCode == 0xFF){
	//	m_model.changeBoxBData.strID = _T("FFFFFFFF");
	//	m_model.changeBoxBData.boxState = REMOVED;
	//}else{
	//	m_model.changeBoxBData.boxState = INSTALLED;
	//}
	//m_model.changeBoxBData.nCount = box_B.ulCurCount;
	//m_model.changeBoxBData.strID = box_B.banknoteBoxID.ToString();

	//// 废币回收箱
	//if (box_C.banknoteBoxID.bLineCode == 0xFF){
	//	m_model.invalidBoxData.strID = _T("FFFFFFFF");
	//	m_model.invalidBoxData.boxState = REMOVED;
	//}else{
	//	m_model.invalidBoxData.boxState = INSTALLED;
	//}
	//m_model.invalidBoxData.nCount = box_C.ulCurCount;
	//m_model.invalidBoxData.strID = box_C.banknoteBoxID.ToString();

	m_model.bIsPrintFailed = false;

}

CString CBanknoteChangeAndInvalidBoxRemovalSvc::TranslateState(BOX_STATE boxState)
{

	if (boxState == INSTALLED)
	{
		return _opl(TXT_BANKNOTEMGR_BOX_ATTACHED);
	}else if (boxState == REMOVED)
	{
		return _opl(TXT_SMARTCARDMGR_DETACHED);
	}else if (boxState == WAIT_TO_BE_INSTALLED)
	{
		return _opl(TXT_BANKNOTEMGR_BOX_READY_INSTALL);
	}else if (boxState ==WAIT_TO_BE_REMOVED)
	{
		return _opl(TXT_SMARTCARDMGR_WAITTODETACH);
	}else{
		return _T("");
	}
}


void CBanknoteChangeAndInvalidBoxRemovalSvc::PrintBanknoteChangeReceiption()
{
	try
	{
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
		/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
		CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();*/


		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), m_Ex_change.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), changeBox.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"), 0);
		strFace1Count_after.Format(_T("%d"), 0);
		// 5元
		CString strFace5Count_before = _T("");
		CString strFace5Count_after = _T("");
		strFace5Count_before.Format(_T("%d"), m_Ex_change.ulCurCount);
		strFace5Count_after.Format(_T("%d"), changeBox.ulCurCount);
		// 10元
		CString strFace10Count_before = _T("");
		CString strFace10Count_after = _T("");
		strFace10Count_before.Format(_T("%d"),0);
		strFace10Count_after.Format(_T("%d"), 0);
		// 20元
		CString strFace20Count_before = _T("");
		CString strFace20Count_after = _T("");
		strFace20Count_before.Format(_T("%d"), 0);
		strFace20Count_after.Format(_T("%d"), 0);
		// 50元
		CString strFace50Count_before = _T("");
		CString strFace50Count_after = _T("");
		strFace50Count_before.Format(_T("%d"), 0);
		strFace50Count_after.Format(_T("%d"), 0);
		// 100元
		CString strFace100Count_before = _T("");
		CString strFace100Count_after = _T("");
		strFace100Count_before.Format(_T("%d"), 0);
		strFace100Count_after.Format(_T("%d"), 0);

		// 加载Template
		CString CoinBox_templatename = _T("TVM_BANK_NOTE_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));																			// 操作类型
			line.Replace(_T("{钱箱}"), _T("纸币找零箱"));
			line.Replace(_T("{操作前钱箱号}"), m_Ex_change.banknoteBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), changeBox.banknoteBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), m_Ex_change.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), changeBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{操作前5元}"), strFace5Count_before);
			line.Replace(_T("{操作后5元}"), strFace5Count_after);
			line.Replace(_T("{操作前10元}"), strFace10Count_before);
			line.Replace(_T("{操作后10元}"), strFace10Count_after);
			line.Replace(_T("{操作前20元}"), strFace20Count_before);
			line.Replace(_T("{操作后20元}"), strFace20Count_after);
			line.Replace(_T("{操作前50元}"), strFace50Count_before);
			line.Replace(_T("{操作后50元}"), strFace50Count_after);
			line.Replace(_T("{操作前100元}"), strFace100Count_before);
			line.Replace(_T("{操作后100元}"), strFace100Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_UNINSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_model.bIsPrintFailed = true;
		//throw;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_model.bIsPrintFailed = true;
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      箱子操作插入库

@param      BOX_INDEX boxType 箱子类型

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeAndInvalidBoxRemovalSvc::InsertOperaterToDatabase(BOX_LOC boxType){
	try{
		Data::BanknoteOperationRecord banknoteOpRecord;

		banknoteOpRecord.dtTransTime = ComGetCurTime();
		banknoteOpRecord.iOperationType = 0x02;// 卸载
		banknoteOpRecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
		if(boxType == CHANGE_BOX){
			banknoteOpRecord.txtBanknoteBoxID = m_model.changeBoxAData.strID;
			banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());
			banknoteOpRecord.iQuantity		   = m_model.changeBoxAData.nCount;
		}
		/*else if(boxType == CHANGE_BOX_B){
		banknoteOpRecord.txtBanknoteBoxID = m_model.changeBoxBData.strID;
		banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_INFO.GetBHChangeBoxBType());
		banknoteOpRecord.iQuantity		   = m_model.changeBoxBData.nCount;
		}
		else if(boxType == INVALID_BOX){
		banknoteOpRecord.txtBanknoteBoxID = m_model.invalidBoxData.strID;
		banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_INFO.GetBHChangeBoxAType());
		banknoteOpRecord.iQuantity		   = m_model.invalidBoxData.nCount;
		}*/
		else{
			return;
		}
		DATA_HELPER->DbInsertBanknoteOperationRecord(banknoteOpRecord);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析纸币面值代码 BIN	1：1元，2：5元，3：10元，4：20元，5：50元，6：100元

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CBanknoteChangeAndInvalidBoxRemovalSvc::TransBanknoteType(int value) {
	switch (value) {
	case 1: return Banknote1Yuan;
	case 2: return Banknote5Yuan;
	case 3: return Banknote10Yuan;
	case 4: return Banknote20Yuan;
	case 5: return Banknote50Yuan;
	case 6: return Banknote100Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}