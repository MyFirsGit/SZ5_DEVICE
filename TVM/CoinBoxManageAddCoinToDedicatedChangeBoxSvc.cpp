#include "stdafx.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxSvc.h"
#include "CoinBoxManageAddCoinToDedicatedChangeBoxDlg.h"
#include "CTCoinCountInfo.h"
#include "TemplateHelper.h"
#include "MainSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INTERNAL_ERROR(errCode) \
{if (COIN_HELPER->IsExceptionCode(errCode)) {\
	throw CCHException(errCode, _T(__FILE__), __LINE__);\
}else\
{throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);}}


BEGIN_MESSAGE_MAP(CCoinChangeBoxAddingSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinChangeBoxAddingSvc::CCoinChangeBoxAddingSvc()
	:CTVMForeService(ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG, new CCoinBoxManageAddCoinToDedicatedChangeBoxDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DEDICATEDCHANGE_BOX,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DEDICATEDCHANGE_BOX,IDD_20112_STATION_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DEDICATEDCHANGE_BOX);

	m_phase = PHASE_SELECT_BOX;
	InitializeDataModel();

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinChangeBoxAddingSvc::~CCoinChangeBoxAddingSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	
	
	//// 同步计数，如果需要的话。
	//if(!COIN_HELPER->SyncCoinCountInfoToFile())
	//	m_DataModel.IsHasException = false;

	// 操作之前各个箱子的数据保存
	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	m_phase = PHASE_SELECT_BOX;
	InitializeDataModel();
	////////////////////////////////////////////////////////////////

	EnterTime = 1;
	SecondaryMenuNumber = -1;
	AddingBoxNumber = -1;
	m_DataModel.BoxNumber = 12;
	IsTipsShow = true;// 需要显示提示
	IsShow = false;

	
	//DedicatedChangeHandleData();

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DEDICATEDCHANGE_BOX);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_DEDICATEDCHANGE_BOX);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::InitializeDataModel()
{
	m_DataModel.addingboxdata.AddingBoxFrameFlowTime = 1;
	m_DataModel.mapBoxInfo.clear();
	map<CHANGE_BOX_INDEX,CHANGE_BOX_INFO> &mapBox = m_DataModel.mapBoxInfo;	 // 使代码简洁 避免每次都使用m_DataModel.mapBoxInfo
	CHANGE_BOX_INFO boxInfo;
	mapBox[CHANGE_BOX_A] = boxInfo;
	mapBox[CHANGE_BOX_B] = boxInfo;
	m_DataModel.opStatus = OP_STATUS_UNKNOW;
	//CTCoinCountInfo::COIN_CHANGE_BOX_INFO ctBoxInfo;
	CTCoinCountInfo::COIN_BOX_INFO ctBoxInfo;
	m_DataModel.IsPrintFailed = FALSE;
	// 读取硬件状态
	try
	{
		tCHADevStatus rsp;
		if(!(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID))){
			COIN_HELPER->CH_GetStatus(&rsp);
			m_DataModel.IsHasException = FALSE;
		}
		else{
			m_DataModel.IsHasException = true;
		}
		if (rsp.bIsChangeBoxAInstall){	// 如果找零箱A在位
			//ctBoxInfo = theCOIN_COUNT.GetChangeCoinboxAInfo();
			ctBoxInfo = theCOIN_COUNT.GetCoinHopperboxAInfo();
			mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
			mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

			boxInfo.GetCoinChangeCountInfo(ctBoxInfo);	// 从软计数中读取到所需信息
			boxInfo.value =TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
			boxInfo.ExCount = ctBoxInfo.ulCurCount;	// 加币前的数量
			mapBox[CHANGE_BOX_A] = boxInfo;
		}
		if (rsp.bIsChangeBoxBInstall){	// 如果找零箱B在位
			boxInfo = CHANGE_BOX_INFO();
			//ctBoxInfo = theCOIN_COUNT.GetChangeCoinboxBInfo();
			ctBoxInfo = theCOIN_COUNT.GetCoinHopperboxBInfo();
			boxInfo.GetCoinChangeCountInfo(ctBoxInfo);
			boxInfo.value = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());;
			boxInfo.ExCount = ctBoxInfo.ulCurCount;
			mapBox[CHANGE_BOX_B] = boxInfo;
		}
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsHasException = TRUE;
	}
	catch(...){
		m_DataModel.IsHasException = TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinChangeBoxAddingSvc::Model& CCoinChangeBoxAddingSvc::GetDataModel()
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
void CCoinChangeBoxAddingSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
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
LRESULT CCoinChangeBoxAddingSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 提示有异常
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}

	try
	{
		switch(m_phase)
		{
		case PHASE_SELECT_BOX:
			{
				int input = theAPP_SESSION.GetInputToInt();
				if(input > 2 || input < 0){
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
					return 0;
				}else{
					if (IsBoxCouldBeAdded((CHANGE_BOX_INDEX)theAPP_SESSION.GetInputToInt())){
						m_index = (CHANGE_BOX_INDEX)input;
						ToAddingCoinPhase();
						theAPP_SESSION.SetMaxInputLength(3);
						if (input == 1){
							theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_DEDICATED_COINA_SET_COUNT));
						}else{
							theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_DEDICATED_COINB_SET_COUNT));
						}
						
					}else{
						//theAPP_SESSION.ShowOfficeGuide
					}
				}
				
			}break;
		case PHASE_ADDING_COIN:
			{
				int nCount = theAPP_SESSION.GetInputToInt();int nMaxInput = 0;
				if(m_index == CHANGE_BOX_A){
					//nMaxInput = abs(theTVM_INFO.GetCHChangeBoxAMax() - theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount);
					nMaxInput = abs(theTVM_INFO.GetCHHopper1MaxCount() - theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount);				
				}
				else{
					//nMaxInput = abs(theTVM_INFO.GetCHChangeBoxBMax() - theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount);
					nMaxInput = abs(theTVM_INFO.GetCHHopper2MaxCount() - theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount);	
				}
				//判断输入的数量是否合法
				if (0 <nCount && nCount <= nMaxInput){
					if (m_index == CHANGE_BOX_A){
						m_DataModel.mapBoxInfo[CHANGE_BOX_A].AddCoin(theAPP_SESSION.GetInputToInt());	// 更新数量
					}
					if (m_index == CHANGE_BOX_B){
						m_DataModel.mapBoxInfo[CHANGE_BOX_B].AddCoin(theAPP_SESSION.GetInputToInt());
					}
					// 开始准备接收
					m_DataModel.opStatus = OP_STATUS_READY_ADDING;
					ToSelectBoxPhase();
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				}
				else{
					ToAddingCoinPhase();
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ERROR_INPUT));
				}
				
			}break;
		default:
			break;
		}
		NotifyDataModelChanged();
	}
	// 异常处理
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.addingboxdata.AddingBoxFrameFlowTime = FirstFrame;
		NotifyDataModelChanged();
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		m_DataModel.addingboxdata.AddingBoxFrameFlowTime = FirstFrame;
		NotifyDataModelChanged();
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取硬币补充箱币种


