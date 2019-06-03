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
// function  : ���������ļ�
// input:    vector<ParameterAndSoftwareUpdate> downLoadFiles ��Ҫ���ص������ļ����б�
// output:   ��   
// return:	  true ���سɹ�
//			  false ����ʧ��
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
		if(!isCorrectVerType)//����ȷ�İ汾���͡�
		{
			continue;
		}
		
		ComCheckAndCreateDirectory(downloadFolder); //���������ļ��С�

		if(needDownLoadFile.version != 0xFFFFFFFF)//�°汾
		{
			downloadPaths.push_back(pair<CString,CString>(needDownLoadFile.downLoadPath,downloadFolder));//���뵽��Ҫ�����б��С�
			if(isProgram)
			{
				needDownLoadFile.SaveToFile(upgradeDescriptionFileName); //��������˵���ļ���				
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
					theVERSION_INI.SetBatchNo(verType,saveVersionInfoProgTypes[i],needDownLoadFile.softwareBatchNO); //����汾��Ϣ��
					theVERSION_INI.SetCompanyID(verType,saveVersionInfoProgTypes[i],needDownLoadFile.providerID);
					theVERSION_INI.SetStartDate(verType,saveVersionInfoProgTypes[i],needDownLoadFile.startDate);
					theVERSION_INI.SetVersionNo(verType,saveVersionInfoProgTypes[i],needDownLoadFile.version);
					theVERSION_INI.SetVersionType(verType,saveVersionInfoProgTypes[i],needDownLoadFile.versionType);
					
				}
			}
		}
		else//ɾ���ض��汾��
		{
			if(isProgram)//����
			{
				DeleteFolder(downloadFolder);//ɾ�������ļ��С�
				theVERSION_INI.RemoveVersion(progType,verType); //ɾ���汾��Ϣ��
			}
			else//����
			{
				CString sParamId = _T("");
				sParamId.Format(_T("%.2X"),needDownLoadFile.parameterID);
				CString sParamFileName = PARAM_PREFIX + sParamId;
				thePARAM_HELPER.DeleteTheParamFile(sParamFileName,needDownLoadFile.versionType);//ɾ���ض��Ĳ�����
			}	
		}
	}
	theFTP_HELPER.DownloadFiles(downloadPaths,false);//�����ļ�
	return true;
}


//****************************************************************************
// 
// function  : ���BOM�����������Ƿ���Ҫ����
// input:     ��
// output:   ��   
// return:	  true ��Ҫ����
//			  false ����Ҫ����
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
// function  : �ƶ����ص�������������Ŀ¼
// input:     ��
// output:   ��   
// return:	  true �����ɹ�
//			  false ����ʧ��
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
// function  : ��������
// input:     ��������֪ͨ������
// output:   ��   
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
// function  : ����Ĭ��֪ͨ��������������
// input:     ��
// output:   ��   
// return:	 
//			 
//****************************************************************************
bool CUpgradeHelper::Upgrade()
{
	return Upgrade(OnUpgrade);
}


