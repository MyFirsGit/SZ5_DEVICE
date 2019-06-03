#include "stdafx.h"
#include "BanknoteChangeModuleTestSvc.h"
#include "BanknoteChangeMaintainDlg.h"
#include "Service.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; 
#endif

BEGIN_MESSAGE_MAP(CBanknoteChangeModuleTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_COMMAND_COMPLETE(BANKNOTE_MODULE_COMMAND_ID_INSERT,OnBanknoteInsertedEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteChangeModuleTestSvc::CBanknoteChangeModuleTestSvc()
	:CTVMForeService(BANKNOTE_CHANGE_MODULE_TEST_SVC)
{
	//设置操作员画面
	GetDialogFlow()->AddDialog(IDD_00501_EXCHANGE_BASE_DLG,new CBanknoteChangeMaintainDlg(this));

	//设置画面组
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_BANKNOTE_CHANGE,_opl(GUIDE_ENTER_F3_ESC));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_BANKNOTE_CHANGE,CBanknoteChangeMaintainDlg::IDD);


	//设置初始画面
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_BANKNOTE_CHANGE);

	IsInit = FALSE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteChangeModuleTestSvc::~CBanknoteChangeModuleTestSvc(){
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      业务启动后需要初始化的数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeModuleTestSvc::OnStart(){

	IsInit = FALSE;
	m_Modol = Modol();
	//设置初始画面
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_BANKNOTE_CHANGE);
	m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;

	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [ENTER]按键处理消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteChangeModuleTestSvc::OnKeyboardEnter(WPARAM , LPARAM ){

	try
	{
		long errCode = 0;
		//CommonFeedback bhstatus;
		//errCode = BH_CHANGE_HELPER->getStatus(bhstatus);
		
		//DEVICE_STATUS_INFO bhStatus;
		//errCode = BH_CHANGE_HELPER->GetStatus(bhStatus);
		int index  = theAPP_SESSION.GetInputToInt();

		// 非超级用户只响应第一个菜单
		if(index != MENU_CODE_1 && !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return FALSE;
		}

		// 输入编号
		if(m_OperationType != OPERATION_TYPE::OPERATION_TYPE_UNKNOW){
			WORD wInput = 0;
			if(index >0 && index < 9999){
				wInput = (index % 10000) & 0xFFFF;
				DoWriteRfid(wInput);
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ERROR_INPUT) + _opl(TXT_COLON));
			}
		}
		else{
			switch(index)
			{
			case MENU_CODE_1:
				{
					//1.重置
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_RESET;
				}
				break;
			case MENU_CODE_2:
				{
					//2.出款
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_COUNT_BILL;
				}
				break;
			case MENU_CODE_31:
				{
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_READ_BOX_A;
				}
				break;
			case MENU_CODE_32:
				{
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_READ_BOX_B;
				}
				break;
			case MENU_CODE_33:
				{
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_READ_BOX_INVALID;
				}
				break;
			case MENU_CODE_41:
				{
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_A;
				}
				break;
			case MENU_CODE_42:
				{
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_B;
				}
				break;
			case MENU_CODE_43:
				{
					m_OperationType = OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_INVALID;
				}
				break;
			default:
				{
					theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
					return TRUE;
				}
				break;
			}
			//执行操作
			DoOperation();

			//theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}

		//执行完成后显示执行结果
		NotifyDataModelChanged();

		return TRUE;
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_IO_OPEN_PORT_ERROR));
		// 纸币找零模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_IO_OPEN_PORT_ERROR));
		// 纸币找零模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteChangeModuleTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	m_OperationType = OPERATION_TYPE::OPERATION_TYPE_F3;
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteChangeModuleTestSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam)
{
	m_OperationType = OPERATION_TYPE::OPERATION_TYPE_ESC;
	__super::OnKeyboardEsc(0,0);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      刷新菜单界面显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeModuleTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [0~9]按键处理消息

@param      (i)WPARAM wParam  数字键值，例如：数字键7，值为7
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CBanknoteChangeModuleTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型数据

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteChangeModuleTestSvc::Modol& CBanknoteChangeModuleTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行操作，返回操作结果

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeModuleTestSvc::DoOperation()
{
	theSERVICE_MGR.SetForeServiceBusy(true);
	theAPP_SESSION.ShowOfficeGuide(GUIDE_BANKNOTE_TEST_TESTING);
	int nMenuNum = 0;
	long errCode = 0;
	bool bError = false;
	try
	{
		if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_RESET)
		{
			nMenuNum = 0;
			//重置
			//先复位，复位失败断开重新连接
			try{
				BH_CHANGE_HELPER->Reset();
			}
			catch(CSysException& e){
				bError = true;
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
				//异常
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
				theEXCEPTION_MGR.ProcessException(e);
				// 纸币模块测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,true);
			}
			catch(...)
			{
				bError = true;
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
				//异常
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
				// 纸币模块测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
			}
			
			// 没有错误即为正确
			if(!bError){
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RESET_FAULT));
			}
		}
		else if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_COUNT_BILL)
		{
			nMenuNum = 1;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_BANKNOTE_TEST_TESTING);
			//两个钱箱各出一张钱
			//BH_CHANGE_HELPER->SendOutBill(1,1);//lichao
			m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
			m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
		}
		else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_READ_BOX_A){
			nMenuNum = 2;
			//RFID_DATA readRfid;
			//readRfid.bankNoteChangeRFID.bLineCode = ZHENGZHOU_CHENGJIAO;
			//readRfid.bankNoteChangeRFID.bType = 0x60;
			//long lRet = BH_RFID_HELPER->getRfid(UEBOXTYPE::UE_BOXA,readRfid);
		
			//if(0 == lRet){
			//	m_Modol.readRfidBoxA = readRfid.bankNoteChangeRFID;
			//}
			//else{
			//	bError = true;
			//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_READ_RFID_FAILE));
			//}
			m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
		}
		else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_READ_BOX_B){
			nMenuNum = 3;
			//RFID_DATA readRfid;
			//readRfid.bankNoteChangeRFID.bLineCode = ZHENGZHOU_CHENGJIAO;
			//readRfid.bankNoteChangeRFID.bType = 0x60;
			//long lRet = BH_RFID_HELPER->getRfid(UEBOXTYPE::UE_BOXB,readRfid);
			//
			//if(0 == lRet){
			//	m_Modol.readRfidBoxB = readRfid.bankNoteChangeRFID;
			//}
			//else{
			//	bError = true;
			//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_READ_RFID_FAILE));
			//}
			m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
		}
		else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_READ_BOX_INVALID){
			nMenuNum = 4;
			//RFID_DATA readRfid;
			//readRfid.bankNoteChangeRFID.bLineCode = ZHENGZHOU_CHENGJIAO;
			//readRfid.bankNoteChangeRFID.bType = 0x62;
			//long lRet = BH_RFID_HELPER->getRfid(UEBOXTYPE::UE_BOXC,readRfid);

			//if(0 == lRet){
			//	m_Modol.readRfidInvalid = readRfid.bankNoteChangeRFID;
			//}
			//else{
			//	bError = true;
			//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_READ_RFID_FAILE));
			//}
			m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
		}
		else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_A){
			nMenuNum = 5;
			CString strShowMsg;
			strShowMsg.Format(_opl(BH_TEST_TEXT_CHANGE_BOXA_SET_ID) + _T("：%02d60"),theMAINTENANCE_INFO.GetLineCode());
			theAPP_SESSION.SetPrefixToGuide(strShowMsg);
			/*m_Modol.WriteRfidBoxA.bLineCode = theMAINTENANCE_INFO.GetLineCode();
			m_Modol.WriteRfidBoxA.bType = 0x60;*/
		}
		else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_B){
			nMenuNum = 6;
			CString strShowMsg;
			strShowMsg.Format(_opl(BH_TEST_TEXT_CHANGE_BOXB_SET_ID) + _T("：%02d60"),theMAINTENANCE_INFO.GetLineCode());
			theAPP_SESSION.SetPrefixToGuide(strShowMsg);
			/*m_Modol.WriteRfidBoxB.bLineCode = theMAINTENANCE_INFO.GetLineCode();
			m_Modol.WriteRfidBoxB.bType = 0x60;*/
		}
		else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_INVALID){
			nMenuNum = 7;
			CString strShowMsg;
			strShowMsg.Format(_opl(BH_TEST_TEXT_CHANGE_COIN_SET_ID) + _T("：%02d62"),theMAINTENANCE_INFO.GetLineCode());
			theAPP_SESSION.SetPrefixToGuide(strShowMsg);
			/*m_Modol.WriteRfidInvalid.bLineCode = theMAINTENANCE_INFO.GetLineCode();
			m_Modol.WriteRfidInvalid.bType = 0x62;*/
		}
	}
	catch(CSysException& e){
		bError = true;
		m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
		//异常
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		theEXCEPTION_MGR.ProcessException(e);
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,true);

		if(nMenuNum == 1){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_CHANGE_FAILED));
		}
	}
	catch(...)
	{
		bError = true;
		m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;
		//异常
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
	}

	if(nMenuNum < 5){
		 !bError ? theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC) : NULL;
	}
	else{
		theAPP_SESSION.SetMaxInputLength(4);
	}
	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID

