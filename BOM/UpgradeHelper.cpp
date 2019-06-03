#include "stdafx.h"
#include "UpgradeHelper.h"
#include "FTPHelper.h"
#include "InfoZip.h"
#include <atlcomtime.h>
#include "SCMessageContentDefine.h"
#include "guideinfo.h"
#include "ServiceMgr.h"
#include "BOMForeService.h"
#include "VersionInfo.h"

//****************************************************************************
// 
// function  : 下载升级文件
// input:    vector<ParameterAndSoftwareUpdate> downLoadFiles 需要下载的升级文件的列表
// output:   无   
// return:	  true 下载成功
//			  false 下载失败
//****************************************************************************

CString CUpgradeHelper::m_strCurrentDirectory;
bool CUpgradeHelper::DownLoadFiles(vector<ParameterAndSoftwareUpdate> paramAndSoftUpgradeDatas)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	vector<pair<CString,CString>> downloadPaths;
	for (int i=0;i<paramAndSoftUpgradeDatas.size();i++)
	{
		ParameterAndSoftwareUpdate needDownLoadFile = paramAndSoftUpgradeDatas[i];
		CString downloadFolder = GetDownloadFolder(needDownLoadFile.parameterID,needDownLoadFile.versionType);
		CString upgradeDescriptionFileName = downloadFolder + UPGRADE_DATA_DESC_FILE_NAME;

		CVersionInfo::PROG_TYPE progType;
		bool isProgram = TryConvertParameterIDToProgarmType(needDownLoadFile.parameterID,progType);

		CVersionInfo::VER_TYPE verType;
		bool isCorrectVerType = TryConvertVerCodeToVerType(needDownLoadFile.versionType,verType);
		if(!isCorrectVerType)//不正确的版本类型。
		{
			continue;
		}
		
		ComCheckAndCreateDirectory(downloadFolder); //并建立新文件夹。

		if(needDownLoadFile.version != 0xFFFFFFFF)//新版本
		{
			downloadPaths.push_back(pair<CString,CString>(needDownLoadFile.downLoadPath,downloadFolder));//加入到需要下载列表中。
			if(isProgram)
			{
				needDownLoadFile.SaveToFile(upgradeDescriptionFileName); //保存升级说明文件。				
				vector<CVersionInfo::PROG_TYPE> saveVersionInfoProgTypes;
				saveVersionInfoProgTypes.push_back(progType);
				if(progType == CVersionInfo::RW_MAIN)
				{
					saveVersionInfoProgTypes.push_back(CVersionInfo::TW_MAIN);
				}
				if(progType==CVersionInfo::RW_APP)
				{
					saveVersionInfoProgTypes.push_back(CVersionInfo::TW_APP);
				}
				
				for(int i=0;i<saveVersionInfoProgTypes.size();i++){
					theVERSION_INI.SetBatchNo(verType,saveVersionInfoProgTypes[i],needDownLoadFile.softwareBatchNO); //保存版本信息。
					theVERSION_INI.SetCompanyID(verType,saveVersionInfoProgTypes[i],needDownLoadFile.providerID);
					theVERSION_INI.SetStartDate(verType,saveVersionInfoProgTypes[i],needDownLoadFile.startDate);
					theVERSION_INI.SetVersionNo(verType,saveVersionInfoProgTypes[i],needDownLoadFile.version);
					theVERSION_INI.SetVersionType(verType,saveVersionInfoProgTypes[i],needDownLoadFile.versionType);
					
				}
			}
		}
		else//删除特定版本。
		{
			if(isProgram)//程序
			{
				DeleteFolder(downloadFolder);//删除程序文件夹。
				theVERSION_INI.RemoveVersion(progType,verType); //删除版本信息。
			}
			else//参数
			{
				CString sParamId = _T("");
				sParamId.Format(_T("%.2X"),needDownLoadFile.parameterID);
				CString sParamFileName = PARAM_PREFIX + sParamId;
				thePARAM_HELPER.DeleteTheParamFile(sParamFileName,needDownLoadFile.versionType);//删除特定的参数。
			}	
		}
	}
	theFTP_HELPER.DownloadFiles(downloadPaths,false);//下载文件
	return true;
}


//****************************************************************************
// 
// function  : 检查BOM及其他程序是否需要升级
// input:     无
// output:   无   
// return:	  true 需要升级
//			  false 不需要升级
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileNeedUpgrade()
{
	CVersionInfo::VER_TYPE verType;
	if(CheckDownloadFileNeedUpgrade(CVersionInfo::MC,verType))
	{
		return true;
	}
	if(CheckDownloadFileNeedUpgrade(CVersionInfo::RW_MAIN,verType))
	{
		return true;
	}
	if(CheckDownloadFileNeedUpgrade(CVersionInfo::RW_APP,verType))
	{
		return true;
	}
	if(CheckDownloadFileNeedUpgrade(CVersionInfo::TW_MAIN,verType))
	{
		return true;
	}
	if(CheckDownloadFileNeedUpgrade(CVersionInfo::TW_APP,verType))
	{
		return true;
	}
	return false;
}


