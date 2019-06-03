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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxManageSvc::CBanknoteBoxManageSvc()
	:CTVMForeService(BANKNOTE_BOX_MANAGE_SVC)
{
    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CBanknoteBoxManageBaseDlg(this));

    // 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F3_ESC));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CBanknoteBoxManageBaseDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION, CBanknoteBoxManageBaseDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, CBanknoteBoxManageBaseDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, CBanknoteBoxManageBaseDlg::IDD);

	// 设置画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_CBMGR_BASE_TO_OPERATION, DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_FINISH, DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION, WM_CBMGR_OPERATION_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_FINISH, WM_CBMGR_FINISH_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ERROR, WM_CBMGR_ERROR_TO_BASE, DIALOG_GROUP_BASE);

    // 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteBoxManageSvc::~CBanknoteBoxManageSvc()
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
CBanknoteBoxManageSvc::Model& CBanknoteBoxManageSvc::GetDataModel()
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
void CBanknoteBoxManageSvc::NotifyDataModelChanged()
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
@brief      处理 [F2/Alt] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteBoxManageSvc::OnKeyboardF2(WPARAM , LPARAM ){
	// 只有在操作界面才响应F2
	if(GetCurDialogGroupID() != DIALOG_GROUP_OPERATION){
		return FALSE;
	}
	try{
		// 纸币箱安装操作
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX && m_DataModel.bIsSelectInstall){
			if ( IsBoxConnected()){
				DoHandleAttachBox();
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_INSTALL_FORBBIDEN)/*T("与纸币箱的连接异常，当前不支持安装操作。")*/);
			}
		}
		// 纸币箱卸载操作
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX && m_DataModel.bIsSelectUninstall){
			DoHandleDetachBox();		
		}
	}//异常处理
	catch (CSysException& e) {
		// 写操作日志
		CString strGuide = _T("");
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
			if (m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("安装成功");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		}
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
			if (!m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("卸载成功");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
		}
		theEXCEPTION_MGR.ProcessException(e);
		CString str;
		//if(e.GetModuleID() == CTVMPrinterException::MODULE_ID )
		//{
		//	str.Format(_T("%s；凭条打印失败。."),strGuide);
		//	theAPP_SESSION.ShowOfficeGuide(str);
		//}else//
		//{
		//	str.Format(_T("%s；数据库写入失败。."),strGuide);
		//	theAPP_SESSION.ShowOfficeGuide(_T("数据库写入失败"));
		//}
	}
	catch (...) {
		// 写操作日志
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
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteBoxManageSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	try{
		// 基本画面（菜单选择）
		if (GetCurDialogGroupID() == DIALOG_GROUP_BASE){
			m_DataModel.bIsSelectInstall = false;
			m_DataModel.bIsSelectUninstall = false;
			DoHandleBase();
		}
		// 操作画面（安装、卸载）
		else if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION){
			int index = theAPP_SESSION.GetInputToInt();
			if(MENU_CODE_1 != index){
				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				return FALSE;
			}
			// 纸币箱安装操作
			if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
				// 显示安装纸币箱为选中状态
				if(!m_DataModel.banknote_box.raw_attached){
					m_DataModel.bIsSelectInstall = true;
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				}
				else{// 提示已经安装，不需要再安装
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_INSTALL));
				}				
			}
			// 纸币箱卸载操作
			else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
				if(m_DataModel.banknote_box.raw_attached){
					m_DataModel.bIsSelectUninstall = true;
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				}
				else{// 提示已经卸载，不能重复卸载
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_ALREADY_UNINSTALL));
				}
			}
			NotifyDataModelChanged();
		}
	}
	//异常处理
	catch (CSysException& e) {
		// 写操作日志
		CString strGuide = _T("");
		if (m_DataModel.operation_type == OPERATION_ATTACH_BOX){
			if (m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("安装成功");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
		}
		else if (m_DataModel.operation_type == OPERATION_DETACH_BOX){
			if (!m_DataModel.banknote_box.now_attached)
			{
				strGuide = _T("卸载成功");
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,false);
		}
		theEXCEPTION_MGR.ProcessException(e);
		CString str;
		//if(e.GetModuleID() == CTVMPrinterException::MODULE_ID )
		//{
		//	str.Format(_T("%s；凭条打印失败。."),strGuide);
		//	 theAPP_SESSION.ShowOfficeGuide(str);
		//}else//
		//{
		//	str.Format(_T("%s；数据库写入失败。."),strGuide);
		//	theAPP_SESSION.ShowOfficeGuide(_T("数据库写入失败"));
		//}
	}
	catch (...) {
		// 写操作日志
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
@brief      菜单选项处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::DoHandleBase()
{
	// 获取输入内容
	int index = theAPP_SESSION.GetInputToInt();
	m_strShowMsg = _T("");
	// 菜单选项
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
		// 菜单选项非法提示
		m_DataModel.operation_type = OPERATION_UNKNOWN;
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return;
	}

	// 画面流转
	DoDialogFlow(WM_CBMGR_BASE_TO_OPERATION);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      安装处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::DoHandleAttachBox()
{
	//// 获取输入内容
	///*int index = theAPP_SESSION.GetInputToInt();*/
	//// 清空显示文言
	//m_strShowMsg = _T("");
	//// 菜单选项
	///*switch (index)
	//{
	//case MENU_CODE_1:*/
	//	// 如果先前无纸币箱
	//	if (m_DataModel.banknote_box.now_attached == false)
	//	{
	//		BH_BOX_ID box_id;
	//		long er_code = BANKNOTE_HELPER->GetBoxID(box_id);
	//		// 如果纸币箱未连接
	//		if ( !BANKNOTE_HELPER->IsBankNoteBoxAttached())
	//		{
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_FAILED_CONNECT));
	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);//写日志
	//			return;
	//		}
	//		// 如果纸币箱ID获取成功
	//		if( E_NO_ERROR !=  BANKNOTE_HELPER->GetBoxID(box_id)) 
	//		{
	//			//?? 对GetBoxID纸币箱安装不成功（硬件获取ID失败）的详细错误代码进行处理？
	//			theAPP_SESSION.ShowOfficeGuide(TXT_BANKNOTEMGR_FAILE_ATTACH);
	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);//写日志
	//			return;
	//		}
	//		// 安装纸币箱：获得纸币箱信息 BANKNOTE_BOX_INFO ，存入本地文件BankNotCountInfo
	//		// 创建空白BanknoteCountInfo，并写入新安装的纸币箱ID
	//		CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = CTBanknoteCountInfo::_baknote_box_info();
	//		boxInfo.banknoteBoxID.Deserialize(box_id.user_id);
	//		
	//		// 把纸币箱ID写入m_DataModel供显示用}
	//		m_DataModel.banknote_box.now_id = boxInfo.banknoteBoxID.ToString();

	//		// 写入文件
	//		if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
	//			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
	//		}
	//		m_DataModel.banknote_box.now_attached = true;

	//		// 纸币箱安装成功
	//		m_strShowMsg = _opl(TXT_BANKNOTEMGR_INSTALL_COMPLETED)/*T("纸币箱安装成功。")*/;
	//		LoadBankNoteBoxInstallReception();//打印纸币箱安装的凭证

	//		// 将纸币箱安装操作存入数据库
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
	//			m_strShowMsg += _T("，插入数据库失败。");
	//		}

	//		// 发送纸币箱安装的电文
	//		CASHBOX_CHANGE change;
	//		
	//		change.SetBoxID(boxInfo.banknoteBoxID.bLineCode, boxInfo.banknoteBoxID.bType,boxInfo.banknoteBoxID.wNum); //箱子ID			
	//		change.handleType       = INSTALL;
	//		change.position         = ONE;
	//		change.State            = VALID_ATTACH;
	//		change.BanNoteBox_Initial();//空箱
	//		change.btCount = change.GetSumCurrency();//取得币种总数
	//
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);//发电文
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,true);
	//		
	//		// 显示文言
	//		theAPP_SESSION.ShowOfficeGuide(m_strShowMsg);
	//		DoDialogFlow(WM_CBMGR_OPERATION_TO_FINISH); // 画面流转
	//	}
	//	// 如果先前有未卸载的纸币箱子
	//	else {
	//		theAPP_SESSION.ShowOfficeGuide(TXT_BANKNOTEMGR_FAILE_PREDETACH);
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_INSTALLATION,false);
	//	}
	//	break;
	//default:
	//	// 菜单选项非法提示
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//	return;
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      卸载处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::DoHandleDetachBox()
{
	//// 获取输入内容
	//int index = theAPP_SESSION.GetInputToInt();
	m_strShowMsg = _T("");
	// 菜单选项
	/*switch (index)
	{
	case MENU_CODE_1:*/
		// 如果先前有纸币箱
		if (m_DataModel.banknote_box.now_attached == true) {
			try{

			// 将本地文件BankNoteCountInfo 文件中纸币箱的数据BANKNOTE_BOX_INFO 清除
			// 创建空白的BanknoteCountInfo struct
			
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO& boxInfo = CTBanknoteCountInfo::_banknote_box_info_();
			boxInfo.banknoteBoxID = CTBanknoteCountInfo::_banknote_box_id();
			m_DataModel.banknote_box.now_id = boxInfo.banknoteBoxID.ToString();
			
			// 写入文件
			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
			}

			// 无需检查（硬件）纸币箱是否已经被成功卸载
			m_DataModel.banknote_box.now_attached = false;
			m_strShowMsg = _opl(TXT_BANKNOTEMGT_REMOVAL_COMPLETED)/*T("卸载成功。")*/;
			// 纸币箱卸载凭证
			LoadBankNoteBoxUninstallReception();
			

			BanknoteBoxCashDetail detail;
			BanknoteBoxRemoveOperationRecord banknoteBoxRemoveOperationRecord;
			banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.clear();
			//发送纸币箱卸载电文
			CASHBOX_CHANGE change;
			change.SetBoxID(boxInfo.banknoteBoxID.bLineCode, boxInfo.banknoteBoxID.bType, boxInfo.banknoteBoxID.bNumInQu,boxInfo.banknoteBoxID.bNum); //纸币箱ID
			change.handleType = UNINSTALL; //操作类型
			change.position = BANKNOTE_COLL_POS; //位置
			change.State = VALID_DETACH;//状态
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

			change.vecCash = vecCash; //币种代码+钱币数量
			change.btCount = change.GetSumCurrency(); //币种总数

			// 上传卸载事件数据
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,true);//写日志
			// 上传钱箱清空数据
			theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendClearBankNotesBoxReport(vecCash);
			//写数据库
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
				m_strShowMsg += _T("插入数据库失败。");
				//throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__, _T("数据库插入数据失败"));
			}
			
			theAPP_SESSION.ShowOfficeGuide(m_strShowMsg);
			DoDialogFlow(WM_CBMGR_OPERATION_TO_FINISH); // 画面流转
		}
		// 如果纸币箱先前已被卸载
		else {
			theAPP_SESSION.ShowOfficeGuide(TXT_BANKNOTEMGR_NEED_ATTACH);
		}

	//	break;
	//default:
	//	// 菜单选项非法提示
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//	return;
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化安装业务数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteBoxManageSvc::InitAttachBoxData()
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化卸载业务数据

