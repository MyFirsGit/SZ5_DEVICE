#include "stdafx.h"
#include "BOM.h"
#include "NetworkItemMaintainDlg.h"
#include "AppSession.h"

#include "FTPDef.h"
#include "FTPHelper.h"
#include "guideinfo.h"
#include "setting.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//编辑框位置
static CRect incomeFailure[] = 
{
	BASE_INFO_RECT(3,7),
};

//网络测试项目,几个BUTTON的名字
static const TCHAR* const NETWORK_CONNECT_BUTTON_NAME				=  _T("SC联机确认");
static const TCHAR* const NETWORK_FTPDOWNLOAD_BUTTON_NAME			=  _T("FTP下载测试");
static const TCHAR* const NETWORK_FTPUPLOAD_BUTTON_NAME				=  _T("FTP上传测试");

//网络测试项目,几个BUTTON的消息
static const TCHAR* const NETWORK_CONNECT_BUTTON_MSG					=  _T("SCCONNECT" )  ;  
static const TCHAR* const NETWORK_FTPDOWNLOAD_BUTTON_MSG				=  _T("FTPDOWNLOAD") ;  
static const TCHAR* const NETWORK_FTPUPLOAD_BUTTON_MSG				=  _T("FTPUPLOAD")   ;  

static const CRect			PING_RESULT_RECT						=	CRect(5,15,485,500);	
static const CRect			PING_RESULT_REFLASH_RECT                =	PING_RESULT_RECT + CRect(5,5,5,5);

//SC联机确认标签
static const TCHAR* const NETWORK_SC_CONNECT_LABEL					= _T("SC联机确认");
static const TCHAR* const INIT_NOTE_MSG								=  _T("网络");

static const TCHAR* const STR_TEST_FTP_FILE_NAME = _T("TestFtp.txt");

