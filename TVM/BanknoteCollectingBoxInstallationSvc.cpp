#include "StdAfx.h"
#include "BanknoteCollectingBoxInstallationSvc.h"
#include "BanknoteCollectingBoxInstallationDlg.h"
#include "TemplateHelper.h"

//static const TCHAR* const WM_CBMGR_BASE_TO_OPERATION = _T("WM_CBMGR_BASE_TO_OPERATION");
static const TCHAR* const WM_CBMGR_OPERATION_TO_FINISH = _T("WM_CBMGR_OPERATION_TO_FINISH");
static const TCHAR* const WM_CBMGR_OPERATION_TO_ERROR = _T("WM_CBMGR_OPERATION_TO_ERROR");
//static const TCHAR* const WM_CBMGR_OPERATION_TO_BASE = _T("WM_CBMGR_OPERATION_TO_BASE");
static const TCHAR* const WM_CBMGR_FINISH_TO_OPERATION = _T("WM_CBMGR_FINISH_TO_OPERATION");
static const TCHAR* const WM_CBMGR_ERROR_TO_OPERATION = _T("WM_CBMGR_ERROR_TO_OPERATION");

BEGIN_MESSAGE_MAP(CBanknoteCollectingBoxInstallationSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxInstallationSvc::CBanknoteCollectingBoxInstallationSvc()
	:CTVMForeService(INSTALL_BANKNOTE_COLLECTION_BOX_SVC)
{
    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CBanknoteCollectingBoxInstallationDlg(this));

    // 设置画面分组
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION, CBanknoteCollectingBoxInstallationDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, CBanknoteCollectingBoxInstallationDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, CBanknoteCollectingBoxInstallationDlg::IDD);

	// 设置画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_FINISH, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_ERROR, DIALOG_GROUP_ERROR);
	/*m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_BASE, DIALOG_GROUP_BASE);*/
	m_pDialogFlow->AddFlow(DIALOG_GROUP_FINISH, WM_CBMGR_FINISH_TO_OPERATION/*WM_CBMGR_FINISH_TO_BASE*/, DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ERROR, WM_CBMGR_ERROR_TO_OPERATION, DIALOG_GROUP_OPERATION);

    // 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OPERATION);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxInstallationSvc::~CBanknoteCollectingBoxInstallationSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxInstallationSvc::Model& CBanknoteCollectingBoxInstallationSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteCollectingBoxInstallationSvc::OnKeyboardF3(WPARAM, LPARAM)
{
	int nID = GetCurDialogGroupID();
	theAPP_SESSION.ReturnMaintainMenu();
	NotifyDataModelChanged();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteCollectingBoxInstallationSvc::OnKeyboardF2(WPARAM , LPARAM )
{
	// 只有在操作界面才响应F2
	if(GetCurDialogGroupID() != DIALOG_GROUP_OPERATION){
		return FALSE;
	}
	try{
		// 纸币箱安装操作
		if ( m_DataModel.strState == _opl(TXT_BANKNOTEMGR_BOX_READY_INSTALL)){
			if ( IsBoxConnected()){
				BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
				BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
				BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode();
				CTBanknoteCountInfo::BANKNOTE_BOX_INFO box_data;
				// 
				int2BCD(bLineCode, (char*)&box_data.banknoteBoxID.bLineCode, sizeof(box_data.banknoteBoxID.bLineCode));
				box_data.banknoteBoxID.bType = 0x08;
				int2BCD(bStationCode, (char*)&box_data.banknoteBoxID.bNumInQu, sizeof(box_data.banknoteBoxID.bNumInQu));
				int2BCD(bMachineCode, (char*)&box_data.banknoteBoxID.bNum, sizeof(box_data.banknoteBoxID.bNum));
				box_data.operateTime = ComGetCurTime();
				box_data.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);

				m_DataModel.strBoxIDSt = box_data.banknoteBoxID.ToString();
				if(theBANKNOTE_COUNT.SetBanknoteboxInfo(box_data)){
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_COMPLETED));
					LoadBankNoteBoxInstallReception();
					//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,true);
					DoDialogFlow(WM_CBMGR_OPERATION_TO_FINISH);
				}else{
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FAILED));
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
				}
				//将纸币箱安装操作存入数据库
				try{
					BanknoteBoxInstallOperationRecord banknoteBoxInstallOperationRecord;
					banknoteBoxInstallOperationRecord.dtTransTime = ComGetCurTime();
					banknoteBoxInstallOperationRecord.txtBanknoteBoxID =  m_DataModel.strBoxIDSt;
					banknoteBoxInstallOperationRecord.iTotalAmount = 0;
					banknoteBoxInstallOperationRecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
					DATA_HELPER->DbInsertBanknoteBoxInstallOperationRecord(banknoteBoxInstallOperationRecord);
				}
				catch(CSysException& e)
				{
					theEXCEPTION_MGR.ProcessException(e);
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_INSERT_DB_FAILED));
				}

				// 发送纸币箱安装的电文
				CASHBOX_CHANGE change;
				change.SetBoxID(box_data.banknoteBoxID.bLineCode, box_data.banknoteBoxID.bType, box_data.banknoteBoxID.bNumInQu, box_data.banknoteBoxID.bNum); //箱子ID
				change.handleType       = INSTALL;
				change.position         = MAGAZINEA;
				change.State            = VALID_ATTACH;
				change.BanNoteBox_Initial();//空箱
				change.btCount = change.vecCash.size();//change.GetSumCurrency();//取得币种总数

				LOG("安装纸币回收箱成功！");
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);//发电文
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,true);

				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FORBBIDEN));
			}

			if(m_DataModel.bIsPrintFailed){
				Sleep(1000);
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败		
			}
			NotifyDataModelChanged();
		}
	}//异常处理
	catch (CSysException& e) {
		// 写操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		// 写操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteCollectingBoxInstallationSvc::OnKeyboardEnter(WPARAM, LPARAM)
{
	m_strShowMsg = _T("");
	switch (m_DataModel.m_phase)
	{
	case PHASE_SET_COMMOND:
		{
			if (theAPP_SESSION.GetInputToInt() >1 || theAPP_SESSION.GetInputToInt() <= 0){
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
				return 1;
			}else{
				if (m_DataModel.strState == _opl(TXT_BANKNOTEMGR_BOX_ATTACHED)){
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_INSTALL));
					return 2;
				}
			}
			
			// 首先判断纸币箱是否物理安装
			if(!BANKNOTE_HELPER->IsCashBoxAttached()){
				bool bIsException = false;
				// 安装后需要复位才可以读到状态
				try{
					BANKNOTE_HELPER->Reset();
				}
				catch(CSysException& e){
					bIsException = true;
					theEXCEPTION_MGR.ProcessException(e);
				}
				catch(...){
					bIsException = true;
				}

				// 纸币箱异常
				if(bIsException){
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FORBBIDEN));
					return -1;
				}

				// 复位后还没有检查到箱子状态，提示箱子不在位，无法安装
				if(!BANKNOTE_HELPER->IsCashBoxAttached()){
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_STANDBY_FOR_UNINSTALL));
					return -1;
				}
			}
			//theAPP_SESSION.SetMaxInputLength(4);			
			//theAPP_SESSION.SetPrefixToGuide(_opl(TXT_BANKNOTEMGR_PLEASE_INPUT_BOX_ID) + _opl(TXT_COLON) + m_DataModel.strPreID);
			//m_DataModel.m_phase = PHASE_SET_ID;
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			m_DataModel.m_phase = PHASE_SET_COMMOND;
			m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_READY_INSTALL);
			m_DataModel.clrState = SOFTGREEN;
			theAPP_SESSION.SetMaxInputLength(1);
		}break;
	case PHASE_SET_ID:
		{
			int inputNum = theAPP_SESSION.GetInputToInt();
			if(inputNum == 0){
				theAPP_SESSION.SetPrefixToGuide(_opl(TXT_BANKNOTEMGR_INPUTID_NOT_AS_EXPECT) + _opl(TXT_COLON) + m_DataModel.strPreID);
				return false;
			}
			else{
				m_DataModel.strBoxIDSt.Format(_T("%02d61%04d"),theMAINTENANCE_INFO.GetLineCode(),inputNum);
			}
			
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			m_DataModel.m_phase = PHASE_SET_COMMOND;
			m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_READY_INSTALL);
			m_DataModel.clrState = SOFTGREEN;
			theAPP_SESSION.SetMaxInputLength(1);
		}break;
	}
	NotifyDataModelChanged();

	return 0;
}


