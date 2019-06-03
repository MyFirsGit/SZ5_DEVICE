#include "StdAfx.h"
#include "VersionInfoSvc.h"
#include "VersionInfoDlg.h"
#include "ParamHelper.h"
#include "usbctl.h"
#include "GuideInfo.h"
#include "UpgradeHelper.h"

static const TCHAR* const IMPORT_INDEX_FILE_PREFIX  = _T("ParameterIndexFile");
static const TCHAR* const IMPORT_INDEX_FILE_SUFFIX  = _T(".DAT");
static const TCHAR* const SECTION_FILE		= _T("FILE");
static const TCHAR* const KEY_FILE_COUNT	= _T("ParameterFileCount");
static const TCHAR* const SECTION_PARAMETER	= _T("Parameter");
static const TCHAR* const KEY_PARAM_TYPE	= _T("ParameterId");
static const TCHAR* const KEY_MAIN_VERSION	= _T("MainVersion");
static const TCHAR* const KEY_FILE_NAME		= _T("Filename");
static const int    const WHNE_RESTART_TIME = 15;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CVersionInfoSvc, CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfoSvc::CVersionInfoSvc(void)
	:CTVMForeService(VERSION_INFO_SVC)
{
	//m_ErrorCode = -1;
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20121_DATAINPUT_SELECT_DLG,new CVersionInfoDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT,_opl(GUIDE_F3_F6_ESC));
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
CVersionInfoSvc::~CVersionInfoSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ�ϵ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CVersionInfoSvc::OnKeyboardEnter(WPARAM , LPARAM )
//{
//	//ȷ�ϵ������ 
//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_DONING));
//	theSERVICE_MGR.SetForeServiceBusy(true);
//	ImportParameter();
//	theSERVICE_MGR.SetForeServiceBusy(false);
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ذ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CVersionInfoSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���½�����ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfoSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ����ģ��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
map<WORD,CVersionInfoSvc::ParameterInputStruct>& CVersionInfoSvc::GetDataModel()
{
	return m_DataModel;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      ��������
//
//@param      void
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
void CVersionInfoSvc::OnStart()
{
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT);
	m_DataModel.clear();
	InitDataModel();
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F3_F6_ESC));
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ����ǰ����������汾

@param      void 

@retval     void

