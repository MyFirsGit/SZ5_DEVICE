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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CParamHelper::CParamHelper(void)
{	
//	m_rwHelper = new CRWTool();

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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

	mapVersionProvider[AFC_DEVICECOMMONPARAM_ID] = &(theAFC_COMMON);				// �豸��������
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_COMMON_PARAM);    // �豸��������     1005

	mapVersionProvider[AFC_STAFFPWDPARAM_ID] = &(theAFC_STAFF);						// �豸����Ա��Ϣ
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_OPERATOR_PARAM);  // �豸����Ա��Ϣ   1006

	mapVersionProvider[AFC_ACCESSLEVELPARAM_ID] = &(theAFC_ACCESS_LEVEL);			// �豸Ȩ�޲���
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_AUTHORITY_PARAM); // �豸Ȩ�޲���     1007

	mapVersionProvider[AFC_TVMOPERATIONPARAM_ID] = &(theAFC_TVM_OPERATION);			// TVM ��Ӫ����
	parameterNames[nIndex++] = 	_opl(TXT_AFC_TVM_OPERATION_PARAM);	  // TVM��Ӫ����      1011

	mapVersionProvider[AFC_DEVICERUNTIMEPARAM_ID] = &(theAFC_DEV_TIME);				// �豸����ʱ�����
	parameterNames[nIndex++] = 	_opl(TXT_AFC_DEVICE_TIME_PARAM);      // �豸����ʱ����� 1089


	// ACC ����
	mapVersionProvider[ACC_BUSINESS_ID] =				&theACC_BUSINESS;
	parameterNames[nIndex++] = _T("0101��Ӫ����");

	mapVersionProvider[ACC_SERVICEFEE_ID] =				&theACC_SERVICEFEE;	
	parameterNames[nIndex++] = 	_T("0102�������������Ѳ���");

	mapVersionProvider[ACC_OVERTIME_UPDATE_ID] =	&theACC_OVERTIMEADJUEST;	
	parameterNames[nIndex++] = 	_T("0103��ʱ�������ò���");

	mapVersionProvider[ACC_DEVICE_ID] =			&theACC_DEVICECONTROL;
	parameterNames[nIndex++] = 	_T("0201�豸���Ʋ���");

	mapVersionProvider[ACC_STATION_ID] =			&theACC_LOCATION;
	parameterNames[nIndex++] = 	_T("0301��վλ�ò���");	

	mapVersionProvider[ACC_CHARGE_STATION_ID] =	&theACC_TOLLSTATION;	
	parameterNames[nIndex++] = 	_T("0302�Ʒ�վ�����");	

	mapVersionProvider[ACC_SECTION_ID] =			&theACC_SECTION;
	parameterNames[nIndex++] = 	_T("0303���β���");

	mapVersionProvider[ACC_CARDATTRIBUTE_ID] =	&theACC_PRODUCT;	
	parameterNames[nIndex++] = 	_T("0401Ʊ�����Բ���");	

	mapVersionProvider[ACC_ISSUE_COMPANY_ID] =	&theACC_ISSUECOMPANY;	
	parameterNames[nIndex++] = 	_T("0501Ʊ�������̲���");

	mapVersionProvider[ACC_BASE_PRICE_TABLE_ID] =	&theACC_BASEPRICE;
	parameterNames[nIndex++] = 	_T("0601����Ʊ�۱����");

	mapVersionProvider[ACC_PRICE_LEVEL_TABLE_ID] = &theACC_PRICELEVEL;	
	parameterNames[nIndex++] = 	_T("0602Ʊ�ۼ�������");

	mapVersionProvider[ACC_PRICE_ADJUEST_TABLE_ID] = &theACC_PRICEADJUEST;	
	parameterNames[nIndex++] = 	_T("0603Ʊ�۵������Ա����");

	mapVersionProvider[ACC_TIME_FLOATING_TABLE_ID] = &theACC_TIMEFLOAT;		
	parameterNames[nIndex++] = 	_T("0604ʱ�両�������");

	mapVersionProvider[ACC_TIME_TYPE_TABLE_ID] =	&theACC_TIMETYPE;
	parameterNames[nIndex++] = 	_T("0605ʱ�����ͱ����");

	mapVersionProvider[ACC_TTIME_TYPE_DETAIL_ID] = &theACC_TIMEDETAIL;	
	parameterNames[nIndex++] = 	_T("0606ʱ��������ϸ����");

	mapVersionProvider[ACC_DISSCOUNT_TABLE_ID] =	&theACC_DISCOUNT;	
	parameterNames[nIndex++] = 	_T("0607�ۿ��ʱ����");

	mapVersionProvider[ACC_SINGLE_BLACK_LIST_ID] = &theACC_SINGLEBLACKLIST;	
	parameterNames[nIndex++] = 	_T("0701��������������");

	mapVersionProvider[ACC_SECTION_BLACK_LIST_ID] = &theACC_SECTIONBLACKLIST;
	parameterNames[nIndex++] = 	_T("0702���κ���������");

	mapVersionProvider[ACC_WRITE_LIST_ID] =		&theACC_WRITELIST;	
	parameterNames[nIndex++] = 	_T("0801����������");

	mapVersionProvider[ACC_OTHER_CITY_CARD_ID] =	&theACC_OTHERCITYCARD;		
	parameterNames[nIndex++] = 	_T("0802��ؿ�/���д�����ղ���");
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ�����в���

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////	
bool CParamHelper::InitAllParam()
{
	bool ret = true;
	DWORD dwtick1=GetTickCount();
	//��ʼ�� acc ����
	if(!InitAccParam(TRUE)){
		ret = false;
	}
	// ��ʼ��Afc����
	if(!InitAfcParam(TRUE)){
		ret = false;
	}	
	DWORD dwtick2=GetTickCount();
	DWORD dwTickCount=dwtick2-dwtick1;
	
	// ���а汾���
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
@brief      ��ָ��δ���汾�л�����ǰ�汾

@param      ��

@retval     ��

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
@brief      ��ʼ��Acc����

@param      (i)bool flag  true:��ǰ�汾  false:δ���汾 

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////	
bool CParamHelper::InitAccParam(bool flag)
{
	bool ret = true;
	try{
		
		CString strMainPath = theTVM_INFO.GetDataPath();
		CString strBackupPath = theTVM_INFO.GetBackupPath();

		// acc param ֻ������ȡ�汾
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
@brief       ��ʼ��Ect����

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// bool CParamHelper::InitEctParam()
// {
// 	bool ret = true;
// 	try{
// 		
// 		// һ��ͨ������������
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
// 		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������InitEctParam���쳣")));
// 	}
// 	return ret;
// }

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��Afc����

@param     (i)bool readflag ��ȡ������־

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::InitAfcParam(bool readflag)
{
	bool ret = true;
	try{
		CString strMainPath = theTVM_INFO.GetDataPath();
		CString strBackupPath = theTVM_INFO.GetBackupPath();

		// �豸����������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1005);
		if(!theAFC_COMMON.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�豸�����������ʼ��ʧ��"));			
			ret =  false;
		}
		// ����Ա������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1006);
		if(!theAFC_STAFF.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("����Ա�������ʼ��ʧ��"));			
			ret =  false;
		}
		// �豸Ȩ�޲�����
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1007);
		if(!theAFC_ACCESS_LEVEL.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�豸Ȩ�޲������ʼ��ʧ��"));			
			ret =  false;
		}
		// TVM��Ӫ������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1011);
		if(!theAFC_TVM_OPERATION.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("TVM��Ӫ�������ʼ��ʧ��"));			
			ret =  false;
		}	
		// �豸����ʱ�����
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1089);
		if(!theAFC_DEV_TIME.Internalize(readflag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�豸����ʱ�������ʼ��ʧ��"));			
			ret =  false;
		}	
		// ��Ӫ�̴���ӳ���
		/*ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1097);
		if(!theAFC_OPERATOR_CODE.Internalize(readflag,strMainPath,strBackupPath)){
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("��Ӫ�̴���ӳ��������ʼ��ʧ��"));			
		ret =  false;
		}*/	
		// ģʽ��������
		//ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1002);
		//if(!theAFC_MODERECORD.Internalize()){
		//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ģʽ����������ʼ��ʧ��"));			
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
		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������InitAfcParam���쳣")));
	}
	return ret;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡACC��ǰ�����汾

@param      ��

@retval     ��   

@exception  ��
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
@brief      ��ȡACCδ�������汾

@param      ��

@retval     ��  

@exception  ��
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
@brief      ��ȡECT��ǰ�����汾

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CParamHelper::GetCurEctParamVersion(VERSION_ECT& curEctVersion)
//{
//	
//	// һ��ͨ������������(��ֵ������)
//	curEctVersion.lBlacklistParam = theECT_BLACKLIST.GetCurMasterVer();
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡECTδ�������汾

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CParamHelper::GetFutEctParamVersion(VERSION_ECT& futEctVersion)
//{
//	futEctVersion.lBlacklistParam = theECT_BLACKLIST.GetFutMasterVer();
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡAFC��ǰ�����汾

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetCurAfcParamVersion(VERSION_AFC& curAfcVersion)
{
	// �豸Ȩ�޲�����
	curAfcVersion.lAccessLevelParam = theAFC_ACCESS_LEVEL.GetCurMasterVer();
	// BOM��Ӫ������
	//??curAfcVersion.lTVMOperationParam = theAFC_TVM_OPERATION.GetCurMasterVer();
	// �豸����������
	curAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetCurMasterVer();
	// ����Ա������
	curAfcVersion.lStaffPwdParam = theAFC_STAFF.GetCurMasterVer();
	// �豸����ʱ�����
	curAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetCurMasterVer();
	// ��Ӫ�̴���ӳ������
	//curAfcVersion.lOpratorCodeTable = theAFC_OPERATOR_CODE.GetCurMasterVer();
	// ģʽ��������
	curAfcVersion.lModeHistoryParam = theAFC_MODERECORD.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡAFCδ�������汾

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetFutAfcParamVersion(VERSION_AFC& futAfcVersion)
{
	// �豸Ȩ�޲�����
	futAfcVersion.lAccessLevelParam = theAFC_ACCESS_LEVEL.GetFutMasterVer();
	// BOM��Ӫ������
	//??futAfcVersion.lTVMOperationParam = theAFC_TVM_OPERATION.GetFutMasterVer();
	// �豸����������
	futAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetFutMasterVer();
	// ����Ա������
	futAfcVersion.lStaffPwdParam = theAFC_STAFF.GetFutMasterVer();
	// �豸����ʱ�����
	futAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetFutMasterVer();
	// ��Ӫ�̴���ӳ������
	//futAfcVersion.lOpratorCodeTable = theAFC_OPERATOR_CODE.GetFutMasterVer();
	// ģʽ��������
	futAfcVersion.lModeHistoryParam = theAFC_MODERECORD.GetFutMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����汾�Ƚ�

@param      (i)DWORD ver1 �²����汾
@param      (i)DWORD ver2 ���ز����汾

@retval     bool  true���汾��һ�£���Ҫ���£� false���汾һ�£�����Ҫ���£�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::CompareVersion(DWORD ver1, DWORD ver2)
{
	// �²����汾Ϊ0��ȫF����
	if ((ver1 != 0) && (ver1 != UNAVALIBLE_VERSION)) {
		if (ver1 != ver2) {
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���±��ز����ļ����������ļ�������Ŀ¼���������ز���Ŀ¼�£�

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::UpdateLocalVersion(VEC_VERSION_INFO& verInfo, bool bUpdate)
{
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("UpdateLocalVersion"), __LINE__, _T("<-"));
	CFileFind finder;	
	try{
		// ���Ҳ��������ļ�
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
				// ȡ�ļ���
				updateFileName = finder.GetFileName();
				// ���汾����
				verType = GetParamVersionType(updateFileName);
				paraType = GetParamType(updateFileName);
				sPrefix = updateFileName.Left(9);

				// ��ǰ��
				if(verType == CUR_VER || verType == CUR_TEST){		
					sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath,false);
					sCurFileNameBackup = GetCurParamName(sPrefix,paraType,sCurFilePathBackup,true);
					//	// ���԰汾������ʽ�汾�Ǳ��ݵ�ǰ��ʽ�汾
					//	//if(verType == CUR_TEST  && GetParamVersionType(sCurFileName) == CUR_VER){
					//	//	BackUpLocalCurVersion(paraType,sCurFileName,sCurFilePath);
					//	//}
				}
				// ������
				else if(verType == FUT_VER || verType == FUT_TEST){
					sCurFileName = GetFutParamName(sPrefix,paraType,sCurFilePath,false);
					sCurFileNameBackup = GetFutParamName(sPrefix,paraType,sCurFilePathBackup,true);
				}	
				// δ֪����
				else{
					throw CParamException(CParamException::FILE_VER_ERR, _T(__FILE__), __LINE__,updateFileName);
				}	

				// �汾��һ��������ļ�
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
				GetVersionChangeInfo(updfilePath,sCurFilePathName,versionChangeInfo);// ��ȡ�汾�л���Ϣ

				if (bUpdate){		
					// �ƶ�����Ŀ¼�����ļ�����ǰ����Ŀ¼
					if(!sCurFilePathName.IsEmpty() && !sCurFileName.IsEmpty() && DeleteFile(sCurFilePathName) != TRUE){
						theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ɾ���ɰ汾�����ļ�ʧ�ܣ�"+sCurFilePathName));	
					}
					if(!sCurFilePathNameBackup.IsEmpty() && !sCurFileNameBackup.IsEmpty() && DeleteFile(sCurFilePathNameBackup) != TRUE){
						theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ɾ�����ݾɰ汾�����ļ�ʧ�ܣ�"+sCurFilePathNameBackup));	
					}
					if(DeleteFile(sCurFilePath + updateFileName) == TRUE){
						//theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�°汾�����ļ��Ѵ���,ɾ��:"+sCurFilePathName));						
					}
					if(DeleteFile(sCurFilePathBackup + updateFileName) == TRUE){
						//theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�����°汾�����ļ��Ѵ���,ɾ��:"+sCurFilePathName));						
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
					// ɾ�������ļ�
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
				//	throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__, _T("�������°汾�뱾�ذ汾��ͬ��" + sCurFileName + "=" + updateFileName));	
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
@brief      ɾ��ָ���ļ��������в����Ĵ���Э��ͷ

@param      CString sParamPath  �����ļ���

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
				// ȡ�ļ���
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
@brief      ɾ��ָ�������ļ�

@param      (i)CString sParamPrefix �����ļ���ǰ׺
@param      (i)BYTE bVerType		�����汾����

@retval     ��

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

	// ��ò�������
	PARAM_TYPE paraType = GetParamType(sParamPrefix);			

	// ��ò����ļ�ǰ׺
	sPrefix = sParamPrefix;
	// ��ǰ�汾����
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
		// 		case  UNKNOW: //δ֪
		// 			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("δ֪�������ͣ�"+sParamName));	
		// 			return;			
		// 		}
	}
	// δ���汾����
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
		// 		case  UNKNOW: //δ֪
		// 			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("δ֪�������ͣ�"+sParamName));	
		// 			return;		
		// 		}
	}
	else{
		CString sTmp(_T(""));
		sTmp.Format(_T("%X"),bVerType);
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("δ֪�����汾���ͣ�"+sTmp));	
		return;
	}
	// ɾ���������ļ�
	DeleteFile(sParamPathFileName);
	// ɾ�����ݲ����ļ�
	DeleteFile(sParamPathFileNameBackup);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ��ָ��Ŀ¼�������ļ�

@param      ��i��PARAMETER_ID paraID				����ID
@param      ��i��PARAM_VERSION_TYPE_CODE versionType	��������

@retval     ��

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

		// ɾ���ļ��м�������
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
@brief      ���ݵ�ǰ��ʽ�汾������ʽ�汾�����ļ��ӱ��ص�ǰ����Ŀ¼����������Ŀ¼�£�

@param      ��

@retval     ��

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
	case  UNKNOW: //δ֪
		sBackupPath = _T("");			
	}
	CString sBackupPathFile = sBackupPath + sCurFileName;
	CString sCurfilePath = sCurFilePath + sCurFileName;

	// �ƶ���ǰ�汾�����ļ�����Ŀ¼
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
@brief      ��ȡ�汾�л���Ϣ

@param      (i)CString srcParam						�²����ļ�
@param      (i)CString dstParam						�ɲ����ļ�
@param      (o)VERSION_CHANGE_INFO& verChangeInfo	�л���Ϣ

@retval     ��

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
	// ��ȡ�ļ���Ϣ��binInfo��
	if (srcFile->ReadBinFile(srcbinInfo) != CXFile::FILE_OPERATION_OK){
		throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
	}
	LPBYTE lpData = srcbinInfo.lpData;
	// ������Ϣͷ
	lpData += 6;
	// ����ID
	iParamOldId = ComMakeWORD(*lpData, *(lpData+1))/*BCD2int((char*)lpData,2)*/;
	verChgTmp.paramId  = iParamOldId;
	lpData += 2; 
	// �°汾��
	verChgTmp.newVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
	lpData +=11;  
	// �°汾��Ч����
	verChgTmp.newStartDate =  BCD2Date((char*)lpData);
	lpData+=4;

	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T("Old Version:") + dstParamFile);
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T("New Version:") + srcParamFile);

	if(PathIsDirectory(dstParamFile) || ComCheckFileExist(dstParamFile) == FALSE){// ������ļ��еĻ��������ļ������ڵĻ�
		verChgTmp.oldVersion = 0xFFFFFFFF;
	}
	else{
		auto_ptr<CXFile> dstFile(new CXFile(dstParamFile));
		VARIABLE_DATA dstbinInfo;
		// ��ȡ�ļ���Ϣ��binInfo��
		if (dstFile->ReadBinFile(dstbinInfo) != CXFile::FILE_OPERATION_OK){
			throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
		}
		lpData = dstbinInfo.lpData;
		// ������Ϣͷ
		lpData += 6;
		// ����ID
		iParamNewId = BCD2int((char*)lpData,2);
		lpData += 2; 
		// �ɰ汾��
		verChgTmp.oldVersion = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData +=11;  
		// ����Ч����
		verChgTmp.oldStartDate =  BCD2Date((char*)lpData);
		lpData+=4;
	}
	// �汾����
	verChgTmp.versionType = GetParamVersionType(srcParamFile);
	verChangeInfo = verChgTmp;
	theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("GetVersionChangeInfo"), __LINE__, _T(">>"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ԭ��ǰ��ʽ�汾������ʽ�汾�����ļ��ӱ���Ŀ¼���������ص�ǰ����Ŀ¼��

@param      (i)PARAM_TYPE paraType			��������
@param      (i)VEC_PARAM_ID vecParamId		��ԭ�Ĳ���ID����
@param      (i)bool allFlag					�Ƿ�ԭ���в���

@retval     ��

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
		case  UNKNOW: //δ֪
			sBackupFilePath = _T("");			
		}
		// ��ԭ���в���
		if(allFlag){						
			BOOL bContinue = finder.FindFile(sBackupFilePath + _T("*.*"));

			while (bContinue) {
				bContinue = finder.FindNextFile();

				if (finder.IsDots()||finder.IsDirectory()) {
					continue;
				}
				else {   
					// ȡ�ļ���
					sBackUpFileName = finder.GetFileName();	
					sPrefix = sBackUpFileName.Left(9);
					sCurFileName = GetCurParamName(sPrefix,paraType,sCurFilePath);

					// �汾��һ��������ļ�
					if(sCurFileName != sBackUpFileName){ 					
						// �ƶ�����Ŀ¼�����ļ�����ǰ����Ŀ¼
						if (MoveFileEx(sBackupFilePath + sBackUpFileName,sCurFilePath + sBackUpFileName,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING) != TRUE) { 
							throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _tl(TXT_MOVE_FILE_FAILURE) + sBackupFilePath + sBackUpFileName + _T("=>") + sCurFilePath + sBackUpFileName);	
						}					
						else{
							// ɾ����ǰ�汾�����ļ�						
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
		// ��ԭָ������
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
@brief      ȡ������Ŀ¼��ָ����Ŀ¼��ָ�������ļ���

@param      ��i��PARAMETER_ID paraID				����ID
@param      ��i��PARAM_VERSION_TYPE_CODE versionType	��������

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetTheParamPathName(WORD paraID,CString ParamPath)
{
	return GetTheParamPathNameImp(paraID,ParamPath,theTVM_INFO.GetDataPath());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ñ���Ŀ¼ָ����Ŀ¼��ָ�������ļ���

@param      ��i��PARAMETER_ID paraID				����ID
@param      ��i��PARAM_VERSION_TYPE_CODE versionType	��������

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetTheParamBackUpPathName(WORD paraID,CString ParamPath)
{
	return GetTheParamPathNameImp(paraID,ParamPath,theTVM_INFO.GetBackupPath());
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ָ��Ŀ¼��ָ�������ļ���

@param      ��i��PARAMETER_ID paraID				����ID
@param      ��i��PARAM_VERSION_TYPE_CODE versionType	��������
@param      ��i��basePath	����ַ
@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetTheParamPathNameImp(WORD paraID,CString ParamPath,CString basePath)
{
	CString retName = _T("");
	CFileFind finder;
	try{
		// ���Ҳ����ļ�

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
				// ȡ���ļ��������ڱȽ�����
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
@brief      ȡ�õ�ǰĿ¼��ָ�������ļ���

@param      ��i��CString sPrefix  �����ļ�ǰ׺
@param      ��i��PARAM_TYPE paraType ��������
@param      ��o��CString sFilePath ����·��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetCurParamName(CString sPrefix,PARAM_TYPE paraType,CString& sFilePath,bool isBackup)
{
	// ���Ҳ����ļ�
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
	case  UNKNOW: //δ֪
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
			// ȡ���ļ���
			retName = finder.GetFileName(); 						
		}
	}
	finder.Close();
	return retName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�õ�ǰĿ¼��ָ�������ļ���

@param      ��i��CString sPrefix  �����ļ�ǰ׺

@retval     CString ����·�����ļ���

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetFutParamName(CString sPrefix,PARAM_TYPE paraType,CString& sFilePath,bool isBackup)
{
	// ���Ҳ����ļ�
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
	case  UNKNOW: //δ֪
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
			// ȡ���ļ���
			retName = finder.GetFileName(); 
		}
	}
	finder.Close();
	return retName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��������

@param      ��i��CString fileName  �����ļ���

@retval     ��

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
		ACC_NOT_SERVICE_STATION_ID			,					// ACCͣ�˳�վ��
		ACC_SINGLE_BLACK_LIST_ID			,					// ACC��������������
		ACC_SECTION_BLACK_LIST_ID			,					// ACC���κ���������
		ACC_WRITE_LIST_ID					,					// ACC����������
		ACC_OTHER_CITY_CARD_ID				,					// ACC��ؿ�/���д�����ղ���
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
@brief      ��ȡ�����ļ��汾����

@param      ��i��CString fileName  �����ļ���

@retval     PARAM_VERSION_TYPE_CODE �汾����

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
@brief      ��ȡ�����ļ��汾����

@param      ��i��CString fileName  �����ļ���

@retval     PARAM_VERSION_TYPE_CODE �汾����

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
@brief      ��ȡ�����汾

@param      (i)CString sPathAndName     �����ļ���

@retval     CString              �����ļ���

@exception  ��
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
@brief      ��ȡ����汾��

@param      (i)CString sPathAndName     �����ļ���

@retval     CString              ����汾��

@exception  ��
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
@brief      ��ȡ�������κ�

@param      (i)CString sPathAndName     �����ļ���

@retval     CString       ���κ�

@exception  ��
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
//@brief      ����version��Ϣ���õ�ǰCD�汾�����ص�ǰ�汾
//
//@param      (o)CD_VERSION &cdVersion  ��ǰ�汾
//@param      (o)WORD wVersionNo        CD�汾��
//
//@retval     ��
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
//		theACC_VERSION.GetCDInfoParam(cd1Info);						// ���CD1��Ϣ����
//		// �жϰ汾1����Ϣ���Ƿ�Ϊ0xFFFFFFFF
//		if (CD_NO_VERSION != cd1Info.cdSetVersion){
//			dateOne = cd1Info.cdActivationDate;           
//			// �ж�����1�Ƿ���Ч��С�ڵ�ǰʱ��
//			if ((0 == ComCheckDateValid(dateOne)) && (1 != CmpTwoDate(dateOne ,ComGetBusiDate()))) {
//				isCD1Update = true;
//			}
//		}
//
//		theACC_VERSION.Internalize(false);
//		CAccVersionInfoParam::CD_INFO_PARAM cd2Info;
//		theACC_VERSION.GetCDInfoParam(cd2Info);						// ���CD2��Ϣ����		
//		// �жϰ汾2����Ϣ���Ƿ�Ϊ0xFFFFFFFF
//		if (CD_NO_VERSION != cd2Info.cdSetVersion){
//			dateTwo = cd2Info.cdActivationDate; 
//			// �ж�����2�Ƿ���Ч��С�ڵ�ǰʱ��
//			if ((0 == ComCheckDateValid(dateTwo)) && (1 != CmpTwoDate(dateTwo ,ComGetBusiDate()))) {
//				isCD2Update = true;
//			}
//		}
//		// ����汾1�Ͱ汾2�汾����0xFFFFFFFF���汾1��Ч
//		if (CD_NO_VERSION == cd1Info.cdSetVersion && CD_NO_VERSION == cd2Info.cdSetVersion){			
//			isCD1Update = true;
//		}
//
//		// ��Ч�汾��Ϣ
//		if (isCD1Update && !isCD2Update) {
//			// �汾1��Ч
//			cdVersion = VERSION_FIRST;
//		}
//		else if (!isCD1Update && isCD2Update) {
//			// �汾2��Ч
//			cdVersion = VERSION_SECOND;
//		}
//		else if (!isCD1Update && !isCD2Update) {
//			// CD�����������쳣�������ж�
//			throw CCPSException(CCPSException::REFRESH_VERSION_ERROR, _T(__FILE__), __LINE__, "CD version ��������");
//		}
//		else {
//			// �汾1�Ͱ汾2ͬʱ��Ч
//			if ( 1 == CmpTwoDate(dateOne ,dateTwo)) {
//				// �汾1��Ч
//				cdVersion = VERSION_FIRST;
//			}
//			else if (-1 == CmpTwoDate(dateOne ,dateTwo)) {
//				// �汾2��Ч
//				cdVersion = VERSION_SECOND;
//			}
//			else {
//				// �汾1�Ͱ汾2����Ч������ͬ����ЧCD�汾���
//				DWORD ver1 = cd1Info.cdSetVersion;
//				DWORD ver2 = cd2Info.cdSetVersion;;
//				if(ver1 > ver2) {
//					cdVersion = VERSION_FIRST;
//				}
//				else if(ver1 < ver2) {
//					cdVersion = VERSION_SECOND;
//				}
//				else {
//					// CD�汾��ͬ����ЧgenerationId���
//					DWORD ger1 = cd1Info.generationId;
//					DWORD ger2 = cd2Info.generationId;
//					if (ger1 >= ger2) {
//						// �汾1���ڻ��ߵ��ڰ汾2������Ч�汾1
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
@brief      ��ȡACC�汾�л���Ϣ

@param      (i)VEC_VERSION_INFO& vecChangeInfo  �汾��Ϣ

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CParamHelper::GetVesionChangeInfo(VEC_VERSION_INFO& vecChangeInfo)
{
	VERSION_CHANGE_INFO verInfo;
	vecChangeInfo.clear();

	//??
	// ϵͳ����
	//verInfo.paramId = ACC_SYSTEM_ID;
	//verInfo.versionType = theACC_SYSTEM.GetCurVerType();
	//verInfo.oldVersion = theACC_SYSTEM.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SYSTEM.GetCurStartDate();
	//verInfo.newVersion = theACC_SYSTEM.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SYSTEM.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// ҵ��������
	//verInfo.paramId = ACC_BUSINESS_ID;
	//verInfo.versionType = theACC_BUSINESS.GetCurVerType();
	//verInfo.oldVersion = theACC_BUSINESS.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_BUSINESS.GetCurStartDate();
	//verInfo.newVersion = theACC_BUSINESS.GetFutMasterVer();
	//verInfo.newStartDate = theACC_BUSINESS.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// �����汾����
	//verInfo.paramId = ACC_VERSION_ID;
	//verInfo.versionType = theACC_VERSION.GetCurVerType();
	//verInfo.oldVersion = theACC_VERSION.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_VERSION.GetCurStartDate();
	//verInfo.newVersion = theACC_VERSION.GetFutMasterVer();
	//verInfo.newStartDate = theACC_VERSION.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// ����������
	//verInfo.paramId = ACC_BLACK_LIST_ID;
	//verInfo.versionType = theACC_BLACKLIST.GetCurVerType();
	//verInfo.oldVersion = theACC_BLACKLIST.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_BLACKLIST.GetCurStartDate();
	//verInfo.newVersion = theACC_BLACKLIST.GetFutMasterVer();
	//verInfo.newStartDate = theACC_BLACKLIST.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	// ��Ʒ����
	//verInfo.paramId = ACC_PRODUCT_ID;
	//verInfo.versionType = theACC_PRODUCT.GetCurVerType();
	//verInfo.oldVersion = theACC_PRODUCT.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_PRODUCT.GetCurStartDate();
	//verInfo.newVersion = theACC_PRODUCT.GetFutMasterVer();
	//verInfo.newStartDate = theACC_PRODUCT.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// λ�ã���վ�����Σ�����
	//verInfo.paramId = ACC_LOCATION_ID;
	//verInfo.versionType = theACC_LOCATION.GetCurVerType();
	//verInfo.oldVersion = theACC_LOCATION.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_LOCATION.GetCurStartDate();
	//verInfo.newVersion = theACC_LOCATION.GetFutMasterVer();
	//verInfo.newStartDate = theACC_LOCATION.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	// ��������
	//verInfo.paramId = ACC_CALANDER_ID;
	//verInfo.versionType = theACC_CALANDER.GetCurVerType();
	//verInfo.oldVersion = theACC_CALANDER.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_CALANDER.GetCurStartDate();
	//verInfo.newVersion = theACC_CALANDER.GetFutMasterVer();
	//verInfo.newStartDate = theACC_CALANDER.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// ���Ѳ���
	//verInfo.paramId = ACC_SHOP_ID;
	//verInfo.versionType = theACC_SHOP.GetCurVerType();
	//verInfo.oldVersion = theACC_SHOP.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SHOP.GetCurStartDate();
	//verInfo.newVersion = theACC_SHOP.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SHOP.GetFutStartDate();
	//vecChangeInfo.push_back(verInfo);

	//// ���۲���
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
@brief      �ж��Ƿ���Ҫ�л��������汾

@param      CString sParamPreFix �����ļ���ǰ׺

@retval     bool  

@exception  ��
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
		
		// �豸����������
		if(sParamPreFix == AFC_PARAM_ID_1005){
			if(theAFC_COMMON.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_COMMON.GetCurMasterVer();
				futVersion = theAFC_COMMON.GetFutMasterVer();
				futStartDate = theAFC_COMMON.GetFutStartDate();
				curStartDate = theAFC_COMMON.GetCurStartDate();
			}
		}
		// ����Ա������
		else if(sParamPreFix == AFC_PARAM_ID_1006){
			if(theAFC_STAFF.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_STAFF.GetCurMasterVer();
				futVersion = theAFC_STAFF.GetFutMasterVer();
				futStartDate = theAFC_STAFF.GetFutStartDate();
				curStartDate = theAFC_STAFF.GetCurStartDate();
			}
		}
		// �豸Ȩ�޲�����
		else if(sParamPreFix == AFC_PARAM_ID_1007){
			if(theAFC_ACCESS_LEVEL.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_ACCESS_LEVEL.GetCurMasterVer();
				futVersion = theAFC_ACCESS_LEVEL.GetFutMasterVer();
				futStartDate = theAFC_ACCESS_LEVEL.GetFutStartDate();
				curStartDate = theAFC_ACCESS_LEVEL.GetCurStartDate();
			}
		}
		// TVM��Ӫ������
		else if(sParamPreFix == AFC_PARAM_ID_1011){
			if(theAFC_TVM_OPERATION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_TVM_OPERATION.GetCurMasterVer();
				futVersion = theAFC_TVM_OPERATION.GetFutMasterVer();
				futStartDate = theAFC_TVM_OPERATION.GetFutStartDate();
				curStartDate = theAFC_TVM_OPERATION.GetCurStartDate();
			}
		}
		// �豸����ʱ�����
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
		// �жϵ�ǰ�汾��Ч�����Ƿ���Ч��С�ڵ�ǰʱ��
		if(curVersion != UNAVALIBLE_VERSION){
			if ((0 == ComCheckDateValid(curStartDate)) && (1 != CmpTwoDate(curStartDate ,ComGetBusiDate()))) {
				isCurVerValidate = true;
			}
		}
		// �жϽ����汾��Ч�����Ƿ���Ч��С�ڵ�ǰʱ��
		if ((0 == ComCheckDateValid(futStartDate)) && (1 != CmpTwoDate(futStartDate ,ComGetBusiDate()))) {
			isFutVerValidate = true;
		}
		// ��ǰ�汾��Ч
		if (isCurVerValidate && !isFutVerValidate) {
			theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
			return false;
		}
		// �����汾��Ч
		else if (!isCurVerValidate && isFutVerValidate) {	
			theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("CheckIsNeedChangeParam"), __LINE__, _T(">>"));
			return true;
		}
		// ��ǰ�汾�ͽ����汾��δ��Ч
		else if (!isCurVerValidate && !isFutVerValidate) {
			// �����������쳣�������ж�
			//throw CParamException(CParamException::REFRESH_VERSION_ERROR, _T(__FILE__), __LINE__, "������Ч�����쳣:"+sParamPreFix);
		}
		// ��ǰ�汾�ͽ����汾ͬʱ��Ч
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
				// ��ǰ�汾�ͽ����汾��Ч������ͬ���汾�Ŵ����Ч
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
@brief      �ѽ����汾�ļ����޸�Ϊ��ǰ�汾

@param      CString sParamPreFix �����ļ���ǰ׺

@retval     CString  ��ǰ�����ļ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::ConvertFutFileNameToCurrentVersion(CString sFutFileName)
{
	CString ret = _T("");
	if(sFutFileName == _T("")){
		return _T("");
	}
	int verTypeTmp = _ttoi(sFutFileName.Right(2));

	// ��ʽ����
	if(verTypeTmp == 1){
		ret = sFutFileName.Left(sFutFileName.GetLength()-2) + _T("00");
	}
	// ���Խ���
	else if(verTypeTmp == 11){
		ret = sFutFileName.Left(sFutFileName.GetLength()-2) + _T("10");
	}	
	// ����
	else{
		ret = _T("");
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȫ·������ļ���

@param      (i)CString sPathAndName     Ŀ¼+�ļ���

@retval     CString              �����ļ���

@exception  ��
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
@brief      ��ȡ��ǰ�汾����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetCurXorSumVersion(){
	return m_strCurVersion;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ�����汾����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CParamHelper::GetFutXorSumVersion(){
	return m_strFurVersion;
}