//****************************************************************************
// 
// function  : 移动下载的升级程序到升级目录
// input:     无
// output:   无   
// return:	  true 升级成功
//			  false 升级失败
//****************************************************************************
bool CUpgradeHelper::MoveDownloadFileToUpgradeFolder()
{
	if(!CheckDownloadFileNeedUpgrade()){
		return true;
	}

	if(!MoveDownloadFileToUpgradeFolderMC()){
		return false;
	}

	if(!MoveDownloadFileToUpgradeFolderTPU())
	{
		return false;
	}
	return true;
}


//****************************************************************************
// 
// function  : 升级程序
// input:     升级过程通知处理函数
// output:   无   
// return:	 
//			 
//****************************************************************************
bool CUpgradeHelper::Upgrade(CUpgradeHelper::OnUpgradeProcessFunc func)
{
	bool updateMCSuccess = Upgrade(CVersionInfo::MC,func);
	
	RW_RSP_STATUS rwStatus;

	bool updateRWMAINSuccess =  false;
	bool updateRWAPPSuccess = false;
	
	try
	{	
		CRWHelper::Reset(rwStatus);
		updateRWMAINSuccess = Upgrade(CVersionInfo::RW_MAIN,func);
		updateRWAPPSuccess = Upgrade(CVersionInfo::RW_APP,func);
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	bool updateTWMAINSuccess = false;
	bool updateTWAPPSuccess = false;
	try
	{
 		CTWHelper::Reset(rwStatus);
 		updateTWMAINSuccess = Upgrade(CVersionInfo::TW_MAIN,func);
 		updateTWAPPSuccess = Upgrade(CVersionInfo::TW_APP,func);
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
	}
	
 	bool hasSuccess = updateMCSuccess || updateRWMAINSuccess || updateRWAPPSuccess || updateTWMAINSuccess || updateTWAPPSuccess  ;
 	bool allSuccess = updateMCSuccess && updateRWMAINSuccess && updateRWAPPSuccess  && updateTWMAINSuccess && updateTWAPPSuccess;

	if(hasSuccess){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_UPDATE_SUCCESS));	
		Sleep(10 * 1000);
	}
	return allSuccess;
}




//****************************************************************************
// 
// function  : 采用默认通知处理函数升级程序
// input:     无
// output:   无   
// return:	 
//			 
//****************************************************************************
bool CUpgradeHelper::Upgrade()
{
	return Upgrade(OnUpgrade);
}


