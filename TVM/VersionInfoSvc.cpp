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
@brief      构造函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfoSvc::CVersionInfoSvc(void)
	:CTVMForeService(VERSION_INFO_SVC)
{
	//m_ErrorCode = -1;
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20121_DATAINPUT_SELECT_DLG,new CVersionInfoDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT,_opl(GUIDE_F3_F6_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DATAINPUT_SELECT,IDD_20121_DATAINPUT_SELECT_DLG);
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

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
@brief      确认导入

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CVersionInfoSvc::OnKeyboardEnter(WPARAM , LPARAM )
//{
//	//确认导入操作 
//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_DONING));
//	theSERVICE_MGR.SetForeServiceBusy(true);
//	ImportParameter();
//	theSERVICE_MGR.SetForeServiceBusy(false);
//	return 0;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief     返回按键

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
@brief      更新界面显示

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
@brief      获取数据模型

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
//@brief      服务启用
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
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DATAINPUT_SELECT);
	m_DataModel.clear();
	InitDataModel();
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F3_F6_ESC));
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化当前参数、程序版本

@param      void 

@retval     void

@exception  void
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfoSvc::InitDataModel()
{
	//取当前设备类型
	AFC_DEVICE_TYPE currDeviceType = theMAINTENANCE_INFO.GetDeviceType();
	bool bIsTvm = (currDeviceType == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM);

	// AFC应用程序参数(TVM应用程序、TPU参数）
	CString programNames[] = {
		_opl(TXT_PROGRAM_TVM_APP),
		_opl(TXT_PROGRAM_TPU_APP),
	};
	WORD programIDs[] = {
		AFC_TVMPROGRAM_ID,
		AFC_TPUPROGRAM_ID,
	};
	CVersionInfo::_PROG_TYPE programTypes[] = {
		CVersionInfo::MC,				// TVM程序
		CVersionInfo::CARD_RW_APP,	    // TPU程序
	};
	//判断当前TPU程序的版本，是否有不一致的版本
	//if(theSETTING.GetCardRWUpdateFlag() != 0 && bIsTvm)
	//{
	//	//如果票卡读卡器未更新成功，取票卡读卡器的版本
	//	programTypes[1] = CVersionInfo::CARD_RW_APP;
	//}
	//else if (theSETTING.GetTokenRWUpdateFlag() != 0 && bIsTvm)
	//{
	//	//如果Token读卡器未更新成功，取Token读卡器的版本
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



	//AFC相关参数
	CString parameterAFCNames[] = {
		_opl(TXT_AFC_DEVICE_COMMON_PARAM),    // 设备公共参数     1005
		_opl(TXT_AFC_DEVICE_OPERATOR_PARAM),  // 设备操作员信息   1006
		_opl(TXT_AFC_DEVICE_AUTHORITY_PARAM), // 设备权限参数     1007
		_opl(TXT_AFC_TVM_OPERATION_PARAM),	  // TVM运营参数      1011
		_opl(TXT_AFC_DEVICE_TIME_PARAM),      // 设备运行时间参数 1089
	};
	WORD parameterAFCIDs[] = {
		AFC_DEVICECOMMONPARAM_ID,
		AFC_STAFFPWDPARAM_ID,
		AFC_ACCESSLEVELPARAM_ID,
		AFC_TVMOPERATIONPARAM_ID,
		AFC_DEVICERUNTIMEPARAM_ID
	};
	//单例
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


	// ACC参数
	CString parameterACCNames[] = 
	{
		_T("0101运营参数"),
		_T("0102行政处理手续费参数"),
		_T("0103超时补交费用参数"),	
		_T("0201设备控制参数"),	
		_T("0301车站位置参数"),	
		_T("0302计费站点参数"),	
		_T("0303区段参数"),	
		_T("0401票卡属性参数"),	
		_T("0501票卡发行商参数"),
		_T("0601基础票价表参数"),
		_T("0602票价级别表参数"),
		_T("0603票价调整策略表参数"),
		_T("0604时间浮动表参数"),
		_T("0605时间类型表参数"),
		_T("0606时间类型明细参数"),
		_T("0607折扣率表参数"),
		//_T("0608停运车站表"),
		_T("0701单条黑名单参数"),
		_T("0702区段黑名单参数"),
		_T("0801白名单参数"),
		_T("0802异地卡/城市代码对照参数"),

	};
	WORD parameterACCIDs[] = 
	{
		ACC_BUSINESS_ID						,					// ACC运营参数
		ACC_SERVICEFEE_ID					,					// ACC行政处理手续费参数
		ACC_OVERTIME_UPDATE_ID				,					// ACC超时补交费用参数
		ACC_DEVICE_ID						,					// ACC设备控制参数
		ACC_STATION_ID						,					// ACC车站位置参数
		ACC_CHARGE_STATION_ID				,					// ACC计费站点参数
		ACC_SECTION_ID						,					// ACC区段参数
		ACC_CARDATTRIBUTE_ID				,					// ACC票卡属性参数
		ACC_ISSUE_COMPANY_ID			    ,					// ACC票卡发行商参数
		ACC_BASE_PRICE_TABLE_ID				,					// ACC基础票价表参数
		ACC_PRICE_LEVEL_TABLE_ID			,					// ACC票价级别表参数
		ACC_PRICE_ADJUEST_TABLE_ID			,					// ACC票价调整策略表参数
		ACC_TIME_FLOATING_TABLE_ID			,					// ACC时间浮动表参数
		ACC_TIME_TYPE_TABLE_ID				,					// ACC时间类型表参数
		ACC_TTIME_TYPE_DETAIL_ID			,					// ACC时间类型明细参数
		ACC_DISSCOUNT_TABLE_ID				,					// ACC折扣率表参数
		//ACC_NOT_SERVICE_STATION_ID			,					// ACC停运车站表
		ACC_SINGLE_BLACK_LIST_ID			,					// ACC单条黑名单参数
		ACC_SECTION_BLACK_LIST_ID			,					// ACC区段黑名单参数
		ACC_WRITE_LIST_ID					,					// ACC白名单参数
		ACC_OTHER_CITY_CARD_ID				,					// ACC异地卡/城市代码对照参数	
	};
	//单例
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

	//// 9700线网地图参数 ---单独处理
	//{
	//	_tagParameterInputStruct &inputStruct = m_DataModel[ACC_STATION_MAP_ID];
	//	inputStruct.name = _T("线网地图参数");
	//	CString strID;
	//	strID.Format(_T("(%x)"), ACC_STATION_MAP_ID);
	//	inputStruct.name += strID;
	//	inputStruct.localCurrentVersion = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::STATION_MAP);
	//	inputStruct.localFutureVersion = theVERSION_INI.GetVersionNo(CVersionInfo::FUTURE,CVersionInfo::STATION_MAP);
	//}


	//分页信息
	int nTotalPage = 0,nCurrPage = 0,nPerPage = 0;
	nCurrPage  = 1;
	nPerPage   = MAX_ROW - 1;
	nTotalPage = m_DataModel.size() % nPerPage == 0 ? m_DataModel.size() / nPerPage : m_DataModel.size() / nPerPage +1;
	SetPageInfo(nTotalPage,nCurrPage,nPerPage);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      从U盘获取需要导出信息

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfoSvc::GetImportInfo()
{
	// 外部媒体认证  
	if(theFunction_INFO.GetUSBAuthFlag() == 1){
		// 如果认证USB设备失败，显示提示文言
		if(!AuthUSB()){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
			return;	
		}
	}
	// 先获取索引文件路径
	CString strUSBDrive = theTVM_INFO.GetUSBDrive();
	DEVICE_ID curDeviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	CString strDeviceID = curDeviceID.ToString();
	// 先从参数索引文件得到文件名
	CString strImportIndexFile = strUSBDrive + IMPORT_INDEX_FILE_PREFIX + strDeviceID + IMPORT_INDEX_FILE_SUFFIX;
	// 参数索引文件是否存在
	if (FALSE == ComCheckFileExist(strImportIndexFile)){
		// 检查通用文件是否存在
		strDeviceID.Format(_T("%.2X%.2X%.2X%.2X"),curDeviceID.bLineId,curDeviceID.bStationId,curDeviceID.bAfcDeviceType,0);
		strImportIndexFile = strUSBDrive + IMPORT_INDEX_FILE_PREFIX + strDeviceID + IMPORT_INDEX_FILE_SUFFIX;
		if(FALSE == ComCheckFileExist(strImportIndexFile)){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_ERROR_INDEX_EXIST));
			return ;
		}
	}

	CGetIni importIndexFile(strImportIndexFile);	
	int fileCount = importIndexFile.GetDataInt(SECTION_FILE, KEY_FILE_COUNT,0);	// 文件数量
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
		case CUR_TEST:		// 当前测试版
		case CUR_VER:		// 当前正式版
			m_DataModel[parameterID].importCurrentFrom = importFileName;
			m_DataModel[parameterID].importCurrentVersion = version;
			break;
		case FUT_TEST:		// 将来测试版
		case FUT_VER:		// 将来正式版
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
//@brief      从外部媒体导入参数(开始导入)
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
	//判断是否在线，如果在线就不允许导入数据
	if (theAPP_SESSION.IsSCConnected())
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_MSGBOX_ONLINE));
		CString comment(_T("INPUT_PARAM"));
		// 导入--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
		return false;
	}
	// 外部媒体认证  
	if(theFunction_INFO.GetUSBAuthFlag() == 1){
		// 如果认证USB设备失败，显示提示文言
		if(!AuthUSB()){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_U_DISK_ERROR));
			CString comment(_T("INPUT_PARAM"));
			// 导入--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
			return false;	
		}
	}
	typedef map<WORD,ParameterInputStruct>::iterator DataItem;
	for(DataItem iter = m_DataModel.begin();iter != m_DataModel.end();iter++)
	{
		DWORD parameterID = iter->first;
		ParameterInputStruct &inputStruct = iter->second;
		//如果导入路径和被导入路径都不为空。
		if(!inputStruct.importCurrentFrom.IsEmpty() && !inputStruct.importCurrentTo.IsEmpty()){
			if (!ComCheckAndCreateDirectory(inputStruct.importCurrentTo))
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_CREATE_DIR_FILED));
			}
			CString toFileName = inputStruct.importCurrentTo + GetFileNameFromFullPath(inputStruct.importCurrentFrom);
			//判断当前导入文件是否存在：存在 ？继续 ：返回错误zheng
			if (!ComCheckFileExist(inputStruct.importCurrentFrom))
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(DATAINPUT_UPDATE_TIP_INPUT_FILE_NOT_EXIST));
				CString comment(_T("INPUT_PARAM"));
				// 导入--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
				return false;
			}
			//实施导入操作
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
			// 导入--操作日志
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
	//1.导入参数
	CParamHelper::VEC_VERSION_INFO VecVersionInfo;
	thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
	// 初始化所有参数		
	if(!thePARAM_HELPER.InitAllParam()){
		theEXCEPTION_MGR.ProcessException(CLogException(CLogException::FILE_READ_FAIL, _T(""), __LINE__,_opl(GUIDE_DATA_INPUT_MSGBOX_PARAM)));

		CString comment(_T("INPUT_PARAM"));
		// 导入--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,false,comment);
	}

	//2.导入收据模板及声音文件
	theUPGRADE_HELPER::UpdateVoiceAndTemplateFolder();

	//3.导入应用程序或GUI文件
	if(theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade()){
		//不显示对话框
		//CMessageBox::Show(_opl(TITLE_DATA_INPUT),_opl(GUIDE_DATA_INPUT_MSGBOX_RESTART),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_10);
		//倒计时关机 时间为WHEN_RESTART_TIME
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
	// 向上位发送当前版本状况
	theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->SendParameterVersion();
	// 显示文言：导入完成
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_DATA_INPUT_FINISH));
	CString comment(_T("INPUT_PARAM"));
	// 导入--操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_INPUT_PARAM,true,comment);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      认证usb设备

@param      none

@retval     bool  ture:成功; false:失败

@exception  无
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