IMPLEMENT_DYNAMIC(CNetworkItemMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CNetworkItemMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnNetworkItemClick)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNetworkItemMaintainDlg::CNetworkItemMaintainDlg(CService* pService)
: COperationDlg(CNetworkItemMaintainDlg::IDD, pService)
{
	// SC联机确认
	BTN_INFO connectTestInfo;
	connectTestInfo.btnMsg = NETWORK_CONNECT_BUTTON_MSG;
	connectTestInfo.text=NETWORK_CONNECT_BUTTON_NAME;

	// FTP上传测试
	BTN_INFO ftpUploadInfo;
	ftpUploadInfo.btnMsg = NETWORK_FTPUPLOAD_BUTTON_MSG;
	ftpUploadInfo.text=NETWORK_FTPUPLOAD_BUTTON_NAME;

	// FTP下载测试
	BTN_INFO ftpDownloadInfo;
	ftpDownloadInfo.btnMsg =NETWORK_FTPDOWNLOAD_BUTTON_MSG;
	ftpDownloadInfo.text=NETWORK_FTPDOWNLOAD_BUTTON_NAME;

	// 添加按钮
	m_vec_Right_1_BTN_INFO.push_back(connectTestInfo);
	m_vec_Right_1_BTN_INFO.push_back(ftpUploadInfo);
	m_vec_Right_1_BTN_INFO.push_back(ftpDownloadInfo);

	m_bIsShowPingResult = false;		// 不显示Ping结果区域
	m_sPingResult = "";					// Ping结果字符串
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNetworkItemMaintainDlg::~CNetworkItemMaintainDlg()
{
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CNetworkItemMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_sNetworkItem = "";						// 初始化网络连接测试按钮消息
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	return __super::OnCreate(lpCreateStruct,GUIDE_AREA|BTN_OK);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNetworkItemMaintainDlg::OnNetworkItemClick(WPARAM wParam,LPARAM lParam)
{
	m_bIsShowPingResult = false;						// 不显示Ping结果区域
	BTN_INFO* pButtonInfo = (BTN_INFO*)lParam;

	m_sNetworkItem = pButtonInfo->btnMsg;
	InvalidateRect(PING_RESULT_REFLASH_RECT);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TEST_CHOOSE_ITEM));
	ShowCompoment(BTN_OK);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认、返回 按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNetworkItemMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try{
		CNetworkTestSvc* pNetworkTestSvc = (CNetworkTestSvc*)GetService();
		if (_T("") == m_sNetworkItem){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TEST_CHOOSE_ITEM));
			return 0;
		}
		// SC连接测试
		if(NETWORK_CONNECT_BUTTON_MSG == m_sNetworkItem){
			char result[CNetworkTestSvc::MAX_PING_RESULT_LLENGTH] = {0};
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TESTING));   
			theSERVICE_MGR.SetForeServiceBusy(true);
			pNetworkTestSvc->PingSC(result);
			CString strResult;
			strResult = result;
			CString strTrim;
			strTrim = strResult.Trim();
			m_sPingResult = add_hsp_right(strTrim,strTrim.GetLength());
			m_bIsShowPingResult = true;
			theSERVICE_MGR.SetForeServiceBusy(false);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_SC_INFO_CONFIRM));
			InvalidateRect(PING_RESULT_REFLASH_RECT);
		}
		// FTP下载测试
		else if ( NETWORK_FTPDOWNLOAD_BUTTON_MSG == m_sNetworkItem ){
			try{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TESTING));
				theSERVICE_MGR.SetForeServiceBusy(true);
				// 连接失败
				if(!theFTP_HELPER.FTPConnect()){
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_CONNECT_FAIL));
					theSERVICE_MGR.SetForeServiceBusy(false);
					
				}
				// 连接成功
				else{
					CString downloadFileRemotePath = theSETTING.GetFileUploadFTPDir() + STR_TEST_FTP_FILE_NAME;
					CString downloadFileLocalDir =theBOM_INFO.GetLogPath() ;
					bool downLoadSuccess = theFTP_HELPER.FTPDownloadFile(downloadFileRemotePath,downloadFileLocalDir);
					// 下载失败
					if (!downLoadSuccess){
						theFTP_HELPER.FTPClose();
						theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_DOWNLOAD_FAIL));
						theSERVICE_MGR.SetForeServiceBusy(false);
						return -1;
					}
					// 删除下载文件
					DeleteFile(downloadFileLocalDir + STR_TEST_FTP_FILE_NAME);
					theFTP_HELPER.FTPClose();
					theSERVICE_MGR.SetForeServiceBusy(false);
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_DOWNLOAD_SUCCESS));
				}
			}
			catch (...) {
				theSERVICE_MGR.SetForeServiceBusy(false);
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_DOWNLOAD_FAIL));
			}
		}
		// FTP上传测试
		else if(NETWORK_FTPUPLOAD_BUTTON_MSG == m_sNetworkItem){
			try{
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TESTING));
				theSERVICE_MGR.SetForeServiceBusy(true);
				// 连接失败
				if (!theFTP_HELPER.FTPConnect()){
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_CONNECT_FAIL));
					theSERVICE_MGR.SetForeServiceBusy(false);
				}
				// 连接成功
				else{
					CString uploadFileRemoteDir   = theSETTING.GetFileUploadFTPDir();
					CString uploadFileLocalPath  = theBOM_INFO.GetLogPath() + STR_TEST_FTP_FILE_NAME;
					CXFile testFtpFile(uploadFileLocalPath);
					testFtpFile.WriteFile(_T("TestFtp"));
					testFtpFile.Close();
					bool uploadSuccess = theFTP_HELPER.FTPUploadFile(uploadFileLocalPath,uploadFileRemoteDir);
					DeleteFile(uploadFileLocalPath);
					// 上传失败
					if(!uploadSuccess){
						theFTP_HELPER.FTPClose();
						theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_UPLOAD_FAIL));
						theSERVICE_MGR.SetForeServiceBusy(false);
						return -1;
					}
					bool bClose = theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_UPLOAD_SUCCESS));
				}
				theSERVICE_MGR.SetForeServiceBusy(false);
			}
			catch (...) {
				theSERVICE_MGR.SetForeServiceBusy(false);
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_FTP_UPLOAD_FAIL));
			}
		}
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TEST_CHOOSE_ITEM));
	}
	catch (...) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_NET_TEST_CHOOSE_ITEM));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNetworkItemMaintainDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	try{
		// 绘制ping结果显示区域
		if (m_bIsShowPingResult) {
			DrawRoundRect(pDC, PING_RESULT_RECT, LINE_BORDER, BLACKGRAY, GRAY);
			CRect rect = PING_RESULT_RECT;
			rect.InflateRect(-15,-10,5,5);

			DrawText(pDC, rect, m_sPingResult.Trim(), theBOM_INFO.GetOperateBtnFont(), theBOM_INFO.GetOperateBtnFontSize(),FW_NORMAL, BLACK,DT_LEFT);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