@param      无

@retval     无

@exception  无
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

	// 如果BanknoteCountInfo记录ID非00000000，则假设有纸币箱。无需硬件检查。
	if (!(boxInfo.banknoteBoxID == CTBanknoteCountInfo::_banknote_box_id())) {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = true;
	}
	else {
		m_DataModel.banknote_box.raw_attached = m_DataModel.banknote_box.now_attached = false;
	}
}

//检测纸币箱ID是否合法
bool CBanknoteBoxManageSvc::CheckBoxID(LPBYTE lpBoxID)
{
	//LineCode ，Type， Number构成BankNoteBoxID
	//如果取得的BankNoteBoxID 前四位 != 0192
	//return false;
	//如果取得的BankNoteBoxID 长度 != 8
	//return false;

	return true;
}

//纸币箱卸载凭证
void CBanknoteBoxManageSvc::LoadBankNoteBoxUninstallReception()
{
	try
	{
		CStringArray saBillTemplate;
		CStringArray saTemp;
		LoadBanknoteBoxData();//读取当前纸币箱中的钱币信息
		vector<BANKNOTE_BOX_DATA>::iterator iter =  vecBanknote_uninstall.begin();
		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BANK_NOTE_BOX_UNINSTALL.template"), saBillTemplate);
		for ( int i = 0; i < saBillTemplate.GetCount();i++)
		{
			CString strLine = saBillTemplate.ElementAt(i);
			strLine.Replace(_T("{纸币箱ID}"), m_DataModel.banknote_box.raw_id);
			strLine.Replace(_T("{卸载前金额}"), ComFormatAmount(100*(m_DataModel.banknote_box.face10*10
				+ m_DataModel.banknote_box.face20*20
				+ m_DataModel.banknote_box.face50*50
				+ m_DataModel.banknote_box.face100*100
				/*+ m_DataModel.banknote_box.face500*500
				+ m_DataModel.banknote_box.face1000*1000*/)));
			if(strLine.Find(_T("{面值}")) >= 0 && iter != vecBanknote_uninstall.end() )
			{
				strLine.Replace(_T("{面值}"), CashValueToString(iter->value));
				strLine.Replace(_T("{数量}"),ComFormatCount(iter->nCount));
				strLine.Replace(_T("{金额}"),ComFormatAmount(iter->lAmount));
				iter ++;
			}
			strLine.Replace(_T("{操作时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			strLine.Replace(_T("{操作员编号}"), theAPP_SESSION.GetUserInfo().sUserID);
			strLine.Replace(_T("{车站名称_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			strLine.Replace(_T("{车站名称_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			strLine.Replace(_T("{车站名称_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			strLine.Replace(_T("{设备ID}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			CStringArray saText;
			ComPrintLineFormatForTemplate(strLine, saText);
			saTemp.Append(saText);
		}
		PRINTER_HELPER->PrintReceiptByBmp(&saTemp,true,true);//打印凭证
	}
	catch (CSysException& e)
	{
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
		m_strShowMsg += _T("打印机异常，打印失败");
	}
	catch (...)
	{
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__,_T("凭证打印失败"));
		theAPP_SESSION.ShowOfficeGuide(_T("卸载成功；凭证打印失败。"));
	}
	
	
}


//纸币箱安装凭证
void CBanknoteBoxManageSvc::LoadBankNoteBoxInstallReception()
{
	try{
		CStringArray saBillTemplate;
		CStringArray saTemp;
		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BANK_NOTE_BOX_INSTALL.template"), saBillTemplate);
		for ( int i = 0; i < saBillTemplate.GetCount();i++)
		{
			CString strLine = saBillTemplate.ElementAt(i);
			strLine.Replace(_T("{纸币箱ID}"), m_DataModel.banknote_box.now_id);
			strLine.Replace(_T("{卸载前金额}"), ComFormatAmount(100*(m_DataModel.banknote_box.face10*10
				+ m_DataModel.banknote_box.face20*20
				+ m_DataModel.banknote_box.face50*50
				+ m_DataModel.banknote_box.face100*100
				/*+ m_DataModel.banknote_box.face500*500
				+ m_DataModel.banknote_box.face1000*1000*/)));
			CString strCurTime = ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
			theAPP_SESSION.GetUserInfo().sUserID;
			strLine.Replace(_T("{操作时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			strLine.Replace(_T("{操作员编号}"), theAPP_SESSION.GetUserInfo().sUserID);
			strLine.Replace(_T("{车站名称_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			strLine.Replace(_T("{车站名称_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			strLine.Replace(_T("{车站名称_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			strLine.Replace(_T("{设备ID}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			CStringArray saText;
			ComPrintLineFormatForTemplate(strLine, saText);
			saTemp.Append(saText);

		}
		PRINTER_HELPER->PrintReceiptByBmp(&saTemp,true,true);//打印凭证
	}
	catch (CSysException&e)
	{
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
		m_strShowMsg += _T("打印机异常，打印失败");
	}
	catch (...)
	{
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		theAPP_SESSION.ShowOfficeGuide(_T("安装成功；凭证打印失败。"));
	}
}

//格式化纸币面值
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

//纸币箱卸载时，获取纸币箱内信息
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
	{//纸币箱是安装着的，但软计数状态是卸载状态
		//非法安装
		return VIOLATE_INSTALL;
	}
	if( !BANKNOTE_HELPER->IsCashBoxAttached() && m_DataModel.banknote_box.now_attached == true)
	{//纸币箱未安装，但软计数上已安装
		//非法卸载
		return VIOLATE_UNINSTALL;
	}
	return NORMAL_OPERATION;
}

//判断纸币箱是否连接
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
@brief      取显示错误文言

@param      无

@retval     CString strMsg

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CBanknoteBoxManageSvc::GetShowErrorMsg(){
	return m_strShowMsg;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Service启动前

@param      无

@retval     无

@exception  无
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