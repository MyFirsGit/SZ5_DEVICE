#include "stdafx.h"
#include "TokenModuleTestSvc.h"
#include "TokenModuleMaintainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_TOKENTEST_MENU_TO_OPERATION = _T("WM_TOKENTEST_MENU_TO_OPERATION");
static const TCHAR* const WM_TOKENTEST_OPERATION_TO_MENU = _T("WM_TOKENTEST_OPERATION_TO_MENU");

BEGIN_MESSAGE_MAP(CTokenModuleTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F2,OnKeyboardF2)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTokenModuleTestSvc::CTokenModuleTestSvc()
	:CTVMForeService(TOKEN_MODULE_TEST_SVC)
{
	//设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG,new CTokenModuleMaintainDlg(this));

	//设置画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MENU,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MENU,IDD_20113_STATION_SET_END_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OPERATION,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OPERATION,IDD_20113_STATION_SET_END_DLG);

	//设置画面扭转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_MENU,WM_TOKENTEST_MENU_TO_OPERATION,DIALOG_GROUP_OPERATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OPERATION,WM_TOKENTEST_OPERATION_TO_MENU,DIALOG_GROUP_MENU);

	//设置初始画面
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MENU);

	m_bIsCatchLeftToken  = false;
	m_bIsCatchRightToken = false;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTokenModuleTestSvc::~CTokenModuleTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      无

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::OnStart(){
	__super::OnStart();
	//设置初始画面
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MENU);

	m_bIsCatchLeftToken  = false;
	m_bIsCatchRightToken = false;
	m_Modol = Modol();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
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
LRESULT CTokenModuleTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MENU)
	{
		DoMenuOperation();
	}
	else if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	{
		DoOperation();
	}
	//刷新界面
	//ReflashUI();
	NotifyDataModelChanged();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      [F3]响应事件

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTokenModuleTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	{
		DoDialogFlow(WM_TOKENTEST_OPERATION_TO_MENU);
		NotifyDataModelChanged();
		ClearModolData();
	}
	else
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [F2]键保存数据响应函数