//****************************************************************************
// 
// function  : 采用给定通知处理函数升级特定的程序。
// input:     CVersionInfo::PROG_TYPE& progType 要升级的程序类型。
// input:     OnUpgradeProcessFunc func 通知处理函数。
// output:      
// return:	 
//			 
//****************************************************************************
bool CUpgradeHelper::Upgrade(CVersionInfo::PROG_TYPE progType,CUpgradeHelper::OnUpgradeProcessFunc func)
{
	if(progType==CVersionInfo::MC)//BOM主控程序不通过该方式升级。
	{
		return false;
	}
	vector<CString> updateFolders;
	GetUpgradeFolder(progType,updateFolders);
	if(updateFolders.size()<=0)
	{
		return false;
	}

	CString updateFolder = updateFolders[0];
	CString updateFilePrefix;
	switch(progType)
	{
		case CVersionInfo::RW_MAIN:
			updateFilePrefix = STR_BOM_RW_01;
			break;
		case CVersionInfo::RW_APP:
			updateFilePrefix = STR_BOM_RW_00;
			break;
		case CVersionInfo::TW_MAIN:
			updateFilePrefix = STR_BOM_RW_01;
			break;
		case CVersionInfo::TW_APP:
			updateFilePrefix = STR_BOM_RW_00;
			break;
		case CVersionInfo::TH_APP:
			updateFilePrefix = STR_BOM_TC_00;
			break;
		case CVersionInfo::TH_MAIN:
			updateFilePrefix = STR_BOM_TC_01;
			break;
		default:
			return false;
	}

	CString updateFile;
	updateFile = SearchOneFile(updateFolder + updateFilePrefix);
	if(updateFile == "")
	{
		return false;
	}
	int ret = 0;
	try{
		if(func!=NULL)
		{
			func(progType,CUpgradeHelper::BEGIN);
		}
		
		switch(progType)
		{
			case CVersionInfo::RW_MAIN:
				ret = CRWHelper::UpdateProgram(true);
				break;
			case CVersionInfo::RW_APP:
				ret = CRWHelper::UpdateProgram(false);
				break;
			case CVersionInfo::TW_MAIN:
				ret = CTWHelper::UpdateProgram(true);
				break;
			case CVersionInfo::TW_APP:
				ret = CTWHelper::UpdateProgram(false);
				break;
			case CVersionInfo::TH_MAIN:
				{
					CTHHelper* thHelper = ((CBOMForeService*)theSERVICE_MGR.GetService(MAIN_SVC))->GetTHHelper();
					//ret = thHelper->UpdateProgram(DLL_TYPE::DLL_BOOT,updateFolder) ? 0 :1;
					break;
				}
			case CVersionInfo::TH_APP:
				{
					CTHHelper* thHelper = ((CBOMForeService*)theSERVICE_MGR.GetService(MAIN_SVC))->GetTHHelper();
					//ret = thHelper->UpdateProgram(DLL_TYPE::DLL_API,updateFolder) ? 0: 1;
					ret = thHelper->UpdateProgram();
					break;
				}
			default:
				break;
		}	
		
		CVersionInfo::VER_TYPE verType;
		PARAM_VERSION_TYPE_CODE  verCode = thePARAM_HELPER.GetProgVersionType(updateFile);

		bool isProgram = TryConvertVerCodeToVerType(verCode,verType);

		if(isProgram && verType == CVersionInfo::FUTURE)
		{
			theVERSION_INI.SwitchVersion(progType,CVersionInfo::FUTURE,CVersionInfo::CURRENT,true);
		}

		func(progType,ret == 0?CUpgradeHelper::SUCCEED:CUpgradeHelper::FAILED);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(MAIN_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		return false;
	}
	return ret == 0?true:false;
	return true;
}

//****************************************************************************
// 
// function  : 采用默认通知处理函数升级特定的程序。
// input:     CVersionInfo::PROG_TYPE& progType 要升级的程序类型。
// output:      
// return:	 
//			 
//****************************************************************************
bool CUpgradeHelper::Upgrade(CVersionInfo::PROG_TYPE progType)
{
	return Upgrade(progType,OnUpgrade);
}

//****************************************************************************
// 
// function  : 程序在升级开始，成功或失败通过的处理函数
// input:     CVersionInfo::PROG_TYPE& progType 程序类型
// input:     UPGRADE_PROCESS& process 通知类型
// output:    无       
// return:	  无
//			 
//****************************************************************************
void CUpgradeHelper::OnUpgrade(CVersionInfo::PROG_TYPE progType,CUpgradeHelper::UPGRADE_PROCESS process)
{
	CString alertMessage;
	CString progName;

	switch(progType)
	{
		case CVersionInfo::MC:
			progName = "BOM主程序";
			break;
		case CVersionInfo::RW_APP:
			progName = "外部读写器应用部";
			break;
		case CVersionInfo::RW_MAIN:
			progName = "外部读写器主控部";
			break;
		case CVersionInfo::TW_APP:
			progName = "内部读写器应用部";
			break;
		case CVersionInfo::TW_MAIN:
			progName = "内部读写器主控部";
			break;
		case CVersionInfo::TH_MAIN:
			progName = "出票机主控部";
			break;
		case CVersionInfo::TH_APP:
			progName = "出票机应用部";
			break;
		default:
			progName = "";
			break;
	}
	switch(process)
	{
		case CUpgradeHelper::BEGIN:
			alertMessage = _T("正在更新")+progName+_T(",请稍候...");
			break;
		case CUpgradeHelper::SUCCEED:
			alertMessage = _T("更新")+progName+_T("成功");
			//theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);//如果更新成功，有硬件更新，改为关机。
			break;
		case CUpgradeHelper::FAILED:
			alertMessage = _T("更新")+progName+_T("失败");
			break;
		default:
			alertMessage = _T("");
	}
	theAPP_SESSION.ShowOfficeGuide(alertMessage);
	Sleep(3*1000);
}

//****************************************************************************
// 
// function  : 检查BOM主程序是否需要升级
// input:     CVersionInfo::PROG_TYPE progType 升级的程序类型
// output:    CVersionInfo::VER_TYPE& verType 升级的版本类型。         
// return:	  true 需要升级，升级版本由verType返回
//			  false 不需要升级
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileNeedUpgrade(__in CVersionInfo::PROG_TYPE progType, __out CVersionInfo::VER_TYPE& verType)
{
	CVersionInfo::VER_TYPE currentVersion = CVersionInfo::CURRENT;
	CVersionInfo::VER_TYPE futureVersion = CVersionInfo::FUTURE;

	CString currentVersionUpgradeFolder =  GetDownloadFolder(progType,currentVersion);
	CString futureVersionUpgradeFolder =   GetDownloadFolder(progType,futureVersion);
	return CheckDownloadFileNeedUpgrade(currentVersionUpgradeFolder,futureVersionUpgradeFolder,verType);
	return false;
}



//****************************************************************************
// 
// function  : 移动下载的BOM程序升级程序到升级目录
// input:    无
// output:   无         
// return:	  true 成功
//			  false 失败
//****************************************************************************
bool CUpgradeHelper::MoveDownloadFileToUpgradeFolderMC()
{
	CVersionInfo::VER_TYPE verType;
	CVersionInfo::PROG_TYPE progType = CVersionInfo::MC;
	if(!CheckDownloadFileNeedUpgrade(progType,verType))//如果不需要更新，直接返回。
	{
		return true;
	}
	vector<CString> upgradeFiles;
	GetDownloadUpgradeFiles(progType,verType,upgradeFiles);
	
	if(upgradeFiles.size()<=0)//没有找到升级文件
	{
		return false;
	}
	
	vector<CString> upgradeFolders;
	GetUpgradeFolderFromDownloadProgramType(progType,upgradeFolders);
	for(int i=0;i<upgradeFolders.size();i++)
	{
		if(!ExtractUpdateFile(upgradeFiles[0],upgradeFolders[i],true))//解压缩不成功
		{
			theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("解压缩不成功"));
			return false;
		}
	}
	ParameterAndSoftwareUpdate parameterAndSoftwareUpdate;
	parameterAndSoftwareUpdate.FromProgramFileName(upgradeFiles[0]);
	CParamHelper::VEC_VERSION_INFO vecVersionChangeInfos;
	CParamHelper::VERSION_CHANGE_INFO versionChangeInfo;

	versionChangeInfo.paramId = parameterAndSoftwareUpdate.parameterID;
	versionChangeInfo.versionType = parameterAndSoftwareUpdate.versionType;

	versionChangeInfo.oldVersion = theVERSION_INI.GetVersionNo(verType,progType);
	versionChangeInfo.oldStartDate = theVERSION_INI.GetStartDate(verType,progType);
	versionChangeInfo.newVersion = parameterAndSoftwareUpdate.version;
	versionChangeInfo.newStartDate = parameterAndSoftwareUpdate.startDate;

	vecVersionChangeInfos.push_back(versionChangeInfo);

	if(theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)!=NULL)
	{
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(vecVersionChangeInfos);
	}
	

	CString downloadFolder = GetDownloadFolder(progType,verType);
	DeleteFolder(downloadFolder);
	if(verType == CVersionInfo::FUTURE)//切换将来版为当前版，并删除将来版。
	{
		theVERSION_INI.SwitchVersion(progType,CVersionInfo::FUTURE,CVersionInfo::CURRENT,true);
	}

	return true;
}

