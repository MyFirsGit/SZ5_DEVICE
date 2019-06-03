#include "StdAfx.h"
#include "VersionInfo.h"
#include "ParameterDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVersionInfo CVersionInfo::theInstance;


CVersionInfo* CVersionInfo::GetInstance(CString fileName)
{
	CVersionInfo* versionInfo = new CVersionInfo();
	versionInfo->Initialize(fileName);
	return versionInfo;
}

void  CVersionInfo::DeleteInstance(CVersionInfo* cVersionInfo)
{
	if(cVersionInfo!=NULL){
		delete cVersionInfo;
		cVersionInfo = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfo::CVersionInfo()
{
    try {
        m_versionIni = NULL;
        // 初期化
		TCHAR szAppPath[_MAX_PATH];

		ComGetAppPath(szAppPath, _MAX_PATH);
		

		CString fileName = CString(szAppPath) + VERSION_INI_FILE_NAME;
        Initialize(fileName);
    }
    catch (...) {
        // 忽略异常信息
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfo::~CVersionInfo()
{
    delete m_versionIni;
    m_versionIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CVersionInfo::Initialize(CString fileName) 
{
    try {
        // 若已初始化，则直接返回
        if (m_versionIni != NULL) {
			return 0;
        }

       

        // check VersionInfo.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // 文件属性改为可写
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_versionIni = new CGetIni(fileName);

		return 0;
    }
    catch(CSysException&) {
        throw;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      在目录名后添加反斜杠

@param      (i)CString sPath    目录

@retval     CString     右面带反斜杠的目录名

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CVersionInfo::AddBackslash(CString sPath)
{
    return sPath.TrimRight(_T("\\")) + _T("\\");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取版本号

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型

@retval      int 版本号

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CVersionInfo::GetVersionNo(VER_TYPE verType,PROG_TYPE progType)
{
	DWORD ret = 0;
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_VERSION_NO");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_VERSION_NO");
	}
	else{
	}
	
	return m_versionIni->GetDataInt(GetSectionName(progType),sKey,0xFFFFFFFF);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取批次号

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型

@retval      int 批次号

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CVersionInfo::GetBatchNo(VER_TYPE verType,PROG_TYPE progType)
{
	int ret = 0;
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_BATCH_NO");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_BATCH_NO");
	}
	else{
	}
	return m_versionIni->GetDataInt(GetSectionName(progType), sKey, 0);
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取厂商ID

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型

@retval      int 厂商ID

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CVersionInfo::GetCompanyID(VER_TYPE verType,PROG_TYPE progType)
{
	int ret = 0;
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_COMPANY_ID");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_COMPANY_ID");
	}
	else{
	}
	return m_versionIni->GetDataInt(GetSectionName(progType), sKey, 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取生效日期

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型

@retval      _DATE 生效日期

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
_DATE CVersionInfo::GetStartDate(VER_TYPE verType,PROG_TYPE progType)
{
	_DATE retDate;
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_START_DATE");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_START_DATE");
	}
	else{
	}
	CString ret =  m_versionIni->GetDataString(GetSectionName(progType), sKey, _T("00000000"));
	retDate.wYear = _ttoi(ret.Left(4));
	retDate.biMonth = _ttoi(ret.Mid(4,2));
	retDate.biDay = _ttoi(ret.Mid(6,2));
	return retDate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取版本类型

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型

@retval      CString 版本类型

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CVersionInfo::GetVersionType(VER_TYPE verType,PROG_TYPE progType)
{
	int ret = 0;
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_VERSION_TYPE");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_VERSION_TYPE");
	}
	else{
	}
	return m_versionIni->GetDataInt(GetSectionName(progType), sKey, verType == CURRENT? 0x000:0x01);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置版本号

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型
@param       (i)int verNo 版本号

@retval      none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfo::SetVersionNo(VER_TYPE verType,PROG_TYPE progType,DWORD verNo)
{
	CString sSetVal(_T(""));
	sSetVal.Format(_T("%d"), verNo);
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_VERSION_NO");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_VERSION_NO";)
	}
	else{
		return;
	}
	m_versionIni->WriteData(GetSectionName(progType),sKey,sSetVal);
	VersionNoChanged.Invoke(verType,progType,verNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置批次号

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型
@param       (i)int batchNo 批次号

@retval      none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfo::SetBatchNo(VER_TYPE verType,PROG_TYPE progType,int batchNo)
{
	CString sSetVal(_T(""));
	sSetVal.Format(_T("%d"), batchNo);
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_BATCH_NO");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_BATCH_NO");
	}
	else{
		return;
	}
	m_versionIni->WriteData(GetSectionName(progType),sKey,sSetVal);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置厂商ID

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型
@param       (i)int startDate	   厂商ID

@retval      none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfo::SetCompanyID(VER_TYPE verType,PROG_TYPE progType,int iCompanyID)
{
	CString sSetVal(_T(""));
	sSetVal.Format(_T("%d"),iCompanyID);
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_COMPANY_ID");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_COMPANY_ID");
	}
	else{
		return;
	}
	m_versionIni->WriteData(GetSectionName(progType),sKey,sSetVal);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置生效日期

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型
@param       (i)_DATE startDate    生效日期

@retval      none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfo::SetStartDate(VER_TYPE verType,PROG_TYPE progType,_DATE startDate) 
{
	CString sKey(_T(""));
	CString sSetVal(_T(""));
	sSetVal.Format(_T("%.4d%.2d%.2d"), startDate.wYear,startDate.biMonth,startDate.biDay);
	if(verType == CURRENT){
		sKey = _T("CURRENT_START_DATE");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_START_DATE");
	}
	else{
		return;
	}
	m_versionIni->WriteData(GetSectionName(progType),sKey,sSetVal);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置版本类型

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型
@param       (i)CString verType 设置版本类型

@retval      none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CVersionInfo::SetVersionType(VER_TYPE verType,PROG_TYPE progType,int iVerType)
{
	CString sSetVal(_T(""));
	sSetVal.Format(_T("%d"),iVerType);
	CString sKey(_T(""));
	if(verType == CURRENT){
		sKey = _T("CURRENT_VERSION_TYPE");
	}
	else if(verType == FUTURE){
		sKey = _T("FUTURE_VERSION_TYPE");
	}
	else{
		return;
	}
	m_versionIni->WriteData(GetSectionName(progType),sKey,sSetVal);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       切换版本

@param       (i)PROG_TYPE progType 程序类型
@param       (i)VER_TYPE fromVerType   切换版本的源版本
@param       (i)VER_TYPE toVerType   切换版本的目的版本
@param       (i)bool removeFromVer   是否删除源版本

@retval      bool 成功或失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CVersionInfo::SwitchVersion(__in PROG_TYPE progType,__in VER_TYPE fromVerType,__in VER_TYPE toVerType,__in bool removeFromVer)
{
	if(fromVerType == toVerType)
	{
		return false;
	}

	DWORD fromVersion = GetVersionNo(fromVerType,progType);
	if(fromVersion == 0xFFFFFFFF)//源版本不存在。
	{
		return false;
	}
	SetVersionNo(toVerType,progType,GetVersionNo(fromVerType,progType));
	SetBatchNo(toVerType,progType,GetBatchNo(fromVerType,progType));
	SetCompanyID(toVerType,progType,GetCompanyID(fromVerType,progType));
	SetStartDate(toVerType,progType,GetStartDate(fromVerType,progType));
	int verType = GetVersionType(fromVerType,progType);
	switch(verType)
	{
		case CUR_TEST:
			verType = FUT_TEST;
			break;
		case CUR_VER:
			verType = FUT_VER;
			break;
		case FUT_TEST:
			verType = CUR_TEST;
			break;
		case FUT_VER:
			verType = CUR_VER;
			break;
		default:
			verType = CUR_VER;
	}
	SetVersionType(toVerType,progType,verType);
	if(removeFromVer)
	{
		RemoveVersion(progType,fromVerType);
	}
	///*theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);*/
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       删除指定程序的指定版本。

@param       (i)VER_TYPE verType   版本类型
@param       (i)PROG_TYPE progType 程序类型

@retval      none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CVersionInfo::RemoveVersion(__in PROG_TYPE progType,__in VER_TYPE verType)
{
	SetVersionNo(verType,progType,0xFFFFFFFF);
	SetBatchNo(verType,progType,0);
	SetCompanyID(verType,progType,0);
	SetStartDate(verType,progType,_DATE());
	SetVersionType(verType,progType,verType == CURRENT ? 0x00:0x01);
	return true;
}


CString CVersionInfo::GetSectionName(__in PROG_TYPE progTye)
{
	CString key;
	switch(progTye){
		case MC:
			key = _T("MC");
			break;
 		case TH_MAIN:
 			key = _T("TH_MAIN");
 			break;
 		case TH_APP:
 			key= _T("TH_APP");
 			break;
		case RW_MAIN:
			key = _T("RW_MAIN");
			break;
		case RW_APP:
			key = _T("RW_APP");
			break;
 		case TW_MAIN:
 			key = _T("TW_MAIN");
 			break;
 		case TW_APP:
 			key = _T("TW_APP");
 			break;
		default:
			break;
	}
	return key;
}