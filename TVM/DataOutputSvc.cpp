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
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSvc::CDataOutputSvc()
:CTVMForeService(DATA_OUTPUT_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20131_DATAOUTPUT_SELECT_DLG,new CDataOutputSelectMaintainDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DATAOUTPUT_SELECT, _opl(GUIDE_DATA_OUTPUT_NEW_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DATAOUTPUT_SELECT, IDD_20131_DATAOUTPUT_SELECT_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAOUTPUT_SELECT);

	m_DeviceID = theMAINTENANCE_INFO.GetCurrentDevice().ToString();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSvc::~CDataOutputSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化第一个画面组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSvc::OnStart(){
	__super::OnStart();
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAOUTPUT_SELECT);

	m_DeviceID = theMAINTENANCE_INFO.GetCurrentDevice().ToString();
	// 连线是不能使用该功能
	if(theTVM_STATUS_MGR.GetCommunicate() == COMMUNICATION::CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		// 判断是否插入U盘
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
@brief      判断USB是否认证通过

@param      无

@retval     bool :true 通过; false 不通过

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CDataOutputSvc::IsUSBAuthed(){
	return m_bIsAuthPass;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回按键

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
@brief      确认处理键

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDataOutputSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	// 连线是不能使用该功能
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

	// 判断是否插入U盘
	if(!PathIsDirectory(theTVM_INFO.GetUSBDrive())){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
		return -1;
	}

	//认证U盘
	if(!IsUSBAuthed()){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
		return -1;
	}
	//提示正在导出数据
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUTING));
	theSERVICE_MGR.SetForeServiceBusy(true);
	long errCode = ExportParameter(m_DataType);
	//AFC交易
	if (m_DataType == DATA_TYPE_CODE::AFC_TRANS && errCode == ERROR_CODE_NORMAL)
	{
		//ACC交易
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

	//显示初始化界面
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_NEW_SELECT));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      导出参数

@param       DATA_TYPE_CODE dataType

@retval     bool  true:成功; false:失败

