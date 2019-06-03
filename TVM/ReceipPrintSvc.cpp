#include "stdafx.h"
#include <locale.h>
#include <algorithm>
#include "ReceipPrintSvc.h"
#include "ReceipPrintBaseDlg.h"
#include "PrinterHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_DLG_MAIN_TO_FILE_LIST = _T("WM_DLG_MAIN_TO_FILE_LIST");
static const TCHAR* const WM_DLG_FILE_LIST_TO_MAIN = _T("WM_DLG_FILE_LIST_TO_MAIN");

BEGIN_MESSAGE_MAP(CReceipPrintSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceipPrintSvc::CReceipPrintSvc()
	:CTVMForeService(RECEIPT_REPRINT_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG, new CReceipPrintBaseDlg(this));

	// ѡ����Ӫ����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MAIN_MENU, _opl(GUIDE_ENTER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MAIN_MENU, CReceipPrintBaseDlg::IDD);
	// ѡ��������µ�ƾ֤
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FILE_LIST, _opl(GUIDE_ENTER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FILE_LIST, CReceipPrintBaseDlg::IDD);
	

	// ������
	m_pDialogFlow->AddFlow(DIALOG_GROUP_MAIN_MENU, WM_DLG_MAIN_TO_FILE_LIST, DIALOG_GROUP_FILE_LIST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_FILE_LIST, WM_DLG_FILE_LIST_TO_MAIN, DIALOG_GROUP_MAIN_MENU);
	
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MAIN_MENU);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceipPrintSvc::~CReceipPrintSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������֮ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::OnStart()
{
	__super::OnStart();
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_MAIN_MENU);
	InitAllPrintedFile();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CReceipPrintSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	try{
		int index = theAPP_SESSION.GetInputToInt();

		if(GetCurDialogGroupID() == DIALOG_GROUP_MAIN_MENU){
			if(index == MENU_CODE_7){
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_PRINT_TRY_WAIT));
				PRINTER_HELPER->PrintLastUnprintReceipt();
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_PRINT_SUCCESS_CHECK_STATUS));
				return TRUE;
			}
			if(index <= MENU_CODE_0 || index > MENU_CODE_6){
				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				return FALSE;
			}

			m_DataModel.curSelectMenu = (MENU_NUMBER)index;

			vector<CString>* vecCurVec;

			GetSelectedFileList(vecCurVec);

			int nPages = 0;
			int nCountPrintFile = 0;
			if(NULL != vecCurVec){
				nPages = vecCurVec->size() % MAX_ROW == 0 ? vecCurVec->size() / MAX_ROW : vecCurVec->size() / MAX_ROW + 1;
				nCountPrintFile = vecCurVec->size();
			}
			SetPageInfo(nPages,1,MAX_ROW);

			DoDialogFlow(WM_DLG_MAIN_TO_FILE_LIST);

			NotifyDataModelChanged();

			int nMaxInputLenght = 1;
			// Set max input menu
			while(nCountPrintFile/10){
				nCountPrintFile/=10;
				nMaxInputLenght++;
			}
			theAPP_SESSION.SetMaxInputLength(nMaxInputLenght);
		}
		else{
			DoPrintSelectFile(index);
		}
	}
	catch(CSysException & e){
		InitAllPrintedFile();
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		InitAllPrintedFile();
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CReceipPrintSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if(GetCurDialogGroupID() == DIALOG_GROUP_FILE_LIST){
		m_DataModel.curSelectMenu = MENU_NUM_NONE;
		DoDialogFlow(WM_DLG_FILE_LIST_TO_MAIN);
		theAPP_SESSION.SetMaxInputLength(1);
	}
	else{
		theAPP_SESSION.ReturnMaintainMenu();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ�������Ѵ�ӡ�����ļ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::InitAllPrintedFile(){
	m_DataModel = _tagModel();

	InitBalanceFileList(m_DataModel.vecBalanceFileList);

	InitPassangerFileList(m_DataModel.vecPassengerFileList);

	InitStaffPrintedFileList();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��ҵ�����ƾ֤�б�

@param      ��o��vector<CString>& vecFilesList;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::InitBalanceFileList(vector<CString>& vecFilesList){
	CString strFilePath(_T(""));

	strFilePath.Format(_T("%s%s"),theTVM_INFO.GetGetReceiptLogPath(),FILTER_AUDIT_LOG);

	SerchAllFilesFormPath(strFilePath,vecFilesList);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ�����г˿�ҵ����ϵ��б�

@param      (O)vector<CString>& vecFilesList;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::InitPassangerFileList(vector<CString>& vecFilesList){
	CString strFilePath(_T(""));

	strFilePath.Format(_T("%s%s"),theTVM_INFO.GetGetReceiptLogPath(),FILTER_PASSENGER_PRINT_HISTORY);

	SerchAllFilesFormPath(strFilePath,vecFilesList);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��վԱ�������б�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::InitStaffPrintedFileList(){
	CString strFilePath(_T(""));
	vector<CString> vecFilesList;

	strFilePath.Format(_T("%s%s"),theTVM_INFO.GetGetReceiptLogPath(),FILTER_STAFF_PRINT_HISTORY);

	SerchAllFilesFormPath(strFilePath,vecFilesList);

	// ��Staff���ݽ��з���
	for(auto iter = vecFilesList.rbegin(); iter != vecFilesList.rend();iter++){
		switch(PRINTER_HELPER->GetPrintTypeFromFileName(*iter)){
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_INSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_UNINSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_INSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_UNINSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLECTION:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_SUPPLY:
			m_DataModel.vecCHOperFileList.push_back(*iter);			// Ӳ�ҹ�����
			break;
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_INSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_UNINSTALL:
			m_DataModel.vecBHReciveFileList.push_back(*iter);		// ֽ�һ�����
			break;
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_INSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_UNINSTALL:
			m_DataModel.vecBHChangeFileList.push_back(*iter);		// ֽ��������
			break;
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_INSTALL:
		case RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_UNINSTALL:
			m_DataModel.vecTHOperFileList.push_back(*iter);			// Ʊ�������
			break;
		default:
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ӡѡ���ƾ��

@param      const CString& strToBePrintFilePathName;����Ҫ����ӡ��ƾ��·����

@retval     long lRet = 0:�ɹ�; �� 0 :ʧ��;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CReceipPrintSvc::TryPrintSelectedFile(const CString& strToBePrintFilePathName){

	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_RECEIPT_PRINT_FAILED));
		//CString strFilePath = strToBePrintFilePathName.Left(strToBePrintFilePathName.ReverseFind('\\'));
		Sleep(1000);
		//strFilePath += _T("\\");

		if(ComCheckFileExist(strToBePrintFilePathName)){

			CXFile cxFile(strToBePrintFilePathName);
			CStringArray csArr;
			CString strLine;

			while(cxFile.ReadString(strLine)){
				LPTSTR lStr = strLine.GetBuffer(0);
				int nSize   = strLine.GetLength();
				if(nSize != 0 && lStr[nSize -1] == '\r'){
					strLine.ReleaseBufferSetLength(nSize - 1);
				}
				ComPrintLineFormatForTemplate(strLine,csArr);
			}
			cxFile.Close();
			// To Print
			PRINTER_HELPER->PrintReceiptByBmp(&csArr,false,true);
			csArr.RemoveAll();
			strLine.ReleaseBuffer();

			theSERVICE_MGR.SetForeServiceBusy(false);
			return 0;
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_PRINT_FILE_NOT_EXISTENT));
		}
	}
	catch(CSysException & e){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_PRINT_FAILURE));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}

	theSERVICE_MGR.SetForeServiceBusy(false);
	return -1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ĳ��Ŀ¼�µ������ļ����������ļ���

@param      ��i��const CString& strPathName,
			 (o) vector<CString>& vecFilesList

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::SerchAllFilesFormPath(const CString& strPathName,vector<CString>& vecFilesList){
	// init path name
	CString strPath;
	strPath = strPathName;


	// Is empty?
	if(strPath.IsEmpty()){
		return;
	}
	else{
		if(strPath.Right(1) != _T("\\")) strPath += _T("\\");
	}

	CFileFind finder;
	CString sFileName;

	bool bIsFind = finder.FindFile(strPath + _T("*.*"));//*.TXT and *.txt

	while(bIsFind){
		bIsFind = finder.FindNextFile();
		if(finder.IsDots()){
			continue;
		}
		else if(finder.IsDirectory()){
			//Find directory level
			SerchAllFilesFormPath(finder.GetFilePath(),vecFilesList);
		}
		else{
			//Find file
			sFileName = finder.GetFilePath();
			if(PRINTER_HELPER->GetPrintTypeFromFileName(sFileName) != RECEIPT_PRINT_TYPE::PRINT_TYPE_NONE){
				vecFilesList.push_back(sFileName);
			}
		}
	}

	finder.Close();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ӡѡ�в˵��Ĵ�ӡ�ļ�

@param      int nSelectFileID;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::DoPrintSelectFile(int nSelectFileID){

	int nCurMaxInputID = 0;

	vector<CString>* vecCurVec;

	switch(m_DataModel.curSelectMenu){
	case MENU_NUM_BALANCE:
		nCurMaxInputID = m_DataModel.vecBalanceFileList.size();vecCurVec = &m_DataModel.vecBalanceFileList;
		break;
	case MENU_NUM_BH_RECV:
		nCurMaxInputID = m_DataModel.vecBHReciveFileList.size();vecCurVec = &m_DataModel.vecBHReciveFileList;
		break;
	case MENU_NUM_BH_CHANGE:
		nCurMaxInputID = m_DataModel.vecBHChangeFileList.size();vecCurVec = &m_DataModel.vecBHChangeFileList;
		break;
	case MENU_NUM_CH_RECEIPT:
		nCurMaxInputID = m_DataModel.vecCHOperFileList.size();vecCurVec = &m_DataModel.vecCHOperFileList;
		break;
	case MENU_NUM_TH_RECEIPT:
		nCurMaxInputID = m_DataModel.vecTHOperFileList.size();vecCurVec = &m_DataModel.vecTHOperFileList;
		break;
	case MENU_NUM_EXCEPTION_RECEIPT:
		nCurMaxInputID = m_DataModel.vecPassengerFileList.size();vecCurVec = &m_DataModel.vecPassengerFileList;
		break;
	default:
		break;
	}

	int nCurMinMenu = (GetCurrentPage() - 1)* MAX_ROW + 1;
	int nCurMaxMenu = (nCurMinMenu + MAX_ROW - 1 < nCurMaxInputID) ? (nCurMinMenu + MAX_ROW -1) : nCurMaxInputID;
	if(nSelectFileID < nCurMinMenu|| nSelectFileID > nCurMaxMenu){
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return;
	}
	
	CString strFilePathName = vecCurVec->at(nSelectFileID - 1);

	TryPrintSelectedFile(strFilePathName);

	NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ѡ��ǰ�˵���Ӧ�Ĵ�ӡ�б�

@param      vector<CString>* pVec;ָ��ǰѡ��˵��Ĵ�ӡ�б�

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceipPrintSvc::GetSelectedFileList(vector<CString>* &pVec){
	switch(m_DataModel.curSelectMenu){
	case MENU_NUM_BALANCE:
		pVec = &m_DataModel.vecBalanceFileList;
		break;
	case MENU_NUM_BH_RECV:
		pVec = &m_DataModel.vecBHReciveFileList;
		break;
	case MENU_NUM_BH_CHANGE:
		pVec = &m_DataModel.vecBHChangeFileList;
		break;
	case MENU_NUM_CH_RECEIPT:
		pVec = &m_DataModel.vecCHOperFileList;
		break;
	case MENU_NUM_TH_RECEIPT:
		pVec = &m_DataModel.vecTHOperFileList;
		break;
	case MENU_NUM_EXCEPTION_RECEIPT:
		pVec = &m_DataModel.vecPassengerFileList;
		break;
	default:
		pVec = NULL;
		break;
	}
}