//****************************************************************************
// 
// function  : 移动下载的BOM程序升级程序到升级目录
// input:    无
// output:   无         
// return:	  true 成功
//			  false 失败
//****************************************************************************
bool CUpgradeHelper::MoveDownloadFileToUpgradeFolderTPU()
{


	CVersionInfo::VER_TYPE verType;
	CVersionInfo::PROG_TYPE progTypes[] = {CVersionInfo::RW_MAIN,CVersionInfo::RW_APP};
	for (int i=0;i<2;i++)
	{
		CVersionInfo::PROG_TYPE progType = progTypes[i];
		if(CheckDownloadFileNeedUpgrade(progType,verType))
		{
			CString downloadFolder = GetDownloadFolder(progType,verType);
			
			vector<CString> upgradeFiles;
			GetDownloadUpgradeFiles(progType,verType,upgradeFiles);

			ParameterAndSoftwareUpdate parameterAndSoftwareUpdate;
			parameterAndSoftwareUpdate.FromProgramFileName(upgradeFiles[0]);
			CParamHelper::VEC_VERSION_INFO vecVersionChangeInfos;
			CParamHelper::VERSION_CHANGE_INFO versionChangeInfo;
			versionChangeInfo.paramId = parameterAndSoftwareUpdate.parameterID;
			versionChangeInfo.versionType = parameterAndSoftwareUpdate.versionType;

			versionChangeInfo.oldVersion = theVERSION_INI.GetVersionNo(verType,progType);
			versionChangeInfo.oldStartDate = theVERSION_INI.GetStartDate(verType,progType);
			versionChangeInfo.newVersion = parameterAndSoftwareUpdate.version;
			versionChangeInfo.newStartDate = parameterAndSoftwareUpdate.startDate;

			vecVersionChangeInfos.push_back(versionChangeInfo);
			if(theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)!=NULL)
			{
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(vecVersionChangeInfos);
			}

			vector<CString> updateFolders;
			GetUpgradeFolderFromDownloadProgramType(progType,updateFolders);
		
			DeleteFile(downloadFolder + UPGRADE_DATA_DESC_FILE_NAME);

			for(int i=0;i<updateFolders.size();i++)
			{
				CopyDirectory(downloadFolder,updateFolders[i]);
			}
			DeleteFolder(downloadFolder);

			if(verType == CVersionInfo::FUTURE)
			{
				theVERSION_INI.SwitchVersion(CVersionInfo::RW_MAIN,CVersionInfo::FUTURE,CVersionInfo::CURRENT,true);
			}
		}
	}
	return true;
}

//****************************************************************************
// 
// function  : 检查当前版本文件夹和将来版本文件夹中升级文件是否能够启用
// input:     CString& currentVersionFolder 当前版本升级文件存放的文件夹 
//			  CString& futureVersionFolder  将来版本升级文件存放的文件夹
// output:    CVersionInfo::VER_TYPE& verType 升级的版本类型。         
// return:	  true 需要升级，升级版本由verType返回
//			  false 不需要升级
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileNeedUpgrade(__in CString& currentVersionFolder,__in CString& futureVersionFolder,__out CVersionInfo::VER_TYPE& verType)
{
	ParameterAndSoftwareUpdate currentVersionUpgradeDataDescription;
	ParameterAndSoftwareUpdate futureVersionUpgradeDataDescription;
	bool currentVersionHasStart = CheckDownloadFileUpgradeDateHasStart(currentVersionFolder,currentVersionUpgradeDataDescription);
	bool futureVersionHasStart =  CheckDownloadFileUpgradeDateHasStart(futureVersionFolder,futureVersionUpgradeDataDescription);

	if(!currentVersionHasStart && !futureVersionHasStart)//不需要升级
	{
		return false;
	}

	if(currentVersionHasStart && !futureVersionHasStart)//当前版启用
	{
		verType = CVersionInfo::CURRENT;
		return true;
	}

	if(!currentVersionHasStart && futureVersionHasStart)//将来版启用
	{
		verType = CVersionInfo::FUTURE;
		return true;
	}

	//同时启用，启用日期晚的
	if( currentVersionUpgradeDataDescription.startDate > futureVersionUpgradeDataDescription.startDate) 
	{
		verType = CVersionInfo::CURRENT;
		return true;
	}
	
	if( currentVersionUpgradeDataDescription.startDate < futureVersionUpgradeDataDescription.startDate) 
	{
		verType = CVersionInfo::FUTURE;
		return true;
	}
	//启用版本大的
	if(currentVersionUpgradeDataDescription.version > futureVersionUpgradeDataDescription.version)
	{
		verType = CVersionInfo::CURRENT;
		return true;
	}
	
	if(currentVersionUpgradeDataDescription.version < futureVersionUpgradeDataDescription.version)
	{
		verType = CVersionInfo::FUTURE;
		return true;
	}
	//启用批次号大的
	if(currentVersionUpgradeDataDescription.softwareBatchNO >= futureVersionUpgradeDataDescription.softwareBatchNO)
	{
		verType = CVersionInfo::CURRENT;
		return true;
	}

	verType = CVersionInfo::FUTURE;
	return true;
}


