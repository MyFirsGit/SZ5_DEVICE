#include "stdafx.h"
#include "NetworkTestSvc.h"
#include "SYSInfo.h"
#include "networkitemmaintaindlg.h"
#include "guideinfo.h"

//消息和消息函数绑定
BEGIN_MESSAGE_MAP(CNetworkTestSvc, CTVMForeService)
	////ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNetworkTestSvc::CNetworkTestSvc()
	:CTVMForeService(NETWORK_TEST_SVC)
{
	//设置操作员画面
	GetDialogFlow()->AddDialog(IDD_20551_NETWORK_ITEM_DLG,new CNetworkItemMaintainDlg(this));

	//设置画面分组
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_NETWORK, _opl(GUIDE_ENTER_F3_ESC));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_NETWORK,IDD_20551_NETWORK_ITEM_DLG);

	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_NETWORK);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNetworkTestSvc::~CNetworkTestSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      启动业务的第一个响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNetworkTestSvc::OnStart(){
	// 设置初始画面组
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_NETWORK);
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	__super::OnStart();

	// 初始化数据
	CNetworkItemMaintainDlg* pDlg = dynamic_cast<CNetworkItemMaintainDlg*>(m_pDialogFlow->GetDialog(CNetworkItemMaintainDlg::IDD));
	if(NULL != pDlg){
		CNetworkItemMaintainDlg::Menu_Info& MenuInfo = pDlg->GetStructData();
		MenuInfo = CNetworkItemMaintainDlg::Menu_Info::_Menu_Status_();
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ping上位服务器

@param      char* pBuff    输出BUFF

@retval     BOOL \n
TRUE:创建成功    FALSE:创建失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CNetworkTestSvc::PingSC(char* pBuff)
{
	try {

		// 获得安全信息
		SECURITY_ATTRIBUTES sa;
		HANDLE hRead,hWrite;

		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		// 创建管道
		if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
			theSERVICE_MGR.SetForeServiceBusy(false);
			return FALSE;
		}

		// 开始ping SC
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		CString trans_exe = _T("ping ");                       // 指定应用程序的路径
		CString sIPAddress = theMAINTENANCE_INFO.GetCpsHostID();
		trans_exe = trans_exe + sIPAddress;                    //ping 10.1.1.30

		if (!CreateProcess(NULL,const_cast<LPTSTR>((LPCTSTR)trans_exe),NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) {
			theSERVICE_MGR.SetForeServiceBusy(false);
			return FALSE;
		}
		WaitForSingleObject(pi.hProcess, 60000 );        // 等待EXE结束(max 60sec)
		CloseHandle(hWrite);
		// 读取测试结果
		DWORD bytesRead;
		ReadFile(hRead,pBuff,MAX_PING_RESULT_LLENGTH-1,&bytesRead,NULL);

		// 返回网络测试结果
		if ( 0 == bytesRead ) {
			CString comment(_T("PingSC"));
			// 网络测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
			return FALSE;
		}
		else {
			CString comment(_T("PingSC"));
			// 网络测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
			return TRUE;
		}
	}
	catch(CSysException&) {
		CString comment(_T("PingSC"));
		// 网络测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
		throw;
	}
	catch (...) 
	{
		CString comment(_T("PingSC"));
		// 网络测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
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
//LRESULT CNetworkTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Enter键响应消息

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNetworkTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	//异常处理
	try
	{
		int index = theAPP_SESSION.GetInputToInt();

		int nMenuPath = 0;

		//Get Dlg point
		CNetworkItemMaintainDlg* pDlg = dynamic_cast<CNetworkItemMaintainDlg*>(m_pDialogFlow->GetDialog(CNetworkItemMaintainDlg::IDD));
		CNetworkItemMaintainDlg::Menu_Info& MenuInfo = pDlg->GetStructData();
		nMenuPath = _ttoi(MenuInfo.strMenuID);

		//当当前路径小于菜单路径时,认为是在主菜单栏
		if (nMenuPath <= 3)
		{
			switch(index)
			{
			case 1:
				{
					theAPP_SESSION.SetKeyboard(KEY_RETURN);
					theAPP_SESSION.SetMaxInputLength(0);
					//测试SC连通性
					theAPP_SESSION.ShowOfficeGuide(GUIDE_NET_TESTING);
					//设置一列显示
					pDlg->UpdateLableGroup(1);
					//实现Ping并现实
					pDlg->PingScConn();

					//设置向导显示
					theAPP_SESSION.ShowOfficeGuide(GUIDE_SC_INFO_CONFIRM);
					//设置当前位置与状态
					MenuInfo.strMenuID = _T("10");        //表示完成测试操作。1代表上级目录，0代表本级目录，\
					因为无子菜单，则用0表示。
					MenuInfo.strMenu_1_Status = COMPLETED;
				}
				break;
			case 2:
				{
					if(!theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
						//显示输入错误菜单编号。
						theAPP_SESSION.ShowOfficeGuide(_T("本账号无权限执行该操作。"));
						return FALSE;
					}
					//ftp上传测试
					pDlg->FtpUploading();
				}
				break;
			case 3:
				{
					if(!theAPP_SESSION.IsSuperUser() && theFunction_INFO.IsFounderOperation()){
						//显示输入错误菜单编号。
						theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ACCOUNT_ERROR)/*GUIDE_MENU_CODE_ILLEGAL*/);
						return FALSE;
					}
					//FTP下载测试
					pDlg->FtpDownloading();
				}
				break;
			default:
				{
					//显示输入错误菜单编号。
					theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
					return FALSE;
				}
			}

		}
		else
		{
			//否则显示输入错误信息
			theAPP_SESSION.ShowOfficeGuide(GUIDE_STATION_SET_INPUT_ERROR);
		}
		NotifyDataModelChanged();
	}
	catch (CSysException& e)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(GUIDE_TEST_FAIL);
	}
	catch (...)
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(DEVICE_TEST_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(GUIDE_TEST_FAIL);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      F3响应函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNetworkTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	int nMenuPath = 0;

	//Get Dlg point
	CNetworkItemMaintainDlg* pDlg = dynamic_cast<CNetworkItemMaintainDlg*>(m_pDialogFlow->GetDialog(CNetworkItemMaintainDlg::IDD));
	CNetworkItemMaintainDlg::Menu_Info& MenuInfo = pDlg->GetStructData();

	nMenuPath = _ttoi(MenuInfo.strMenuID);
	//AfxMessageBox(MenuInfo.strMenuID);

	if (nMenuPath > 4 )
	{
		pDlg->RebackToMenu();
		MenuInfo.strMenuID.Format(_T("0"));
		theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER);
		theAPP_SESSION.SetMaxInputLength(1);
	}
	else
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新界面显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CNetworkTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}