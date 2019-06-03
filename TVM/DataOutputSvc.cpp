#include "stdafx.h"
#include "DataOutputSvc.h"
#include "DataOutputSelectMaintainDlg.h"
#include "usbctl.h"
#include "GuideInfo.h"
#include "loghelper.h"
#include "CParamHelp.h"
#include "TxnDataEnum.h"

static const TCHAR* const EXPORT_INDEX_FILE  = _T("SleMemoryIndexFile.DAT");
static const TCHAR* const SECTION_FILE		= _T("FILE");
static const TCHAR* const KEY_FILE_TYPE		= _T("FileType");
static const TCHAR* const KEY_CREAT_TIME		= _T("Created_time");
static const TCHAR* const KEY_MODIFY_TIME	= _T("Modified_time");
static const TCHAR* const KEY_DEVICE_COUNT	= _T("Device_Count");
static const TCHAR* const KEY_DEVICE_NO		= _T("Dev");
static const TCHAR* const KEY_FILE_NAME		= _T("FileName_");
static const TCHAR* const KEY_FILE_CNT		= _T("FileCount");

static const TCHAR* const UNSEND_DIRECTORY = _T("unsend\\");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDataOutputSvc, CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSvc::CDataOutputSvc()
:CTVMForeService(DATA_OUTPUT_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20131_DATAOUTPUT_SELECT_DLG,new CDataOutputSelectMaintainDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DATAOUTPUT_SELECT, _opl(GUIDE_DATA_OUTPUT_NEW_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DATAOUTPUT_SELECT, IDD_20131_DATAOUTPUT_SELECT_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAOUTPUT_SELECT);

	m_DeviceID = theMAINTENANCE_INFO.GetCurrentDevice().ToString();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSvc::~CDataOutputSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����һ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSvc::OnStart(){
	__super::OnStart();
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAOUTPUT_SELECT);

	m_DeviceID = theMAINTENANCE_INFO.GetCurrentDevice().ToString();
	// �����ǲ���ʹ�øù���
	if(theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		// �ж��Ƿ����U��
		if(!PathIsDirectory(theTVM_INFO.GetUSBDrive())){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
			return;
		}
		else{
			if(theFunction_INFO.GetUSBAuthFlag() == 1){
				if(AuthUSB()){
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_NEW_SELECT));
				}
				else{
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
					return;
				}
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_NEW_SELECT));
				m_bIsAuthPass = true;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ж�USB�Ƿ���֤ͨ��

@param      ��

@retval     bool :true ͨ��; false ��ͨ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CDataOutputSvc::IsUSBAuthed(){
	return m_bIsAuthPass;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ذ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataOutputSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ�ϴ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataOutputSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	// �����ǲ���ʹ�øù���
	if(theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	int index = theAPP_SESSION.GetInputToInt();

	switch(index)
	{
	case MENU_CODE_1:
		{
			m_DataType = DATA_TYPE_CODE::BUS_DATA;
		}
		break;
	case MENU_CODE_2:
		{
			m_DataType = DATA_TYPE_CODE::AFC_TRANS;//AFC\ACC													
		}
		break;
	case MENU_CODE_3:
		{
			m_DataType = DATA_TYPE_CODE::LOG_DATA;
		}
		break;
	default:
		{
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return 0;
		}
		break;
	}

	// �ж��Ƿ����U��
	if(!PathIsDirectory(theTVM_INFO.GetUSBDrive())){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
		return -1;
	}

	//��֤U��
	if(!IsUSBAuthed()){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
		return -1;
	}
	//��ʾ���ڵ�������
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUTING));
	theSERVICE_MGR.SetForeServiceBusy(true);
	long errCode = ExportParameter(m_DataType);
	//AFC����
	if (m_DataType == DATA_TYPE_CODE::AFC_TRANS && errCode == ERROR_CODE_NORMAL)
	{
		//ACC����
		m_DataType = DATA_TYPE_CODE::ACC_TRANS;
		errCode = ExportParameter(m_DataType);
	}

	if(errCode == ERROR_CODE_CREATE_FILE_FAILED)
	{
	}
	else if(errCode == ERROR_CODE_OTHER_ERROR)
	{		
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_FAIL));	
	}
	else
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_FINISH));
	}
	Sleep(1200);
	theSERVICE_MGR.SetForeServiceBusy(false);

	//��ʾ��ʼ������
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_NEW_SELECT));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param       DATA_TYPE_CODE dataType

