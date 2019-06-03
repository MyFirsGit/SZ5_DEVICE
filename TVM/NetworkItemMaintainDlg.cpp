#include "StdAfx.h"
#include "NetworkItemMaintainDlg.h"
#include "FTPDef.h"
#include "FTPHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//映射消息到对话框
IMPLEMENT_DYNAMIC(CNetworkItemMaintainDlg,COperationDlg)
	BEGIN_MESSAGE_MAP(CNetworkItemMaintainDlg,COperationDlg)
		ON_WM_ACTIVATE()
		ON_WM_CREATE()
	END_MESSAGE_MAP()

	//Ftp测试文件名
	static const TCHAR* const STR_TEST_FTP_FILE_NAME = _T("TestFtp.txt");
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      标准构造函数

	@param      (i) CService pService

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CNetworkItemMaintainDlg::CNetworkItemMaintainDlg(CService* pService)
		: COperationDlg(CNetworkItemMaintainDlg::IDD,pService)
	{
		m_baseInfo->detailInfo.row        = _MAX_ROW;
		m_baseInfo->detailInfo.labelGroup = m_lablegroup;

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CNetworkItemMaintainDlg::~CNetworkItemMaintainDlg()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示菜单选项到控件

	@param      (i)LABEL_GROUP* pLableGroup  
	@param      (i)UINT uLableGroupNum

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::SetLableGroup(const LABEL_GROUP* pLableGroup,const UINT uLableGroupNum)
	{
		UINT uThisNum = 0;
		for( ; uThisNum < _MAX_ROW; uThisNum++)
		{
			if (uThisNum < uLableGroupNum)
			{
				const LABEL_GROUP* pTemp = pLableGroup + uThisNum; //定位到当前的Lable
				m_lablegroup[uThisNum].nameLeft = pTemp->nameLeft;
				m_lablegroup[uThisNum].nameRight = pTemp->nameRight;
			}
			else
			{
				m_lablegroup[uThisNum].nameLeft = _T("");
				m_lablegroup[uThisNum].nameRight = _T("");
			}
		}

	}

	////////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面创建函数

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:创建成功    -1:创建失败

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CNetworkItemMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		//创建基本显示和向导显示区
		if (-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA)) 
		{
			return -1;
		}

		//设置GStatic宽度
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 400);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 200);

		//设置GStatic显示属性，左对齐
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_RIGHT);


		return 0; //Return OK
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面激活,当多个画面写在同一个dlg中的时候，需要经过OnActive来激活画面。

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		__super::OnActivate(nState,pWndOther,bMinimized);
		theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_INPUT|KEY_ENTER);
		theAPP_SESSION.SetMaxInputLength(1);
		ClearLableData();

		LABEL_GROUP showMenu[3];                            //菜单选项
		showMenu[0].nameLeft = _T("[1]");
		showMenu[0].nameLeft+= _opl(SC_TEST_TEXT_CONNECTION);

		if(theAPP_SESSION.IsSuperUser() || theFunction_INFO.IsFounderOperation()){	
			showMenu[1].nameLeft = _T("[2]");
			showMenu[1].nameLeft+= _opl(SC_TEST_TEXT_FTP_UPLOAD);

			showMenu[2].nameLeft = _T("[3]");
			showMenu[2].nameLeft+= _opl(SC_TEST_TEXT_FTP_DOWNLOAD);
		}
		SetLableGroup(showMenu,3);                         //显示到MainLableGroup

		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);   //设置提示向导文言
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      局部更新数据,单元格

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::UpdateAreaLable(int nRow,int nCol,CString strValue,COLORREF itemColor)
	{
		if (0 == nCol)
		{
			m_lablegroup[nRow].nameLeft = strValue;
		}
		else if (1 == nCol)
		{
			m_lablegroup[nRow].nameRight = strValue;
		}

		m_baseInfoArea->SetLabelColor(nRow,nCol,itemColor);

		//UpdateData(TRUE);
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      刷新值到控件

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::reflushBaseArea()
	{
		//与UpdateData(TRUE)类似
		m_baseInfoArea->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      清空Lable对话框数据

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::ClearLableData()
	{
		for ( int nRow = 0; nRow < _MAX_ROW; nRow++)
		{
			m_lablegroup[nRow].nameLeft  = _T("");
			m_lablegroup[nRow].nameRight = _T("");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief     更新基础组件显示组

	@param      (i)显示的列数

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	BOOL CNetworkItemMaintainDlg::UpdateLableGroup(int nCol)
	{
		BOOL bRet = FALSE;
		//只显示一列
		if(1 == nCol)
		{
			ClearLableData();
			//设置GStatic宽度
			m_baseInfoArea->SetLableWidth(0,RESOLUTION * 600);
			m_baseInfoArea->SetLableWidth(1,0);

			//设置GStatic显示属性，左对齐
			m_baseInfoArea->SetLableAlign(0,DT_TOP | DT_LEFT);
			m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
		}
		else if( 2 == nCol)
		{
			ClearLableData();
			//设置GStatic宽度
			m_baseInfoArea->SetLableWidth(0,RESOLUTION * 400);
			m_baseInfoArea->SetLableWidth(1,RESOLUTION * 200);

			//设置GStatic显示属性，右对齐
			m_baseInfoArea->SetLableAlign(0,DT_LEFT);
			m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
		}

		bRet = TRUE;
		return bRet;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      将Ping返回的字符窜以Ping的正常方式显示在static表格中间。

	@param      (i)被切割的字符串，以$为分割;(o)分割好的字符串集

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::CutCStringToPingStyle(CString& strBeCut,vector<CString>& vStrPing)
	{
		CString strTemp("");
		CString strGetCut(strBeCut);
		vStrPing.clear();

		if (!strBeCut.IsEmpty())
		{
			while(TRUE)
			{
				strTemp = "";
				strTemp = strGetCut.Left(strGetCut.Find(_T("$")));

				//1.如果不为空，直接放在容器中
				if (!strTemp.IsEmpty())
				{
					vStrPing.push_back(strTemp);
				}
				//2.如果不是尾字符串，填入空显示
				else if(strGetCut.Find(_T("$")) != -1)
				{
					vStrPing.push_back(_T(" "));
				}
				//3.如果为尾字符串，直接填入容器中
				else
				{
					strTemp = strGetCut;
					vStrPing.push_back(strTemp);
					//跳出循环
					break; 
				}
				//截取字符串
				strGetCut = strGetCut.Right(strGetCut.GetLength() - strGetCut.Find(_T("$")) - 1);
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      Ping测试显示在界面上

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::PingScConn()
	{
		CNetworkTestSvc* pServices = (CNetworkTestSvc*)GetService();
		char result[CNetworkTestSvc::MAX_PING_RESULT_LLENGTH] = {0};
		theSERVICE_MGR.SetForeServiceBusy(true);

		//服务函数取得Ping返回的结果
		pServices->PingSC(result);

		//统一格式
		CString strResult;
		CString strTrim;
		CString strShowUp;

		strResult = result;	
		strTrim = strResult.Trim();
		strShowUp = add_hsp_right(strTrim,strTrim.GetLength());

		//替换换行符为$，为了比较容易分割字符串
		strShowUp.Replace(_T("\r\n"),_T("$"));


		//分割字符串，显示到界面
		vector<CString> vStrData;
		CutCStringToPingStyle(strShowUp,vStrData);
		vector<CString>::iterator it = vStrData.begin();
		int nRow = 0;
		while(it != vStrData.end())
		{
			UpdateAreaLable(nRow,0,*it,BLACK);
			nRow++;
			it++;
			//当现实行数大于最大行时跳出循环
			if (nRow > _MAX_ROW)
			{
				break;
			}
		}
		//AfxMessageBox(strShowUp);
		theSERVICE_MGR.SetForeServiceBusy(false);

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      FTP上传测试

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::FtpUploading()
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_NET_TESTING);
		theSERVICE_MGR.SetForeServiceBusy(true);
		try
		{
			if (!theFTP_HELPER.FTPConnect())
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_CONNECT_FAIL);
				theSERVICE_MGR.SetForeServiceBusy(false);
				//显示错误结果
				UpdateAreaLable(1,1,FAILED,RED);
				m_Menu_Info.strMenu_2_Status = FAILED;

				CString comment(_T("FtpUploading"));
				// 网络测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
			}
			else
			{
				// 取得FTP上传路径
				CString strUploadFileDir = theTVM_SETTING.GetFileUploadFTPDir();
				//创建本地上传文件及路径
				CString strUploadFileLocalPath = CreateFTPTestFile();
				bool bUploadTest = theFTP_HELPER.FTPUploadFile(strUploadFileLocalPath,strUploadFileDir);

				DeleteFile(strUploadFileLocalPath);

				if (!bUploadTest)
				{
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_UPLOAD_FAIL);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//显示错误结果
					UpdateAreaLable(1,1,FAILED,RED);
					m_Menu_Info.strMenu_2_Status = FAILED;

					CString comment(_T("FtpUploading"));
					// 网络测试--操作日志
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
					return ;
				}
				else
				{
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_UPLOAD_SUCCESS);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//显示正确结果
					UpdateAreaLable(1,1,SUCCESS,SOFTGREEN);
					m_Menu_Info.strMenu_2_Status = SUCCESS;

					CString comment(_T("FtpUploading"));
					// 网络测试--操作日志
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
				}

			}
		}
		catch(...)
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_CONNECT_FAIL);
			theSERVICE_MGR.SetForeServiceBusy(false);
			//显示错误结果
			UpdateAreaLable(1,1,FAILED,RED);
			m_Menu_Info.strMenu_2_Status = FAILED;

			CString comment(_T("FtpUploading"));
			// 网络测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
		}
		reflushBaseArea();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      FTP下载测试

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::FtpDownloading()
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_NET_TESTING);
		theSERVICE_MGR.SetForeServiceBusy(true);
		try
		{
			if (!theFTP_HELPER.FTPConnect())
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_CONNECT_FAIL);
				theSERVICE_MGR.SetForeServiceBusy(false);
				//显示错误结果
				UpdateAreaLable(2,1,FAILED,RED);
				m_Menu_Info.strMenu_3_Status = FAILED;

				CString comment(_T("FtpDownloading"));
				// 网络测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
			}
			else
			{
				// 取得FTP下载文件
				CString strDownloadFileDir = theTVM_SETTING.GetFileUploadFTPDir() + STR_TEST_FTP_FILE_NAME;
				//本地文件保存路径
				CString strDownloadDir = theTVM_INFO.GetLogPath();
				bool bUploadTest = theFTP_HELPER.FTPDownloadFile(strDownloadFileDir,strDownloadDir);

				if (!bUploadTest)
				{
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_DOWNLOAD_FAIL);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//显示错误结果
					UpdateAreaLable(2,1,FAILED,RED);
					m_Menu_Info.strMenu_3_Status = FAILED;

					CString comment(_T("FtpDownloading"));
					// 网络测试--操作日志
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
					return ;
				}
				else
				{
					//删除下载的测试文件
					DeleteFile(strDownloadDir + STR_TEST_FTP_FILE_NAME);

					//关闭连接
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_DOWNLOAD_SUCCESS);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//显示正确结果
					UpdateAreaLable(2,1,SUCCESS,SOFTGREEN);
					m_Menu_Info.strMenu_3_Status = SUCCESS;

					CString comment(_T("FtpDownloading"));
					// 网络测试--操作日志
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
				}

			}
		}
		catch(...)
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_CONNECT_FAIL);
			theSERVICE_MGR.SetForeServiceBusy(false);
			//显示错误结果
			UpdateAreaLable(2,1,FAILED,RED);
			m_Menu_Info.strMenu_3_Status = FAILED;

			CString comment(_T("FtpDownloading"));
			// 网络测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
		}
		reflushBaseArea();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      创建上传FTP测试文件

	@param      none

	@retval     CString 文件路径

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CString CNetworkItemMaintainDlg::CreateFTPTestFile()
	{
		CString uploadFileLocalPath  = theTVM_INFO.GetLogPath() + STR_TEST_FTP_FILE_NAME;
		// 写入测试数据到测试文件中
		CXFile testFtpFile(uploadFileLocalPath);
		CString fileName = _T("TestFtp");
		testFtpFile.WriteFile(fileName);
		return uploadFileLocalPath;
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      结构体传递值接口

	@param      none

	@retval     m_Menu_Info

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CNetworkItemMaintainDlg::Menu_Info& CNetworkItemMaintainDlg::GetStructData()
	{
		return m_Menu_Info;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      F3响应事件，返回上级菜单

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::RebackToMenu()
	{
		ClearLableData();
		//恢复原有显示格式
		UpdateLableGroup(2);

		LABEL_GROUP showMenu[3];                            //菜单选项
		showMenu[0].nameLeft = _T("[1] ");
		showMenu[0].nameLeft+= _opl(SC_TEST_TEXT_CONNECTION);

		if(theAPP_SESSION.IsSuperUser()|| theFunction_INFO.IsFounderOperation()){
			showMenu[1].nameLeft = _T("[2] ");
			showMenu[1].nameLeft+= _opl(SC_TEST_TEXT_FTP_UPLOAD);

			showMenu[2].nameLeft = _T("[3] ");
			showMenu[2].nameLeft+= _opl(SC_TEST_TEXT_FTP_DOWNLOAD);
		}	

		SetLableGroup(showMenu,3);                         //显示到MainLableGroup

		UpdateAreaLable(0,1,m_Menu_Info.strMenu_1_Status,SOFTGREEN);
		if(theAPP_SESSION.IsSuperUser()|| theFunction_INFO.IsFounderOperation()){
			UpdateAreaLable(1,1,m_Menu_Info.strMenu_2_Status,m_Menu_Info.strMenu_2_Status == SUCCESS ? SOFTGREEN : RED);
			UpdateAreaLable(2,1,m_Menu_Info.strMenu_3_Status,m_Menu_Info.strMenu_3_Status == SUCCESS ? SOFTGREEN : RED);
		}
		//刷新界面
		reflushBaseArea();
	}
