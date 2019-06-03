#include "StdAfx.h"
#include "ParamHelper.h"
#include "ParameterDef.h"
#include "cparamhelp.h"
#include "CAccParamMaster.h"
#include "CParamException.h"
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

typedef bool (CParamHelper::*INIALLPARAM)(bool);
typedef struct _tag_allparam
{
	INIALLPARAM		iniallparam;
	CParamHelper*	paramHelper;
	HANDLE			hEvent;
	bool			bflag;
	bool			bret;
}ALLPARAM;
unsigned int _stdcall iniAllParam(void* lparam)
{
	ALLPARAM* pallparam=(ALLPARAM*)lparam;
	pallparam->bret=(pallparam->paramHelper->*(pallparam->iniallparam))(pallparam->bflag);
	::SetEvent(pallparam->hEvent);
	_endthreadex(0);
	return 0;
}
#define UD_ALLPARAM 2
bool CParamHelper::InitParam()
{
	int nIndex = 0;

	mapVersionProvider[AFC_DEVICECOMMONPARAM_ID] = &(theAFC_COMMON);				// 设备公共参数
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_COMMON_PARAM);    // 设备公共参数     1005

	mapVersionProvider[AFC_STAFFPWDPARAM_ID] = &(theAFC_STAFF);						// 设备操作员信息
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_OPERATOR_PARAM);  // 设备操作员信息   1006

	mapVersionProvider[AFC_ACCESSLEVELPARAM_ID] = &(theAFC_ACCESS_LEVEL);			// 设备权限参数
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_AUTHORITY_PARAM); // 设备权限参数     1007

	mapVersionProvider[AFC_TVMOPERATIONPARAM_ID] = &(theAFC_TVM_OPERATION);			// TVM 运营参数
	parameterNames[nIndex++] = 	_opl(TXT_AFC_TVM_OPERATION_PARAM);	  // TVM运营参数      1011

	mapVersionProvider[AFC_DEVICERUNTIMEPARAM_ID] = &(theAFC_DEV_TIME);				// 设备运行时间参数
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_TIME_PARAM);      // 设备运行时间参数 1089


	// ACC 参数
	mapVersionProvider[ACC_BUSINESS_ID] =				&theACC_BUSINESS;
	parameterNames[nIndex++] = _T("0101运营参数");

	mapVersionProvider[ACC_SERVICEFEE_ID] =				&theACC_SERVICEFEE;	
	parameterNames[nIndex++] = 	_T("0102行政处理手续费参数");

	mapVersionProvider[ACC_OVERTIME_UPDATE_ID] =	&theACC_OVERTIMEADJUEST;	
	parameterNames[nIndex++] = 	_T("0103超时补交费用参数");

	mapVersionProvider[ACC_DEVICE_ID] =			&theACC_DEVICECONTROL;
	parameterNames[nIndex++] = 	_T("0201设备控制参数");

	mapVersionProvider[ACC_STATION_ID] =			&theACC_LOCATION;
	parameterNames[nIndex++] = 	_T("0301车站位置参数");	

	mapVersionProvider[ACC_CHARGE_STATION_ID] =	&theACC_TOLLSTATION;	
	parameterNames[nIndex++] = 	_T("0302计费站点参数");	

	mapVersionProvider[ACC_SECTION_ID] =			&theACC_SECTION;
	parameterNames[nIndex++] = 	_T("0303区段参数");

	mapVersionProvider[ACC_CARDATTRIBUTE_ID] =	&theACC_PRODUCT;	
	parameterNames[nIndex++] = 	_T("0401票卡属性参数");	

	mapVersionProvider[ACC_ISSUE_COMPANY_ID] =	&theACC_ISSUECOMPANY;	
	parameterNames[nIndex++] = 	_T("0501票卡发行商参数");

	mapVersionProvider[ACC_BASE_PRICE_TABLE_ID] =	&theACC_BASEPRICE;
	parameterNames[nIndex++] = 	_T("0601基础票价表参数");

	mapVersionProvider[ACC_PRICE_LEVEL_TABLE_ID] = &theACC_PRICELEVEL;	
	parameterNames[nIndex++] = 	_T("0602票价级别表参数");

	mapVersionProvider[ACC_PRICE_ADJUEST_TABLE_ID] = &theACC_PRICEADJUEST;	
	parameterNames[nIndex++] = 	_T("0603票价调整策略表参数");

	mapVersionProvider[ACC_TIME_FLOATING_TABLE_ID] = &theACC_TIMEFLOAT;		
	parameterNames[nIndex++] = 	_T("0604时间浮动表参数");

	mapVersionProvider[ACC_TIME_TYPE_TABLE_ID] =	&theACC_TIMETYPE;
	parameterNames[nIndex++] = 	_T("0605时间类型表参数");

	mapVersionProvider[ACC_TTIME_TYPE_DETAIL_ID] = &theACC_TIMEDETAIL;	
	parameterNames[nIndex++] = 	_T("0606时间类型明细参数");

	mapVersionProvider[ACC_DISSCOUNT_TABLE_ID] =	&theACC_DISCOUNT;	
	parameterNames[nIndex++] = 	_T("0607折扣率表参数");

	mapVersionProvider[ACC_SINGLE_BLACK_LIST_ID] = &theACC_SINGLEBLACKLIST;	
	parameterNames[nIndex++] = 	_T("0701单条黑名单参数");

	mapVersionProvider[ACC_SECTION_BLACK_LIST_ID] = &theACC_SECTIONBLACKLIST;
	parameterNames[nIndex++] = 	_T("0702区段黑名单参数");

	mapVersionProvider[ACC_WRITE_LIST_ID] =		&theACC_WRITELIST;	
	parameterNames[nIndex++] = 	_T("0801白名单参数");

	mapVersionProvider[ACC_OTHER_CITY_CARD_ID] =	&theACC_OTHERCITYCARD;		
	parameterNames[nIndex++] = 	_T("0802异地卡/城市代码对照参数");
	return true;
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
	DWORD dwtick1=GetTickCount();
	//初始化 acc 参数
	if(!InitAccParam(TRUE)){
		ret = false;
	}
	// 初始化Afc参数
	if(!InitAfcParam(TRUE)){
		ret = false;
	}	
	DWORD dwtick2=GetTickCount();
	DWORD dwTickCount=dwtick2-dwtick1;
	
	// 所有版本异或
	m_strCurVersion.Format(_T("%u"),
		theACC_BASEPRICE.GetCurMasterVer() ^
		theACC_BUSINESS.GetCurMasterVer()^
		theACC_DEVICECONTROL.GetCurMasterVer() ^
		theACC_DISCOUNT.GetCurMasterVer()^
		theACC_ISSUECOMPANY.GetCurMasterVer()^
		theACC_LOCATION.GetCurMasterVer()^
		//theACC_NOTSERVSTATION.GetCurMasterVer()^
		theACC_OTHERCITYCARD.GetCurMasterVer()^
		theACC_OVERTIMEADJUEST.GetCurMasterVer()^
		theACC_PRICEADJUEST.GetCurMasterVer()^
		theACC_PRICELEVEL.GetCurMasterVer()^
		theACC_SECTION.GetCurMasterVer()^
		theACC_SERVICEFEE.GetCurMasterVer()^
		theACC_SINGLEBLACKLIST.GetCurMasterVer()^
		theACC_PRODUCT.GetCurMasterVer()^
		theACC_TIMEDETAIL.GetCurMasterVer()^
		theACC_TIMEFLOAT.GetCurMasterVer()^
		theACC_TIMETYPE.GetCurMasterVer()^
		theACC_TOLLSTATION.GetCurMasterVer()^
		theACC_WRITELIST.GetCurMasterVer()^
		theACC_SECTIONBLACKLIST.GetCurMasterVer()^
		theAFC_COMMON.GetCurMasterVer()^
		theAFC_STAFF.GetCurMasterVer()^
		theAFC_ACCESS_LEVEL.GetCurMasterVer()^
		theAFC_TVM_OPERATION.GetCurMasterVer()^
		theAFC_DEV_TIME.GetCurMasterVer()
		);

	m_strFurVersion.Format(_T("%u"),
		theACC_BASEPRICE.GetFutMasterVer() ^
		theACC_BUSINESS.GetFutMasterVer()^
		theACC_DEVICECONTROL.GetFutMasterVer() ^
		theACC_DISCOUNT.GetFutMasterVer()^
		theACC_ISSUECOMPANY.GetFutMasterVer()^
		theACC_LOCATION.GetFutMasterVer()^
		//theACC_NOTSERVSTATION.GetFutMasterVer()^
		theACC_OTHERCITYCARD.GetFutMasterVer()^
		theACC_OVERTIMEADJUEST.GetFutMasterVer()^
		theACC_PRICEADJUEST.GetFutMasterVer()^
		theACC_PRICELEVEL.GetFutMasterVer()^
		theACC_SECTION.GetFutMasterVer()^
		theACC_SERVICEFEE.GetFutMasterVer()^
		theACC_SINGLEBLACKLIST.GetFutMasterVer()^
		theACC_PRODUCT.GetFutMasterVer()^
		theACC_TIMEDETAIL.GetFutMasterVer()^
		theACC_TIMEFLOAT.GetFutMasterVer()^
		theACC_TIMETYPE.GetFutMasterVer()^
		theACC_TOLLSTATION.GetFutMasterVer()^
		theACC_WRITELIST.GetFutMasterVer()^
		theACC_SECTIONBLACKLIST.GetFutMasterVer()^
		theAFC_COMMON.GetFutMasterVer()^
		theAFC_STAFF.GetFutMasterVer()^
		theAFC_ACCESS_LEVEL.GetFutMasterVer()^
		theAFC_TVM_OPERATION.GetFutMasterVer()^
		theAFC_DEV_TIME.GetFutMasterVer()
		);

	return ret;
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
			if(sCurFileNameNew!=_T("")){
				DeleteFile(sCurFilePath+sCurFileName);
				if(CopyFile(sFutFilePath+sfutFileName,sCurFilePath+sCurFileNameNew,FALSE)){
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("ChangeFutVerToCurVer"), __LINE__, sfutFileName + _T("->") + sCurFileNameNew);
					DeleteFile(sFutFilePath+sfutFileName);
				}
				else{
					throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _tl(TXT_DELETE_FILE_FAILURE) + sFutFilePath+sfutFileName + _T("=>") + sCurFilePath+sCurFileNameNew);	
				}
			}

			sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath,true);
			sfutFileName = GetFutParamName(sPrefix,paraType,sFutFilePath,true);

			sCurFileNameNew = ConvertFutFileNameToCurrentVersion(sfutFileName);
			if(sCurFileNameNew!=_T("")){
				DeleteFile(sCurFilePath+sCurFileName);
				if(CopyFile(sFutFilePath+sfutFileName,sCurFilePath+sCurFileNameNew,FALSE)){
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("ChangeFutVerToCurVer"), __LINE__, sfutFileName + _T("->") + sCurFileNameNew);
					DeleteFile(sFutFilePath+sfutFileName);
				}
				else{
					throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _tl(TXT_COPY_FILE_FAILURE) + sFutFilePath+sfutFileName + _T("=>") + sCurFilePath+sCurFileNameNew);	
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_CHANG_CURVER_EXCEPTION)+sPrefix);
	}
}

