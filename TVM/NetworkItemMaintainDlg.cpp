#include "StdAfx.h"
#include "NetworkItemMaintainDlg.h"
#include "FTPDef.h"
#include "FTPHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//ӳ����Ϣ���Ի���
IMPLEMENT_DYNAMIC(CNetworkItemMaintainDlg,COperationDlg)
	BEGIN_MESSAGE_MAP(CNetworkItemMaintainDlg,COperationDlg)
		ON_WM_ACTIVATE()
		ON_WM_CREATE()
	END_MESSAGE_MAP()

	//Ftp�����ļ���
	static const TCHAR* const STR_TEST_FTP_FILE_NAME = _T("TestFtp.txt");
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��׼���캯��

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
	@brief      ��������

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
	@brief      ��ʾ�˵�ѡ��ؼ�

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
				const LABEL_GROUP* pTemp = pLableGroup + uThisNum; //��λ����ǰ��Lable
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
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct  

	@retval     int    0:�����ɹ�    -1:����ʧ��

	@exception  CSysException
	*/
	////////////////////////////////////////////////////////////////////////////
	int CNetworkItemMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		//����������ʾ������ʾ��
		if (-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA)) 
		{
			return -1;
		}

		//����GStatic���
		m_baseInfoArea->SetLableWidth(0,RESOLUTION * 400);
		m_baseInfoArea->SetLableWidth(1,RESOLUTION * 200);

		//����GStatic��ʾ���ԣ������
		m_baseInfoArea->SetLableAlign(0,DT_LEFT);
		m_baseInfoArea->SetLableAlign(1,DT_RIGHT);


		return 0; //Return OK
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���漤��,���������д��ͬһ��dlg�е�ʱ����Ҫ����OnActive������档

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

		LABEL_GROUP showMenu[3];                            //�˵�ѡ��
		showMenu[0].nameLeft = _T("[1]");
		showMenu[0].nameLeft+= _opl(SC_TEST_TEXT_CONNECTION);

		if(theAPP_SESSION.IsSuperUser() || theFunction_INFO.IsFounderOperation()){	
			showMenu[1].nameLeft = _T("[2]");
			showMenu[1].nameLeft+= _opl(SC_TEST_TEXT_FTP_UPLOAD);

			showMenu[2].nameLeft = _T("[3]");
			showMenu[2].nameLeft+= _opl(SC_TEST_TEXT_FTP_DOWNLOAD);
		}
		SetLableGroup(showMenu,3);                         //��ʾ��MainLableGroup

		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);   //������ʾ������
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �ֲ���������,��Ԫ��

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
	@brief      ˢ��ֵ���ؼ�

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::reflushBaseArea()
	{
		//��UpdateData(TRUE)����
		m_baseInfoArea->Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���Lable�Ի�������

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
	@brief     ���»��������ʾ��

	@param      (i)��ʾ������

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	BOOL CNetworkItemMaintainDlg::UpdateLableGroup(int nCol)
	{
		BOOL bRet = FALSE;
		//ֻ��ʾһ��
		if(1 == nCol)
		{
			ClearLableData();
			//����GStatic���
			m_baseInfoArea->SetLableWidth(0,RESOLUTION * 600);
			m_baseInfoArea->SetLableWidth(1,0);

			//����GStatic��ʾ���ԣ������
			m_baseInfoArea->SetLableAlign(0,DT_TOP | DT_LEFT);
			m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
		}
		else if( 2 == nCol)
		{
			ClearLableData();
			//����GStatic���
			m_baseInfoArea->SetLableWidth(0,RESOLUTION * 400);
			m_baseInfoArea->SetLableWidth(1,RESOLUTION * 200);

			//����GStatic��ʾ���ԣ��Ҷ���
			m_baseInfoArea->SetLableAlign(0,DT_LEFT);
			m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
		}

		bRet = TRUE;
		return bRet;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��Ping���ص��ַ�����Ping��������ʽ��ʾ��static����м䡣

	@param      (i)���и���ַ�������$Ϊ�ָ�;(o)�ָ�õ��ַ�����

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

				//1.�����Ϊ�գ�ֱ�ӷ���������
				if (!strTemp.IsEmpty())
				{
					vStrPing.push_back(strTemp);
				}
				//2.�������β�ַ������������ʾ
				else if(strGetCut.Find(_T("$")) != -1)
				{
					vStrPing.push_back(_T(" "));
				}
				//3.���Ϊβ�ַ�����ֱ������������
				else
				{
					strTemp = strGetCut;
					vStrPing.push_back(strTemp);
					//����ѭ��
					break; 
				}
				//��ȡ�ַ���
				strGetCut = strGetCut.Right(strGetCut.GetLength() - strGetCut.Find(_T("$")) - 1);
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      Ping������ʾ�ڽ�����

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

		//������ȡ��Ping���صĽ��
		pServices->PingSC(result);

		//ͳһ��ʽ
		CString strResult;
		CString strTrim;
		CString strShowUp;

		strResult = result;	
		strTrim = strResult.Trim();
		strShowUp = add_hsp_right(strTrim,strTrim.GetLength());

		//�滻���з�Ϊ$��Ϊ�˱Ƚ����׷ָ��ַ���
		strShowUp.Replace(_T("\r\n"),_T("$"));


		//�ָ��ַ�������ʾ������
		vector<CString> vStrData;
		CutCStringToPingStyle(strShowUp,vStrData);
		vector<CString>::iterator it = vStrData.begin();
		int nRow = 0;
		while(it != vStrData.end())
		{
			UpdateAreaLable(nRow,0,*it,BLACK);
			nRow++;
			it++;
			//����ʵ�������������ʱ����ѭ��
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
	@brief      FTP�ϴ�����

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
				//��ʾ������
				UpdateAreaLable(1,1,FAILED,RED);
				m_Menu_Info.strMenu_2_Status = FAILED;

				CString comment(_T("FtpUploading"));
				// �������--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
			}
			else
			{
				// ȡ��FTP�ϴ�·��
				CString strUploadFileDir = theTVM_SETTING.GetFileUploadFTPDir();
				//���������ϴ��ļ���·��
				CString strUploadFileLocalPath = CreateFTPTestFile();
				bool bUploadTest = theFTP_HELPER.FTPUploadFile(strUploadFileLocalPath,strUploadFileDir);

				DeleteFile(strUploadFileLocalPath);

				if (!bUploadTest)
				{
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_UPLOAD_FAIL);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//��ʾ������
					UpdateAreaLable(1,1,FAILED,RED);
					m_Menu_Info.strMenu_2_Status = FAILED;

					CString comment(_T("FtpUploading"));
					// �������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
					return ;
				}
				else
				{
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_UPLOAD_SUCCESS);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//��ʾ��ȷ���
					UpdateAreaLable(1,1,SUCCESS,SOFTGREEN);
					m_Menu_Info.strMenu_2_Status = SUCCESS;

					CString comment(_T("FtpUploading"));
					// �������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
				}

			}
		}
		catch(...)
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_CONNECT_FAIL);
			theSERVICE_MGR.SetForeServiceBusy(false);
			//��ʾ������
			UpdateAreaLable(1,1,FAILED,RED);
			m_Menu_Info.strMenu_2_Status = FAILED;

			CString comment(_T("FtpUploading"));
			// �������--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
		}
		reflushBaseArea();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      FTP���ز���

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
				//��ʾ������
				UpdateAreaLable(2,1,FAILED,RED);
				m_Menu_Info.strMenu_3_Status = FAILED;

				CString comment(_T("FtpDownloading"));
				// �������--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
			}
			else
			{
				// ȡ��FTP�����ļ�
				CString strDownloadFileDir = theTVM_SETTING.GetFileUploadFTPDir() + STR_TEST_FTP_FILE_NAME;
				//�����ļ�����·��
				CString strDownloadDir = theTVM_INFO.GetLogPath();
				bool bUploadTest = theFTP_HELPER.FTPDownloadFile(strDownloadFileDir,strDownloadDir);

				if (!bUploadTest)
				{
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_DOWNLOAD_FAIL);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//��ʾ������
					UpdateAreaLable(2,1,FAILED,RED);
					m_Menu_Info.strMenu_3_Status = FAILED;

					CString comment(_T("FtpDownloading"));
					// �������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
					return ;
				}
				else
				{
					//ɾ�����صĲ����ļ�
					DeleteFile(strDownloadDir + STR_TEST_FTP_FILE_NAME);

					//�ر�����
					theFTP_HELPER.FTPClose();
					theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_DOWNLOAD_SUCCESS);
					theSERVICE_MGR.SetForeServiceBusy(false);
					//��ʾ��ȷ���
					UpdateAreaLable(2,1,SUCCESS,SOFTGREEN);
					m_Menu_Info.strMenu_3_Status = SUCCESS;

					CString comment(_T("FtpDownloading"));
					// �������--������־
					theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,true,comment);
				}

			}
		}
		catch(...)
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_FTP_CONNECT_FAIL);
			theSERVICE_MGR.SetForeServiceBusy(false);
			//��ʾ������
			UpdateAreaLable(2,1,FAILED,RED);
			m_Menu_Info.strMenu_3_Status = FAILED;

			CString comment(_T("FtpDownloading"));
			// �������--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_PINPAD_UNIT_TEST,false,comment);
		}
		reflushBaseArea();
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �����ϴ�FTP�����ļ�

	@param      none

	@retval     CString �ļ�·��

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	CString CNetworkItemMaintainDlg::CreateFTPTestFile()
	{
		CString uploadFileLocalPath  = theTVM_INFO.GetLogPath() + STR_TEST_FTP_FILE_NAME;
		// д��������ݵ������ļ���
		CXFile testFtpFile(uploadFileLocalPath);
		CString fileName = _T("TestFtp");
		testFtpFile.WriteFile(fileName);
		return uploadFileLocalPath;
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �ṹ�崫��ֵ�ӿ�

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
	@brief      F3��Ӧ�¼��������ϼ��˵�

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CNetworkItemMaintainDlg::RebackToMenu()
	{
		ClearLableData();
		//�ָ�ԭ����ʾ��ʽ
		UpdateLableGroup(2);

		LABEL_GROUP showMenu[3];                            //�˵�ѡ��
		showMenu[0].nameLeft = _T("[1] ");
		showMenu[0].nameLeft+= _opl(SC_TEST_TEXT_CONNECTION);

		if(theAPP_SESSION.IsSuperUser()|| theFunction_INFO.IsFounderOperation()){
			showMenu[1].nameLeft = _T("[2] ");
			showMenu[1].nameLeft+= _opl(SC_TEST_TEXT_FTP_UPLOAD);

			showMenu[2].nameLeft = _T("[3] ");
			showMenu[2].nameLeft+= _opl(SC_TEST_TEXT_FTP_DOWNLOAD);
		}	

		SetLableGroup(showMenu,3);                         //��ʾ��MainLableGroup

		UpdateAreaLable(0,1,m_Menu_Info.strMenu_1_Status,SOFTGREEN);
		if(theAPP_SESSION.IsSuperUser()|| theFunction_INFO.IsFounderOperation()){
			UpdateAreaLable(1,1,m_Menu_Info.strMenu_2_Status,m_Menu_Info.strMenu_2_Status == SUCCESS ? SOFTGREEN : RED);
			UpdateAreaLable(2,1,m_Menu_Info.strMenu_3_Status,m_Menu_Info.strMenu_3_Status == SUCCESS ? SOFTGREEN : RED);
		}
		//ˢ�½���
		reflushBaseArea();
	}