@exception  void
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfoSvc::InitDataModel()
{
	//ȡ��ǰ�豸����
	AFC_DEVICE_TYPE currDeviceType = theMAINTENANCE_INFO.GetDeviceType();
	bool bIsTvm = (currDeviceType == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM);

	// AFCӦ�ó������(TVMӦ�ó���TPU������
	CString programNames[] = {
		_opl(TXT_PROGRAM_TVM_APP),
		_opl(TXT_PROGRAM_TPU_APP),
	};
	WORD programIDs[] = {
		AFC_TVMPROGRAM_ID,
		AFC_TPUPROGRAM_ID,
	};
	CVersionInfo::_PROG_TYPE programTypes[] = {
		CVersionInfo::MC,				// TVM����
		CVersionInfo::CARD_RW_APP,	    // TPU����
	};
	//�жϵ�ǰTPU����İ汾���Ƿ��в�һ�µİ汾
	//if(theSETTING.GetCardRWUpdateFlag() != 0 && bIsTvm)
	//{
	//	//���Ʊ��������δ���³ɹ���ȡƱ���������İ汾
	//	programTypes[1] = CVersionInfo::CARD_RW_APP;
	//}
	//else if (theSETTING.GetTokenRWUpdateFlag() != 0 && bIsTvm)
	//{
	//	//���Token������δ���³ɹ���ȡToken�������İ汾
	//	programTypes[1] = CVersionInfo::TOKEN_RW_APP;
	//}
	for(int i = 0;i < 2;i++)
	{
		_tagParameterInputStruct &inputStruct = m_DataModel[programIDs[i]];
		inputStruct.name = programNames[i];
		CString strID;
		strID.Format(_T("(%x)"), programIDs[i]);
		inputStruct.name += strID;
		inputStruct.localCurrentVersion = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,programTypes[i]);
		inputStruct.localFutureVersion = theVERSION_INI.GetVersionNo(CVersionInfo::FUTURE,programTypes[i]);
	}



	//AFC��ز���
	CString parameterAFCNames[] = {
		_opl(TXT_AFC_DEVICE_COMMON_PARAM),    // �豸��������     1005
		_opl(TXT_AFC_DEVICE_OPERATOR_PARAM),  // �豸����Ա��Ϣ   1006
		_opl(TXT_AFC_DEVICE_AUTHORITY_PARAM), // �豸Ȩ�޲���     1007
		_opl(TXT_AFC_TVM_OPERATION_PARAM),	  // TVM��Ӫ����      1011
		_opl(TXT_AFC_DEVICE_TIME_PARAM),      // �豸����ʱ����� 1089
	};
	WORD parameterAFCIDs[] = {
		AFC_DEVICECOMMONPARAM_ID,
		AFC_STAFFPWDPARAM_ID,
		AFC_ACCESSLEVELPARAM_ID,
		AFC_TVMOPERATIONPARAM_ID,
		AFC_DEVICERUNTIMEPARAM_ID
	};
	//����
	CAfcParamMaster* parameterAFCLocalVersionProviders[] = {
		&theAFC_COMMON,
		&theAFC_STAFF,
		&theAFC_ACCESS_LEVEL,
		&theAFC_TVM_OPERATION,
		&theAFC_DEV_TIME,
	};
	TCHAR szRunPath[MAX_PATH];
	ComGetAppPath(szRunPath, MAX_PATH);
	for(int i = 0;i < 5;i++)
	{
		_tagParameterInputStruct &inputStruct = m_DataModel[parameterAFCIDs[i]];
		inputStruct.name = parameterAFCNames[i];
		CString strID;
		strID.Format(_T("(%x)"), parameterAFCIDs[i]);
		inputStruct.name += strID;
		inputStruct.localCurrentVersion = parameterAFCLocalVersionProviders[i]->GetCurMasterVer();
		inputStruct.localFutureVersion = parameterAFCLocalVersionProviders[i]->GetFutMasterVer();
	}


	// ACC����
	CString parameterACCNames[] = 
	{
		_T("0101��Ӫ����"),
		_T("0102�������������Ѳ���"),
		_T("0103��ʱ�������ò���"),	
		_T("0201�豸���Ʋ���"),	
		_T("0301��վλ�ò���"),	
		_T("0302�Ʒ�վ�����"),	
		_T("0303���β���"),	
		_T("0401Ʊ�����Բ���"),	
		_T("0501Ʊ�������̲���"),
		_T("0601����Ʊ�۱����"),
		_T("0602Ʊ�ۼ�������"),
		_T("0603Ʊ�۵������Ա����"),
		_T("0604ʱ�両�������"),
		_T("0605ʱ�����ͱ����"),
		_T("0606ʱ��������ϸ����"),
		_T("0607�ۿ��ʱ����"),
		//_T("0608ͣ�˳�վ��"),
		_T("0701��������������"),
		_T("0702���κ���������"),
		_T("0801����������"),
		_T("0802��ؿ�/���д�����ղ���"),

	};
	WORD parameterACCIDs[] = 
	{
		ACC_BUSINESS_ID						,					// ACC��Ӫ����
		ACC_SERVICEFEE_ID					,					// ACC�������������Ѳ���
		ACC_OVERTIME_UPDATE_ID				,					// ACC��ʱ�������ò���
		ACC_DEVICE_ID						,					// ACC�豸���Ʋ���
		ACC_STATION_ID						,					// ACC��վλ�ò���
		ACC_CHARGE_STATION_ID				,					// ACC�Ʒ�վ�����
		ACC_SECTION_ID						,					// ACC���β���
		ACC_CARDATTRIBUTE_ID				,					// ACCƱ�����Բ���
		ACC_ISSUE_COMPANY_ID			    ,					// ACCƱ�������̲���
		ACC_BASE_PRICE_TABLE_ID				,					// ACC����Ʊ�۱����
		ACC_PRICE_LEVEL_TABLE_ID			,					// ACCƱ�ۼ�������
		ACC_PRICE_ADJUEST_TABLE_ID			,					// ACCƱ�۵������Ա����
		ACC_TIME_FLOATING_TABLE_ID			,					// ACCʱ�両�������
		ACC_TIME_TYPE_TABLE_ID				,					// ACCʱ�����ͱ����
		ACC_TTIME_TYPE_DETAIL_ID			,					// ACCʱ��������ϸ����
		ACC_DISSCOUNT_TABLE_ID				,					// ACC�ۿ��ʱ����
		//ACC_NOT_SERVICE_STATION_ID			,					// ACCͣ�˳�վ��
		ACC_SINGLE_BLACK_LIST_ID			,					// ACC��������������
		ACC_SECTION_BLACK_LIST_ID			,					// ACC���κ���������
		ACC_WRITE_LIST_ID					,					// ACC����������
		ACC_OTHER_CITY_CARD_ID				,					// ACC��ؿ�/���д�����ղ���	
	};
	//����
	CAccParamMaster* parameterACCLocalVersionProviders[] = 
	{
		&theACC_BUSINESS,
		&theACC_SERVICEFEE,		
		&theACC_OVERTIMEADJUEST,	
		&theACC_DEVICECONTROL,
		&theACC_LOCATION,
		&theACC_TOLLSTATION,
		&theACC_SECTION,
		&theACC_PRODUCT,
		&theACC_ISSUECOMPANY,
		&theACC_BASEPRICE,
		&theACC_PRICELEVEL,
		&theACC_PRICEADJUEST,
		&theACC_TIMEFLOAT,	
		&theACC_TIMETYPE,		
		&theACC_TIMEDETAIL,	
		&theACC_DISCOUNT,	
		//&theACC_NOTSERVSTATION,
		&theACC_SINGLEBLACKLIST,
		&theACC_SECTIONBLACKLIST,
		&theACC_WRITELIST,
		&theACC_OTHERCITYCARD,	

	};
	for(int i = 0;i < 20;i++)
	{
		_tagParameterInputStruct &inputStruct = m_DataModel[parameterACCIDs[i]];
		inputStruct.name = parameterACCNames[i];
		CString strID;
		strID.Format(_T("(%x)"), parameterACCIDs[i]);
		inputStruct.name += strID;
		inputStruct.localCurrentVersion = parameterACCLocalVersionProviders[i]->GetCurMasterVer();
		inputStruct.localFutureVersion = parameterACCLocalVersionProviders[i]->GetFutMasterVer();
	}

	//// 9700������ͼ���� ---��������
	//{
	//	_tagParameterInputStruct &inputStruct = m_DataModel[ACC_STATION_MAP_ID];
	//	inputStruct.name = _T("������ͼ����");
	//	CString strID;
	//	strID.Format(_T("(%x)"), ACC_STATION_MAP_ID);
	//	inputStruct.name += strID;
	//	inputStruct.localCurrentVersion = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::STATION_MAP);
	//	inputStruct.localFutureVersion = theVERSION_INI.GetVersionNo(CVersionInfo::FUTURE,CVersionInfo::STATION_MAP);
	//}


	//��ҳ��Ϣ
	int nTotalPage = 0,nCurrPage = 0,nPerPage = 0;
	nCurrPage  = 1;
	nPerPage   = MAX_ROW - 1;
	nTotalPage = m_DataModel.size() % nPerPage == 0 ? m_DataModel.size() / nPerPage : m_DataModel.size() / nPerPage +1;
	SetPageInfo(nTotalPage,nCurrPage,nPerPage);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��U�̻�ȡ��Ҫ������Ϣ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfoSvc::GetImportInfo()
{
	// �ⲿý����֤  
	if(theFunction_INFO.GetUSBAuthFlag() == 1){
		// �����֤USB�豸ʧ�ܣ���ʾ��ʾ����
		if(!AuthUSB()){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
			return;	
		}
	}
	// �Ȼ�ȡ�����ļ�·��
	CString strUSBDrive = theTVM_INFO.GetUSBDrive();
	DEVICE_ID curDeviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	CString strDeviceID = curDeviceID.ToString();
	// �ȴӲ��������ļ��õ��ļ���
	CString strImportIndexFile = strUSBDrive + IMPORT_INDEX_FILE_PREFIX + strDeviceID + IMPORT_INDEX_FILE_SUFFIX;
	// ���������ļ��Ƿ����
	if (FALSE == ComCheckFileExist(strImportIndexFile)){
		// ���ͨ���ļ��Ƿ����
		strDeviceID.Format(_T("%.2X%.2X%.2X%.2X"),curDeviceID.bLineId,curDeviceID.bStationId,curDeviceID.bAfcDeviceType,0);
		strImportIndexFile = strUSBDrive + IMPORT_INDEX_FILE_PREFIX + strDeviceID + IMPORT_INDEX_FILE_SUFFIX;
		if(FALSE == ComCheckFileExist(strImportIndexFile)){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_ERROR_INDEX_EXIST));
			return ;
		}
	}

	CGetIni importIndexFile(strImportIndexFile);	
	int fileCount = importIndexFile.GetDataInt(SECTION_FILE, KEY_FILE_COUNT,0);	// �ļ�����
	for(int i = 1;i <= fileCount;i++)
	{
		CString sectionName;
		sectionName.Format(_T("%d"),i);
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
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���ⲿý�嵼�����(��ʼ����)
//
//@param      none
//
//@retval     none
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
bool CVersionInfoSvc::ImportParameter()
{
	//�ж��Ƿ����ߣ�������߾Ͳ�����������
	if (theAPP_SESSION.IsSCConnected())
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_MSGBOX_ONLINE));
		CString comment(_T("INPUT_PARAM"));
		// ����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
		return false;
	}
	// �ⲿý����֤  
	if(theFunction_INFO.GetUSBAuthFlag() == 1){
		// �����֤USB�豸ʧ�ܣ���ʾ��ʾ����
		if(!AuthUSB()){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
			CString comment(_T("INPUT_PARAM"));
			// ����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
			return false;	
		}
	}
	typedef map<WORD,ParameterInputStruct>::iterator DataItem;
	for(DataItem iter = m_DataModel.begin();iter != m_DataModel.end();iter++)
	{
		DWORD parameterID = iter->first;
		ParameterInputStruct &inputStruct = iter->second;
		//�������·���ͱ�����·������Ϊ�ա�
		if(!inputStruct.importCurrentFrom.IsEmpty() && !inputStruct.importCurrentTo.IsEmpty()){
			if (!ComCheckAndCreateDirectory(inputStruct.importCurrentTo))
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_CREATE_DIR_FILED));
			}
			CString toFileName = inputStruct.importCurrentTo + GetFileNameFromFullPath(inputStruct.importCurrentFrom);
			//�жϵ�ǰ�����ļ��Ƿ���ڣ����� ������ �����ش���zheng
			if (!ComCheckFileExist(inputStruct.importCurrentFrom))
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(DATAINPUT_UPDATE_TIP_INPUT_FILE_NOT_EXIST));
				CString comment(_T("INPUT_PARAM"));
				// ����--������־
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
				return false;
			}
			//ʵʩ�������
			CopyFile(inputStruct.importCurrentFrom,toFileName,FALSE);
			switch(parameterID)
			{
			case AFC_TVMPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC,inputStruct.importCurrentVersion);
				break;
			case AFC_TPUPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CHARGE_RW_APP,inputStruct.importCurrentVersion);
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CARD_RW_APP,inputStruct.importCurrentVersion);
				//theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_RW_APP,inputStruct.importCurrentVersion);
				break;
				/*case AFC_TVMGUI_ID:
				case AFC_AVMGUI_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::GUI,inputStruct.importCurrentVersion);
				break;
				case AFC_TVMVOICE_ID:
				case AFC_AVMVOICE_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::VOICE,inputStruct.importCurrentVersion);
				break;
				case AFC_TVMTEMPLATE_ID:
				case AFC_AVMTEMPLATE_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TEMPLATE,inputStruct.importCurrentVersion);
				break;*/
			default:
				break;
			}
		}
		else if(!inputStruct.importFutureFrom.IsEmpty() && !inputStruct.importFutureTo.IsEmpty()){

			if (!ComCheckAndCreateDirectory(inputStruct.importFutureTo))
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_CREATE_DIR_FILED));
			}
			CString toFileName = inputStruct.importFutureTo + GetFileNameFromFullPath(inputStruct.importFutureFrom);
			CopyFile(inputStruct.importFutureFrom,toFileName,FALSE);
			switch(parameterID)
			{
			case AFC_TVMPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::MC,inputStruct.importFutureVersion);
				break;
			case AFC_TPUPROGRAM_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::CHARGE_RW_APP,inputStruct.importFutureVersion);
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::CARD_RW_APP,inputStruct.importFutureVersion);
				//theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::TOKEN_RW_APP,inputStruct.importFutureVersion);
				break;
				/*case AFC_TVMGUI_ID:
				case AFC_AVMGUI_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::GUI,inputStruct.importFutureVersion);
				break;
				case AFC_TVMVOICE_ID:
				case AFC_AVMVOICE_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::VOICE,inputStruct.importFutureVersion);
				break;
				case AFC_TVMTEMPLATE_ID:
				case AFC_AVMTEMPLATE_ID:
				theVERSION_INI.SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::TEMPLATE,inputStruct.importFutureVersion);
				break;*/
			default:
				break;
			}
		}else
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_FAILURE));
			CString comment(_T("INPUT_PARAM"));
			// ����--������־
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
			return false;
		}
	}
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM);
	if(theFunction_INFO.GetUSBAuthFlag() == 1){
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);	
		CString sUpdatePath = CString(szRunPath) + PM_UPDATE_DIR;
		thePARAM_HELPER.RemoveParamTransHead(sUpdatePath);
	}
	//1.�������
	CParamHelper::VEC_VERSION_INFO VecVersionInfo;
	thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
	// ��ʼ�����в���		
	if(!thePARAM_HELPER.InitAllParam()){
		theEXCEPTION_MGR.ProcessException(CLogException(CLogException::FILE_READ_FAIL, _T(""), __LINE__,_opl(GUIDE_DATA_INPUT_MSGBOX_PARAM)));

		CString comment(_T("INPUT_PARAM"));
		// ����--������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
	}

	//2.�����վ�ģ�弰�����ļ�
	theUPGRADE_HELPER::UpdateVoiceAndTemplateFolder();

	//3.����Ӧ�ó����GUI�ļ�
	if(theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade()){
		//����ʾ�Ի���
		//CMessageBox::Show(_opl(TITLE_DATA_INPUT),_opl(GUIDE_DATA_INPUT_MSGBOX_RESTART),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		//����ʱ�ػ� ʱ��ΪWHEN_RESTART_TIME
		int nSecTime = WHNE_RESTART_TIME;
		CString strTemp;
		while(nSecTime--)
		{
			strTemp.Format(_T("%s(%d)"),_opl(GUIDE_DATA_INPUT_MSGBOX_RESTART),nSecTime);
			theAPP_SESSION.ShowOfficeGuide(strTemp);
			Sleep(1000);
		}
		theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
		theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
		return true;
	}
	// ����λ���͵�ǰ�汾״��
	theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->SendParameterVersion();
	// ��ʾ���ԣ��������
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_FINISH));
	CString comment(_T("INPUT_PARAM"));
	// ����--������־
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,true,comment);
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
bool CVersionInfoSvc::AuthUSB()
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