void CBanknoteCollectingBoxInstallationSvc::InitAttachBoxData()
{
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.banknote_box.raw_id = boxInfo.banknoteBoxID.ToString();

	// 如果BanknoteCountInfo记录ID为00000000，则假设无纸币箱。无需硬件检查。
	if (boxInfo.banknoteBoxID == CTBanknoteCountInfo::_banknote_box_id()) {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = false;
	}
	else {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = true;
	}
}



//检测纸币箱ID是否合法
bool CBanknoteCollectingBoxInstallationSvc::CheckBoxID(LPBYTE lpBoxID)
{
	//LineCode ，Type， Number构成BankNoteBoxID
	//如果取得的BankNoteBoxID 前四位 != 0192
	//return false;
	//如果取得的BankNoteBoxID 长度 != 8
	//return false;

	return true;
}

//纸币箱安装凭证
void CBanknoteCollectingBoxInstallationSvc::LoadBankNoteBoxInstallReception()
{

	try
	{
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO Col_info = theBANKNOTE_COUNT.GetBanknoteboxInfo();

		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), m_Ex_Col.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), Col_info.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"),  m_Ex_Col.ulCount_CNY1);
		strFace1Count_after.Format(_T("%d"), Col_info.ulCount_CNY1);
		// 5元
		CString strFace5Count_before = _T("");
		CString strFace5Count_after = _T("");
		strFace5Count_before.Format(_T("%d"), m_Ex_Col.ulCount_CNY5);
		strFace5Count_after.Format(_T("%d"), Col_info.ulCount_CNY5);
		// 10元
		CString strFace10Count_before = _T("");
		CString strFace10Count_after = _T("");
		strFace10Count_before.Format(_T("%d"), m_Ex_Col.ulCount_CNY10);
		strFace10Count_after.Format(_T("%d"), Col_info.ulCount_CNY10);
		// 20元
		CString strFace20Count_before = _T("");
		CString strFace20Count_after = _T("");
		strFace20Count_before.Format(_T("%d"), m_Ex_Col.ulCount_CNY20);
		strFace20Count_after.Format(_T("%d"), Col_info.ulCount_CNY20);
		// 50元
		CString strFace50Count_before = _T("");
		CString strFace50Count_after = _T("");
		strFace50Count_before.Format(_T("%d"), m_Ex_Col.ulCount_CNY50);
		strFace50Count_after.Format(_T("%d"), Col_info.ulCount_CNY50);
		// 100元
		CString strFace100Count_before = _T("");
		CString strFace100Count_after = _T("");
		strFace100Count_before.Format(_T("%d"), m_Ex_Col.ulCount_CNY100);
		strFace100Count_after.Format(_T("%d"), Col_info.ulCount_CNY100);

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
			line.Replace(_T("{钱箱}"), _T("纸币回收箱"));
			line.Replace(_T("{操作前钱箱号}"), m_Ex_Col.banknoteBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), Col_info.banknoteBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), m_Ex_Col.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), Col_info.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
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
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_INSTALL);

		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		//m_strShowMsg = _opl(TXT_COINMGR_PRINTER_FAILURE);
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.bIsPrintFailed = true;
		//throw;
	}
	catch (...) {
		//m_strShowMsg = _opl(TXT_COINMGR_PRINTER_FAILURE);
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.bIsPrintFailed = true;
	}
	//try{
	//	CStringArray saBillTemplate;
	//	CStringArray saTemp;
	//	theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BANK_NOTE_BOX.template"), saBillTemplate);
	//	for ( int i = 0; i < saBillTemplate.GetCount();i++)
	//	{
	//		CString strLine = saBillTemplate.ElementAt(i);
	//		strLine.Replace(_T("{纸币箱ID}"), m_DataModel.banknote_box.now_id);
	//		strLine.Replace(_T("{卸载前金额}"), ComFormatAmount(100*(m_DataModel.banknote_box.face10*10
	//			+ m_DataModel.banknote_box.face20*20
	//			+ m_DataModel.banknote_box.face50*50
	//			+ m_DataModel.banknote_box.face100*100
	//			+ m_DataModel.banknote_box.face5 *5 
	//			+ m_DataModel.banknote_box.face1)));
	//		CString strCurTime = ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
	//		theAPP_SESSION.GetUserInfo().sUserID;
	//		strLine.Replace(_T("{操作时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
	//		strLine.Replace(_T("{操作员编号}"), theAPP_SESSION.GetUserInfo().sUserID);
	//		strLine.Replace(_T("{车站名称_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
	//		strLine.Replace(_T("{车站名称_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
	//		strLine.Replace(_T("{车站名称_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
	//		strLine.Replace(_T("{设备ID}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

	//		CStringArray saText;
	//		ComPrintLineFormatForTemplate(strLine, saText);
	//		saTemp.Append(saText);

	//	}
	//	PRINTER_HELPER->PrintReceiptByBmp(&saTemp,true,true);//打印凭证
	//}
	//catch (CSysException&e)
	//{
	//	//throw;
	//	theEXCEPTION_MGR.ProcessException(e);
	//	m_strShowMsg += _T("打印机异常，打印失败");
	//}
	//catch (...)
	//{
	//	throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	//	theAPP_SESSION.ShowOfficeGuide(_T("安装成功；凭证打印失败。"));
	//}
}