typedef void (CParamHelper::*CHANGEVER)(CParamHelper::PARAM_TYPE,CString);
typedef bool (CParameterMaster::*INTERNALIZE)(bool,CString,CString);
typedef struct _tag_iniparameter
{
	CHANGEVER					changever;
	CParamHelper::PARAM_TYPE	paramType;
	CString						szPreFix;
	INTERNALIZE					internalize;
	bool						bReadFlag;
	CString						szDataPath;
	CString						szBackupPath;
	HANDLE*						pEvent;
	CParamHelper*				paramHelper;
	CParameterMaster*			paramMaster;
	bool						bRet;
	_tag_iniparameter()
	{
		bRet=true;
	}
}INIPARAMETER;

unsigned __stdcall initializeParameter(void* lpParam)
{
	INIPARAMETER* pParameter;
	pParameter=(INIPARAMETER*)lpParam;
	if(NULL!=pParameter)
	{
		CParamHelper* pThis=pParameter->paramHelper;
		(pThis->*(pParameter->changever))(pParameter->paramType,pParameter->szPreFix);
		CParameterMaster* pMaster=pParameter->paramMaster;
		bool bInteralize=(pMaster->*(pParameter->internalize))(pParameter->bReadFlag,pParameter->szDataPath,pParameter->szBackupPath);
		if(bInteralize==false)
		{
			pParameter->bRet=false;
		}
	}
	BOOL bset=SetEvent(*pParameter->pEvent);
	_endthreadex(0);
	return 0;
}

