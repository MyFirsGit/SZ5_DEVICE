#include "stdafx.h"
#include "CoinBoxManageAddCoinToCycleChangeBoxDlg.h"
#include "CoinBoxManageAddCoinToCycleChangeBoxSvc.h"
#include "CTCoinCountInfo.h"
#include "TemplateHelper.h"

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


BEGIN_MESSAGE_MAP(CCoinBoxManageAddCoinToCycleChangeBoxSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageAddCoinToCycleChangeBoxSvc::CCoinBoxManageAddCoinToCycleChangeBoxSvc()
	:CTVMForeService(ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CCoinBoxManageAddCoinToCycleChangeBoxDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CYCLECHANGE_BOX,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CYCLECHANGE_BOX,IDD_20113_STATION_SET_END_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_CYCLECHANGE_BOX);
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageAddCoinToCycleChangeBoxSvc::~CCoinBoxManageAddCoinToCycleChangeBoxSvc()
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
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);	// 启用指定按键
	m_work_status = 0;

	EnterTime = 1;
	AddingCount = 0;
	Stop_Insert = 0;

	InitializeDataModel();
	CycleChangeHandleData();

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_CYCLECHANGE_BOX);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_CYCLECHANGE_BOX);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Service结束处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CCoinBoxManageAddCoinToCycleChangeBoxSvc::OnStop(){
//	// 必须停止读取
//	if(m_work_status & WORK_STATUS_READ_AMOUNT){
//		COIN_HELPER->Chp_Stop_Insert_Info();
//	}
//	return true;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::InitializeDataModel()
{
	/*m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxID = _T("");
	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxTye = _T("");
	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity = _T("0");
	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity = _T("0");
	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAfterAddingQuantity = _T("0");
	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = -1;
	m_DataModel.cyclechangeboxdata.nAddingCoinCount = 0;*/
	m_DataModel.cyclechangeboxdata = _CycleChangeBoxData();

	m_DataModel.IsHasException = FALSE;
	m_DataModel.IsPrinterSuccessfull = TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageAddCoinToCycleChangeBoxSvc::Model& CCoinBoxManageAddCoinToCycleChangeBoxSvc::GetDataModel()
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
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::NotifyDataModelChanged()
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
LRESULT CCoinBoxManageAddCoinToCycleChangeBoxSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	try
	{
		// 开始接收硬币处理
		if(1 == EnterTime)
		{
			DoCoinAdding();
		}
		// 停止、取消加币处理
		else if(2 == EnterTime)
		{
			StopCancleCoinAdding();
			//theSERVICE_MGR.SetForeServiceBusy(false);
		}
	}
	// 异常处理
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);//设置不为忙碌状态，不忙碌状态开始计时器
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = FirstFrame;
		NotifyDataModelChanged();
		EnterTime = 1;
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = FirstFrame;
		NotifyDataModelChanged();
		EnterTime = 1;
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取硬币循环找零箱币种

@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinBoxManageAddCoinToCycleChangeBoxSvc::GetHopperCoinType(int hopetype)
{
	CString CurrencyType = _T("");
	switch(hopetype)									// 从TVM.INI文件中读取的键值
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
@brief      硬币专用找零箱处理ID、数量信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::CycleChangeHandleData()
{
	//try
	//{
	//	LONG count = -1;

	//	COIN_DATA coindata;		// 硬币循环找零箱枚数

	//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxID = theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.ToString();	// 硬币循环找零箱ID
	//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxTye = GetHopperCoinType(theTVM_INFO.GetCHStockMoneyType());		// 硬币循环找零箱的面值

	//	// 读硬币循环找零箱枚数
	//	COIN_HELPER->CoinDataRead(coindata);

	//	m_DataModel.cyclechangeboxdata.nBeforeAddingStockCount = coindata.stock1_num;										// 小Hopper加币前数量
	//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity.Format(_T("%ld"),coindata.stock1_num);					// 硬币循环找零箱数量

	//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = FirstFrame;
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
@brief      开始接收硬币处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::DoCoinAdding()
{
	//int index = theAPP_SESSION.GetInputToInt();				// 获取向导栏输入内容
	//LONG count = -1;
	//long errCode = 0;
	//CCHReadReceivs* chReadReceiveAmount;

	//if(1 != index)
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
	//	return;
	//}
	//else
	//{
	//	
	//	count = _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity);
	//	// 硬币循环找零现有枚数大于120，不可以加币
	//	if(count >= theTVM_INFO.GetCHStock1Max())
	//	{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_FULL));
	//		return;
	//	}
	//	// 硬币循环找零箱现有枚数小于120，可以加币
	//	else
	//	{
	//		m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = SecondFrame;
	//		NotifyDataModelChanged();
	//		
	//		EnterTime = 2;
	//		//__super::OnBusy(true);
	//		theSERVICE_MGR.SetForeServiceBusy(true);
	//		errCode = COIN_HELPER->Chp_EnableAndGetInsertInfoForCoinManage(this);			// 开始接收硬币	
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_ADDING_COIN));
	//		if(E_NO_CH_ERROR != errCode)	// 错误代码不为0
	//		{
	//			INTERNAL_ERROR(errCode);	// 判断是硬件异常、软件异常分别处理
	//		}	
	//		else{//设置运行状态
	//			m_work_status = WORK_STATUS_ACCEPT_COIN | WORK_STATUS_READ_AMOUNT;
	//		}
	//	}
	//	
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      接收到硬币回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::OnCoinInserted(UINT res,CCHReadReceivs* chReadReceiveAmount)
{
	//try{
	//	long errCode = 0;
	//	int stockType = theTVM_INFO.GetCHStockMoneyType();				// CH 循环找零箱币种(0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP)
	//	int newcount = -1;
	//	int Num_radix10 = -1;
	//	CString radixchange;
	//	CString CollectionBoxID;
	//	CH_RSP_ACCEPT_INSERT_INFO rsp;
	//	CH_COMMON_RSP chcommonrsp;			// 硬币收纳解析
	//	CTCoinCountInfo::COIN_BOX_INFO coinboxinfo;
	//	CTCoinCountInfo::COIN_TEMPORARY1_INFO cointemporary1info;
	//	

	//	// 停止读取金额异常
	//	if(E_NO_CH_ERROR != res && CCHException::ERROR_CH_STOP_READ_AMOUNT != res)
	//	{
	//		INTERNAL_ERROR(errCode);	// 判断是硬件异常、软件异常分别处理
	//	}
	//	// 停止读取金额正常
	//	else
	//	{
	//		m_work_status |= ~ WORK_STATUS_READ_AMOUNT;
	//		// 业务调用停止读取金额
	//		if(res == CCHException::ERROR_CH_STOP_READ_AMOUNT)
	//		{
	//			//__super::OnBusy(false);
	//			theSERVICE_MGR.SetForeServiceBusy(false);
	//			// 停止加币
	//			if(MENU_CODE_1 == MenuCode)
	//			{
	//				if(1 != Stop_Insert)
	//				{
	//					errCode = COIN_HELPER->CH_EndReceiveForCoinMange();		// 停止接收硬币
	//					m_work_status |= ~WORK_STATUS_ACCEPT_COIN;
	//				}

	//				// 暂存不为空
	//				if(theCOIN_COUNT.GetCoinTemporary1Info().ulCount_1yuan != 0){
	//					m_work_status |= WORK_STATUS_ACCEPT_COIN;
	//					errCode = COIN_HELPER->CH_Accept(1,2,chcommonrsp);					// 硬币收纳(同步)
	//					m_work_status |= ~WORK_STATUS_ACCEPT_COIN;
	//				}
	//
	//				// 数据更新
	//				m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAfterAddingQuantity.Format(_T("%d"),chcommonrsp.coin_info.stock1 == 0 ? m_DataModel.cyclechangeboxdata.nBeforeAddingStockCount + m_DataModel.cyclechangeboxdata.nAddingCoinCount : chcommonrsp.coin_info.stock1);
	//				m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = ThridTFrame;
	//				NotifyDataModelChanged();

	//				// 硬币循环找零箱枚数 -- 软件计数
	//				coinboxinfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//				coinboxinfo.ulCurCount = chcommonrsp.coin_info.stock1 == 0 ? m_DataModel.cyclechangeboxdata.nBeforeAddingStockCount + m_DataModel.cyclechangeboxdata.nAddingCoinCount : chcommonrsp.coin_info.stock1;
	//				theCOIN_COUNT.SetCoinHopperboxCInfo(coinboxinfo);

	//				// 暂存器清空
	//				cointemporary1info.ulCount_1yuan = 0;
	//				theCOIN_COUNT.SetCoinTemporary1Info(cointemporary1info);

	//				// 数据导入数据库
	//				Dataintodatabase();
	//				// 上传电文--硬币循环找零箱补充
	//				Sendcables();

	//				PrintCoinBoxExchangeReceipt();
	//			}
	//			// 取消加币
	//			else if(MENU_CODE_2 == MenuCode)
	//			{
	//			//	if(1 != Stop_Insert)
	//			//	{
	//			//		errCode = COIN_HELPER->CH_EndReceiveForCoinMange();		// 停止接收硬币
	//			//		// 错误代码不为0
	//			//		if(E_NO_CH_ERROR != errCode)	
	//			//		{
	//			//			INTERNAL_ERROR(errCode);									// 判断是硬件异常、软件异常分别处理
	//			//		}
	//			//	}

	//			//	errCode = COIN_HELPER->CH_Refund();						// 返还暂存器硬币
	//			//	// 错误代码不为0
	//			//	if(E_NO_CH_ERROR != errCode)	
	//			//	{
	//			//		INTERNAL_ERROR(errCode);									// 判断是硬件异常、软件异常分别处理
	//			//	}

	//			//	// 数据更新
	//			//	newcount = theCOIN_COUNT.GetCoinHopperboxCInfo().ulCurCount;
	//			//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAfterAddingQuantity.Format(_T("%d"),newcount);
	//			//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity = _T("0");
	//			//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = ThridTFrame;
	//			//	NotifyDataModelChanged();

	//			//	// 暂存器清空
	//			//	cointemporary1info.ulCount_1yuan = 0;
	//			//	theCOIN_COUNT.SetCoinTemporary1Info(cointemporary1info);
	//			}
	//			Stop_Insert = 0;
	//			return;
	//		}
	//		// 读取硬币数量
	//		else
	//		{
	//			// 设置为忙碌状态
	//			//__super::OnBusy(true);
	//			theSERVICE_MGR.SetForeServiceBusy(true);
	//			CH_RSP_ACCEPT_INSERT_INFO m_response;
	//			CH_COMMON_RSP_ACCEPT_COIN_INFO coin_info;
	//			chReadReceiveAmount->GetCHResponse(&m_response);
	//			switch(stockType)
	//			{
	//			case 1://0.5MOP
	//				m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity.Format(_T("%d"),m_response.insert_info.escrow2_05);
	//				break;
	//			case 2://1MOP
	//				m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity.Format(_T("%d"),m_DataModel.cyclechangeboxdata.nAddingCoinCount + m_response.insert_info.escrow1);
	//				coin_info.escrow1 = m_response.insert_info.escrow1;

	//				coin_info.escrow2_2 = theCOIN_COUNT.GetCoinTemporary2Info().ulCount_2yuan;
	//				coin_info.escrow2_5 = theCOIN_COUNT.GetCoinTemporary2Info().ulCount_5yuan;
	//				coin_info.escrow2_05 = theCOIN_COUNT.GetCoinTemporary2Info().ulCount_5jiao;
	//				//theSERVICE_MGR.SetState(MAINTENANCE);
	//				COIN_HELPER->SyncCoinTemporaryInfoToMasterFile(coin_info);
	//				break;
	//			case 3://2MOP
	//				m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity.Format(_T("%d"),m_response.insert_info.escrow2_2);
	//				break;
	//			case 4://5MOP
	//				m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity.Format(_T("%d"),m_response.insert_info.escrow2_5);
	//				break;
	//			default:
	//				break;
	//			}
	//			//m_response.insert_info.escrow1 >= theTVM_INFO.GetCHTemporary1Max()||
	//			// 暂存 + stock达到最大值，提示停止加币
	//			if((m_response.insert_info.escrow1 + _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity)) >= theTVM_INFO.GetCHStock1Max())
	//			{
	//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_FULL));
	//				errCode = COIN_HELPER->CH_EndReceiveForCoinMange();		// 停止接收硬币
	//				m_work_status |= ~WORK_STATUS_BEGIN_RECIVE;
	//				Sleep(50);
	//				Stop_Insert =1;
	//				COIN_HELPER->Chp_Get_Insert_Info(this);					// 开始读取金额
	//				m_work_status |= WORK_STATUS_READ_AMOUNT;
	//			}
	//			// 暂存达到最大值：直接倒入stock，再继续接受，并提示消息
	//			else if(m_response.insert_info.escrow1 >= theTVM_INFO.GetCHTemporary1Max()){

	//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_SHOW_MAX_TEMPRORARY_INSERT_MAX));

	//				errCode = COIN_HELPER->CH_EndReceiveForCoinMange();		// 停止接收硬币
	//				m_work_status |= ~WORK_STATUS_BEGIN_RECIVE;
	//				Sleep(50);

	//				if(E_NO_CH_ERROR == errCode){
	//					m_work_status |= WORK_STATUS_ACCEPT_COIN;// 正在收纳硬币
	//					// 将硬币倒入循环找零箱,回调继续开始读取金额
	//					errCode = COIN_HELPER->CH_Accept(1,2,chcommonrsp); // 硬币收纳(同步)
	//					m_work_status |= ~WORK_STATUS_ACCEPT_COIN;// 收纳完成
	//					Sleep(50);
	//					if(E_NO_CH_ERROR == errCode){
	//						m_DataModel.cyclechangeboxdata.nAddingCoinCount += m_response.insert_info.escrow1;//累加计数
	//						int nCountStock = _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity) + m_response.insert_info.escrow1;
	//						m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity.Format(_T("%d"),nCountStock);
	//						// 同步软计数
	//						coinboxinfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//						coinboxinfo.ulCurCount = chcommonrsp.coin_info.stock1;
	//						theCOIN_COUNT.SetCoinHopperboxCInfo(coinboxinfo);

	//						// 暂存器清空
	//						cointemporary1info.ulCount_1yuan = 0;
	//						theCOIN_COUNT.SetCoinTemporary1Info(cointemporary1info);

	//						// 继续接受硬币
	//						errCode = COIN_HELPER->Chp_EnableAndGetInsertInfoForCoinManage(this);
	//						if(E_NO_CH_ERROR == errCode){
	//							m_work_status |= (WORK_STATUS_BEGIN_RECIVE | WORK_STATUS_READ_AMOUNT);
	//							// 提示“请继续投入硬币”
	//							theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_CONTINUE_ADDING_COIN));
	//						}
	//						//else{//错误处理
	//						//	INTERNAL_ERROR(errCode);
	//						//}
	//					}
	//					//else {// 错误处理
	//					//	INTERNAL_ERROR(errCode);
	//					//}
	//				}
	//				//else{// 错误处理
	//				//	INTERNAL_ERROR(errCode);
	//				//}
	//			}
	//			else{
	//				COIN_HELPER->Chp_Get_Insert_Info(this);				// 开始读取金额
	//				m_work_status |= WORK_STATUS_READ_AMOUNT;
	//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_CYCLE_CHANGE_BOX_ADDING_COIN));					
	//			}		
	//			NotifyDataModelChanged();
	//		}
	//	}
	//}
	//catch(CSysException& e){
	//	// 根据错误类型进行判断
	//	if(m_work_status & WORK_STATUS_ACCEPT_COIN){//收纳硬币产生错误
	//		LOG("收纳硬币产生错误");
	//		m_work_status |= ~WORK_STATUS_ACCEPT_COIN;
	//		//Do nothing
	//	}
	//	else if(m_work_status & WORK_STATUS_READ_AMOUNT){//读金额错误
	//		LOG("读金额错误");
	//		if(m_work_status & WORK_STATUS_BEGIN_RECIVE){
	//			try{// 停止接收
	//				COIN_HELPER->CH_EndReceiveForCoinMange();
	//			}
	//			catch(...){
	//				//Do nothing
	//			}
	//			m_work_status |= ~WORK_STATUS_BEGIN_RECIVE;
	//		}
	//	}
	//	else if(m_work_status & WORK_STATUS_BEGIN_RECIVE){//开始接收
	//		LOG("开始接收");
	//		try{// 停止接收
	//			COIN_HELPER->CH_EndReceiveForCoinMange();
	//		}
	//		catch(...){
	//			//Do nothing
	//		}
	//		m_work_status |= ~WORK_STATUS_BEGIN_RECIVE;
	//	}
	//	//__super::OnBusy(false);
	//	theSERVICE_MGR.SetForeServiceBusy(false);//设置不为忙碌状态，不忙碌状态开始计时器
	//	theEXCEPTION_MGR.ProcessException(e);
	//	m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime = FirstFrame;
	//	NotifyDataModelChanged();
	//	EnterTime = 1;
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	//}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止或者取消加币

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::StopCancleCoinAdding()
{
	//CH_COMMON_RSP chcommonrsp;			// 硬币收纳解析
	//COIN_DATA coindata;		
	//MenuCode = theAPP_SESSION.GetInputToInt();				// 获取向导栏输入内容
	//long errCode = 0;
	//
	//if(MenuCode != MENU_CODE_1 /*|| MenuCode > MENU_CODE_2*/)
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
	//	return;
	//}
	//else
	//{
	//	// 停止加币
	//	if(MENU_CODE_1 == MenuCode)
	//	{
	//		COIN_HELPER->Chp_Stop_Insert_Info(); // 停止读取金额			
	//	}
	//	// 取消加币
	//	//else if(MENU_CODE_2 == MenuCode)
	//	//{
	//	//	//COIN_HELPER->Chp_Stop_Insert_Info(); // 停止读取金额				
	//	//}
	//}
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
LRESULT CCoinBoxManageAddCoinToCycleChangeBoxSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	// 硬币循环找零箱加币基本界面和完成界面可以返回，加币中界面不可以返回
	if(SecondFrame != m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxFrameFlowTime)
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上传电文

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::Sendcables()
{
	//CASHBOX_CHANGE  coinbox;
	//CASH_TYPE_COUNT cashtypecount;
	//vector<CASH_TYPE_COUNT> vecCash;

	//// 硬币循环找零箱--操作类型
	//coinbox.handleType = INSTALL;
	//// 硬币循环找零箱--ID
	///*CString cycleboxid;
	//cycleboxid.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.bStationCode);
	//BYTE stationcode = _tcstoul(cycleboxid,NULL,16);
	//coinbox.SetHopperBoxID(1, 153,stationcode,theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.wNum);*/
	//coinbox.SetBoxID(theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.bLineCode,theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.bType,theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.wNum);
	//// 硬币循环找零箱位置ID
	//coinbox.position = ONE;
	//// 硬币循环找零箱状态
	//coinbox.State = VALID_ATTACH;
	//// 硬币循环找零箱币种总数
	//coinbox.btCount = 1;
	//// 硬币循环找零箱币种代码、数量
	//cashtypecount.cash_type = Coin1MOP;
	//cashtypecount.cash_count =  _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity);
	//vecCash.push_back(cashtypecount);
	//coinbox.vecCash = vecCash;
	//
	//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(coinbox);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币循环找零箱加币凭证打印

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::PrintCoinBoxExchangeReceipt()
{
	//try
	//{
	//	COIN_DATA coindata;
	//	CString coincount = _T("");			
	//	CString cointotal;

	//	CString CoinBox_templatename = _T("TVM_COINBOX.template");
	//	CStringArray printtext;
	//	CStringArray TemplateContents;

	//	COIN_HELPER->CoinDataRead(coindata);

	//	BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

	//	int Index = 0;
	//	while(Index<TemplateContents.GetCount())
	//	{
	//		CString& line = TemplateContents.ElementAt(Index);	
	//		line.Replace(_T("{安装}"),_T("補充"));																				// 操作类型
	//		line.Replace(_T("{Install}"),_T("Added"));																			// 操作类型
	//		line.Replace(_T("{Instalar}"),_T("Adicionado"));																	// 操作类型

	//		line.Replace(_T("{專用找零箱1}"),_T("專用找零箱1"));
	//		line.Replace(_T("{專用找零箱2}"),_T("專用找零箱2"));
	//		line.Replace(_T("{回收箱1}"),_T("回收箱1"));
	//		line.Replace(_T("{回收箱2}"),_T("回收箱2"));
	//		if(0 != m_DataModel.cyclechangeboxdata.nAddingCoinCount)
	//		{
	//			line.Replace(_T("{迴圈找零箱}"),_T("*迴圈找零箱"));
	//		}
	//		else{
	//			line.Replace(_T("{迴圈找零箱}"),_T("迴圈找零箱"));
	//		}
	//		line.Replace(_T("{99FF9991}"),theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString());							// 硬币专用找零箱AID
	//		line.Replace(_T("{99FF9992}"),theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString());							// 硬币专用找零箱BID
	//		line.Replace(_T("{99FF9993}"),theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.ToString());							// 硬币循环找零箱ID

	//		line.Replace(_T("{1}"),GetHopperCoinType(theTVM_INFO.GetCHHopper1Type()));												// 硬币专用找零箱A币种
	//		line.Replace(_T("{2}"),GetHopperCoinType(theTVM_INFO.GetCHHopper2Type()));												// 硬币专用找零箱B币种
	//		line.Replace(_T("{3}"),GetHopperCoinType(theTVM_INFO.GetCHStockMoneyType()));										// 硬币循环找零箱币种

	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount);
	//		line.Replace(_T("{H191}"),coincount);																				// 硬币专用找零箱A数量--操作前

	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount);
	//		line.Replace(_T("{H192}"),coincount);																				// 硬币专用找零箱B数量--操作前
	//		
	//		CString strTemp;
	//		strTemp.Format(_T("%d"),m_DataModel.cyclechangeboxdata.nBeforeAddingStockCount);
	//		line.Replace(_T("{H193}"),strTemp);																				// 硬币循环找零箱数量--操作前

	//		// 硬币回收箱A--50AVOS
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao);
	//		line.Replace(_T("{Z191}"),coincount);
	//		line.Replace(_T("{Z195}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao*50);																			 																			
	//		line.Replace(_T("{Z191MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z195MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱A--1MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan);
	//		line.Replace(_T("{Z192}"),coincount);
	//		line.Replace(_T("{Z196}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*100);																			 																			
	//		line.Replace(_T("{Z192MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z196MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱A--2MOP
	//		line.Replace(_T("{Z193}"),_T("0"));
	//		line.Replace(_T("{Z197}"),_T("0"));
	//		line.Replace(_T("{Z193MOP}"),_T("0.00 MOP"));
	//		line.Replace(_T("{Z197MOP}"),_T("0.00 MOP"));

	//		// 硬币回收箱A--5MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan);
	//		line.Replace(_T("{Z194}"),coincount);
	//		line.Replace(_T("{Z198}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan*500);																																					
	//		line.Replace(_T("{Z194MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z198MOP}"),cointotal+_T("MOP"));
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao*50 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*100 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan*500);

	//		if(_T("FFFFFFFF") == theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString())
	//		{
	//			line.Replace(_T("{99FF9994}"),_T("--------"));
	//			line.Replace(_T("{99FF9996}"),_T("--------"));
	//			line.Replace(_T("{S191MOP}"),_T("--------"));
	//			line.Replace(_T("{S192MOP}"),_T("--------"));
	//		}
	//		else
	//		{
	//			line.Replace(_T("{99FF9994}"),theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString());					// 硬币回收箱AID--操作前			
	//			line.Replace(_T("{99FF9996}"),theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString());					// 硬币回收箱AID--操作后
	//			line.Replace(_T("{S191MOP}"),cointotal+_T("MOP"));
	//			line.Replace(_T("{S192MOP}"),cointotal+_T("MOP"));
	//		}

	//		// 硬币回收箱B--50AVOS
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao);
	//		line.Replace(_T("{Z291}"),coincount);
	//		line.Replace(_T("{Z295}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao*50);																			 																			
	//		line.Replace(_T("{Z291MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z295MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--1MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan);
	//		line.Replace(_T("{Z292}"),coincount);
	//		line.Replace(_T("{Z296}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan*100);																			 																			
	//		line.Replace(_T("{Z292MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z296MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--2MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_2yuan);
	//		line.Replace(_T("{Z293}"),coincount);
	//		line.Replace(_T("{Z297}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan*200);																			 																			
	//		line.Replace(_T("{Z293MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z297MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--5MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5yuan);
	//		line.Replace(_T("{Z294}"),coincount);
	//		line.Replace(_T("{Z298}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5yuan*500);																																					
	//		line.Replace(_T("{Z294MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z298MOP}"),cointotal+_T("MOP"));

	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao*50 + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan*100 + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_2yuan*200 + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5yuan*500);

	//		if(_T("FFFFFFFF") == theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString())
	//		{
	//			line.Replace(_T("{99FF9995}"),_T("--------"));
	//			line.Replace(_T("{99FF9997}"),_T("--------"));
	//			line.Replace(_T("{S291MOP}"),_T("--------"));
	//			line.Replace(_T("{S292MOP}"),_T("--------"));
	//		}
	//		else
	//		{
	//			line.Replace(_T("{99FF9995}"),theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString());					// 硬币回收箱AID--操作前			
	//			line.Replace(_T("{99FF9997}"),theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString());					// 硬币回收箱AID--操作后
	//			line.Replace(_T("{S291MOP}"),cointotal+_T("MOP"));
	//			line.Replace(_T("{S292MOP}"),cointotal+_T("MOP"));
	//		}

	//		// 操作后
	//		coincount.Format(_T("%d"),coindata.hopper1_num);
	//		line.Replace(_T("{H194}"),coincount);																				// 硬币专用找零箱A数量--操作后

	//		coincount.Format(_T("%d"),coindata.hopper2_num);
	//		line.Replace(_T("{H195}"),coincount);																				// 硬币专用找零箱B数量--操作后

	//		coincount.Format(_T("%d"),coindata.stock1_num);
	//		line.Replace(_T("{H196}"),coincount);																				// 硬币循环找零箱数量--操作后

	//		line.Replace(_T("{操作時間}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
	//		line.Replace(_T("{操作員編號}"),theAPP_SESSION.GetUserInfo().sUserID);
	//		line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
	//		line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
	//		line.Replace(_T("{Nome_da_estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
	//		line.Replace(_T("{設備編號}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

	//		CStringArray receiptline;
	//		ComPrintLineFormatForTemplate(line,receiptline);
	//		printtext.Append(receiptline);
	//		Index++;
	//	}
	//	PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//}
	//catch(CSysException&e) {
	//	m_DataModel.IsPrinterSuccessfull = FALSE;
	//	theEXCEPTION_MGR.ProcessException(e);		
	//}
	//catch (...) {
	//	m_DataModel.IsPrinterSuccessfull = FALSE;
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      数据导入数据库

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAddCoinToCycleChangeBoxSvc::Dataintodatabase()
{
	//try
	//{
	//	CoinAddOperationRecord addcoinrecord;
	//	//addcoinrecord.iID = 0;								// 记录编号
	//	addcoinrecord.dtOperationTime = ComGetCurTime();		// 操作时间

	//	addcoinrecord.txtCoinAddBoxID = _T("");							// 补充箱编号
	//	addcoinrecord.txtCoinChangeBoxID = m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxID;	// 找零箱编号
	//	// 币种
	//	int index = theTVM_INFO.GetCHStockMoneyType();
	//	switch(index)
	//	{
	//	case 1:
	//		addcoinrecord.iCoinTypeCode = 0x16;
	//		break;
	//	case 2:
	//		addcoinrecord.iCoinTypeCode = 0x11;
	//		break;
	//	case 3:
	//		addcoinrecord.iCoinTypeCode = 0x12;
	//		break;
	//	case 4:
	//		addcoinrecord.iCoinTypeCode = 0x15;
	//		break;
	//	default:
	//		break;
	//	}
	//	addcoinrecord.iQuantityBeforeAdd = _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxQuantity);						// 补充前数量	
	//	addcoinrecord.iQuantityAdd = _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAddingQuantity);						// 补充数量
	//	addcoinrecord.iQuantityAfterAdd = _ttoi(m_DataModel.cyclechangeboxdata.CycleChangeCoinBoxAfterAddingQuantity);				// 补充后数量
	//	addcoinrecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;															// 操作员ID
	//	addcoinrecord.txtComment = _T("");
	//	addcoinrecord.txtReserve = _T("");	

	//	DATA_HELPER->DbInsertCoinAddOperationRecord(addcoinrecord);
	//}
	//catch (CSysException& e) {
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
	//
}