//****************************************************************************
// 
// function : 检查某升级目录下升级文件是否能够启用。
// input:     CString& upgradeDataFolder 升级文件存放的目录。
// output:    ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate 升级描述类       
// return:	  true 已过生效时间，应该升级
//			  false 未到生效时间，不应该升级
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileUpgradeDateHasStart(__in const CString& upgradeDataFolder,__out ParameterAndSoftwareUpdate& parameterAndSoftwareUpdate)
{

	CString upgradeDataDescriptionFileName = SearchOneFile(upgradeDataFolder + UPGRADE_DATA_DESC_FILE_NAME);
	vector<CString> upgradeDataFileNames;
	SearchFile(upgradeDataFolder + _T("*"),upgradeDataFileNames);
	if(upgradeDataDescriptionFileName!="") //找到升级描述文件
	{
		if(upgradeDataFileNames.size()<2)//只有升级描述文件，没有升级文件。
		{
			DeleteFile(upgradeDataDescriptionFileName);//删除升级描述文件
			return false;
		}
		CXFile cxFile(upgradeDataDescriptionFileName,_T(""));
		VARIABLE_DATA binFileData;
		cxFile.ReadBinFileWithoutSUM(binFileData);
		if(binFileData.nLen>=84)//可解析的描述文件。
		{
			 parameterAndSoftwareUpdate = ParameterAndSoftwareUpdate(binFileData.lpData);
			 return CheckDownloadFileUpgradeDateHasStart(parameterAndSoftwareUpdate);
		}
		DeleteFile(upgradeDataDescriptionFileName);//删除错误的升级描述文件
	}
	
	if(upgradeDataFileNames.size()>0)////没有升级描述文件，但是有其他文件，视为升级文件。
	{
		parameterAndSoftwareUpdate.FromProgramFileName(upgradeDataFileNames[0]);
		parameterAndSoftwareUpdate.startDate = _DATE(COleDateTime::GetCurrentTime());
		return CheckDownloadFileUpgradeDateHasStart(parameterAndSoftwareUpdate);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      解压缩程序更新包

@param      (i)CString sProgramFileUpdateDir   压缩文件路径
@param      (i)CString sFileName			   压缩文件名
@param      (i)bool isAPP					   是否是BOM主程序

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CUpgradeHelper::ExtractUpdateFile(CString sourceFilePath,CString targetFolder, bool isAPP)
{
	CInfoZip InfoZip;
	try
	{	
		SetFileAttributes(sourceFilePath,FILE_ATTRIBUTE_NORMAL);
		// 获得系统路径
		CString sAppPath(_T(""));
		GetAppPath(sAppPath);

		USES_CONVERSION;
		if(InfoZip.AutoExtractFiles(T2A(sourceFilePath), T2A(targetFolder)) == FALSE) {
			DeleteFile(sourceFilePath);
			return false;
		}
		if (!DeleteFile(sourceFilePath)) {
			return false;			
		}

		// 如果是BOM应用程序
		if (isAPP) {
			// 拷贝壳程序到主程序目录
			CString cBomStarterFilePath =  SearchOneFile(targetFolder + _T("Launcher.exe"));
			if(cBomStarterFilePath!="")
			{
				SetFileAttributes(sAppPath + _T("Launcher.exe"), FILE_ATTRIBUTE_NORMAL);
				SetFileAttributes(targetFolder + _T("Launcher.exe"), FILE_ATTRIBUTE_NORMAL);
				CopyFile(cBomStarterFilePath,sAppPath + _T("Launcher.exe"),FALSE);
				DeleteFile(cBomStarterFilePath);
			}
			CString cUpdateFilePath = SearchOneFile(targetFolder+_T("BOM_UPDATE_BEFORE.bat"));
			if(cUpdateFilePath!="")
			{
				ComExecuteApp(cUpdateFilePath,NULL);
			}

			CString cVersionInfoPath = SearchOneFile(targetFolder + _T("VersionInfo.INI"));
			if(cVersionInfoPath!="")
			{
				SetFileAttributes(targetFolder + _T("VersionInfo.INI"), FILE_ATTRIBUTE_NORMAL);

				CVersionInfo* newVersionInfo = CVersionInfo::GetInstance(targetFolder + _T("VersionInfo.INI"));

				newVersionInfo->SetVersionType(CVersionInfo::CURRENT,CVersionInfo::MC,theVERSION_INI.GetVersionType(CVersionInfo::CURRENT,CVersionInfo::MC));
				newVersionInfo->SetBatchNo(CVersionInfo::CURRENT,CVersionInfo::MC,theVERSION_INI.GetBatchNo(CVersionInfo::CURRENT,CVersionInfo::MC));
				newVersionInfo->SetCompanyID(CVersionInfo::CURRENT,CVersionInfo::MC,theVERSION_INI.GetCompanyID(CVersionInfo::CURRENT,CVersionInfo::MC));
				newVersionInfo->SetStartDate(CVersionInfo::CURRENT,CVersionInfo::MC,theVERSION_INI.GetStartDate(CVersionInfo::CURRENT,CVersionInfo::MC));
				newVersionInfo->SetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC,theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC));

				newVersionInfo->SetVersionType(CVersionInfo::FUTURE,CVersionInfo::MC,theVERSION_INI.GetVersionType(CVersionInfo::FUTURE,CVersionInfo::MC));
				newVersionInfo->SetBatchNo(CVersionInfo::FUTURE,CVersionInfo::MC,theVERSION_INI.GetBatchNo(CVersionInfo::FUTURE,CVersionInfo::MC));
				newVersionInfo->SetCompanyID(CVersionInfo::FUTURE,CVersionInfo::MC,theVERSION_INI.GetCompanyID(CVersionInfo::FUTURE,CVersionInfo::MC));
				newVersionInfo->SetStartDate(CVersionInfo::FUTURE,CVersionInfo::MC,theVERSION_INI.GetStartDate(CVersionInfo::FUTURE,CVersionInfo::MC));
				newVersionInfo->SetVersionNo(CVersionInfo::FUTURE,CVersionInfo::MC,theVERSION_INI.GetVersionNo(CVersionInfo::FUTURE,CVersionInfo::MC));


				CVersionInfo::DeleteInstance(newVersionInfo);
			}
			//拷贝TH程序。
 			vector<CString> thUpgradeFiles;
 			SearchFile(targetFolder + _T("BOM_TC_*.bin"),thUpgradeFiles);
 			for (int i=0;i<thUpgradeFiles.size();i++)
 			{
 				CopyFile(thUpgradeFiles[i],sAppPath + BOM_TC_UPDATE_DIR + GetFileNameFromFullPath(thUpgradeFiles[i]),FALSE);
 				DeleteFile(thUpgradeFiles[i]);	
 			}			
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      解压缩程序更新包

@param      (i)CString sProgramFileUpdateDir   压缩文件路径
@param      (i)CString sFileName			   压缩文件名
@param      (i)bool isAPP					   是否是BOM主程序

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CUpgradeHelper::ExtractUpdateFileToCurrentFolder(CString sProgramFileUpdateDir,CString sFileName,bool isAPP)
{
	return ExtractUpdateFile(sProgramFileUpdateDir+_T("\\")+sFileName,sProgramFileUpdateDir,isAPP);
}

//****************************************************************************
// 
// function : 检查某升级目录下升级文件是否能够启用。
// input:     CString& upgradeDataFolder 升级文件存放的目录。
// output:    无       
// return:	  true 已过生效时间，应该升级
//			  false 未到生效时间，不应该升级
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileUpgradeDateHasStart(__in const ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate)
{
	_DATE currentDate(COleDateTime::GetCurrentTime());
	switch(ParameterAndSoftwareUpdate.versionType)
	{
		case CUR_VER:
		case CUR_TEST: //当前版，直接生效。
				return true; 
		case FUT_VER:
		case FUT_TEST:
				return  (currentDate > ParameterAndSoftwareUpdate.startDate) || (ParameterAndSoftwareUpdate.startDate == currentDate); //生效日期小于或者等于今天，则生效。
		default:
				return false;
	}
	return false;
}


//****************************************************************************
// 
// function : 获得存放程序升级文件的目录。
// input:     CVersionInfo::PROG_TYPE progType 程序类型
// input:    CVersionInfo::VER_TYPE& verType 升级的版本类型。      
// return:	  CString 升级路径
//			  
//****************************************************************************
__inline CString CUpgradeHelper::GetDownloadFolder(__in  CVersionInfo::PROG_TYPE& progType,__in CVersionInfo::VER_TYPE& verType)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);

	CString downloadFolder;

	switch(progType)
	{
		case CVersionInfo::MC: 
		case CVersionInfo::TH_MAIN: //TH升级程序与BOM程序一起打包。
		case CVersionInfo::TH_APP:
			downloadFolder = BOM_MC_DOWNLOAD_DIR;
			break;
		case CVersionInfo::RW_APP:
			downloadFolder = BOM_TPU_APP_DOWNLOAD_DIR;
			break;
		case CVersionInfo::RW_MAIN:
			downloadFolder = BOM_TPU_MAIN_DOWNLOAD_DIR;
			break;
		default:
			break;
	}
	CString versionTypeFolder;
	switch(verType)
	{
		case CVersionInfo::CURRENT:
			versionTypeFolder="CURRENT\\";
			break;
		case CVersionInfo::FUTURE:
			versionTypeFolder="FUTURE\\";
			break;
		default:
			break;
	}
	return CString(szAppPath) + downloadFolder + versionTypeFolder;
}

//****************************************************************************
// 
// function : 获得存放下载文件的目录。
// input:    PARAMETER_ID& progType 程序类型
// input:   PARAM_VERSION_TYPE_CODE& verCode 升级的版本类型。      
// return:	  CString 升级路径
//			  
//****************************************************************************
__inline  CString CUpgradeHelper::GetDownloadFolder(__in PARAMETER_ID& progType,__in  PARAM_VERSION_TYPE_CODE& verCode)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);

	CString versionTypeFolder;
	switch(verCode)
	{
	case CUR_VER:
	case CUR_TEST:
		versionTypeFolder="CURRENT\\";
		break;
	case FUT_VER:
	case FUT_TEST:
		versionTypeFolder="FUTURE\\";
		break;
	default:
		break;
	}

	CString downloadFolder;
	switch(progType)
	{
	case AFC_BOMPROGRAM_ID:
		downloadFolder = CString(szAppPath) + BOM_MC_DOWNLOAD_DIR + versionTypeFolder;
		break;
	case AFC_TPUPROGRAM_ID:
		downloadFolder = CString(szAppPath) + BOM_TPU_APP_DOWNLOAD_DIR + versionTypeFolder;
		break;
	//case AFC_TPUMAINPROGRAM_ID:
	//	downloadFolder = CString(szAppPath) + BOM_TPU_MAIN_DOWNLOAD_DIR + versionTypeFolder;
	//	break;
	default:
		downloadFolder = CString(szAppPath) + BOM_PM_UPDATE_DIR;
		break;
	}

	return downloadFolder;
}


