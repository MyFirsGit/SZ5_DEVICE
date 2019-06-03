#include "StdAfx.h"
#include "BanknoteCollectingBoxRemovalSvc.h"
#include "BanknoteCollectingBoxRemovalDlg.h"
#include "TemplateHelper.h"

//static const TCHAR* const WM_CBMGR_BASE_TO_OPERATION = _T("WM_CBMGR_BASE_TO_OPERATION");
static const TCHAR* const WM_CBMGR_OPERATION_TO_FINISH = _T("WM_CBMGR_OPERATION_TO_FINISH");
static const TCHAR* const WM_CBMGR_OPERATION_TO_ERROR = _T("WM_CBMGR_OPERATION_TO_ERROR");
//static const TCHAR* const WM_CBMGR_OPERATION_TO_BASE = _T("WM_CBMGR_OPERATION_TO_BASE");
static const TCHAR* const WM_CBMGR_FINISH_TO_OPERATION  = _T("WM_CBMGR_FINISH_TO_OPERATION");
static const TCHAR* const WM_CBMGR_ERROR_TO_OPERATION = _T("WM_CBMGR_ERROR_TO_OPERATION");

BEGIN_MESSAGE_MAP(CBanknoteCollectingBoxRemovalSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxRemovalSvc::CBanknoteCollectingBoxRemovalSvc()
	:CTVMForeService(REMOVE_BANKNOTE_COLLECTION_BOX_SVC)
{
    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CBanknoteCollectingBoxRemovalDlg(this));

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION, CBanknoteCollectingBoxRemovalDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, CBanknoteCollectingBoxRemovalDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, CBanknoteCollectingBoxRemovalDlg::IDD);

	// 设置画面流转
	/*m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_CBMGR_BASE_TO_OPERATION, DIALOG_GROUP_OPERATION);*/
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_FINISH, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_ERROR, DIALOG_GROUP_ERROR);
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_FINISH, WM_CBMGR_FINISH_TO_OPERATION, DIALOG_GROUP_OPERATION);
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
CBanknoteCollectingBoxRemovalSvc::~CBanknoteCollectingBoxRemovalSvc()
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
CBanknoteCollectingBoxRemovalSvc::Model& CBanknoteCollectingBoxRemovalSvc::GetDataModel()
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
void CBanknoteCollectingBoxRemovalSvc::NotifyDataModelChanged()
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
LRESULT CBanknoteCollectingBoxRemovalSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
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
LRESULT CBanknoteCollectingBoxRemovalSvc::OnKeyboardF2(WPARAM , LPARAM )
{
	// 只有在操作界面才响应F2
	if(GetCurDialogGroupID() != DIALOG_GROUP_OPERATION){
		return FALSE;
	}

	// 纸币箱卸载操作
	if (m_DataModel.strState == _opl(TXT_BANKNOTEMGR_BOX_READY_UNINSTALL)){
		if(!BANKNOTE_HELPER->IsCashBoxAttached()){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_STANDBY_FOR_UNINSTALL));
			return -1;
		}
		DoHandleDetachBox();
	}
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
LRESULT CBanknoteCollectingBoxRemovalSvc::OnKeyboardEnter(WPARAM, LPARAM)
{

	// 操作画面（安装、卸载）
	if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION){
		int index = theAPP_SESSION.GetInputToInt();
		if(MENU_CODE_1 != index){	// 输入的编号不合法
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return 1;
		}else{
			if(m_DataModel.strState != _opl(TXT_BANKNOTEMGR_BOX_DETACHED)){	// 已安装的回收箱才可卸载
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_READY_UNINSTALL);
				m_DataModel.clrState = SOFTRED;
			}else{// 提示已经卸载，不能重复卸载
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_UNINSTALL));
			}
		}
		NotifyDataModelChanged();
	}
	return 0;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      卸载处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalSvc::DoHandleDetachBox()
{	
	//  清除本地纸币箱的计数信息
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo;
	boxInfo.banknoteBoxID = CTBanknoteCountInfo::_banknote_box_id();
	m_DataModel.box_data.now_id = boxInfo.banknoteBoxID.ToString();
	boxInfo.BanknoteInit();
	boxInfo.operateTime = ComGetCurTime();
	boxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	// 写计数文件失败
	if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGY_RECOVERY_BOX_UNINSTALL_FAILED));
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
	}
	// 写计数文件成功
	else{
		LOG("卸载纸币回收箱成功！1，5，10，20，50，100元纸币数：%d,%d,%d,%d,%d,%d", 
			m_Ex_Col.ulCount_CNY1, m_Ex_Col.ulCount_CNY5, m_Ex_Col.ulCount_CNY10, m_Ex_Col.ulCount_CNY20, m_Ex_Col.ulCount_CNY50, m_Ex_Col.ulCount_CNY100);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGT_REMOVAL_COMPLETED));
		m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_DETACHED);
		LoadBankNoteBoxUninstallReception();
		m_DataModel.clrState = RED;
		m_DataModel.box_data.now_id = _T("FFFFFFFF");

		//// 清空计数
		//try{
		//	BANKNOTE_HELPER->ClearCollectionUnitCount();
		//}
		//catch(...){
		//	LOG("清空纸币回收箱硬件计数失败。");
		//}
	}

	// 生成纸币回收箱卸载电文
	CASHBOX_CHANGE change;
	// 纸币箱ID（卸载前）
	change.SetBoxID(m_Ex_Col.banknoteBoxID.bLineCode, m_Ex_Col.banknoteBoxID.bType, m_Ex_Col.banknoteBoxID.bNumInQu,m_Ex_Col.banknoteBoxID.bNum); 
	// 操作类型
	change.handleType = UNINSTALL; 
	// 位置
	change.position = MAGAZINEA; 
	// 状态
	change.State = VALID_DETACH;
	CASH_TYPE_COUNT cash;
	// 1元
	cash.cash_type  = Banknote1Yuan;					
	cash.cash_count = m_DataModel.box_data.face1;
	change.vecCash.push_back(cash);
	// 5元
	cash.cash_type  = Banknote5Yuan;					
	cash.cash_count = m_DataModel.box_data.face5;
	change.vecCash.push_back(cash);
	// 10元
	cash.cash_type = Banknote10Yuan;		
	cash.cash_count = m_DataModel.box_data.face10;
	change.vecCash.push_back(cash);
	// 20 元
	cash.cash_type  = Banknote20Yuan;						
	cash.cash_count = m_DataModel.box_data.face20;
	change.vecCash.push_back(cash);
	// 50 元
	cash.cash_type  = Banknote50Yuan;						
	cash.cash_count = m_DataModel.box_data.face50;
	change.vecCash.push_back(cash);
	// 100 元
	cash.cash_type  = Banknote100Yuan;						
	cash.cash_count = m_DataModel.box_data.face100;
	change.vecCash.push_back(cash);
	 // 币种总数
	change.btCount = change.vecCash.size();
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);	
	
	// 生成纸币钱箱卸载操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,true);//写日志

	// 插入纸币钱箱卸载操作记录到数据库
	try{
		BanknoteBoxCashDetail detail;
		BanknoteBoxRemoveOperationRecord banknoteBoxRemoveOperationRecord;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.clear();
		// 1元
		detail.iBanknoteTypeCode = Banknote1Yuan;
		detail.iQuantity = m_DataModel.box_data.face1;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
		// 5元
		detail.iBanknoteTypeCode = Banknote5Yuan;
		detail.iQuantity = m_DataModel.box_data.face5;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
		// 10元
		detail.iBanknoteTypeCode = Banknote10Yuan;
		detail.iQuantity = m_DataModel.box_data.face10;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
		// 20元
		detail.iBanknoteTypeCode = Banknote20Yuan;
		detail.iQuantity = m_DataModel.box_data.face20;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
		// 50元
		detail.iBanknoteTypeCode = Banknote50Yuan;
		detail.iQuantity = m_DataModel.box_data.face50;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
		// 100元
		detail.iBanknoteTypeCode = Banknote100Yuan;
		detail.iQuantity = m_DataModel.box_data.face100;
		banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
		// 操作时间
		banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();
		// 钱箱ID
		banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtBanknoteBoxID = m_DataModel.box_data.raw_id;
		banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.iTotalAmount = (m_DataModel.box_data.face10*10
			+ m_DataModel.box_data.face20*20
			+ m_DataModel.box_data.face50*50
			+ m_DataModel.box_data.face100*100
			+ m_DataModel.box_data.face5 * 5 
			+ m_DataModel.box_data.face1 * 1) * 100;
		// 操作员ID
		banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
		// 插库
		DATA_HELPER->DbInsertBanknoteBoxRemoveOperationRecord(banknoteBoxRemoveOperationRecord);
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_UNINSTALL_INSERT_DB_FAILED));
	}
	// 设置纸币回收箱钱卸下状态
	theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	
	// 凭条打印失败
	if(m_DataModel.bIsPrintFailed){
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败		
	}
	// 显示卸载完成界面
	DoDialogFlow(WM_CBMGR_OPERATION_TO_FINISH);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化卸载业务数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalSvc::InitDetachBoxData()
{
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	m_DataModel.box_data.raw_id = boxInfo.banknoteBoxID.ToString();
	m_DataModel.box_data.face1  = boxInfo.ulCount_CNY1;
	m_DataModel.box_data.face5  = boxInfo.ulCount_CNY5;
	m_DataModel.box_data.face10 = boxInfo.ulCount_CNY10;
	m_DataModel.box_data.face20 = boxInfo.ulCount_CNY20;
	m_DataModel.box_data.face50 = boxInfo.ulCount_CNY50;
	m_DataModel.box_data.face100 = boxInfo.ulCount_CNY100;
}

