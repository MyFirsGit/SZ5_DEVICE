#include "stdafx.h"
#include "DataInputSvc.h"
#include "DataInputSelectMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char* const TXT_BTN_PAGE_UP		= "��һҳ";
static const char* const TXT_BTN_PAGE_DOWN		= "��һҳ";
static const char* const WM_BTN_PAGE_UP			= "PAGEUP";
static const char* const WM_BTN_PAGE_DOWN		= "PAGEDOWN";

// ������Ϣ�б�ͷ
static const char* const STR_KIND					= "��������";
static const char* const STR_LOCAL_VERSION			= "       ����(��ǰ/����)";
static const char* const STR_IMPORT_VERSION			= "     ����(��ǰ/����)";

IMPLEMENT_DYNAMIC(CDataInputSelectMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CDataInputSelectMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_MESSAGE(WM_XBUTTON,XButtonClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataInputSelectMaintainDlg::CDataInputSelectMaintainDlg(CService* pService) : 
	COperationDlg(CDataInputSelectMaintainDlg::IDD, pService)
{
	m_DataNum = 0;
	m_TotalPageNum = 0;
	m_CurrentPageNum = 0;

	m_baseInfo->titleInfo.titleName = "��������";						// ����
	m_baseInfo->detailInfo.row = 11;                                    // ����

	UIINFO pageUpButtonUI;
	pageUpButtonUI.m_pParentWnd = this;
	pageUpButtonUI.m_Location.y = BTN_OK_RECT.top;
	pageUpButtonUI.m_Location.x = 10;
	pageUpButtonUI.m_Size = CSize(80,50);
	BTN_INFO pageUpButtonInfo;
	pageUpButtonInfo.btnMsg = WM_BTN_PAGE_UP;
	pageUpButtonInfo.isEnable = true;
	pageUpButtonInfo.btnID = 0;
	pageUpButtonInfo.text = TXT_BTN_PAGE_UP;
	m_btnPageUp = new CXButton(pageUpButtonUI,pageUpButtonInfo);

	UIINFO pageDownButtonUI;
	pageDownButtonUI.m_pParentWnd = this;
	pageDownButtonUI.m_Location.y = BTN_OK_RECT.top;
	pageDownButtonUI.m_Location.x = 110;
	pageDownButtonUI.m_Size = CSize(80,50);
	BTN_INFO pageDownButtonInfo;
	pageDownButtonInfo.btnMsg = WM_BTN_PAGE_DOWN;
	pageDownButtonInfo.isEnable = true;
	pageDownButtonInfo.btnID = 0;
	pageDownButtonInfo.text = TXT_BTN_PAGE_DOWN;
	m_btnPageDown = new CXButton(pageDownButtonUI,pageDownButtonInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataInputSelectMaintainDlg::~CDataInputSelectMaintainDlg()
{
	delete m_btnPageUp;
	m_btnPageUp = NULL;

	delete m_btnPageDown;
	m_btnPageDown = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      OnCreate

@param      (i)LPCREATESTRUCT lpCreateStruct ���崴��������

@retval     int		\n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CDataInputSelectMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDataInputSvc* m_DataInputSvc = GetService<CDataInputSvc>();
	try{
		// ������������ȷ�ϰ�ť
		if (__super::OnCreate(lpCreateStruct,  GUIDE_AREA | BTN_OK) == -1){
			return -1;
		}
		m_btnPageUp->Create();
		m_btnPageDown->Create();

		delete m_baseInfoArea;
		m_baseInfoArea = NULL;
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LabelDataImportInfo);

		InitImportInfo();

		if(m_DataInputSvc->m_ErrorCode == 0){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_INPUT_SELECT));
			m_btnOK->SetVisible(true);
		}
		else if(m_DataInputSvc->m_ErrorCode == 1){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_INPUT_MDDIR_NOT_FIND));
			m_btnOK->SetVisible(false);
		}
		else if(m_DataInputSvc->m_ErrorCode == 2){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_INPUT_PARA_INDEX_NOT_EXIST));
			m_btnOK->SetVisible(false);
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_INPUT_PARA_INDEX_OTHER_ERROR));
			m_btnOK->SetVisible(false);
		}
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(m_DataInputSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ���쳣��Ϣ�б�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::InitImportInfo()
{
	m_LabelDataImportInfo[0].nameLeft = STR_KIND;
	m_LabelDataImportInfo[0].nameCenter = STR_LOCAL_VERSION;
	m_LabelDataImportInfo[0].nameRight = STR_IMPORT_VERSION;

	CDataInputSvc* m_DataInputSvc = GetService<CDataInputSvc>();
	map<WORD,CDataInputSvc::ParameterInputStruct> &dataModel = m_DataInputSvc->GetDataModel();
	m_DataNum = dataModel.size();
	if(m_DataNum == 0){
		for(int i = 1;i < m_baseInfo->detailInfo.row;i++)
		{
			m_LabelDataImportInfo[i].nameLeft = "";
			m_LabelDataImportInfo[i].nameCenter = "";
			m_LabelDataImportInfo[i].nameRight = "";
		}
		m_btnPageUp->SetVisible(false);
		m_btnPageDown->SetVisible(false);
	}
	else{
		m_TotalPageNum = (m_DataNum%(m_baseInfo->detailInfo.row - 1) == 0) ? m_DataNum/(m_baseInfo->detailInfo.row - 1) : m_DataNum/(m_baseInfo->detailInfo.row - 1) + 1;
		m_CurrentPageNum = 1;
		ShowImportData();
		if(m_TotalPageNum == 1){
			m_btnPageUp->SetVisible(false);
			m_btnPageDown->SetVisible(false);
		}
		else{
			m_btnPageUp->SetVisible(true);
			m_btnPageDown->SetVisible(true);
			m_btnPageUp->SetEnabled(false);
			m_btnPageDown->SetEnabled(true);
		}
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���»��� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSelectMaintainDlg::ShowImportData()
{
	CDataInputSvc* m_DataInputSvc = GetService<CDataInputSvc>();
	map<WORD,CDataInputSvc::ParameterInputStruct> &dataModel = m_DataInputSvc->GetDataModel();
	map<WORD,CDataInputSvc::ParameterInputStruct>::iterator iter = dataModel.begin();
	int indexStar = (m_CurrentPageNum - 1)*(m_baseInfo->detailInfo.row - 1);
	int indexEnd = m_CurrentPageNum*(m_baseInfo->detailInfo.row - 1) - 1;
	for(int k = 0;k < indexStar;k++)
	{
		iter++;
	}
	for(int i = 1;i < m_baseInfo->detailInfo.row;i++)
	{
		if(indexStar < m_DataNum){
			CDataInputSvc::ParameterInputStruct &inputStruct = iter->second;
			CString name;
			name.Format(_T("%s(%.2X%.2X)"),inputStruct.name,HIBYTE(iter->first),LOBYTE(iter->first));
			m_LabelDataImportInfo[i].nameLeft = name;
			CString strLocalCurrentVersion;
			if(inputStruct.localCurrentVersion == 0 || inputStruct.localCurrentVersion == -1){
				strLocalCurrentVersion = _T("--");
			}
			else{
				strLocalCurrentVersion.Format(_T("%d"),inputStruct.localCurrentVersion);
			}
			CString strLocalFutureVersion;
			if(inputStruct.localFutureVersion == 0 || inputStruct.localFutureVersion == -1){
				strLocalFutureVersion = _T("--");
			}
			else{
				strLocalFutureVersion.Format(_T("%d"),inputStruct.localFutureVersion);
			}
			CString strImportCurrentVersion;
			if(inputStruct.importCurrentVersion == 0 || inputStruct.importCurrentVersion == -1){
				strImportCurrentVersion = _T("--");
			}
			else{
				strImportCurrentVersion.Format(_T("%d"),inputStruct.importCurrentVersion);
			}
			CString strImportFutureVersion;
			if(inputStruct.importFutureVersion == 0 || inputStruct.importFutureVersion == -1){
				strImportFutureVersion = _T("--");
			}
			else{
				strImportFutureVersion.Format(_T("%d"),inputStruct.importFutureVersion);
			}
			m_LabelDataImportInfo[i].nameCenter = _T("       ") + strLocalCurrentVersion + _T("/") + strLocalFutureVersion;
			m_LabelDataImportInfo[i].nameRight = _T("     ") + strImportCurrentVersion + _T("/") + strImportFutureVersion;
			iter++;
		}
		else{
			m_LabelDataImportInfo[i].nameLeft = _T("");
			m_LabelDataImportInfo[i].nameCenter = _T("");
			m_LabelDataImportInfo[i].nameRight = _T("");
		}
		indexStar++;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataInputSelectMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CDataInputSvc* m_DataInputSvc = GetService<CDataInputSvc>();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// �ж��Ƿ��ѻ�״̬
		if (!theAPP_SESSION.IsSCConnected()){
			// ��ʼ��������
			if(m_DataInputSvc->ImportParameter()){
				// �ڵ�������ʾ�������
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(DUIDE_DATA_INPUT_FINISH_ACC));
			}
			else{
				// �ڵ�������ʾ����ʧ��
				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(DUIDE_DATA_INPUT_FAIL));
			}
			HideCompoment(BTN_OK);
		}
		else{
			CMessageBox::Show(_T("��ʾ"),_T("����״̬������������ݵ���"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(m_DataInputSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**8
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataInputSelectMaintainDlg::XButtonClick(WPARAM wParam, LPARAM lParam)
{
	CDataInputSvc* m_DataInputSvc = GetService<CDataInputSvc>();
	try {
		CString sMsg = (LPCTSTR)lParam;
		if(sMsg == WM_BTN_PAGE_UP){
			m_CurrentPageNum--;
			ShowImportData();
			if(m_CurrentPageNum == 1){
				m_btnPageUp->SetEnabled(false);
				m_btnPageDown->SetEnabled(true);
			}
			else{
				m_btnPageUp->SetEnabled(true);
				m_btnPageDown->SetEnabled(true);
			}
		}
		else if(sMsg == WM_BTN_PAGE_DOWN){
			m_CurrentPageNum++;
			ShowImportData();
			if(m_CurrentPageNum == m_TotalPageNum){
				m_btnPageUp->SetEnabled(true);
				m_btnPageDown->SetEnabled(false);
			}
			else{
				m_btnPageUp->SetEnabled(true);
				m_btnPageDown->SetEnabled(true);
			}
		}
		m_btnPageUp->SetSelect(false);
		m_btnPageDown->SetSelect(false);
		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(m_DataInputSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}