//****************************************************************************
// 
// function : 获得升级文件存放的目录。
// input:     CVersionInfo::PROG_TYPE progType 程序类型 
// output:	 vector<CString>& updateFolders 升级文件存放的目录
// return:	 无
//			  
//****************************************************************************
__inline void CUpgradeHelper::GetUpgradeFolderFromDownloadProgramType(__in CVersionInfo::PROG_TYPE& progType,__out vector<CString>& updateFolders)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);


	CString updateFolder;

	switch(progType)
	{
	case CVersionInfo::MC: 
		updateFolders.push_back(CString(szAppPath)+BOM_MC_UPDATE_DIR);
		break;
	case CVersionInfo::TH_MAIN: //TH升级程序与BOM程序一起打包。
	case CVersionInfo::TH_APP:
		updateFolders.push_back(CString(szAppPath)+BOM_TC_UPDATE_DIR);
		break;
	case CVersionInfo::RW_APP:
	case CVersionInfo::RW_MAIN:
		updateFolders.push_back(CString(szAppPath)+BOM_IC_UPDATE_DIR);
		updateFolders.push_back(CString(szAppPath)+BOM_TW_UPDATE_DIR);
		break;
	
	default:
		break;
	}
}


//****************************************************************************
// 
// function : 获得升级文件存放的目录。
// input:     CVersionInfo::PROG_TYPE progType 程序类型 
// output:	 vector<CString>& updateFolders 升级文件存放的目录
// return:	 无
//			  
//****************************************************************************
__inline void CUpgradeHelper::GetUpgradeFolder(__in CVersionInfo::PROG_TYPE& progType,__out vector<CString>& updateFolders)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);


	CString updateFolder;

	switch(progType)
	{
	case CVersionInfo::MC: 
		updateFolders.push_back(CString(szAppPath)+BOM_MC_UPDATE_DIR);
		break;
	case CVersionInfo::TH_MAIN: //TH升级程序与BOM程序一起打包。
	case CVersionInfo::TH_APP:
		updateFolders.push_back(CString(szAppPath)+BOM_TC_UPDATE_DIR);
		break;
	case CVersionInfo::RW_APP:
	case CVersionInfo::RW_MAIN:
		updateFolders.push_back(CString(szAppPath)+BOM_IC_UPDATE_DIR);
		break;
	case CVersionInfo::TW_APP:
	case CVersionInfo::TW_MAIN:
		updateFolders.push_back(CString(szAppPath)+BOM_TW_UPDATE_DIR);
		break;
	default:
		break;
	}
}