static void iniProcess(INIPARAMETER* param,int index,const TCHAR* tcParamID,CParameterMaster* paramMaster)
{
	INIPARAMETER* ptemp=param+index;
	ptemp->szPreFix=tcParamID;
	ptemp->paramMaster=paramMaster;
	unsigned int ithreadID=0;
	HANDLE hthread=(HANDLE)_beginthreadex(NULL,0,initializeParameter,ptemp,0,&ithreadID);
	CloseHandle(hthread);
	hthread=NULL;
}

#define UD_COUNT	9

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
		
		CString strMainPath = theTVM_INFO.GetDataPath();
		CString strBackupPath = theTVM_INFO.GetBackupPath();

		// acc param 只解析获取版本
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0101);
		if(!theACC_BUSINESS.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0102);
		if(!theACC_SERVICEFEE.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0103);
		if(!theACC_OVERTIMEADJUEST.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0201);
		if(!theACC_DEVICECONTROL.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0301);
		if(!theACC_LOCATION.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(AFC_VER,ACC_PARAM_ID_0302);
		if(!theACC_TOLLSTATION.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0303);
		if(!theACC_SECTION.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0401);
		if(!theACC_PRODUCT.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0501);
		if(!theACC_ISSUECOMPANY.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0601);
		if(!theACC_BASEPRICE.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0602);
		if(!theACC_PRICELEVEL.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0603);
		if(!theACC_PRICEADJUEST.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0604);
		if(!theACC_TIMEFLOAT.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0605);
		if(!theACC_TIMETYPE.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0606);
		if(!theACC_TIMEDETAIL.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0607);
		if(!theACC_DISCOUNT.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		//ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0608);
		//if(!theACC_NOTSERVSTATION.Internalize(flag,strMainPath,strBackupPath)){
		//	ret = false;
		//}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0701);
		if(!theACC_SINGLEBLACKLIST.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0702);
		if(!theACC_SECTIONBLACKLIST.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0801);
		if(!theACC_WRITELIST.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0802);
		if(!theACC_OTHERCITYCARD.Internalize(flag,strMainPath,strBackupPath)){
			ret = false;
		}
		if (!ret)
		{
			throw;
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
		theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_INIPARA_EXCEPTION)));
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
// bool CParamHelper::InitEctParam()
// {
// 	bool ret = true;
// 	try{
// 		
// 		// 一卡通黑名单参数类
// 		ChangeFutVerToCurVer(ECT_VER,ECT_PARAM_ID_4011);
// 		if(!theECT_BLACKLIST.Internalize()){
// 			ret =  false;
// 		}	
// 	}
// 	catch (CSysException& e) {	
// 		ret =  false;
// 		throw;		
// 	}
// 	catch (...) {
// 		ret =  false;
// 		throw;
// 		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("函数‘InitEctParam’异常")));
// 	}
// 	return ret;
// }

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化Afc参数

