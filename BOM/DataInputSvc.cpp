#include "stdafx.h"
#include "DataInputSvc.h"
#include "DataInputSelectMaintainDlg.h"
#include "ParamHelper.h"
#include "usbctl.h"
#include "GuideInfo.h"
#include "UpgradeHelper.h"

static const TCHAR* const IMPORT_INDEX_FILE_PREFIX  = _T("ParameterIndexFile");
static const TCHAR* const IMPORT_INDEX_FILE_SUFFIX  = _T(".DAT");

static const TCHAR* const SECTION_FILE		= _T("FILE");
static const TCHAR* const KEY_FILE_COUNT		= _T("ParameterFileCount");

static const TCHAR* const SECTION_PARAMETER	= _T("Parameter");
static const TCHAR* const KEY_PARAM_TYPE		= _T("ParameterId");
static const TCHAR* const KEY_MAIN_VERSION	= _T("MainVersion");
static const TCHAR* const KEY_FILE_NAME		= _T("Filename");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDataInputSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CDataInputSvc::CDataInputSvc()
:CBOMForeService(DATA_INPUT_SVC,MAIN_SVC)
{
	m_ErrorCode = -1;

	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20121_DATAINPUT_SELECT_DLG,new CDataInputSelectMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_DATA_INPUT_SELECT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DATAINPUT_SELECT,IDD_20121_DATAINPUT_SELECT_DLG);
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CDataInputSvc::~CDataInputSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      void

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSvc::OnStart()
{
	InitDataModel();
	GetImportInfo();
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
map<WORD,CDataInputSvc::ParameterInputStruct>& CDataInputSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSvc::InitDataModel()
{
	// AFCϵͳ�ڲ�����
	CString programNames[] = {_T("BOMӦ�ó���"),_T("TPUӦ�ó���")};
	WORD programIDs[] = {AFC_BOMPROGRAM_ID,AFC_TPUPROGRAM_ID};
	CVersionInfo::_PROG_TYPE programTypes[] = {CVersionInfo::MC,CVersionInfo::RW_APP};
	for(int i = 0;i < 2;i++)
	{
		_tagParameterInputStruct &inputStruct = m_DataModel[programIDs[i]];
		inputStruct.name = programNames[i];
		inputStruct.localCurrentVersion = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,programTypes[i]);
		inputStruct.localFutureVersion = theVERSION_INI.GetVersionNo(CVersionInfo::FUTURE,programTypes[i]);
		PARAMETER_ID parameterID = (PARAMETER_ID)programIDs[i];
		PARAM_VERSION_TYPE_CODE verCode = CUR_VER;
		inputStruct.importCurrentTo = theUPGRADE_HELPER::GetDownloadFolder(parameterID,verCode);
		verCode = FUT_VER;
		inputStruct.importFutureTo = theUPGRADE_HELPER::GetDownloadFolder( parameterID,verCode);
	}
	CString parameterAFCNames[] = {_T("ģʽ����"),_T("�豸��������"),_T("�豸����Ա��Ϣ"),_T("�豸Ȩ�޲���"),_T("�豸����ʱ�����"),_T("BOM��Ӫ����")};
	WORD parameterAFCIDs[] = {AFC_MODEHISTORY_ID,AFC_DEVICECOMMONPARAM_ID,AFC_STAFFPWDPARAM_ID,AFC_ACCESSLEVELPARAM_ID,AFC_DEVICERUNTIMEPARAM_ID,AFC_BOMOPERATIONPARAM_ID};
	CAfcParamMaster* parameterAFCLocalVersionProviders[] = {&theAFC_MODERECORD,&theAFC_COMMON,&theAFC_STAFF,&theAFC_ACCESS_LEVEL,&theAFC_DEV_TIME,&theAFC_BOM_OPERATION};
	TCHAR szRunPath[MAX_PATH];
	ComGetAppPath(szRunPath, MAX_PATH);
	for(int i = 0;i < 6;i++)
	{
		_tagParameterInputStruct &inputStruct = m_DataModel[parameterAFCIDs[i]];
		inputStruct.name = parameterAFCNames[i];
		inputStruct.localCurrentVersion = parameterAFCLocalVersionProviders[i]->GetCurMasterVer();
		inputStruct.localFutureVersion = parameterAFCLocalVersionProviders[i]->GetFutMasterVer();
		inputStruct.importCurrentTo = CString(szRunPath) + BOM_PM_UPDATE_DIR;
		inputStruct.importFutureTo = CString(szRunPath) + BOM_PM_UPDATE_DIR;
	}

	// ACC����
	CString parameterACCNames[] = 
	{
		_T("��Ӫ����"),_T("�������������Ѳ���"),_T("�豸���Ʋ���"),_T("��վλ�ò���"),_T("�Ʒ�վ�����"),
		_T("Ʊ�����Բ���"),_T("��������������"),_T("���κ���������"),_T("��Ӫ�̲���"),_T("����Ʊ�۱����"),_T("Ʊ�ۼ�������"),
		_T("Ʊ�۵������Ա����"),_T("ʱ�����ͱ����"),_T("ʱ��������ϸ����"),_T("ʱ�両�������"),_T("�ۿ��ʱ����"),
		_T("����������"),_T("��ؿ�/���д�����ղ���"),_T("��ʱ�������ò���"),_T("���β���")
	};
	WORD parameterACCIDs[] = 
	{
		ACC_BUSINESS_ID,ACC_SERVICEFEE_ID,ACC_DEVICE_ID,ACC_STATION_ID,ACC_CHARGE_STATION_ID,
		ACC_CARDATTRIBUTE_ID,ACC_SINGLE_BLACK_LIST_ID,ACC_SECTION_BLACK_LIST_ID,ACC_ISSUE_COMPANY_ID,ACC_BASE_PRICE_TABLE_ID,ACC_PRICE_LEVEL_TABLE_ID,
		ACC_PRICE_ADJUEST_TABLE_ID,ACC_TIME_TYPE_TABLE_ID,ACC_TTIME_TYPE_DETAIL_ID,ACC_TIME_FLOATING_TABLE_ID,ACC_DISSCOUNT_TABLE_ID,
		ACC_WRITE_LIST_ID,ACC_OTHER_CITY_CARD_ID,ACC_OVERTIME_UPDATE_ID,ACC_SECTION_ID
	};
	CAccParamMaster* parameterACCLocalVersionProviders[] = 
	{
		&theACC_BUSINESS,&theACC_SERVICEFEE,&theACC_DEVICECONTROL,&theACC_LINE,&theACC_TOLLSTATION,
		&theACC_TICKET,&theACC_SINGLEBLACKLIST,&theACC_SECTIONBLACKLIST,&theACC_ISSUECOMPANY,&theACC_BASEPRICE,&theACC_PRICELEVEL,
		&theACC_PRICEADJUEST,&theACC_TIMETYPE,&theACC_TIMEDETAIL,&theACC_TIMEFLOAT,&theACC_DISCOUNT,
		&theACC_WRITELIST,&theACC_OTHERCITYCARD,&theACC_OVERTIMEADJUEST,&theACC_SECTION
	};
	for(int i = 0;i < 20;i++)
	{
		_tagParameterInputStruct &inputStruct = m_DataModel[parameterACCIDs[i]];
		inputStruct.name = parameterACCNames[i];
		inputStruct.localCurrentVersion = parameterACCLocalVersionProviders[i]->GetCurMasterVer();
		inputStruct.localFutureVersion = parameterACCLocalVersionProviders[i]->GetFutMasterVer();
		inputStruct.importCurrentTo = CString(szRunPath) + BOM_PM_UPDATE_DIR;
		inputStruct.importFutureTo = CString(szRunPath) + BOM_PM_UPDATE_DIR;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CDataInputSvc::GetImportInfo()
{
	// �ⲿý����֤  
	if(theBOM_INFO.GetUSBAuthFlag() == 1){
		// �����֤USB�豸ʧ�ܣ���ʾBOM����
		if(!AuthUSB()){
			m_ErrorCode = 1;
			return;	
		}
	}
	// �Ȼ�ȡ�����ļ�·��
	CString strUSBDrive = theBOM_INFO.GetBOMUSBDrive();
	DEVICE_ID curDeviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	CString strDeviceID = curDeviceID.ToString();
	// �ȴӲ��������ļ��õ��ļ���
	CString strImportIndexFile = strUSBDrive + IMPORT_INDEX_FILE_PREFIX + strDeviceID + IMPORT_INDEX_FILE_SUFFIX;
	// ���������ļ��Ƿ����
	if (FALSE == ComCheckFileExist(strImportIndexFile)){
		// ���ͨ���ļ��Ƿ����
		strDeviceID.Format(_T("%.2X%.2X%.2X%.2X"),curDeviceID.bAfcDeviceType,curDeviceID.bLineId,curDeviceID.bStationId/*HIBYTE(curDeviceID.wAfcStationID),LOBYTE(curDeviceID.wAfcStationID)*/,0);
		strImportIndexFile = strUSBDrive + IMPORT_INDEX_FILE_PREFIX + strDeviceID + IMPORT_INDEX_FILE_SUFFIX;
		if(FALSE == ComCheckFileExist(strImportIndexFile)){
			m_ErrorCode = 2;
			return ;
		}
	}

	CGetIni importIndexFile(strImportIndexFile);	
	int fileCount = importIndexFile.GetDataInt(SECTION_FILE, KEY_FILE_COUNT,0);	// �ļ�����
	for(int i = 1;i <= fileCount;i++)
	{
		CString sectionName;
		sectionName.Format(_T("%.4d"),i);
		sectionName = SECTION_PARAMETER + sectionName;
		WORD parameterID= importIndexFile.GetDataInt(sectionName,KEY_PARAM_TYPE);
		char buffer[2];
		int2BCD(parameterID,buffer,2);
		parameterID = ComMakeWORD(buffer[0],buffer[1]);
		if(m_DataModel.find(parameterID) == m_DataModel.end()){
			continue;
		}
		CString importFileName = importIndexFile.GetDataString(sectionName,KEY_FILE_NAME);
		importFileName.Replace(_T("/"),_T("\\"));
		importFileName = strUSBDrive + importFileName;
		PARAM_VERSION_TYPE_CODE typeCode = thePARAM_HELPER.GetParamVersionType(importFileName);
		DWORD version = importIndexFile.GetDataInt(sectionName,KEY_MAIN_VERSION) ;
		switch(typeCode)
		{
		case CUR_TEST:		// ��ǰ���԰�
		case CUR_VER:		// ��ǰ��ʽ��
			m_DataModel[parameterID].importCurrentFrom = importFileName;
			m_DataModel[parameterID].importCurrentVersion = version;
			break;
		case FUT_TEST:		// �������԰�
		case FUT_VER:		// ������ʽ��
			m_DataModel[parameterID].importFutureFrom = importFileName;
			m_DataModel[parameterID].importFutureVersion = version;
			break;
		default:
			break;
		}
	}
	m_ErrorCode = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ⲿý�嵼�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CDataInputSvc::ImportParameter()
{
	typedef map<WORD,ParameterInputStruct>::iterator DataItem;
	for(DataItem iter = m_DataModel.begin();iter != m_DataModel.end();iter++)
	{
		DWORD parameterID = iter->first;
		ParameterInputStruct &inputStruct = iter->second;
		if(!inputStruct.importCurrentFrom.IsEmpty() && !inputStruct.importCurrentTo.IsEmpty()){
			CString toFileName = inputStruct.importCurrentTo + GetFileNameFromFullPath(inputStruct.importCurrentFrom);
			CopyFile(inputStruct.importCurrentFrom,toFileName,FALSE);
			switch(parameterID)
			{
			case AFC_BOMPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC,inputStruct.importCurrentVersion);
				break;
			case AFC_TPUPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP,inputStruct.importCurrentVersion);
				break;
			default:
				break;
			}
		}
		else if(!inputStruct.importFutureFrom.IsEmpty() && !inputStruct.importFutureTo.IsEmpty()){
			CString toFileName = inputStruct.importFutureTo + GetFileNameFromFullPath(inputStruct.importFutureFrom);
			CopyFile(inputStruct.importFutureFrom,toFileName,FALSE);
			switch(parameterID)
			{
			case AFC_BOMPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::MC,inputStruct.importFutureVersion);
				break;
			case AFC_TPUPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::RW_APP,inputStruct.importFutureVersion);
				break;
			default:
				break;
			}
		}
	}
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM);
	if(theBOM_INFO.GetUSBAuthFlag() == 1){
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);	
		CString sUpdatePath = CString(szRunPath) + BOM_PM_UPDATE_DIR;
		thePARAM_HELPER.RemoveParamTransHead(sUpdatePath);
	}
	CParamHelper::VEC_VERSION_INFO VecVersionInfo;
	thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
	// ��ʼ�����в���		
	if(thePARAM_HELPER.InitAllParam()){
		theACC_LINE.Internalize(TRUE,theBOM_INFO.GetDataPath(),theBOM_INFO.GetBackupPath());
	}
	else{			
		theEXCEPTION_MGR.ProcessException(CLogException(CLogException::FILE_READ_FAIL, _T(""), __LINE__,_T("��ʼ�������쳣")));
	}
	if(theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade()){
		CMessageBox::Show(_T("���ݵ���"),_T("�����и��£���Ҫ��������ȷ�ϣ�"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
		theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
		return true;
	}
	theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->SendParameterVersion();
	// ��ʾBOM���ԣ��������
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(DUIDE_DATA_INPUT_FINISH_ACC));
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��֤usb�豸

@param      none

@retval     bool  ture:�ɹ�; false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CDataInputSvc::AuthUSB()
{
	try{
		return uc_authenticate() == 1;
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return false;
}