//格式化纸币面值
CString CBanknoteCollectingBoxInstallationSvc::CashValueToString(BankNoteAndCoinType_t& value)
{
	CString strValue;
	switch(value)
	{

	case  Banknote1Yuan: strValue = _T("1 元"); break;
	case  Banknote5Yuan: strValue = _T("5 元"); break;
	case  Banknote10Yuan: strValue = _T("10 元"); break;
	case  Banknote20Yuan: strValue = _T("20 元"); break;
	case  Banknote50Yuan: strValue = _T("50 元"); break;
	case  Banknote100Yuan: strValue = _T("100 元"); break;
	default: break;
	}
	return strValue;
}

//判断纸币箱是否连接
bool CBanknoteCollectingBoxInstallationSvc::IsBoxConnected()
{
	/*if ( BANKNOTE_HELPER != NULL && BANKNOTE_HELPER->IsBankNoteBoxAttached())
	{
	return true;
	}
	return false;*/
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取显示错误文言

@param      无

@retval     CString strMsg

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CBanknoteCollectingBoxInstallationSvc::GetShowErrorMsg(){
	return m_strShowMsg;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Service启动

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationSvc::OnStart()
{
	__super::OnStart();

	m_Ex_Col = theBANKNOTE_COUNT.GetBanknoteboxInfo();

	m_DataModel = _tagModel();
	InitializeDataModel();
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OPERATION);
	m_pDialogFlow->ShowFirstDialogGroup();
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
	m_strShowMsg = _T("");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化数据模型

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationSvc::InitializeDataModel()
{
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO BN_data = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.strBoxIDEx = BN_data.banknoteBoxID.ToString();
	if (m_DataModel.strBoxIDEx == _T("FFFFFFFF")){
		m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_DETACHED);
		m_DataModel.clrState = RED;
	}else{
		m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_ATTACHED);
		m_DataModel.clrState = GREEN;
	}
	m_DataModel.strBoxIDSt = m_DataModel.strBoxIDEx;
	m_DataModel.m_phase = PHASE_SET_COMMOND;
}


