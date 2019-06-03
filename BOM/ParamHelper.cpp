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
	
	//bool currentVersion = true;
// 	CD_VERSION cdVersion = VERSION_FIRST;
// 	WORD wVersionNo = CD_NO_VERSION;
// 	GetCurrentCDVesion(cdVersion,wVersionNo);
// 	SetCurrentCDVesion(cdVersion);

// 	ȡ�õ�ǰCD�汾��
// 	int curVersionNo = theBOM_INFO.GetCurrentCDVersion();
// 	if(curVersionNo != 0 && (curVersionNo != wVersionNo)){
// 		// ���͵���
// 		theSERVICE_MGR.GetService(SC_SVC)->InsertTXN_EVENT_CD_VERSION_STATUS();
// 	}
// 	theBOM_INFO.SetCurrentCDVersion(wVersionNo);

	//CString dataPath = theBOM_INFO.GetDataPath() + PARAM_ACC_CURRENT_DIR;
	//CString dataPathBackup = theBOM_INFO.GetBackupPath() +PARAM_ACC_BACKUP_DIR;

	//
	//RemoveParamTransHead(dataPath);
	//RemoveParamTransHead(dataPathBackup);


	// ��ʼ��Acc����
	if(!InitAccParam(TRUE)){
		ret = false;
	}
	// ��ʼ��Ect����
	//if(!InitEctParam()){
	//	ret = false;
	//}
	// ��ʼ��Afc����
	if(!InitAfcParam(TRUE)){
		ret = false;
	}
	return ret;
}

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
		CString strMainPath = theBOM_INFO.GetDataPath();
		CString strBackupPath = theBOM_INFO.GetBackupPath();

		// ��Ӫ����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0101);
		if(!theACC_BUSINESS.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// �������������Ѳ���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0102);
		if(!theACC_SERVICEFEE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}	

		// ��վλ�ò���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0301);
		if(!theACC_LINE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// Ʊ�����Բ���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0401);
		if(!theACC_TICKET.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACCͣ�˳�վ��
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0608);
		if(!theACC_NOTSERVSTATION.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ACC�豸���Ʋ���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0201);
		if(!theACC_DEVICECONTROL.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC�Ʒ�վ�����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0302);
		if(!theACC_TOLLSTATION.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC���β���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0303);
		if(!theACC_SECTION.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ACC��������������
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0701);
		if(!theACC_SINGLEBLACKLIST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ACCƱ�������̲���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0501);
		if(!theACC_ISSUECOMPANY.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}


		// ���κ���������
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0702);
		if(!theACC_SECTIONBLACKLIST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ����Ʊ�۱����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0601);
		if(!theACC_BASEPRICE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// Ʊ�ۼ�������
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0602);
		if(!theACC_PRICELEVEL.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// Ʊ�۵������Ա����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0603);
		if(!theACC_PRICEADJUEST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ʱ�����ͱ����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0605);
		if(!theACC_TIMETYPE.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ʱ��������ϸ����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0606);
		if(!theACC_TIMEDETAIL.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ʱ�両�������
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0604);
		if(!theACC_TIMEFLOAT.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// �ۿ��ʱ����
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0607);
		if(!theACC_DISCOUNT.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ����������
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0801);
		if(!theACC_WRITELIST.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}
		// ��ؿ�/���д�����ղ���
		ChangeFutVerToCurVer(ACC_VER,ACC_PARAM_ID_0802);
		if(!theACC_OTHERCITYCARD.Internalize(flag,strMainPath,strBackupPath)){
			ret =  false;
		}

		// ��ʱ�������ò���
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
		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������InitAccParam���쳣")));
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
bool CParamHelper::InitEctParam()
{
	bool ret = true;
	try{
		
		// һ��ͨ������������
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
		//theEXCEPTION_MGR.ProcessException(CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������InitEctParam���쳣")));
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��Afc����

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CParamHelper::InitAfcParam(bool flag)
{
	bool ret = true;
	try{
		CString strMainPath = theBOM_INFO.GetDataPath();
		CString strBackupPath = theBOM_INFO.GetBackupPath();

		// �豸����������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1005);
		if(!theAFC_COMMON.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�豸�����������ʼ��ʧ��"));			
			ret =  false;
		}
		// ����Ա������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1006);
		if(!theAFC_STAFF.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("����Ա�������ʼ��ʧ��"));			
			ret =  false;
		}
		// �豸Ȩ�޲�����
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1007);
		if(!theAFC_ACCESS_LEVEL.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�豸Ȩ�޲������ʼ��ʧ��"));			
			ret =  false;
		}
		// BOM��Ӫ������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1021);
		if(!theAFC_BOM_OPERATION.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("BOM��Ӫ�������ʼ��ʧ��"));			
			ret =  false;
		}	
		// �豸����ʱ�����
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1089);
		if(!theAFC_DEV_TIME.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("�豸����ʱ�������ʼ��ʧ��"));			
			ret =  false;
		}	
		// ģʽ��������
		ChangeFutVerToCurVer(AFC_VER,AFC_PARAM_ID_1002);
		if(!theAFC_MODERECORD.Internalize(flag,strMainPath,strBackupPath)){
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ģʽ����������ʼ��ʧ��"));			
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
	// ��Ӫ����
	curAccVersion.lBusinessParam = theACC_BUSINESS.GetCurMasterVer();
	// �������������Ѳ���
	curAccVersion.lServerFeeParam = theACC_SERVICEFEE.GetCurMasterVer();
	// ��վλ�ò���
	curAccVersion.lStationParam = theACC_LINE.GetCurMasterVer();
	// Ʊ�����Բ���
	curAccVersion.lCardAttributeParam = theACC_TICKET.GetCurMasterVer();
	// ACCͣ�˳�վ��
	curAccVersion.lNotServStationParam = theACC_NOTSERVSTATION.GetCurMasterVer();
	// ACC�豸���Ʋ���
	curAccVersion.lDeviceControlParam = theACC_DEVICECONTROL.GetCurMasterVer();
	// ACC�Ʒ�վ�����
	curAccVersion.lPayStationParam = theACC_TOLLSTATION.GetCurMasterVer();
	// ACC���β���
	curAccVersion.lSectionParam = theACC_SECTION.GetCurMasterVer();
	// ACC��������������
	curAccVersion.lSingleBlackListParam = theACC_SINGLEBLACKLIST.GetCurMasterVer();
	// ���κ���������
	curAccVersion.lSectionBlackListParam = theACC_SECTIONBLACKLIST.GetCurMasterVer();
	// Ʊ�������̲���
	curAccVersion.lissueCompanyParam = theACC_ISSUECOMPANY.GetCurMasterVer();
	// ����Ʊ�۱����
	curAccVersion.lBasePriceParam = theACC_BASEPRICE.GetCurMasterVer();
	// Ʊ�ۼ�������
	curAccVersion.lPrieLevelParam = theACC_PRICELEVEL.GetCurMasterVer();
	// Ʊ�۵������Ա����
	curAccVersion.lPriceAdjuestParam = theACC_PRICEADJUEST.GetCurMasterVer();
	// ʱ�����ͱ����
	curAccVersion.lTimeTypeParam = theACC_TIMETYPE.GetCurMasterVer();
	// ʱ��������ϸ����
	curAccVersion.lTimeDetailParam = theACC_TIMEDETAIL.GetCurMasterVer();
	// ʱ�両�������
	curAccVersion.lTimeFloatParam = theACC_TIMEFLOAT.GetCurMasterVer();
	// �ۿ��ʱ����
	curAccVersion.lDiscountParam = theACC_DISCOUNT.GetCurMasterVer();
	// ����������
	curAccVersion.lWriteListParam = theACC_WRITELIST.GetCurMasterVer();
	// ��ؿ�/���д�����ղ���
	curAccVersion.lOtherCityCardParam = theACC_OTHERCITYCARD.GetCurMasterVer();
	// ��ʱ�������ò���
	curAccVersion.lOverTimeAdjuestParam = theACC_OVERTIMEADJUEST.GetCurMasterVer();
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
	// ��Ӫ����
	futAccVersion.lBusinessParam = theACC_BUSINESS.GetFutMasterVer();
	// �������������Ѳ���
	futAccVersion.lServerFeeParam = theACC_SERVICEFEE.GetFutMasterVer();
	// ��վλ�ò���
	futAccVersion.lStationParam = theACC_LINE.GetFutMasterVer();
	// Ʊ�����Բ���
	futAccVersion.lCardAttributeParam = theACC_TICKET.GetFutMasterVer();
	// ACCͣ�˳�վ��
	futAccVersion.lNotServStationParam = theACC_NOTSERVSTATION.GetFutMasterVer();
	// ACC�豸���Ʋ���
	futAccVersion.lDeviceControlParam = theACC_DEVICECONTROL.GetFutMasterVer();
	// ACC�Ʒ�վ�����
	futAccVersion.lPayStationParam = theACC_TOLLSTATION.GetFutMasterVer();
	// ACC���β���
	futAccVersion.lSectionParam = theACC_SECTION.GetFutMasterVer();
	// ACC��������������
	futAccVersion.lSingleBlackListParam = theACC_SINGLEBLACKLIST.GetFutMasterVer();
	// ���κ���������
	futAccVersion.lSectionBlackListParam = theACC_SECTIONBLACKLIST.GetFutMasterVer();
	// Ʊ�������̲���
	futAccVersion.lissueCompanyParam = theACC_ISSUECOMPANY.GetFutMasterVer();
	// ����Ʊ�۱����
	futAccVersion.lBasePriceParam = theACC_BASEPRICE.GetFutMasterVer();
	// Ʊ�ۼ�������
	futAccVersion.lPrieLevelParam = theACC_PRICELEVEL.GetFutMasterVer();
	// Ʊ�۵������Ա����
	futAccVersion.lPriceAdjuestParam = theACC_PRICEADJUEST.GetFutMasterVer();
	// ʱ�����ͱ����
	futAccVersion.lTimeTypeParam = theACC_TIMETYPE.GetFutMasterVer();
	// ʱ��������ϸ����
	futAccVersion.lTimeDetailParam = theACC_TIMEDETAIL.GetFutMasterVer();
	// ʱ�両�������
	futAccVersion.lTimeFloatParam = theACC_TIMEFLOAT.GetFutMasterVer();
	// �ۿ��ʱ����
	futAccVersion.lDiscountParam = theACC_DISCOUNT.GetFutMasterVer();
	// ����������
	futAccVersion.lWriteListParam = theACC_WRITELIST.GetFutMasterVer();
	// ��ؿ�/���д�����ղ���
	futAccVersion.lOtherCityCardParam = theACC_OTHERCITYCARD.GetFutMasterVer();
	// ��ʱ�������ò���
	futAccVersion.lOverTimeAdjuestParam = theACC_OVERTIMEADJUEST.GetFutMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡECT��ǰ�����汾

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetCurEctParamVersion(VERSION_ECT& curEctVersion)
{
	
	// һ��ͨ������������(��ֵ������)
	curEctVersion.lBlacklistParam = theECT_BLACKLIST.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡECTδ�������汾

@param      ��

@retval     bool   true���ɹ�   false��ʧ��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamHelper::GetFutEctParamVersion(VERSION_ECT& futEctVersion)
{
	futEctVersion.lBlacklistParam = theECT_BLACKLIST.GetFutMasterVer();
}

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
	curAfcVersion.lOperationParam = theAFC_BOM_OPERATION.GetCurMasterVer();
	// �豸����������
	curAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetCurMasterVer();
	// ����Ա������
	curAfcVersion.lStaffPwdParam = theAFC_STAFF.GetCurMasterVer();
	// �豸����ʱ�����
	curAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetCurMasterVer();
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
	futAfcVersion.lOperationParam = theAFC_BOM_OPERATION.GetFutMasterVer();
	// �豸����������
	futAfcVersion.lDeviceCommonParam = theAFC_COMMON.GetFutMasterVer();
	// ����Ա������
	futAfcVersion.lStaffPwdParam = theAFC_STAFF.GetFutMasterVer();
	// �豸����ʱ�����
	futAfcVersion.lDeviceRunTimeParam = theAFC_DEV_TIME.GetFutMasterVer();
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
void CParamHelper::UpdateLocalVersion(VEC_VERSION_INFO& verInfo)
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
				CString sCurFilePathName = sCurFilePath + sCurFileName;
				CString sCurFilePathNameBackup = sCurFilePathBackup + sCurFileNameBackup;
				if(sCurFileName == ""){
					sCurFilePathName = "";
				}
				if(sCurFileNameBackup == ""){
					sCurFilePathNameBackup = "";
				}
				GetVersionChangeInfo(updfilePath,sCurFilePathName,versionChangeInfo);// ��ȡ�汾�л���Ϣ

				// �ƶ�����Ŀ¼�����ļ�����ǰ����Ŀ¼
				if(DeleteFile(sCurFilePathName) != TRUE){
					theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ɾ���ɰ汾�����ļ�ʧ�ܣ�"+sCurFilePathName));	
				}
				if(DeleteFile(sCurFilePathNameBackup) != TRUE){
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
						throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("�ƶ��ļ�ʧ�ܣ�") + updfilePath + _T("=>") + sCurFilePath + updateFileName);
					}	
				}					
				else{
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("MoveFile"), __LINE__,updfilePath + _T("->") + sCurFilePath + updateFileName+_T(" OK"));
					// ɾ�������ļ�
					//if(sCurFilePathName != ""){
					if(DeleteFile(updfilePath) != TRUE){
						throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__,_T("ɾ�������ļ�ʧ�ܣ�") + updfilePath);
					}
					//theDEBUG_LOG->WriteData("CParamHelper", "UpdateLocalVersion", __LINE__, "DeleteFile" + sCurFilePathName);
					//}
					verInfo.push_back(versionChangeInfo);
				}	
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������UpdateLocalVersion���쳣"));
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
					throw CParamException(CParamException::FILE_SAM_ERR, _T(__FILE__), __LINE__,strParamPathName + _T("CRCУ�����"));
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������RemoveParamTransHead���쳣"));
	}
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
			if(sCurFileNameNew!=""){
				DeleteFile(sCurFilePath+sCurFileName);
				if(CopyFile(sFutFilePath+sfutFileName,sCurFilePath+sCurFileNameNew,FALSE)){
					theDEBUG_LOG->WriteData(_T("CParamHelper"), _T("ChangeFutVerToCurVer"), __LINE__, sfutFileName + _T("->") + sCurFileNameNew);
					DeleteFile(sFutFilePath+sfutFileName);
				}
				else{
					throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("ɾ���ļ�ʧ�ܣ�") + sFutFilePath+sfutFileName + _T("=>") + sCurFilePath+sCurFileNameNew);	
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
					throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("�����ļ�ʧ�ܣ�") + sFutFilePath+sfutFileName +_T( "=>") + sCurFilePath+sCurFileNameNew);	
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������ChangeFutVerToCurVer���쳣"));
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
	CString sDataPath = theBOM_INFO.GetDataPath();
	CString sBackupPath = theBOM_INFO.GetBackupPath();

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
		memset(DelFileSource, 0x00, MAX_PATH);
		_tcscat(DelFileSource, sDirPath + _T("*.*"));

		fos.pFrom = DelFileSource;

		// ɾ���ļ��м�������
		SHFileOperation( &fos);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������DeleteAllFileFromTheDir���쳣"));
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
			throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("�����ļ�ʧ�ܣ�") + sCurfilePath + _T("=>") + sBackupPathFile);	
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

	auto_ptr<CXFile> srcFile(new CXFile(srcParamFile,_T("")));
	VARIABLE_DATA srcbinInfo;
	// ��ȡ�ļ���Ϣ��binInfo��
	if (srcFile->ReadBinFile(srcbinInfo) != CXFile::FILE_OPERATION_OK){
		throw CParamException(CParamException::FILE_READ_FAIL, _T(__FILE__), __LINE__);
	}
	LPBYTE lpData = srcbinInfo.lpData;
	// ������Ϣͷ
	lpData += 6;
	// ����ID
	iParamOldId = BCD2int((char*)lpData,2);
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

	if(ComCheckFileExist(dstParamFile) == FALSE){
		verChgTmp.oldVersion = 0xFFFFFFFF;
	}
	else{
		auto_ptr<CXFile> dstFile(new CXFile(dstParamFile, _T("")));
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
							throw CParamException(CParamException::FILE_DEL_FAIL, _T(__FILE__), __LINE__, _T("�ƶ��ļ�ʧ�ܣ�" + sBackupFilePath + sBackUpFileName + _T("=>") + sCurFilePath + sBackUpFileName));	
						}					
						else{
							// ɾ����ǰ�汾�����ļ�						
							if(DeleteFile(sCurFilePath + sCurFileName) != TRUE){
								throw CParamException(CParamException::FILE_DEL_FAIL,_T(__FILE__), __LINE__,_T("ɾ����ǰ�汾�����ļ�ʧ�ܣ�") + sCurFilePath + sCurFileName);
							}
						}
					}
					else{
						throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__, _T("�������ݰ汾�뱾�ذ汾��ͬ��") + sCurFileName + _T("=") + sBackUpFileName);	
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������RestoreCurVersion���쳣"));
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
	return GetTheParamPathNameImp(paraID,ParamPath,theBOM_INFO.GetDataPath());
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
	return GetTheParamPathNameImp(paraID,ParamPath,theBOM_INFO.GetBackupPath());
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������GetTheParamPathNameImp���쳣"));
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
		ACC_BUSINESS_ID						,					// ACC��Ӫ����
		ACC_SERVICEFEE_ID					,					// ACC�������������Ѳ���
		ACC_OVERTIME_UPDATE_ID				,					// ACC��ʱ�������ò���
		ACC_DEVICE_ID						,					// ACC�豸���Ʋ���
		ACC_STATION_ID						,					// ACC��վλ�ò���
		ACC_CHARGE_STATION_ID				,					// ACC�Ʒ�վ�����
		ACC_SECTION_ID						,					// ACC���β���
		ACC_CARDATTRIBUTE_ID				,					// ACCƱ�����Բ���
		ACC_BASE_PRICE_TABLE_ID				,					// ACC����Ʊ�۱����
		ACC_PRICE_LEVEL_TABLE_ID			,					// ACCƱ�ۼ�������
		ACC_PRICE_ADJUEST_TABLE_ID			,					// ACCƱ�۵������Ա����
		ACC_TIME_FLOATING_TABLE_ID			,					// ACCʱ�両�������
		ACC_TIME_TYPE_TABLE_ID				,					// ACCʱ�����ͱ����
		ACC_TTIME_TYPE_DETAIL_ID			,					// ACCʱ��������ϸ����
		ACC_DISSCOUNT_TABLE_ID				,					// ACC�ۿ��ʱ����
		ACC_NOT_SERVICE_STATION_ID			,					// ACCͣ�˳�վ��
		ACC_WRITE_LIST_ID					,					// ACC����������
		ACC_OTHER_CITY_CARD_ID				,					// ACC��ؿ�/���д�����ղ���
		ACC_SINGLE_BLACK_LIST_ID			,					// ACC��������������
		ACC_SECTION_BLACK_LIST_ID			,					// ACC���κ���������
		ACC_ISSUE_COMPANY_ID	    		,					// ACCƱ�������̲���
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
@brief      ��ȡ����汾��

