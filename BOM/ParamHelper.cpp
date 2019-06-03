#include "StdAfx.h"
#include "ParamHelper.h"
#include "ParameterDef.h"
#include "cparamhelp.h"
#include "CAccParamMaster.h"
#include "CParamException.h"
#include "CEctParamMaster.h"
#include "DebugLog.h"
#include "ExceptionLog.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CParamHelper CParamHelper::theInstance;


//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParamHelper::CParamHelper(void)
{	
//	m_rwHelper = new CRWTool();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParamHelper::~CParamHelper(void)
{	
	//delete m_rwHelper;
	//m_rwHelper = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化所有参数

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////	
bool CParamHelper::InitAllParam()
{
	bool ret = true;
	
	//bool currentVersion = true;
// 	CD_VERSION cdVersion = VERSION_FIRST;
// 	WORD wVersionNo = CD_NO_VERSION;
// 	GetCurrentCDVesion(cdVersion,wVersionNo);
// 	SetCurrentCDVesion(cdVersion);

// 	取得当前CD版本号
// 	int curVersionNo = theBOM_INFO.GetCurrentCDVersion();
// 	if(curVersionNo != 0 && (curVersionNo != wVersionNo)){
// 		// 发送电文
// 		theSERVICE_MGR.GetService(SC_SVC)->InsertTXN_EVENT_CD_VERSION_STATUS();
// 	}
// 	theBOM_INFO.SetCurrentCDVersion(wVersionNo);

	//CString dataPath = theBOM_INFO.GetDataPath() + PARAM_ACC_CURRENT_DIR;
	//CString dataPathBackup = theBOM_INFO.GetBackupPath() +PARAM_ACC_BACKUP_DIR;

	//
	//RemoveParamTransHead(dataPath);
	//RemoveParamTransHead(dataPathBackup);


	// 初始化Acc参数
	if(!InitAccParam(TRUE)){
		ret = false;
	}
	// 初始化Ect参数
	//if(!InitEctParam()){
	//	ret = false;
	//}
	// 初始化Afc参数
	if(!InitAfcParam(TRUE)){
		ret = false;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化Acc参数

@param      (i)bool flag  true:当前版本  false:未来版本 

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////	
bool CParamHelper::InitAccParam(bool flag)
{
	bool ret = true;
	try{		
		CString strMainPath = theBOM_INFO.GetDataPath();
		CString strBackupPath = theBOM_INFO.GetBackupPath();

		// 运营参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0101);
		if(!theACC_BUSINESS.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 行政处理手续费参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0102);
		if(!theACC_SERVICEFEE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}	

		// 车站位置参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0301);
		if(!theACC_LINE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 票卡属性参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0401);
		if(!theACC_TICKET.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC停运车站表
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0608);
		if(!theACC_NOTSERVSTATION.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ACC设备控制参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0201);
		if(!theACC_DEVICECONTROL.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC计费站点参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0302);
		if(!theACC_TOLLSTATION.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC区段参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0303);
		if(!theACC_SECTION.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC单条黑名单参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0701);
		if(!theACC_SINGLEBLACKLIST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ACC票卡发行商参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0501);
		if(!theACC_ISSUECOMPANY.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}


		// 区段黑名单参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0702);
		if(!theACC_SECTIONBLACKLIST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 基础票价表参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0601);
		if(!theACC_BASEPRICE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 票价级别表参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0602);
		if(!theACC_PRICELEVEL.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 票价调整策略表参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0603);
		if(!theACC_PRICEADJUEST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// 时间类型表参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0605);
		if(!theACC_TIMETYPE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 时间类型明细参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0606);
		if(!theACC_TIMEDETAIL.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// 时间浮动表参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0604);
		if(!theACC_TIMEFLOAT.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 折扣率表参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0607);
		if(!theACC_DISCOUNT.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// 白名单参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0801);
		if(!theACC_WRITELIST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// 异地卡/城市代码对照参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0802);
		if(!theACC_OTHERCITYCARD.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// 超时补交费用参数
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0103);
		if(!theACC_OVERTIMEADJUEST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		return ret;
	}
	catch (CSysException& e) {	
		ret =  false;
		throw;
		//theEXCEPTION_MGR.ProcessException(e);		
	}
	catch (...) {
		ret = false;
		throw;
		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘InitAccParam’异常")));
	}	
	return ret;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief       初始化Ect参数

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::InitEctParam()
{
	bool ret = true;
	try{
		
		// 一卡通黑名单参数类
		ChangeFutVerToCurVer(ECT_VER,ECT_PARAM_ID_4011);
		if(!theECT_BLACKLIST.Internalize()){
			ret =  false;
		}	
	}
	catch (CSysException& e) {	
		ret =  false;
		throw;		
	}
	catch (...) {
		ret =  false;
		throw;
		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘InitEctParam’异常")));
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化Afc参数

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::InitAfcParam(bool flag)
{
	bool ret = true;
	try{
		CString strMainPath = theBOM_INFO.GetDataPath();
		CString strBackupPath = theBOM_INFO.GetBackupPath();

		// 设备公共参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1005);
		if(!theAFC_COMMON.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("设备公共参数类初始化失败"));			
			ret =  false;
		}
		// 操作员参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1006);
		if(!theAFC_STAFF.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("操作员参数类初始化失败"));			
			ret =  false;
		}
		// 设备权限参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1007);
		if(!theAFC_ACCESS_LEVEL.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("设备权限参数类初始化失败"));			
			ret =  false;
		}
		// BOM运营参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1021);
		if(!theAFC_BOM_OPERATION.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("BOM运营参数类初始化失败"));			
			ret =  false;
		}	
		// 设备运行时间参数
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1089);
		if(!theAFC_DEV_TIME.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("设备运行时间参数初始化失败"));			
			ret =  false;
		}	
		// 模式履历参数
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1002);
		if(!theAFC_MODERECORD.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("模式履历参数初始化失败"));			
			ret =  false;
		}
	}
	catch (CSysException& e) {	
		ret =  false;
		throw;		
	}
	catch (...) {
		ret =  false;
		throw;
		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘InitAfcParam’异常")));
	}
	return ret;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      获取ACC当前参数版本

@param      无

@retval     无   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetCurAccParamVersion(VERSION_ACC& curAccVersion)
{
	// 运营参数
	curAccVersion.lBusinessParam = theACC_BUSINESS.GetCurMasterVer();
	// 行政处理手续费参数
	curAccVersion.lServerFeeParam = theACC_SERVICEFEE.GetCurMasterVer();
	// 车站位置参数
	curAccVersion.lStationParam = theACC_LINE.GetCurMasterVer();
	// 票卡属性参数
	curAccVersion.lCardAttributeParam = theACC_TICKET.GetCurMasterVer();
	// ACC停运车站表
	curAccVersion.lNotServStationParam = theACC_NOTSERVSTATION.GetCurMasterVer();
	// ACC设备控制参数
	curAccVersion.lDeviceControlParam = theACC_DEVICECONTROL.GetCurMasterVer();
	// ACC计费站点参数
	curAccVersion.lPayStationParam = theACC_TOLLSTATION.GetCurMasterVer();
	// ACC区段参数
	curAccVersion.lSectionParam = theACC_SECTION.GetCurMasterVer();
	// ACC单条黑名单参数
	curAccVersion.lSingleBlackListParam = theACC_SINGLEBLACKLIST.GetCurMasterVer();
	// 区段黑名单参数
	curAccVersion.lSectionBlackListParam = theACC_SECTIONBLACKLIST.GetCurMasterVer();
	// 票卡发行商参数
	curAccVersion.lissueCompanyParam = theACC_ISSUECOMPANY.GetCurMasterVer();
	// 基础票价表参数
	curAccVersion.lBasePriceParam = theACC_BASEPRICE.GetCurMasterVer();
	// 票价级别表参数
	curAccVersion.lPrieLevelParam = theACC_PRICELEVEL.GetCurMasterVer();
	// 票价调整策略表参数
	curAccVersion.lPriceAdjuestParam = theACC_PRICEADJUEST.GetCurMasterVer();
	// 时间类型表参数
	curAccVersion.lTimeTypeParam = theACC_TIMETYPE.GetCurMasterVer();
	// 时间类型明细参数
	curAccVersion.lTimeDetailParam = theACC_TIMEDETAIL.GetCurMasterVer();
	// 时间浮动表参数
	curAccVersion.lTimeFloatParam = theACC_TIMEFLOAT.GetCurMasterVer();
	// 折扣率表参数
	curAccVersion.lDiscountParam = theACC_DISCOUNT.GetCurMasterVer();
	// 白名单参数
	curAccVersion.lWriteListParam = theACC_WRITELIST.GetCurMasterVer();
	// 异地卡/城市代码对照参数
	curAccVersion.lOtherCityCardParam = theACC_OTHERCITYCARD.GetCurMasterVer();
	// 超时补交费用参数
	curAccVersion.lOverTimeAdjuestParam = theACC_OVERTIMEADJUEST.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取ACC未来参数版本

@param      无

@retval     无  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetFutAccParamVersion(VERSION_ACC& futAccVersion)
{
	// 运营参数
	futAccVersion.lBusinessParam = theACC_BUSINESS.GetFutMasterVer();
	// 行政处理手续费参数
	futAccVersion.lServerFeeParam = theACC_SERVICEFEE.GetFutMasterVer();
	// 车站位置参数
	futAccVersion.lStationParam = theACC_LINE.GetFutMasterVer();
	// 票卡属性参数
	futAccVersion.lCardAttributeParam = theACC_TICKET.GetFutMasterVer();
	// ACC停运车站表
	futAccVersion.lNotServStationParam = theACC_NOTSERVSTATION.GetFutMasterVer();
	// ACC设备控制参数
	futAccVersion.lDeviceControlParam = theACC_DEVICECONTROL.GetFutMasterVer();
	// ACC计费站点参数
	futAccVersion.lPayStationParam = theACC_TOLLSTATION.GetFutMasterVer();
	// ACC区段参数
	futAccVersion.lSectionParam = theACC_SECTION.GetFutMasterVer();
	// ACC单条黑名单参数
	futAccVersion.lSingleBlackListParam = theACC_SINGLEBLACKLIST.GetFutMasterVer();
	// 区段黑名单参数
	futAccVersion.lSectionBlackListParam = theACC_SECTIONBLACKLIST.GetFutMasterVer();
	// 票卡发行商参数
	futAccVersion.lissueCompanyParam = theACC_ISSUECOMPANY.GetFutMasterVer();
	// 基础票价表参数
	futAccVersion.lBasePriceParam = theACC_BASEPRICE.GetFutMasterVer();
	// 票价级别表参数
	futAccVersion.lPrieLevelParam = theACC_PRICELEVEL.GetFutMasterVer();
	// 票价调整策略表参数
	futAccVersion.lPriceAdjuestParam = theACC_PRICEADJUEST.GetFutMasterVer();
	// 时间类型表参数
	futAccVersion.lTimeTypeParam = theACC_TIMETYPE.GetFutMasterVer();
	// 时间类型明细参数
	futAccVersion.lTimeDetailParam = theACC_TIMEDETAIL.GetFutMasterVer();
	// 时间浮动表参数
	futAccVersion.lTimeFloatParam = theACC_TIMEFLOAT.GetFutMasterVer();
	// 折扣率表参数
	futAccVersion.lDiscountParam = theACC_DISCOUNT.GetFutMasterVer();
	// 白名单参数
	futAccVersion.lWriteListParam = theACC_WRITELIST.GetFutMasterVer();
	// 异地卡/城市代码对照参数
	futAccVersion.lOtherCityCardParam = theACC_OTHERCITYCARD.GetFutMasterVer();
	// 超时补交费用参数
	futAccVersion.lOverTimeAdjuestParam = theACC_OVERTIMEADJUEST.GetFutMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取ECT当前参数版本

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetCurEctParamVersion(VERSION_ECT& curEctVersion)
{
	
	// 一卡通黑名单参数类(充值黑名单)
	curEctVersion.lBlacklistParam = theECT_BLACKLIST.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取ECT未来参数版本

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetFutEctParamVersion(VERSION_ECT& futEctVersion)
{
	futEctVersion.lBlacklistParam = theECT_BLACKLIST.GetFutMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取AFC当前参数版本

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetCurAfcParamVersion(VERSION_AFC& curAfcVersion)
{
	// 设备权限参数类
	curAfcVersion.lAccessLevelParam = theAFC_ACCESS_LEVEL.GetCurMasterVer();
	// BOM运营参数类
	curAfcVersion.lOperationParam = theAFC_BOM_OPERATION.GetCurMasterVer();
	// 设备公共参数类
	curAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetCurMasterVer();
	// 操作员参数类
	curAfcVersion.lStaffPwdParam = theAFC_STAFF.GetCurMasterVer();
	// 设备运行时间参数
	curAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetCurMasterVer();
	// 模式履历参数
	curAfcVersion.lModeHistoryParam = theAFC_MODERECORD.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取AFC未来参数版本

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetFutAfcParamVersion(VERSION_AFC& futAfcVersion)
{
	// 设备权限参数类
	futAfcVersion.lAccessLevelParam = theAFC_ACCESS_LEVEL.GetFutMasterVer();
	// BOM运营参数类
	futAfcVersion.lOperationParam = theAFC_BOM_OPERATION.GetFutMasterVer();
	// 设备公共参数类
	futAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetFutMasterVer();
	// 操作员参数类
	futAfcVersion.lStaffPwdParam = theAFC_STAFF.GetFutMasterVer();
	// 设备运行时间参数
	futAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetFutMasterVer();
	// 模式履历参数
	futAfcVersion.lModeHistoryParam = theAFC_MODERECORD.GetFutMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      参数版本比较

@param      (i)DWORD ver1 新参数版本
@param      (i)DWORD ver2 本地参数版本

@retval     bool  true：版本不一致（需要更新） false：版本一致（不需要更新）

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::CompareVersion(DWORD ver1, DWORD ver2)
{
	// 新参数版本为0和全F忽略
	if ((ver1 != 0) && (ver1 != UNAVALIBLE_VERSION)) {
		if (ver1 != ver2) {
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新本地参数文件（将参数文件从下载目录拷贝到本地参数目录下）

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::UpdateLocalVersion(VEC_VERSION_INFO& verInfo)
{
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("UpdateLocalVersion"), __LINE__, _T("<-"));
	CFileFind finder;	
	try{
		// 查找参数更新文件
		CString sPrefix = _T("");
		CString sCurFileName = _T("");
		CString sCurFilePath = _T("");

		CString sCurFileNameBackup = _T("");
		CString sCurFilePathBackup = _T("");

		CString updateFileName = _T("");
		CString updfilePath = _T("");
		PARAM_TYPE paraType = UNKNOW;
		PARAM_VERSION_TYPE_CODE verType = VER_TYPE_UNKNOW;
		VERSION_CHANGE_INFO versionChangeInfo;
		verInfo.clear();

		CString dataPath = theBOM_INFO.GetDataPath();
		CString dataPathBackup = theBOM_INFO.GetBackupPath();

		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		CString sUpdatePath = CString(szRunPath) + BOM_PM_UPDATE_DIR;

		BOOL bContinue = finder.FindFile(sUpdatePath + _T("*.*"));		
		while (bContinue) {
			bContinue = finder.FindNextFile();

			if (finder.IsDots()||finder.IsDirectory()) {
				continue;
			}
			else {   
				// 取文件名
				updateFileName = finder.GetFileName();				
				// 检查版本类型
				verType = GetParamVersionType(updateFileName);
				paraType = GetParamType(updateFileName);					
				sPrefix = updateFileName.Left(9);

				// 当前版
				if(verType == CUR_VER || verType == CUR_TEST){		
					sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath,false);
					sCurFileNameBackup = GetCurParamName(sPrefix,paraType,sCurFilePathBackup,true);
					//	// 测试版本覆盖正式版本是备份当前正式版本
					//	//if(verType == CUR_TEST  && GetParamVersionType(sCurFileName) == CUR_VER){
					//	//	BackUpLocalCurVersion(paraType,sCurFileName,sCurFilePath);
					//	//}
				}
				// 将来版
				else if(verType == FUT_VER || verType == FUT_TEST){
					sCurFileName = GetFutParamName(sPrefix,paraType,sCurFilePath,false);
					sCurFileNameBackup = GetFutParamName(sPrefix,paraType,sCurFilePathBackup,true);
				}	
				// 未知类型
				else{
					throw CParamException(CParamException::FILE_VER_ERR, _T(__FILE__), __LINE__,updateFileName);
				}	

				// 版本不一致则更新文件
				//if(sCurFileName != updateFileName){ 						
				updfilePath = sUpdatePath + updateFileName;	
				CString sCurFilePathName = sCurFilePath + sCurFileName;
				CString sCurFilePathNameBackup = sCurFilePathBackup + sCurFileNameBackup;
				if(sCurFileName == ""){
					sCurFilePathName = "";
				}
				if(sCurFileNameBackup == ""){
					sCurFilePathNameBackup = "";
				}
				GetVersionChangeInfo(updfilePath,sCurFilePathName,versionChangeInfo);// 获取版本切换信息

				// 移动备份目录参数文件到当前参数目录
				if(DeleteFile(sCurFilePathName) != TRUE){
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("删除旧版本参数文件失败："+sCurFilePathName));	
				}
				if(DeleteFile(sCurFilePathNameBackup) != TRUE){
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("删除备份旧版本参数文件失败："+sCurFilePathNameBackup));	
				}
				if(DeleteFile(sCurFilePath + updateFileName) == TRUE){
					//theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("新版本参数文件已存在,删除:"+sCurFilePathName));						
				}
				if(DeleteFile(sCurFilePathBackup + updateFileName) == TRUE){
					//theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("备份新版本参数文件已存在,删除:"+sCurFilePathName));						
				}
				if (CopyFile(updfilePath,sCurFilePath + updateFileName,FALSE) != TRUE) {
					BOOL copyFileSuccess = FALSE;
					for(int i=0;i<2;i++)	
					{
						::Sleep(100);
						copyFileSuccess = CopyFile(updfilePath,sCurFilePath + updateFileName,FALSE);
						if(copyFileSuccess)
						{
							break;
						}
					}
					if(!copyFileSuccess)
					{
						throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("移动文件失败：") + updfilePath + _T("=>") + sCurFilePath + updateFileName);
					}	
				}					
				else{
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("MoveFile"), __LINE__,updfilePath + _T("->") + sCurFilePath + updateFileName+_T(" OK"));
					// 删除更新文件
					//if(sCurFilePathName != ""){
					if(DeleteFile(updfilePath) != TRUE){
						throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__,_T("删除更新文件失败：") + updfilePath);
					}
					//theDEBUG_LOG->WriteData("CParamHelper", "UpdateLocalVersion", __LINE__, "DeleteFile" + sCurFilePathName);
					//}
					verInfo.push_back(versionChangeInfo);
				}	
				//}
				//else{
				//	throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, _T("参数更新版本与本地版本相同：" + sCurFileName + "=" + updateFileName));	
				//}	
			}
		}
		theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("UpdateLocalVersion"), __LINE__, _T("->"));
	}
	catch(CSysException&) {
		finder.Close();
		throw;
	}
	catch (...) {
		finder.Close();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘UpdateLocalVersion’异常"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定文件夹下所有参数的传输协议头

@param      CString sParamPath  参数文件夹

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::RemoveParamTransHead(CString sParamPath)
{
	theDEBUG_LOG->WriteData(_T("ParamMgr"), _T("RemoveParamTransHead"), __LINE__, _T("<-"));
	if(sParamPath.GetLength() == 0){
		throw CParamException(CParamException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__,sParamPath);
	}
	CString strParamPath = sParamPath;
	CString strParamFileName("");
	CString strParamPathName("");
	CFileFind finder;	
	try{
		WORD subRet = 0;
		CParamHelp paramHelp;
		BOOL bContinue = finder.FindFile(strParamPath + _T("*.*"));		
		while (bContinue) {
			bContinue = finder.FindNextFile();

			if (finder.IsDots()||finder.IsDirectory()) {
				continue;
			}
			else {   
				// 取文件名
				strParamFileName = finder.GetFileName();
				strParamPathName = strParamPath + strParamFileName;
				subRet = paramHelp.DeleteTransHead(strParamPathName);
				if(subRet != 0){
					break;
				}
			}
		}
		finder.Close();
		if(subRet != 0){
			switch(subRet){
				case 1:
					throw CParamException(CParamException::FILE_SAM_ERR, _T(__FILE__), __LINE__,strParamPathName + _T("CRC校验错误"));
					break;
				case 2: 
					throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__,strParamPathName);
					break;
				case 3: 
					throw CParamException(CParamException::FILE_WRITE_FAIL,_T(__FILE__), __LINE__,strParamPathName);
					break;
				case 4: 
					throw CParamException(CParamException::FILE_SAM_ERR, _T(__FILE__), __LINE__,strParamPathName);
					break;
				default:
					throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,strParamPathName);
					break;
			}
		}
		theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("RemoveParamTransHead"), __LINE__, _T("->"));
	}
	
	catch(CSysException&) {
		finder.Close();
		throw;
	}
	catch (...) {
		finder.Close();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘RemoveParamTransHead’异常"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将指定未来版本切换至当前版本

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::ChangeFutVerToCurVer(PARAM_TYPE paraType,CString sPrefix)
{
	theDEBUG_LOG->WriteData(_T("ParamMgr"), _T("ChangeFutVerToCurVer"), __LINE__, _T("<<") + sPrefix);
	CFileFind finder;	
	try{
		if(CheckIsNeedChangeParam(sPrefix)){
			CString sCurFileName = _T("");
			CString sfutFileName = _T("");
			CString sCurFilePath = _T("");
			CString sFutFilePath = _T("");
			CString sCurFileNameNew = _T("");
			sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath,false);
			sfutFileName = GetFutParamName(sPrefix,paraType,sFutFilePath,false);

			sCurFileNameNew = ConvertFutFileNameToCurrentVersion(sfutFileName);
			if(sCurFileNameNew!=""){
				DeleteFile(sCurFilePath+sCurFileName);
				if(CopyFile(sFutFilePath+sfutFileName,sCurFilePath+sCurFileNameNew,FALSE)){
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("ChangeFutVerToCurVer"), __LINE__, sfutFileName + _T("->") + sCurFileNameNew);
					DeleteFile(sFutFilePath+sfutFileName);
				}
				else{
					throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("删除文件失败：") + sFutFilePath+sfutFileName + _T("=>") + sCurFilePath+sCurFileNameNew);	
				}
			}

			sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath,true);
			sfutFileName = GetFutParamName(sPrefix,paraType,sFutFilePath,true);

			sCurFileNameNew = ConvertFutFileNameToCurrentVersion(sfutFileName);
			if(sCurFileNameNew!=""){
				DeleteFile(sCurFilePath+sCurFileName);
				if(CopyFile(sFutFilePath+sfutFileName,sCurFilePath+sCurFileNameNew,FALSE)){
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("ChangeFutVerToCurVer"), __LINE__, sfutFileName + _T("->") + sCurFileNameNew);
					DeleteFile(sFutFilePath+sfutFileName);
				}
				else{
					throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("拷贝文件失败：") + sFutFilePath+sfutFileName +_T( "=>") + sCurFilePath+sCurFileNameNew);	
				}
			}
		}
		theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("ChangeFutVerToCurVer"), __LINE__, _T("<<") + sPrefix);
	}
	catch(CSysException&) {
		finder.Close();
		throw;
	}
	catch (...) {
		finder.Close();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘ChangeFutVerToCurVer’异常"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定参数文件

@param      (i)CString sParamPrefix 参数文件名前缀
@param      (i)BYTE bVerType		参数版本类型

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::DeleteTheParamFile(CString sParamPrefix,BYTE bVerType)
{
	CString sPrefix(_T(""));
	CString sDeleteParamFileName(_T(""));
	CString sParamPathFileName(_T(""));
	CString sParamPathFileNameBackup(_T(""));
	CString sDataPath = theBOM_INFO.GetDataPath();
	CString sBackupPath = theBOM_INFO.GetBackupPath();

	// 获得参数类型
	PARAM_TYPE paraType = GetParamType(sParamPrefix);			

	// 获得参数文件前缀
	sPrefix = sParamPrefix;
	// 当前版本参数
	if(bVerType == CUR_VER || bVerType == CUR_TEST){
		sDeleteParamFileName = thePARAM_HELPER.GetCurParamName(sPrefix,paraType,sParamPathFileName,false);
		sParamPathFileName = sParamPathFileName + sDeleteParamFileName;

		sDeleteParamFileName = thePARAM_HELPER.GetCurParamName(sPrefix,paraType,sParamPathFileNameBackup,true);
		sParamPathFileNameBackup = sParamPathFileNameBackup + sDeleteParamFileName;
		// 		switch(paraType)
		// 		{
		// 		case  ACC_VER:	//ACC
		// 			sParamPathFileName = sDataPath + PARAM_ACC_CURRENT_DIR + sParamName;
		// 			sParamPathFileNameBackup = sBackupPath + PARAM_ACC_CURRENT_DIR + sParamName;
		// 			break;
		// 		case  ECT_VER://ECT
		// 			sParamPathFileName = sDataPath + PARAM_ECT_CURRENT_DIR + sParamName;
		// 			sParamPathFileNameBackup = sBackupPath + PARAM_ECT_CURRENT_DIR + sParamName;
		// 			break;
		// 		case  AFC_VER://AFC
		// 			sParamPathFileName = sDataPath + PARAM_AFC_CURRENT_DIR + sParamName;
		// 			sParamPathFileNameBackup = sBackupPath + PARAM_AFC_CURRENT_DIR + sParamName;
		// 			break;
		// 		case  UNKNOW: //未知
		// 			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("未知参数类型："+sParamName));	
		// 			return;			
		// 		}
	}
	// 未来版本参数
	else if(bVerType == FUT_VER || bVerType == FUT_TEST){
		sDeleteParamFileName = thePARAM_HELPER.GetFutParamName(sPrefix,paraType,sParamPathFileName,false);
		sParamPathFileName = sParamPathFileName + sDeleteParamFileName;

		sDeleteParamFileName = thePARAM_HELPER.GetFutParamName(sPrefix,paraType,sParamPathFileNameBackup,true);
		sParamPathFileNameBackup = sParamPathFileNameBackup + sDeleteParamFileName;
		// 		switch(paraType)
		// 		{
		// 		case  ACC_VER:	//ACC
		// 			sParamPathFileName = sDataPath + PARAM_ACC_FUTURE_DIR + sParamName; 
		// 			sParamPathFileNameBackup = sBackupPath + PARAM_ACC_FUTURE_DIR + sParamName;
		// 			break;
		// 		case  ECT_VER://ECT
		// 			sParamPathFileName = sDataPath + PARAM_ECT_FUTURE_DIR + sParamName;
		// 			sParamPathFileNameBackup = sBackupPath + PARAM_ECT_FUTURE_DIR + sParamName;
		// 			break;
		// 		case  AFC_VER://AFC
		// 			sParamPathFileName = sDataPath + PARAM_AFC_FUTURE_DIR + sParamName;
		// 			sParamPathFileNameBackup = sBackupPath + PARAM_AFC_FUTURE_DIR + sParamName;
		// 			break;
		// 		case  UNKNOW: //未知
		// 			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("未知参数类型："+sParamName));	
		// 			return;		
		// 		}
	}
	else{
		CString sTmp(_T(""));
		sTmp.Format(_T("%X"),bVerType);
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("未知参数版本类型："+sTmp));	
		return;
	}
	// 删除主参数文件
	DeleteFile(sParamPathFileName);
	// 删除备份参数文件
	DeleteFile(sParamPathFileNameBackup);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定目录下所有文件

@param      （i）PARAMETER_ID paraID				参数ID
@param      （i）PARAM_VERSION_TYPE_CODE versionType	参数类型

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::DeleteAllFileFromTheDir(CString sDirPath)
{
	CString sFileName(_T(""));
	try{	
		SHFILEOPSTRUCT fos ;
		ZeroMemory( &fos, sizeof( fos)) ;
		fos.hwnd = NULL;
		fos.wFunc = FO_DELETE ;
		fos.fFlags = FOF_SILENT  | FOF_NOCONFIRMATION;

		TCHAR   DelFileSource[MAX_PATH];
		memset(DelFileSource, 0x00, MAX_PATH);
		_tcscat(DelFileSource, sDirPath + _T("*.*"));

		fos.pFrom = DelFileSource;

		// 删除文件夹及其内容
		SHFileOperation( &fos);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘DeleteAllFileFromTheDir’异常"));
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      备份当前正式版本（将正式版本参数文件从本地当前参数目录拷贝到备份目录下）

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::BackUpLocalCurVersion(PARAM_TYPE paraType,CString sCurFileName,CString sCurFilePath)
{
	CString sDataPath = theBOM_INFO.GetDataPath();
	CString sBackupPath = _T("");
	switch(paraType)
	{
	case  ACC_VER:	//ACC
		sBackupPath = sDataPath + PARAM_ACC_BACKUP_DIR;
		break;
	case  ECT_VER://ECT
		sBackupPath = sDataPath + PARAM_ECT_BACKUP_DIR;
		break;
	case  AFC_VER://AFC
		sBackupPath = sDataPath + PARAM_AFC_BACKUP_DIR;
		break;
	case  UNKNOW: //未知
		sBackupPath = _T("");			
	}
	CString sBackupPathFile = sBackupPath + sCurFileName;
	CString sCurfilePath = sCurFilePath + sCurFileName;

	// 移动当前版本参数文件备份目录
	if (CopyFile(sCurfilePath,sBackupPathFile,FALSE) != TRUE) { 
		BOOL copyFileSuccess = FALSE;
		for(int i=0;i<2;i++)	
		{
			::Sleep(100);
			copyFileSuccess = CopyFile(sCurfilePath,sBackupPathFile,FALSE);
			if(copyFileSuccess)
			{
				break;
			}
		}
		if(!copyFileSuccess)
		{
			throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("拷贝文件失败：") + sCurfilePath + _T("=>") + sBackupPathFile);	
		}	
		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取版本切换信息

@param      (i)CString srcParam						新参数文件
@param      (i)CString dstParam						旧参数文件
@param      (o)VERSION_CHANGE_INFO& verChangeInfo	切换信息

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void  CParamHelper::GetVersionChangeInfo(CString srcParamFile,CString dstParamFile,VERSION_CHANGE_INFO& verChangeInfo)
{
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T("<<"));
	WORD iParamOldId = 0; 
	WORD iParamNewId = 0;
	CParamHelper::VERSION_CHANGE_INFO verChgTmp;

	auto_ptr<CXFile> srcFile(new CXFile(srcParamFile,_T("")));
	VARIABLE_DATA srcbinInfo;
	// 读取文件信息到binInfo中
	if (srcFile->ReadBinFile(srcbinInfo) != CXFile::FILE_OPERATION_OK){
		throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
	}
	LPBYTE lpData = srcbinInfo.lpData;
	// 跳过信息头
	lpData += 6;
	// 参数ID
	iParamOldId = BCD2int((char*)lpData,2);
	verChgTmp.paramId  = iParamOldId;
	lpData += 2; 
	// 新版本号
	verChgTmp.newVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
	lpData +=11;  
	// 新版本生效日期
	verChgTmp.newStartDate =  BCD2Date((char*)lpData);
	lpData+=4;

	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T("Old Version:") + dstParamFile);
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T("New Version:") + srcParamFile);

	if(ComCheckFileExist(dstParamFile) == FALSE){
		verChgTmp.oldVersion = 0xFFFFFFFF;
	}
	else{
		auto_ptr<CXFile> dstFile(new CXFile(dstParamFile, _T("")));
		VARIABLE_DATA dstbinInfo;
		// 读取文件信息到binInfo中
		if (dstFile->ReadBinFile(dstbinInfo) != CXFile::FILE_OPERATION_OK){
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		lpData = dstbinInfo.lpData;
		// 跳过信息头
		lpData += 6;
		// 参数ID
		iParamNewId = BCD2int((char*)lpData,2);
		lpData += 2; 
		// 旧版本号
		verChgTmp.oldVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData +=11;  
		// 旧生效日期
		verChgTmp.oldStartDate =  BCD2Date((char*)lpData);
		lpData+=4;
	}
	// 版本类型
	verChgTmp.versionType = GetParamVersionType(srcParamFile);
	verChangeInfo = verChgTmp;
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      还原当前正式版本（将正式版本参数文件从备份目录拷贝到本地当前参数目录）

@param      (i)PARAM_TYPE paraType			参数类型
@param      (i)VEC_PARAM_ID vecParamId		还原的参数ID数组
@param      (i)bool allFlag					是否还原所有参数

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::RestoreCurVersion(PARAM_TYPE paraType,VEC_PARAM_ID vecParamId,bool allFlag)
{
	CFileFind finder;	
	try{
		CString sDataPath = theBOM_INFO.GetDataPath();
		CString sBackupFilePath = _T("");
		CString sBackUpFileName = _T("");
		CString sCurFilePath = _T("");
		CString sPrefix = _T("");
		CString sCurFileName = _T("");		
		switch(paraType)
		{
		case  ACC_VER:	//ACC
			sBackupFilePath = sDataPath + PARAM_ACC_BACKUP_DIR;
			break;
		case  ECT_VER://ECT
			sBackupFilePath = sDataPath + PARAM_ECT_BACKUP_DIR;
			break;
		case  AFC_VER://AFC
			sBackupFilePath = sDataPath + PARAM_AFC_BACKUP_DIR;
			break;
		case  UNKNOW: //未知
			sBackupFilePath = _T("");			
		}
		// 还原所有参数
		if(allFlag){						
			BOOL bContinue = finder.FindFile(sBackupFilePath + _T("*.*"));

			while (bContinue) {
				bContinue = finder.FindNextFile();

				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {   
					// 取文件名
					sBackUpFileName = finder.GetFileName();	
					sPrefix = sBackUpFileName.Left(9);
					sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath);

					// 版本不一致则更新文件
					if(sCurFileName != sBackUpFileName){ 					
						// 移动备份目录参数文件到当前参数目录
						if (MoveFileEx(sBackupFilePath + sBackUpFileName,sCurFilePath + sBackUpFileName,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING) != TRUE) { 
							throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("移动文件失败：" + sBackupFilePath + sBackUpFileName + _T("=>") + sCurFilePath + sBackUpFileName));	
						}					
						else{
							// 删除当前版本参数文件						
							if(DeleteFile(sCurFilePath + sCurFileName) != TRUE){
								throw CParamException(CParamException::FILE_DEL_FAIL,_T(__FILE__), __LINE__,_T("删除当前版本参数文件失败：") + sCurFilePath + sCurFileName);
							}
						}
					}
					else{
						throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__, _T("参数备份版本与本地版本相同：") + sCurFileName + _T("=") + sBackUpFileName);	
					}	
				}
			}
		}
		// 还原指定参数
		else{

		}
	}
	catch(CSysException&) {
		finder.Close();
		throw;
	}
	catch (...) {
		finder.Close();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘RestoreCurVersion’异常"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得数据目录下指定子目录下指定参数文件名

@param      （i）PARAMETER_ID paraID				参数ID
@param      （i）PARAM_VERSION_TYPE_CODE versionType	参数类型

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetTheParamPathName(WORD paraID,CString ParamPath)
{
	return GetTheParamPathNameImp(paraID,ParamPath,theBOM_INFO.GetDataPath());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得备份目录指定子目录下指定参数文件名

@param      （i）PARAMETER_ID paraID				参数ID
@param      （i）PARAM_VERSION_TYPE_CODE versionType	参数类型

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetTheParamBackUpPathName(WORD paraID,CString ParamPath)
{
	return GetTheParamPathNameImp(paraID,ParamPath,theBOM_INFO.GetBackupPath());
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      取得指定目录下指定参数文件名

@param      （i）PARAMETER_ID paraID				参数ID
@param      （i）PARAM_VERSION_TYPE_CODE versionType	参数类型
@param      （i）basePath	基地址
@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetTheParamPathNameImp(WORD paraID,CString ParamPath,CString basePath)
{
	CString retName = _T("");
	CFileFind finder;
	try{
		// 查找参数文件

		CString sPath = basePath;
		CString sFilePath = _T("");

		if(ParamPath.GetLength() == 0){
			return _T("");
		}
		sFilePath = sPath + ParamPath;

		BYTE paraIDtmp[2] = {0x00,0x00};
		paraIDtmp[0] = HIBYTE(paraID);
		paraIDtmp[1] = LOBYTE(paraID);

		WORD tmp = BCD2int((char*)paraIDtmp,2);
		CString sParaID = _T("");
		sParaID.Format(_T("%.4d"), tmp);
		CString prefix = PARAM_PREFIX + sParaID;

		BOOL bContinue = finder.FindFile(sFilePath + prefix + _T("*.*"));

		while (bContinue) {
			bContinue = finder.FindNextFile();

			if (finder.IsDots()||finder.IsDirectory()) {
				continue;
			}
			else {   
				// 取出文件名，用于比较日期
				retName = finder.GetFileName(); 						
			}
		}	
		retName = sFilePath + retName;	
	}
	
	catch(CSysException&) {
		finder.Close();
		throw;
	}
	catch (...) {
		finder.Close();
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘GetTheParamPathNameImp’异常"));
	}
	finder.Close();
	return retName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得当前目录下指定参数文件名

@param      （i）CString sPrefix  参数文件前缀
@param      （i）PARAM_TYPE paraType 参数类型
@param      （o）CString sFilePath 参数路径

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetCurParamName(CString sPrefix,PARAM_TYPE paraType,CString& sFilePath,bool isBackup)
{
	// 查找参数文件
	CString retName = _T("");	
	CString sPath = _T("");
	if(!isBackup){
		sPath = theBOM_INFO.GetDataPath();
	}
	else{
		sPath = theBOM_INFO.GetBackupPath();
	}

	switch(paraType)
	{
	case  ACC_VER:	//ACC
		sFilePath = sPath + PARAM_ACC_CURRENT_DIR;
		break;
	case  ECT_VER://ECT
		sFilePath = sPath + PARAM_ECT_CURRENT_DIR;
		break;
	case  AFC_VER://AFC
		sFilePath = sPath + PARAM_AFC_CURRENT_DIR;
		break;
	case  UNKNOW: //未知
		sFilePath = _T("");
		return retName;	
	}
	CFileFind finder;			
	BOOL bContinue = finder.FindFile(sFilePath + sPrefix + _T("*.*"));

	while (bContinue) {
		bContinue = finder.FindNextFile();

		if (finder.IsDots()||finder.IsDirectory()) {
			continue;
		}
		else {   
			// 取出文件名
			retName = finder.GetFileName(); 						
		}
	}
	finder.Close();
	return retName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得当前目录下指定参数文件名

@param      （i）CString sPrefix  参数文件前缀

@retval     CString 参数路径及文件名

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetFutParamName(CString sPrefix,PARAM_TYPE paraType,CString& sFilePath,bool isBackup)
{
	// 查找参数文件
	CString retName = _T("");	
	CString sPath = _T("");
	if(!isBackup){
		sPath = theBOM_INFO.GetDataPath();
	}
	else{
		sPath = theBOM_INFO.GetBackupPath();
	}
	switch(paraType)
	{
	case  ACC_VER:	//ACC
		sFilePath = sPath + PARAM_ACC_FUTURE_DIR;
		break;
	case  ECT_VER://ECT
		sFilePath = sPath + PARAM_ECT_FUTURE_DIR;
		break;
	case  AFC_VER://AFC
		sFilePath = sPath + PARAM_AFC_FUTURE_DIR;
		break;
	case  UNKNOW: //未知
		sFilePath = _T("");
		return retName;	
	}
	CFileFind finder;			
	BOOL bContinue = finder.FindFile(sFilePath + sPrefix + _T("*.*"));

	while (bContinue) {
		bContinue = finder.FindNextFile();

		if (finder.IsDots()||finder.IsDirectory()) {
			continue;
		}
		else {   
			// 取出文件名
			retName = finder.GetFileName(); 
		}
	}
	finder.Close();
	return retName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数类型

@param      （i）CString fileName  参数文件名

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CParamHelper::PARAM_TYPE CParamHelper::GetParamType(CString fileName)
{
	PARAM_TYPE retVal = UNKNOW;

	CString sParaID = fileName.Mid(5,4);
	//WORD iParaID ;
	//BYTE buffer[10];
	//sscanf_s(sParaID,"%x",buffer,2);
	//WORD iParaID = MAKEWORD(buffer[0],buffer[1]);
	WORD iParaID = _tcstol(sParaID,NULL,16);
	WORD afcParameterIDs[] = {
		AFC_MODEHISTORY_ID , 
		AFC_DEVICECOMMONPARAM_ID, 
		AFC_STAFFPWDPARAM_ID, 
		AFC_ACCESSLEVELPARAM_ID, 
		AFC_BOMOPERATIONPARAM_ID, 
		AFC_DEVICERUNTIMEPARAM_ID, 
	};
	WORD accParameterIDs[] = 
	{
		ACC_BUSINESS_ID						,					// ACC运营参数
		ACC_SERVICEFEE_ID					,					// ACC行政处理手续费参数
		ACC_OVERTIME_UPDATE_ID				,					// ACC超时补交费用参数
		ACC_DEVICE_ID						,					// ACC设备控制参数
		ACC_STATION_ID						,					// ACC车站位置参数
		ACC_CHARGE_STATION_ID				,					// ACC计费站点参数
		ACC_SECTION_ID						,					// ACC区段参数
		ACC_CARDATTRIBUTE_ID				,					// ACC票卡属性参数
		ACC_BASE_PRICE_TABLE_ID				,					// ACC基础票价表参数
		ACC_PRICE_LEVEL_TABLE_ID			,					// ACC票价级别表参数
		ACC_PRICE_ADJUEST_TABLE_ID			,					// ACC票价调整策略表参数
		ACC_TIME_FLOATING_TABLE_ID			,					// ACC时间浮动表参数
		ACC_TIME_TYPE_TABLE_ID				,					// ACC时间类型表参数
		ACC_TTIME_TYPE_DETAIL_ID			,					// ACC时间类型明细参数
		ACC_DISSCOUNT_TABLE_ID				,					// ACC折扣率表参数
		ACC_NOT_SERVICE_STATION_ID			,					// ACC停运车站表
		ACC_WRITE_LIST_ID					,					// ACC白名单参数
		ACC_OTHER_CITY_CARD_ID				,					// ACC异地卡/城市代码对照参数
		ACC_SINGLE_BLACK_LIST_ID			,					// ACC单条黑名单参数
		ACC_SECTION_BLACK_LIST_ID			,					// ACC区段黑名单参数
		ACC_ISSUE_COMPANY_ID	    		,					// ACC票卡发行商参数
	};
	/*WORD ectParameterIDs[] = 
	{
		ECT_SHOPBLACKLIST_ID,
	};*/
	if(CValidation::IsInRange(iParaID,afcParameterIDs))
	{
		retVal = AFC_VER;
	} 
	else if(CValidation::IsInRange(iParaID,accParameterIDs))
	{
		retVal = ACC_VER;
	}
	/*else if(CValidation::IsInRange(iParaID,ectParameterIDs)){
		retVal = ECT_VER;
	}*/
	else{
		retVal = UNKNOW;
	}
	return retVal;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件版本类型

@param      （i）CString fileName  参数文件名

@retval     PARAM_VERSION_TYPE_CODE 版本类型

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
PARAM_VERSION_TYPE_CODE CParamHelper::GetParamVersionType(CString fileName)
{
	PARAM_VERSION_TYPE_CODE retVal = VER_TYPE_UNKNOW;

	CString sVerType = fileName.Right(2);	

	if(sVerType == _T("00")){
		retVal = CUR_VER;
	}
	else if(sVerType == _T("01")){
		retVal = FUT_VER;
	}
	else if(sVerType == _T("10")){
		retVal = CUR_TEST;
	}
	else if(sVerType == _T("11")){
		retVal = FUT_TEST;
	}
	else{
		retVal = VER_TYPE_UNKNOW;
	}
	return retVal;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取程序文件版本类型

@param      （i）CString fileName  程序文件名

@retval     PARAM_VERSION_TYPE_CODE 版本类型

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
PARAM_VERSION_TYPE_CODE CParamHelper::GetProgVersionType(CString fileName)
{
	PARAM_VERSION_TYPE_CODE retVal = VER_TYPE_UNKNOW;

	CString sVerType = fileName.Right(2);	

	if(sVerType == _T("00")){
		retVal = CUR_VER;
	}
	else if(sVerType == _T("01")){
		retVal = FUT_VER;
	}
	else if(sVerType == _T("10")){
		retVal = CUR_TEST;
	}
	else if(sVerType == _T("11")){
		retVal = FUT_TEST;
	}
	else{
		retVal = VER_TYPE_UNKNOW;
	}
	return retVal;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数版本

@param      (i)CString sPathAndName     参数文件名

@retval     CString              参数文件名

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetParamVerFromParamName(CString sParamName)
{
	CString strRet("");
	int nPos = sParamName.ReverseFind('.');
	if (nPos != -1)	{
		strRet = sParamName.Mid(0,nPos);
	}
	else{
		strRet = sParamName;
	}
	nPos = strRet.ReverseFind('.');	
	if (nPos != -1)	{
		strRet = strRet.Mid(nPos+1);
	}
	else{
		strRet = sParamName;
	}
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取程序版本号

@param      (i)CString sPathAndName     程序文件名

@retval     CString              程序版本号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetProgVerFromProgName(CString sProgName)
{
	CString strRet("");
	int nPos = sProgName.ReverseFind('.');
	if (nPos != -1)	{
		strRet = sProgName.Mid(0,nPos);
	}
	else{
		strRet = sProgName;
	}
	nPos = strRet.ReverseFind('.');	
	if (nPos != -1)	{
		strRet = strRet.Mid(0,nPos);
	}
	else{
		strRet = sProgName;
	}
	nPos = strRet.ReverseFind('.');	
	if (nPos != -1)	{
		strRet = strRet.Mid(nPos+1);
	}
	else{
		strRet = sProgName;
	}
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取程序批次号

@param      (i)CString sPathAndName     程序文件名

@retval     CString       批次号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetProgBatchNoFromProgName(CString sProgName)
{
	CString strRet("");
	int nPos = sProgName.ReverseFind('.');
	if (nPos != -1)	{
		strRet = sProgName.Mid(0,nPos);
	}
	else{
		strRet = sProgName;
	}
	nPos = strRet.ReverseFind('.');	
	if (nPos != -1)	{
		strRet = strRet.Mid(nPos+1);
	}
	else{
		strRet = sProgName;
	}
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
///**
//@brief      根据version信息设置当前CD版本并返回当前版本
//
//@param      (o)CD_VERSION &cdVersion  当前版本
//@param      (o)WORD wVersionNo        CD版本号
//
//@retval     无
//
//@exception  CSysException
//*/
//////////////////////////////////////////////////////////////////////////
//void CParamHelper::GetCurrentCDVesion(CD_VERSION &cdVersion,WORD &wVersionNo)
//{
//	try {
//		bool isCD1Update = false;
//		bool isCD2Update = false;
//		_DATE dateOne;
//		_DATE dateTwo;
//
//		theACC_VERSION.Internalize(true);
//		CAccVersionInfoParam::CD_INFO_PARAM cd1Info;
//		theACC_VERSION.GetCDInfoParam(cd1Info);						// 获得CD1信息参数
//		// 判断版本1的信息，是否为0xFFFFFFFF
//		if (CD_NO_VERSION != cd1Info.cdSetVersion){
//			dateOne = cd1Info.cdActivationDate;           
//			// 判断日期1是否有效且小于当前时间
//			if ((0 == ComCheckDateValid(dateOne)) && (1 != CmpTwoDate(dateOne ,ComGetBusiDate()))) {
//				isCD1Update = true;
//			}
//		}
//
//		theACC_VERSION.Internalize(false);
//		CAccVersionInfoParam::CD_INFO_PARAM cd2Info;
//		theACC_VERSION.GetCDInfoParam(cd2Info);						// 获得CD2信息参数		
//		// 判断版本2的信息，是否为0xFFFFFFFF
//		if (CD_NO_VERSION != cd2Info.cdSetVersion){
//			dateTwo = cd2Info.cdActivationDate; 
//			// 判断日期2是否有效且小于当前时间
//			if ((0 == ComCheckDateValid(dateTwo)) && (1 != CmpTwoDate(dateTwo ,ComGetBusiDate()))) {
//				isCD2Update = true;
//			}
//		}
//		// 如果版本1和版本2版本都是0xFFFFFFFF，版本1生效
//		if (CD_NO_VERSION == cd1Info.cdSetVersion && CD_NO_VERSION == cd2Info.cdSetVersion){			
//			isCD1Update = true;
//		}
//
//		// 生效版本信息
//		if (isCD1Update && !isCD2Update) {
//			// 版本1生效
//			cdVersion = VERSION_FIRST;
//		}
//		else if (!isCD1Update && isCD2Update) {
//			// 版本2生效
//			cdVersion = VERSION_SECOND;
//		}
//		else if (!isCD1Update && !isCD2Update) {
//			// CD参数错误，抛异常，程序中断
//			throw CCPSException(CCPSException::REFRESH_VERSION_ERROR, _T(__FILE__), __LINE__, "CD version 参数错误");
//		}
//		else {
//			// 版本1和版本2同时生效
//			if ( 1 == CmpTwoDate(dateOne ,dateTwo)) {
//				// 版本1生效
//				cdVersion = VERSION_FIRST;
//			}
//			else if (-1 == CmpTwoDate(dateOne ,dateTwo)) {
//				// 版本2生效
//				cdVersion = VERSION_SECOND;
//			}
//			else {
//				// 版本1和版本2的生效日期相同，生效CD版本大的
//				DWORD ver1 = cd1Info.cdSetVersion;
//				DWORD ver2 = cd2Info.cdSetVersion;;
//				if(ver1 > ver2) {
//					cdVersion = VERSION_FIRST;
//				}
//				else if(ver1 < ver2) {
//					cdVersion = VERSION_SECOND;
//				}
//				else {
//					// CD版本相同，生效generationId大的
//					DWORD ger1 = cd1Info.generationId;
//					DWORD ger2 = cd2Info.generationId;
//					if (ger1 >= ger2) {
//						// 版本1大于或者等于版本2，都生效版本1
//						cdVersion = VERSION_FIRST;
//					}
//					else {
//						cdVersion = VERSION_SECOND;
//					}
//				}
//			}
//		}
//		wVersionNo = (cdVersion == VERSION_FIRST)?cd1Info.cdSetVersion:cd2Info.cdSetVersion;
//	}
//	catch (CSysException& e) {			
//		theEXCEPTION_MGR.ProcessException(e);	
//	}
//	catch (...) {
//		throw;
//	}
//}
//
////////////////////////////////////////////////////////////////////////
/**
@brief      获取ACC版本切换信息

@param      (i)VEC_VERSION_INFO& vecChangeInfo  版本信息

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CParamHelper::GetVesionChangeInfo(VEC_VERSION_INFO& vecChangeInfo)
{
	VERSION_CHANGE_INFO verInfo;
	vecChangeInfo.clear();

	// 系统参数
	//verInfo.paramId = ACC_SYSTEMPARAM_ID;
	//verInfo.versionType = theACC_SYSTEM.GetCurVerType();
	//verInfo.oldVersion = theACC_SYSTEM.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SYSTEM.GetCurStartDate();
	//verInfo.newVersion = theACC_SYSTEM.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SYSTEM.GetFutStartDate();	
	//vecChangeInfo.push_back(verInfo);

	// 运营参数
	verInfo.paramId = ACC_BUSINESS_ID;
	verInfo.versionType = theACC_BUSINESS.GetCurVerType();
	verInfo.oldVersion = theACC_BUSINESS.GetCurMasterVer();
	verInfo.oldStartDate = theACC_BUSINESS.GetCurStartDate();
	verInfo.newVersion = theACC_BUSINESS.GetFutMasterVer();
	verInfo.newStartDate = theACC_BUSINESS.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 行政处理手续费参数
	verInfo.paramId = ACC_SERVICEFEE_ID;
	verInfo.versionType = theACC_SERVICEFEE.GetCurVerType();
	verInfo.oldVersion = theACC_SERVICEFEE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SERVICEFEE.GetCurStartDate();
	verInfo.newVersion = theACC_SERVICEFEE.GetFutMasterVer();
	verInfo.newStartDate = theACC_SERVICEFEE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 车站位置参数
	verInfo.paramId = ACC_STATION_ID;
	verInfo.versionType = theACC_LINE.GetCurVerType();
	verInfo.oldVersion = theACC_LINE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_LINE.GetCurStartDate();
	verInfo.newVersion = theACC_LINE.GetFutMasterVer();
	verInfo.newStartDate = theACC_LINE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 票卡属性参数
	verInfo.paramId = ACC_CARDATTRIBUTE_ID;
	verInfo.versionType = theACC_TICKET.GetCurVerType();
	verInfo.oldVersion = theACC_TICKET.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TICKET.GetCurStartDate();
	verInfo.newVersion = theACC_TICKET.GetFutMasterVer();
	verInfo.newStartDate = theACC_TICKET.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC停运车站表
	verInfo.paramId = ACC_NOT_SERVICE_STATION_ID;
	verInfo.versionType = theACC_NOTSERVSTATION.GetCurVerType();
	verInfo.oldVersion = theACC_NOTSERVSTATION.GetCurMasterVer();
	verInfo.oldStartDate = theACC_NOTSERVSTATION.GetCurStartDate();
	verInfo.newVersion = theACC_NOTSERVSTATION.GetFutMasterVer();
	verInfo.newStartDate = theACC_NOTSERVSTATION.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC设备控制参数
	verInfo.paramId = ACC_DEVICE_ID;
	verInfo.versionType = theACC_DEVICECONTROL.GetCurVerType();
	verInfo.oldVersion = theACC_DEVICECONTROL.GetCurMasterVer();
	verInfo.oldStartDate = theACC_DEVICECONTROL.GetCurStartDate();
	verInfo.newVersion = theACC_DEVICECONTROL.GetFutMasterVer();
	verInfo.newStartDate = theACC_DEVICECONTROL.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC计费站点参数
	verInfo.paramId = ACC_CHARGE_STATION_ID;
	verInfo.versionType = theACC_TOLLSTATION.GetCurVerType();
	verInfo.oldVersion = theACC_TOLLSTATION.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TOLLSTATION.GetCurStartDate();
	verInfo.newVersion = theACC_TOLLSTATION.GetFutMasterVer();
	verInfo.newStartDate = theACC_TOLLSTATION.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC区段参数
	verInfo.paramId = ACC_SECTION_ID;
	verInfo.versionType = theACC_SECTION.GetCurVerType();
	verInfo.oldVersion = theACC_SECTION.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SECTION.GetCurStartDate();
	verInfo.newVersion = theACC_SECTION.GetFutMasterVer();
	verInfo.newStartDate = theACC_SECTION.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC单条黑名单参数
	verInfo.paramId = ACC_SINGLE_BLACK_LIST_ID;
	verInfo.versionType = theACC_SINGLEBLACKLIST.GetCurVerType();
	verInfo.oldVersion = theACC_SINGLEBLACKLIST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SINGLEBLACKLIST.GetCurStartDate();
	verInfo.newVersion = theACC_SINGLEBLACKLIST.GetFutMasterVer();
	verInfo.newStartDate = theACC_SINGLEBLACKLIST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 区段黑名单参数
	verInfo.paramId = ACC_SECTION_BLACK_LIST_ID;
	verInfo.versionType = theACC_SECTIONBLACKLIST.GetCurVerType();
	verInfo.oldVersion = theACC_SECTIONBLACKLIST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SECTIONBLACKLIST.GetCurStartDate();
	verInfo.newVersion = theACC_SECTIONBLACKLIST.GetFutMasterVer();
	verInfo.newStartDate = theACC_SECTIONBLACKLIST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 区段黑名单参数
	verInfo.paramId = ACC_ISSUE_COMPANY_ID;
	verInfo.versionType = theACC_ISSUECOMPANY.GetCurVerType();
	verInfo.oldVersion = theACC_ISSUECOMPANY.GetCurMasterVer();
	verInfo.oldStartDate = theACC_ISSUECOMPANY.GetCurStartDate();
	verInfo.newVersion = theACC_ISSUECOMPANY.GetFutMasterVer();
	verInfo.newStartDate = theACC_ISSUECOMPANY.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 基础票价表参数
	verInfo.paramId = ACC_BASE_PRICE_TABLE_ID;
	verInfo.versionType = theACC_BASEPRICE.GetCurVerType();
	verInfo.oldVersion = theACC_BASEPRICE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_BASEPRICE.GetCurStartDate();
	verInfo.newVersion = theACC_BASEPRICE.GetFutMasterVer();
	verInfo.newStartDate = theACC_BASEPRICE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 票价级别表参数
	verInfo.paramId = ACC_PRICE_LEVEL_TABLE_ID;
	verInfo.versionType = theACC_PRICELEVEL.GetCurVerType();
	verInfo.oldVersion = theACC_PRICELEVEL.GetCurMasterVer();
	verInfo.oldStartDate = theACC_PRICELEVEL.GetCurStartDate();
	verInfo.newVersion = theACC_PRICELEVEL.GetFutMasterVer();
	verInfo.newStartDate = theACC_PRICELEVEL.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 票价调整策略表参数
	verInfo.paramId = ACC_PRICE_ADJUEST_TABLE_ID;
	verInfo.versionType = theACC_PRICEADJUEST.GetCurVerType();
	verInfo.oldVersion = theACC_PRICEADJUEST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_PRICEADJUEST.GetCurStartDate();
	verInfo.newVersion = theACC_PRICEADJUEST.GetFutMasterVer();
	verInfo.newStartDate = theACC_PRICEADJUEST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 时间类型表参数
	verInfo.paramId = ACC_TIME_TYPE_TABLE_ID;
	verInfo.versionType = theACC_TIMETYPE.GetCurVerType();
	verInfo.oldVersion = theACC_TIMETYPE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TIMETYPE.GetCurStartDate();
	verInfo.newVersion = theACC_TIMETYPE.GetFutMasterVer();
	verInfo.newStartDate = theACC_TIMETYPE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 时间类型明细参数
	verInfo.paramId = ACC_TTIME_TYPE_DETAIL_ID;
	verInfo.versionType = theACC_TIMEDETAIL.GetCurVerType();
	verInfo.oldVersion = theACC_TIMEDETAIL.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TIMEDETAIL.GetCurStartDate();
	verInfo.newVersion = theACC_TIMEDETAIL.GetFutMasterVer();
	verInfo.newStartDate = theACC_TIMEDETAIL.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 时间浮动表参数
	verInfo.paramId = ACC_TIME_FLOATING_TABLE_ID;
	verInfo.versionType = theACC_TIMEFLOAT.GetCurVerType();
	verInfo.oldVersion = theACC_TIMEFLOAT.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TIMEFLOAT.GetCurStartDate();
	verInfo.newVersion = theACC_TIMEFLOAT.GetFutMasterVer();
	verInfo.newStartDate = theACC_TIMEFLOAT.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 折扣率表参数
	verInfo.paramId = ACC_DISSCOUNT_TABLE_ID;
	verInfo.versionType = theACC_DISCOUNT.GetCurVerType();
	verInfo.oldVersion = theACC_DISCOUNT.GetCurMasterVer();
	verInfo.oldStartDate = theACC_DISCOUNT.GetCurStartDate();
	verInfo.newVersion = theACC_DISCOUNT.GetFutMasterVer();
	verInfo.newStartDate = theACC_DISCOUNT.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 白名单参数
	verInfo.paramId = ACC_WRITE_LIST_ID;
	verInfo.versionType = theACC_WRITELIST.GetCurVerType();
	verInfo.oldVersion = theACC_WRITELIST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_WRITELIST.GetCurStartDate();
	verInfo.newVersion = theACC_WRITELIST.GetFutMasterVer();
	verInfo.newStartDate = theACC_WRITELIST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 异地卡/城市代码对照参数
	verInfo.paramId = ACC_OTHER_CITY_CARD_ID;
	verInfo.versionType = theACC_OTHERCITYCARD.GetCurVerType();
	verInfo.oldVersion = theACC_OTHERCITYCARD.GetCurMasterVer();
	verInfo.oldStartDate = theACC_OTHERCITYCARD.GetCurStartDate();
	verInfo.newVersion = theACC_OTHERCITYCARD.GetFutMasterVer();
	verInfo.newStartDate = theACC_OTHERCITYCARD.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// 超时补交费用参数
	verInfo.paramId = ACC_OVERTIME_UPDATE_ID;
	verInfo.versionType = theACC_OVERTIMEADJUEST.GetCurVerType();
	verInfo.oldVersion = theACC_OVERTIMEADJUEST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_OVERTIMEADJUEST.GetCurStartDate();
	verInfo.newVersion = theACC_OVERTIMEADJUEST.GetFutMasterVer();
	verInfo.newStartDate = theACC_OVERTIMEADJUEST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断是否需要切换至将来版本

@param      CString sParamPreFix 参数文件名前缀

@retval     bool  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::CheckIsNeedChangeParam(CString sParamPreFix)
{
	try{
		theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T("<<"));
		bool ret = true;
		DWORD curVersion = 0;
		DWORD futVersion = 0;
		_DATE curStartDate;
		_DATE futStartDate;
		CString strMainPath = theBOM_INFO.GetDataPath();
		CString strBackupPath = theBOM_INFO.GetBackupPath();

		// 设备公共参数类
		if(sParamPreFix == AFC_PARAM_ID_1005){
			if(theAFC_COMMON.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_COMMON.GetCurMasterVer();
				futVersion = theAFC_COMMON.GetFutMasterVer();
				futStartDate = theAFC_COMMON.GetFutStartDate();
				curStartDate = theAFC_COMMON.GetCurStartDate();
			}
		}
		// 操作员参数类
		else if(sParamPreFix == AFC_PARAM_ID_1006){
			if(theAFC_STAFF.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_STAFF.GetCurMasterVer();
				futVersion = theAFC_STAFF.GetFutMasterVer();
				futStartDate = theAFC_STAFF.GetFutStartDate();
				curStartDate = theAFC_STAFF.GetCurStartDate();
			}
		}
		// 设备权限参数类
		else if(sParamPreFix == AFC_PARAM_ID_1007){
			if(theAFC_ACCESS_LEVEL.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_ACCESS_LEVEL.GetCurMasterVer();
				futVersion = theAFC_ACCESS_LEVEL.GetFutMasterVer();
				futStartDate = theAFC_ACCESS_LEVEL.GetFutStartDate();
				curStartDate = theAFC_ACCESS_LEVEL.GetCurStartDate();
			}
		}
		// TVM运营参数类
		else if(sParamPreFix == AFC_PARAM_ID_1021){
			if(theAFC_BOM_OPERATION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_BOM_OPERATION.GetCurMasterVer();
				futVersion = theAFC_BOM_OPERATION.GetFutMasterVer();
				futStartDate = theAFC_BOM_OPERATION.GetFutStartDate();
				curStartDate = theAFC_BOM_OPERATION.GetCurStartDate();
			}
		}
		// 设备运行时间参数
		else if(sParamPreFix == AFC_PARAM_ID_1089){
			if(theAFC_DEV_TIME.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_DEV_TIME.GetCurMasterVer();
				futVersion = theAFC_DEV_TIME.GetFutMasterVer();
				futStartDate = theAFC_DEV_TIME.GetFutStartDate();
				curStartDate = theAFC_DEV_TIME.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0101){
			if(theACC_BUSINESS.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_BUSINESS.GetCurMasterVer();
				futVersion = theACC_BUSINESS.GetFutMasterVer();
				futStartDate = theACC_BUSINESS.GetFutStartDate();
				curStartDate = theACC_BUSINESS.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0102){
			if(theACC_SERVICEFEE.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_SERVICEFEE.GetCurMasterVer();
				futVersion = theACC_SERVICEFEE.GetFutMasterVer();
				futStartDate = theACC_SERVICEFEE.GetFutStartDate();
				curStartDate = theACC_SERVICEFEE.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0103){
			if(theACC_OVERTIMEADJUEST.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_OVERTIMEADJUEST.GetCurMasterVer();
				futVersion = theACC_OVERTIMEADJUEST.GetFutMasterVer();
				futStartDate = theACC_OVERTIMEADJUEST.GetFutStartDate();
				curStartDate = theACC_OVERTIMEADJUEST.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0201){
			if(theACC_DEVICECONTROL.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_DEVICECONTROL.GetCurMasterVer();
				futVersion = theACC_DEVICECONTROL.GetFutMasterVer();
				futStartDate = theACC_DEVICECONTROL.GetFutStartDate();
				curStartDate = theACC_DEVICECONTROL.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0301){
			if(theACC_LOCATION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_LOCATION.GetCurMasterVer();
				futVersion = theACC_LOCATION.GetFutMasterVer();
				futStartDate = theACC_LOCATION.GetFutStartDate();
				curStartDate = theACC_LOCATION.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0302){
			if(theACC_TOLLSTATION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_TOLLSTATION.GetCurMasterVer();
				futVersion = theACC_TOLLSTATION.GetFutMasterVer();
				futStartDate = theACC_TOLLSTATION.GetFutStartDate();
				curStartDate = theACC_TOLLSTATION.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0303){
			if(theACC_SECTION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_SECTION.GetCurMasterVer();
				futVersion = theACC_SECTION.GetFutMasterVer();
				futStartDate = theACC_SECTION.GetFutStartDate();
				curStartDate = theACC_SECTION.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0401){
			if(theACC_PRODUCT.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_PRODUCT.GetCurMasterVer();
				futVersion = theACC_PRODUCT.GetFutMasterVer();
				futStartDate = theACC_PRODUCT.GetFutStartDate();
				curStartDate = theACC_PRODUCT.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0501){
			if(theACC_ISSUECOMPANY.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_ISSUECOMPANY.GetCurMasterVer();
				futVersion = theACC_ISSUECOMPANY.GetFutMasterVer();
				futStartDate = theACC_ISSUECOMPANY.GetFutStartDate();
				curStartDate = theACC_ISSUECOMPANY.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0601){
			if(theACC_BASEPRICE.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_BASEPRICE.GetCurMasterVer();
				futVersion = theACC_BASEPRICE.GetFutMasterVer();
				futStartDate = theACC_BASEPRICE.GetFutStartDate();
				curStartDate = theACC_BASEPRICE.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0602){
			if(theACC_PRICELEVEL.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_PRICELEVEL.GetCurMasterVer();
				futVersion = theACC_PRICELEVEL.GetFutMasterVer();
				futStartDate = theACC_PRICELEVEL.GetFutStartDate();
				curStartDate = theACC_PRICELEVEL.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0603){
			if(theACC_PRICEADJUEST.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_PRICEADJUEST.GetCurMasterVer();
				futVersion = theACC_PRICEADJUEST.GetFutMasterVer();
				futStartDate = theACC_PRICEADJUEST.GetFutStartDate();
				curStartDate = theACC_PRICEADJUEST.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0604){
			if(theACC_TIMEFLOAT.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_TIMEFLOAT.GetCurMasterVer();
				futVersion = theACC_TIMEFLOAT.GetFutMasterVer();
				futStartDate = theACC_TIMEFLOAT.GetFutStartDate();
				curStartDate = theACC_TIMEFLOAT.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0605){
			if(theACC_TIMETYPE.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_TIMETYPE.GetCurMasterVer();
				futVersion = theACC_TIMETYPE.GetFutMasterVer();
				futStartDate = theACC_TIMETYPE.GetFutStartDate();
				curStartDate = theACC_TIMETYPE.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0606){
			if(theACC_TIMEDETAIL.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_TIMEDETAIL.GetCurMasterVer();
				futVersion = theACC_TIMEDETAIL.GetFutMasterVer();
				futStartDate = theACC_TIMEDETAIL.GetFutStartDate();
				curStartDate = theACC_TIMEDETAIL.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0607){
			if(theACC_DISCOUNT.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_DISCOUNT.GetCurMasterVer();
				futVersion = theACC_DISCOUNT.GetFutMasterVer();
				futStartDate = theACC_DISCOUNT.GetFutStartDate();
				curStartDate = theACC_DISCOUNT.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0701){
			if(theACC_SINGLEBLACKLIST.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_SINGLEBLACKLIST.GetCurMasterVer();
				futVersion = theACC_SINGLEBLACKLIST.GetFutMasterVer();
				futStartDate = theACC_SINGLEBLACKLIST.GetFutStartDate();
				curStartDate = theACC_SINGLEBLACKLIST.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0702){
			if(theACC_SECTIONBLACKLIST.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_SECTIONBLACKLIST.GetCurMasterVer();
				futVersion = theACC_SECTIONBLACKLIST.GetFutMasterVer();
				futStartDate = theACC_SECTIONBLACKLIST.GetFutStartDate();
				curStartDate = theACC_SECTIONBLACKLIST.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0801){
			if(theACC_WRITELIST.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_WRITELIST.GetCurMasterVer();
				futVersion = theACC_WRITELIST.GetFutMasterVer();
				futStartDate = theACC_WRITELIST.GetFutStartDate();
				curStartDate = theACC_WRITELIST.GetCurStartDate();
			}
		}
		else if(sParamPreFix == ACC_PARAM_ID_0802){
			if(theACC_OTHERCITYCARD.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theACC_OTHERCITYCARD.GetCurMasterVer();
				futVersion = theACC_OTHERCITYCARD.GetFutMasterVer();
				futStartDate = theACC_OTHERCITYCARD.GetFutStartDate();
				curStartDate = theACC_OTHERCITYCARD.GetCurStartDate();
			}
		}	
		//////////////////////////////////////
		else if(sParamPreFix == ECT_PARAM_ID_4011){
			// 一卡通黑名单参数类
			if(theECT_BLACKLIST.Internalize()){
				curVersion = theECT_BLACKLIST.GetCurMasterVer();
				futVersion = theECT_BLACKLIST.GetFutMasterVer();
				futStartDate = theECT_BLACKLIST.GetFutStartDate();
				curStartDate = theECT_BLACKLIST.GetCurStartDate();
			}
		}
		
		if(futVersion == UNAVALIBLE_VERSION){
			theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
			return false;
		}
		bool isCurVerValidate = false;
		bool isFutVerValidate = false;		    
		// 判断当前版本生效日期是否有效且小于当前时间
		if(curVersion != UNAVALIBLE_VERSION){
			if ((0 == ComCheckDateValid(curStartDate)) && (1 != CmpTwoDate(curStartDate ,ComGetBusiDate()))) {
				isCurVerValidate = true;
			}
		}
		// 判断将来版本生效日期是否有效且小于当前时间
		if ((0 == ComCheckDateValid(futStartDate)) && (1 != CmpTwoDate(futStartDate ,ComGetBusiDate()))) {
			isFutVerValidate = true;
		}
		// 当前版本生效
		if (isCurVerValidate && !isFutVerValidate) {
			theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
			return false;
		}
		// 将来版本生效
		else if (!isCurVerValidate && isFutVerValidate) {	
			theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
			return true;
		}
		// 当前版本和将来版本均未生效
		else if (!isCurVerValidate && !isFutVerValidate) {
			// 参数错误，抛异常，程序中断
			//throw CParamException(CParamException::REFRESH_VERSION_ERROR, _T(__FILE__), __LINE__, "参数生效日期异常:"+sParamPreFix);
		}
		// 当前版本和将来版本同时生效
		else {			
			if (1 == CmpTwoDate(curStartDate,futStartDate)) {
				theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
				return false;
			}
			else if (-1 == CmpTwoDate(curStartDate,futStartDate)) {
				theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
				return true;
			}
			else {
				// 当前版本和将来版本生效日期相同，版本号大的生效
				if(futVersion > curVersion) {
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
					return true;
				}
				else if(curVersion < futVersion) {
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
					return false;
				}
				else {
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
					return false;
				}
			}
		}
		theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
		return ret;	
	}
	
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘CheckIsNeedChangeParam’异常"));
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      把将来版本文件名修改为当前版本

@param      CString sParamPreFix 参数文件名前缀

@retval     CString  当前参数文件名

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::ConvertFutFileNameToCurrentVersion(CString sFutFileName)
{
	CString ret = _T("");
	if(sFutFileName == _T("")){
		return _T("");
	}
	int verTypeTmp = _ttoi(sFutFileName.Right(2));

	// 正式将来
	if(verTypeTmp == 1){
		ret = sFutFileName.Left(sFutFileName.GetLength()-2) + _T("00");
	}
	// 测试将来
	else if(verTypeTmp == 11){
		ret = sFutFileName.Left(sFutFileName.GetLength()-2) + _T("10");
	}	
	// 其他
	else{
		ret = _T("");
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      从全路径获得文件名

@param      (i)CString sPathAndName     目录+文件名

@retval     CString              参数文件名

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetFileNameFromFullPath(CString sPathAndName)
{
	CString strRet("");
	int nPos = sPathAndName.ReverseFind('\\');
	if (nPos != -1)	{
		strRet = sPathAndName.Mid(nPos+1);
	}
	else{
		nPos = sPathAndName.ReverseFind('/');
		if (nPos != -1)	{
			strRet = sPathAndName.Mid(nPos+1);
		}
		else{			
			strRet = sPathAndName;
		}
	}
	return strRet;
}