//void CBanknoteCollectingBoxInstallationSvc::PrintBanknoteChangeReceiption()
//{
	//try
	//{
	//	CTBanknoteCountInfo::BANKNOTE_BOX_INFO Col_info = theBANKNOTE_COUNT.GetBanknoteboxInfo();

	//	DatalightInfo coindata;
	//	CString coincount = _T("");			
	//	CString cointotal;

	//	CString CoinBox_templatename = _T("TVM_BANK_NOTE_BOX.template");
	//	CStringArray printtext;
	//	CStringArray TemplateContents;

	//	COIN_HELPER->CoinDataRead(coindata);

	//	BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

	//	int Index = 0;
	//	CString strCount;

	//	while(Index<TemplateContents.GetCount())
	//	{
	//		CString& line = TemplateContents.ElementAt(Index);	
	//		line.Replace(_T("{操作}"),_T("安装"));																				// 操作类型


	//		// 操作前 ID
	//		line.Replace(_T("{前ID}"),m_Ex_Col.banknoteBoxID.ToString());
	//		
	//		line.Replace(_T("{前金额}"),ComFormatAmount(m_Ex_Col.BankNotesCountAvos()));

	//		line.Replace(_T("{前数01}"),ComFormatCount(m_Ex_Col.ulCount_1)); 
	//		line.Replace(_T("{前数05}"),ComFormatCount(m_Ex_Col.ulCount_5));
	//		line.Replace(_T("{前数10}"),ComFormatCount(m_Ex_Col.ulCount_10));
	//		line.Replace(_T("{前数20}"),ComFormatCount(m_Ex_Col.ulCount_20));
	//		line.Replace(_T("{前数50}"),ComFormatCount(m_Ex_Col.ulCount_50));
	//		line.Replace(_T("{前数00}"),ComFormatCount(m_Ex_Col.ulCount_100));

	//		// 操作前各个币种的金额
	//		line.Replace(_T("{前金01}") ,ComFormatAmount(m_Ex_Col.ulCount_1*1000)); 
	//		line.Replace(_T("{前金05}") ,ComFormatAmount(m_Ex_Col.ulCount_5*5000)); 
	//		line.Replace(_T("{前金10}") ,ComFormatAmount(m_Ex_Col.ulCount_10*10000));
	//		line.Replace(_T("{前金20}") ,ComFormatAmount(m_Ex_Col.ulCount_20*20000));
	//		line.Replace(_T("{前金50}") ,ComFormatAmount(m_Ex_Col.ulCount_50*50000));
	//		line.Replace(_T("{前金00}") ,ComFormatAmount(m_Ex_Col.ulCount_100*100000));


	//		// 操作后
	//		line.Replace(_T("{后ID}"),Col_info.banknoteBoxID.ToString());
	//		line.Replace(_T("{后金额}"),ComFormatAmount(Col_info.BankNotesCountAvos()));
	//		

	//		// 操作后各个币种的数量
	//		line.Replace(_T("{后数01}"),ComFormatCount(Col_info.ulCount_1)); 
	//		line.Replace(_T("{后数05}"),ComFormatCount(Col_info.ulCount_5));
	//		line.Replace(_T("{后数10}"),ComFormatCount(Col_info.ulCount_10));
	//		line.Replace(_T("{后数20}"),ComFormatCount(Col_info.ulCount_20));
	//		line.Replace(_T("{后数50}"),ComFormatCount(Col_info.ulCount_50));
	//		line.Replace(_T("{后数00}"),ComFormatCount(Col_info.ulCount_100));

	//		// 操作后各个币种的金额
	//		line.Replace(_T("{后金01}") ,ComFormatAmount(Col_info.ulCount_1*1000)); 
	//		line.Replace(_T("{后金05}") ,ComFormatAmount(Col_info.ulCount_5*5000)); 
	//		line.Replace(_T("{后金10}") ,ComFormatAmount(Col_info.ulCount_10*10000));
	//		line.Replace(_T("{后金20}") ,ComFormatAmount(Col_info.ulCount_20*20000));
	//		line.Replace(_T("{后金50}") ,ComFormatAmount(Col_info.ulCount_50*50000));
	//		line.Replace(_T("{后金00}") ,ComFormatAmount(Col_info.ulCount_100*100000));


	//		line.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
	//		line.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
	//		line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
	//		line.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

	//		CStringArray receiptline;
	//		ComPrintLineFormatForTemplate(line,receiptline);
	//		printtext.Append(receiptline);
	//		Index++;
	//	}
	//	PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//}
	//catch(CSysException& e) {
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//	theEXCEPTION_MGR.ProcessException(e);

	//	//throw;
	//}
	//catch (...) {
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//}
//}