@retval     bool  true:�ɹ�; false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CDataOutputSvc::ExportParameter(DATA_TYPE_CODE dataType)
{
	long result = ERROR_CODE_NORMAL;
	CString sUsbDrive = theTVM_INFO.GetUSBDrive();	
	CString sDataPath = theTVM_INFO.GetDataPath();
	CString sBackupDataPath = theTVM_INFO.GetBackupPath();
	CString sExPath = theTVM_INFO.GetExportPath();	
	CString sDesPath = sUsbDrive + sExPath + m_DeviceID + _T("\\");

	// �������ݵ��������ļ�	
	CString sExportIndexFilePathName = sUsbDrive + EXPORT_INDEX_FILE; 
	CGetIni m_exportIndexFile(sExportIndexFilePathName);
	//�ж��Ƿ����u��
	bool bIsCreated = m_exportIndexFile.WriteData(SECTION_FILE,KEY_FILE_TYPE,_T("1"));
	//�������ɹ�,֤��û�в���U��
	if (!bIsCreated)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
		return ERROR_CODE_CREATE_FILE_FAILED;
	}

	// ��������ļ�û�д���ʱ�䣬��ʱ��д��
	CString sfileCnt = m_exportIndexFile.GetDataString(SECTION_FILE,KEY_CREAT_TIME,_T(""));
	if(sfileCnt.IsEmpty()){
		_DATE_TIME creatIndexTime = ComGetCurTime();
		CString strCreatIndexTime;
		strCreatIndexTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),creatIndexTime.wYear,creatIndexTime.biMonth,creatIndexTime.biDay,creatIndexTime.biHour,creatIndexTime.biMinute,creatIndexTime.biSecond);
		m_exportIndexFile.WriteData(SECTION_FILE,KEY_CREAT_TIME,strCreatIndexTime); 
	}

	// д��  Modified_time
	m_exportIndexFile.WriteData(SECTION_FILE,KEY_MODIFY_TIME,_T("0"));

	// д��  Device_Count
	CString sDeviceCnt;
	int fileCnt = m_exportIndexFile.GetDataInt(m_DeviceID,KEY_FILE_CNT,-1);
	if(fileCnt == -1){	
		m_exportIndexFile.WriteData(m_DeviceID,KEY_FILE_CNT,_T("0"));

		int DeviceCnt = m_exportIndexFile.GetDataInt(SECTION_FILE,KEY_DEVICE_COUNT,0);				
		DeviceCnt++;	
		sDeviceCnt.Format(_T("%d"),DeviceCnt);
		m_exportIndexFile.WriteData(SECTION_FILE,KEY_DEVICE_COUNT,sDeviceCnt);

		// �豸ID�б�
		CString sDeviceNo = KEY_DEVICE_NO + sDeviceCnt;
		m_exportIndexFile.WriteData(SECTION_FILE,sDeviceNo,m_DeviceID);	
	}

	CString sSrcPath;
	CString sSrcBackupPath;
	CString sPreFix;
	EXPORT_DATA_TYPE edataType;
	switch(dataType){
	case ACC_TRANS:	
		sSrcPath = sDataPath + ACC_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sSrcBackupPath = sBackupDataPath + ACC_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sPreFix = _T("Deal.2001.");
		edataType = TXN_TYPE;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_DEAL);
		break;
	case AFC_TRANS:
		sSrcPath = sDataPath + AFC_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sSrcBackupPath = sBackupDataPath + AFC_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sPreFix = _T("Deal.2001.");
		edataType = TXN_TYPE;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_DEAL);
		break;
	//case ECT_TRANS:
	//	sSrcPath = sDataPath + ECT_TXN_FILE_PATH + UNSEND_DIRECTORY;
	//	sSrcBackupPath = sBackupDataPath + ECT_TXN_FILE_PATH + UNSEND_DIRECTORY;
	//	sPreFix = _T("Deal.2001.");
	//	edataType = TXN_TYPE;
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_DEAL);
	//	break;
	case BUS_DATA:
		sSrcPath = sDataPath + BUS_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sSrcBackupPath = sBackupDataPath + BUS_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sPreFix = _T("Oper.3001.");	
		edataType = BUS_TYPE;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_OPER);
		break;
	case EVEN_DATA:
		sSrcPath = sDataPath + EVN_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sSrcBackupPath = sBackupDataPath + EVN_TXN_FILE_PATH + UNSEND_DIRECTORY;
		sPreFix = _T("Oper.3001.");	
		edataType = BUS_TYPE;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_OPER);
		break;
	case LOG_DATA:	
		edataType = PAM_TYPE;
		sSrcPath = theTVM_INFO.GetLogPath();

		CString slogFileName;
		if(PrepareUploadLogFile(slogFileName) == 1){
			//ɾ����־�ļ�
			DeleteFile(sSrcPath + slogFileName);
			return ERROR_CODE_OTHER_ERROR;
		}		

		// [�豸ID]  FileCount
		int fileCnt = m_exportIndexFile.GetDataInt(m_DeviceID,KEY_FILE_CNT,0);
		fileCnt++;
		CString sfileCnt;
		sfileCnt.Format(_T("%d"),fileCnt);	
		//�жϵ���Ŀ¼�Ƿ����
		if (!ComCheckAndCreateDirectory(sDesPath))
		{
			//����Ŀ¼ʧ��
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL));
			return ERROR_CODE_CREATE_FILE_FAILED;
		}	
		// �����ļ�
		if(CopyFile(sSrcPath + slogFileName,sDesPath + slogFileName,FALSE) == FALSE){
			result = ERROR_CODE_OTHER_ERROR;
		}
		else{
			// ����ļ�����Э��ͷ
			CParamHelp paramHelp;
			paramHelp.AddTransHead(sDesPath + slogFileName,(BYTE)edataType);

			m_exportIndexFile.WriteData(m_DeviceID,KEY_FILE_CNT,sfileCnt);

			CString sFileName = GetFileKeyName(fileCnt);
			CString sFileNameVal = sDesPath + slogFileName;
			sFileNameVal.Replace(sUsbDrive,_T(""));
			sFileNameVal.Replace(_T("\\"),_T("/"));		
			m_exportIndexFile.WriteData(m_DeviceID,sFileName,sFileNameVal);
			
			// ɾ����־�ļ�
			DeleteFile(sSrcPath + slogFileName);
		}

		// д�� Modified_time
		_DATE_TIME modifyIndexTime = ComGetCurTime();
		CString strModifyIndexTime;
		strModifyIndexTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),modifyIndexTime.wYear,modifyIndexTime.biMonth,modifyIndexTime.biDay,modifyIndexTime.biHour,modifyIndexTime.biMinute,modifyIndexTime.biSecond);
		m_exportIndexFile.WriteData(SECTION_FILE,KEY_MODIFY_TIME,strModifyIndexTime);
		
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_DEBUG);
		return result;
	}

	// �������ݺ�ҵ�����ݵ���
	vector<CString> fileNames;
	SearchFile(sSrcPath + _T("\\*.*"),fileNames);
	vector<CString>::iterator iter = fileNames.begin();
	while (iter != fileNames.end()) {
		// [�豸ID]  FileCount
		int fileCnt = m_exportIndexFile.GetDataInt(m_DeviceID,KEY_FILE_CNT,0);
		fileCnt++;

		_DATE_TIME creatTime = ComGetCurTime();
		CString strCreatTime;
		strCreatTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),creatTime.wYear,creatTime.biMonth,creatTime.biDay,creatTime.biHour,creatTime.biMinute,creatTime.biSecond);

		CString sfileCnt;
		sfileCnt.Format(_T("%d"),fileCnt);

		CString sSrcFileName;
		sSrcFileName = sPreFix + m_DeviceID + _T(".") + strCreatTime + _T(".") + sfileCnt;

		//�жϵ���Ŀ¼�Ƿ����
		if (!ComCheckAndCreateDirectory(sDesPath))
		{
			//����Ŀ¼ʧ��
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL));
			return ERROR_CODE_CREATE_FILE_FAILED;
		}	
		// �����ļ�
		if(CopyFile((*iter),sDesPath + sSrcFileName,FALSE) == FALSE){
			result = ERROR_CODE_OTHER_ERROR;
		}
		else{
			// ����ļ�����Э��ͷ
			CParamHelp paramHelp;
			paramHelp.AddTransHead(sDesPath + sSrcFileName,(BYTE)edataType);

			m_exportIndexFile.WriteData(m_DeviceID,KEY_FILE_CNT,sfileCnt);

			CString sFileName = GetFileKeyName(fileCnt);
			CString sFileNameVal = sDesPath + sSrcFileName;
			sFileNameVal.Replace(sUsbDrive,_T(""));
			sFileNameVal.Replace(_T("\\"),_T("/"));
			m_exportIndexFile.WriteData(m_DeviceID,sFileName,sFileNameVal);

			DeleteFile((*iter));
			DeleteFile(sSrcBackupPath + GetFileNameFromFullPath((*iter)));
		}
		iter ++;
	}

	// д�� Modified_time
	_DATE_TIME modifyIndexTime = ComGetCurTime();
	CString strModifyIndexTime;
	strModifyIndexTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),modifyIndexTime.wYear,modifyIndexTime.biMonth,modifyIndexTime.biDay,modifyIndexTime.biHour,modifyIndexTime.biMinute,modifyIndexTime.biSecond);
	m_exportIndexFile.WriteData(SECTION_FILE,KEY_MODIFY_TIME,strModifyIndexTime);

	if(dataType == ACC_TRANS){
		if(!theEXCEPTION_MGR.HasException(CTxnDataException::MODULE_ID,CTxnDataException::VALIDATE_NOT_SEND_FAILD))
		{
			theACC_TXN_MGR.ProtectUnsendPackage();
		}
		theACC_TXN_MGR.InitTxnData();
	}
	else if(dataType == AFC_TRANS){
		theAFC_TXN_MGR.InitTxnData();
	}
	else if(dataType == ECT_TRANS){
		theECT_TXN_MGR.InitTxnData();
	}
	else if(dataType == EVEN_DATA){
		theEVN_TXN_MGR.InitTxnData();
	}
	else if(dataType == BUS_DATA){
		theBUS_TXN_MGR.InitTxnData();
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ô洢�ļ��豸ID���ļ���key��

@param      (i)int ifileNum      �ļ����

@retval     CString              ����section��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CDataOutputSvc::GetFileKeyName(int ifileNum)
{
	CString sFileCount;
	sFileCount.Format(_T("%.4d"),ifileNum);
	return KEY_FILE_NAME + sFileCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��֤usb�豸

@param      none

@retval     bool  ture:�ɹ�; false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CDataOutputSvc::AuthUSB()
{
	bool bret = false;
	try{
		int ret = 0;
		ret = uc_authenticate();
		if(ret == 1){
			m_bIsAuthPass = true;
		}
		else{
			m_bIsAuthPass = false;
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(DATA_OUTPUT_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return m_bIsAuthPass;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������־�ϴ��ļ�

@param      (i)CString sDate	   ��־����
@param      (o)CString& fileName   �������ļ���

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CDataOutputSvc::PrepareUploadLogFile(CString& fileName)
{
	vector<_DATE> logDates;
	logDates.push_back(ComGetCurDate());
	CString filePath;
	int result = CLogHelper::PrepareUploadLogFile(filePath,logDates);
	fileName = GetFileNameFromFullPath(filePath);

	if (result != 1) {
		return 1;
	}
	return 0;
}