//检测纸币箱ID是否合法
bool CBanknoteCollectingBoxRemovalSvc::CheckBoxID(LPBYTE lpBoxID)
{
	//LineCode ，Type， Number构成BankNoteBoxID
	//如果取得的BankNoteBoxID 前四位 != 0192
	//return false;
	//如果取得的BankNoteBoxID 长度 != 8
	//return false;

	return true;
}

//纸币箱卸载凭证
//void CBanknoteCollectingBoxRemovalSvc::LoadBankNoteBoxUninstallReception()
//{
//	try
//	{
//		CStringArray saBillTemplate;
//		CStringArray saTemp;
//		LoadBanknoteBoxData();//读取当前纸币箱中的钱币信息
//		vector<BANKNOTE_BOX_DATA>::iterator iter =  vecBanknote_uninstall.begin();
//		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BANK_NOTE_BOX_UNINSTALL.template"), saBillTemplate);
//		for ( int i = 0; i < saBillTemplate.GetCount();i++)
//		{
//			CString strLine = saBillTemplate.ElementAt(i);
//			strLine.Replace(_T("{纸币箱ID}"), m_DataModel.box_data.raw_id);
//			strLine.Replace(_T("{卸载前金额}"), ComFormatAmount(100*(m_DataModel.box_data.face10*10
//				+ m_DataModel.box_data.face20*20
//				+ m_DataModel.box_data.face50*50
//				+ m_DataModel.box_data.face100*100
//				+ m_DataModel.box_data.face1
//				+ m_DataModel.box_data.face10*10)));
//			if(strLine.Find(_T("{面值}")) >= 0 && iter != vecBanknote_uninstall.end() )
//			{
//				strLine.Replace(_T("{面值}"), CashValueToString(iter->value));
//				strLine.Replace(_T("{数量}"),ComFormatCount(iter->nCount));
//				strLine.Replace(_T("{金额}"),ComFormatAmount(iter->lAmount));
//				iter ++;
//			}
//			strLine.Replace(_T("{操作时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//			strLine.Replace(_T("{操作员编号}"), theAPP_SESSION.GetUserInfo().sUserID);
//			strLine.Replace(_T("{车站名称_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
//			strLine.Replace(_T("{车站名称_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
//			strLine.Replace(_T("{车站名称_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
//			strLine.Replace(_T("{设备ID}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());
//
//			CStringArray saText;
//			ComPrintLineFormatForTemplate(strLine, saText);
//			saTemp.Append(saText);
//		}
//		PRINTER_HELPER->PrintReceiptByBmp(&saTemp,true,true);//打印凭证
//	}
//	catch (CSysException& e)
//	{
//		//throw;
//		theEXCEPTION_MGR.ProcessException(e);
//		m_strShowMsg += _T("打印机异常，打印失败");
//	}
//	catch (...)
//	{
//		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__,_T("凭证打印失败"));
//		theAPP_SESSION.ShowOfficeGuide(_T("卸载成功；凭证打印失败。"));
//	}
//	
//	
//}