//****************************************************************************
// 
// function : 获得升级文件。
// input:     CVersionInfo::PROG_TYPE progType 程序类型
// input:    CVersionInfo::VER_TYPE& verType 升级的版本类型。 
// output:	 vector<CString>& upgradeFiles 升级文件
// return:	  无
//			  
//****************************************************************************
__inline void CUpgradeHelper::GetDownloadUpgradeFiles(__in  CVersionInfo::PROG_TYPE& progType,__in CVersionInfo::VER_TYPE& verType,__out vector<CString>& upgradeFiles)
{
	CString downloadFolder =GetDownloadFolder(progType,verType);
	CString searchCondition =downloadFolder  + _T("*");
	SearchFile(searchCondition,upgradeFiles);

	CString ParameterAndSoftwareUpdate = downloadFolder + UPGRADE_DATA_DESC_FILE_NAME;
	vector<CString>::iterator iter = upgradeFiles.begin();
	while(iter!=upgradeFiles.end()){
		if (*iter == ParameterAndSoftwareUpdate)
		{
			upgradeFiles.erase(iter);
			break;
		}
		iter++;
	}
	
}

//****************************************************************************
// 
// function : 将VersionCode转化为VersionType。
// input:	 const PARAM_VERSION_TYPE_CODE& verCode
// output:	 CVersionInfo::VER_TYPE& verType
// return:   bool 成功转换，否则转换不成功。 
//			  
//****************************************************************************
__inline  bool CUpgradeHelper::TryConvertVerCodeToVerType(__in const PARAM_VERSION_TYPE_CODE& verCode,__out CVersionInfo::VER_TYPE& verType)
{
	switch(verCode)
	{
		case CUR_VER:
		case CUR_TEST:
			verType = CVersionInfo::CURRENT;
			break;
		case FUT_VER:
		case FUT_TEST:
			verType = CVersionInfo::FUTURE;
			break;
		default:
			return false;
	}

	return true;
}