@param      (i)CString sPathAndName     �����ļ���

@retval     CString              ����汾��

@exception  ��
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
@brief      ��ȡ�������κ�

@param      (i)CString sPathAndName     �����ļ���

@retval     CString       ���κ�

@exception  ��
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

	// ϵͳ����
	//verInfo.paramId = ACC_SYSTEMPARAM_ID;
	//verInfo.versionType = theACC_SYSTEM.GetCurVerType();
	//verInfo.oldVersion = theACC_SYSTEM.GetCurMasterVer();
	//verInfo.oldStartDate = theACC_SYSTEM.GetCurStartDate();
	//verInfo.newVersion = theACC_SYSTEM.GetFutMasterVer();
	//verInfo.newStartDate = theACC_SYSTEM.GetFutStartDate();	
	//vecChangeInfo.push_back(verInfo);

	// ��Ӫ����
	verInfo.paramId = ACC_BUSINESS_ID;
	verInfo.versionType = theACC_BUSINESS.GetCurVerType();
	verInfo.oldVersion = theACC_BUSINESS.GetCurMasterVer();
	verInfo.oldStartDate = theACC_BUSINESS.GetCurStartDate();
	verInfo.newVersion = theACC_BUSINESS.GetFutMasterVer();
	verInfo.newStartDate = theACC_BUSINESS.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// �������������Ѳ���
	verInfo.paramId = ACC_SERVICEFEE_ID;
	verInfo.versionType = theACC_SERVICEFEE.GetCurVerType();
	verInfo.oldVersion = theACC_SERVICEFEE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SERVICEFEE.GetCurStartDate();
	verInfo.newVersion = theACC_SERVICEFEE.GetFutMasterVer();
	verInfo.newStartDate = theACC_SERVICEFEE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ��վλ�ò���
	verInfo.paramId = ACC_STATION_ID;
	verInfo.versionType = theACC_LINE.GetCurVerType();
	verInfo.oldVersion = theACC_LINE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_LINE.GetCurStartDate();
	verInfo.newVersion = theACC_LINE.GetFutMasterVer();
	verInfo.newStartDate = theACC_LINE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// Ʊ�����Բ���
	verInfo.paramId = ACC_CARDATTRIBUTE_ID;
	verInfo.versionType = theACC_TICKET.GetCurVerType();
	verInfo.oldVersion = theACC_TICKET.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TICKET.GetCurStartDate();
	verInfo.newVersion = theACC_TICKET.GetFutMasterVer();
	verInfo.newStartDate = theACC_TICKET.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACCͣ�˳�վ��
	verInfo.paramId = ACC_NOT_SERVICE_STATION_ID;
	verInfo.versionType = theACC_NOTSERVSTATION.GetCurVerType();
	verInfo.oldVersion = theACC_NOTSERVSTATION.GetCurMasterVer();
	verInfo.oldStartDate = theACC_NOTSERVSTATION.GetCurStartDate();
	verInfo.newVersion = theACC_NOTSERVSTATION.GetFutMasterVer();
	verInfo.newStartDate = theACC_NOTSERVSTATION.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC�豸���Ʋ���
	verInfo.paramId = ACC_DEVICE_ID;
	verInfo.versionType = theACC_DEVICECONTROL.GetCurVerType();
	verInfo.oldVersion = theACC_DEVICECONTROL.GetCurMasterVer();
	verInfo.oldStartDate = theACC_DEVICECONTROL.GetCurStartDate();
	verInfo.newVersion = theACC_DEVICECONTROL.GetFutMasterVer();
	verInfo.newStartDate = theACC_DEVICECONTROL.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC�Ʒ�վ�����
	verInfo.paramId = ACC_CHARGE_STATION_ID;
	verInfo.versionType = theACC_TOLLSTATION.GetCurVerType();
	verInfo.oldVersion = theACC_TOLLSTATION.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TOLLSTATION.GetCurStartDate();
	verInfo.newVersion = theACC_TOLLSTATION.GetFutMasterVer();
	verInfo.newStartDate = theACC_TOLLSTATION.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC���β���
	verInfo.paramId = ACC_SECTION_ID;
	verInfo.versionType = theACC_SECTION.GetCurVerType();
	verInfo.oldVersion = theACC_SECTION.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SECTION.GetCurStartDate();
	verInfo.newVersion = theACC_SECTION.GetFutMasterVer();
	verInfo.newStartDate = theACC_SECTION.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ACC��������������
	verInfo.paramId = ACC_SINGLE_BLACK_LIST_ID;
	verInfo.versionType = theACC_SINGLEBLACKLIST.GetCurVerType();
	verInfo.oldVersion = theACC_SINGLEBLACKLIST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SINGLEBLACKLIST.GetCurStartDate();
	verInfo.newVersion = theACC_SINGLEBLACKLIST.GetFutMasterVer();
	verInfo.newStartDate = theACC_SINGLEBLACKLIST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ���κ���������
	verInfo.paramId = ACC_SECTION_BLACK_LIST_ID;
	verInfo.versionType = theACC_SECTIONBLACKLIST.GetCurVerType();
	verInfo.oldVersion = theACC_SECTIONBLACKLIST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_SECTIONBLACKLIST.GetCurStartDate();
	verInfo.newVersion = theACC_SECTIONBLACKLIST.GetFutMasterVer();
	verInfo.newStartDate = theACC_SECTIONBLACKLIST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ���κ���������
	verInfo.paramId = ACC_ISSUE_COMPANY_ID;
	verInfo.versionType = theACC_ISSUECOMPANY.GetCurVerType();
	verInfo.oldVersion = theACC_ISSUECOMPANY.GetCurMasterVer();
	verInfo.oldStartDate = theACC_ISSUECOMPANY.GetCurStartDate();
	verInfo.newVersion = theACC_ISSUECOMPANY.GetFutMasterVer();
	verInfo.newStartDate = theACC_ISSUECOMPANY.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ����Ʊ�۱����
	verInfo.paramId = ACC_BASE_PRICE_TABLE_ID;
	verInfo.versionType = theACC_BASEPRICE.GetCurVerType();
	verInfo.oldVersion = theACC_BASEPRICE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_BASEPRICE.GetCurStartDate();
	verInfo.newVersion = theACC_BASEPRICE.GetFutMasterVer();
	verInfo.newStartDate = theACC_BASEPRICE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// Ʊ�ۼ�������
	verInfo.paramId = ACC_PRICE_LEVEL_TABLE_ID;
	verInfo.versionType = theACC_PRICELEVEL.GetCurVerType();
	verInfo.oldVersion = theACC_PRICELEVEL.GetCurMasterVer();
	verInfo.oldStartDate = theACC_PRICELEVEL.GetCurStartDate();
	verInfo.newVersion = theACC_PRICELEVEL.GetFutMasterVer();
	verInfo.newStartDate = theACC_PRICELEVEL.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// Ʊ�۵������Ա����
	verInfo.paramId = ACC_PRICE_ADJUEST_TABLE_ID;
	verInfo.versionType = theACC_PRICEADJUEST.GetCurVerType();
	verInfo.oldVersion = theACC_PRICEADJUEST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_PRICEADJUEST.GetCurStartDate();
	verInfo.newVersion = theACC_PRICEADJUEST.GetFutMasterVer();
	verInfo.newStartDate = theACC_PRICEADJUEST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ʱ�����ͱ����
	verInfo.paramId = ACC_TIME_TYPE_TABLE_ID;
	verInfo.versionType = theACC_TIMETYPE.GetCurVerType();
	verInfo.oldVersion = theACC_TIMETYPE.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TIMETYPE.GetCurStartDate();
	verInfo.newVersion = theACC_TIMETYPE.GetFutMasterVer();
	verInfo.newStartDate = theACC_TIMETYPE.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ʱ��������ϸ����
	verInfo.paramId = ACC_TTIME_TYPE_DETAIL_ID;
	verInfo.versionType = theACC_TIMEDETAIL.GetCurVerType();
	verInfo.oldVersion = theACC_TIMEDETAIL.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TIMEDETAIL.GetCurStartDate();
	verInfo.newVersion = theACC_TIMEDETAIL.GetFutMasterVer();
	verInfo.newStartDate = theACC_TIMEDETAIL.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ʱ�両�������
	verInfo.paramId = ACC_TIME_FLOATING_TABLE_ID;
	verInfo.versionType = theACC_TIMEFLOAT.GetCurVerType();
	verInfo.oldVersion = theACC_TIMEFLOAT.GetCurMasterVer();
	verInfo.oldStartDate = theACC_TIMEFLOAT.GetCurStartDate();
	verInfo.newVersion = theACC_TIMEFLOAT.GetFutMasterVer();
	verInfo.newStartDate = theACC_TIMEFLOAT.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// �ۿ��ʱ����
	verInfo.paramId = ACC_DISSCOUNT_TABLE_ID;
	verInfo.versionType = theACC_DISCOUNT.GetCurVerType();
	verInfo.oldVersion = theACC_DISCOUNT.GetCurMasterVer();
	verInfo.oldStartDate = theACC_DISCOUNT.GetCurStartDate();
	verInfo.newVersion = theACC_DISCOUNT.GetFutMasterVer();
	verInfo.newStartDate = theACC_DISCOUNT.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ����������
	verInfo.paramId = ACC_WRITE_LIST_ID;
	verInfo.versionType = theACC_WRITELIST.GetCurVerType();
	verInfo.oldVersion = theACC_WRITELIST.GetCurMasterVer();
	verInfo.oldStartDate = theACC_WRITELIST.GetCurStartDate();
	verInfo.newVersion = theACC_WRITELIST.GetFutMasterVer();
	verInfo.newStartDate = theACC_WRITELIST.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ��ؿ�/���д�����ղ���
	verInfo.paramId = ACC_OTHER_CITY_CARD_ID;
	verInfo.versionType = theACC_OTHERCITYCARD.GetCurVerType();
	verInfo.oldVersion = theACC_OTHERCITYCARD.GetCurMasterVer();
	verInfo.oldStartDate = theACC_OTHERCITYCARD.GetCurStartDate();
	verInfo.newVersion = theACC_OTHERCITYCARD.GetFutMasterVer();
	verInfo.newStartDate = theACC_OTHERCITYCARD.GetFutStartDate();	
	vecChangeInfo.push_back(verInfo);
	// ��ʱ�������ò���
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
		CString strMainPath = theBOM_INFO.GetDataPath();
		CString strBackupPath = theBOM_INFO.GetBackupPath();

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
		else if(sParamPreFix == AFC_PARAM_ID_1021){
			if(theAFC_BOM_OPERATION.readVersionDataInfo(strMainPath,strBackupPath)){
				curVersion = theAFC_BOM_OPERATION.GetCurMasterVer();
				futVersion = theAFC_BOM_OPERATION.GetFutMasterVer();
				futStartDate = theAFC_BOM_OPERATION.GetFutStartDate();
				curStartDate = theAFC_BOM_OPERATION.GetCurStartDate();
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
			// һ��ͨ������������
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
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__,_T("������CheckIsNeedChangeParam���쳣"));
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
