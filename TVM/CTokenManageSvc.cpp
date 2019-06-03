//#include "stdafx.h"
//#include "CTokenManageSvc.h"
//#include "CTokenBoxInfoDlg.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
//// 画面扭转消息
//static const TCHAR* const WM_TOKEN_MSG_MAIN_TO_INSTALL			= _T("WM_TOKEN_MSG_MAIN_TO_INSTALL");
//static const TCHAR* const WM_TOKEN_MSG_MAIN_TO_UNINSTALL		= _T("WM_TOKEN_MSG_MAIN_TO_UNINSTALL");
//static const TCHAR* const WM_TOKEN_MSG_MAIN_TO_RECYCLE			= _T("WM_TOKEN_MSG_MAIN_TO_RECYCLE");
//
//static const TCHAR* const WM_TOKEN_MSG_INSTALL_TO_INSTALLED		= _T("WM_TOKEN_MSG_INSTALL_TO_INSTALLED");
//static const TCHAR* const WM_TOKEN_MSG_UNINSTALL_TO_UNINSTALLED	= _T("WM_TOKEN_MSG_UNINSTALL_TO_UNINSTALLED");
//static const TCHAR* const WM_TOKEN_MSG_RECYCLE_TO_RECYCLED		= _T("WM_TOKEN_MSG_RECYCLE_TO_RECYCLED");
//
//static const TCHAR* const WM_TOKEN_MSG_INSTALL_TO_MAIN			= _T("WM_TOKEN_MSG_INSTALL_TO_MAIN");
//static const TCHAR* const WM_TOKEN_MSG_UNINSTALL_TO_MAIN		= _T("WM_TOKEN_MSG_UNINSTALL_TO_MAIN");
//static const TCHAR* const WM_TOKEN_MSG_RECYCLE_TO_MAIN			= _T("WM_TOKEN_MSG_RECYCLE_TO_MAIN");
//
//static const TCHAR* const WM_TOKEN_MSG_INSTALLED_TO_INSTALL		= _T("WM_TOKEN_MSG_INSTALLED_TO_INSTALL");
//static const TCHAR* const WM_TOKEN_MSG_UNINSTALLED_TO_UNINSTALL	= _T("WM_TOKEN_MSG_UNINSTALLED_TO_UNINSTALL");
//static const TCHAR* const WM_TOKEN_MSG_RECYCLED_TO_RECYCLE		= _T("WM_TOKEN_MSG_RECYCLED_TO_RECYCLE");
//
//const int TOKEN_CLEAR	= 2;
//
//
//// 消息映射
//BEGIN_MESSAGE_MAP(CTokenManageSvc,CTVMForeService)
//END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////
///*
//@brief      构造函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CTokenManageSvc::CTokenManageSvc():CTVMForeService(TOKEN_BOX_MANAGE_SVC)
//{
//	//设置操作员画面
//	m_pDialogFlow->AddDialog(CTokenBoxInfoDlg::IDD,new CTokenBoxInfoDlg(this));
//
//	//设置画面组
// 	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MAIN,_opl(GUIDE_ENTER_F3_ESC));
// 	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MAIN,CTokenBoxInfoDlg::IDD);
// 
//	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INSTALL,_opl(GUIDE_ENTER_F3_ESC));
//	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INSTALL,CTokenBoxInfoDlg::IDD);
//
//	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INSTALLED,_opl(GUIDE_ENTER_F3_ESC));
//	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INSTALLED,CTokenBoxInfoDlg::IDD);
//
//	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_UNINSTALL,_opl(GUIDE_ENTER_F3_ESC));
//	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_UNINSTALL,CTokenBoxInfoDlg::IDD);
//
//	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_UNINSTALLED,_opl(GUIDE_ENTER_F3_ESC));
//	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_UNINSTALLED,CTokenBoxInfoDlg::IDD);
//
//	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_RECYCLE,_opl(GUIDE_ENTER_F3_ESC));
//	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_RECYCLE,CTokenBoxInfoDlg::IDD);
//
//	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_RECYCLED,_opl(GUIDE_ENTER_F3_ESC));
//	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_RECYCLED,CTokenBoxInfoDlg::IDD);
//	
//	//设置画面扭转(菜单到二级菜单)
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_MAIN,WM_TOKEN_MSG_MAIN_TO_INSTALL,DIALOG_GROUP_INSTALL);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_MAIN,WM_TOKEN_MSG_MAIN_TO_UNINSTALL,DIALOG_GROUP_UNINSTALL);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_MAIN,WM_TOKEN_MSG_MAIN_TO_RECYCLE,DIALOG_GROUP_RECYCLE);
//
//	// 二级菜单到结束
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_INSTALL,WM_TOKEN_MSG_INSTALL_TO_INSTALLED,DIALOG_GROUP_INSTALLED);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_UNINSTALL,WM_TOKEN_MSG_UNINSTALL_TO_UNINSTALLED,DIALOG_GROUP_UNINSTALLED);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_RECYCLE,WM_TOKEN_MSG_RECYCLE_TO_RECYCLED,DIALOG_GROUP_RECYCLED);
//
//	// 二级菜单到主菜单
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_INSTALL,WM_TOKEN_MSG_INSTALL_TO_MAIN,DIALOG_GROUP_MAIN);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_UNINSTALL,WM_TOKEN_MSG_UNINSTALL_TO_MAIN,DIALOG_GROUP_MAIN);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_RECYCLE,WM_TOKEN_MSG_RECYCLE_TO_MAIN,DIALOG_GROUP_MAIN);
//
//	// 结束界面到二级菜单
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_INSTALLED,WM_TOKEN_MSG_INSTALLED_TO_INSTALL,DIALOG_GROUP_INSTALL);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_UNINSTALLED,WM_TOKEN_MSG_UNINSTALLED_TO_UNINSTALL,DIALOG_GROUP_UNINSTALL);
//	m_pDialogFlow->AddFlow(DIALOG_GROUP_RECYCLED,WM_TOKEN_MSG_RECYCLED_TO_RECYCLE,DIALOG_GROUP_RECYCLE);
//
//
//	//设置初始画面
//	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MAIN);
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      析构函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CTokenManageSvc::~CTokenManageSvc(){
//
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      业务开始时
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::OnStart(){
//	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MAIN);
//	// 初始化数据
//	m_DataModel = _data_model_();
//	// 可以先判断下TOKEN模块是否有异常
//	/*try{
//		TOKEN_RSP_READ_STATUS status;
//		TOKEN_HELPER->ReadStatus(status);
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENMODULEEXCEPTION));
//		m_DataModel.bIsException = true;
//	}
//	catch(...){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENMODULEEXCEPTION));
//		m_DataModel.bIsException = true;
//	}*/
//	
//	m_bIsAutoInputRFID = theFunction_INFO.IsAutoReadRfidWithoutManual();
//	//theAPP_SESSION.SetKeyboard(KEY_RETURN | KEY_INPUT);
//	__super::OnStart();
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      界面刷新
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::NotifyDataModelChanged(){
//	__super::NotifyDataModelChanged();
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      Enter按键消息处理
//
//@param      (i)WPARAM wParam;未使用
//@param      (i)LPARAM lParam;未使用
//
//@retval     BOOL TRUE:处理成功;FALSE:处理失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//LRESULT CTokenManageSvc::OnKeyboardEnter(WPARAM wParam,LPARAM lParam){
//	// 判断当前TOKEN模块是否连接
//	/*if(!theAPP_SESSION.IsTOKENModuleConnected()){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_DISCONNECT));
//		return FALSE;
//	}*/
//	
//	int index = theAPP_SESSION.GetInputToInt();
//	// 1、主菜单
//	if(GetCurDialogGroupID() == DIALOG_GROUP_MAIN){
//		InitDataModel();
//		DoMainOperator();
//	}
//	// 2. 二级菜单
//	else if(GetCurDialogGroupID() == DIALOG_GROUP_INSTALL){		// 安装
//		if(m_DataModel.operationType == OPERATION_TYPE::OPERATION_TYPE_NONE)
//			DoInstallOperator();
//		else
//			DoInstallInputOperator();
//	}
//	else if(GetCurDialogGroupID() == DIALOG_GROUP_UNINSTALL){	// 卸载
//		DoUninstallOperator();
//	}
//	else if(GetCurDialogGroupID() == DIALOG_GROUP_RECYCLE){		// 回收
//		// Do not input anything
//		m_DataModel.AfterTokenBoxA.bIsSelected  = false;
//		m_DataModel.AfterTokenBoxB.bIsSelected  = false;
//		int index = theAPP_SESSION.GetInputToInt();
//		switch(index){
//			case MENU_CODE_1:
//				m_DataModel.AfterTokenBoxA.bIsSelected  = true;
//				break;
//			case MENU_CODE_2:
//				m_DataModel.AfterTokenBoxB.bIsSelected  = true;
//				break;
//			default:
//				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));	// 菜单输入非
//				return TRUE;
//		}
//		
//		CTokenBoxInfoDlg* pCurDlg = dynamic_cast<CTokenBoxInfoDlg*>(GetCurOperationDlg());
//		pCurDlg->UpdateUI();
//	}
//	// 3. 完成界面
//	else{
//		// Do nothing
//	}
//	return TRUE;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      初始化所有数据
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::InitDataModel(){
//	//Reset Initdata
//	m_DataModel = _data_model_();
//	// Token箱状态
//	m_DataModel.BeforeTokenBoxA.strBoxID	= theTOKEN_COUNT.GetTokenboxAInfo().tokenBoxID.ToString();
//	m_DataModel.BeforeTokenBoxA.nQuantity	= theTOKEN_COUNT.GetTokenboxAInfo().ulCurCount;
//	m_DataModel.BeforeTokenBoxB.strBoxID	= theTOKEN_COUNT.GetTokenboxBInfo().tokenBoxID.ToString();
//	m_DataModel.BeforeTokenBoxB.nQuantity	= theTOKEN_COUNT.GetTokenboxBInfo().ulCurCount;
//
//	// Hopper 状态
//	m_DataModel.BeforeTokenHopperA.strBoxID	= theTOKEN_COUNT.GetHopperAInfo().tokenBoxID.ToString();
//	m_DataModel.BeforeTokenHopperA.nQuantity= theTOKEN_COUNT.GetHopperAInfo().ulCurCount;
//	m_DataModel.BeforeTokenHopperB.strBoxID = theTOKEN_COUNT.GetHopperBInfo().tokenBoxID.ToString();
//	m_DataModel.BeforeTokenHopperB.nQuantity= theTOKEN_COUNT.GetHopperBInfo().ulCurCount;
//
//	// 废票箱状态
//	m_DataModel.BeforeInvalidTokenBox.strBoxID = theTOKEN_COUNT.GetTokenboxWastedInfo().tokenBoxID.ToString();
//	m_DataModel.BeforeInvalidTokenBox.nQuantity= theTOKEN_COUNT.GetTokenboxWastedInfo().ulCurCount;
//
//	// 回收箱状态
//	m_DataModel.BeforeCollectedTokenBox.strBoxID = theTOKEN_COUNT.GetTokenboxCollectedInfo().tokenBoxID.ToString();
//	m_DataModel.BeforeCollectedTokenBox.nQuantity= theTOKEN_COUNT.GetTokenboxCollectedInfo().ulCurCount;
//
//	// 状态为：已安装或已卸下
//	m_DataModel.BeforeTokenBoxA.emStatus		= m_DataModel.BeforeTokenBoxA.strBoxID == INVALID_ID ? STATUS_UNINSTALL : STATUS_INSTALLED;
//	m_DataModel.BeforeTokenBoxB.emStatus		= m_DataModel.BeforeTokenBoxB.strBoxID == INVALID_ID ? STATUS_UNINSTALL : STATUS_INSTALLED;
//	m_DataModel.BeforeInvalidTokenBox.emStatus	= m_DataModel.BeforeInvalidTokenBox.strBoxID == INVALID_ID ? STATUS_UNINSTALL : STATUS_INSTALLED;
//	m_DataModel.BeforeCollectedTokenBox.emStatus	= m_DataModel.BeforeCollectedTokenBox.strBoxID == INVALID_ID ? STATUS_UNINSTALL : STATUS_INSTALLED;
//	m_DataModel.BeforeTokenHopperA.emStatus		= m_DataModel.BeforeTokenHopperA.strBoxID == INVALID_ID ? STATUS_UNINSTALL : STATUS_INSTALLED;
//	m_DataModel.BeforeTokenHopperB.emStatus		= m_DataModel.BeforeTokenHopperB.strBoxID == INVALID_ID ? STATUS_UNINSTALL : STATUS_INSTALLED;
//
//	// 操作员及操作时间
//	m_DataModel.BeforeTokenBoxA.ulOperatorID				= theTOKEN_COUNT.GetTokenboxAInfo().ulOperatorID;
//	m_DataModel.BeforeTokenBoxB.ulOperatorID				= theTOKEN_COUNT.GetTokenboxBInfo().ulOperatorID;
//	m_DataModel.BeforeInvalidTokenBox.ulOperatorID		= theTOKEN_COUNT.GetTokenboxWastedInfo().ulOperatorID;
//	m_DataModel.BeforeCollectedTokenBox.ulOperatorID		= theTOKEN_COUNT.GetTokenboxCollectedInfo().ulOperatorID;
//	m_DataModel.BeforeTokenBoxA.operateTime			= theTOKEN_COUNT.GetTokenboxAInfo().operateTime;
//	m_DataModel.BeforeTokenBoxB.operateTime			= theTOKEN_COUNT.GetTokenboxBInfo().operateTime;
//	m_DataModel.BeforeInvalidTokenBox.operateTime		= theTOKEN_COUNT.GetTokenboxWastedInfo().operateTime;
//	m_DataModel.BeforeCollectedTokenBox.operateTime	= theTOKEN_COUNT.GetTokenboxCollectedInfo().operateTime;
//
//	m_DataModel.AfterTokenBoxA				= m_DataModel.BeforeTokenBoxA;
//	m_DataModel.AfterTokenBoxB				= m_DataModel.BeforeTokenBoxB;
//	m_DataModel.AfterTokenHopperA			= m_DataModel.BeforeTokenHopperA;
//	m_DataModel.AfterTokenHopperB			= m_DataModel.BeforeTokenHopperB;
//	m_DataModel.AfterInvalidTokenBox		= m_DataModel.BeforeInvalidTokenBox;
//	m_DataModel.AfterCollectedTokenBox		= m_DataModel.BeforeCollectedTokenBox;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      F2键消息处理
//
//@param      (i)WPARAM wParam;未使用
//@param      (i)LPARAM lParam;未使用
//
//@retval     BOOL TRUE:处理成功;FALSE:处理失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//LRESULT CTokenManageSvc::OnKeyboardF2(WPARAM wParam, LPARAM lParam){
//	// 判断当前TOKEN模块是否连接
//	if(!theAPP_SESSION.IsTOKENModuleConnected()){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_DISCONNECT));
//		return FALSE;
//	}
//
//	if(m_DataModel.bIsException){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENMODULEEXCEPTION));
//		return FALSE;
//	}
//	// 安装界面
//	if(GetCurDialogGroupID() == DIALOG_GROUP_INSTALL){
//		bool bInstallTokenBoxA , bInstallTokenBoxB,bInstallInvalidTokenBox,bInstallCollectedBox;	
//		bInstallTokenBoxA = bInstallTokenBoxB = bInstallInvalidTokenBox = bInstallCollectedBox = false;
//		// 是否安装A箱
//		if(m_DataModel.AfterTokenBoxA.bIsSelected/* && m_DataModel.AfterTokenBoxA.nQuantity != 0*/)	bInstallTokenBoxA = true; else bInstallTokenBoxA = false;
//		// 是否安装B箱
//		if(m_DataModel.AfterTokenBoxB.bIsSelected /*&& m_DataModel.AfterTokenBoxB.nQuantity != 0*/)	bInstallTokenBoxB = true; else bInstallTokenBoxB = false;
//		// 是否安装废票箱
//		bInstallInvalidTokenBox = m_DataModel.AfterInvalidTokenBox.bIsSelected;
//		// 是否安装回收箱
//		bInstallCollectedBox = m_DataModel.AfterCollectedTokenBox.bIsSelected;
//
//		// 如果有安装选项
//		if( bInstallTokenBoxA || bInstallTokenBoxB || bInstallInvalidTokenBox || bInstallCollectedBox){
//			// 判断是否拉开挡板,是否在位等？
//			if(bInstallTokenBoxA){
//				if(IsTokenBoxAStandby()){
//					if(IsTokenBoxAApronOpen()){
//						// Do follow
//					}
//					else{
//						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDOPENAPRON));
//						return FALSE;
//					}
//				}
//				else{
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENANOTOK));
//					return FALSE;
//				}
//			}
//			if(bInstallTokenBoxB){
//				if(IsTokenBoxBStandby()){
//					if(IsTokenBoxBApronOpen()){
//						// Do follow
//					}
//					else{
//						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDOPENAPRON));
//						return FALSE;
//					}
//				}
//				else{
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBNOTOK));
//					return FALSE;
//				}
//			}
//			if(bInstallInvalidTokenBox){
//				if(!IsInvalidBoxStandby()){
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTEDNOTATTACH));
//					return FALSE;
//				}
//			}
//			if(bInstallCollectedBox){
//				if(!IsCollectedBoxStandby()){
//					theAPP_SESSION.ShowOfficeGuide(_T("没有安装回收箱"));
//					return FALSE;
//				}
//			}
//
//			// 安装A箱
//			if(bInstallTokenBoxA){
//				DoInstallTokenBoxA();
//			}
//			// 安装B箱
//			if(bInstallTokenBoxB){
//				DoInstallTokenBoxB();
//			}
//			// 安装废票箱
//			if(bInstallInvalidTokenBox){
//				DoInstallInvalidTokenBox();
//			}
//			// 安装回收箱
//			if(bInstallCollectedBox){
//				DoInstallCollectedTokenBox();
//			}
//
//			if (bInstallTokenBoxA || bInstallTokenBoxB){
//				UINT nCleanNum = 0;
//				TOKEN_HELPER->Init(&nCleanNum);//如果是票箱AB，需要初始化，使模块自动备票到暂存区，否则第一次售票会失败。）
//			}
//
//			// 安装完成，需要打印凭证
//			m_DataModel.bPrintSuccess = PrintReciept();
//
//			// 转到完成界面
//			DoDialogFlow(WM_TOKEN_MSG_INSTALL_TO_INSTALLED);
//		}
//		else{
//			// 请先选择需要安装的箱子后再保存。
//		}
//	}
//	// 卸载界面
//	else if(GetCurDialogGroupID() == DIALOG_GROUP_UNINSTALL){
//		if(m_DataModel.AfterTokenBoxA.bIsSelected || m_DataModel.AfterTokenBoxB.bIsSelected || m_DataModel.AfterInvalidTokenBox.bIsSelected || m_DataModel.AfterCollectedTokenBox.bIsSelected){
//			// 再次判断是否在位
//			// 20180105, lichao: 为了实际操作方便，卸载时去掉箱子在位的检查
//			//if(m_DataModel.AfterTokenBoxA.bIsSelected ){
//			//	if(!IsTokenBoxAStandby()){
//			//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENANOTATTACH));
//			//		return FALSE;				
//			//	}
//			//}
//			//if(m_DataModel.AfterTokenBoxB.bIsSelected){
//			//	if(!IsTokenBoxBStandby()){
//			//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBNOTATTACH));
//			//		return FALSE;
//			//	}
//			//}
//			//if(m_DataModel.AfterInvalidTokenBox.bIsSelected){
//			//	if(!IsInvalidBoxStandby()){
//			//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTEDNOTATTACH));
//			//		return FALSE;
//			//	}
//			//}
//			//if(m_DataModel.AfterCollectedTokenBox.bIsSelected){
//			//	if(!IsCollectedBoxStandby()){
//			//		theAPP_SESSION.ShowOfficeGuide(_T("没有安装回收箱"));
//			//		return FALSE;
//			//	}
//			//}
//
//			// 卸载Token箱A
//			if(m_DataModel.AfterTokenBoxA.bIsSelected){
//				DoUninstallTokenBoxA();
//			}
//			// 卸载Token箱B
//			if(m_DataModel.AfterTokenBoxB.bIsSelected){
//				DoUninstallTokenBoxB();
//			}
//			// 卸载废票箱
//			if(m_DataModel.AfterInvalidTokenBox.bIsSelected){
//				DoUninstallInvalidTokenBox();
//			}
//			// 卸载废票箱
//			if(m_DataModel.AfterCollectedTokenBox.bIsSelected){
//				DoUninstallCollectedTokenBox();
//			}
//
//			// 卸载完成需要打印
//			m_DataModel.bPrintSuccess = PrintReciept();
//
//			DoDialogFlow(WM_TOKEN_MSG_UNINSTALL_TO_UNINSTALLED);// 转到卸载完成界面
//		}
//		else{
//			// 请先选择需要的卸下箱子后再保存
//		}
//	}
//	// 回收界面
//	else if(GetCurDialogGroupID() == DIALOG_GROUP_RECYCLE){
//		/*if(m_DataModel.AfterTokenHopperA.nQuantity == 0 && m_DataModel.AfterTokenHopperB.nQuantity == 0){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NO_NEED_RECYCLE));
//		return FALSE;
//		}*/
//
//		//TOKEN_READ_WRITE_RFID sTimRfidInfo;
//		//long lRet = TOKEN_HELPER->TokenCyclerRfidRead(&sTimRfidInfo);
//
//		//TOKEN_READ_WRITE_RFID sTimRfidInfo1;
//		//lRet = TOKEN_HELPER->TokenCyclerRfidWrite(&sTimRfidInfo1);
//
//		if (!m_DataModel.AfterTokenBoxA.bIsSelected && !m_DataModel.AfterTokenBoxB.bIsSelected){
//			return TRUE;
//		}
//
//		if (m_DataModel.AfterTokenBoxA.bIsSelected){
//			DoRecycleOperator(BOX_A);
//		}
//		if (m_DataModel.AfterTokenBoxB.bIsSelected){
//			DoRecycleOperator(BOX_B);
//		}		
//
//		m_DataModel.bIsException = PrintReciept();
//		// 转到回收完成界面
//		DoDialogFlow(WM_TOKEN_MSG_RECYCLE_TO_RECYCLED);
//	}
//	return TRUE;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      F3键消息处理
//
//@param      (i)WPARAM wParam;未使用
//@param      (i)LPARAM lParam;未使用
//
//@retval     BOOL TRUE:处理成功;FALSE:处理失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//LRESULT CTokenManageSvc::OnKeyboardF3(WPARAM , LPARAM ){
//	UINT dlg = GetCurDialogGroupID();
//	if(dlg == DIALOG_GROUP_MAIN){
//		theAPP_SESSION.ReturnMaintainMenu();
//	}
//	else if(dlg == DIALOG_GROUP_INSTALL){
//		DoDialogFlow(WM_TOKEN_MSG_INSTALL_TO_MAIN);
//	}
//	else if(dlg == DIALOG_GROUP_UNINSTALL){
//		DoDialogFlow(WM_TOKEN_MSG_UNINSTALL_TO_MAIN);
//	}
//	else if(dlg == DIALOG_GROUP_RECYCLE){
//		DoDialogFlow(WM_TOKEN_MSG_RECYCLE_TO_MAIN);
//	}
//	else if(dlg == DIALOG_GROUP_INSTALLED){
//		DoDialogFlow(WM_TOKEN_MSG_INSTALLED_TO_INSTALL);
//		InitDataModel();
//	}
//	else if(dlg == DIALOG_GROUP_UNINSTALLED){
//		DoDialogFlow(WM_TOKEN_MSG_UNINSTALLED_TO_UNINSTALL);
//		InitDataModel();
//	}
//	else if(dlg == DIALOG_GROUP_RECYCLED){
//		DoDialogFlow(WM_TOKEN_MSG_RECYCLED_TO_RECYCLE);
//		InitDataModel();
//	}
//	return TRUE;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      响应主菜单操作
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoMainOperator(){
//	int index = theAPP_SESSION.GetInputToInt();
//
//	switch(index){
//	case MENU_CODE_1:
//		{
//			DoDialogFlow(WM_TOKEN_MSG_MAIN_TO_INSTALL);		// 转到安装界面
//			//theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_F2);
//		}
//		break;
//	case MENU_CODE_2:
//		{
//			DoDialogFlow(WM_TOKEN_MSG_MAIN_TO_UNINSTALL);	// 转到卸载界面
//			//theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN | KEY_F2);
//		}
//		break;
//	case MENU_CODE_3:
//		{
//			DoDialogFlow(WM_TOKEN_MSG_MAIN_TO_RECYCLE);		// 转到回收界面
//			//theAPP_SESSION.SetKeyboard(KEY_RETURN | KEY_F2);// 后退及F2按键
//		}		
//		break;
//	default:
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));	// 菜单输入非法
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      响应安装操作
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoInstallOperator(){
//	// 取菜单输入项
//	int index = theAPP_SESSION.GetInputToInt();
//	switch(index){
//	case MENU_CODE_1:// Token A ID
//		{
//			if(m_DataModel.BeforeTokenBoxA.emStatus == STATUS_INSTALLED){	// 已安装
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDUNINSTALLTOKENA));
//			}
//			else{
//				// 判断箱子是否真的在位？
//				if(IsTokenBoxAStandby()){
//					// 自动读取
//					if(m_bIsAutoInputRFID){
//						m_DataModel.AfterTokenBoxA.strBoxID = TOKEN_HELPER->ReadTokenBoxARFID();
//						if(m_DataModel.AfterTokenBoxA.strBoxID==_T("")){
//							theAPP_SESSION.ShowOfficeGuide(_T("读取票箱RFID失败"));
//							return;
//						}
//						m_DataModel.AfterTokenBoxA.emStatus	 = STATUS_READY_INSTALL;
//						m_DataModel.AfterTokenBoxA.bIsSelected = true;
//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
//						// 更新界面显示
//						NotifyDataModelChanged();
//					}
//					// 提示输入RFID
//					else{
//						theAPP_SESSION.SetMaxInputLength(LEN_INPUT_RFID);
//						ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_TOKENA_ID));
//						m_DataModel.operationType = OPERATION_TYPE_INPUT_BOXA_ID;
//					}
//				}
//				// 提示TOKEN箱A不在位
//				else{
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENANOTATTACH));
//				}
//			}
//		}
//		break;
//	case MENU_CODE_2:// Token A Quantity
//		{
//			if(m_DataModel.BeforeTokenBoxA.emStatus == STATUS_INSTALLED){	// 已安装
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDUNINSTALLTOKENA));
//			}
//			else{
//				// 判断是否输入ID
//				if(m_DataModel.AfterTokenBoxA.bIsSelected){
//					if(IsTokenBoxAStandby()){
//						theAPP_SESSION.SetMaxInputLength(LEN_INPUT_QUANTITY);
//						ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_TOKENA_QUANTITY));
//						m_DataModel.operationType = OPERATION_TYPE_INPUT_BOXA_QUANTITY;
//					}
//					// 提示TOKEN箱A不在位
//					else{
//						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENANOTATTACH));
//					}
//				}
//				else{
//					// 提示先输入ID
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_PLEASE_INPUT_ID_FIRST));
//				}
//			}
//		}
//		break;
//	case MENU_CODE_3:// Token B ID 
//		{
//			if(m_DataModel.BeforeTokenBoxB.emStatus == STATUS_INSTALLED){	// 已安装
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDUNINSTALLTOKENB));
//			}
//			else{
//				// 判断箱子是否真的在位？
//				if(IsTokenBoxBStandby()){
//					// 自动读取
//					if(m_bIsAutoInputRFID){
//						m_DataModel.AfterTokenBoxB.strBoxID = TOKEN_HELPER->ReadTokenBoxBRFID();
//						if(m_DataModel.AfterTokenBoxB.strBoxID==_T("")){
//							theAPP_SESSION.ShowOfficeGuide(_T("读取票箱RFID失败"));
//							return;
//						}
//						m_DataModel.AfterTokenBoxB.emStatus	 = STATUS_READY_INSTALL;
//						m_DataModel.AfterTokenBoxB.bIsSelected = true;
//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
//						// 更新界面显示
//						NotifyDataModelChanged();
//					}
//					// 提示输入RFID
//					else{
//						theAPP_SESSION.SetMaxInputLength(LEN_INPUT_RFID);
//						ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_TOKENB_ID));
//						m_DataModel.operationType = OPERATION_TYPE_INPUT_BOXB_ID;
//					}
//				}
//				// 提示TOKEN箱B不在位
//				else{
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBNOTATTACH));
//				}
//			}
//		}
//		break;
//	case MENU_CODE_4:// Token B Quantity
//		{
//			if(m_DataModel.BeforeTokenBoxB.emStatus == STATUS_INSTALLED){	// 已安装
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDUNINSTALLTOKENB));
//			}
//			else{
//				if(m_DataModel.AfterTokenBoxB.bIsSelected){
//					if(IsTokenBoxBStandby()){
//						theAPP_SESSION.SetMaxInputLength(LEN_INPUT_QUANTITY);
//						ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_TOKENB_QUANTITY));
//						m_DataModel.operationType = OPERATION_TYPE_INPUT_BOXB_QUANTITY;
//					}
//					else
//						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBNOTATTACH));
//				}
//				else{
//					// 提示先输入ID
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_PLEASE_INPUT_ID_FIRST));
//				}
//			}
//		}
//		break;
//	case MENU_CODE_5:// Token Invalid ID
//		{
//			if(m_DataModel.BeforeInvalidTokenBox.emStatus == STATUS_INSTALLED){	// 已安装
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NEEDUNINSTALLWASTED));
//			}
//			else{
//				if(IsInvalidBoxStandby()){
//					if(m_bIsAutoInputRFID){
//						m_DataModel.AfterInvalidTokenBox.strBoxID = TOKEN_HELPER->ReadInvalidBoxRFID();
//						if(m_DataModel.AfterInvalidTokenBox.strBoxID==_T("")){
//							theAPP_SESSION.ShowOfficeGuide(_T("读取票箱RFID失败"));
//							return;
//						}
//						m_DataModel.AfterInvalidTokenBox.emStatus	 = STATUS_READY_INSTALL;
//						m_DataModel.AfterInvalidTokenBox.bIsSelected = true;
//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
//						// 更新界面显示
//						NotifyDataModelChanged();
//					}
//					else{
//						theAPP_SESSION.SetMaxInputLength(LEN_INPUT_RFID);
//						ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_WASTED_ID));
//						m_DataModel.operationType = OPERATION_TYPE_INPUT_INVALID_BOX_ID;
//					}
//				}
//				else{
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTEDNOTATTACH));
//				}
//			}
//		}
//		break;
//	case MENU_CODE_6:// Token回收箱
//		{
//			if(m_DataModel.BeforeCollectedTokenBox.emStatus == STATUS_INSTALLED){	// 已安装
//				theAPP_SESSION.ShowOfficeGuide(_T("请先卸载已经安装的回收箱"));
//			}
//			else{
//				if(IsCollectedBoxStandby()){
//					if(m_bIsAutoInputRFID){
//						m_DataModel.AfterCollectedTokenBox.strBoxID = TOKEN_HELPER->ReadTokenBoxCollectRFID();
//						m_DataModel.AfterCollectedTokenBox.emStatus	 = STATUS_READY_INSTALL;
//						m_DataModel.AfterCollectedTokenBox.bIsSelected = true;
//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
//						// 更新界面显示
//						NotifyDataModelChanged();
//					}
//					else{
//						theAPP_SESSION.SetMaxInputLength(LEN_INPUT_RFID);
//						ShowPrefixGuideMsg(_T("回收箱的编号"));
//						m_DataModel.operationType = OPERATION_TYPE_INPUT_COLLECTED_BOX_ID;
//					}
//				}
//				else{
//					theAPP_SESSION.ShowOfficeGuide(_T("没有安装回收箱"));
//				}
//			}
//		}
//		break;
//	default:
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));	// 菜单输入非法
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      响应安装输入操作
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoInstallInputOperator(){
//	int nNumberOrRFID = theAPP_SESSION.GetInputToInt();
//	CString strTemp(EMPTY_STR);
//	switch(m_DataModel.operationType){
//	case OPERATION_TYPE_INPUT_BOXA_ID:
//		{
//			if(nNumberOrRFID == 0){
//				ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_TOKENA_ID));
//				return;
//			}
//			else{
//				strTemp.Format(_T("%02d94%04d"),theMAINTENANCE_INFO.GetLineCode(),nNumberOrRFID);// 检测合法性
//				if(strTemp.CompareNoCase(m_DataModel.AfterTokenBoxB.strBoxID) == 0){
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKEN_BOX_ID_CANNOT_THE_SAME_AS_OTHER));///*T("输入ID错误，请确保该ID唯一。")*/
//					return;
//				}
//				else{
//					m_DataModel.AfterTokenBoxA.bIsSelected = true;
//					m_DataModel.AfterTokenBoxA.strBoxID = strTemp;
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENAIDREADED));
//				}
//			}
//		}
//		break;
//	case OPERATION_TYPE_INPUT_BOXA_QUANTITY:
//		{
//			// 需要检测数量合法性
//			// 可以输入的最大值
//			int nHopperMaxAdd = theTVM_INFO.GetTokenHopperAMaxCount() - theTOKEN_COUNT.GetHopperAInfo().ulCurCount;
//			int nMaxAddingTokenACount = theTVM_INFO.GetTokenBoxAMaxCount() + (nHopperMaxAdd > 0 ? 0 : nHopperMaxAdd);
//
//			if(nNumberOrRFID < 0 || nNumberOrRFID > nMaxAddingTokenACount){
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_INVALID_CARD_QUANTITY));
//				return;
//			}
//			else{
//				m_DataModel.AfterTokenBoxA.emStatus	 = STATUS_READY_INSTALL;
//				m_DataModel.AfterTokenBoxA.nQuantity = nNumberOrRFID;
//				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//			}
//		}
//		break;
//	case OPERATION_TYPE_INPUT_BOXB_ID:
//		{
//			if(nNumberOrRFID == 0){
//				ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_TOKENB_ID));
//				return;
//			}
//			else{
//				strTemp.Format(_T("%02d94%04d"),theMAINTENANCE_INFO.GetLineCode(),nNumberOrRFID);// 检测合法性
//				if(strTemp.CompareNoCase(m_DataModel.AfterTokenBoxA.strBoxID) == 0){
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKEN_BOX_ID_CANNOT_THE_SAME_AS_OTHER));///*T("输入ID错误，请确保该ID唯一。")*/
//					return;
//				}
//				else{
//					m_DataModel.AfterTokenBoxB.bIsSelected = true;
//					m_DataModel.AfterTokenBoxB.strBoxID = strTemp;
//					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBIDREADED));
//				}
//			}
//		}
//		break;
//	case OPERATION_TYPE_INPUT_BOXB_QUANTITY:
//		{
//			// 需要检测数量合法性
//			// 可以输入的最大值
//			int nHopperMaxAdd = theTVM_INFO.GetTokenHopperBMaxCount() - theTOKEN_COUNT.GetHopperBInfo().ulCurCount;
//			int nMaxAddingTokenBCount = theTVM_INFO.GetTokenBoxAMaxCount() + (nHopperMaxAdd > 0 ? 0 : nHopperMaxAdd);
//
//			if(nNumberOrRFID < 0 || nNumberOrRFID > nMaxAddingTokenBCount){
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_SMARTCARDMGR_INVALID_CARD_QUANTITY));
//				return;
//			}
//			else{
//				m_DataModel.AfterTokenBoxB.emStatus	 = STATUS_READY_INSTALL;
//				m_DataModel.AfterTokenBoxB.nQuantity = nNumberOrRFID;
//				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//			}
//		}
//		break;
//	case OPERATION_TYPE_INPUT_INVALID_BOX_ID:
//		{
//			if(nNumberOrRFID == 0){
//				ShowPrefixGuideMsg(_opl(TXT_TOKENMGR_WASTED_ID));
//				return;
//			}
//			else{
//				strTemp.Format(_T("%02d95%04d"),theMAINTENANCE_INFO.GetLineCode(),nNumberOrRFID);
//				m_DataModel.AfterInvalidTokenBox.emStatus	 = STATUS_READY_INSTALL;
//				m_DataModel.AfterInvalidTokenBox.bIsSelected = true;
//				m_DataModel.AfterInvalidTokenBox.strBoxID = strTemp;
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTEDIDREADED));
//			}
//		}
//		break;
//	case OPERATION_TYPE_INPUT_COLLECTED_BOX_ID:
//		{
//			if(nNumberOrRFID == 0){
//				ShowPrefixGuideMsg(_T("回收箱的编号"));
//				return;
//			}
//			else{
//				strTemp.Format(_T("%02d95%04d"),theMAINTENANCE_INFO.GetLineCode(),nNumberOrRFID);
//				m_DataModel.AfterCollectedTokenBox.emStatus	 = STATUS_READY_INSTALL;
//				m_DataModel.AfterCollectedTokenBox.bIsSelected = true;
//				m_DataModel.AfterCollectedTokenBox.strBoxID = strTemp;
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTEDIDREADED));
//			}
//		}
//		break;
//	default:
//		break;
//	}
//	// 记录信息后重置安装输入状态
//	m_DataModel.operationType = OPERATION_TYPE::OPERATION_TYPE_NONE;
//	theAPP_SESSION.SetMaxInputLength(1);
//	// 更新界面显示
//	NotifyDataModelChanged();
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      响应卸载操作
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoUninstallOperator(){
//	// 取菜单输入项
//	int index = theAPP_SESSION.GetInputToInt();
//	switch(index){
//	case MENU_CODE_1:
//		{
//			if(m_DataModel.BeforeTokenBoxA.emStatus == STATUS_INSTALLED){
//				//// 判断是否达到将空，否则不允许卸载
//				//if(theAFC_TVM_OPERATION.GetTicketNearEmptyCount() < m_DataModel.BeforeTokenBoxA.nQuantity){
//				//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENACANNOTARRIVEEMPTY));
//				//}
//				//else{
//				//	// 判断箱子是否在位？
//				//	if(IsTokenBoxAStandby()){
//						m_DataModel.AfterTokenBoxA.bIsSelected  = true;
//						m_DataModel.AfterTokenBoxA.emStatus		= STATUS_READY_UNINSTALL;// 待卸载
//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//				//	}
//				//	else{
//				//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENA_ATTACHERROR));	// 箱子非法卸下
//				//		theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
//				//	}
//				//}
//			}
//			else{
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENANOTATTACH));// 没有安装TOKEN箱A
//				theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//			}
//		}
//		break;
//	case MENU_CODE_2:
//		{
//			if(m_DataModel.BeforeTokenBoxB.emStatus == STATUS_INSTALLED){
//				//if(theAFC_TVM_OPERATION.GetTicketNearEmptyCount() < m_DataModel.BeforeTokenBoxB.nQuantity){
//				//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBCANNOTARRIVEEMPTY));
//				//}
//				//else{
//				//	if(IsTokenBoxBStandby()){
//						m_DataModel.AfterTokenBoxB.bIsSelected  = true;
//						m_DataModel.AfterTokenBoxB.emStatus		= STATUS_READY_UNINSTALL;// 待卸下
//						theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//				//	}
//				//	else{
//				//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENB_ATTACHERROR));	// 箱子非法卸下
//				//		theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
//				//	}
//				//}
//			}
//			else{
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENBNOTATTACH));// 没有安装TOKEN箱B
//				theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//			}
//		}
//		break;
//	case MENU_CODE_3:
//		{
//			if(m_DataModel.BeforeInvalidTokenBox.emStatus == STATUS_INSTALLED){
//				//if(IsInvalidBoxStandby()){
//					m_DataModel.AfterInvalidTokenBox.bIsSelected = true;
//					m_DataModel.AfterInvalidTokenBox.emStatus	 = STATUS_READY_UNINSTALL;// 待卸下
//					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//				//}
//				//else{
//				//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTED_ATTACHERROR));	// 箱子非法卸下
//				//	theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
//				//}
//			}
//			else{
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_WASTEDNOTATTACH));// 没有安装废票箱
//				theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//			}
//		}
//		break;
//	case MENU_CODE_4:
//		{
//			if(m_DataModel.BeforeCollectedTokenBox.emStatus == STATUS_INSTALLED){
//				//if(IsInvalidBoxStandby()){
//					m_DataModel.AfterCollectedTokenBox.bIsSelected = true;
//					m_DataModel.AfterCollectedTokenBox.emStatus	 = STATUS_READY_UNINSTALL;// 待卸下
//					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//				//}
//				//else{
//				//	theAPP_SESSION.ShowOfficeGuide(_T("回收箱安装错误"));	// 箱子非法卸下
//				//	theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
//				//}
//			}
//			else{
//				theAPP_SESSION.ShowOfficeGuide(_T("没有安装回收箱"));// 
//				theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//			}
//		}
//		break;
//	case MENU_CODE_5:
//		{
//			CString strLastErrorMsg(_T(""));
//			// 卸载所有的，把能卸载的都卸载
//			if(m_DataModel.BeforeTokenBoxA.emStatus == STATUS_INSTALLED || m_DataModel.BeforeTokenBoxB.emStatus == STATUS_INSTALLED
//				|| m_DataModel.BeforeInvalidTokenBox.emStatus == STATUS_INSTALLED || m_DataModel.BeforeCollectedTokenBox.emStatus == STATUS_INSTALLED){
//				if(m_DataModel.BeforeTokenBoxA.emStatus == STATUS_INSTALLED /*&& IsTokenBoxAStandby()*/){
//				//	if( m_DataModel.BeforeTokenBoxA.nQuantity < theAFC_TVM_OPERATION.GetTicketNearEmptyCount()){
//						m_DataModel.AfterTokenBoxA.bIsSelected  = true;
//						m_DataModel.AfterTokenBoxA.emStatus		= STATUS_READY_UNINSTALL;
//					//}
//					//else{
//					//	strLastErrorMsg = _opl(TXT_TOKENMGR_TOKENACANNOTARRIVEEMPTY);
//					//}
//				}
//
//				if(m_DataModel.BeforeTokenBoxB.emStatus == STATUS_INSTALLED /*&& IsTokenBoxBStandby() */){
//					//if( m_DataModel.BeforeTokenBoxB.nQuantity < theAFC_TVM_OPERATION.GetTicketNearEmptyCount()){
//						m_DataModel.AfterTokenBoxB.bIsSelected  = true;
//						m_DataModel.AfterTokenBoxB.emStatus		= STATUS_READY_UNINSTALL;
//					//}
//					//else{
//					//	strLastErrorMsg = _opl(TXT_TOKENMGR_TOKENBCANNOTARRIVEEMPTY);
//					//}
//				}
//
//				if(m_DataModel.BeforeInvalidTokenBox.emStatus == STATUS_INSTALLED/* && IsInvalidBoxStandby()*/){
//					m_DataModel.AfterInvalidTokenBox.bIsSelected = true;
//					m_DataModel.AfterInvalidTokenBox.emStatus	 = STATUS_READY_UNINSTALL;// 待卸下
//				}
//
//				if(m_DataModel.BeforeCollectedTokenBox.emStatus == STATUS_INSTALLED/* && IsCollectedBoxStandby()*/){
//					m_DataModel.AfterCollectedTokenBox.bIsSelected = true;
//					m_DataModel.AfterCollectedTokenBox.emStatus	 = STATUS_READY_UNINSTALL;// 待卸下
//				}
//
//				if(m_DataModel.AfterTokenBoxA.bIsSelected || m_DataModel.AfterTokenBoxB.bIsSelected
//					|| m_DataModel.AfterInvalidTokenBox.bIsSelected || m_DataModel.AfterCollectedTokenBox.bIsSelected){
//					// 提示按F2保存
//					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_ESC));
//				}
//				else{// 未选中任何卸载的箱子
//					if(strLastErrorMsg.IsEmpty()){
//						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NOBOXATTACH));
//					}
//					else{
//						theAPP_SESSION.ShowOfficeGuide(strLastErrorMsg);
//					}
//				}
//			}
//			else{
//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_NOBOXATTACH));	// 没有安装任何票箱
//			}
//		}
//		break;
//	default:
//		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));	// 菜单输入非法
//		break;
//	}
//
//	NotifyDataModelChanged();
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      响应回收操作
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoRecycleOperator(int boxNo)
//{
//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_RECYCLEING));
//	int iCount_A = 0;
//	int iCount_B = 0;
//	UINT nCleanNum = 0;
//	CTTokenCountInfo::TOKEN_BOX_INFO hopperAInfo = theTOKEN_COUNT.GetHopperAInfo();
//	CTTokenCountInfo::TOKEN_BOX_INFO hopperBInfo = theTOKEN_COUNT.GetHopperBInfo();
//	CTTokenCountInfo::TOKEN_BOX_INFO wastedInfo = theTOKEN_COUNT.GetTokenboxWastedInfo();
//	CTTokenCountInfo::TOKEN_BOX_INFO collectedInfo = theTOKEN_COUNT.GetTokenboxCollectedInfo();
//
//	// 回收
//	try{
//		TOKEN_RSP_GET_DEV_STATUS status;
//		TOKEN_HELPER->GetDevStatus(&status);
//		// 先回收左边Hopper的
//		if (boxNo==BOX_A){
//			// 1. 清空hopper
//			try{
//				LOG("Hopper A回收前：Hopper A-%d, Hopper B-%d，废票箱-%d, 回收箱-%d", hopperAInfo.ulCurCount, hopperBInfo.ulCurCount, wastedInfo.ulCurCount, collectedInfo.ulCurCount);
//				m_DataModel.AfterTokenHopperA.bIsSelected = true;
//				//TOKEN_HELPER->TOKENRecycleCard(TOKEN_HOPPER_1,issue);
//				// 使用清空命令回收token
//				TOKEN_HELPER->CleanOut(TOKEN_BOX_A);
//				if(status.bHasCardAtTemporaryA){
//					iCount_A = hopperAInfo.ulCurCount - 1;
//					hopperAInfo.ulCurCount = 1;
//				}
//				else{
//					iCount_A = hopperAInfo.ulCurCount;
//					hopperAInfo.ulCurCount = 0;
//				}
//				collectedInfo.ulCurCount += iCount_A;
//				theTOKEN_COUNT.SetHopperAInfo(hopperAInfo);
//				theTOKEN_COUNT.SetTokenboxCollectedInfo(collectedInfo);
//			}
//			catch(CSysException& e){
//				theEXCEPTION_MGR.ProcessException(e);
//				m_DataModel.AfterTokenHopperA.bIsSelected = false;
//				m_DataModel.AfterTokenHopperA.emStatus = STATUS_RECYCLE_ERROR;
//			}
//			catch(...){
//				m_DataModel.AfterTokenHopperA.emStatus = STATUS_RECYCLE_ERROR;
//			}
//			if(m_DataModel.AfterTokenHopperA.emStatus == STATUS_RECYCLE_ERROR){
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,false);
//				return;
//			}
//
//			// 2. 暂存区的票到读写区（由于暂存区的票不会被清理，且无法被清理到回收箱，因此这里需要主动将暂存区的票清理到废票箱。）
//			if(m_DataModel.AfterTokenHopperA.emStatus != STATUS_RECYCLE_ERROR){
//				try{
//					if(status.bHasCardAtTemporaryA){
//						TOKEN_HELPER->CardOut(TOKEN_BOX_A, &nCleanNum);
//					}
//				}
//				catch(CSysException& e){
//					theEXCEPTION_MGR.ProcessException(e);
//					m_DataModel.AfterTokenHopperA.bIsSelected = false;
//					m_DataModel.AfterTokenHopperA.emStatus = STATUS_RECYCLE_ERROR;
//				}
//				catch(...){
//					m_DataModel.AfterTokenHopperA.emStatus = STATUS_RECYCLE_ERROR;
//				}
//			}
//			if(m_DataModel.AfterTokenHopperA.emStatus == STATUS_RECYCLE_ERROR){
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,false);
//				return;
//			}
//
//			// 3. 读写区的票到废票箱（由于暂存区的票不会被清理，且无法被清理到回收箱，因此这里需要主动将暂存区的票清理到废票箱。）
//			if(m_DataModel.AfterTokenHopperA.emStatus != STATUS_RECYCLE_ERROR){
//				try{
//					if (nCleanNum!=0){
//						TOKEN_HELPER->RetractCard(&nCleanNum);
//						wastedInfo.ulCurCount += nCleanNum;
//						hopperAInfo.ulCurCount = hopperAInfo.ulCurCount - 1;
//						theTOKEN_COUNT.SetHopperAInfo(hopperAInfo);
//						theTOKEN_COUNT.SetTokenboxWastedInfo(wastedInfo);
//					}
//				}
//				catch(CSysException& e){
//					theEXCEPTION_MGR.ProcessException(e);
//					m_DataModel.AfterTokenHopperA.bIsSelected = false;
//					m_DataModel.AfterTokenHopperA.emStatus = STATUS_RECYCLE_ERROR;
//				}
//				catch(...){
//					m_DataModel.AfterTokenHopperA.emStatus = STATUS_RECYCLE_ERROR;
//				}
//			}
//
//			if(m_DataModel.AfterTokenHopperA.emStatus == STATUS_RECYCLE_ERROR){
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,false);
//				return;
//			}
//
//			// 4. 更新状态，上送操作日志
//			theTVM_STATUS_MGR.SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,true);
//			if(m_DataModel.AfterTokenHopperA.emStatus != STATUS_RECYCLE_ERROR){
//				m_DataModel.AfterTokenHopperA.nQuantity = 0;
//				m_DataModel.AfterCollectedTokenBox.nQuantity += iCount_A;
//				TokenCollectOperationRecord hopperAColl;
//				hopperAColl.dtOperationTime			= ComGetCurTime();
//				hopperAColl.txtTokenBoxID			= hopperAInfo.tokenBoxID.ToString();
//				hopperAColl.iQuantityBeforeCollect	= m_DataModel.BeforeTokenHopperA.nQuantity;
//				hopperAColl.iQuantityCollect		= iCount_A/*m_DataModel.BeforeTokenHopperA.nQuantity*/;
//				hopperAColl.iQuantityAfterCollect	= hopperAInfo.ulCurCount;
//				hopperAColl.txtTokenCollectBoxID	= INVALID_ID;
//				hopperAColl.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
//				hopperAColl.txtReserve				= _T("");
//				hopperAColl.txtComment				= _T("");
//
//				try{
//					DATA_HELPER->DbInsertTokenCollectOperationRecord(hopperAColl);
//				}
//				catch(...){
//					m_DataModel.bInsertDataBaseSuccess = false;
//				}
//			}
//			LOG("Hopper A回收后：Hopper A-%d, Hopper B-%d，废票箱-%d, 回收箱-%d", hopperAInfo.ulCurCount, hopperBInfo.ulCurCount, wastedInfo.ulCurCount, collectedInfo.ulCurCount);
//		}
//
//
//
//
//
//		// 再回收右边Hopper的
//		if (boxNo==BOX_B){
//			// 1. 清空hopper
//			try{
//				LOG("Hopper B回收前：Hopper A-%d, Hopper B-%d，废票箱-%d, 回收箱-%d", hopperAInfo.ulCurCount, hopperBInfo.ulCurCount, wastedInfo.ulCurCount, collectedInfo.ulCurCount);
//				m_DataModel.AfterTokenHopperB.bIsSelected = true;
//				//TOKEN_HELPER->TOKENRecycleCard(TOKEN_HOPPER_2,issue);
//				// 使用清空命令回收token
//				TOKEN_HELPER->CleanOut(TOKEN_BOX_B);
//				if(status.bHasCardAtTemporaryB){
//					iCount_B = hopperBInfo.ulCurCount - 1;
//					hopperBInfo.ulCurCount = 1;
//				}
//				else{
//					iCount_B = hopperBInfo.ulCurCount;
//					hopperBInfo.ulCurCount = 0;
//				}
//				collectedInfo.ulCurCount += iCount_B;
//				theTOKEN_COUNT.SetHopperBInfo(hopperBInfo);
//				theTOKEN_COUNT.SetTokenboxCollectedInfo(collectedInfo);
//			}
//			catch(CSysException& e){
//				theEXCEPTION_MGR.ProcessException(e);
//				m_DataModel.AfterTokenHopperB.bIsSelected = false;
//				m_DataModel.AfterTokenHopperB.emStatus = STATUS_RECYCLE_ERROR;
//			}
//			catch(...){
//				m_DataModel.AfterTokenHopperB.emStatus = STATUS_RECYCLE_ERROR;
//			}
//			if(m_DataModel.AfterTokenHopperB.emStatus == STATUS_RECYCLE_ERROR){
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,false);
//				return;
//			}
//
//			// 2. 暂存区的票到读写区（由于暂存区的票不会被清理，且无法被清理到回收箱，因此这里需要主动将暂存区的票清理到废票箱。）
//			if(m_DataModel.AfterTokenHopperB.emStatus != STATUS_RECYCLE_ERROR){
//				try{
//					if(status.bHasCardAtTemporaryB){
//						TOKEN_HELPER->CardOut(TOKEN_BOX_B, &nCleanNum);
//					}
//				}
//				catch(CSysException& e){
//					theEXCEPTION_MGR.ProcessException(e);
//					m_DataModel.AfterTokenHopperB.bIsSelected = false;
//					m_DataModel.AfterTokenHopperB.emStatus = STATUS_RECYCLE_ERROR;
//				}
//				catch(...){
//					m_DataModel.AfterTokenHopperB.emStatus = STATUS_RECYCLE_ERROR;
//				}
//			}
//			if(m_DataModel.AfterTokenHopperB.emStatus == STATUS_RECYCLE_ERROR){
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,false);
//				return;
//			}
//
//			// 3. 读写区的票到废票箱（由于暂存区的票不会被清理，且无法被清理到回收箱，因此这里需要主动将暂存区的票清理到废票箱。）
//			if(m_DataModel.AfterTokenHopperB.emStatus != STATUS_RECYCLE_ERROR){
//				try{
//					if (nCleanNum!=0){
//						TOKEN_HELPER->RetractCard(&nCleanNum);
//						wastedInfo.ulCurCount += nCleanNum;
//						hopperBInfo.ulCurCount = hopperBInfo.ulCurCount - 1;
//						theTOKEN_COUNT.SetHopperBInfo(hopperBInfo);
//						theTOKEN_COUNT.SetTokenboxWastedInfo(wastedInfo);
//					}
//				}
//				catch(CSysException& e){
//					theEXCEPTION_MGR.ProcessException(e);
//					m_DataModel.AfterTokenHopperB.bIsSelected = false;
//					m_DataModel.AfterTokenHopperB.emStatus = STATUS_RECYCLE_ERROR;
//				}
//				catch(...){
//					m_DataModel.AfterTokenHopperB.emStatus = STATUS_RECYCLE_ERROR;
//				}
//			}
//
//			if(m_DataModel.AfterTokenHopperB.emStatus == STATUS_RECYCLE_ERROR){
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,false);
//				return;
//			}
//
//			// 4. 更新状态，上送操作日志
//			theTVM_STATUS_MGR.SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,true);
//			if(m_DataModel.AfterTokenHopperB.emStatus != STATUS_RECYCLE_ERROR){
//				m_DataModel.AfterTokenHopperB.nQuantity = 0;
//				m_DataModel.AfterCollectedTokenBox.nQuantity += iCount_B;
//				TokenCollectOperationRecord hopperBColl;
//				hopperBColl.dtOperationTime			= ComGetCurTime();
//				hopperBColl.txtTokenBoxID			= hopperBInfo.tokenBoxID.ToString();
//				hopperBColl.iQuantityBeforeCollect	= m_DataModel.BeforeTokenHopperB.nQuantity;
//				hopperBColl.iQuantityCollect		= iCount_B/*m_DataModel.BeforeTokenHopperA.nQuantity*/;
//				hopperBColl.iQuantityAfterCollect	= hopperBInfo.ulCurCount;
//				hopperBColl.txtTokenCollectBoxID	= INVALID_ID;
//				hopperBColl.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
//				hopperBColl.txtReserve				= _T("");
//				hopperBColl.txtComment				= _T("");
//
//				try{
//					DATA_HELPER->DbInsertTokenCollectOperationRecord(hopperBColl);
//				}
//				catch(...){
//					m_DataModel.bInsertDataBaseSuccess = false;
//				}
//			}
//			LOG("Hopper B回收后：Hopper A-%d, Hopper B-%d，废票箱-%d, 回收箱-%d", hopperAInfo.ulCurCount, hopperBInfo.ulCurCount, wastedInfo.ulCurCount, collectedInfo.ulCurCount);
//		}
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENMODULEEXCEPTION));
//	}
//	catch(...){
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_TOKENMODULEEXCEPTION));
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      显示前缀提示语
//
//@param      CString strInput;需要显示的内容
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::ShowPrefixGuideMsg(CString strInput){
//	// 添加冒号
//	CString strShowUp(strInput);
//	if(!strShowUp.IsEmpty()){
//		// 删除所有提示与输入信息
//		theAPP_SESSION.DeleteAllInput();
//		strShowUp += _T(":");
//		theAPP_SESSION.SetPrefixToGuide(strShowUp);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      TOKEN箱A是否在位?
//
//@param      无
//
//@retval     bool true:在位；false：不在位
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::IsTokenBoxAStandby(){
//	try{
//		TOKEN_RSP_GET_DEV_STATUS status;
//		TOKEN_HELPER->GetDevStatus(&status);
//		if(status.bBoxAExist)
//			return true;
//		else
//			return false;
//	}
//	catch(...){
//		return false;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      TOKEN箱B是否在位?
//
//@param      无
//
//@retval     bool true:在位；false：不在位
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::IsTokenBoxBStandby(){
//	try{
//		TOKEN_RSP_GET_DEV_STATUS status;
//		TOKEN_HELPER->GetDevStatus(&status);
//		if(status.bBoxBExist)
//			return true;
//		else
//			return false;
//	}
//	catch(...){
//		return false;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      废票箱是否在位？
//
//@param      无
//
//@retval     bool true:在位；false：不在位
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::IsInvalidBoxStandby(){
//	try{
//		// 废票箱是否在位，是由IO检测的
//		IO_RSP_STATUS_INFO ioStatus;
//		IO_HELPER->GetIOStatus(ioStatus);
//		if(ioStatus.isTokenInvBoxReady)
//			return true;
//		else 
//			return false;
//	}
//	catch(...){
//		return false;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      回收箱是否在位？
//
//@param      无
//
//@retval     bool true:在位；false：不在位
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::IsCollectedBoxStandby(){
//	try{
//		// 回收箱是否在位，是由IO检测的
//		IO_RSP_STATUS_INFO ioStatus;
//		IO_HELPER->GetIOStatus(ioStatus);
//		if(ioStatus.isTokenColBoxReady)
//			return true;
//		else 
//			return false;
//	}
//	catch(...){
//		return false;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      A箱挡板是否打开
//
//@param      无
//
//@retval     bool true:打开；false:未打开
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::IsTokenBoxAApronOpen(){
//	try{
//		// 挡板的判断？？？//lichao
//		return true;
//
//		//TOKEN_RSP_READ_STATUS  status;
//		//TOKEN_HELPER->ReadStatus(status);
//		//if(status.box1BezelNotCloseOK != 0){
//		//	return true;lichao
//		//}
//		return false;
//	}
//	catch(...){
//		return false;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      B箱挡板是否打开
//
//@param      无
//
//@retval     bool true:打开；false:未打开
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::IsTokenBoxBApronOpen(){
//	try{
//		// 挡板的判断？？？//lichao
//		return true;
//
//		//TOKEN_RSP_READ_STATUS  status;
//		//TOKEN_HELPER->ReadStatus(status);
//		//if(status.box2BezelNotCloseOK != 0){
//		//	return true;lichao
//		//}
//		return false;
//	}
//	catch(...){
//		return false;
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      安装TOKEN箱A
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoInstallTokenBoxA(){
//	//try{
//	//	// 设置Token箱A的状态
//	//	TCHAR* pChar = NULL;
//	//	WORD wNum = _tcstoul(m_DataModel.AfterTokenBoxA.strBoxID.Mid(4),&pChar,16);
//	//	CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxInfo;
//	//	BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
//	//	int2BCD(bLineCode, (char*)&tokenBoxInfo.tokenBoxID.bLineCode, sizeof(tokenBoxInfo.tokenBoxID.bLineCode));
//	//	tokenBoxInfo.tokenBoxID.bType = 0x01;	
//	//	tokenBoxInfo.tokenBoxID.wNum = wNum;
//	//	tokenBoxInfo.ulInitCount		  = 0;
//	//	tokenBoxInfo.ulCurCount			  = 0;
//	//	tokenBoxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//	//	tokenBoxInfo.operateTime = ComGetCurTime();
//	//	m_DataModel.AfterTokenBoxA.ulOperatorID = tokenBoxInfo.ulOperatorID;
//	//	m_DataModel.AfterTokenBoxA.operateTime = tokenBoxInfo.operateTime;
//	//	theTOKEN_COUNT.SetTokenboxAInfo(tokenBoxInfo);
//
//	//	// 设置Hopper的数量
//	//	CTTokenCountInfo::TOKEN_BOX_INFO hopperAInfo = theTOKEN_COUNT.GetHopperAInfo();
//	//	LOG("A票箱安装前：Hopper A-%d", hopperAInfo.ulCurCount);
//	//	m_DataModel.BeforeTokenHopperA.nQuantity = hopperAInfo.ulCurCount;
//	//	m_DataModel.BeforeTokenHopperA.strBoxID	 = hopperAInfo.tokenBoxID.ToString();
//
//	//	// 设置上次留存
//	//	hopperAInfo.ulInitCount  = hopperAInfo.ulCurCount;
//
//	//	hopperAInfo.ulCurCount	+= m_DataModel.AfterTokenBoxA.nQuantity;
//
//	//	theTOKEN_COUNT.SetHopperAInfo(hopperAInfo);
//	//	m_DataModel.AfterTokenHopperA.nQuantity = hopperAInfo.ulCurCount;
//	//	m_DataModel.AfterTokenHopperA.strBoxID	= hopperAInfo.tokenBoxID.ToString();
//	//	m_DataModel.AfterTokenHopperA.bIsSelected = true;
//
//	//	// 转换安装前后TOKEN箱数量的变化
//	//	m_DataModel.BeforeTokenBoxA.nQuantity = m_DataModel.AfterTokenBoxA.nQuantity;
//	//	m_DataModel.AfterTokenBoxA.nQuantity  = 0;
//	//	m_DataModel.BeforeTokenBoxA.strBoxID  = m_DataModel.AfterTokenBoxA.strBoxID;
//
//	//	// 插入数据库
//	//	TokenBoxAddOperationRecord tokenBoxAddOperationRecord;
//	//	tokenBoxAddOperationRecord.dtOperationTime			= ComGetCurTime();
//	//	tokenBoxAddOperationRecord.txtTokenAddBoxID			= m_DataModel.AfterTokenBoxA.strBoxID;
//	//	tokenBoxAddOperationRecord.txtTokenHopperBoxID		= hopperAInfo.tokenBoxID.ToString();
//	//	tokenBoxAddOperationRecord.iQuantityBeforeAdd		= m_DataModel.BeforeTokenHopperA.nQuantity;
//	//	tokenBoxAddOperationRecord.iQuantityAdd				= m_DataModel.BeforeTokenBoxA.nQuantity;
//	//	tokenBoxAddOperationRecord.iQuantityAfterAdd		= hopperAInfo.ulCurCount;
//	//	tokenBoxAddOperationRecord.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
//	//	try{
//	//		DATA_HELPER->DbInsertTokenBoxAddOperationRecord(tokenBoxAddOperationRecord);
//	//	}
//	//	catch(CSysException& e){
//	//		theEXCEPTION_MGR.ProcessException(e);
//	//		m_DataModel.bInsertDataBaseSuccess = false;
//	//	}
//	//	catch(...){
//	//		m_DataModel.bInsertDataBaseSuccess = false;
//	//	}
//
//	//	//上发数据
//	//	MAGAZINE_CHANGE change;
//	//	change.operType =  ATTACH;
//	//	change.magazineId.bLineCode = tokenBoxInfo.tokenBoxID.bLineCode;
//	//	change.magazineId.bType = tokenBoxInfo.tokenBoxID.bType;
//	//	change.magazineId.wNum = tokenBoxInfo.tokenBoxID.wNum;
//	//	change.magazne =MAGAZINEC;
//	//	change.cardType = 0;//UL卡类型
//	//	change.providerId = 0x01;
//	//	change.cardProductType = 0;
//	//	change.preProperty = 0x01;
//	//	change.childproductId = 0x01;
//	//	change.CardCount = m_DataModel.BeforeTokenBoxA.nQuantity;
//	//	change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_ATTACH;
//	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//
//	//	// 记录操作日志
//	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,true);
//
//	//	m_DataModel.AfterTokenBoxA.emStatus	= STATUS_INSTALLED;
//	//	// 设置Hopper状态
//	//	theTVM_STATUS_MGR.SetTokenHopper1NumStatus(MAG_NUM_NORMAL);
//
//	//	// 设置Token箱的安装状态
//	//	theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
//
//	//	// 设置Token箱的数量状态
//	//	theTVM_STATUS_MGR.SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
//	//	LOG("A票箱安装后：Hopper A-%d", hopperAInfo.ulCurCount);
//	//}
//	//catch(CSysException& e){
//	//	theEXCEPTION_MGR.ProcessException(e);
//	//	m_DataModel.AfterTokenBoxA.emStatus = STATUS_INSTALL_ERROR;// 安装出现异常
//	//}
//	//catch(...){
//	//	m_DataModel.AfterTokenBoxA.emStatus = STATUS_INSTALL_ERROR;// 安装出现异常
//	//}
//
//	//if(m_DataModel.AfterTokenBoxA.emStatus == STATUS_INSTALL_ERROR){
//	//	// 记录操作日志
//	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,false);
//	//}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      安装TOKEN箱B
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoInstallTokenBoxB(){
//	try{
//		// 设置Token箱B的状态
//		TCHAR* pChar = NULL;
//		WORD wNum = _tcstoul(m_DataModel.AfterTokenBoxB.strBoxID.Mid(4),&pChar,16);
//		CTTokenCountInfo::TOKEN_BOX_INFO tokenBoxInfo;
//		BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
//		int2BCD(bLineCode, (char*)&tokenBoxInfo.tokenBoxID.bLineCode, sizeof(tokenBoxInfo.tokenBoxID.bLineCode));
//		tokenBoxInfo.tokenBoxID.bType = 0x01;	
//		tokenBoxInfo.tokenBoxID.wNum	= wNum;
//		tokenBoxInfo.ulInitCount		  = 0;
//		tokenBoxInfo.ulCurCount			  = 0;
//		tokenBoxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//		tokenBoxInfo.operateTime = ComGetCurTime();
//		m_DataModel.AfterTokenBoxB.ulOperatorID = tokenBoxInfo.ulOperatorID;
//		m_DataModel.AfterTokenBoxB.operateTime = tokenBoxInfo.operateTime;
//		theTOKEN_COUNT.SetTokenboxBInfo(tokenBoxInfo);
//
//		// 设置Hopper的数量
//		CTTokenCountInfo::TOKEN_BOX_INFO hopperBInfo = theTOKEN_COUNT.GetHopperBInfo();
//		LOG("B票箱安装前：Hopper B-%d", hopperBInfo.ulCurCount);
//		m_DataModel.BeforeTokenHopperB.nQuantity = hopperBInfo.ulCurCount;
//		m_DataModel.BeforeTokenHopperB.strBoxID	 = hopperBInfo.tokenBoxID.ToString();
//
//		// 设置上次留存
//		hopperBInfo.ulInitCount  = hopperBInfo.ulCurCount;
//
//		hopperBInfo.ulCurCount	+= m_DataModel.AfterTokenBoxB.nQuantity;
//
//		theTOKEN_COUNT.SetHopperBInfo(hopperBInfo);
//		m_DataModel.AfterTokenHopperB.nQuantity = hopperBInfo.ulCurCount;
//		m_DataModel.AfterTokenHopperB.strBoxID	= hopperBInfo.tokenBoxID.ToString();
//		m_DataModel.AfterTokenHopperB.bIsSelected	= true;
//
//		// 转换安装前后TOKEN箱数量的变化
//		m_DataModel.BeforeTokenBoxB.nQuantity = m_DataModel.AfterTokenBoxB.nQuantity;
//		m_DataModel.AfterTokenBoxB.nQuantity  = 0;
//		m_DataModel.BeforeTokenBoxB.strBoxID  = m_DataModel.AfterTokenBoxB.strBoxID;
//
//		// 插入数据库
//		/*TokenBoxInstallOperationRecord installRecord;
//		installRecord.dtOperationTime		= ComGetCurTime();
//		installRecord.iQuantity				= m_DataModel.AfterTokenBoxB.nQuantity;
//		installRecord.txtTokenBoxID			= m_DataModel.AfterTokenBoxB.strBoxID;
//		installRecord.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
//		installRecord.txtComment			= _T("");
//		installRecord.txtReserve			= _T("");
//		DATA_HELPER->DbInsertTokenBoxInstallOperationRecord(installRecord);*/
//		TokenBoxAddOperationRecord tokenAddRecord;
//		tokenAddRecord.dtOperationTime		= ComGetCurTime();
//		tokenAddRecord.iQuantityBeforeAdd	= m_DataModel.BeforeTokenHopperB.nQuantity;
//		tokenAddRecord.iQuantityAdd			= m_DataModel.BeforeTokenBoxB.nQuantity;
//		tokenAddRecord.iQuantityAfterAdd	= hopperBInfo.ulCurCount;
//		tokenAddRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
//		tokenAddRecord.txtTokenAddBoxID		= m_DataModel.BeforeTokenBoxB.strBoxID;
//		tokenAddRecord.txtTokenHopperBoxID	= m_DataModel.AfterTokenHopperB.strBoxID;
//		tokenAddRecord.txtReserve			= _T("");
//		tokenAddRecord.txtComment			= _T("");
//		try{
//			DATA_HELPER->DbInsertTokenBoxAddOperationRecord(tokenAddRecord);
//		}
//		catch(CSysException& e){
//			theEXCEPTION_MGR.ProcessException(e);
//			m_DataModel.bInsertDataBaseSuccess = false;
//		}
//		catch(...){
//			m_DataModel.bInsertDataBaseSuccess = false;
//		}
//
//		//上发数据
//		MAGAZINE_CHANGE change;
//		change.operType =  ATTACH;
//		change.magazineId.bLineCode = tokenBoxInfo.tokenBoxID.bLineCode;
//		change.magazineId.bType = tokenBoxInfo.tokenBoxID.bType;
//		change.magazineId.wNum = tokenBoxInfo.tokenBoxID.wNum;
//		change.magazne =MAGAZINEB;
//		change.cardType =0;//UL卡类型
//		change.providerId = 0x01;
//		change.cardProductType = 0;
//		change.preProperty = 0x01;
//		change.childproductId = 0x01;
//		change.CardCount = m_DataModel.BeforeTokenBoxB.nQuantity;
//		change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_ATTACH;	// LIU_WENSHENG 内部接口文档对应
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//
//		// 记录操作日志
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,true);
//
//		m_DataModel.AfterTokenBoxB.emStatus	= STATUS_INSTALLED;
//
//		// 设置Hopper状态
//		theTVM_STATUS_MGR.SetTokenHopper2NumStatus(MAG_NUM_NORMAL);
//
//		// 设置Token箱的安装状态
//		theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
//
//		// 设置Token箱的数量状态
//		theTVM_STATUS_MGR.SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
//
//		LOG("B票箱安装后：Hopper B-%d", hopperBInfo.ulCurCount);
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		m_DataModel.AfterTokenBoxB.emStatus = STATUS_INSTALL_ERROR;// 安装出现异常
//	}
//	catch(...){
//		m_DataModel.AfterTokenBoxB.emStatus = STATUS_INSTALL_ERROR;// 安装出现异常
//	}
//
//	if(m_DataModel.AfterTokenBoxB.emStatus == STATUS_INSTALL_ERROR){
//		// 记录操作日志
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,false);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      安装TOKEN废票箱
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoInstallInvalidTokenBox(){
//	try{
//		TCHAR* pChar = NULL;
//		WORD wNum	 = _tcstoul(m_DataModel.AfterInvalidTokenBox.strBoxID.Mid(4),&pChar,16);
//		CTTokenCountInfo::TOKEN_BOX_INFO invalidToken;
//		BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
//		int2BCD(bLineCode, (char*)&invalidToken.tokenBoxID.bLineCode, sizeof(invalidToken.tokenBoxID.bLineCode));
//		invalidToken.tokenBoxID.bType = 0x03;	
//		invalidToken.tokenBoxID.wNum = wNum;
//		invalidToken.ulCurCount  = 0;
//		invalidToken.ulInitCount = 0;
//		invalidToken.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//		invalidToken.operateTime = ComGetCurTime();
//		theTOKEN_COUNT.SetTokenboxWastedInfo(invalidToken);
//		m_DataModel.AfterInvalidTokenBox/*BeforeInvalidTokenBox*/.strBoxID = m_DataModel.AfterInvalidTokenBox.strBoxID = invalidToken.tokenBoxID.ToString();
//		m_DataModel.AfterInvalidTokenBox/*BeforeInvalidTokenBox*/.nQuantity = m_DataModel.AfterInvalidTokenBox.nQuantity = 0;
//		m_DataModel.AfterInvalidTokenBox.bIsSelected = true;
//		m_DataModel.AfterInvalidTokenBox.ulOperatorID = invalidToken.ulOperatorID;
//		m_DataModel.AfterInvalidTokenBox.operateTime = invalidToken.operateTime;
//
//		// 插入数据库(预留)
//		TokenBoxInstallOperationRecord installRecord;
//		installRecord.dtOperationTime		= ComGetCurTime();
//		installRecord.iQuantity				= 0;
//		installRecord.txtTokenBoxID			= m_DataModel.AfterInvalidTokenBox.strBoxID;
//		installRecord.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
//		installRecord.txtComment			= _T("");
//		installRecord.txtReserve			= _T("");
//		DATA_HELPER->DbInsertTokenBoxInstallOperationRecord(installRecord);
//
//		// 发送给上位一个消息
//		MAGAZINE_CHANGE change;
//		change.operType =  ATTACH;
//		change.magazineId.bLineCode = invalidToken.tokenBoxID.bLineCode;
//		change.magazineId.bType = invalidToken.tokenBoxID.bType;
//		change.magazineId.wNum = invalidToken.tokenBoxID.wNum;
//		change.magazne = MAGAZINEA;
//		change.cardType = 0;//UL卡类型
//		change.providerId = 0x01;
//		change.cardProductType = 0;
//		change.preProperty = 0x01;
//		change.childproductId = 0x01;
//		change.CardCount = 0x00;
//		change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_ATTACH;
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//
//		// 记录操作日志
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,true);
//
//		m_DataModel.AfterInvalidTokenBox.emStatus	= STATUS_INSTALLED;
//
//		// 废票箱的安装状态
//		theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
//
//		// 废票箱数量状态
//		theTVM_STATUS_MGR.SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL); // liu_wensheng 2016-1-26 card废票箱没有空状态
//
//		LOG("废票箱安装成功！");
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_INSTALL_ERROR;
//	}
//	catch(...){
//		m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_INSTALL_ERROR;
//	}
//
//	// 安装失败
//	if(m_DataModel.AfterInvalidTokenBox.emStatus == STATUS_INSTALL_ERROR){
//		// 记录操作日志
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,false);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      安装TOKEN回收箱
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoInstallCollectedTokenBox(){
//	try{
//		TCHAR* pChar = NULL;
//		WORD wNum	 = _tcstoul(m_DataModel.AfterCollectedTokenBox.strBoxID.Mid(4),&pChar,16);
//		CTTokenCountInfo::TOKEN_BOX_INFO CollectedToken;
//		BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
//		int2BCD(bLineCode, (char*)&CollectedToken.tokenBoxID.bLineCode, sizeof(CollectedToken.tokenBoxID.bLineCode));
//		CollectedToken.tokenBoxID.bType = 0x02;	
//		CollectedToken.tokenBoxID.wNum	= wNum;
//		CollectedToken.ulCurCount  = 0;
//		CollectedToken.ulInitCount = 0;
//		CollectedToken.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//		CollectedToken.operateTime = ComGetCurTime();
//		theTOKEN_COUNT.SetTokenboxCollectedInfo(CollectedToken);
//		m_DataModel.AfterCollectedTokenBox/*BeforeCollectedTokenBox*/.strBoxID = m_DataModel.AfterCollectedTokenBox.strBoxID = CollectedToken.tokenBoxID.ToString();
//		m_DataModel.AfterCollectedTokenBox/*BeforeCollectedTokenBox*/.nQuantity = m_DataModel.AfterCollectedTokenBox.nQuantity = 0;
//		m_DataModel.AfterCollectedTokenBox.bIsSelected = true;
//		m_DataModel.AfterCollectedTokenBox.ulOperatorID = CollectedToken.ulOperatorID;
//		m_DataModel.AfterCollectedTokenBox.operateTime = CollectedToken.operateTime;
//
//		// 插入数据库(预留)
//		TokenBoxInstallOperationRecord installRecord;
//		installRecord.dtOperationTime		= ComGetCurTime();
//		installRecord.iQuantity				= 0;
//		installRecord.txtTokenBoxID			= m_DataModel.AfterCollectedTokenBox.strBoxID;
//		installRecord.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
//		installRecord.txtComment			= _T("");
//		installRecord.txtReserve			= _T("");
//		DATA_HELPER->DbInsertTokenBoxInstallOperationRecord(installRecord);
//
//		// 发送给上位一个消息
//		MAGAZINE_CHANGE change;
//		change.operType =  ATTACH;
//		change.magazineId.bLineCode = CollectedToken.tokenBoxID.bLineCode;
//		change.magazineId.bType = CollectedToken.tokenBoxID.bType;
//		change.magazineId.wNum = CollectedToken.tokenBoxID.wNum;
//		change.magazne = MAGAZINEF;
//		change.cardType = 0;//UL卡类型
//		change.providerId = 0x01;
//		change.cardProductType = 0;
//		change.preProperty = 0x01;
//		change.childproductId = 0x01;
//		change.CardCount = 0x00;
//		change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_ATTACH;
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//
//		// 记录操作日志
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,true);
//
//		m_DataModel.AfterCollectedTokenBox.emStatus	= STATUS_INSTALLED;
//
//		// 回收箱的安装状态
//		theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
//
//		// 回收箱数量状态
//		theTVM_STATUS_MGR.SetTokenCollectedBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL); 
//
//		LOG("回收箱安装成功！");
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		m_DataModel.AfterCollectedTokenBox.emStatus = STATUS_INSTALL_ERROR;
//	}
//	catch(...){
//		m_DataModel.AfterCollectedTokenBox.emStatus = STATUS_INSTALL_ERROR;
//	}
//
//	// 安装失败
//	if(m_DataModel.AfterCollectedTokenBox.emStatus == STATUS_INSTALL_ERROR){
//		// 记录操作日志
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_INSTALLATION,false);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      卸载TOKEN箱A
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoUninstallTokenBoxA(){
//	try{
//		// 硬件移除TOKEN箱A
//		if(RemoveTokenBoxA()){
//			CTTokenCountInfo::TOKEN_BOX_INFO beforUninstall;
//			beforUninstall = theTOKEN_COUNT.GetTokenboxAInfo();
//
//			CTTokenCountInfo::TOKEN_BOX_INFO tokenAInfo;
//			tokenAInfo.tokenBoxID.bLineCode = 0xFF;
//			tokenAInfo.tokenBoxID.bType		= 0xFF;
//			tokenAInfo.tokenBoxID.wNum		= 0xFFFF;
//			tokenAInfo.ulCurCount = 0;
//			tokenAInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//			tokenAInfo.operateTime = ComGetCurTime();
//			BOOL bRet = theTOKEN_COUNT.SetTokenboxAInfo(tokenAInfo);
//
//			// 卸载后
//			//m_DataModel.AfterTokenBoxA.strBoxID = tokenAInfo.tokenBoxID.ToString();
//			m_DataModel.AfterTokenBoxA.nQuantity= 0;
//			m_DataModel.AfterTokenBoxA.strBoxID = INVALID_ID;
//			m_DataModel.AfterTokenBoxA.ulOperatorID = tokenAInfo.ulOperatorID;
//			m_DataModel.AfterTokenBoxA.operateTime = tokenAInfo.operateTime;
//
//			if(bRet){
//				// 插入数据库（预留）
//				TokenBoxRemoveOperationRecord removeRecord;
//				removeRecord.dtOperationTime	= ComGetCurTime();
//				removeRecord.iOriginalQuantity	= 0;
//				removeRecord.iRemoveQuantity	= 0;
//				removeRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
//				removeRecord.txtTokenBoxID		= m_DataModel.BeforeTokenBoxA.strBoxID;
//				removeRecord.txtReserve			= _T("");
//				removeRecord.txtComment			= _T("");
//				DATA_HELPER->DbInsertTokenBoxRemoveOperationRecord(removeRecord);
//
//				// 上位数据lichao
//				MAGAZINE_CHANGE change;
//				change.operType =  DETACH;
//				change.magazineId.Deserialize(&beforUninstall.tokenBoxID.bLineCode);
//				change.magazineId.bLineCode = beforUninstall.tokenBoxID.bLineCode;
//				change.magazineId.bType = beforUninstall.tokenBoxID.bType;
//				change.magazineId.wNum = beforUninstall.tokenBoxID.wNum;
//				change.magazne =MAGAZINEC;
//				change.cardType =0;//UL卡类型
//				change.providerId = 0x01;
//				change.cardProductType = 0;
//				change.preProperty = 0x01;
//				change.childproductId = 0x01;
//				change.CardCount = 0x00;
//				change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//			
//				// 记录操作日志
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,true);
//			}
//
//			m_DataModel.AfterTokenBoxA.emStatus = STATUS_UNINSTALL;
//
//			// Token箱安装状态
//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//
//			// Token箱数量状态
//			theTVM_STATUS_MGR.SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
//
//			LOG("票箱A卸载成功！");
//		}
//		else{
//			// 卸载失败
//			m_DataModel.AfterTokenBoxA.emStatus = STATUS_UNINSTALL_ERROR;
//		}
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		// 卸载失败
//		m_DataModel.AfterTokenBoxA.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	catch(...){
//		// 卸载失败
//		m_DataModel.AfterTokenBoxA.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//
//	// 产生错误
//	if(m_DataModel.AfterTokenBoxA.emStatus == STATUS_UNINSTALL_ERROR){
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,false);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      卸载TOKEN箱B
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoUninstallTokenBoxB(){
//	try{
//		// 硬件移除TOKEN箱B
//		if(RemoveTokenBoxB()){
//			CTTokenCountInfo::TOKEN_BOX_INFO beforUninstall;
//			beforUninstall = theTOKEN_COUNT.GetTokenboxBInfo();
//
//			CTTokenCountInfo::TOKEN_BOX_INFO tokenBInfo;
//			tokenBInfo.tokenBoxID.bLineCode	= 0xFF;
//			tokenBInfo.tokenBoxID.bType		= 0xFF;
//			tokenBInfo.tokenBoxID.wNum		= 0xFFFF;
//			tokenBInfo.ulCurCount = 0;
//			tokenBInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//			tokenBInfo.operateTime = ComGetCurTime();
//
//			BOOL bRet = theTOKEN_COUNT.SetTokenboxBInfo(tokenBInfo);
//			m_DataModel.AfterTokenBoxB.strBoxID = INVALID_ID;
//			m_DataModel.AfterTokenBoxB.ulOperatorID = tokenBInfo.ulOperatorID;
//			m_DataModel.AfterTokenBoxB.operateTime = tokenBInfo.operateTime;
//
//			// 卸载后
//			//m_DataModel.AfterTokenBoxB.strBoxID = tokenBInfo.tokenBoxID.ToString();
//			m_DataModel.AfterTokenBoxB.nQuantity= 0;
//			if(bRet){
//				// 插入数据库（预留）
//				TokenBoxRemoveOperationRecord removeRecord;
//				removeRecord.dtOperationTime	= ComGetCurTime();
//				removeRecord.iOriginalQuantity	= 0;
//				removeRecord.iRemoveQuantity	= 0;
//				removeRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
//				removeRecord.txtTokenBoxID		= m_DataModel.BeforeTokenBoxB.strBoxID;
//				removeRecord.txtReserve			= _T("");
//				removeRecord.txtComment			= _T("");
//				DATA_HELPER->DbInsertTokenBoxRemoveOperationRecord(removeRecord);
//
//				// 上位数据lichao
//				MAGAZINE_CHANGE change;
//				change.operType =  DETACH;
//				change.magazineId.Deserialize(&beforUninstall.tokenBoxID.bLineCode);
//				change.magazineId.bLineCode = beforUninstall.tokenBoxID.bLineCode;
//				change.magazineId.bType = beforUninstall.tokenBoxID.bType;
//				change.magazineId.wNum = beforUninstall.tokenBoxID.wNum;
//				change.magazne =MAGAZINEB;
//				change.cardType =0;//UL卡类型
//				change.providerId = 0x01;
//				change.cardProductType = 0;
//				change.preProperty = 0x01;
//				change.childproductId = 0x01;
//				change.CardCount = 0x00;
//				change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//			
//				// 记录操作日志
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,true);
//			}
//
//			m_DataModel.AfterTokenBoxB.emStatus = STATUS_UNINSTALL;
//
//			// Token箱安装状态:已卸载
//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//
//			// Token箱数量状态:已空
//			theTVM_STATUS_MGR.SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
//
//			LOG("票箱B卸载成功！");
//		}
//		else{
//			// 卸载失败
//			m_DataModel.AfterTokenBoxB.emStatus = STATUS_UNINSTALL_ERROR;
//		}
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		// 卸载失败
//		m_DataModel.AfterTokenBoxB.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	catch(...){
//		// 卸载失败
//		m_DataModel.AfterTokenBoxB.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//
//	// 产生错误
//	if(m_DataModel.AfterTokenBoxB.emStatus == STATUS_UNINSTALL_ERROR){
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,false);
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      卸载废票箱
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoUninstallInvalidTokenBox(){
//	try{
//		if(RemoveInvalidTokenBox()){
//			CTTokenCountInfo::TOKEN_BOX_INFO tokenInfo,beforeInfo;
//			beforeInfo = theTOKEN_COUNT.GetTokenboxWastedInfo();
//
//			tokenInfo.tokenBoxID.bLineCode	= 0xFF;
//			tokenInfo.tokenBoxID.bType		= 0xFF;
//			tokenInfo.tokenBoxID.wNum		= 0xFFFF;
//			tokenInfo.ulCurCount = 0;
//			tokenInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//			tokenInfo.operateTime = ComGetCurTime();
//
//			m_DataModel.AfterInvalidTokenBox.strBoxID = _T("--------");
//			m_DataModel.AfterInvalidTokenBox.nQuantity = 0;
//			m_DataModel.AfterInvalidTokenBox.ulOperatorID = tokenInfo.ulOperatorID;
//			m_DataModel.AfterInvalidTokenBox.operateTime = tokenInfo.operateTime;
//
//			bool bRet = theTOKEN_COUNT.SetTokenboxWastedInfo(tokenInfo);
//
//			m_DataModel.AfterInvalidTokenBox.nQuantity = 0;
//			if(bRet){
//				// 插入数据库
//				TokenCollectBoxRemoveOperationRecord removeWastedInfo;
//				removeWastedInfo.dtTransTime	= ComGetCurTime();
//				removeWastedInfo.txtTokenBoxID	= beforeInfo.tokenBoxID.ToString();
//				removeWastedInfo.iQuantity		= beforeInfo.ulCurCount;
//				removeWastedInfo.txtOperatorID	= theAPP_SESSION.GetUserInfo().sUserID;
//				removeWastedInfo.txtComment		= _T("");
//				removeWastedInfo.txtReserve		= _T("");
//				try{
//					DATA_HELPER->DbInsertTokenCollectBoxRemoveOperationRecord(removeWastedInfo);
//				}
//				catch(CSysException& e){
//					theEXCEPTION_MGR.ProcessException(e);
//					m_DataModel.bInsertDataBaseSuccess = false;
//				}
//				catch(...){
//					m_DataModel.bInsertDataBaseSuccess = false;
//				}				
//
//				MAGAZINE_CHANGE change;
//				change.operType =  DETACH;
//				change.magazineId.Deserialize(&beforeInfo.tokenBoxID.bLineCode);
//				change.magazineId.bLineCode = beforeInfo.tokenBoxID.bLineCode;
//				change.magazineId.bType = beforeInfo.tokenBoxID.bType;
//				change.magazineId.wNum = beforeInfo.tokenBoxID.wNum;
//				change.magazne = MAGAZINEA;
//				change.cardType =0;//UL卡类型
//				change.providerId = 0x01;
//				change.cardProductType = 0;
//				change.preProperty = 0x01;
//				change.childproductId = 0x01;
//				change.CardCount = beforeInfo.ulCurCount;
//				change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//			
//				// 记录操作日志
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,true);
//			}
//
//			m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_UNINSTALL;
//
//			// 废票箱安装状态:卸载
//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//
//			// 废票箱数量状态:空
//			theTVM_STATUS_MGR.SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
//
//			LOG("废票箱卸载成功:%d！", beforeInfo.ulCurCount);
//		}
//		else
//			m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	catch(...){
//		m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	// 产生错误
//	if(m_DataModel.AfterInvalidTokenBox.emStatus == STATUS_UNINSTALL_ERROR){
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,false);
//	}
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      卸载废票箱
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTokenManageSvc::DoUninstallCollectedTokenBox(){
//	try{
//		if(1){
//			CTTokenCountInfo::TOKEN_BOX_INFO tokenInfo,beforeInfo;
//			beforeInfo = theTOKEN_COUNT.GetTokenboxCollectedInfo();
//
//			tokenInfo.tokenBoxID.bLineCode	= 0xFF;
//			tokenInfo.tokenBoxID.bType		= 0xFF;
//			tokenInfo.tokenBoxID.wNum		= 0xFFFF;
//			tokenInfo.ulCurCount = 0;
//			tokenInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
//			tokenInfo.operateTime = ComGetCurTime();
//
//			m_DataModel.AfterCollectedTokenBox.strBoxID = _T("--------");
//			m_DataModel.AfterCollectedTokenBox.nQuantity = 0;
//
//			bool bRet = theTOKEN_COUNT.SetTokenboxCollectedInfo(tokenInfo);
//
//			m_DataModel.AfterCollectedTokenBox.nQuantity = 0;
//			m_DataModel.AfterCollectedTokenBox.ulOperatorID = tokenInfo.ulOperatorID;
//			m_DataModel.AfterCollectedTokenBox.operateTime = tokenInfo.operateTime;
//
//			if(bRet){
//				// 插入数据库
//				TokenCollectBoxRemoveOperationRecord removeWastedInfo;
//				removeWastedInfo.dtTransTime	= ComGetCurTime();
//				removeWastedInfo.txtTokenBoxID	= beforeInfo.tokenBoxID.ToString();
//				removeWastedInfo.iQuantity		= beforeInfo.ulCurCount;
//				removeWastedInfo.txtOperatorID	= theAPP_SESSION.GetUserInfo().sUserID;
//				removeWastedInfo.txtComment		= _T("");
//				removeWastedInfo.txtReserve		= _T("");
//				try{
//					DATA_HELPER->DbInsertTokenCollectBoxRemoveOperationRecord(removeWastedInfo);
//				}
//				catch(CSysException& e){
//					theEXCEPTION_MGR.ProcessException(e);
//					m_DataModel.bInsertDataBaseSuccess = false;
//				}
//				catch(...){
//					m_DataModel.bInsertDataBaseSuccess = false;
//				}				
//
//				MAGAZINE_CHANGE change;
//				change.operType =  DETACH;
//				change.magazineId.Deserialize(&beforeInfo.tokenBoxID.bLineCode);
//				change.magazineId.bLineCode = beforeInfo.tokenBoxID.bLineCode;
//				change.magazineId.bType = beforeInfo.tokenBoxID.bType;
//				change.magazineId.wNum = beforeInfo.tokenBoxID.wNum;
//				change.magazne = MAGAZINEA;
//				change.cardType =0;//UL卡类型
//				change.providerId = 0x01;
//				change.cardProductType = 0;
//				change.preProperty = 0x01;
//				change.childproductId = 0x01;
//				change.CardCount = beforeInfo.ulCurCount;
//				change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
//
//				// 记录操作日志
//				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,true);
//			}
//
//			m_DataModel.AfterInvalidTokenBox.emStatus = STATUS_UNINSTALL;
//
//			// 废票箱安装状态:卸载
//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
//
//			// 废票箱数量状态:空
//			theTVM_STATUS_MGR.SetTokenCollectedBoxNumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
//
//			LOG("回收箱卸载成功:%d！", beforeInfo.ulCurCount);
//		}
//		else
//			m_DataModel.AfterCollectedTokenBox.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		m_DataModel.AfterCollectedTokenBox.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	catch(...){
//		m_DataModel.AfterCollectedTokenBox.emStatus = STATUS_UNINSTALL_ERROR;
//	}
//	// 产生错误
//	if(m_DataModel.AfterCollectedTokenBox.emStatus == STATUS_UNINSTALL_ERROR){
//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKENBOX_REMOVAL,false);
//	}
//}
//
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      打开TOKEN箱A的安全扣
//
//@param      无
//
//@retval     bool true:成功;false:失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::RemoveTokenBoxA(){
//	try{
//		//TOKEN_HELPER->RemoveMagazine(TOKEN_MAGAZINE::TOKEN_MAGAZINE_A);lichao
//		return true;
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//	catch(...){
//		return false;
//	}
//	return true;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      打开TOKEN箱B的安全扣
//
//@param      无
//
//@retval     bool true:成功;false:失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::RemoveTokenBoxB(){
//	try{
//		//TOKEN_HELPER->RemoveMagazine(TOKEN_MAGAZINE::TOKEN_MAGAZINE_B);lichao
//		return true;
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//	catch(...){
//		return false;
//	}
//	return true;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      打开废票箱的安全扣
//
//@param      无
//
//@retval     bool true:成功;false:失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::RemoveInvalidTokenBox(){
//	try{
//		// 不需要硬件动作移除
//		//TOKEN_HELPER->RemoveMagazine(TOKEN_MAGAZINE::TOKEN_MAGAZINE_C);lichao
//		return true;
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//		return false;
//	}
//	catch(...){
//		return false;
//	}
//	return true;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      打印操作
//
//@param      无
//
//@retval     bool true:成功;false:失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTokenManageSvc::PrintReciept(){
//	bool bRet = false;
//	try{
//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_TOKENMGR_SHOW_PRINTING));
//
//		CString strOperatorID_before = _T("");
//		CString strOperatorID_after = _T("");
//		CString strTicketCount_before = _T("");
//		CString strTicketCount_after = _T("");
//		// Token箱A
//		if(m_DataModel.AfterTokenBoxA.bIsSelected){
//			strOperatorID_before.Format(_T("%06d"), m_DataModel.BeforeTokenBoxA.ulOperatorID);
//			strOperatorID_after.Format(_T("%06d"), m_DataModel.AfterTokenBoxA.ulOperatorID);
//			strTicketCount_before.Format(_T("%d"),  m_DataModel.BeforeTokenBoxA.nQuantity);
//			strTicketCount_after.Format(_T("%d"), m_DataModel.AfterTokenBoxA.nQuantity);
//			// 加载Template
//			CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
//			CStringArray printtext;
//			CStringArray TemplateContents;
//			BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
//			int Index = 0;
//			while(Index<TemplateContents.GetCount())
//			{
//				CString& line = TemplateContents.ElementAt(Index);	
//				line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
//
//				if(GetCurDialogGroupID() == DIALOG_GROUP_INSTALL){
//					line.Replace(_T("{票箱}"), _T("票箱1"));
//					line.Replace(_T("{操作前票箱号}"), INVALID_ID);
//					line.Replace(_T("{操作后票箱号}"), m_DataModel.AfterTokenBoxA.strBoxID);
//					line.Replace(_T("{操作前数量}"), strTicketCount_after);
//					line.Replace(_T("{操作后数量}"), strTicketCount_before);//界面体现的是票箱票到hopper中的过程，凭条体现的是安装前后的数量编号，两者相反。
//					line.Replace(_T("{操作前操作员}"), strOperatorID_before);
//					line.Replace(_T("{操作后操作员}"), strOperatorID_after);
//					line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
//					line.Replace(_T("{操作前时间}"), m_DataModel.BeforeTokenBoxA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//					line.Replace(_T("{操作后时间}"), m_DataModel.AfterTokenBoxA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//					line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//				}
//				else if (GetCurDialogGroupID() == DIALOG_GROUP_UNINSTALL){
//					line.Replace(_T("{票箱}"), _T("票箱1"));
//					line.Replace(_T("{操作前票箱号}"), m_DataModel.BeforeTokenBoxA.strBoxID);
//					line.Replace(_T("{操作后票箱号}"), INVALID_ID);
//					line.Replace(_T("{操作前数量}"), strTicketCount_after);
//					line.Replace(_T("{操作后数量}"), strTicketCount_before);//界面体现的是票箱票到hopper中的过程，凭条体现的是安装前后的数量编号，两者相反。
//					line.Replace(_T("{操作前操作员}"), strOperatorID_before);
//					line.Replace(_T("{操作后操作员}"), strOperatorID_after);
//					line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
//					line.Replace(_T("{操作前时间}"), m_DataModel.BeforeTokenBoxA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//					line.Replace(_T("{操作后时间}"), m_DataModel.AfterTokenBoxA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//					line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//				}
//				else{
//					line.Replace(_T("{更换票箱}"), _T("回收票箱"));
//					line.Replace(_T("{票箱}"), _T("Hopper A"));
//					line.Replace(_T("{操作前票箱号}"), m_DataModel.BeforeTokenBoxA.strBoxID);
//					line.Replace(_T("{操作后票箱号}"), m_DataModel.AfterTokenBoxA.strBoxID);
//					line.Replace(_T("{操作前数量}"), strTicketCount_before);
//					line.Replace(_T("{操作后数量}"), strTicketCount_after);
//					line.Replace(_T("{操作前操作员}"), strOperatorID_before);
//					line.Replace(_T("{操作后操作员}"), theAPP_SESSION.GetUserInfo().sUserID);
//					line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
//					line.Replace(_T("{操作前时间}"), m_DataModel.BeforeTokenBoxA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//					line.Replace(_T("{操作后时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//					line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//				}
//
//				CStringArray receiptline;
//				ComPrintLineFormatForTemplate(line,receiptline);
//				printtext.Append(receiptline);
//				Index++;
//			}
//			// 开始打印
//			PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
//			Sleep(3000);//等待打印机完成打印
//		}
//
//		// Token箱B
//		if(m_DataModel.AfterTokenBoxB.bIsSelected){
//			strOperatorID_before.Format(_T("%06d"), m_DataModel.BeforeTokenBoxB.ulOperatorID);
//			strOperatorID_after.Format(_T("%06d"), m_DataModel.AfterTokenBoxB.ulOperatorID);
//			strTicketCount_before.Format(_T("%d"),  m_DataModel.BeforeTokenBoxB.nQuantity);
//			strTicketCount_after.Format(_T("%d"), m_DataModel.AfterTokenBoxB.nQuantity);
//			// 加载Template
//			CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
//			CStringArray printtext;
//			CStringArray TemplateContents;
//			BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
//			int Index = 0;
//			while(Index<TemplateContents.GetCount())
//			{
//				CString& line = TemplateContents.ElementAt(Index);	
//				line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
//				if(GetCurDialogGroupID() == DIALOG_GROUP_RECYCLE){
//					line.Replace(_T("{更换票箱}"), _T("回收票箱"));
//					line.Replace(_T("{票箱}"), _T("Hopper B"));
//				}
//				else{
//					line.Replace(_T("{票箱}"), _T("票箱2"));
//				}
//
//				if(GetCurDialogGroupID() == DIALOG_GROUP_INSTALL){
//					line.Replace(_T("{操作前票箱号}"), INVALID_ID);
//					line.Replace(_T("{操作后票箱号}"), m_DataModel.AfterTokenBoxB.strBoxID);
//					line.Replace(_T("{操作前数量}"), strTicketCount_after);
//					line.Replace(_T("{操作后数量}"), strTicketCount_before);//界面体现的是票箱票到hopper中的过程，凭条体现的是安装前后的数量编号，两者相反。
//				}
//				else if (GetCurDialogGroupID() == DIALOG_GROUP_UNINSTALL){
//					line.Replace(_T("{操作前票箱号}"), m_DataModel.BeforeTokenBoxB.strBoxID);
//					line.Replace(_T("{操作后票箱号}"), INVALID_ID);
//					line.Replace(_T("{操作前数量}"), strTicketCount_after);
//					line.Replace(_T("{操作后数量}"), strTicketCount_before);//界面体现的是票箱票到hopper中的过程，凭条体现的是安装前后的数量编号，两者相反。
//				}
//				else{
//					line.Replace(_T("{操作前票箱号}"), m_DataModel.BeforeTokenBoxB.strBoxID);
//					line.Replace(_T("{操作后票箱号}"), m_DataModel.AfterTokenBoxB.strBoxID);
//					line.Replace(_T("{操作前数量}"), strTicketCount_before);
//					line.Replace(_T("{操作后数量}"), strTicketCount_after);
//				}
//				line.Replace(_T("{操作前操作员}"), strOperatorID_before);
//				line.Replace(_T("{操作后操作员}"), strOperatorID_after);
//				line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
//				line.Replace(_T("{操作前时间}"), m_DataModel.BeforeTokenBoxB.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//				line.Replace(_T("{操作后时间}"), m_DataModel.AfterTokenBoxB.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//				line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//
//				CStringArray receiptline;
//				ComPrintLineFormatForTemplate(line,receiptline);
//				printtext.Append(receiptline);
//				Index++;
//			}
//			// 开始打印
//			PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
//			Sleep(3000);//等待打印机完成打印
//		}
//
//		// 废票箱
//		if(m_DataModel.AfterInvalidTokenBox.bIsSelected){
//			strOperatorID_before.Format(_T("%06d"), m_DataModel.BeforeInvalidTokenBox.ulOperatorID);
//			strOperatorID_after.Format(_T("%06d"), m_DataModel.AfterInvalidTokenBox.ulOperatorID);
//			strTicketCount_before.Format(_T("%d"),  m_DataModel.BeforeInvalidTokenBox.nQuantity);
//			strTicketCount_after.Format(_T("%d"), m_DataModel.AfterInvalidTokenBox.nQuantity);
//			// 加载Template
//			CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
//			CStringArray printtext;
//			CStringArray TemplateContents;
//			BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
//			int Index = 0;
//			while(Index<TemplateContents.GetCount())
//			{
//				CString& line = TemplateContents.ElementAt(Index);	
//				line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
//				line.Replace(_T("{票箱}"), _T("废票箱"));
//				line.Replace(_T("{操作前票箱号}"), m_DataModel.BeforeInvalidTokenBox.strBoxID);
//				line.Replace(_T("{操作后票箱号}"), m_DataModel.AfterInvalidTokenBox.strBoxID);
//				line.Replace(_T("{操作前操作员}"), strOperatorID_before);
//				line.Replace(_T("{操作后操作员}"), strOperatorID_after);
//				line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
//				line.Replace(_T("{操作前时间}"), m_DataModel.BeforeInvalidTokenBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//				line.Replace(_T("{操作后时间}"), m_DataModel.AfterInvalidTokenBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//				line.Replace(_T("{操作前数量}"), strTicketCount_before);
//				line.Replace(_T("{操作后数量}"), strTicketCount_after);
//				line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//
//				CStringArray receiptline;
//				ComPrintLineFormatForTemplate(line,receiptline);
//				printtext.Append(receiptline);
//				Index++;
//			}
//			// 开始打印
//			PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
//			Sleep(3000);//等待打印机完成打印
//		}
//
//		// 回收箱
//		if(m_DataModel.AfterCollectedTokenBox.bIsSelected){
//			strOperatorID_before.Format(_T("%06d"), m_DataModel.BeforeCollectedTokenBox.ulOperatorID);
//			strOperatorID_after.Format(_T("%06d"), m_DataModel.AfterCollectedTokenBox.ulOperatorID);
//			strTicketCount_before.Format(_T("%d"),  m_DataModel.BeforeCollectedTokenBox.nQuantity);
//			strTicketCount_after.Format(_T("%d"), m_DataModel.AfterCollectedTokenBox.nQuantity);
//			// 加载Template
//			CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
//			CStringArray printtext;
//			CStringArray TemplateContents;
//			BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
//			int Index = 0;
//			while(Index<TemplateContents.GetCount())
//			{
//				CString& line = TemplateContents.ElementAt(Index);	
//				line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
//				line.Replace(_T("{票箱}"), _T("回收箱"));
//				line.Replace(_T("{操作前票箱号}"), m_DataModel.BeforeCollectedTokenBox.strBoxID);
//				line.Replace(_T("{操作后票箱号}"), m_DataModel.AfterCollectedTokenBox.strBoxID);
//				line.Replace(_T("{操作前操作员}"), strOperatorID_before);
//				line.Replace(_T("{操作后操作员}"), strOperatorID_after);
//				line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
//				line.Replace(_T("{操作前时间}"), m_DataModel.BeforeCollectedTokenBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//				line.Replace(_T("{操作后时间}"), m_DataModel.AfterCollectedTokenBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
//				line.Replace(_T("{操作前数量}"), strTicketCount_before);
//				line.Replace(_T("{操作后数量}"), strTicketCount_after);
//				line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
//
//				CStringArray receiptline;
//				ComPrintLineFormatForTemplate(line,receiptline);
//				printtext.Append(receiptline);
//				Index++;
//			}
//			// 开始打印
//			PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
//			Sleep(3000);//等待打印机完成打印
//		}
//
//		bRet = true;
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch(...){
//
//	}
//
//	return bRet;
//}
//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      将FFFFFFFF转换为--------
//
//@param      CString boxID;
//
//@retval     转换后的ID
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTokenManageSvc::TransforPrintID(CString boxID){
//	CString strId;
//	strId = PRINT_ID;
//	if(boxID.CompareNoCase(INVALID_ID) != 0)
//		strId	= boxID;
//	return strId;
//}