@exception  无
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

	// 创建数据导出索引文件	
	CString sExportIndexFilePathName = sUsbDrive + EXPORT_INDEX_FILE; 
	CGetIni m_exportIndexFile(sExportIndexFilePathName);
	//判断是否插入u盘
	bool bIsCreated = m_exportIndexFile.WriteData(SECTION_FILE,KEY_FILE_TYPE,_T("1"));
	//创建不成功,证明没有插入U盘
	if (!bIsCreated)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND));
		return ERROR_CODE_CREATE_FILE_FAILED;
	}

	// 如果索引文件没有创建时间，将时间写入
	CString sfileCnt = m_exportIndexFile.GetDataString(SECTION_FILE,KEY_CREAT_TIME,_T(""));
	if(sfileCnt.IsEmpty()){
		_DATE_TIME creatIndexTime = ComGetCurTime();
		CString strCreatIndexTime;
		strCreatIndexTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),creatIndexTime.wYear,creatIndexTime.biMonth,creatIndexTime.biDay,creatIndexTime.biHour,creatIndexTime.biMinute,creatIndexTime.biSecond);
		m_exportIndexFile.WriteData(SECTION_FILE,KEY_CREAT_TIME,strCreatIndexTime); 
	}

	// 写入  Modified_time
	m_exportIndexFile.WriteData(SECTION_FILE,KEY_MODIFY_TIME,_T("0"));

	// 写入  Device_Count
	CString sDeviceCnt;
	int fileCnt = m_exportIndexFile.GetDataInt(m_DeviceID,KEY_FILE_CNT,-1);
	if(fileCnt == -1){	
		m_exportIndexFile.WriteData(m_DeviceID,KEY_FILE_CNT,_T("0"));

		int DeviceCnt = m_exportIndexFile.GetDataInt(SECTION_FILE,KEY_DEVICE_COUNT,0);				
		DeviceCnt++;	
		sDeviceCnt.Format(_T("%d"),DeviceCnt);
		m_exportIndexFile.WriteData(SECTION_FILE,KEY_DEVICE_COUNT,sDeviceCnt);

		// 设备ID列表
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
			//删除日志文件
			DeleteFile(sSrcPath + slogFileName);
			return ERROR_CODE_OTHER_ERROR;
		}		

		// [设备ID]  FileCount
		int fileCnt = m_exportIndexFile.GetDataInt(m_DeviceID,KEY_FILE_CNT,0);
		fileCnt++;
		CString sfileCnt;
		sfileCnt.Format(_T("%d"),fileCnt);	
		//判断导出目录是否存在
		if (!ComCheckAndCreateDirectory(sDesPath))
		{
			//创建目录失败
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL));
			return ERROR_CODE_CREATE_FILE_FAILED;
		}	
		// 拷贝文件
		if(CopyFile(sSrcPath + slogFileName,sDesPath + slogFileName,FALSE) == FALSE){
			result = ERROR_CODE_OTHER_ERROR;
		}
		else{
			// 添加文件传输协议头
			CParamHelp paramHelp;
			paramHelp.AddTransHead(sDesPath + slogFileName,(BYTE)edataType);

			m_exportIndexFile.WriteData(m_DeviceID,KEY_FILE_CNT,sfileCnt);

			CString sFileName = GetFileKeyName(fileCnt);
			CString sFileNameVal = sDesPath + slogFileName;
			sFileNameVal.Replace(sUsbDrive,_T(""));
			sFileNameVal.Replace(_T("\\"),_T("/"));		
			m_exportIndexFile.WriteData(m_DeviceID,sFileName,sFileNameVal);
			
			// 删除日志文件
			DeleteFile(sSrcPath + slogFileName);
		}

		// 写入 Modified_time
		_DATE_TIME modifyIndexTime = ComGetCurTime();
		CString strModifyIndexTime;
		strModifyIndexTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),modifyIndexTime.wYear,modifyIndexTime.biMonth,modifyIndexTime.biDay,modifyIndexTime.biHour,modifyIndexTime.biMinute,modifyIndexTime.biSecond);
		m_exportIndexFile.WriteData(SECTION_FILE,KEY_MODIFY_TIME,strModifyIndexTime);
		
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_OUTPUT_DEBUG);
		return result;
	}

	// 交易数据和业务数据导出
	vector<CString> fileNames;
	SearchFile(sSrcPath + _T("\\*.*"),fileNames);
	vector<CString>::iterator iter = fileNames.begin();
	while (iter != fileNames.end()) {
		// [设备ID]  FileCount
		int fileCnt = m_exportIndexFile.GetDataInt(m_DeviceID,KEY_FILE_CNT,0);
		fileCnt++;

		_DATE_TIME creatTime = ComGetCurTime();
		CString strCreatTime;
		strCreatTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),creatTime.wYear,creatTime.biMonth,creatTime.biDay,creatTime.biHour,creatTime.biMinute,creatTime.biSecond);

		CString sfileCnt;
		sfileCnt.Format(_T("%d"),fileCnt);

		CString sSrcFileName;
		sSrcFileName = sPreFix + m_DeviceID + _T(".") + strCreatTime + _T(".") + sfileCnt;

		//判断导出目录是否存在
		if (!ComCheckAndCreateDirectory(sDesPath))
		{
			//创建目录失败
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL));
			return ERROR_CODE_CREATE_FILE_FAILED;
		}	
		// 拷贝文件
		if(CopyFile((*iter),sDesPath + sSrcFileName,FALSE) == FALSE){
			result = ERROR_CODE_OTHER_ERROR;
		}
		else{
			// 添加文件传输协议头
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

	// 写入 Modified_time
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
@brief      获得存储文件设备ID中文件名key名

@param      (i)int ifileNum      文件编号

@retval     CString              参数section名

@exception  无
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
@brief      认证usb设备

@param      none

@retval     bool  ture:成功; false:失败

@exception  无
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
@brief      创建日志上传文件

@param      (i)CString sDate	   日志日期
@param      (o)CString& fileName   创建的文件名

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