@param      int nRfid

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteChangeModuleTestSvc::DoWriteRfid(int nRfid){
	
	WORD wRfid = 0;

	//nRfid = 1234    -> wRfid = 0x1234

	for(int n = 0; n< 4;n++){
		wRfid |= ((nRfid%10) & 0x0f) << n * 4;
		nRfid /= 10;
	}

	if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_A){
		m_Modol.WriteRfidBoxA.bLineCode = theMAINTENANCE_INFO.GetLineCode();
		m_Modol.WriteRfidBoxA.bType = 0x60;
		m_Modol.WriteRfidBoxA.bNum = wRfid;
		//RFID_DATA writeData;
		//writeData.bankNoteChangeRFID = m_Modol.WriteRfidBoxA;
		//long lRet = BH_RFID_HELPER->setRfid(UEBOXTYPE::UE_BOXA,writeData);

		//if(0 == lRet){
		//	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_CHANGE_BOXA_SET_RFID_SUCCESS));
		//}
		//else{
		//	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_CHANGE_BOXA_SET_RFID_FAILED));
		//	m_Modol.WriteRfidBoxA = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
		//}
	}
	else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_B){
		m_Modol.WriteRfidBoxB.bLineCode = theMAINTENANCE_INFO.GetLineCode();
		m_Modol.WriteRfidBoxB.bType = 0x60;
		m_Modol.WriteRfidBoxB.bNum = wRfid;

		//RFID_DATA writeData;
		//writeData.bankNoteChangeRFID = m_Modol.WriteRfidBoxB;
		//long lRet = BH_RFID_HELPER->setRfid(UEBOXTYPE::UE_BOXB,writeData);

		//if(0 == lRet){
		//	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_CHANGE_BOXB_SET_RFID_SUCCESS));
		//}
		//else{
		//	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_CHANGE_BOXB_SET_RFID_FAILED));
		//	m_Modol.WriteRfidBoxB = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
		//}
	}
	else if(m_OperationType == OPERATION_TYPE::OPERATION_TYPE_WRITE_BOX_INVALID){
		m_Modol.WriteRfidInvalid.bLineCode = theMAINTENANCE_INFO.GetLineCode();
		m_Modol.WriteRfidInvalid.bType = 0x62;
		m_Modol.WriteRfidInvalid.bNum = wRfid;

		//RFID_DATA writeData;
		//writeData.bankNoteChangeRFID = m_Modol.WriteRfidInvalid;
		//long lRet = BH_RFID_HELPER->setRfid(UEBOXTYPE::UE_BOXC,writeData);

		//if(0 == lRet){
		//	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_CHANGE_BOXC_SET_RFID_SUCCESS));
		//}
		//else{
		//	theAPP_SESSION.ShowOfficeGuide(_opl(BH_TEST_TEXT_CHANGE_BOXC_SET_RFID_FAILED));
		//	m_Modol.WriteRfidInvalid = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
		//}
	}
	
	m_OperationType = OPERATION_TYPE::OPERATION_TYPE_UNKNOW;	
}