//格式化纸币面值
CString CBanknoteCollectingBoxRemovalSvc::CashValueToString(BankNoteAndCoinType_t& value)
{
	CString strValue;
	switch(value)
	{
		/*case Banknote10MOP:strValue = _T("  10MOP");break;
		case Banknote20MOP:strValue = _T("  20MOP");break;
		case Banknote50MOP:strValue = _T("  50MOP");break;
		case Banknote100MOP:strValue = _T(" 100MOP");break;
		case Banknote500MOP:strValue = _T(" 500MOP");break;
		case Banknote1000MOP:strValue = _T("1000MOP");break;*/

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

//纸币箱卸载时，获取纸币箱内信息
void CBanknoteCollectingBoxRemovalSvc::LoadBanknoteBoxData()
{
	/*vecBanknote_uninstall.clear();

	BANKNOTE_BOX_DATA notedata;
	notedata.value = Banknote1Yuan;
	notedata.nCount = m_DataModel.banknote_box.face1;
	notedata.lAmount =  notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote5Yuan;
	notedata.nCount = m_DataModel.banknote_box.face5;
	notedata.lAmount = 5 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote10MOP;
	notedata.nCount = m_DataModel.banknote_box.face10;
	notedata.lAmount = 10 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote20MOP;
	notedata.nCount = m_DataModel.banknote_box.face20;
	notedata.lAmount = 20 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote50MOP;
	notedata.nCount = m_DataModel.banknote_box.face50;
	notedata.lAmount = 50 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote100MOP;
	notedata.nCount = m_DataModel.banknote_box.face100;
	notedata.lAmount = 100 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);


	notedata.value = Banknote500MOP;
	notedata.nCount = m_DataModel.banknote_box.face500;
	notedata.lAmount = 500 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);

	notedata.value = Banknote1000MOP;
	notedata.nCount = m_DataModel.banknote_box.face1000;
	notedata.lAmount = 1000 * notedata.nCount * 100;
	vecBanknote_uninstall.push_back(notedata);*/
}


//判断纸币箱是否连接
bool CBanknoteCollectingBoxRemovalSvc::IsBoxConnected()
{
	if ( BANKNOTE_HELPER != NULL && BANKNOTE_HELPER->IsCashBoxAttached())
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取显示错误文言

@param      无

@retval     CString strMsg

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CBanknoteCollectingBoxRemovalSvc::GetShowErrorMsg(){
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
void CBanknoteCollectingBoxRemovalSvc::OnStart()
{
	__super::OnStart();
	m_DataModel = _tagModel();
	InitializeDataModel();

	//InitDetachBoxData();
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OPERATION);
	m_pDialogFlow->ShowFirstDialogGroup();
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化数据模型

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxRemovalSvc::InitializeDataModel()
{
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO info = theBANKNOTE_COUNT.GetBanknoteboxInfo();	// 取软计数数据
	if (info.banknoteBoxID.ToString() == _T("FFFFFFFF")){
		m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_DETACHED);
		m_DataModel.clrState = RED;
	}else{
		m_DataModel.strState = _opl(TXT_BANKNOTEMGR_BOX_ATTACHED);
		m_DataModel.clrState = GREEN;
	}
	m_DataModel.box_data.raw_id = info.banknoteBoxID.ToString();
	m_DataModel.box_data.now_id = info.banknoteBoxID.ToString();
	m_DataModel.box_data.SetBanknoteCount(info);		// 读取软计数中各个币种的数量

	m_Ex_Col = theBANKNOTE_COUNT.GetBanknoteboxInfo();
}

void CBanknoteCollectingBoxRemovalSvc::LoadBankNoteBoxUninstallReception()
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
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_UNINSTALL);

		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.bIsPrintFailed = true;
		//throw;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.bIsPrintFailed = true;
	}
}