//****************************************************************************
// 
// function  : ���ø���֪ͨ�����������ض��ĳ���
// input:     CVersionInfo::PROG_TYPE& progType Ҫ�����ĳ������͡�
// input:     OnUpgradeProcessFunc func ֪ͨ��������
// output:      
// return:	 
//			 
//****************************************************************************
bool CUpgradeHelper::Upgrade(CVersionInfo::PROG_TYPE progType,CUpgradeHelper::OnUpgradeProcessFunc func)
{
	if(progType==CVersionInfo::MC)//BOM���س���ͨ���÷�ʽ������
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
// function  : ����Ĭ��֪ͨ�����������ض��ĳ���
// input:     CVersionInfo::PROG_TYPE& progType Ҫ�����ĳ������͡�
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
// function  : ������������ʼ���ɹ���ʧ��ͨ���Ĵ�����
// input:     CVersionInfo::PROG_TYPE& progType ��������
// input:     UPGRADE_PROCESS& process ֪ͨ����
// output:    ��       
// return:	  ��
//			 
//****************************************************************************
void CUpgradeHelper::OnUpgrade(CVersionInfo::PROG_TYPE progType,CUpgradeHelper::UPGRADE_PROCESS process)
{
	CString alertMessage;
	CString progName;

	switch(progType)
	{
		case CVersionInfo::MC:
			progName = "BOM������";
			break;
		case CVersionInfo::RW_APP:
			progName = "�ⲿ��д��Ӧ�ò�";
			break;
		case CVersionInfo::RW_MAIN:
			progName = "�ⲿ��д�����ز�";
			break;
		case CVersionInfo::TW_APP:
			progName = "�ڲ���д��Ӧ�ò�";
			break;
		case CVersionInfo::TW_MAIN:
			progName = "�ڲ���д�����ز�";
			break;
		case CVersionInfo::TH_MAIN:
			progName = "��Ʊ�����ز�";
			break;
		case CVersionInfo::TH_APP:
			progName = "��Ʊ��Ӧ�ò�";
			break;
		default:
			progName = "";
			break;
	}
	switch(process)
	{
		case CUpgradeHelper::BEGIN:
			alertMessage = _T("���ڸ���")+progName+_T(",���Ժ�...");
			break;
		case CUpgradeHelper::SUCCEED:
			alertMessage = _T("����")+progName+_T("�ɹ�");
			//theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);//������³ɹ�����Ӳ�����£���Ϊ�ػ���
			break;
		case CUpgradeHelper::FAILED:
			alertMessage = _T("����")+progName+_T("ʧ��");
			break;
		default:
			alertMessage = _T("");
	}
	theAPP_SESSION.ShowOfficeGuide(alertMessage);
	Sleep(3*1000);
}

//****************************************************************************
// 
// function  : ���BOM�������Ƿ���Ҫ����
// input:     CVersionInfo::PROG_TYPE progType �����ĳ�������
// output:    CVersionInfo::VER_TYPE& verType �����İ汾���͡�         
// return:	  true ��Ҫ�����������汾��verType����
//			  false ����Ҫ����
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
// function  : �ƶ����ص�BOM����������������Ŀ¼
// input:    ��
// output:   ��         
// return:	  true �ɹ�
//			  false ʧ��
//****************************************************************************
bool CUpgradeHelper::MoveDownloadFileToUpgradeFolderMC()
{
	CVersionInfo::VER_TYPE verType;
	CVersionInfo::PROG_TYPE progType = CVersionInfo::MC;
	if(!CheckDownloadFileNeedUpgrade(progType,verType))//�������Ҫ���£�ֱ�ӷ��ء�
	{
		return true;
	}
	vector<CString> upgradeFiles;
	GetDownloadUpgradeFiles(progType,verType,upgradeFiles);
	
	if(upgradeFiles.size()<=0)//û���ҵ������ļ�
	{
		return false;
	}
	
	vector<CString> upgradeFolders;
	GetUpgradeFolderFromDownloadProgramType(progType,upgradeFolders);
	for(int i=0;i<upgradeFolders.size();i++)
	{
		if(!ExtractUpdateFile(upgradeFiles[0],upgradeFolders[i],true))//��ѹ�����ɹ�
		{
			theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,_T("��ѹ�����ɹ�"));
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
	if(verType == CVersionInfo::FUTURE)//�л�������Ϊ��ǰ�棬��ɾ�������档
	{
		theVERSION_INI.SwitchVersion(progType,CVersionInfo::FUTURE,CVersionInfo::CURRENT,true);
	}

	return true;
}

//****************************************************************************
// 
// function  : �ƶ����ص�BOM����������������Ŀ¼
// input:    ��
// output:   ��         
// return:	  true �ɹ�
//			  false ʧ��
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
// function  : ��鵱ǰ�汾�ļ��кͽ����汾�ļ����������ļ��Ƿ��ܹ�����
// input:     CString& currentVersionFolder ��ǰ�汾�����ļ���ŵ��ļ��� 
//			  CString& futureVersionFolder  �����汾�����ļ���ŵ��ļ���
// output:    CVersionInfo::VER_TYPE& verType �����İ汾���͡�         
// return:	  true ��Ҫ�����������汾��verType����
//			  false ����Ҫ����
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileNeedUpgrade(__in CString& currentVersionFolder,__in CString& futureVersionFolder,__out CVersionInfo::VER_TYPE& verType)
{
	ParameterAndSoftwareUpdate currentVersionUpgradeDataDescription;
	ParameterAndSoftwareUpdate futureVersionUpgradeDataDescription;
	bool currentVersionHasStart = CheckDownloadFileUpgradeDateHasStart(currentVersionFolder,currentVersionUpgradeDataDescription);
	bool futureVersionHasStart =  CheckDownloadFileUpgradeDateHasStart(futureVersionFolder,futureVersionUpgradeDataDescription);

	if(!currentVersionHasStart && !futureVersionHasStart)//����Ҫ����
	{
		return false;
	}

	if(currentVersionHasStart && !futureVersionHasStart)//��ǰ������
	{
		verType = CVersionInfo::CURRENT;
		return true;
	}

	if(!currentVersionHasStart && futureVersionHasStart)//����������
	{
		verType = CVersionInfo::FUTURE;
		return true;
	}

	//ͬʱ���ã������������
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
	//���ð汾���
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
	//�������κŴ��
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
// function : ���ĳ����Ŀ¼�������ļ��Ƿ��ܹ����á�
// input:     CString& upgradeDataFolder �����ļ���ŵ�Ŀ¼��
// output:    ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate ����������       
// return:	  true �ѹ���Чʱ�䣬Ӧ������
//			  false δ����Чʱ�䣬��Ӧ������
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileUpgradeDateHasStart(__in const CString& upgradeDataFolder,__out ParameterAndSoftwareUpdate& parameterAndSoftwareUpdate)
{

	CString upgradeDataDescriptionFileName = SearchOneFile(upgradeDataFolder + UPGRADE_DATA_DESC_FILE_NAME);
	vector<CString> upgradeDataFileNames;
	SearchFile(upgradeDataFolder + _T("*"),upgradeDataFileNames);
	if(upgradeDataDescriptionFileName!="") //�ҵ����������ļ�
	{
		if(upgradeDataFileNames.size()<2)//ֻ�����������ļ���û�������ļ���
		{
			DeleteFile(upgradeDataDescriptionFileName);//ɾ�����������ļ�
			return false;
		}
		CXFile cxFile(upgradeDataDescriptionFileName,_T(""));
		VARIABLE_DATA binFileData;
		cxFile.ReadBinFileWithoutSUM(binFileData);
		if(binFileData.nLen>=84)//�ɽ����������ļ���
		{
			 parameterAndSoftwareUpdate = ParameterAndSoftwareUpdate(binFileData.lpData);
			 return CheckDownloadFileUpgradeDateHasStart(parameterAndSoftwareUpdate);
		}
		DeleteFile(upgradeDataDescriptionFileName);//ɾ����������������ļ�
	}
	
	if(upgradeDataFileNames.size()>0)////û�����������ļ��������������ļ�����Ϊ�����ļ���
	{
		parameterAndSoftwareUpdate.FromProgramFileName(upgradeDataFileNames[0]);
		parameterAndSoftwareUpdate.startDate = _DATE(COleDateTime::GetCurrentTime());
		return CheckDownloadFileUpgradeDateHasStart(parameterAndSoftwareUpdate);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ѹ��������°�

@param      (i)CString sProgramFileUpdateDir   ѹ���ļ�·��
@param      (i)CString sFileName			   ѹ���ļ���
@param      (i)bool isAPP					   �Ƿ���BOM������

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CUpgradeHelper::ExtractUpdateFile(CString sourceFilePath,CString targetFolder, bool isAPP)
{
	CInfoZip InfoZip;
	try
	{	
		SetFileAttributes(sourceFilePath,FILE_ATTRIBUTE_NORMAL);
		// ���ϵͳ·��
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

		// �����BOMӦ�ó���
		if (isAPP) {
			// �����ǳ���������Ŀ¼
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
			//����TH����
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
@brief      ��ѹ��������°�

@param      (i)CString sProgramFileUpdateDir   ѹ���ļ�·��
@param      (i)CString sFileName			   ѹ���ļ���
@param      (i)bool isAPP					   �Ƿ���BOM������

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CUpgradeHelper::ExtractUpdateFileToCurrentFolder(CString sProgramFileUpdateDir,CString sFileName,bool isAPP)
{
	return ExtractUpdateFile(sProgramFileUpdateDir+_T("\\")+sFileName,sProgramFileUpdateDir,isAPP);
}

//****************************************************************************
// 
// function : ���ĳ����Ŀ¼�������ļ��Ƿ��ܹ����á�
// input:     CString& upgradeDataFolder �����ļ���ŵ�Ŀ¼��
// output:    ��       
// return:	  true �ѹ���Чʱ�䣬Ӧ������
//			  false δ����Чʱ�䣬��Ӧ������
//****************************************************************************
bool CUpgradeHelper::CheckDownloadFileUpgradeDateHasStart(__in const ParameterAndSoftwareUpdate& ParameterAndSoftwareUpdate)
{
	_DATE currentDate(COleDateTime::GetCurrentTime());
	switch(ParameterAndSoftwareUpdate.versionType)
	{
		case CUR_VER:
		case CUR_TEST: //��ǰ�棬ֱ����Ч��
				return true; 
		case FUT_VER:
		case FUT_TEST:
				return  (currentDate > ParameterAndSoftwareUpdate.startDate) || (ParameterAndSoftwareUpdate.startDate == currentDate); //��Ч����С�ڻ��ߵ��ڽ��죬����Ч��
		default:
				return false;
	}
	return false;
}


//****************************************************************************
// 
// function : ��ô�ų��������ļ���Ŀ¼��
// input:     CVersionInfo::PROG_TYPE progType ��������
// input:    CVersionInfo::VER_TYPE& verType �����İ汾���͡�      
// return:	  CString ����·��
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
		case CVersionInfo::TH_MAIN: //TH����������BOM����һ������
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
// function : ��ô�������ļ���Ŀ¼��
// input:    PARAMETER_ID& progType ��������
// input:   PARAM_VERSION_TYPE_CODE& verCode �����İ汾���͡�      
// return:	  CString ����·��
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
// function : ��������ļ���ŵ�Ŀ¼��
// input:     CVersionInfo::PROG_TYPE progType �������� 
// output:	 vector<CString>& updateFolders �����ļ���ŵ�Ŀ¼
// return:	 ��
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
	case CVersionInfo::TH_MAIN: //TH����������BOM����һ������
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
// function : ��������ļ���ŵ�Ŀ¼��
// input:     CVersionInfo::PROG_TYPE progType �������� 
// output:	 vector<CString>& updateFolders �����ļ���ŵ�Ŀ¼
// return:	 ��
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
	case CVersionInfo::TH_MAIN: //TH����������BOM����һ������
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
// function : ��������ļ���
// input:     CVersionInfo::PROG_TYPE progType ��������
// input:    CVersionInfo::VER_TYPE& verType �����İ汾���͡� 
// output:	 vector<CString>& upgradeFiles �����ļ�
// return:	  ��
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
// function : ��VersionCodeת��ΪVersionType��
// input:	 const PARAM_VERSION_TYPE_CODE& verCode
// output:	 CVersionInfo::VER_TYPE& verType
// return:   bool �ɹ�ת��������ת�����ɹ��� 
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
// function : ��PARAMETER_IDת��ΪPROG_TYPE��
// input:	 const PARAMETER_ID& praameterID
// output:	 CVersionInfo::PROG_TYPE& progType
// return:   bool �ɹ�ת��������ת�����ɹ��� 
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
@brief      ����Ŀ¼
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