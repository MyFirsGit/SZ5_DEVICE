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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfo::CVersionInfo()
{
    try {
        m_versionIni = NULL;
        // ���ڻ�
		TCHAR szAppPath[_MAX_PATH];

		ComGetAppPath(szAppPath, _MAX_PATH);
		

		CString fileName = CString(szAppPath) + VERSION_INI_FILE_NAME;
        Initialize(fileName);
    }
    catch (...) {
        // �����쳣��Ϣ
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CVersionInfo::~CVersionInfo()
{
    delete m_versionIni;
    m_versionIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CVersionInfo::Initialize(CString fileName) 
{
    try {
        // ���ѳ�ʼ������ֱ�ӷ���
        if (m_versionIni != NULL) {
			return 0;
        }

       

        // check VersionInfo.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // �ļ����Ը�Ϊ��д
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
@brief      ��Ŀ¼������ӷ�б��

@param      (i)CString sPath    Ŀ¼

@retval     CString     �������б�ܵ�Ŀ¼��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CVersionInfo::AddBackslash(CString sPath)
{
    return sPath.TrimRight(_T("\\")) + _T("\\");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�汾��

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������

@retval      int �汾��

@exception   ��
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
@brief       ��ȡ���κ�

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������

@retval      int ���κ�

@exception   ��
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
@brief       ��ȡ����ID

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������

@retval      int ����ID

@exception   ��
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
@brief       ��ȡ��Ч����

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������

@retval      _DATE ��Ч����

@exception   ��
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
@brief       ��ȡ�汾����

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������

@retval      CString �汾����

@exception   ��
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
@brief       ���ð汾��

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������
@param       (i)int verNo �汾��

@retval      none

@exception   ��
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
@brief       �������κ�

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������
@param       (i)int batchNo ���κ�

@retval      none

@exception   ��
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
@brief       ���ó���ID

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������
@param       (i)int startDate	   ����ID

@retval      none

@exception   ��
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
@brief       ������Ч����

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������
@param       (i)_DATE startDate    ��Ч����

@retval      none

@exception   ��
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
@brief       ���ð汾����

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������
@param       (i)CString verType ���ð汾����

@retval      none

@exception   ��
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
@brief       �л��汾

@param       (i)PROG_TYPE progType ��������
@param       (i)VER_TYPE fromVerType   �л��汾��Դ�汾
@param       (i)VER_TYPE toVerType   �л��汾��Ŀ�İ汾
@param       (i)bool removeFromVer   �Ƿ�ɾ��Դ�汾

@retval      bool �ɹ���ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CVersionInfo::SwitchVersion(__in PROG_TYPE progType,__in VER_TYPE fromVerType,__in VER_TYPE toVerType,__in bool removeFromVer)
{
	if(fromVerType == toVerType)
	{
		return false;
	}

	DWORD fromVersion = GetVersionNo(fromVerType,progType);
	if(fromVersion == 0xFFFFFFFF)//Դ�汾�����ڡ�
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
@brief       ɾ��ָ�������ָ���汾��

@param       (i)VER_TYPE verType   �汾����
@param       (i)PROG_TYPE progType ��������

@retval      none

@exception   ��
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