@param     (i)bool readflag 读取参数标志

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::InitAfcParam(bool readflag)
{
	bool ret = true;
	try{
		CString strMainPath = theTVM_INFO.GetDataPath();
		CString strBackupPath = theTVM_INFO.GetBackupPath();

		// 设备公共参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1005);
		if(!theAFC_COMMON.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("设备公共参数类初始化失败"));			
			ret =  false;
		}
		// 操作员参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1006);
		if(!theAFC_STAFF.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("操作员参数类初始化失败"));			
			ret =  false;
		}
		// 设备权限参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1007);
		if(!theAFC_ACCESS_LEVEL.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("设备权限参数类初始化失败"));			
			ret =  false;
		}
		// TVM运营参数类
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1011);
		if(!theAFC_TVM_OPERATION.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("TVM运营参数类初始化失败"));			
			ret =  false;
		}	
		// 设备运行时间参数
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1089);
		if(!theAFC_DEV_TIME.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("设备运行时间参数初始化失败"));			
			ret =  false;
		}	
		// 运营商代码映射表
		/*ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1097);
		if(!theAFC_OPERATOR_CODE.Internalize(readflag,strMainPath,strBackupPath)){
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("运营商代码映射表参数初始化失败"));			
		ret =  false;
		}*/	
		// 模式履历参数
		//ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1002);
		//if(!theAFC_MODERECORD.Internalize()){
		//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("模式履历参数初始化失败"));			
		//	ret =  false;
		//}
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
	curAccVersion.lBusinessParam= theACC_BUSINESS.GetCurMasterVer();			
	curAccVersion.lServerFeeParam= theACC_SERVICEFEE.GetCurMasterVer();		
	curAccVersion.lOverTimeAdjuestParam= theACC_OVERTIMEADJUEST.GetCurMasterVer();	
	curAccVersion.lDeviceControlParam= theACC_DEVICECONTROL.GetCurMasterVer();	
	curAccVersion.lStationParam= theACC_LOCATION.GetCurMasterVer();			
	curAccVersion.lPayStationParam= theACC_TOLLSTATION.GetCurMasterVer();		
	curAccVersion.lCardAttributeParam= theACC_PRODUCT.GetCurMasterVer();	
	curAccVersion.lBasePriceParam= theACC_BASEPRICE.GetCurMasterVer();		
	curAccVersion.lPrieLevelParam= theACC_PRICELEVEL.GetCurMasterVer();		
	curAccVersion.lPriceAdjuestParam= theACC_PRICEADJUEST.GetCurMasterVer();		
	curAccVersion.lTimeFloatParam= theACC_TIMEFLOAT.GetCurMasterVer();		
	curAccVersion.lTimeTypeParam= theACC_TIMETYPE.GetCurMasterVer();			
	curAccVersion.lTimeDetailParam= theACC_TIMEDETAIL.GetCurMasterVer();		
	curAccVersion.lDiscountParam= theACC_DISCOUNT.GetCurMasterVer();			
	//curAccVersion.lNotServStationParam= theACC_NOTSERVSTATION.GetCurMasterVer();	
	curAccVersion.lWriteListParam= theACC_WRITELIST.GetCurMasterVer();		
	curAccVersion.lOtherCityCardParam= theACC_OTHERCITYCARD.GetCurMasterVer();	
	curAccVersion.lSingleBlackListParam= theACC_SINGLEBLACKLIST.GetCurMasterVer();	
	curAccVersion.lSectionBlackListParam= theACC_SECTIONBLACKLIST.GetCurMasterVer();	
	curAccVersion.lSectionParam= theACC_SECTION.GetCurMasterVer();			
	curAccVersion.lissueCompanyParam= theACC_ISSUECOMPANY.GetCurMasterVer();		
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
	futAccVersion.lBusinessParam= theACC_BUSINESS.GetFutMasterVer();			
	futAccVersion.lServerFeeParam= theACC_SERVICEFEE.GetFutMasterVer();		
	futAccVersion.lOverTimeAdjuestParam= theACC_OVERTIMEADJUEST.GetFutMasterVer();	
	futAccVersion.lDeviceControlParam= theACC_DEVICECONTROL.GetFutMasterVer();	
	futAccVersion.lStationParam= theACC_LOCATION.GetFutMasterVer();			
	futAccVersion.lPayStationParam= theACC_TOLLSTATION.GetFutMasterVer();		
	futAccVersion.lCardAttributeParam= theACC_PRODUCT.GetFutMasterVer();	
	futAccVersion.lBasePriceParam= theACC_BASEPRICE.GetFutMasterVer();		
	futAccVersion.lPrieLevelParam= theACC_PRICELEVEL.GetFutMasterVer();		
	futAccVersion.lPriceAdjuestParam= theACC_PRICEADJUEST.GetFutMasterVer();		
	futAccVersion.lTimeFloatParam= theACC_TIMEFLOAT.GetFutMasterVer();		
	futAccVersion.lTimeTypeParam= theACC_TIMETYPE.GetFutMasterVer();			
	futAccVersion.lTimeDetailParam= theACC_TIMEDETAIL.GetFutMasterVer();		
	futAccVersion.lDiscountParam= theACC_DISCOUNT.GetFutMasterVer();			
	//futAccVersion.lNotServStationParam= theACC_NOTSERVSTATION.GetFutMasterVer();	
	futAccVersion.lWriteListParam= theACC_WRITELIST.GetFutMasterVer();		
	futAccVersion.lOtherCityCardParam= theACC_OTHERCITYCARD.GetFutMasterVer();	
	futAccVersion.lSingleBlackListParam= theACC_SINGLEBLACKLIST.GetFutMasterVer();	
	futAccVersion.lSectionBlackListParam= theACC_SECTIONBLACKLIST.GetFutMasterVer();	
	futAccVersion.lSectionParam= theACC_SECTION.GetFutMasterVer();			
	futAccVersion.lissueCompanyParam= theACC_ISSUECOMPANY.GetFutMasterVer();		

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取ECT当前参数版本

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CParamHelper::GetCurEctParamVersion(VERSION_ECT& curEctVersion)
//{
//	
//	// 一卡通黑名单参数类(充值黑名单)
//	curEctVersion.lBlacklistParam = theECT_BLACKLIST.GetCurMasterVer();
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取ECT未来参数版本