//****************************************************************************
// 
// function : 将PARAMETER_ID转化为PROG_TYPE。
// input:	 const PARAMETER_ID& praameterID
// output:	 CVersionInfo::PROG_TYPE& progType
// return:   bool 成功转换，否则转换不成功。 
//			  
//****************************************************************************
__inline  bool CUpgradeHelper::TryConvertParameterIDToProgarmType(__in const PARAMETER_ID& parameterID,__out CVersionInfo::PROG_TYPE& progType)
{
	switch(parameterID)
	{
		case AFC_BOMPROGRAM_ID:
			progType = CVersionInfo::MC;
			break;
		case AFC_TPUPROGRAM_ID:
			progType = CVersionInfo::RW_APP;
			break;
		//case AFC_TPUMAINPROGRAM_ID:
		//	progType = CVersionInfo::RW_MAIN;
		//	break;
		default:
			return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      拷贝目录
@param      none
@retval     none
@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL CUpgradeHelper::CopyDirectory(LPCTSTR lpszSourceDir, LPCTSTR lpszDestDir)
{
	BOOL bRet = TRUE;
	CString sDestination;

	if(FALSE == ComCheckDirectory(lpszSourceDir))
	{
		return FALSE;
	}

	if(FALSE == ComCheckDirectory(lpszDestDir))
	{
		return FALSE;
	}

	CString sDirectory = lpszSourceDir;
	CString sDirectory2 = lpszSourceDir;
	sDirectory2.TrimLeft('\\');

	int nPos = sDirectory.ReverseFind('\\');
	if (nPos != -1)
	{
		sDirectory = sDirectory.Mid(nPos+1);
	}

	ComSetCurrentDir(lpszSourceDir,m_strCurrentDirectory);
	bRet = RecursiveFileList(sDirectory,sDirectory2, lpszDestDir, TRUE);
	return bRet;	
}


BOOL CUpgradeHelper::RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir, BOOL bIsFirst)
{

	CFileFind finder;
	CStringArray sDirectoryArray;
	CString sDestination;
	CString sPrevDirectory;

	/* remember current directory */
	sPrevDirectory = m_strCurrentDirectory;

	/* go to requested directory */
	m_strCurrentDirectory = m_strCurrentDirectory + _T("\\") + lpszPath;

	/* add directory (without filename), so we can create the directory structure later */
	CString strPath = sPrevDirectory;

	ComGetDestDir(strPath + _T("\\") + lpszPath,lpszOrgPath,lpszDestDir,sDestination);
	if(bIsFirst==FALSE)
	{
		CreateDirectory(sDestination,0);
	}
	CString sFileName, sPath;
	BOOL bContinue = finder.FindFile(m_strCurrentDirectory + _T("\\*.*"));
	if (!bContinue)
	{
		/* the directory is empty; just close up and return. */
		finder.Close();

		/* go back to parent directory */
		m_strCurrentDirectory = sPrevDirectory;
		return TRUE;
	}

	while (bContinue)
	{
		bContinue = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		/* is selected item a directory ? */
		if (finder.IsDirectory())
		{
			sDirectoryArray.Add(finder.GetFileName());
		}
		else
		{
			sFileName = finder.GetFileName();
			sPath = sPrevDirectory + _T("\\") + lpszPath;
			CString sLocalFileName = RenServerName2LocalName(sFileName);

			if(sLocalFileName != _T(""))
			{
				SetFileAttributes(sDestination+_T("\\")+ sLocalFileName,FILE_ATTRIBUTE_NORMAL);
				if(FALSE == CopyFile(sPath+_T("\\")+ sFileName, sDestination+_T("\\")+ sLocalFileName, FALSE))
				{
					return FALSE;
				}
			}
		}
	}
	finder.Close();

	/* get contains of directories */
	for (int i = 0; i < sDirectoryArray.GetSize(); i++)
	{
		if (FALSE == RecursiveFileList(sDirectoryArray.GetAt(i), lpszOrgPath, lpszDestDir, FALSE))
		{
			m_strCurrentDirectory = sPrevDirectory;
			return FALSE;
		}
	}

	/* go back to parent directory */
	m_strCurrentDirectory = sPrevDirectory;

	return TRUE;
}

CString CUpgradeHelper::RenServerName2LocalName(CString sServerFileName)
{
	CString sLocalName = _T("");	
	sLocalName = sServerFileName;
	return sLocalName;
}