@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinChangeBoxAddingSvc::GetHopperCoinType(int hopetype)
{
	CString CurrencyType = _T("");
	switch(hopetype)		// 从TVM.INI文件中读取的键值
	{
	case MENU_CODE_0:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//多币种
		break;
	case MENU_CODE_1:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//50AVOS
		break;
	case MENU_CODE_2:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_OMOP);		//1MOP
		break;
	case MENU_CODE_3:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_SMOP);		//2MOP
		break;
	case MENU_CODE_4:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FMOP);		//5MOP
		break;
	default:
		break;
	}
	return CurrencyType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取硬币专用找零箱硬币类型


@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CCoinChangeBoxAddingSvc::GetHopperType(int hopetype)
{
	BYTE CurrencyType = 0;
	switch(hopetype)		// 从TVM.INI文件中读取的键值
	{
	case MENU_CODE_1:
		CurrencyType = 4;		//50AVOS
		break;
	case MENU_CODE_2:
		CurrencyType = 1;		//1MOP
		break;
	case MENU_CODE_3:
		CurrencyType = 2;		//2MOP
		break;
	case MENU_CODE_4:
		CurrencyType = 3;		//5MOP
		break;
	default:
		break;
	}
	return CurrencyType;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      硬币专用找零箱处理ID、数量信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::DedicatedChangeHandleData()
{

	//try
	//{
	//	LONG count = -1;

	//	COIN_DATA coindata;		// 硬币专用找零箱A、B枚数

	//	m_DataModel.dedicatedchangeboxdata.DedicatedChangeCoinAID = theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();		// 硬币专用找零箱AID
	//	m_DataModel.dedicatedchangeboxdata.DedicatedChangeCoinBID = theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();		// 硬币专用找零箱BID

	//	m_DataModel.addingboxdata.AddingBoxATypeData = GetHopperCoinType(theTVM_INFO.GetCHHopper1Type());							// 硬币补充箱A的面值
	//	m_DataModel.addingboxdata.AddingBoxBTypeData = GetHopperCoinType(theTVM_INFO.GetCHHopper2Type());							// 硬币补充箱B的面值

	//	// 读硬币专用找零箱A、B枚数
	//	COIN_HELPER->CoinDataRead(coindata);

	//	m_DataModel.dedicatedchangeboxdata.QuantityA.Format(_T("%ld"),coindata.hopper1_num);										// 硬币专用找零箱A数量										

	//	m_DataModel.dedicatedchangeboxdata.QuantityB.Format(_T("%ld"),coindata.hopper2_num);										// 硬币专用找零箱B数量

	//	CheckCoinAddingBoxIsInstalled(m_DataModel.BoxNumber);																		// 检测硬币补充箱是否安装																										
	//	GetCoinAddingBoxID(m_DataModel.BoxNumber);																					// 获取硬币补充箱ID
	//	m_DataModel.addingboxdata.AddingBoxFrameFlowTime = FirstFrame;	
	//	NotifyDataModelChanged();
	//}
	//// 异常处理
	//catch (CSysException& e) {
	//	m_DataModel.IsHasException = TRUE;
	//	NotifyDataModelChanged();
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	m_DataModel.IsHasException = TRUE;
	//	NotifyDataModelChanged();
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      专用找零箱加币界面处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::DedicatedChangeAddingCoinFrame()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      专用找零箱加币处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::DoAddingCoinToDedicatedChangeBox()
{
	int count = theAPP_SESSION.GetInputToInt();				// 获取向导栏输入内容
	int MaxAddACount = -1;
	switch(SecondaryMenuNumber)
	{
		// 硬币专用找零箱A加币
	case MENU_CODE_1:
		MaxAddACount = theTVM_INFO.GetCHHopper1MaxCount() - theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount ;	// HOPPER1一次补币最大枚数
		// 补充数量非法
		if(count < 1 || count > MaxAddACount || MaxAddACount < 0)	
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDING_NUMBER_ERROR)); 
		}
		//
		else{
			m_DataModel.addingboxdata.AddingBoxQuantityA.Format(_T("%d"),count);
			NotifyDataModelChanged();
			m_DataModel.doaddingcoin.DoAddingQuantityA = count;
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			EnterTime = 1;
		}
		break;
		// 硬币专用找零箱B加币
	case MENU_CODE_2:
		MaxAddACount = theTVM_INFO.GetCHHopper2MaxCount() - theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;	// HOPPER2一次补币最大枚数 
		if(count < 1 || count > MaxAddACount || MaxAddACount < 0)	// 补充数量非法
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDING_NUMBER_ERROR)); 
		}
		else
		{
			m_DataModel.addingboxdata.AddingBoxQuantityB.Format(_T("%d"),count);
			NotifyDataModelChanged();
			m_DataModel.doaddingcoin.DoAddingQuantityB = count;
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			EnterTime = 1;
		}
		break;
	default:
		break;
	}



}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息（保存）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinChangeBoxAddingSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	try{
		// 提示有异常
		if(m_DataModel.IsHasException){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
			return FALSE;
		}

		// 初始化状态不响应F2
		if(m_DataModel.opStatus == OP_STATUS_UNKNOW || m_DataModel.opStatus == OP_STATUS_FINISH_ADDING){
			return false;
		}

		// 状态为准备加币
		if(m_DataModel.opStatus == OP_STATUS_READY_ADDING){
			m_DataModel.opStatus = OP_STATUS_RUNING_ADDING;
			// 显示加币提示
			IsShow = true;
			theSERVICE_MGR.SetForeServiceBusy(true);
			NotifyDataModelChanged();
			return 0;
		}

		// 解除忙碌状态
		theSERVICE_MGR.SetForeServiceBusy(false);

		// 状态为正在加币中
		if(m_DataModel.opStatus == OP_STATUS_RUNING_ADDING){
			m_DataModel.opStatus = OP_STATUS_FINISH_ADDING;
			CString strMsg(_T(""));
			bool bBoxAAdd = false,bBoxBAdd = false;
			if (m_DataModel.mapBoxInfo[CHANGE_BOX_A].AddingCount > 0){
				//CTCoinCountInfo::COIN_CHANGE_BOX_INFO boxInfo =  theCOIN_COUNT.GetChangeCoinboxAInfo();
				CTCoinCountInfo::COIN_BOX_INFO boxInfo =  theCOIN_COUNT.GetCoinHopperboxAInfo();
				boxInfo.ulCurCount += m_DataModel.mapBoxInfo[CHANGE_BOX_A].AddingCount;
				boxInfo.operateTime = ComGetCurTime();
				boxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
				m_DataModel.mapBoxInfo[CHANGE_BOX_A].CurrentCount = boxInfo.ulCurCount;

				//if (theCOIN_COUNT.SetChangeCoinboxAInfo(boxInfo)){
				if (theCOIN_COUNT.SetCoinHopperboxAInfo(boxInfo)){
					strMsg += _opl(TXT_COINMGR_ADDCOIN_CYCLE_CHANGE_BOXA_SUCCESS);//theAPP_SESSION.ShowOfficeGuide(_T("找零箱A补币成功"));
				}
				else{
					strMsg += _opl(TXT_COINMGR_ADDCOIN_CYCLE_CHANGE_BOXA_FAILED);//theAPP_SESSION.ShowOfficeGuide(_T("找零箱A补币失败"));
				}
				InsertDataToDB(CHANGE_BOX_A);
				//Sendcables(CHANGE_BOX_A);

				LOG("硬币hopper A加币%d个成功！加币后：%d", m_DataModel.mapBoxInfo[CHANGE_BOX_A].AddingCount, boxInfo.ulCurCount);
				m_DataModel.addingboxdata.AddingBoxFrameFlowTime = 3;
				bBoxAAdd = true;
				PrintCoinBoxExchangeReceipt(true);
			}
			if (m_DataModel.mapBoxInfo[CHANGE_BOX_B].AddingCount > 0){
				//CTCoinCountInfo::COIN_CHANGE_BOX_INFO boxInfo =  theCOIN_COUNT.GetChangeCoinboxBInfo();
				CTCoinCountInfo::COIN_BOX_INFO boxInfo =  theCOIN_COUNT.GetCoinHopperboxBInfo();
				boxInfo.ulCurCount += m_DataModel.mapBoxInfo[CHANGE_BOX_B].AddingCount;
				boxInfo.operateTime = ComGetCurTime();
				boxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
				m_DataModel.mapBoxInfo[CHANGE_BOX_B].CurrentCount = boxInfo.ulCurCount;

				//if (theCOIN_COUNT.SetChangeCoinboxBInfo(boxInfo)){
				if (theCOIN_COUNT.SetCoinHopperboxBInfo(boxInfo)){
					strMsg += _T("循环找零箱A加币成功");//_opl(TXT_COINMGR_ADDCOIN_CYCLE_CHANGE_BOXB_SUCCESS);//theAPP_SESSION.ShowOfficeGuide(_T("找零箱B补币成功"));
				}
				else{
					strMsg += _T("循环找零箱B加币成功");//_opl(TXT_COINMGR_ADDCOIN_CYCLE_CHANGE_BOXB_FAILED);//theAPP_SESSION.ShowOfficeGuide(_T("找零箱B补币失败"));
				}
				InsertDataToDB(CHANGE_BOX_B);
				//Sendcables(CHANGE_BOX_B);
				LOG("硬币hopper B加币%d个成功！加币后：%d", m_DataModel.mapBoxInfo[CHANGE_BOX_B].AddingCount, boxInfo.ulCurCount);
				m_DataModel.addingboxdata.AddingBoxFrameFlowTime = 3;
				bBoxBAdd = true;
				PrintCoinBoxExchangeReceipt(false);
			}
			if (bBoxAAdd || bBoxBAdd){

				// 操作之前各个箱子的数据保存
				mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
				mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

				mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
				mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

				mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
				mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();
				strMsg += _opl(GUIDE_F3_ESC);

				theAPP_SESSION.ShowOfficeGuide(strMsg);

				// 提示打印失败
				if(m_DataModel.IsPrintFailed){
					Sleep(1000);
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));
				}
				NotifyDataModelChanged();
				IsShow = false;

				// 同步计数
				COIN_HELPER->CH_SetCoinNum();
			}
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));
	}
	catch(...){
		// Do nothing
	}
	
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Clear] 键按下消息（删除）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinChangeBoxAddingSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;		// 置按键次数为1，重新从选择菜单编号开始
		theAPP_SESSION.SetMaxInputLength(1);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinChangeBoxAddingSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	InitializeDataModel();
	theSERVICE_MGR.SetForeServiceBusy(false);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [ESC] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinChangeBoxAddingSvc::OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardEsc(0,0);
	theSERVICE_MGR.SetForeServiceBusy(false);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上传电文