@param      无

@retval     bool   true：成功   false：失败   

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CParamHelper::GetFutEctParamVersion(VERSION_ECT& futEctVersion)
//{
//	futEctVersion.lBlacklistParam = theECT_BLACKLIST.GetFutMasterVer();
//}

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
	//??curAfcVersion.lTVMOperationParam = theAFC_TVM_OPERATION.GetCurMasterVer();
	// 设备公共参数类
	curAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetCurMasterVer();
	// 操作员参数类
	curAfcVersion.lStaffPwdParam = theAFC_STAFF.GetCurMasterVer();
	// 设备运行时间参数
	curAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetCurMasterVer();
	// 运营商代码映射表参数
	//curAfcVersion.lOpratorCodeTable = theAFC_OPERATOR_CODE.GetCurMasterVer();
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
	//??futAfcVersion.lTVMOperationParam = theAFC_TVM_OPERATION.GetFutMasterVer();
	// 设备公共参数类
	futAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetFutMasterVer();
	// 操作员参数类
	futAfcVersion.lStaffPwdParam = theAFC_STAFF.GetFutMasterVer();
	// 设备运行时间参数
	futAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetFutMasterVer();
	// 运营商代码映射表参数
	//futAfcVersion.lOpratorCodeTable = theAFC_OPERATOR_CODE.GetFutMasterVer();
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
void CParamHelper::UpdateLocalVersion(VEC_VERSION_INFO& verInfo, bool bUpdate)
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

		CString dataPath = theTVM_INFO.GetDataPath();
		CString dataPathBackup = theTVM_INFO.GetBackupPath();

		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		CString sUpdatePath = CString(szRunPath) + PM_UPDATE_DIR;

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
				CString sCurFilePathName = sCurFileName.IsEmpty() ? sCurFilePath : sCurFilePath + sCurFileName;
				CString sCurFilePathNameBackup = sCurFileNameBackup.IsEmpty() ? sCurFilePathBackup : sCurFilePathBackup + sCurFileNameBackup;
				//if(sCurFileName == _T("")){
					//sCurFilePathName = _T("");
				//}
				//if(sCurFileNameBackup == _T("")){
					//sCurFilePathNameBackup = _T("");
				//}
				if(sCurFilePathName.IsEmpty() && sCurFilePathNameBackup.IsEmpty()){
					continue;
				}
				GetVersionChangeInfo(updfilePath,sCurFilePathName,versionChangeInfo);// 获取版本切换信息

				if (bUpdate){		
					// 移动备份目录参数文件到当前参数目录
					if(!sCurFilePathName.IsEmpty() && !sCurFileName.IsEmpty() && DeleteFile(sCurFilePathName) != TRUE){
						theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("删除旧版本参数文件失败："+sCurFilePathName));	
					}
					if(!sCurFilePathNameBackup.IsEmpty() && !sCurFileNameBackup.IsEmpty() && DeleteFile(sCurFilePathNameBackup) != TRUE){
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
							throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _tl(TXT_MOVE_FILE_FAILURE) + updfilePath + _T("=>") + sCurFilePath + updateFileName);
						}	
					}					

					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("MoveFile"), __LINE__,updfilePath + _T("->") + sCurFilePath + updateFileName+_T(" OK"));
					// 删除更新文件
					//if(sCurFilePathName != ""){
					if(!updfilePath.IsEmpty() && DeleteFile(updfilePath) != TRUE){
						throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__,_tl(TXT_DELETE_UPDATE_FILE_FAILURE) + updfilePath);
					//theDEBUG_LOG->WriteData("CParamHelper", "UpdateLocalVersion", __LINE__, "DeleteFile" + sCurFilePathName);
					//}
					}
				}

				verInfo.push_back(versionChangeInfo);

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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_UPDATE_VERSION_EXCEPTION));
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
	CString strParamFileName(_T(""));
	CString strParamPathName(_T(""));
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
					throw CParamException(CParamException::FILE_SAM_ERR, _T(__FILE__), __LINE__,strParamPathName + _tl(TXT_CRC_ERROR));
					break;
				case 2: 
					throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__,strParamPathName);
					break;
				case 3: 
					throw CParamException(CParamException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__,strParamPathName);
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_REMOVE_TRANS_HEAD_EXCEPTION));
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
	CString sDataPath = theTVM_INFO.GetDataPath();
	CString sBackupPath = theTVM_INFO.GetBackupPath();

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
		memset(DelFileSource, 0x00, MAX_PATH*sizeof(TCHAR));
		_tcscat(DelFileSource, sDirPath + _T("*.*"));

		fos.pFrom = DelFileSource;

		// 删除文件夹及其内容
		SHFileOperation( &fos);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_DEL_ALL_EXCEPTION));
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
	CString sDataPath = theTVM_INFO.GetDataPath();
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
			throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _tl(TXT_COPY_FILE_FAILURE) + sCurfilePath + _T("=>") + sBackupPathFile);	
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

	auto_ptr<CXFile> srcFile(new CXFile(srcParamFile));
	VARIABLE_DATA srcbinInfo;
	// 读取文件信息到binInfo中
	if (srcFile->ReadBinFile(srcbinInfo) != CXFile::FILE_OPERATION_OK){
		throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
	}
	LPBYTE lpData = srcbinInfo.lpData;
	// 跳过信息头
	lpData += 6;
	// 参数ID
	iParamOldId = ComMakeWORD(*lpData, *(lpData+1))/*BCD2int((char*)lpData,2)*/;
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

	if(PathIsDirectory(dstParamFile) || ComCheckFileExist(dstParamFile) == FALSE){// 如果是文件夹的话，或者文件不存在的话
		verChgTmp.oldVersion = 0xFFFFFFFF;
	}
	else{
		auto_ptr<CXFile> dstFile(new CXFile(dstParamFile));
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
		CString sDataPath = theTVM_INFO.GetDataPath();
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
							throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _tl(TXT_MOVE_FILE_FAILURE) + sBackupFilePath + sBackUpFileName + _T("=>") + sCurFilePath + sBackUpFileName);	
						}					
						else{
							// 删除当前版本参数文件						
							if(DeleteFile(sCurFilePath + sCurFileName) != TRUE){
								throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__,_tl(TXT_DELETE_CURRENT_FAIL) + sCurFilePath + sCurFileName);
							}
						}
					}
					else{
						throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__, _tl(TXT_PARA_SAME) + sCurFileName + _T("=") + sBackUpFileName);	
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_CUR_VERSION_EXCEPTION));
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
	return GetTheParamPathNameImp(paraID,ParamPath,theTVM_INFO.GetDataPath());
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
	return GetTheParamPathNameImp(paraID,ParamPath,theTVM_INFO.GetBackupPath());
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_FUNCTION_GET_PATH_NAME_EXCEPTION));
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
		sPath = theTVM_INFO.GetDataPath();
	}
	else{
		sPath = theTVM_INFO.GetBackupPath();
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
		sPath = theTVM_INFO.GetDataPath();
	}
	else{
		sPath = theTVM_INFO.GetBackupPath();
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
	WORD iParaID = _tcstol(sParaID,NULL,16);
	WORD afcParameterIDs[] = {
		AFC_MODEHISTORY_ID , 
		AFC_DEVICECOMMONPARAM_ID, 
		AFC_STAFFPWDPARAM_ID, 
		AFC_ACCESSLEVELPARAM_ID, 
		AFC_TVMOPERATIONPARAM_ID, 
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
		ACC_ISSUE_COMPANY_ID			    ,					// ACC票卡发行商参数
		ACC_BASE_PRICE_TABLE_ID				,					// ACC基础票价表参数
		ACC_PRICE_LEVEL_TABLE_ID			,					// ACC票价级别表参数
		ACC_PRICE_ADJUEST_TABLE_ID			,					// ACC票价调整策略表参数
		ACC_TIME_FLOATING_TABLE_ID			,					// ACC时间浮动表参数
		ACC_TIME_TYPE_TABLE_ID				,					// ACC时间类型表参数
		ACC_TTIME_TYPE_DETAIL_ID			,					// ACC时间类型明细参数
		ACC_DISSCOUNT_TABLE_ID				,					// ACC折扣率表参数
		ACC_NOT_SERVICE_STATION_ID			,					// ACC停运车站表
		ACC_SINGLE_BLACK_LIST_ID			,					// ACC单条黑名单参数
		ACC_SECTION_BLACK_LIST_ID			,					// ACC区段黑名单参数
		ACC_WRITE_LIST_ID					,					// ACC白名单参数
		ACC_OTHER_CITY_CARD_ID				,					// ACC异地卡/城市代码对照参数
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
	CString strRet(_T(""));
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
	CString strRet(_T(""));
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
	CString strRet(_T(""));
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

	//??
	// 系统参数
	//verInfo.paramId = ACC_SYSTEM_ID;
	//verInfo.versionType = theACC_SYSTEM.GetCurVerType();
	//verInfo.oldVersion = theACC_SYSTEM.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SYSTEM.GetCurStartDate();
	//verInfo.newVersion = theACC_SYSTEM.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SYSTEM.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// 业务规则参数
	//verInfo.paramId = ACC_BUSINESS_ID;
	//verInfo.versionType = theACC_BUSINESS.GetCurVerType();
	//verInfo.oldVersion = theACC_BUSINESS.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_BUSINESS.GetCurStartDate();
	//verInfo.newVersion = theACC_BUSINESS.GetFutMasterVer();
	//verInfo.newStartDate = theACC_BUSINESS.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// 参数版本声明
	//verInfo.paramId = ACC_VERSION_ID;
	//verInfo.versionType = theACC_VERSION.GetCurVerType();
	//verInfo.oldVersion = theACC_VERSION.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_VERSION.GetCurStartDate();
	//verInfo.newVersion = theACC_VERSION.GetFutMasterVer();
	//verInfo.newStartDate = theACC_VERSION.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// 黑名单参数
	//verInfo.paramId = ACC_BLACK_LIST_ID;
	//verInfo.versionType = theACC_BLACKLIST.GetCurVerType();
	//verInfo.oldVersion = theACC_BLACKLIST.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_BLACKLIST.GetCurStartDate();
	//verInfo.newVersion = theACC_BLACKLIST.GetFutMasterVer();
	//verInfo.newStartDate = theACC_BLACKLIST.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	// 产品参数
	//verInfo.paramId = ACC_PRODUCT_ID;
	//verInfo.versionType = theACC_PRODUCT.GetCurVerType();
	//verInfo.oldVersion = theACC_PRODUCT.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_PRODUCT.GetCurStartDate();
	//verInfo.newVersion = theACC_PRODUCT.GetFutMasterVer();
	//verInfo.newStartDate = theACC_PRODUCT.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// 位置（车站、区段）参数
	//verInfo.paramId = ACC_LOCATION_ID;
	//verInfo.versionType = theACC_LOCATION.GetCurVerType();
	//verInfo.oldVersion = theACC_LOCATION.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_LOCATION.GetCurStartDate();
	//verInfo.newVersion = theACC_LOCATION.GetFutMasterVer();
	//verInfo.newStartDate = theACC_LOCATION.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	// 日历参数
	//verInfo.paramId = ACC_CALANDER_ID;
	//verInfo.versionType = theACC_CALANDER.GetCurVerType();
	//verInfo.oldVersion = theACC_CALANDER.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_CALANDER.GetCurStartDate();
	//verInfo.newVersion = theACC_CALANDER.GetFutMasterVer();
	//verInfo.newStartDate = theACC_CALANDER.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// 消费参数
	//verInfo.paramId = ACC_SHOP_ID;
	//verInfo.versionType = theACC_SHOP.GetCurVerType();
	//verInfo.oldVersion = theACC_SHOP.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SHOP.GetCurStartDate();
	//verInfo.newVersion = theACC_SHOP.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SHOP.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// 销售参数
	//verInfo.paramId = ACC_SALAS_ID;
	//verInfo.versionType = theACC_SALES.GetCurVerType();
	//verInfo.oldVersion = theACC_SALES.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SALES.GetCurStartDate();
	//verInfo.newVersion = theACC_SALES.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SALES.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);
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
		
		CString strMainPath = theTVM_INFO.GetDataPath();
		CString strBackupPath = theTVM_INFO.GetBackupPath();
		
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
		else if(sParamPreFix == AFC_PARAM_ID_1011){
			if(theAFC_TVM_OPERATION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_TVM_OPERATION.GetCurMasterVer();
				futVersion = theAFC_TVM_OPERATION.GetFutMasterVer();
				futStartDate = theAFC_TVM_OPERATION.GetFutStartDate();
				curStartDate = theAFC_TVM_OPERATION.GetCurStartDate();
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
		//else if(sParamPreFix == ACC_PARAM_ID_0608){
		//	if(theACC_NOTSERVSTATION.readVersionDataInfo(strMainPath,strBackupPath)){
		//		curVersion = theACC_NOTSERVSTATION.GetCurMasterVer();
		//		futVersion = theACC_NOTSERVSTATION.GetFutMasterVer();
		//		futStartDate = theACC_NOTSERVSTATION.GetFutStartDate();
		//		curStartDate = theACC_NOTSERVSTATION.GetCurStartDate();
		//	}
		//}
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_tl(TXT_CHANGE_PARAM));
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
	CString strRet(_T(""));
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

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取当前版本异或和

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetCurXorSumVersion(){
	return m_strCurVersion;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取将来版本异或和

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetFutXorSumVersion(){
	return m_strFurVersion;
}