@param      (i)WPARAM 无用
@param      (i)LPARAM 无用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTokenModuleTestSvc::OnKeyboardF2(WPARAM , LPARAM )
{
	//保存修改的RFID数据
	if (GetCurDialogGroupID() == DIALOG_GROUP_OPERATION)
	{
		theSERVICE_MGR.SetForeServiceBusy(true);
		if (m_Modol.operationType == OPERATION_TYPE_WRITE_ID)
		{
			bool bIsOk = true;
			try
			{
				if (m_Modol.box_id.strRFIDBox1.IsEmpty()&&
					m_Modol.box_id.strRFIDBox2.IsEmpty())
				{
					//没有需要修改项
					theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_NEED_ALTER_RFID);
					return TRUE ;
				}
				long errCode = 0; 
				//RFID is not null update it!
				//1.Token箱1
				if (!m_Modol.box_id.strRFIDBox1.IsEmpty())
				{
					//TOKEN_CMD_SET_BOX_DATA buffSetBox;                     //需要写入的数据
					//_DATE_TIME dateTime = ComGetCurTime();
					//buffSetBox.magazine = TOKEN_MAGAZINE::TOKEN_MAGAZINE_A;//箱子编号
					//buffSetBox.tokenData.dateTime = dateTime;
					//CString strTemp;
					//strTemp = m_Modol.box_id.strRFIDBox1;                  //01940001(RFID 结构)
					////CString to Byte
					//int nLenth = strTemp.GetLength() / 2;                  //写前四位，有两位保留。
					//for (int i = 0; i < nLenth;i++)
					//{
					//	buffSetBox.tokenData.logicID[i] = _ttoi(strTemp.Left(2));//可能需要转16进制！！
					//	strTemp = strTemp.Right(strTemp.GetLength() - 2);
					//}
		
					//bool bWriteSuccess = false;
					//TOKEN_HELPER->TokenDataWrite(buffSetBox,bWriteSuccess);//写数据，并返回结果。
					//if (!bWriteSuccess)
					//{
					//	bIsOk = false;
					//}
				}
				//2.Token箱2
				if (!m_Modol.box_id.strRFIDBox2.IsEmpty())
				{
					//TOKEN_CMD_SET_BOX_DATA buffSetBox;
					//_DATE_TIME dateTime = ComGetCurTime();
					//buffSetBox.magazine = TOKEN_MAGAZINE::TOKEN_MAGAZINE_B;
					//buffSetBox.tokenData.dateTime = dateTime;
					//CString strTemp;
					//strTemp = m_Modol.box_id.strRFIDBox2;//01940002
					////CString to Byte
					//int nLenth = strTemp.GetLength() / 2;
					//for (int i = 0; i < nLenth;i++)
					//{
					//	buffSetBox.tokenData.logicID[i] =  _ttoi(strTemp.Left(2));
					//	strTemp = strTemp.Right(strTemp.GetLength() - 2);
					//}

					//bool bWriteSuccess = false;
					//TOKEN_HELPER->TokenDataWrite(buffSetBox,bWriteSuccess);
					//if (!bWriteSuccess)
					//{
					//	bIsOk = false;
					//}
				}
				//Result
				if (bIsOk)
				{
					//ok
					m_Modol.MenuInfo[7].strMenuStatus = COMPLETED;
					theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_SAVE_RFID_SUCCESS);
				}
				else
				{
					m_Modol.MenuInfo[7].strMenuStatus = FAILED;
					theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_SAVE_RFID_FAILED);
				}
			}
			catch(CSysException& e)
			{
				m_Modol.MenuInfo[7].strMenuStatus = FAILED;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_SAVE_RFID_ERROR);
				theEXCEPTION_MGR.ProcessException(e);
			}
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	return TRUE;
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
//LRESULT CTokenModuleTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      数据改变后，通知窗体更新数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型数据

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTokenModuleTestSvc::Modol& CTokenModuleTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      菜单选项调度函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::DoMenuOperation()
{
	int index = theAPP_SESSION.GetInputToInt();

	// 非超级用户只响应第一个菜单
	if(index != Menu_Code_1 && !theAPP_SESSION.IsSuperUser()){
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return;
	}

	bool bIsLeft = false;

	switch(index)
	{
	case Menu_Code_1:
		{
			//1.Reset
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TESTING));
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (ResetTest())
			{
				m_Modol.MenuInfo[0].strMenuStatus = SUCCESS;		
			}
			else
			{
				m_Modol.MenuInfo[0].strMenuStatus = FAILED;
			}
			theSERVICE_MGR.SetForeServiceBusy(false);
			//设置提示文言
			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
		break;
	case Menu_Code_21:
		{
			 bIsLeft = true;
		}
	case Menu_Code_22:
		{
			//2.Catch Token
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TESTING));
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (CatchTokenTest(bIsLeft))
			{
				m_Modol.MenuInfo[1].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.MenuInfo[1].strMenuStatus = FAILED;
			}
			theSERVICE_MGR.SetForeServiceBusy(false);

			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
		break;
	case Menu_Code_3:
		{
			//3.Putout Token
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TESTING));
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (PutoutTokenTest())
			{
				m_Modol.MenuInfo[2].strMenuStatus = SUCCESS;
				//设置提示文言
				theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
			}
			else
			{
				m_Modol.MenuInfo[2].strMenuStatus = FAILED;
			}
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	//case Menu_Code_41:
	//	{
	//		 bIsLeft = true;
	//	}
	//case Menu_Code_42:
	case Menu_Code_4:
		{
			//4.Recycle Token
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TESTING));
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (RecycleTokenBoxTest(bIsLeft))
			{
				m_Modol.MenuInfo[3].strMenuStatus = SUCCESS;
				//设置提示文言
				theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
			}
			else
			{
				m_Modol.MenuInfo[3].strMenuStatus = FAILED;
			}
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	case Menu_Code_51:
		{
			 bIsLeft = true;
		}
	case Menu_Code_52:
		{
			//5.Clear Token Box
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TESTING));
			theSERVICE_MGR.SetForeServiceBusy(true);
			if (ClearTokenBoxTest(bIsLeft))
			{
				m_Modol.MenuInfo[4].strMenuStatus = SUCCESS;
			}
			else
			{
				m_Modol.MenuInfo[4].strMenuStatus = FAILED;
			}
			theSERVICE_MGR.SetForeServiceBusy(false);
			//设置提示文言
			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
		break;
	//case Menu_Code_61:
	//	{
	//		 bIsLeft = true;
	//	}
	//case Menu_Code_62:
	//	{
	//		//6.Detach Token Box
	//		theSERVICE_MGR.SetForeServiceBusy(true);
	//		if (DetachTokenBoxTest(bIsLeft))
	//		{
	//			m_Modol.MenuInfo[5].strMenuStatus = SUCCESS;
	//		}
	//		else
	//		{
	//			m_Modol.MenuInfo[5].strMenuStatus = FAILED;
	//		}
	//		theSERVICE_MGR.SetForeServiceBusy(false);
	//		//设置提示文言
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//	}
	//	break;
	//case Menu_Code_7:
	//	{
	//		//7.Read Token Box ID 
	//		m_Modol.operationType = OPERATION_TYPE_READ_ID;
	//		DoDialogFlow(WM_TOKENTEST_MENU_TO_OPERATION);
	//		theSERVICE_MGR.SetForeServiceBusy(true);
	//		ReadTokenBoxIdTest();
	//		theSERVICE_MGR.SetForeServiceBusy(false);
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CONFORM_RFID);			
	//	}
	//	break;
	//case Menu_Code_8:
	//	{
	//		//8.Write Token Box ID
	//		m_Modol.operationType = OPERATION_TYPE_WRITE_ID;
	//		DoDialogFlow(WM_TOKENTEST_MENU_TO_OPERATION);
	//	}
	//	break;
	//case Menu_Code_9:
	//	{
	//		//9.Read Version
	//		theSERVICE_MGR.SetForeServiceBusy(true);
	//		ReadVersionTest();
	//		theSERVICE_MGR.SetForeServiceBusy(false);
	//		//设置提示文言
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//	}
	//	break;
	default:
		{
			//Error Menu Number
			m_Modol.operationType = OPERATION_TYPE_UNKNOW;
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return;
		}
		break;
	}

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      操作界面调度函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::DoOperation()
{
	//根据不同的操作，做出不同的判断
	switch(m_Modol.operationType)
	{
	case OPERATION_TYPE_READ_ID:
		{
			//1.读Token票箱RFID(Waring)
			theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_CONFORM_RFID);
		}
		break;
	case OPERATION_TYPE_WRITE_ID:
		{
			//2.写Token票箱RFID
			theSERVICE_MGR.SetForeServiceBusy(true);
			WriteTokenBoxIdTest();
			theSERVICE_MGR.SetForeServiceBusy(false);
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重置操作测试

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTokenModuleTestSvc::ResetTest()
{
	long errCode = 0;
	try
	{
		errCode = TOKEN_HELPER->Reset();
		if (errCode == NO_ERROR)
		{
			CString comment(_T("ResetTest"));
			// 硬币模块测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
			return true;
		}
		CString comment(_T("ResetTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		return false;
	}
	catch(CSysException& e)
	{
		CString comment(_T("ResetTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      抓取Token操作测试

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTokenModuleTestSvc::CatchTokenTest(bool bIsLeft)
{
	long errCode = 0;
	try
	{
		UINT retNum = 0;
		TOKEN_BOX_NO boxNo = TOKEN_BOX_A;
		//左、右票箱
		if (bIsLeft){
			boxNo = TOKEN_BOX_B;
		}
		else{
			boxNo = TOKEN_BOX_A;
		}

		//发送取票命令，返回执行结果
		errCode = TOKEN_HELPER->CardOut(boxNo,&retNum);
		if (errCode == NO_ERROR)
		{
			CString comment(_T("CatchTokenTest"));
			// 硬币模块测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
			
			//成功送到出票口
			return true;					
		}
		else
		{
			CString comment(_T("CatchTokenTest"));
			// 硬币模块测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
			return false;
		}

	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      放出Token操作测试

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTokenModuleTestSvc::PutoutTokenTest()
{
	long errCode = 0;
	try
	{
		UINT retNum = 0;
		errCode = TOKEN_HELPER->SendCard(&retNum);
		if (errCode == NO_ERROR){
			return true;
		}
		else{
			return false;
		}

		CString comment(_T("PutoutTokenTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
	}
	catch(CSysException& e)
	{
		CString comment(_T("PutoutTokenTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      回收Token操作测试

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTokenModuleTestSvc::RecycleTokenBoxTest(bool bIsLeft)
{
	long errCode = 0;
	try
	{
		UINT retNum = 0;
		errCode = TOKEN_HELPER->RetractCard(&retNum);
		if (errCode == NO_ERROR){
			return true;
		}
		else{
			return false;
		}

		CString comment(_T("RecycleTokenBoxTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
	}
	catch(CSysException& e)
	{
		CString comment(_T("RecycleTokenBoxTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清空Token箱操作测试

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTokenModuleTestSvc::ClearTokenBoxTest(bool bIsLeft)
{
	long errCode = 0;
	try
	{
		if (bIsLeft)
		{
			TOKEN_HELPER->CleanOut(TOKEN_BOX_B);
		}
		else
		{
			TOKEN_HELPER->CleanOut(TOKEN_BOX_A);
		}
		CString comment(_T("ClearTokenBoxTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
	}
	catch(CSysException& e)
	{
		CString comment(_T("ClearTokenBoxTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		//错误
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      卸载Token箱操作测试

@param      none

@retval     (o) true  or  false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTokenModuleTestSvc::DetachTokenBoxTest(bool bIsLeft)
{
	try
	{
		//TOKEN_MAGAZINE boxId;
		//if (bIsLeft)
		//{
		//	boxId = TOKEN_MAGAZINE::TOKEN_MAGAZINE_A;

		//}
		//else
		//{
		//	boxId = TOKEN_MAGAZINE::TOKEN_MAGAZINE_B;
		//}

		//TOKEN_HELPER->RemoveMagazine(boxId);//无异常，即正常。

		CString comment(_T("DetachTokenBoxTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
	}
	catch(CSysException& e)
	{
		CString comment(_T("DetachTokenBoxTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读票箱RFID

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::ReadTokenBoxIdTest()
{
	try
	{
		//TOKEN_MAGAZINE boxId;
		//TOKEN_RSP_READ_TOKEN_DATA  box1Data;
		////票箱1  RFID
		//boxId = TOKEN_MAGAZINE::TOKEN_MAGAZINE_A;
		//if (TOKEN_HELPER->TokenDataRead(boxId,box1Data))
		//{
		//	//Byte[] to CString
		//	CString strTemp;
		//	int nCount = sizeof(box1Data.boxData.logicID) - 2;//两个预留字节
		//	for (int i = 0; i< nCount;++i)
		//	{
		//		strTemp.Format(_T("%02d"),box1Data.boxData.logicID[i]);
		//		if (!strTemp.IsEmpty())
		//		{
		//			m_Modol.box_id.strRFIDBox1 += strTemp;
		//		}
		//	}
		//}
		//else
		//{
		//	m_Modol.box_id.strRFIDBox1 = UNKNOW;
		//}
		////票箱2  RFID
		//TOKEN_RSP_READ_TOKEN_DATA box2Data;
		//boxId = TOKEN_MAGAZINE::TOKEN_MAGAZINE_B;
		//if (TOKEN_HELPER->TokenDataRead(boxId,box2Data))
		//{
		//	//Byte[] to CString 
		//	CString strTemp;
		//	int nCount = sizeof(box2Data.boxData.logicID) - 2;//两个预留字节
		//	for (int i = 0; i< nCount;++i)
		//	{
		//		strTemp.Format(_T("%02d"),box2Data.boxData.logicID[i]);
		//		if (!strTemp.IsEmpty())
		//		{
		//			m_Modol.box_id.strRFIDBox2 += strTemp;
		//		}
		//	}
		//	m_Modol.MenuInfo[6].strMenuStatus = COMPLETED;
		//}
		//else
		//{
		//	m_Modol.MenuInfo[6].strMenuStatus = FAILED;
		//	m_Modol.box_id.strRFIDBox2 = UNKNOW;
		//}
		CString comment(_T("ReadTokenBoxIdTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
	}
	catch (CSysException& e)
	{
		//Set default rfid
		m_Modol.box_id.strRFIDBox1 = UNKNOW;
		m_Modol.box_id.strRFIDBox2 = UNKNOW;

		CString comment(_T("ReadTokenBoxIdTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写钱箱RFID

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::WriteTokenBoxIdTest()
{
	CString strMenuInfo = theAPP_SESSION.GetInputToString();
	if (strMenuInfo.GetLength() != 5)
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_COIN_TEST_INPUT_ERROR_RETRY);
		return;
	}

	int nMainMenu = _ttoi(strMenuInfo.Left(1));

	CString strRFIDHead;

	switch(nMainMenu)
	{
	case MENU_CODE_1:
		{
			strRFIDHead.Format(_T("0194%s"),strMenuInfo.Right(strMenuInfo.GetLength() - 1));
			m_Modol.box_id.strRFIDBox1 = strRFIDHead;
			//设置提示文言
			theAPP_SESSION.ShowOfficeGuide(GUIDE_TOKEN_TEST_INPUT_TOKEN_BOX_NUM2);
		}
		break;
	case MENU_CODE_2:
		{
			strRFIDHead.Format(_T("0194%s"),strMenuInfo.Right(strMenuInfo.GetLength() - 1));
			m_Modol.box_id.strRFIDBox2 = strRFIDHead;
			//设置提示文言
			theAPP_SESSION.ShowOfficeGuide(GUIDE_TOKEN_TEST_INPUT_SUCCESS_F2_SAVE);
		}
		break;
	default:
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		}
		break;
	}
	CString comment(_T("WriteTokenBoxIdTest"));
	// 硬币模块测试--操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读固件版本

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::ReadVersionTest()
{
	try
	{	
		TOKEN_RSP_GET_VERSION version;
		TOKEN_HELPER->GetVersion(&version);
		m_Modol.MenuInfo[8].strMenuStatus .Format(_T("%X-%X"),version.ucModNum,version.ucFwVer);

		CString comment(_T("ReadVersionTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,true,comment);

	}
	catch(CSysException& e)
	{
		m_Modol.MenuInfo[8].strMenuStatus = FAILED;
		CString comment(_T("ReadVersionTest"));
		// 硬币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_UNIT_TEST,false,comment);
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清空可变数据，防止数据重用

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTokenModuleTestSvc::ClearModolData()
{
	//操作类型状态
	m_Modol.operationType = OPERATION_TYPE_UNKNOW;
		
	//RFID状态
	m_Modol.box_id.strRFIDBox1 = UNKNOW;
	m_Modol.box_id.strRFIDBox2 = UNKNOW;
}