@param      int boxnumber

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::Sendcables(CHANGE_BOX_INDEX boxnumber)
{
	BYTE line = 0;
	BYTE Type = 0;
	WORD wNum = 0;
	CString coindedicatedboxid;
	CASHBOX_CHANGE  coinbox;
	
	CASHBOX_CHANGE	cash_change;
	cash_change.handleType = INSTALL;//ADD;定义类型中没有补充，暂定为安装
	vector<CASH_TYPE_COUNT>  vecCash;
	switch(boxnumber)
	{
	case CHANGE_BOX_A:
		{
			CHANGE_BOX_INFO &box = m_DataModel.mapBoxInfo[CHANGE_BOX_A];
			line = mEx_change_A.coinBoxID.bLineCode;
			Type = mEx_change_A.coinBoxID.bType;
			wNum = mEx_change_A.coinBoxID.bNum;
			cash_change.SetBoxID(line,Type,mEx_change_A.coinBoxID.bNumInQu,wNum);
			cash_change.position = MAGAZINEC;
			cash_change.State	 = VALID_ATTACH;
			cash_change.btCount = 1;
			CASH_TYPE_COUNT coin;
			coin.cash_type = box.value;	// 面额
			coin.cash_count= box.AddingCount; // 数量
			vecCash.push_back(coin);
			cash_change.vecCash = vecCash;
		}break;
	case CHANGE_BOX_B:
		{
			CHANGE_BOX_INFO &box = m_DataModel.mapBoxInfo[CHANGE_BOX_B];
			line = mEx_change_B.coinBoxID.bLineCode;
			Type = mEx_change_B.coinBoxID.bType;
			wNum = mEx_change_B.coinBoxID.bNum;
			cash_change.SetBoxID(line,Type,mEx_change_B.coinBoxID.bNumInQu,wNum);
			cash_change.position = MAGAZINEB;
			cash_change.State	 = VALID_ATTACH;
			cash_change.btCount = 1;
			CASH_TYPE_COUNT coin;
			coin.cash_type = box.value;	// 面额
			coin.cash_count= box.AddingCount; // 数量
			vecCash.push_back(coin);
			cash_change.vecCash = vecCash;
		}break;
	default:
		break;
	}
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(cash_change);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打包币种代码、数量

@param      int hopetype	count	

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
vector<CASH_TYPE_COUNT> CCoinChangeBoxAddingSvc::PackageCoinTypeAndCount(int hopetype,int count)
{
	CASH_TYPE_COUNT cointypecount;
	vector<CASH_TYPE_COUNT> vecCash;
	switch(hopetype)
	{
		/*case coin_50avos:
		cointypecount.cash_type = Coin50AVOS;
		cointypecount.cash_count = count;
		vecCash.push_back(cointypecount);
		break;
		case coin_1mop:
		cointypecount.cash_type = Coin1MOP;
		cointypecount.cash_count = count;
		vecCash.push_back(cointypecount);
		break;
		case coin_2mop:
		cointypecount.cash_type = Coin2MOP;
		cointypecount.cash_count = count;
		vecCash.push_back(cointypecount);
		break;
		case coin_5mop:
		cointypecount.cash_type = Coin5MOP;
		cointypecount.cash_count = count;
		vecCash.push_back(cointypecount);
		break;*/
	}
	return vecCash;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币专用找零箱加币凭证打印

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::PrintCoinBoxExchangeReceipt(bool bBoxA)
{
	try
	{
		//CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
		//CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

		CTCoinCountInfo::COIN_BOX_INFO hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
		CTCoinCountInfo::COIN_BOX_INFO hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

		//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();


		CTCoinCountInfo::COIN_BOX_INFO operate_hopper_before;
		CTCoinCountInfo::COIN_BOX_INFO operate_hopper_after;
		if (bBoxA){
			operate_hopper_before = mEx_hopper_A;
			operate_hopper_after = hopper_A;
		}
		else{
			operate_hopper_before = mEx_hopper_B;
			operate_hopper_after = hopper_B;
		}

		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), operate_hopper_before.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), operate_hopper_after.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"),  operate_hopper_before.ulCurCount);
		strFace1Count_after.Format(_T("%d"), operate_hopper_after.ulCurCount);
		// 加载Template
		CString CoinBox_templatename = _T("TVM_COIN_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
			line.Replace(_T("{钱箱}"), _T("1元硬币备用找零箱"));
			line.Replace(_T("{操作前钱箱号}"), operate_hopper_before.coinBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), operate_hopper_after.coinBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), operate_hopper_before.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), operate_hopper_after.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_SUPPLY);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsPrintFailed = TRUE;
		//throw;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.IsPrintFailed = TRUE;
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      数据导入数据库

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinChangeBoxAddingSvc::InsertDataToDB(CHANGE_BOX_INDEX boxnumber)
{
	try
	{
		CoinAddOperationRecord addcoinrecord;
		addcoinrecord.dtOperationTime = ComGetCurTime();		// 操作时间
		if(CHANGE_BOX_A == boxnumber)
		{
			CHANGE_BOX_INFO &box_info = m_DataModel.mapBoxInfo[CHANGE_BOX_A];
			addcoinrecord.txtCoinAddBoxID = INVALID_ID;							// 补充箱编号	郑州没有补充箱
			addcoinrecord.txtCoinChangeBoxID = box_info.strID;						// 找零箱编号
			addcoinrecord.iCoinTypeCode	= TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
			addcoinrecord.iQuantityBeforeAdd = box_info.ExCount;					// 补充前数量	
			addcoinrecord.iQuantityAdd = box_info.AddingCount;						// 补充数量
			addcoinrecord.iQuantityAfterAdd = box_info.CurrentCount;				// 补充后数量
		}
		else if(CHANGE_BOX_B == boxnumber)
		{
			CHANGE_BOX_INFO &box_info = m_DataModel.mapBoxInfo[CHANGE_BOX_B];
			addcoinrecord.txtCoinAddBoxID = INVALID_ID;						// 补充箱编号		
			addcoinrecord.txtCoinChangeBoxID = box_info.strID;						// 找零箱编号
			// 币种
			addcoinrecord.iCoinTypeCode	= TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
			addcoinrecord.iQuantityBeforeAdd = box_info.ExCount;					// 补充前数量	
			addcoinrecord.iQuantityAdd = box_info.AddingCount;						// 补充数量
			addcoinrecord.iQuantityAfterAdd = box_info.CurrentCount;				// 补充后数量
		}
		addcoinrecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;			// 操作员ID

		DATA_HELPER->DbInsertCoinAddOperationRecord(addcoinrecord);
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

}


// 判断所选的找零箱是否可加币
bool CCoinChangeBoxAddingSvc::IsBoxCouldBeAdded(const CHANGE_BOX_INDEX box)
{
	try{
		bool could = false;
		tCHADevStatus rsp;
		COIN_HELPER->CH_GetStatus(&rsp);

		switch(box)
		{
		case CHANGE_BOX_A:
			{
				if (rsp.bIsChangeBoxAInstall){ // 箱子在位
					CTCoinCountInfo::COIN_CHANGE_BOX_INFO boxInfo = theCOIN_COUNT.GetChangeCoinboxAInfo();
					if (boxInfo.coinBoxID.ToString().Compare(_T("FFFFFFFF")) != 0){ // 箱子处于正常安装状态
						could = true;
					}else{ // 软计数中箱子未安装
						theAPP_SESSION.ShowOfficeGuide(_T("硬币找零箱A未安装，请安装。"));
						could = false;
					}
				}else{ // 箱子不在位
					could = false;
				}
			}break;
		case CHANGE_BOX_B:
			{
				if (rsp.bIsChangeBoxBInstall){ // 箱子在位
					CTCoinCountInfo::COIN_CHANGE_BOX_INFO boxInfo = theCOIN_COUNT.GetChangeCoinboxBInfo();
					if (boxInfo.coinBoxID.ToString().Compare(_T("FFFFFFFF")) != 0){ // 箱子处于正常安装状态
						could = true;
					}else{ // 软计数中箱子未安装
						theAPP_SESSION.ShowOfficeGuide(_T("硬币找零箱B未安装，请安装。"));
						could = false;
					}
				}else{ // 箱子不在位
					could = false;
				}
			}break;

		default:
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ERROR_INPUT));
			break;
		}
		return could;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...){
		return false;
	}
}

void CCoinChangeBoxAddingSvc::ToSelectBoxPhase()
{
	m_phase	= PHASE_SELECT_BOX;
}

void CCoinChangeBoxAddingSvc::ToAddingCoinPhase()
{
	m_phase = PHASE_ADDING_COIN;
}

// 是否要显示加币提示
bool CCoinChangeBoxAddingSvc::IsShowTips()
{
	return IsShow;
}